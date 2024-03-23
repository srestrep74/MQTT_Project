#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packages/connect/connect.h"

#define BUFFER_SIZE 1024

void print_connect_message(const ConnectMessage_t *msg)
{
    printf("Connect Message:\n");
    printf("  Client ID: %s\n", msg->clientId);
    printf("  Keep Alive: %d\n", msg->keepAlive);
    printf("  Connect Flags: 0x%02X\n", msg->connectFlags);
    if (msg->willTopic)
    {
        printf("  Will Topic: %s\n", msg->willTopic);
    }
    if (msg->willMessage)
    {
        printf("  Will Message: %s\n", msg->willMessage);
    }
    if (msg->userName)
    {
        printf("  User Name: %s\n", msg->userName);
    }
    if (msg->password)
    {
        printf("  Password: %s\n", msg->password);
    }
}

int main()
{
    ConnectMessage_t connectMsg;
    uint8_t buffer[BUFFER_SIZE];
    uint32_t bufferLength = BUFFER_SIZE;
    ConnectMessage_t *decodedMsg;

    // Initialize CONNECT message
    init_connect_message(&connectMsg, "MyClientID", 60, "WillTopic", "WillMessage", "MyUserName", "MyPassword");

    // Encode CONNECT message
    int encodeResult = encode_connect_message(&connectMsg, buffer, &bufferLength);
    if (encodeResult != 0)
    {
        printf("Failed to encode CONNECT message\n");
        free_connect_message(&connectMsg);
        return 1;
    }

    // Decode CONNECT message
    decodedMsg = decode_connect_message(buffer, bufferLength);
    if (decodedMsg == NULL)
    {
        printf("Failed to decode CONNECT message\n");
        free_connect_message(&connectMsg);
        return 1;
    }

    // Print original CONNECT message
    printf("Original CONNECT message:\n");
    print_connect_message(&connectMsg);
    printf("\n");

    // Print decoded CONNECT message
    printf("Decoded CONNECT message:\n");
    print_connect_message(decodedMsg);

    // Free memory
    free_connect_message(&connectMsg);
    free_connect_message(decodedMsg);

    return 0;
}