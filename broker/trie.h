#ifndef TRIE_H
#define TRIE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Estructura de un nodo del Trie
typedef struct TrieNode
{
    struct TrieNode *children[128];
    bool isEndOfTopic;
    int num_subscribers;
    char **subscribers;
    char *last_message;
} TrieNode;

TrieNode *createNode();
void insertTopic(TrieNode *root, const char *topic, const char *message);
void subscribeTopic(TrieNode *root, const char *topic, const char *subscriber);
TrieNode *findTopicNode(TrieNode *root, const char *topic);
char **getSubscribers(TrieNode *node);
void destroyTrie(TrieNode *root);

#endif
