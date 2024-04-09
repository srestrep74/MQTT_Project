#include "../../../include/server/topic_tree/topic_tree.h"

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

TopicNode *getChildNodeHelper(TopicNode *parent, const char *name)
{
    TopicNode *child = parent->children;
    while (child != NULL)
    {
        if (strcmp(child->name, name) == 0)
            return child;
        child = child->next_sibling;
    }

    // Crear un nuevo nodo hijo
    TopicNode *newChild = createTopicNode(name);
    if (newChild == NULL)
        return NULL;

    // Suscribir a los suscriptores del padre al nuevo nodo hijo
    int numSubscribers = 0;
    int **subscribers = getSubscribers(parent, &numSubscribers);
    if (subscribers != NULL)
    {
        newChild->subscribers = (int **)malloc(numSubscribers * sizeof(int *));
        if (newChild->subscribers != NULL)
        {
            for (int i = 0; i < numSubscribers; i++)
            {
                newChild->subscribers[i] = subscribers[i];
            }
            newChild->num_subscribers = numSubscribers;
        }
        // free(subscribers);
    }

    // Agregar el nuevo nodo hijo a la lista de hijos del padre
    newChild->next_sibling = parent->children;
    parent->children = newChild;

    return newChild;
}

int **getSubscribers(TopicNode *node, int *numSubscribers)
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
