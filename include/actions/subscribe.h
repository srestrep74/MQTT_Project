#ifndef SUBSCRIBE_ACTION_H
#define SUBSCRIBE_ACTION_H

#include "../topic_tree/topic_tree.h"

void subscribeToTopics(TopicNode *, const char **, int, const char *);
void subscribeToWildcardPlus(TopicNode *, const char *, const char *);
void subscribeToChildrenBeforeWildcard(TopicNode *, const char *, const char *); // Cambiar nombre
void subscribeToChildren(TopicNode *, const char *);
void addSubscriber(TopicNode *, const char *);

#endif