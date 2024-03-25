#include "messages/base/message.h"
#include "encoders/fixed_header.h"
#include "encoders/utf8.h"
#include <stdlib.h>
#include <stdio.h>

void print_encoded_remaining_length(const uint8_t *encoded_data, int encoded_length)
{
    printf("Encoded remaining length: ");
    for (int i = 0; i < encoded_length; i++)
    {
        for (int j = 7; j >= 0; j--)
            printf("%d ", (encoded_data[i] >> j) & 1);

        printf(" ");
    }
    printf("\n");
}

int main()
{
    message connect_msg;
    connect_msg.type = CONNECT;
    initialize_message(&connect_msg);
    connect_msg.set_fixed_header(&connect_msg);
    connect_msg.set_variable_header(&connect_msg);
    connect_msg.set_payload(&connect_msg);
    encode(&connect_msg);

    return 0;
}