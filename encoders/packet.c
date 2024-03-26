#include "packet.h"

size_t serialize_message(const message *msg, char *buffer, size_t buffer_size)
{
    size_t offset = 0;

    // Copiar el tipo de mensaje
    memcpy(buffer + offset, &msg->type, sizeof(int8_t));
    offset += sizeof(int8_t);

    // Copiar el fixed header
    memcpy(buffer + offset, &msg->fixed_header, sizeof(msg->fixed_header));
    offset += sizeof(msg->fixed_header);

    // Copiar el variable header
    memcpy(buffer + offset, &msg->variable_header, sizeof(msg->variable_header));
    offset += sizeof(msg->variable_header);

    // Copiar el payload
    memcpy(buffer + offset, &msg->payload, sizeof(msg->payload));
    offset += sizeof(msg->payload);

    return offset;
}

void deserialize_message(const char *buffer, size_t buffer_size, message *msg)
{
    size_t offset = 0;

    // Leer el tipo de mensaje
    memcpy(&msg->type, buffer + offset, sizeof(int8_t));
    offset += sizeof(int8_t);

    // Leer el fixed header
    memcpy(&msg->fixed_header, buffer + offset, sizeof(msg->fixed_header));
    offset += sizeof(msg->fixed_header);

    // Leer el variable header
    memcpy(&msg->variable_header, buffer + offset, sizeof(msg->variable_header));
    offset += sizeof(msg->variable_header);

    // Leer el payload
    memcpy(&msg->payload, buffer + offset, sizeof(msg->payload));
    offset += sizeof(msg->payload);
}