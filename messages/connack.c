#include "connack.h"
#include <stdlib.h>
#include <string.h>

void set_fixed_header_connack(message *message)
{
    message->fixed_header = malloc(sizeof(struct fixed_header));
    initialize_fixed_header(message->fixed_header);
    set_retain(message->fixed_header, 0);
    set_qos(message->fixed_header, 0);
    set_dup(message->fixed_header, 0);
    set_type(message->fixed_header, CONNACK);
}

void set_variable_header_connack(message *message)
{
    message->variable_header = malloc(sizeof(union variable_header));
    message->variable_header->connack_variable_h.session_present = 0;
    message->variable_header->connack_variable_h.return_code = MQTT_CONN_ACCEPTED;
}

void set_payload_connack(message *message)
{
    message->payload = NULL;
}
