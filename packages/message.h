#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_REMAINING_LENGTH_BYTES 4

typedef struct
{
    uint8_t type;
    uint8_t flags;
    uint32_t remainingLength;
    uint8_t remainingLengthBytes[MAX_REMAINING_LENGTH_BYTES];
    uint8_t *payload;
    uint32_t payloadLength;
} BaseMessage_t;

void init_base_message(BaseMessage_t *msg);
void set_message_type(BaseMessage_t *msg, uint8_t type);
void set_message_flags(BaseMessage_t *msg, uint8_t flags);
int set_payload(BaseMessage_t *msg, const uint8_t *payload, uint32_t payloadLength);
void encode_remaining_length(uint32_t length, uint8_t *buffer);
uint32_t get_remaining_length_bytes(uint32_t length);
void free_base_message(BaseMessage_t *msg);

#endif