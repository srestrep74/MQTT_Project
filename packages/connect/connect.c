#include "connect.h"
#include <string.h>
#include <stdlib.h>
#include "../message.h"

static uint32_t get_remaining_length(const ConnectMessage_t *msg)
{
    uint32_t remainingLength = 10;                /* Length of fixed part of CONNECT message */
    remainingLength += strlen(msg->clientId) + 2; /* Client ID length + Client ID */

    if (msg->willTopic)
    {
        remainingLength += 2 + msg->willTopicLength; /* Will Topic length + Will Topic */
    }

    if (msg->willMessage)
    {
        remainingLength += 2 + msg->willMessageLength; /* Will Message length + Will Message */
    }

    if (msg->userName)
    {
        remainingLength += 2 + msg->userNameLength; /* User Name length + User Name */
    }

    if (msg->password)
    {
        remainingLength += 2 + msg->passwordLength; /* Password length + Password */
    }

    return remainingLength;
}

void init_connect_message(ConnectMessage_t *msg, const char *clientId, uint16_t keepAlive, const char *willTopic, const char *willMessage, const char *userName, const char *password)
{
    init_base_message(&msg->base);
    set_message_type(&msg->base, CONNECT);

    strncpy(msg->clientId, clientId, MAX_CLIENT_ID_LENGTH);
    msg->clientId[MAX_CLIENT_ID_LENGTH] = '\0';

    msg->keepAlive = keepAlive;
    msg->connectFlags = 0;

    if (willTopic)
    {
        msg->willTopicLength = strlen(willTopic);
        msg->willTopic = malloc(msg->willTopicLength + 1);
        if (msg->willTopic == NULL)
        {
            // Handle memory allocation failure
            return;
        }
        strcpy(msg->willTopic, willTopic);
        msg->connectFlags |= WILL_FLAG;
    }
    else
    {
        msg->willTopicLength = 0;
        msg->willTopic = NULL;
    }

    if (willMessage)
    {
        msg->willMessageLength = strlen(willMessage);
        msg->willMessage = malloc(msg->willMessageLength + 1);
        if (msg->willMessage == NULL)
        {
            // Handle memory allocation failure
            free(msg->willTopic);
            return;
        }
        strcpy(msg->willMessage, willMessage);
        msg->connectFlags |= WILL_FLAG;
    }
    else
    {
        msg->willMessageLength = 0;
        msg->willMessage = NULL;
    }

    if (userName)
    {
        msg->userNameLength = strlen(userName);
        msg->userName = malloc(msg->userNameLength + 1);
        if (msg->userName == NULL)
        {
            // Handle memory allocation failure
            free(msg->willTopic);
            free(msg->willMessage);
            return;
        }
        strcpy(msg->userName, userName);
        msg->connectFlags |= USER_NAME_FLAG;
    }
    else
    {
        msg->userNameLength = 0;
        msg->userName = NULL;
    }

    if (password)
    {
        msg->passwordLength = strlen(password);
        msg->password = malloc(msg->passwordLength + 1);
        if (msg->password == NULL)
        {
            // Handle memory allocation failure
            free(msg->willTopic);
            free(msg->willMessage);
            free(msg->userName);
            return;
        }
        strcpy(msg->password, password);
        msg->connectFlags |= PASSWORD_FLAG;
    }
    else
    {
        msg->passwordLength = 0;
        msg->password = NULL;
    }

    set_message_flags(&msg->base, msg->connectFlags);
}

