#ifndef CLIENT_ENCODERS_H
#define CLIENT_ENCODERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "../packet/packet.h"

unsigned char *encode_message_client(Packet packet, size_t total_size);
Packet decode_message_client(int client_socket);

#endif