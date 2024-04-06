#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura del nodo del árbol
typedef struct TopicNode
{
    char *name;
    char *last_message;
    char **subscribers; // Lista de suscriptores
    int num_subscribers;
    struct TopicNode *children;
    struct TopicNode *next_sibling;
} TopicNode;

// Función para crear un nuevo nodo del árbol de tópicos
TopicNode *createTopicNode(const char *name)
{
    TopicNode *newNode = (TopicNode *)malloc(sizeof(TopicNode));
    if (newNode != NULL)
    {
        newNode->name = strdup(name);
        newNode->last_message = NULL;
        newNode->subscribers = NULL;
        newNode->num_subscribers = 0;
        newNode->children = NULL;
        newNode->next_sibling = NULL;
    }
    return newNode;
}

TopicNode *getChildNodeHelper(TopicNode *parent, const char *name)
{
    TopicNode *child = parent->children;
    while (child != NULL)
    {
        if (strcmp(child->name, name) == 0)
            return child;
        child = child->next_sibling;
    }

    // Si el hijo no existe, lo creamos
    TopicNode *newChild = createTopicNode(name);
    if (newChild != NULL)
    {
        newChild->next_sibling = parent->children;
        parent->children = newChild;
    }
    return newChild;
}

// Función para buscar o crear un nodo hijo en el árbol de tópicos
TopicNode *getChildNode(TopicNode *parent, const char *topic)
{
    if (parent == NULL || topic == NULL)
        return NULL;

    // Creamos una copia del tópico para no modificar el original
    char *topicCopy = strdup(topic);
    if (topicCopy == NULL)
        return NULL;

    // Dividimos el tópico en partes
    char *token = strtok(topicCopy, "/");
    TopicNode *currentNode = parent;
    while (token != NULL)
    {
        // Buscamos o creamos el nodo hijo correspondiente
        currentNode = getChildNodeHelper(currentNode, token);
        if (currentNode == NULL)
            break;
        token = strtok(NULL, "/");
    }

    free(topicCopy);
    return currentNode;
}

// Función para agregar un suscriptor a un nodo del árbol
void addSubscriber(TopicNode *node, const char *subscriber)
{
    if (node == NULL || subscriber == NULL)
        return;

    // Verificar si el suscriptor ya está presente en la lista de suscriptores
    for (int i = 0; i < node->num_subscribers; ++i)
    {
        if (strcmp(node->subscribers[i], subscriber) == 0)
            return; // El suscriptor ya está presente, no es necesario agregarlo nuevamente
    }

    // Añadir el suscriptor a la lista
    node->subscribers = realloc(node->subscribers, (node->num_subscribers + 1) * sizeof(char *));
    node->subscribers[node->num_subscribers] = strdup(subscriber);
    node->num_subscribers++;
}

// Función para liberar la memoria utilizada por el árbol de tópicos
void freeTopicTree(TopicNode *root)
{
    if (root == NULL)
        return;

    // Liberar memoria de los suscriptores
    for (int i = 0; i < root->num_subscribers; i++)
    {
        free(root->subscribers[i]);
    }
    free(root->subscribers);

    // Liberar memoria de los hijos
    TopicNode *child = root->children;
    while (child != NULL)
    {
        TopicNode *next = child->next_sibling;
        freeTopicTree(child);
        child = next;
    }

    // Liberar memoria del nodo actual
    free(root->name);
    free(root->last_message);
    free(root);
}

char **getSubscribers(TopicNode *node, int *numSubscribers)
{
    if (node == NULL || numSubscribers == NULL)
        return NULL;

    *numSubscribers = node->num_subscribers;
    return node->subscribers;
}

TopicNode *getChildren(TopicNode *node)
{
    if (node == NULL)
        return NULL;

    return node->children;
}

void printTree(TopicNode *node, int depth)
{
    if (node == NULL)
        return;

    // Imprimir el nombre del nodo con su nivel de profundidad
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("- %s\n", node->name);

    // Recursivamente imprimir los hijos del nodo
    printTree(node->children, depth + 1);

    // Imprimir los hermanos del nodo
    printTree(node->next_sibling, depth);
}

void publishMessage(TopicNode *node, const char *message)
{
    if (node == NULL || message == NULL)
        return;

    // Liberar el mensaje anterior si existe
    if (node->last_message != NULL)
    {
        free(node->last_message);
    }

    // Asignar el nuevo mensaje al nodo
    node->last_message = strdup(message);
}

void subscribeToChildren(TopicNode *node, const char *subscriber)
{
    if (node == NULL || subscriber == NULL)
        return;

    // Suscribir al cliente al nodo actual
    addSubscriber(node, subscriber);

    // Recorrer recursivamente los hijos y suscribir al cliente a cada uno de ellos
    TopicNode *child = node->children;
    while (child != NULL)
    {
        subscribeToChildren(child, subscriber);
        child = child->next_sibling;
    }
}

