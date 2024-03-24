#include "temp/message.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

    message *new_message = malloc(sizeof(message));

    set_fixed_header(new_message);
    set_variable_header(new_message);
    set_payload(new_message);

    unsigned char message_type = get_type(*(new_message->fixed_header));
    unsigned char flags = get_qos(*(new_message->fixed_header));

    printf("Tipo del mensaje: %d\n", message_type );
    printf("Tipo del mensaje: %s",(new_message->payload->connect_payload.user_name));
    

    return 0;
}