int encode_connect_message(ConnectMessage_t *msg, uint8_t *buffer, uint32_t *bufferLength)
{
    uint32_t len = 0;
    uint32_t remainingLength = get_remaining_length(msg);

    if (*bufferLength < remainingLength + 2)
    {
        // Buffer is too small to encode the message
        return -1;
    }

    /* Encode fixed header */
    buffer[len++] = msg->base.type;
    buffer[len++] = msg->base.flags;

    /* Encode remaining length */
    encode_remaining_length(remainingLength, buffer + len);
    len += get_remaining_length_bytes(remainingLength);

    /* Encode variable header and payload */
    buffer[len++] = 0x00; /* Protocol Name Length MSB */
    buffer[len++] = 0x04; /* Protocol Name Length LSB */
    buffer[len++] = 'M';
    buffer[len++] = 'Q';
    buffer[len++] = 'T';
    buffer[len++] = 'T';
    buffer[len++] = 0x04;                         /* Protocol Level */
    buffer[len++] = msg->connectFlags;            /* Connect Flags */
    buffer[len++] = (msg->keepAlive >> 8) & 0xFF; /* Keep Alive MSB */
    buffer[len++] = msg->keepAlive & 0xFF;        /* Keep Alive LSB */

    /* Client ID */
    uint16_t clientIdLength = strlen(msg->clientId);
    buffer[len++] = (clientIdLength >> 8) & 0xFF; /* Client ID Length MSB */
    buffer[len++] = clientIdLength & 0xFF;        /* Client ID Length LSB */
    memcpy(buffer + len, msg->clientId, clientIdLength);
    len += clientIdLength;

    /* Will Topic */
    if (msg->willTopic)
    {
        buffer[len++] = (msg->willTopicLength >> 8) & 0xFF; /* Will Topic Length MSB */
        buffer[len++] = msg->willTopicLength & 0xFF;        /* Will Topic Length LSB */
        memcpy(buffer + len, msg->willTopic, msg->willTopicLength);
        len += msg->willTopicLength;
    }

    /* Will Message */
    if (msg->willMessage)
    {
        buffer[len++] = (msg->willMessageLength >> 8) & 0xFF; /* Will Message Length MSB */
        buffer[len++] = msg->willMessageLength & 0xFF;        /* Will Message Length LSB */
        memcpy(buffer + len, msg->willMessage, msg->willMessageLength);
        len += msg->willMessageLength;
    }

    /* User Name */
    if (msg->userName)
    {
        buffer[len++] = (msg->userNameLength >> 8) & 0xFF; /* User Name Length MSB */
        buffer[len++] = msg->userNameLength & 0xFF;        /* User Name Length LSB */
        memcpy(buffer + len, msg->userName, msg->userNameLength);
        len += msg->userNameLength;
    }

    /* Password */
    if (msg->password)
    {
        buffer[len++] = (msg->passwordLength >> 8) & 0xFF; /* Password Length MSB */
        buffer[len++] = msg->passwordLength & 0xFF;        /* Password Length LSB */
        memcpy(buffer + len, msg->password, msg->passwordLength);
        len += msg->passwordLength;
    }

    *bufferLength = len;
    return 0;
}

