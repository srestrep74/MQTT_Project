#ifndef CLIENT_ENCODERS_H
#define CLIENT_ENCODERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "../packet/packet.h"

// Function definitions
unsigned char *encode_message_client(Packet, size_t);
Packet decode_message_client(int);

#endif