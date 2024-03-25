#include "fixed_header.h"

void encode(message *message)
{
    int total = calculate_total_length(message);
    uint8_t encoded_remaining_length[4] = {0};
    int encoded_length = encode_remaining_length(total, encoded_remaining_length);
    memcpy(message->fixed_header->remaining_length, encoded_remaining_length, encoded_length);
}

int decode(message *message)
{
    int encoded = 0;
    int encoded_length = decode_remaining_length(message->fixed_header->remaining_length, &encoded);
    return encoded;
}