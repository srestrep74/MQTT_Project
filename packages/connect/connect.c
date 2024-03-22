#include "connect.h"
#include <string.h>
#include <stdlib.h>

void init_connect_message(connect_message_t *msg, const char *client_id, uint16_t keep_alive, const char *will_topic, const char *will_message, const char *user_name, const char *password)
{
    init_base_message(&msg->base);
    set_message_type(&msg->base, CONNECT);

    strncpy(msg->client_id, client_id, MAX_CLIENT_ID_LENGTH);

    // Check this
    msg->client_id[MAX_CLIENT_ID_LENGTH] = '\0';

    msg->keep_alive = keep_alive;

    if (will_topic)
    {
        msg->will_topic_length = strlen(will_topic);
        msg->will_topic = malloc(msg->will_topic_length + 1);
        strcpy(msg->will_topic, will_topic);
        msg->connect_flags |= WILL_FLAG;
    }
    else
    {
        msg->will_topic_length = 0;
        msg->will_topic = NULL;
    }

    if (will_message)
    {
        msg->will_message_length = strlen(will_message);
        msg->will_message = malloc(msg->will_message_length + 1);
        strcpy(msg->will_message, will_message);
        msg->connect_flags |= WILL_FLAG;
    }
    else
    {
        msg->will_message_length = 0;
        msg->will_message = NULL;
    }

    if (user_name)
    {
        msg->user_name_length = strlen(user_name);
        msg->user_name = malloc(msg->user_name_length + 1);
        strcpy(msg->user_name, user_name);
        msg->connect_flags |= USER_NAME_FLAG;
    }
    else
    {
        msg->user_name_length = 0;
        msg->user_name = NULL;
    }

    if (password)
    {
        msg->password_length = strlen(password);
        msg->password = malloc(msg->password_length + 1);
        strcpy(msg->password, password);
        msg->connect_flags |= PASSWORD_FLAG;
    }
    else
    {
        msg->password_length = 0;
        msg->password = NULL;
    }

    set_message_flags(&msg->base, msg->connect_flags);
}

void encode_connect_message(connect_message_t *msg, uint8_t *buffer, uint32_t *buffer_length)
{
    uint32_t len = 0;
    uint32_t remaining_length = 0;

    /* Codifica el encabezado fijo */
    buffer[len++] = msg->base.type;
    buffer[len++] = msg->base.flags;

    /* Calcula la longitud restante */
    remaining_length += 10;                         /* Longitud de la parte fija del mensaje CONNECT */
    remaining_length += strlen(msg->client_id) + 2; /* Client ID length + Client ID */
    if (msg->will_topic)
    {
        remaining_length += 2 + msg->will_topic_length; /* Will Topic length + Will Topic */
    }
    if (msg->will_message)
    {
        remaining_length += 2 + msg->will_message_length; /* Will Message length + Will Message */
    }
    if (msg->user_name)
    {
        remaining_length += 2 + msg->user_name_length; /* User Name length + User Name */
    }
    if (msg->password)
    {
        remaining_length += 2 + msg->password_length; /* Password length + Password */
    }

    /* Codifica la longitud restante */
    encode_remaining_length(remaining_length, buffer + len, &len);

    /* Codifica el encabezado variable y la carga útil */
    buffer[len++] = 0x00; /* Protocol Name Length MSB */
    buffer[len++] = 0x04; /* Protocol Name Length LSB */
    buffer[len++] = 'M';
    buffer[len++] = 'Q';
    buffer[len++] = 'T';
    buffer[len++] = 'T';
    buffer[len++] = 0x04;                          /* Protocol Level */
    buffer[len++] = msg->connect_flags;            /* Connect Flags */
    buffer[len++] = (msg->keep_alive >> 8) & 0xFF; /* Keep Alive MSB */
    buffer[len++] = msg->keep_alive & 0xFF;        /* Keep Alive LSB */

    /* Client ID */
    uint16_t client_id_length = strlen(msg->client_id);
    buffer[len++] = (client_id_length >> 8) & 0xFF; /* Client ID Length MSB */
    buffer[len++] = client_id_length & 0xFF;        /* Client ID Length LSB */
    memcpy(buffer + len, msg->client_id, client_id_length);
    len += client_id_length;

    /* Will Topic */
    if (msg->will_topic)
    {
        buffer[len++] = (msg->will_topic_length >> 8) & 0xFF; /* Will Topic Length MSB */
        buffer[len++] = msg->will_topic_length & 0xFF;        /* Will Topic Length LSB */
        memcpy(buffer + len, msg->will_topic, msg->will_topic_length);
        len += msg->will_topic_length;
    }

    /* Will Message */
    if (msg->will_message)
    {
        buffer[len++] = (msg->will_message_length >> 8) & 0xFF; /* Will Message Length MSB */
        buffer[len++] = msg->will_message_length & 0xFF;        /* Will Message Length LSB */
        memcpy(buffer + len, msg->will_message, msg->will_message_length);
        len += msg->will_message_length;
    }

    /* User Name */
    if (msg->user_name)
    {
        buffer[len++] = (msg->user_name_length >> 8) & 0xFF; /* User Name Length MSB */
        buffer[len++] = msg->user_name_length & 0xFF;        /* User Name Length LSB */
        memcpy(buffer + len, msg->user_name, msg->user_name_length);
        len += msg->user_name_length;
    }

    /* Password */
    if (msg->password)
    {
        buffer[len++] = (msg->password_length >> 8) & 0xFF; /* Password Length MSB */
        buffer[len++] = msg->password_length & 0xFF;        /* Password Length LSB */
        memcpy(buffer + len, msg->password, msg->password_length);
        len += msg->password_length;
    }

    *buffer_length = len;
}

