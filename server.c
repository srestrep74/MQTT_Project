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

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the server socket to the address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        perror("Failed to listen");
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
            const char *response = "500 CONNECTED\n";
            send(client_socket, response, strlen(response), 0);

            message connect_msg;
           size_t received_size;
           recv(client_socket, &received_size, sizeof(received_size), 0);
           printf("Received size: %zu\n", received_size);
           char r_buffer[1024];
           recv(client_socket,&r_buffer, received_size, 0);
         printf("%s","HOLA");
           deserialize_message(r_buffer, received_size, &connect_msg);        
 
           char *username;
           decode_string(&connect_msg.payload->connect_payload.user_name, &username);
 
           printf("Tipo de mensaje recibido: %s\n", username);
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