#include "messages/message.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{

    message connect_msg;
    connect_msg.type = CONNECT;
    initialize_message(&connect_msg);
    connect_msg.set_fixed_header(&connect_msg);
    connect_msg.set_variable_header(&connect_msg);
    connect_msg.set_payload(&connect_msg);

    printf("%d\n", get_type(*connect_msg.fixed_header));

    message connack_msg;
    connack_msg.type = CONNACK;
    initialize_message(&connack_msg);
    connack_msg.set_fixed_header(&connack_msg);
    connack_msg.set_variable_header(&connack_msg);
    connack_msg.set_payload(&connack_msg);

    printf("%d\n", get_type(*connack_msg.fixed_header));

    return 0;
}