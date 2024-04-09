#ifndef SUBSCRIBE_ACTION_H
#define SUBSCRIBE_ACTION_H

#include "../server/topic_tree/topic_tree.h"
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

void subscribeToTopics(TopicNode *, const char **, int, const int);
void subscribeToWildcardPlus(TopicNode *, const char *, const int);
void subscribeToChildrenBeforeWildcard(TopicNode *, const char *, const int); // Cambiar nombre
void subscribeToChildren(TopicNode *, const int);
void addSubscriber(TopicNode *, const int);
uint16_t get_packet_id();

#endif