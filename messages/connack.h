#ifndef CONNACK_H
#define CONNACK_H

#include "message.h"

void set_fixed_header_connack(message *msg);
void set_variable_header_connack(message *msg);
void set_payload_connack(message *msg);

#endif
