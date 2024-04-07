#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../include/server/server.h"
#include "../../include/packet/packet.h"
#include "../../include/server_constants.h"
#include "../../include/actions/publish.h"
#include "../../include/server/handlers.h"

#define MAX_CLIENTS 100

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

unsigned char *encode_message(Packet packet, size_t total_size)
{
    unsigned char *buffer = malloc(total_size);
    size_t offset = 0;

    memcpy(buffer + offset, &(packet.fixed_header), sizeof(packet.fixed_header));
    offset += sizeof(packet.fixed_header);

    memcpy(buffer + offset, &(packet.remaining_length), sizeof(packet.remaining_length));
    offset += sizeof(packet.remaining_length);

    if (packet.variable_header)
    {
        memcpy(buffer + offset, packet.variable_header, packet.remaining_length);
        offset += packet.remaining_length;
    }

    if (packet.payload)
    {
        memcpy(buffer + offset, packet.payload, sizeof(packet.payload));
        offset += sizeof(packet.payload);
    }
    return buffer;
}

void send_packet(int client_socket, Packet packet)
{
    size_t total_size = sizeof(packet.fixed_header) + sizeof(packet.remaining_length) + sizeof(packet.payload) + packet.remaining_length;
    unsigned char *buffer = encode_message(packet, total_size);
    write(client_socket, buffer, total_size);
}

Packet decode_message(int client_socket)
{
    Packet packet = {0};
    unsigned char buffer[1000];

    ssize_t data = read(client_socket, buffer, sizeof(buffer));

    if (data == -1)
        return packet;

    size_t offset = 0;

    memcpy(&(packet.fixed_header), buffer + offset, sizeof(packet.fixed_header));
    offset += sizeof(packet.fixed_header);

    memcpy(&(packet.remaining_length), buffer + offset, sizeof(packet.remaining_length));
    offset += sizeof(packet.remaining_length);

    packet.variable_header = malloc(packet.remaining_length);
    memcpy(packet.variable_header, buffer + offset, packet.remaining_length);
    offset += packet.remaining_length;

    size_t payload_size = data - offset;
    packet.payload = malloc(payload_size + 1);
    memcpy(packet.payload, buffer + offset, payload_size);
    packet.payload[payload_size] = '\0';

    return packet;
}


void *handler(void *arg)
{
    int client_socket = *((int *)arg);
    printf("%d\n", client_socket);
    Packet packet = decode_message(client_socket);

    if (client_handler(client_socket, packet))
    {
        while(1){
            //printf("Entre\n");
            packet = decode_message(client_socket);
            if(get_type(&packet.fixed_header) == PUBLISH){
                const char *message = utf8_decode(packet.payload);
                char *topic = utf8_decode(get_topic(&packet));

                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                publish_handler(packet, tree->root, topic, message);
                pthread_mutex_unlock(&tree->mutex);
            }else if(get_type(&packet.fixed_header) == SUBSCRIBE){
                char *topic = utf8_decode(packet.payload);

                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                subscribe_handler(packet, tree->root, topic, client_socket);
                pthread_mutex_unlock(&tree->mutex);
            }else if(get_type(&packet.fixed_header) == DISCONNECT){
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

int main()
{
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
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

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

    printf("Server is running on %s:%d\n", SERVER_IP, SERVER_PORT);

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
        printf("%d\n", client_socket);
        pthread_create(&thread, NULL, handler, &client_socket);
    }

    close(server_socket);

    return 0;
}