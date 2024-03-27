#ifndef TOPIC_NODE_H
#define TOPIC_NODE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct TopicNode
{
    char *topic_name;
    struct TopicNode *children;
    struct TopicNode *next_sibling;
    struct TopicNode *parent;
} TopicNode;

TopicNode *add_topic_node(TopicNode *parent, const char *topic_name);
TopicNode *find_topic_node(TopicNode *root, const char *topic_name);
void free_topic_tree(TopicNode *root);

#endif