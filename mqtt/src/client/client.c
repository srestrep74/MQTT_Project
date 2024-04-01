#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../../include/packet/packet.h"
#include "../../include/client_constants.h"

unsigned char *encode_message(Packet packet, size_t total_size)
{
    unsigned char *buffer = malloc(total_size);
    size_t offset = 0;
    memcpy(buffer + offset, &(packet.fixed_header), sizeof(packet.fixed_header));
    offset += sizeof(packet.fixed_header);
    memcpy(buffer + offset, &(packet.remaining_length), sizeof(packet.remaining_length));
    offset += sizeof(packet.remaining_length);
    memcpy(buffer + offset, packet.variable_header, packet.remaining_length);
    offset += packet.remaining_length;
    memcpy(buffer + offset, packet.payload, sizeof(packet.payload));
    offset += sizeof(packet.payload);
    return buffer;
}

void send_packet(int client_socket, Packet packet)
{
    size_t total_size = sizeof(packet.fixed_header) + sizeof(packet.remaining_length) + sizeof(packet.payload) + packet.remaining_length;
    unsigned char *buffer = encode_message(packet, total_size);
    write(client_socket, buffer, total_size);
}

char* generate_client_id1() {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char* client_id = (char*)malloc(MAX_CLIENT_ID_LENGTH + 1);
    if (client_id == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el identificador del cliente.\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int length = rand() % MAX_CLIENT_ID_LENGTH + 1;
    for (int i = 0; i < length; i++) {
        client_id[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    client_id[length] = '\0';

    return client_id;
}

int main()
{
    char* id = generate_client_id1();
    printf("%s\n", id);
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Connected to the server...");
    }

    Packet packet = create_connect_message();

    printf("\n%d\n", packet.fixed_header);
    printf("%d\n", packet.remaining_length);
    printf("%s\n", packet.payload);

    send_packet(client_socket, packet);
    printf("HOAL");
    close(client_socket);

    return 0;
}