#ifndef MESSAGE_BUILDER_H
#define MESSAGE_BUILDER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "message.h"

typedef struct builder builder;

struct builder
{
    void (*reset)(builder *self);
    void (*set_fixed_header)(builder *builder, struct fixed_header *fixed_header);
    void (*set_variable_header)(builder *builder, union variable_header *variable_header);
    void (*set_payload)(builder *builder, union payload *payload);
    message *(*get_message)(builder *self);
};

/*struct message
{
    struct fixed_header *fixed_header;
    union variable_header *variable_header;
    union payload *payload;
};

void initialize_message(struct message *message);
void set_fixed_header(struct message *message);
void set_variable_header(struct message *message);
void set_payload(struct message *message); */

#endif