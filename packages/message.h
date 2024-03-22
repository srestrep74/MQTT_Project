#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_REMAINING_LENGTH_BYTES 4

typedef struct
{
    uint8_t type;
    uint8_t flags;
    uint32_t remaining_length;
    uint8_t remaining_length_bytes[MAX_REMAINING_LENGTH_BYTES];
    uint8_t *payload;
} base_message_t;

void init_base_message(base_message_t *msg);
void set_message_type(base_message_t *msg, uint8_t type);
void set_message_flags(base_message_t *msg, uint8_t flags);
void set_remaining_length(base_message_t *msg, uint32_t length);
void set_payload(base_message_t *msg, uint8_t *payload, uint32_t payload_length);
void free_base_message(base_message_t *msg);

#endif