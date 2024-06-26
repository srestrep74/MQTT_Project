#ifndef SERVER_ENCODERS_H
#define SERVER_ENCODERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "../packet/packet.h"

// Function definitions
unsigned char *encode_message_server(Packet, size_t);
Packet decode_message_server(int);

#endif