#ifndef CONNECT_H
#define CONNECT_H

#include "../base/message.h"

void set_fixed_header_connect(message *msg);
void set_variable_header_connect(message *msg);
void set_payload_connect(message *msg);

#endif
