#ifndef PUBLISH_H
#define PUBLISH_H

#include "../base/message.h"

void set_fixed_header_publish(message *msg);
void set_variable_header_publish(message *msg);
void set_payload_publish(message *msg);
void set_topic(message *message, char *topic);
void set_data(message *message, char *data);

#endif
