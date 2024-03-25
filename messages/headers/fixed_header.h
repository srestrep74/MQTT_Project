#ifndef FIXED_HEADER_H
#define FIXED_HEADER_H

#include <stdint.h>

#define RETAIN_MASK 0x01
#define QOS_MASK 0x06
#define DUP_MASK 0x08
#define TYPE_MASK 0xF0

struct fixed_header
{
    int8_t byte;
    int8_t remaining_length[4];
};

void initialize_fixed_header(struct fixed_header *header);

// Getter functions
unsigned char get_retain(struct fixed_header header);
unsigned char get_qos(struct fixed_header header);
unsigned char get_dup(struct fixed_header header);
unsigned char get_type(struct fixed_header header);

// Setter functions
void set_retain(struct fixed_header *header, unsigned char value);
void set_qos(struct fixed_header *header, unsigned char value);
void set_dup(struct fixed_header *header, unsigned char value);
void set_type(struct fixed_header *header, unsigned char value);

int encode_remaining_length(int8_t remaining_length, uint8_t *encoded_data);
int decode_remaining_length(uint8_t *encoded_data, int *decoded_length);

#endif
