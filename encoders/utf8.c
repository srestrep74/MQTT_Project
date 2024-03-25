#include "utf8.h"
#include <stdlib.h>
#include <string.h>

int length_of_utf8_string(const char *string)
{
    int length = 0;
    while (*string)
    {
        if ((*string & 0xC0) != 0x80)
        {
            length++;
        }
        string++;
    }
    return length;
}

void encode_string(const char *string, utf8_string *encoded_string)
{

    int string_length = length_of_utf8_string(string);

    encoded_string->msb = (uint8_t)(string_length >> 8);
    encoded_string->lsb = (uint8_t)(string_length & 0xFF);

    encoded_string->data = (uint8_t *)malloc(string_length + 1);

    strcpy((char *)encoded_string->data, string);
}

void decode_string(const utf8_string *encoded_string, char **decoded_string)
{
    int string_length = (encoded_string->msb << 8) | encoded_string->lsb;

    *decoded_string = (char *)malloc(string_length + 1);
    memcpy(*decoded_string, encoded_string->data, string_length);
    (*decoded_string)[string_length] = '\0';
}