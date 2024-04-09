#ifndef SUBSCRIBE_ACTION_H
#define SUBSCRIBE_ACTION_H

#include "../server/topic_tree/topic_tree.h"

void subscribeToTopics(TopicNode *, const char **, int, const int);
void subscribeToWildcardPlus(TopicNode *, const char *, const int);
void subscribeToChildrenBeforeWildcard(TopicNode *, const char *, const int); // Cambiar nombre
void subscribeToChildren(TopicNode *, const int);
void addSubscriber(TopicNode *, const int);

#endif