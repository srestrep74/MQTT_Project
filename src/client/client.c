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
#include "../../include/encoders/client_encoders.h"

// Function to send a packet over a socket
void send_packet(int client_socket, Packet packet)
{
    size_t total_size = sizeof(packet.fixed_header) + sizeof(packet.remaining_length) + sizeof(packet.payload) + packet.remaining_length + (packet.remaining_length - sizeof(packet.variable_header));
    unsigned char *buffer = encode_message_client(packet, total_size);
    write(client_socket, buffer, total_size);
}

// Function to create a socket
int create_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

// Function to receive messages from the server
void *receive_messages(void *arg)
{
    int client_socket = *(int *)arg;
    char message[1024];
    ssize_t data;

    while (1)
    {
        data = read(client_socket, message, sizeof(message));
        if (data > 0)
        {
            printf("\n\x1b[35m╔══════════════════════════════════════════════════════╗\n");
            printf("\x1b[35m║ \x1b[1mReceived message from server: \x1b[0m%s", message);
            printf("\x1b[35m║\n");
            printf("\x1b[35m╚══════════════════════════════════════════════════════╝\x1b[0m\n");
            display_menu();
            fflush(stdout);
            continue;
        }
        else if (data == 0)
        {
            printf("Connection closed by server.\n");
            close(client_socket);
            pthread_exit(NULL);
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    pthread_exit(NULL);
}

// Main function
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

        Packet connect = create_connect_message();
        send_packet(client_socket, connect);

        Packet connack = decode_message_client(client_socket);
        if (get_type(&(connack.fixed_header)) != CONNACK)
        {
            printf("Error: Invalid server response\n");
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
                display_menu();
                scanf("%d", &choice);
                getchar();

                switch (choice)
                {
                case 1:
                    printf("\nYou have selected: \x1b[32mSubscriber\x1b[0m\n");

                    int num_topics;
                    printf("Enter the number of topics : ");
                    if (scanf("%d", &num_topics) != 1 || num_topics <= 0)
                    {
                        printf("Invalid number of topics. \n");
                        return 1;
                    }
                    const char **topics = (const char **)malloc(num_topics * sizeof(const char *));
                    printf("Enter the topics : \n");

                    char topicc[100];
                    for (int i = 0; i < num_topics; i++)
                    {
                        printf("Topic %d: ", i + 1);
                        if (scanf("%s", topicc) != 1)
                        {
                            printf("Error receiving the topic. \n");
                            free(topics);
                            return 1;
                        }
                        topics[i] = strdup(topicc);
                    }

                    Packet sub = create_subscribe_message(topics, num_topics);
                    send_packet(client_socket, sub);

                    break;
                case 2:
                    printf("\nYou have selected: \x1b[34mPublisher\x1b[0m\n");
                    char topic[100], message[100];
                    printf("Enter the topic: ");
                    fgets(topic, sizeof(topic), stdin);
                    topic[strcspn(topic, "\n")] = '\0';
                    printf("Enter the message: ");
                    fgets(message, sizeof(message), stdin);
                    Packet pub = create_publish_message(topic, message);
                    send_packet(client_socket, pub);
                    break;
                case 3:
                    printf("\nYou have selected: \x1b[31mDisconnect :( Bye bye!!!\x1b[0m\n ");
                    Packet disconnect = create_disconnect_message();
                    send_packet(client_socket, disconnect);
                    close(client_socket);
                    return 0;
                    break;
                default:
                    printf("\nInvalid option. Please select \x1b[33m1\x1b[0m, \x1b[33m2\x1b[0m, or \x1b[33m3\x1b[0m.\n");
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

// Function to send a packet over a socket
void send_packet(int client_socket, Packet packet)
{
    size_t total_size = sizeof(packet.fixed_header) + sizeof(packet.remaining_length) + sizeof(packet.payload) + packet.remaining_length + (packet.remaining_length - sizeof(packet.variable_header));
    unsigned char *buffer = encode_message_client(packet, total_size);
    write(client_socket, buffer, total_size);
}

// Function to create a socket
int create_socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

// Function to receive messages from the server
void *receive_messages(void *arg)
{
    int client_socket = *(int *)arg;
    char message[1024];
    ssize_t data;

    while (1)
    {
        data = read(client_socket, message, sizeof(message));
        if (data > 0)
        {
            printf("\n\x1b[35m╔══════════════════════════════════════════════════════╗\n");
            printf("\x1b[35m║ \x1b[1mReceived message from server: \x1b[0m%s", message);
            printf("\x1b[35m║\n");
            printf("\x1b[35m╚══════════════════════════════════════════════════════╝\x1b[0m\n");
            display_menu();
            fflush(stdout);
            continue;
        }
        else if (data == 0)
        {
            printf("Connection closed by server.\n");
            close(client_socket);
            pthread_exit(NULL);
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    pthread_exit(NULL);
}