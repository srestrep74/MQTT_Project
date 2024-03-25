#include "messages/base/message.h"
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

    message connack_msg;
    connack_msg.type = CONNACK;
    initialize_message(&connack_msg);
    connack_msg.set_fixed_header(&connack_msg);
    connack_msg.set_variable_header(&connack_msg);
    connack_msg.set_payload(&connack_msg);

    message pub_msg;
    pub_msg.type = PUBLISH;
    initialize_message(&pub_msg);
    pub_msg.set_fixed_header(&pub_msg);
    pub_msg.set_variable_header(&pub_msg);
    pub_msg.set_payload(&pub_msg);

    printf("%s\n", pub_msg.payload->publish_payload.data);

    return 0;
}