#include "../../include/actions/subscribe.h"

void subscribeToTopics(TopicNode *root, const char **topics, int numTopics, const int subscriber)
{
    if (root == NULL || topics == NULL )
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

void subscribeToWildcardPlus(TopicNode *node, const char *pattern, const int subscriber)
{
    if (node == NULL || pattern == NULL )
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

// Función para suscribir un cliente a todos los hijos de un tópico dado
void subscribeToChildrenBeforeWildcard(TopicNode *root, const char *topic, const int subscriber)
{
    if (root == NULL || topic == NULL )
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

void subscribeToChildren(TopicNode *node, const int subscriber)
{
    if (node == NULL )
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

// Función para agregar un suscriptor a un nodo del árbol
void addSubscriber(TopicNode *node, const int subscriber)
{
    if (node == NULL )
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