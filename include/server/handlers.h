#ifndef HANDLERS_H
#define HANDLERS_H

#include "../packet/packet.h"
#include "topic_tree/topic_tree.h"
#include "../actions/publish.h"
#include "../constants.h"

#include <stdbool.h>

void publish_handler(Packet, TopicNode *, const char *, const char *);
bool client_handler(int, Packet);

#endif