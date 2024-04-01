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
    unsigned char fixed_header;
    unsigned char remaining_length_bytes[4];
    ssize_t data;

    // Leer la cabecera fija del mensaje
    data = read(client_socket, &fixed_header, sizeof(fixed_header));
    if (data <= 0) {
        // Error de lectura
        perror("Error al leer la cabecera fija del mensaje");
        exit(EXIT_FAILURE);
    }

    // Leer el campo de longitud restante del mensaje (variable)
    size_t offset = 0;
    while (offset < sizeof(remaining_length_bytes)) {
        ssize_t bytes_read = read(client_socket, remaining_length_bytes + offset, sizeof(remaining_length_bytes) - offset);
        if (bytes_read <= 0) {
            // Error de lectura
            perror("Error al leer el campo de longitud restante del mensaje");
            exit(EXIT_FAILURE);
        }
        offset += bytes_read;
    }

    // Decodificar la longitud restante del mensaje
    size_t remaining_length = 0;
    int multiplier = 1;
    int byte;
    offset = 0;
    do {
        byte = remaining_length_bytes[offset];
        remaining_length += (byte & 127) * multiplier;
        multiplier *= 128;
        offset++;
    } while ((byte & 128) != 0 && offset < sizeof(remaining_length_bytes));

    // Asignar memoria para el header variable y el payload del mensaje
    packet.variable_header = malloc(remaining_length);
    packet.payload = malloc(remaining_length);

    // Leer el header variable y el payload del mensaje
    offset = 0;
    while (offset < remaining_length) {
        ssize_t bytes_read = read(client_socket, packet.variable_header + offset, remaining_length - offset);
        if (bytes_read <= 0) {
            // Error de lectura
            perror("Error al leer el header variable y el payload del mensaje");
            exit(EXIT_FAILURE);
        }
        offset += bytes_read;
    }

    // Asegurarse de que el header variable y el payload estén terminados con un carácter nulo
    packet.variable_header[remaining_length] = '\0';
    packet.payload[remaining_length] = '\0';


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
    free(packet.variable_header);
    free(packet.payload);
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