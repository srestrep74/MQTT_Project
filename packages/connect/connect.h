#ifndef CONNECT_MESSAGE_H
#define CONNECT_MESSAGE_H

#include "../message.h"
#include "../types_messages.h"
#include "../flags_messages.h"

#define MAX_CLIENT_ID_LENGTH 23
#define MAX_WILL_TOPIC_LENGTH 65535
#define MAX_WILL_MESSAGE_LENGTH 65535
#define MAX_USER_NAME_LENGTH 65535
#define MAX_PASSWORD_LENGTH 65535

typedef struct
{
    BaseMessage_t base;
    uint8_t connectFlags;
    uint16_t keepAlive;
    char clientId[MAX_CLIENT_ID_LENGTH + 1];
    uint16_t willTopicLength;
    char *willTopic;
    uint16_t willMessageLength;
    char *willMessage;
    uint16_t userNameLength;
    char *userName;
    uint16_t passwordLength;
    char *password;
} ConnectMessage_t;

void init_connect_message(ConnectMessage_t *msg, const char *clientId, uint16_t keepAlive, const char *willTopic, const char *willMessage, const char *userName, const char *password);
int encode_connect_message(ConnectMessage_t *msg, uint8_t *buffer, uint32_t *bufferLength);
ConnectMessage_t *decode_connect_message(const uint8_t *buffer, uint32_t bufferLength);
void free_connect_message(ConnectMessage_t *msg);

#endif