#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/server/topic_tree/topic_tree.h"

typedef struct
{
    TopicNode *root;
    pthread_mutex_t mutex;
} Tree;

Tree *get_tree();

#endif