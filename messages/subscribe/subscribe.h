#ifndef SUBSCRIBE_H
#define SUBSCRIBE_H

#include "../base/message.h"

void set_fixed_header_subscribe(message *msg);
void set_variable_header_subscribe(message *msg);
void set_payload_subscribe(message *msg);
struct subscribe create_subscription(char *topic, int8_t qos);
void set_topics(message *message, char **topics, int num_topics);

#endif