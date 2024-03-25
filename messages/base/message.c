#include <stddef.h>
#include "message.h"
#include "../connect/connect.h"
#include "../connack/connack.h"
#include "../publish/publish.h"

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
