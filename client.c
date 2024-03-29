#include "messages/base/message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server_constants.h"

void print_message_fields(const message *msg) {
    printf("Type: %d\n", msg->type);
    printf("Fixed Header:\n");
    printf("  Retain: %d\n", get_retain(*msg->fixed_header));
    printf("  QoS: %d\n", get_qos(*msg->fixed_header));
    printf("  Dup: %d\n", get_dup(*msg->fixed_header));
    printf("  Type: %d\n", get_type(*msg->fixed_header));
    printf("Variable Header:\n");
    printf("  Protocol Name: %s\n", msg->variable_header->connect_variable_h.protocol_name);
    printf("  Version: %d\n", msg->variable_header->connect_variable_h.version);
    printf("Payload Length: %zu\n", msg->payload_length);
}

void serialize_message(const message *msg, char *buffer, size_t buffer_size) {
    if (buffer_size < calculate_total_length(msg)) {
        perror("El búfer no es lo suficientemente grande para almacenar el mensaje");
        exit(EXIT_FAILURE);
    }
    memcpy(buffer, msg, calculate_total_length(msg));
}

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    message msg;
    initialize_message(&msg);
    set_fixed_header_connect(&msg);
    set_variable_header_connect(&msg);
    set_payload_connect(&msg);

    int total_length = calculate_total_length(&msg);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    int bytes_sent_size = send(sock, &total_length, sizeof(int), 0);
    if (bytes_sent_size < 0) {
        perror("Error al enviar el tamaño del mensaje al servidor");
        return -1;
    }
    printf("Bytes sent for message size: %d\n", bytes_sent_size);

    char serialized_buffer[total_length];

    serialize_message(&msg, serialized_buffer, sizeof(serialized_buffer));

    int bytes_sent_message = send(sock, serialized_buffer, total_length, 0);
    if (bytes_sent_message < 0) {
        perror("Error al enviar el mensaje al servidor");
        return -1;
    }
    printf("Bytes sent for message: %d\n", bytes_sent_message);

    printf("Message sent:\n");
    print_message_fields(&msg);

    valread = read(sock, buffer, 1024);
    if (valread < 0) {
        perror("Error al leer la respuesta del servidor");
        return -1;
    }
    printf("Message received from server: %s\n", buffer);
    destroy_message(&msg);
    return 0;
}
