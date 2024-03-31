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

size_t serialize_message(const message *msg, char *buffer, size_t buffer_size)
{
    size_t offset = 0;

    // Copiar el tipo de mensaje
    memcpy(buffer + offset, &msg->type, sizeof(int8_t));
    offset += sizeof(int8_t);

    // Copiar el fixed header
    memcpy(buffer + offset, &msg->fixed_header, sizeof(msg->fixed_header));
    offset += sizeof(msg->fixed_header);

    // Copiar el variable header
    memcpy(buffer + offset, &msg->variable_header, sizeof(msg->variable_header));
    offset += sizeof(msg->variable_header);

    // Copiar el payload
    memcpy(buffer + offset, &msg->payload, sizeof(msg->payload));
    offset += sizeof(msg->payload);

    return offset;
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

    size_t serialized_size = serialize_message(&msg, serialized_buffer, total_length);

    // Verificar si la serialización fue exitosa y obtener el tamaño total serializado
    if (serialized_size > 0) {
        printf("La estructura message ha sido serializada con éxito. Tamaño total serializado: %zu bytes.\n", serialized_size);
        // Ahora puedes enviar el búfer serializado a través de la red, escribirlo en un archivo, etc.
    } else {
        printf("Error al serializar la estructura message.\n");
    }

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
