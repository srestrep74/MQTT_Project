#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <trie.h>
#include "../messages/base/message.h"

#define NUM_CLIENTS 100

typedef struct
{
    int socket;
    struct sockaddr_in address;
} client;

typedef struct
{
    TrieNode *root;
    client *clients[NUM_CLIENTS];
} server;

void initialize_server(server *server);
void handle_publish(server *server, message *msg);

#endif