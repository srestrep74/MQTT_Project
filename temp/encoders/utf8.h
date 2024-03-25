#ifndef UTF8_H
#define UTF8_H

#include <stdint.h>

typedef struct {
    uint8_t msb; 
    uint8_t lsb; 
    uint8_t *data; 
    uint16_t length;
} EncodedString;

void encode_string(const char *string, EncodedString *encoded_string);

#endif
