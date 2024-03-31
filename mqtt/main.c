#include "include/packet/packet.h"

#include <stdio.h>

int main()
{
    const char *id = "123";
    Packet connect = create_connect_message(id);
    printf("%d\n", connect.fixed_header);
    printf("%d\n", connect.remaining_length);
    printf("%d\n", connect.variable_header[9]);
    printf("%s\n", connect.payload);

    Packet pub = create_publish_message("a/b", "data1");

    return 0;
}