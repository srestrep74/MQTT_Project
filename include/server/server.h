#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/server/topic_tree/topic_tree.h"
#include "../packet/packet.h"

// Structs
typedef struct
{
    TopicNode *root;
    pthread_mutex_t mutex;
} Tree;

typedef struct
{
    int client_socket;
    int client_id;
    struct sockaddr_in client_addr;
    char *log_file;
    char *server_ip;
} ThreadInfo;

// Function definitions
Tree *get_tree();
Packet decode_message(int);
void send_packet(int, Packet);

#endif