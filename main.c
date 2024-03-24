#include "temp/message.h"

int main()
{

    message *new_message = malloc(sizeof(message));

    set_fixed_header(new_message);
    set_variable_header(new_message);
    set_payload(new_message);

    return 0;
}