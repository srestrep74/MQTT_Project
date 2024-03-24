#include "temp/message.h"

int main()
{

    struct message *new_message;

    set_fixed_header(new_message);
    set_variable_header(new_message);
    set_payload(new_message);

    printf("%d\n", new_message->fixed_header->byte);

    return 0;
}