ConnectMessage_t *decode_connect_message(const uint8_t *buffer, uint32_t bufferLength)
{
    ConnectMessage_t *msg = malloc(sizeof(ConnectMessage_t));
    if (msg == NULL)
    {
        // Handle memory allocation failure
        return NULL;
    }

    uint32_t len = 0;

    /* Decode fixed header */
    msg->base.type = buffer[len++];
    msg->base.flags = buffer[len++];

    /* Decode remaining length */
    msg->base.remainingLength = 0;
    uint32_t multiplier = 1;
    uint8_t encodedByte;
    do
    {
        encodedByte = buffer[len++];
        msg->base.remainingLength += (encodedByte & 0x7F) * multiplier;
        multiplier *= 128;
    } while ((encodedByte & 0x80) != 0 && len < bufferLength);

    if (len >= bufferLength)
    {
        // Invalid remaining length encoding
        free_connect_message(msg);
        return NULL;
    }

    /* Decode variable header and payload */
    uint16_t protocolNameLength = (buffer[len++] << 8) | buffer[len++]; /* Protocol Name Length */
    len += protocolNameLength;                                          /* Skip Protocol Name */

    uint8_t protocolLevel = buffer[len++];                 /* Protocol Level */
    msg->connectFlags = buffer[len++];                     /* Connect Flags */
    msg->keepAlive = (buffer[len++] << 8) | buffer[len++]; /* Keep Alive */

    /* Client ID */
    uint16_t clientIdLength = (buffer[len++] << 8) | buffer[len++]; /* Client ID Length */
    if (clientIdLength > MAX_CLIENT_ID_LENGTH)
    {
        // Handle client ID length error
        free_connect_message(msg);
        return NULL;
    }
    memcpy(msg->clientId, buffer + len, clientIdLength);
    msg->clientId[clientIdLength] = '\0';
    len += clientIdLength;

    /* Will Topic */
    if (msg->connectFlags & WILL_FLAG)
    {
        msg->willTopicLength = (buffer[len++] << 8) | buffer[len++]; /* Will Topic Length */
        msg->willTopic = malloc(msg->willTopicLength + 1);
        if (msg->willTopic == NULL)
        {
            // Handle memory allocation failure
            free_connect_message(msg);
            return NULL;
        }
        memcpy(msg->willTopic, buffer + len, msg->willTopicLength);
        msg->willTopic[msg->willTopicLength] = '\0';
        len += msg->willTopicLength;
    }
    else
    {
        msg->willTopicLength = 0;
        msg->willTopic = NULL;
    }

    /* Will Message */
    if (msg->connectFlags & WILL_FLAG)
    {
        msg->willMessageLength = (buffer[len++] << 8) | buffer[len++]; /* Will Message Length */
        msg->willMessage = malloc(msg->willMessageLength + 1);
        if (msg->willMessage == NULL)
        {
            // Handle memory allocation failure
            free(msg->willTopic);
            free_connect_message(msg);
            return NULL;
        }
        memcpy(msg->willMessage, buffer + len, msg->willMessageLength);
        msg->willMessage[msg->willMessageLength] = '\0';
        len += msg->willMessageLength;
    }
    else
    {
        msg->willMessageLength = 0;
        msg->willMessage = NULL;
    }

    /* User Name */
    if (msg->connectFlags & USER_NAME_FLAG)
    {
        msg->userNameLength = (buffer[len++] << 8) | buffer[len++]; /* User Name Length */

        // Check if the User Name Length is valid
        if (msg->userNameLength > MAX_USER_NAME_LENGTH)
        {
            // Handle invalid User Name length
            free_connect_message(msg);
            return NULL;
        }

        msg->userName = malloc(msg->userNameLength + 1);
        if (msg->userName == NULL)
        {
            // Handle memory allocation failure
            free(msg->willTopic);
            free(msg->willMessage);
            free_connect_message(msg);
            return NULL;
        }

        memcpy(msg->userName, buffer + len, msg->userNameLength);
        msg->userName[msg->userNameLength] = '\0';
        len += msg->userNameLength;
    }
    else
    {
        msg->userNameLength = 0;
        msg->userName = NULL;
    }

    /* Password */
    if (msg->connectFlags & PASSWORD_FLAG)
    {
        msg->passwordLength = (buffer[len++] << 8) | buffer[len++]; /* Password Length */
        msg->password = malloc(msg->passwordLength + 1);
        if (msg->password == NULL)
        {
            // Handle memory allocation failure
            free(msg->willTopic);
            free(msg->willMessage);
            free(msg->userName);
            free_connect_message(msg);
            return NULL;
        }
        memcpy(msg->password, buffer + len, msg->passwordLength);
        msg->password[msg->passwordLength] = '\0';
        len += msg->passwordLength;
    }
    else
    {
        msg->passwordLength = 0;
        msg->password = NULL;
    }

    return msg;
}

void free_connect_message(ConnectMessage_t *msg)
{
    free_base_message(&msg->base);
    free(msg->willTopic);
    free(msg->willMessage);
    free(msg->userName);
    free(msg->password);
    free(msg);
}