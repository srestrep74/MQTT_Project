#ifndef UNSUBSCRIBE_ACTION_H
#define UNSUBSCRIBE_ACTION_H

#include "../server/topic_tree/topic_tree.h"
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

void unsubscribeToTopics(TopicNode *, char **, int, int);

#endif