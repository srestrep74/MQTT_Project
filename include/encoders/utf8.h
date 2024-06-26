#ifndef UTF8_H
#define UTF8_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function definitions
uint8_t *utf8_encode(const char *);
char *utf8_decode(const uint8_t *);

#endif