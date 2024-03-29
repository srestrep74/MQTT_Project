#include "subscribe.h"
#include <stdlib.h>
#include <string.h>

void set_fixed_header_subscribe(message *message)
{
    message->fixed_header = malloc(sizeof(struct fixed_header));
    initialize_fixed_header(message->fixed_header);
    set_retain(message->fixed_header, 0);
    set_qos(message->fixed_header, QOS);
    set_dup(message->fixed_header, 0);
    set_type(message->fixed_header, SUBSCRIBE);
}

void set_variable_header_subscribe(message *message)
{
    message->variable_header = malloc(sizeof(union variable_header));
    message->variable_header->subscribe_variable_h.subscribe_message_id_MSB = 0;
    message->variable_header->subscribe_variable_h.subscribe_message_id_LSB = 10;
}

struct subscribe create_subscription(char *topic, int8_t qos)
{
    struct subscribe new_sub;
    new_sub.subscribe_length_MSB = 0;
    new_sub.subscribe_length_LSB = strlen(topic);
    new_sub.subscribe_topic_name = topic;
    new_sub.requested_qos = qos;
    return new_sub;
}

void set_payload_subscribe(message *message)
{
    message->payload = malloc(sizeof(union payload));
}

void set_topics(message *message, char **topics, int num_topics)
{
    struct subscribe_list_payload payload;
    payload.subscribe_size = num_topics;
    payload.subscribe_list = malloc(payload.subscribe_size * sizeof(struct subscribe));
    for (int i = 0; i < num_topics; i++)
    {
        payload.subscribe_list[i] = create_subscription(topics[i], 0);
    }
    message->payload->sub_payload = payload;
}
