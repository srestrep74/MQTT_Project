#ifndef CONNECT_MESSAGE_H
#define CONNECT_MESSAGE_H

#include "./message.h"
#include "./types_messages.h"
#include "./flags_messages.h"

#define MAX_CLIENT_ID_LENGTH 23
#define MAX_WILL_TOPIC_LENGTH 65535
#define MAX_WILL_MESSAGE_LENGTH 65535
#define MAX_USER_NAME_LENGTH 65535
#define MAX_PASSWORD_LENGTH 65535

typedef struct
{
    base_message_t base;
    uint8_t connect_flags;
    uint16_t keep_alive;
    char client_id[MAX_CLIENT_ID_LENGTH + 1];
    uint16_t will_topic_length;
    char *will_topic;
    uint16_t will_message_length;
    char *will_message;
    uint16_t user_name_length;
    char *user_name;
    uint16_t password_length;
    char *password;
} connect_message_t;

void init_connect_message(connect_message_t *msg, const char *client_id, uint16_t keep_alive, const char *will_topic, const char *will_message, const char *user_name, const char *password);
void encode_connect_message(connect_message_t *msg, uint8_t *buffer, uint32_t *buffer_length);
void free_connect_message(connect_message_t *msg);

#endif