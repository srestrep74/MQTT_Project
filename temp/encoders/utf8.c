#include "utf8.h"

#include <stdlib.h>
#include <string.h>

void encode_string(const char *string, EncodedString *encoded_string) {
    encoded_string->length = strlen(string);

    encoded_string->msb = (uint8_t)(encoded_string->length >> 8);
    encoded_string->lsb = (uint8_t)(encoded_string->length & 0xFF);

    encoded_string->data = (uint8_t *)malloc(encoded_string->length + 1);

    strcpy((char *)encoded_string->data, string);
}