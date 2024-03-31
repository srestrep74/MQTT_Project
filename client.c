#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "messages/base/message.h"
#include "encoders/fixed_header.h"
#include "encoders/utf8.h"

#include "client_constants.h"

#define COMMAND_BUFFER_SIZE (BUFFER_SIZE * 2)

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command_and_data[COMMAND_BUFFER_SIZE];

    // Create the client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server from: %s:%d\n", SERVER_IP, SERVER_PORT);
    printf("Enter \"%s\" to exit\n", QUIT_COMMAND);
    printf("Input commands:\n");

    while (1)
    {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove the newline character

        if (strcmp(buffer, QUIT_COMMAND) == 0)
        {
            break;
        }
        else if (strcmp(buffer, CONNECT_COMMAND) == 0)
        {
            // Enviar el comando CONNECT como un string
            send(client_socket, buffer, strlen(buffer), 0);

            // Luego, enviar la estructura del mensaje CONNECT
            message connect_msg;
            connect_msg.type = CONNECT;
            initialize_message(&connect_msg);
            connect_msg.set_fixed_header(&connect_msg);
            connect_msg.set_variable_header(&connect_msg);
            connect_msg.set_payload(&connect_msg);
            encode(&connect_msg);

            printf("Tipo : %d\n", connect_msg.type);

            // Serializar el mensaje CONNECT
            char serialized_msg[sizeof(message)];
            memcpy(serialized_msg, &connect_msg, sizeof(message));

            // Enviar la longitud de los datos al servidor
            size_t data_length = sizeof(serialized_msg);
            send(client_socket, &data_length, sizeof(data_length), 0);

            // Enviar los datos serializados al servidor
            send(client_socket, serialized_msg, data_length, 0);

            // Esperar la respuesta del servidor
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received == -1)
            {
                perror("Failed to receive data");
                break;
            }

            buffer[bytes_received] = '\0';
            printf("%s\n", buffer);
        }
        else
        {
            send(client_socket, buffer, strlen(buffer), 0);

            memset(buffer, 0, BUFFER_SIZE);
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received == -1)
            {
                perror("Failed to receive data");
                break;
            }

            buffer[bytes_received] = '\0';
            printf("%s\n", buffer);
        }
    }

    printf("Closing connection...BYE BYE...\n");
    close(client_socket);

    return 0;
}
