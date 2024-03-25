#include "connect.h"
#include <stdlib.h>
#include <string.h>

void set_fixed_header_connect(message *message)
{
    message->fixed_header = malloc(sizeof(struct fixed_header));
    initialize_fixed_header(message->fixed_header);
    set_retain(message->fixed_header, 0);
    set_qos(message->fixed_header, QOS);
    set_dup(message->fixed_header, 0);
    set_type(message->fixed_header, CONNECT);
}

void set_variable_header_connect(message *message)
{
    message->variable_header = malloc(sizeof(union variable_header));
    message->variable_header->connect_variable_h.connect_length_MSB = 0;
    message->variable_header->connect_variable_h.connect_length_LSB = 6;
    strcpy(message->variable_header->connect_variable_h.protocol_name, PROTOCOL_NAME);
    message->variable_header->connect_variable_h.version = VERSION;

    int user = 1 << 7;
    int pas = 1 << 6;
    int qos = 1 << 3;
    int flag = 1 << 2;

    message->variable_header->connect_variable_h.flags = 0;
    message->variable_header->connect_variable_h.flags |= user;
    message->variable_header->connect_variable_h.flags |= pas;
    message->variable_header->connect_variable_h.flags |= qos;
    message->variable_header->connect_variable_h.flags |= flag;

    message->variable_header->connect_variable_h.keep_alive_MSB = 0;
    message->variable_header->connect_variable_h.keep_alive_LSB = 10;
}

void set_payload_connect(message *message)
{
    message->payload = malloc(sizeof(union payload));
    message->payload->connect_payload.user_name = "username";
    message->payload->connect_payload.password = "password";
    message->payload->connect_payload.client_identifier = "123";
    message->payload->connect_payload.will_message = "Hola mundo";
    message->payload->connect_payload.will_topic = "test";
}
