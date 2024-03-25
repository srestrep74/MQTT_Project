#ifndef UTF8_H
#define UTF8_H

#include <stdint.h>

typedef struct
{
    uint8_t msb;
    uint8_t lsb;
    uint8_t *data;
} utf8_string;

void encode_string(const char *string, utf8_string *encoded_string);
void decode_string(const utf8_string *encoded_string, char **decoded_string);
int length_of_utf8_string(const char *string);

#endif