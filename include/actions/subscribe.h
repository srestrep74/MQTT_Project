#ifndef SUBSCRIBE_ACTION_H
#define SUBSCRIBE_ACTION_H

#include "../server/topic_tree/topic_tree.h"
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

// Function to subscribe to multiple topics
void subscribeToTopics(TopicNode *, const char **, int, const int);

// Function to subscribe to children
void subscribeToWildcardPlus(TopicNode *, const char *, const int);
void subscribeToChildrenBeforeWildcard(TopicNode *, const char *, const int); 
void subscribeToChildren(TopicNode *, const int);
void addSubscriber(TopicNode *, const int);
uint16_t get_packet_id();

#endif