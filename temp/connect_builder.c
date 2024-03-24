#include "connect_builder.h"

/*void initialize_connect_builder(struct connect_builder *builder)
{
}

void set_connect_fixed_header(struct connect_builder *builder)
{
}

void set_connect_variable_header(struct connect_builder *builder, const char *client_identifier, const char *will_topic, const char *will_message, const char *user_name, const char *password)
{
}

void set_connect_payload(struct connect_builder *builder, const char *client_identifier, const char *will_topic, const char *will_message, const char *user_name, const char *password)
{
} */

builder *new_connect_builder(void)
{
    connect_builder *connect_builder = malloc(sizeof(*connect_builder));
    connect_builder->builder.set_fixed_header = (void (*)(builder *))set_fixed_header;
    connect_builder->builder.set_variable_header = (void (*)(builder *))set_variable_header;
    connect_builder->builder.set_payload = (void (*)(builder *))set_payload;
    connect_builder->builder.reset = (void (*)(builder *))reset;
    return &connect_builder->builder;
}

void set_fixed_header(connect_builder *self, struct fixed_header *fixed_header)
{
    self->message.fixed_header = fixed_header;
}

void set_variable_header(connect_builder *self, union variable_header *variable_header)
{
    self->message.variable_header = variable_header;
}

void set_payload(connect_builder *self, union payload *payload)
{
    self->message.payload = payload;
}

void reset(connect_builder *self)
{
    self->message.fixed_header = NULL;
    self->message.variable_header = NULL;
    self->message.payload = NULL;
}

void get_message(connect_builder *self)
{
    message *message = &self->message;
    reset(self);
    return message;
}
