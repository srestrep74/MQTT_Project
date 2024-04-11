#include "../../include/encoders/utf8.h"

// Function to encode a UTF-8 string
uint8_t *utf8_encode(const char *input)
{
    size_t len = strlen(input);
    size_t total_length = len + 2;
    uint8_t *output = (uint8_t *)malloc(total_length + 1); 

    if (output == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        return NULL;
    }

    output[0] = (len >> 8) & 0xFF;
    output[1] = len & 0xFF;
    memcpy(output + 2, input, len);
    output[total_length] = '\0'; 

    return output;
}

// Function to decode a UTF-8 string
char *utf8_decode(const uint8_t *input)
{
    size_t len = (input[0] << 8) | input[1];

    char *output = (char *)malloc(len + 1);
    if (output == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria\n");
        return NULL;
    }

    memcpy(output, input + 2, len + 1);
    output[len] = '\0';

    return output;
}