connect_message_t *decode_connect_message(uint8_t *buffer, uint32_t buffer_length)
{
    connect_message_t *msg = malloc(sizeof(connect_message_t));
    uint32_t len = 0;

    /* Decodifica el encabezado fijo */
    msg->base.type = buffer[len++];
    msg->base.flags = buffer[len++];

    /* Decodifica la longitud restante */
    msg->base.remaining_length = 0;
    uint32_t multiplier = 1;
    uint8_t encoded_byte;
    do
    {
        encoded_byte = buffer[len++];
        msg->base.remaining_length += (encoded_byte & 0x7F) * multiplier;
        multiplier *= 128;
    } while ((encoded_byte & 0x80) != 0);

    /* Decodifica el encabezado variable y la carga útil */
    uint16_t protocol_name_length = (buffer[len++] << 8) | buffer[len++]; /* Protocol Name Length */
    len += protocol_name_length;                                          /* Omitimos el campo Protocol Name */

    uint8_t protocol_level = buffer[len++];                 /* Protocol Level */
    msg->connect_flags = buffer[len++];                     /* Connect Flags */
    msg->keep_alive = (buffer[len++] << 8) | buffer[len++]; /* Keep Alive */

    /* Client ID */
    uint16_t client_id_length = (buffer[len++] << 8) | buffer[len++]; /* Client ID Length */
    strncpy(msg->client_id, (char *)(buffer + len), client_id_length);
    msg->client_id[client_id_length] = '\0';
    len += client_id_length;

    /* Will Topic */
    if (msg->connect_flags & WILL_FLAG)
    {
        msg->will_topic_length = (buffer[len++] << 8) | buffer[len++]; /* Will Topic Length */
        msg->will_topic = malloc(msg->will_topic_length + 1);
        memcpy(msg->will_topic, buffer + len, msg->will_topic_length);
        msg->will_topic[msg->will_topic_length] = '\0';
        len += msg->will_topic_length;
    }
    else
    {
        msg->will_topic_length = 0;
        msg->will_topic = NULL;
    }

    /* Will Message */
    if (msg->connect_flags & WILL_FLAG)
    {
        msg->will_message_length = (buffer[len++] << 8) | buffer[len++]; /* Will Message Length */
        msg->will_message = malloc(msg->will_message_length + 1);
        memcpy(msg->will_message, buffer + len, msg->will_message_length);
        msg->will_message[msg->will_message_length] = '\0';
        len += msg->will_message_length;
    }
    else
    {
        msg->will_message_length = 0;
        msg->will_message = NULL;
    }

    /* User Name */
    if (msg->connect_flags & USER_NAME_FLAG)
    {
        msg->user_name_length = (buffer[len++] << 8) | buffer[len++]; /* User Name Length */
        msg->user_name = malloc(msg->user_name_length + 1);
        memcpy(msg->user_name, buffer + len, msg->user_name_length);
        msg->user_name[msg->user_name_length] = '\0';
        len += msg->user_name_length;
    }
    else
    {
        msg->user_name_length = 0;
        msg->user_name = NULL;
    }

    /* Password */
    if (msg->connect_flags & PASSWORD_FLAG)
    {
        msg->password_length = (buffer[len++] << 8) | buffer[len++]; /* Password Length */
        msg->password = malloc(msg->password_length + 1);
        memcpy(msg->password, buffer + len, msg->password_length);
        msg->password[msg->password_length] = '\0';
        len += msg->password_length;
    }
    else
    {
        msg->password_length = 0;
        msg->password = NULL;
    }

    return msg;
}

void free_connect_message(connect_message_t *msg)
{
    free_base_message(&msg->base);
    if (msg->will_topic)
        free(msg->will_topic);
    if (msg->will_message)
        free(msg->will_message);
    if (msg->user_name)
        free(msg->user_name);
    if (msg->password)
        free(msg->password);
}