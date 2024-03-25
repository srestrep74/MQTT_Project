#include "message.h"
#include "connect.h"
#include "connack.h"

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
        // Agrega más casos según sea necesario
    }
}
