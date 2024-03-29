#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include "messages/base/message.h"
#include "encoders/fixed_header.h"
#include "encoders/utf8.h"
#include "encoders/packet.h"
#include "server_constants.h"
#define TOTAL_FIELDS_EXPECTED 16

void print_fixed_header(const struct fixed_header *header) {
    printf("Fixed Header:\n");
    printf("  Raw Data:");
    for (size_t i = 0; i < sizeof(struct fixed_header); ++i) {
        printf(" %02X", ((unsigned char *)header)[i]);
    }
    printf("\n");
}

void print_message_fields(const message *msg) {
    if (msg == NULL) {
        printf("Message is NULL\n");
        return;
    }

    printf("Type: %d\n", msg->type);
    
    if (msg->fixed_header != NULL) {
        printf("Fixed Header:\n");
        printf("  Retain: %d\n", get_retain(*msg->fixed_header));
        printf("  QoS: %d\n", get_qos(*msg->fixed_header));
        printf("  Dup: %d\n", get_dup(*msg->fixed_header));
        printf("  Type: %d\n", get_type(*msg->fixed_header));
    } else {
        printf("Fixed Header is NULL\n");
    }
    printf("Payload Length: %zu\n", msg->payload_length);
}


void deserialize_message(const char *buffer, size_t buffer_size, message *msg) {
    if (buffer_size < sizeof(message)) {
        perror("Tamaño de búfer insuficiente para deserializar el mensaje");
        exit(EXIT_FAILURE);
    }
    memcpy(msg, buffer, buffer_size);
}
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    message msg;
    initialize_message(&msg);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( SERVER_PORT );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("Error al leer del socket");
        exit(EXIT_FAILURE);
    }
    printf("%ld\n", bytes_read);
    int total_length;
    memcpy(&total_length, buffer, sizeof(int));

    char *message_buffer = malloc(total_length);
    if (message_buffer == NULL) {
        perror("Error al asignar memoria para el mensaje");
        exit(EXIT_FAILURE);
    }

    ssize_t total_bytes_read = 0;
    while (total_bytes_read < total_length) {
        ssize_t bytes_read = read(new_socket, message_buffer + total_bytes_read, total_length - total_bytes_read);
        if (bytes_read < 0) {
            perror("Error al leer del socket");
            exit(EXIT_FAILURE);
        } else if (bytes_read == 0) {
            printf("El cliente ha cerrado la conexión\n");
            exit(EXIT_FAILURE);
        }
        total_bytes_read += bytes_read;
    }
    printf("%ld\n", total_bytes_read);
    printf("hola");
    deserialize_message(message_buffer, total_length, &msg);  
    print_message_fields(&msg);
    free(message_buffer);
}
