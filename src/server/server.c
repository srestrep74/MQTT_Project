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
#include "../../include/encoders/server_encoders.h"

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

void send_packet(int client_socket, Packet packet)
{
    size_t total_size = sizeof(packet.fixed_header) + sizeof(packet.remaining_length) + sizeof(packet.payload) + packet.remaining_length;
    unsigned char *buffer = encode_message_server(packet, total_size);
    write(client_socket, buffer, total_size);
}

void *handler(void *arg)
{
    ClientInfo *client_info = (ClientInfo *)arg;
    int client_socket = client_info->client_socket;
    int client_id = client_info->client_id;

    printf("Handling client %d\n", client_id);
    Packet packet = decode_message_server(client_socket);

    if (client_handler(client_socket, packet))
    {
        while (1)
        {
            // printf("Entre\n");
            packet = decode_message_server(client_socket);
            if (get_type(&packet.fixed_header) == PUBLISH)
            {
                const char *message = utf8_decode(packet.payload);
                char *topic = utf8_decode(get_topic(&packet));
                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                publish_handler(packet, tree->root, topic, message);
                pthread_mutex_unlock(&tree->mutex);
            }
            else if (get_type(&packet.fixed_header) == SUBSCRIBE)
            {
                int offset = 0;
                char **topics = NULL;
                int num_topics = 0;
                while (offset < packet.remaining_length)
                {
                    // Obtener la longitud del tópico
                    int topic_length = packet.payload[offset++];

                    // Reservar memoria para el tópico y copiarlo
                    char *topic = (char *)malloc(topic_length + 2); // +1 para el carácter nulo
                    memcpy(topic, &packet.payload[offset], topic_length);
                    // topic[topic_length] = '\0'; // Agregar el carácter nulo al final

                    // Incrementar el desplazamiento
                    offset += topic_length + 2;
                    num_topics++;
                    topics = (char **)realloc(topics, num_topics * sizeof(char *));
                    topics[num_topics - 1] = topic;
                    topic[topic_length] = '\0';
                }

                for (int i = 0; i < num_topics - 1; i++)
                {
                    printf("Topic tree: %s\n", topics[i]);
                }
                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                subscribe_handler(packet, tree->root, topics, client_socket);
                pthread_mutex_unlock(&tree->mutex);
                /*char *topic = utf8_decode(packet.payload);

                Tree *tree = get_tree();
                pthread_mutex_lock(&tree->mutex);
                subscribe_handler(packet, tree->root, topic, client_socket);
                pthread_mutex_unlock(&tree->mutex);*/
            }
            else if (get_type(&packet.fixed_header) == DISCONNECT)
            {
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
        ClientInfo client_info = {client_socket, client_id};
        pthread_create(&thread, NULL, handler, &client_info);

        client_id++;
    }

    close(server_socket);

    return 0;
}