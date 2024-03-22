#include "message.h"
#include <stdlib.h>

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

void init_base_message(base_message_t *msg)
{
    msg->type = 0;
    msg->flags = 0;
    msg->remaining_length = 0;
    msg->payload = NULL;
}

void set_message_type(base_message_t *msg, uint8_t type)
{
    msg->type = type;
}

void set_message_flags(base_message_t *msg, uint8_t flags)
{
    msg->flags = flags;
}

void set_remaining_length(base_message_t *msg, uint32_t length)
{
    msg->remaining_length = length;
    encode_remaining_length(length, msg->remaining_length_bytes);
}

void set_payload(base_message_t *msg, uint8_t *payload, uint32_t payload_length)
{
    msg->payload = malloc(payload_length);
    if (msg->payload)
    {
        memcpy(msg->payload, payload, payload_length);
        set_remaining_length(msg, payload_length);
    }
}

void free_base_message(base_message_t *msg)
{
    if (msg->payload)
    {
        free(msg->payload);
    }
}