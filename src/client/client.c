#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

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

    if (packet.variable_header)
    {
        memcpy(buffer + offset, packet.variable_header, packet.remaining_length);
        offset += packet.remaining_length;
    }

    if (packet.payload)
    {
        memcpy(buffer + offset, packet.payload, packet.remaining_length);
        offset += sizeof(packet.payload);
    }
    return buffer;
}

void send_packet(int client_socket, Packet packet)
{
    size_t total_size = sizeof(packet.fixed_header) + sizeof(packet.remaining_length) + sizeof(packet.payload) + packet.remaining_length + (packet.remaining_length - sizeof(packet.variable_header));
    unsigned char *buffer = encode_message(packet, total_size);
    write(client_socket, buffer, total_size);
}

int create_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
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

    packet.variable_header = malloc(packet.variable_header);
    memcpy(packet.variable_header, buffer + offset, packet.remaining_length);
    offset += packet.remaining_length;

    size_t payload_size = data - offset;
    packet.payload = malloc(sizeof(packet.payload));
    memcpy(packet.payload, buffer + offset, sizeof(packet.payload));

    return packet;
}

void *receive_messages(void *arg)
{
    int client_socket = *(int *)arg;
    char message[1024];
    ssize_t data;

    while (1)
    {
        // Leer el mensaje del servidor
        data = read(client_socket, message, sizeof(message));
        if (data > 0)
        {
            printf("Received message from server: %s\n", message);
            break;
        }
        else if (data == 0)
        {
            // El servidor cerró la conexión
            printf("Connection closed by server.\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    // Salir del hilo
    pthread_exit(NULL);
}

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = create_socket();
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
        printf("Connected to the server...\n");
        printf("Connected to the server...%d\n", client_socket);

        Packet connect = create_connect_message();
        send_packet(client_socket, connect);

        Packet connack = decode_message(client_socket);
        if (get_type(&(connack.fixed_header)) != CONNACK)
        {
            printf("Error: Respuesta no válida del servidor\n");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        uint8_t return_code = connack.variable_header[1];
        if (return_code == CONNACK_CONNECTION_ACCEPTED)
        {
            printf("The server has accepted the connection.\n");
            int choice;

            pthread_t thread_id;
            if (pthread_create(&thread_id, NULL, receive_messages, &client_socket) != 0)
            {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }

            while (1)
            {

                printf("Select an option:\n");
                printf("1. Subscriber\n");
                printf("2. Publisher\n");
                printf("3. Disconnect\n");
                printf("Enter the number corresponding to your choice: ");

                scanf("%d", &choice);

                getchar();

                switch (choice)
                {
                case 1:
                    printf("You have selected Subscriber.\n");
                    char topic_sub[100];
                    printf("Enter the topic\n");
                    fgets(topic_sub, sizeof(topic_sub), stdin);
                    topic_sub[strcspn(topic_sub, "\n")] = '\0';
                    Packet sub = create_subscribe_message(topic_sub);
                    send_packet(client_socket, sub);
                    break;
                case 2:
                    printf("You have selected Publisher.\n");
                    char topic[100], message[100];
                    printf("Enter the topic\n");
                    fgets(topic, sizeof(topic), stdin);
                    topic[strcspn(topic, "\n")] = '\0';
                    printf("Enter the message\n");
                    fgets(message, sizeof(message), stdin);
                    Packet pub = create_publish_message(topic, message);
                    send_packet(client_socket, pub);
                    break;
                case 3:
                    break;
                default:
                    printf("Invalid option. Please select 1 or 2.\n");
                    break;
                }
            }
        }
        else
        {
            printf("The server has denied the connection (Return code: %d).\n", return_code);
        }
    }

    close(client_socket);

    return 0;
}