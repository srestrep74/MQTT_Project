#include "../../include/responses/connect.h"

Packet create_connack_message(uint8_t return_code) {
    Packet connack;

    set_type(&(connack.fixed_header), CONNACK);

    set_remaining_length(&(connack.remaining_length), 2);

    connack.variable_header = malloc(2);
    if (connack.variable_header == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el encabezado variable del CONNACK.\n");
        exit(EXIT_FAILURE);
    }

    connack.variable_header[0] = 0x00;
    connack.variable_header[1] = return_code;

    return connack;
}