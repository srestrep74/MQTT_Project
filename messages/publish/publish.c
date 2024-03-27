#include "publish.h"
#include <stdlib.h>
#include <string.h>

void set_fixed_header_publish(message *message)
{
    message->fixed_header = malloc(sizeof(struct fixed_header));
    initialize_fixed_header(message->fixed_header);
    set_retain(message->fixed_header, 0);
    set_qos(message->fixed_header, QOS);
    set_dup(message->fixed_header, 0);
    set_type(message->fixed_header, PUBLISH);
}

void set_variable_header_publish(message *message)
{
    message->variable_header = malloc(sizeof(union variable_header));
    message->variable_header->publish_variable_h.publish_length_MSB = 0;
    message->variable_header->publish_variable_h.topic_name = "topic";
    message->variable_header->publish_variable_h.publish_length_LSB = sizeof(message->variable_header->publish_variable_h.topic_name);
    message->variable_header->publish_variable_h.message_id_MSB = 0;
    message->variable_header->publish_variable_h.message_id_LSB = 10;
}

void set_payload_publish(message *message)
{
    message->payload = malloc(sizeof(union payload));
    message->payload->publish_payload.data = "Hello";
}
