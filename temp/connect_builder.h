#ifndef CONNECT_BUILDER_H
#define CONNECT_BUILDER_H

#include "message_builder.h"
#include "message.h"

/*struct connect_builder
{
    struct message *message;
};

void initialize_connect_builder(struct connect_builder *builder);
void set_connect_fixed_header(struct connect_builder *builder);
void set_connect_variable_header(struct connect_builder *builder, const char *client_identifier, const char *will_topic, const char *will_message, const char *user_name, const char *password);
void set_connect_payload(struct connect_builder *builder, const char *client_identifier, const char *will_topic, const char *will_message, const char *user_name, const char *password); */

typedef struct connect_builder
{
    builder builder;
    message message;
} connect_builder;

void set_fixed_header(connect_builder *self, struct fixed_header *fixed_header);
void set_variable_header(connect_builder *self, union variable_header *variable_header);
void set_payload(connect_builder *self, union payload *payload);
void reset(connect_builder *self);
void get_message(connect_builder *self);

#endif