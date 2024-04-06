#ifndef TOPIC_TREE_H
#define TOPIC_TREE_H
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

TopicNode *createTopicNode(const char *);
TopicNode *getChildNode(TopicNode *, const char *);
TopicNode *getChildNodeHelper(TopicNode *, const char *);
char **getSubscribers(TopicNode *, int *);
TopicNode *getChildren(TopicNode *);
void printTree(TopicNode *, int);
void freeTopicTree(TopicNode *root);

#endif
