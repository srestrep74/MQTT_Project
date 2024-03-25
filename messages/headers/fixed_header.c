#include "fixed_header.h"

void initialize_fixed_header(struct fixed_header *header)
{
    header->byte = 0;
    header->remaining_length = 0;
}

unsigned char get_retain(struct fixed_header header)
{
    return header.byte & RETAIN_MASK;
}

unsigned char get_qos(struct fixed_header header)
{
    return (header.byte & QOS_MASK) >> 1;
}

unsigned char get_dup(struct fixed_header header)
{
    return (header.byte & DUP_MASK) >> 3;
}

unsigned char get_type(struct fixed_header header)
{
    return (header.byte & TYPE_MASK) >> 4;
}

// Setter functions
void set_retain(struct fixed_header *header, unsigned char value)
{
    header->byte = (header->byte & ~RETAIN_MASK) | (value & RETAIN_MASK);
}

void set_qos(struct fixed_header *header, unsigned char value)
{
    header->byte = (header->byte & ~QOS_MASK) | ((value << 1) & QOS_MASK);
}

void set_dup(struct fixed_header *header, unsigned char value)
{
    header->byte = (header->byte & ~DUP_MASK) | ((value << 3) & DUP_MASK);
}

void set_type(struct fixed_header *header, unsigned char value)
{
    header->byte = (header->byte & ~TYPE_MASK) | ((value << 4) & TYPE_MASK);
}

int encode_remaining_length(int remaining_length, uint8_t *encoded_data)
{
    int i = 0;
    do
    {
        uint8_t encoded_byte = remaining_length % 128;
        remaining_length /= 128;
        if (remaining_length > 0)
        {
            encoded_byte |= 0x80;
        }
        encoded_data[i++] = encoded_byte;
    } while (remaining_length > 0);
    return i;
}

int decode_remaining_length(const uint8_t *encoded_data, int *decoded_length)
{
    int multiplier = 1;
    *decoded_length = 0;
    for (int i = 0; i < 4; i++)
    {
        *decoded_length += (encoded_data[i] & 127) * multiplier;
        if ((encoded_data[i] & 128) == 0)
        {
            break;
        }
        multiplier *= 128;
    }
    return 1;
}