#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/server/topic_tree/topic_tree.h"
#include "../packet/packet.h"

typedef struct
{
    TopicNode *root;
    pthread_mutex_t mutex;
} Tree;

typedef struct
{
    int client_socket;
    int client_id;
} ClientInfo;

Tree *get_tree();
Packet decode_message(int);
void send_packet(int client_socket, Packet packet);

#endif