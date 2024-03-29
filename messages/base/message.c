#include <stddef.h>
#include "message.h"
#include "../connect/connect.h"
#include "../connack/connack.h"
#include "../publish/publish.h"
#include "../subscribe/subscribe.h"

void initialize_message(message *msg)
{
    switch (msg->type)
    {
    case CONNECT:
        msg->set_fixed_header = set_fixed_header_connect;
        msg->set_variable_header = set_variable_header_connect;
        msg->set_payload = set_payload_connect;
        break;
    case CONNACK:
        msg->set_fixed_header = set_fixed_header_connack;
        msg->set_variable_header = set_variable_header_connack;
        msg->set_payload = set_payload_connack;
        break;
    case PUBLISH:
        msg->set_fixed_header = set_fixed_header_publish;
        msg->set_variable_header = set_variable_header_publish;
        msg->set_payload = set_payload_publish;
        msg->set_topic = set_topic;
        msg->set_data = set_data;
        break;
    case SUBSCRIBE:
        msg->set_fixed_header = set_fixed_header_subscribe;
        msg->set_variable_header = set_variable_header_subscribe;
        msg->set_payload = set_payload_subscribe;
        msg->set_topics = set_topics;
        break;
    }
}

void destroy_message(message *msg)
{
    if (msg != NULL)
    {
        free(msg->fixed_header);
        free(msg->variable_header);
        free(msg->payload);
    }
}

int calculate_total_length(message *msg)
{
    int total = 0;
    total += sizeof(msg->variable_header->connect_variable_h.connect_length_MSB) +
             sizeof(msg->variable_header->connect_variable_h.connect_length_LSB) +
             sizeof(msg->variable_header->connect_variable_h.protocol_name) +
             sizeof(msg->variable_header->connect_variable_h.version) +
             sizeof(msg->variable_header->connect_variable_h.flags) +
             sizeof(msg->variable_header->connect_variable_h.keep_alive_MSB) +
             sizeof(msg->variable_header->connect_variable_h.keep_alive_LSB);

    total += sizeof(msg->payload->connect_payload.client_identifier) +
             sizeof(msg->payload->connect_payload.password) + sizeof(msg->payload->connect_payload.user_name) +
             sizeof(msg->payload->connect_payload.will_message) + sizeof(msg->payload->connect_payload.will_topic);

    return total;
}
