#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../include/server/server.h"
#include "../../include/packet/packet.h"
#include "../../include/actions/publish.h"
#include "../../include/server/handlers.h"
#include "../../include/encoders/server_encoders.h"

#define MAX_CLIENTS 100

// Function definitions
Tree *get_tree()
{
    static Tree tree;
    static bool init = false;
    if (!init)
    {
        tree.root = createTopicNode("/");
        pthread_mutex_init(&tree.mutex, NULL);
        init = true;
    }

    return &tree;
}

// Function to send a packet over a socket
void send_packet(int client_socket, Packet packet)
{
    size_t total_size = sizeof(packet.fixed_header) + sizeof(packet.remaining_length) + sizeof(packet.payload) + packet.remaining_length;
    unsigned char *buffer = encode_message_server(packet, total_size);
    write(client_socket, buffer, total_size);
    free(buffer);
}

// Function to handle client requests
void *handler(void *arg)
{
    ThreadInfo *thread_info = (ThreadInfo *)arg;
    int client_socket = thread_info->client_socket;
    int client_id = thread_info->client_id;
    struct sockaddr_in client_addr = thread_info->client_addr;
    char *client_ip = inet_ntoa(client_addr.sin_addr);
    char *log_file = thread_info->log_file;
    char *server_ip = thread_info->server_ip;

    printf("Handling client %d\n", client_id);

    Packet packet = decode_message_server(client_socket);

    if (client_handler(client_socket, packet))
    {
        while (1)
        {
            packet = decode_message_server(client_socket);

            if (get_type(&packet.fixed_header) == PUBLISH)
            {
                const char *message = utf8_decode(packet.payload);
                char *topic = utf8_decode(get_topic(&packet));
                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                publish_handler(packet, tree->root, topic, message);
                pthread_mutex_unlock(&tree->mutex);
                log_activity(log_file, client_ip, "PUBLISH", server_ip);
            }
            else if (get_type(&packet.fixed_header) == SUBSCRIBE)
            {
                int offset = 0;
                char **topics = NULL;
                int num_topics = 0;
                while (offset < packet.remaining_length)
                {
                    int topic_length = packet.payload[offset++];
                    char *topic = (char *)malloc(topic_length + 1);
                    memcpy(topic, &packet.payload[offset], topic_length);
                    topic[topic_length] = '\0';
                    offset += topic_length + 1;
                    num_topics++;
                    topics = (char **)realloc(topics, num_topics * sizeof(char *));
                    topics[num_topics - 1] = topic;
                    topic[topic_length] = '\0';

                }

                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                subscribe_handler(packet, tree->root, topics, client_socket, num_topics - 1);
                pthread_mutex_unlock(&tree->mutex);
                log_activity(log_file, client_ip, "SUBSCRIBE", server_ip);
            }
            else if (get_type(&packet.fixed_header) == UNSUBSCRIBE)
            {
                int offset = 0;
                char **topicss = NULL;
                int num_topicss = 0;
                while (offset < packet.remaining_length)
                {
                    int topic_length = packet.payload[offset++];

                    char *topic = (char *)malloc(topic_length + 2); 
                    memcpy(topic, &packet.payload[offset], topic_length);
                    offset += topic_length + 1;
                    num_topicss++;
                    topicss = (char **)realloc(topicss, num_topicss * sizeof(char *));
                    topicss[num_topicss - 1] = topic;
                    topic[topic_length] = '\0';
                }
                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                unsubscribe_handler(packet, tree->root, topicss, client_socket, num_topicss - 1);
                pthread_mutex_unlock(&tree->mutex);
                log_activity(log_file, client_ip, "UNSUBSCRIBE", server_ip);
            }
            else if (get_type(&packet.fixed_header) == DISCONNECT)
            {
                log_activity(log_file, client_ip, "DISCONNECT", server_ip);
                close(client_socket);
                break;
            }
        }
    }
    else
    {
        printf("Error in CONNECT\n");
    }

    close(client_socket);
    return NULL;
}

// Main function
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <log_filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
 
    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *log_file = argv[3];

    int server_socket, client_socket;
    socklen_t client_len;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is running on %s:%d\n", ip, port);

    int client_id = 0;

    while (1)
    {
        printf("Waiting for incoming connections...\n");

        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1)
        {
            perror("Failed to accept connection");
            printf("Error code: %d\n", errno);
            exit(0);
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        pthread_t thread;
        ThreadInfo client_info = {client_socket, client_id, client_addr, log_file, ip};
        pthread_create(&thread, NULL, handler, &client_info);

        client_id++;
    }

    close(server_socket);

    return 0;
}
