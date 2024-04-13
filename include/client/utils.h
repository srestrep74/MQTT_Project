#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "../packet/packet.h"

// Structure definitions
typedef struct
{
    int client_socket;
    char *server_ip;
    char *client_ip;
    char *log_name;
} SocketInfo;

// Function definitions
char *generate_client_id();
void display_menu();

#endif