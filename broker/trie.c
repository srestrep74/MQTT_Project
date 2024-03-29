#include "trie.h"

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

// Insertar un tópico en el Trie
void insertTopic(TrieNode *root, const char *topic, const char *message)
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
    free(current->last_message);
    current->last_message = strdup(message);
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
