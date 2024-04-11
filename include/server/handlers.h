#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdbool.h>

#include "../packet/packet.h"
#include "../constants.h"
#include "server.h"

// Function definitions
bool client_handler(int, Packet);
void publish_handler(Packet, TopicNode *, const char *, const char *);
void unsubscribe_handler(Packet, TopicNode *, const char **, int, int);
void subscribe_handler(Packet, TopicNode *, const char **, int, int);

#endif