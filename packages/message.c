#include "message.h"
#include <stdlib.h>
#include <string.h>

void encode_remaining_length(uint32_t length, uint8_t *buffer)
{
    uint8_t byte;
    int i = 0;
    do
    {
        byte = length % 128;
        length /= 128;
        if (length > 0)
        {
            byte |= 0x80;
        }
        buffer[i++] = byte;
    } while (length > 0 && i < MAX_REMAINING_LENGTH_BYTES);
}

uint32_t get_remaining_length_bytes(uint32_t length)
{
    uint32_t bytes = 0;
    do
    {
        length /= 128;
        bytes++;
    } while (length > 0);
    return bytes;
}

void init_base_message(BaseMessage_t *msg)
{
    msg->type = 0;
    msg->flags = 0;
    msg->remainingLength = 0;
    msg->payload = NULL;
    msg->payloadLength = 0;
}

void set_message_type(BaseMessage_t *msg, uint8_t type)
{
    msg->type = type;
}

void set_message_flags(BaseMessage_t *msg, uint8_t flags)
{
    msg->flags = flags;
}

int set_payload(BaseMessage_t *msg, const uint8_t *payload, uint32_t payloadLength)
{
    if (msg->payload)
    {
        free(msg->payload);
    }

    msg->payload = malloc(payloadLength);
    if (msg->payload == NULL)
    {
        return -1; // Error: failed to allocate memory
    }

    memcpy(msg->payload, payload, payloadLength);
    msg->payloadLength = payloadLength;
    msg->remainingLength = payloadLength;
    encode_remaining_length(payloadLength, msg->remainingLengthBytes);

    return 0;
}

void free_base_message(BaseMessage_t *msg)
{
    if (msg->payload)
    {
        free(msg->payload);
    }
}