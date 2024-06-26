#ifndef TOPIC_TREE_H
#define TOPIC_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct definitions
typedef struct TopicNode
{
    char *name;
    char *last_message;
    int **subscribers;
    int num_subscribers;
    struct TopicNode *children;
    struct TopicNode *next_sibling;
} TopicNode;

// Function definitions
TopicNode *createTopicNode(const char *);
TopicNode *getChildNode(TopicNode *, const char *);
TopicNode *getChildNodeHelper(TopicNode *, const char *);
int **getSubscribers(TopicNode *, int *);
TopicNode *getChildren(TopicNode *);
void printTree(TopicNode *, int);
void freeTopicTree(TopicNode *);

#endif
