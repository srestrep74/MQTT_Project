#include "temp/encoders/utf8.h"
#include <stdlib.h>
#include <stdio.h>

void print_binary(uint8_t byte) {
    for (int i = 7; i >= 0; --i) {
        printf("%d", (byte >> i) & 1);
    }
}

int main() {

    //message connect 

    /*
    message *new_message = malloc(sizeof(message));

    set_fixed_header(new_message);
    set_variable_header(new_message);
    set_payload(new_message);

    unsigned char message_type = get_type(*(new_message->fixed_header));
    unsigned char flags = get_qos(*(new_message->fixed_header));

    printf("Tipo del mensaje: %d\n", message_type );
    printf("Tipo del mensaje: %s",(new_message->payload->connect_payload.user_name));
    */
    const char *string = "Hello, world!";
    EncodedString encoded_string; 

    encode_string(string, &encoded_string);

    printf("Byte 1 (MSB): 0x%02X - ", encoded_string.msb);
    print_binary(encoded_string.msb);
    printf("\n");

    printf("Byte 2 (LSB): 0x%02X - ", encoded_string.lsb);
    print_binary(encoded_string.lsb);
    printf("\n");

    printf("Datos de la cadena en UTF-8:\n");
    for (int i = 0; i < encoded_string.length; ++i) {
        printf("Byte %d: 0x%02X - ", i + 3, encoded_string.data[i]);
        print_binary(encoded_string.data[i]);
        printf("\n");
    }

    free(encoded_string.data);

    return 0;
}
