#ifndef CONNECT_H
#define CONNECT_H

#include "../responses_constants.h"
#include "../packet/packet.h"

// Function definitions
Packet create_connack_message(uint8_t);

#endif