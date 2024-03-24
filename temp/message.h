#ifndef MESSAGE_H
#define MESSAGE_H

#include "constants.h"
#include "fixed_header.h"
#include "variable_header.h"
#include "payload.h"

typedef struct message
{
    struct fixed_header *fixed_header;
    union variable_header *variable_header;
    union payload *payload;
} message;

void set_fixed_header(message *message);
void set_variable_header(message *message);
void set_payload(message *message);

#endif
