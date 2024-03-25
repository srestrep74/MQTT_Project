#ifndef MESSAGE_H
#define MESSAGE_H

#include "../constants.h"
#include "../headers/fixed_header.h"
#include "../headers/variable_header.h"
#include "../headers/payload.h"

typedef struct message
{
    int8_t type;
    struct fixed_header *fixed_header;
    union variable_header *variable_header;
    union payload *payload;
    void (*set_fixed_header)(struct message *);
    void (*set_variable_header)(struct message *);
    void (*set_payload)(struct message *);
} message;

void initialize_message(message *msg);
void destroy_message(message *msg);
int calculate_total_length(message *msg);

#endif
