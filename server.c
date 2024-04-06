#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "messages/base/message.h"
#include "encoders/fixed_header.h"
#include "encoders/utf8.h"
#include "encoders/packet.h"
#include "server_constants.h" // Include the constants header file

// Function prototypes
void *client_handler(void *client_socket);

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    pthread_t threads[MAX_CLIENTS];
    int thread_count = 0;

    // Leer el payload
    memcpy(&msg->payload, buffer + offset, sizeof(msg->payload));
    offset += sizeof(msg->payload);
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

    printf("Server is running on %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1)
    {
        printf("Waiting for incoming connections...\n");

        // Accept an incoming connection
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1)
        {
            perror("Failed to accept connection");
            continue;
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread to handle the client
        if (thread_count < MAX_CLIENTS)
        {
            pthread_create(&threads[thread_count++], NULL, client_handler, (void *)&client_socket);
        }
        else
        {
            printf("Maximum number of clients reached. Connection rejected.\n");
            close(client_socket);
        }
    }

    // Clean up
    close(server_socket);
    return 0;
}

void *client_handler(void *client_socket_ptr)
{
    int client_socket = *(int *)client_socket_ptr;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while (1)
    {
        // Receive data from the client
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == -1)
        {
            perror("Failed to receive data");
            break;
        }
        else if (bytes_received == 0)
        {
            printf("Client disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received data: %s\n", buffer);

        // Process the received data and send a response
        char *command = strtok(buffer, " ");
        if (strcmp(command, HELO_COMMAND) == 0)
        {
            const char *response = "100 OK\n";
            send(client_socket, response, strlen(response), 0);
        }
        else if (strcmp(command, QUIT_COMMAND) == 0)
        {
            const char *response = "200 BYE\n";
            send(client_socket, response, strlen(response), 0);
            break;
        }
        else if (strcmp(command, DATA_COMMAND) == 0)
        {
            const char *response = "300 DRCV\n";
            send(client_socket, response, strlen(response), 0);
        }
        else if (strcmp(command, CONNECT_COMMAND) == 0)
        {
            // Recibir el comando CONNECT como un string
            printf("Received command: %s\n", buffer);

            // Luego, recibir la estructura del mensaje CONNECT
            // Recibir la longitud de los datos del cliente
            size_t data_length;
            recv(client_socket, &data_length, sizeof(data_length), 0);

            // Recibir los datos serializados del cliente
            char serialized_msg[data_length];
            recv(client_socket, serialized_msg, data_length, 0);
            for (size_t i = 0; i < data_length; i++)
            {
                printf("%02X ", (unsigned char)serialized_msg[i]);
            }
            printf("\n");

            // Deserializar el mensaje CONNECT
            message received_msg;
            memcpy(&received_msg, serialized_msg, sizeof(received_msg));

            // Procesar el mensaje CONNECT
            // Aquí deberías verificar los campos del mensaje recibido y realizar las acciones correspondientes

            // Enviar la respuesta al cliente
            const char *response = "500 CONNECT\n";
            send(client_socket, response, strlen(response), 0);
        }
        else
        {
            const char *response = "400 BCMD\nCommand-Description: Bad command\n\r";
            send(client_socket, response, strlen(response), 0);
        }
    }

    close(client_socket);
    pthread_exit(NULL);
}
