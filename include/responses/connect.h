#ifndef CONNECT_H
#define CONNECT_H

#include "../responses_constants.h"
#include "../packet/packet.h"

Packet create_connack_message(uint8_t);

#endif