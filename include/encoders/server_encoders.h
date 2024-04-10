#ifndef SERVER_ENCODERS_H
#define SERVER_ENCODERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "../packet/packet.h"

// Function definitions
unsigned char *encode_message_server(Packet packet, size_t total_size);
Packet decode_message_server(int client_socket);

#endif