#include "../../../include/server/topic_tree/topic_tree.h"

// Function to create a new topic node
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

// Function to get the child node of a parent node based on the topic
TopicNode *getChildNode(TopicNode *parent, const char *topic)
{
    if (parent == NULL || topic == NULL)
        return NULL;

    char *topicCopy = strdup(topic);
    if (topicCopy == NULL)
        return NULL;

    char *token = strtok(topicCopy, "/");
    TopicNode *currentNode = parent;
    while (token != NULL)
    {
        currentNode = getChildNodeHelper(currentNode, token);
        if (currentNode == NULL)
            break;
        token = strtok(NULL, "/");
    }


    free(topicCopy);
    return currentNode;
}

// Helper function to get the child node of a parent node based on the name
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

// Function to get the subscribers of a topic node
int **getSubscribers(TopicNode *node, int *numSubscribers)
{
    if (node == NULL || numSubscribers == NULL)
        return NULL;

    *numSubscribers = node->num_subscribers;
    return node->subscribers;
}

// Function to get the children of a topic node
TopicNode *getChildren(TopicNode *node)
{
    if (node == NULL)
        return NULL;

    return node->children;
}

// Function to print the topic tree
void printTree(TopicNode *node, int depth)
{
    
    if (node == NULL)
        return;

    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("|- %s\n", node->name);

    printTree(node->children, depth + 1);
    printTree(node->next_sibling, depth);
    printf("\n");
}

// Function to free the memory allocated for the topic tree
void freeTopicTree(TopicNode *root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < root->num_subscribers; i++)
    {
        free(root->subscribers[i]);
    }
    free(root->subscribers);

    TopicNode *child = root->children;
    while (child != NULL)
    {
        TopicNode *next = child->next_sibling;
        freeTopicTree(child);
        child = next;
    }

    free(root->name);
    free(root->last_message);

    free(root);
}