// Función para suscribir un cliente a todos los hijos de un tópico dado
void subscribeToChildrenBeforeWildcard(TopicNode *root, const char *topic, const char *subscriber)
{
    if (root == NULL || topic == NULL || subscriber == NULL)
        return;

    // Creamos una copia del tópico para no modificar el original
    char *topicCopy = strdup(topic);
    if (topicCopy == NULL)
        return;

    // Dividimos el tópico en partes
    char *token = strtok(topicCopy, "/");
    TopicNode *currentNode = root;
    char *partialTopic = (char *)malloc(strlen(topic) + 1);
    partialTopic[0] = '\0'; // Inicializamos como string vacío

    // Recorremos el tópico para encontrar el nodo correspondiente al último hijo antes del "#"
    while (token != NULL && strcmp(token, "#") != 0)
    {
        strcat(partialTopic, token); // Construimos el tópico parcial
        strcat(partialTopic, "/");   // Agregamos el separador para el siguiente token
        currentNode = getChildNodeHelper(currentNode, token);
        token = strtok(NULL, "/");
    }

    // Verificamos si encontramos un nodo antes del "#"
    if (currentNode != NULL)
    {
        // Suscribir al cliente a todos los hijos del nodo encontrado
        subscribeToChildren(currentNode, subscriber);
    }

    free(topicCopy);
    free(partialTopic);
}

void subscribeToWildcardPlus(TopicNode *node, const char *pattern, const char *subscriber)
{
    if (node == NULL || pattern == NULL || subscriber == NULL)
        return;

    // Creamos una copia del patrón para no modificar el original
    char *patternCopy = strdup(pattern);
    if (patternCopy == NULL)
        return;

    // Dividimos el patrón en partes
    char *token = strtok(patternCopy, "/");
    char *partialPattern = (char *)malloc(strlen(pattern) + 1);
    partialPattern[0] = '\0'; // Inicializamos como string vacío

    // Recorremos el patrón para encontrar los nodos que coinciden
    while (token != NULL)
    {
        strcat(partialPattern, token); // Construimos el patrón parcial
        strcat(partialPattern, "/");   // Agregamos el separador para el siguiente token

        // Si encontramos el wildcard '+', continuamos la búsqueda recursiva en los hijos
        if (strcmp(token, "+") == 0)
        {
            TopicNode *child = node->children;
            while (child != NULL)
            {
                subscribeToWildcardPlus(child, pattern + strlen(partialPattern), subscriber);
                child = child->next_sibling;
            }
            break; // No es necesario continuar con el patrón parcial después del wildcard '+'
        }

        // Si no es el wildcard '+', buscamos el nodo que coincida con el token actual
        node = getChildNodeHelper(node, token);
        if (node == NULL)
            break;

        token = strtok(NULL, "/");
    }

    // Si hemos alcanzado el final del patrón, suscribimos al cliente al nodo actual
    if (token == NULL && node != NULL)
    {
        addSubscriber(node, subscriber);
    }

    free(patternCopy);
    free(partialPattern);
}

void subscribeToTopics(TopicNode *root, const char **topics, int numTopics, const char *subscriber)
{
    if (root == NULL || topics == NULL || subscriber == NULL)
        return;

    for (int i = 0; i < numTopics; ++i)
    {
        const char *topic = topics[i];

        // Comprobar si el tópico contiene el wildcard "#"
        if (strstr(topic, "#") != NULL)
        {
            // Suscribir al cliente a todos los hijos del tópico antes del wildcard "#"
            subscribeToChildrenBeforeWildcard(root, topic, subscriber);
        }
        // Comprobar si el tópico contiene el wildcard "+"
        else if (strstr(topic, "+") != NULL)
        {
            // Suscribir al cliente a todos los nodos que coinciden con el patrón "+"
            subscribeToWildcardPlus(root, topic, subscriber);
        }
        else
        {
            // Suscribir al cliente al tópico normalmente
            TopicNode *node = getChildNode(root, topic);
            if (node != NULL)
            {
                addSubscriber(node, subscriber);
            }
        }
    }
}

int main()
{
    // Ejemplo de uso
    TopicNode *root = createTopicNode("/");

    const char *topic = "sensors/temperature/hot";
    TopicNode *node1 = getChildNode(root, topic);
    topic = "sensors/temperature/wet";
    TopicNode *node2 = getChildNode(root, topic);
    topic = "sensors/humidity";
    TopicNode *node3 = getChildNode(root, topic);
    topic = "t1/t2/hot";
    TopicNode *node4 = getChildNode(root, topic);
    topic = "t1/t2/wet";
    TopicNode *node5 = getChildNode(root, topic);
    topic = "t1/t3";
    TopicNode *node6 = getChildNode(root, topic);
    topic = "sensors/humidity/hot";
    TopicNode *node7 = getChildNode(root, topic);

    const char *topics[] = {"sensors/+/hot", "sensors/#"};
    const int numTopics = sizeof(topics) / sizeof(topics[0]);
    const char *subscriber = "new_subscriber";

    subscribeToTopics(root, topics, numTopics, subscriber);

    int numSubscribers = 0;
    TopicNode *test = getChildNode(root, "sensors/humidity/hot");
    char **subscribers = getSubscribers(test, &numSubscribers);
    if (subscribers != NULL)
    {
        printf("Los suscriptores del tópico : \n");
        for (int i = 0; i < numSubscribers; i++)
        {
            printf("- %s\n", subscribers[i]);
        }
    }

    printTree(root, 0);

    // Liberar memoria utilizada por el árbol de tópicos
    freeTopicTree(root);

    return 0;
}
