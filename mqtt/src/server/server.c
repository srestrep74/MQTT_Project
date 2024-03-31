#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>

#include "../../include/packet/packet.h"
#include "../../include/server_constants.h"

#define MAX_CLIENTS 100

Packet decode_message(int client_socket)
{
    Packet packet;
    unsigned char buffer[100];
    ssize_t data = read(client_socket, buffer, sizeof(buffer));
    size_t offset = 0;

    memcpy(&(packet.fixed_header), buffer + offset, sizeof(packet.fixed_header));
    offset += sizeof(packet.fixed_header);

    memcpy(&(packet.remaining_length), buffer + offset, sizeof(packet.remaining_length));
    offset += sizeof(packet.remaining_length);

    packet.variable_header = malloc(packet.variable_header);
    memcpy(packet.variable_header, buffer + offset, packet.remaining_length);
    offset += packet.remaining_length;

    size_t payload_size = data - offset;
    packet.payload = malloc(payload_size);
    memcpy(packet.payload, buffer + offset, payload_size);

    return packet;
}
void *handler(void *arg)
{
    int client_socket = *((int *)arg);
    Packet packet = decode_message(client_socket);
    printf(" Fixed Header : %d\n", packet.fixed_header);
    printf("RL : %d\n", packet.remaining_length);
    printf("VH : %d\n", packet.variable_header[1]);
    printf("Payload : %s\n", packet.payload);
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
        pthread_create(&thread, NULL, handler, &client_socket);
    }

    close(server_socket);

    return 0;
}