#include "../../../include/tree/trie.h"

// Crear un nuevo nodo del Trie
TrieNode *createNode()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node)
    {
        node->isEndOfTopic = false;
        node->num_subscribers = 0;
        node->subscribers = NULL;
        node->last_message = NULL;
        for (int i = 0; i < 128; i++)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

void insertMessageInMatchingNodes(TrieNode *node, const char *message)
{
    if (node == NULL)
    {
        return;
    }

    if (node->isEndOfTopic)
    {
        free(node->last_message);
        node->last_message = strdup(message);
    }

    for (int i = 0; i < 128; i++)
    {
        insertMessageInMatchingNodes(node->children[i], message);
    }
}

void insertTopicWithWildcard(TrieNode *root, const char *topic, const char *message)
{
    TrieNode *current = root;
    char *topic_copy = strdup(topic); // Hacer una copia mutable del tema
    char *token = strtok(topic_copy, "/");
    while (token)
    {
        if (strcmp(token, "#") == 0)
        {
            // Wildcard multi-nivel, insertar el mensaje en el nodo actual y todos sus descendientes
            insertMessageInMatchingNodes(current, message);
            break;
        }
        else if (strcmp(token, "+") == 0)
        {
            // Wildcard de nivel único, continuar la inserción recursiva
            // solo en el siguiente nivel
            for (int i = 0; i < 128; i++)
            {
                if (current->children[i])
                {
                    insertMessageInMatchingNodes(current->children[i], message);
                }
            }
        }
        else
        {
            int index = (int)*token;
            if (!current->children[index])
            {
                current->children[index] = createNode();
            }
            current = current->children[index];
        }
        token = strtok(NULL, "/");
    }
    // Insertar el mensaje en el nodo final del tema
    insertMessageInMatchingNodes(current, message);
    free(topic_copy); // Liberar la memoria de la copia del tema
}

// Modificación de la función insertTopic original para que llame a insertTopicWithWildcard si se detecta un filtro en el tema
void insertTopic(TrieNode *root, const char *topic, const char *message)
{
    if (strstr(topic, "#") || strstr(topic, "+"))
    {
        // El tema contiene wildcards, usar la función especializada
        insertTopicWithWildcard(root, topic, message);
    }
    else
    {
        // El tema no contiene wildcards, insertar el mensaje como antes
        TrieNode *current = root;
        for (int i = 0; i < strlen(topic); i++)
        {
            int index = (int)topic[i];
            if (!current->children[index])
            {
                current->children[index] = createNode();
            }
            current = current->children[index];
        }
        current->isEndOfTopic = true;
        free(current->last_message);
        current->last_message = strdup(message);
    }
}

// Suscribirse a un tópico
void subscribeTopic(TrieNode *root, const char *topic, const char *subscriber)
{
    TrieNode *current = root;
    for (int i = 0; i < strlen(topic); i++)
    {
        int index = (int)topic[i];
        if (!current->children[index])
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfTopic = true;
    current->num_subscribers++;
    current->subscribers = realloc(current->subscribers, current->num_subscribers * sizeof(char *));
    current->subscribers[current->num_subscribers - 1] = strdup(subscriber);
}

TrieNode *findTopicNode(TrieNode *root, const char *topic)
{
    TrieNode *current = root;
    for (int i = 0; i < strlen(topic); i++)
    {
        int index = (int)topic[i];
        if (!current->children[index])
        {
            return NULL; // Tópico no encontrado
        }
        current = current->children[index];
    }
    return current; // Devolver el nodo del último tópico encontrado
}

char **getSubscribers(TrieNode *node)
{
    if (node && node->isEndOfTopic)
    {
        return node->subscribers;
    }
    return NULL; // Tópico no encontrado o no tiene suscriptores
}

// Liberar la memoria utilizada por el Trie
void destroyTrie(TrieNode *root)
{
    if (root)
    {
        for (int i = 0; i < 128; i++)
        {
            destroyTrie(root->children[i]);
        }
        free(root->subscribers);
        free(root);
    }
}
