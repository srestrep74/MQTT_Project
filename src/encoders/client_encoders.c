#include "../../include/encoders/client_encoders.h"

// Function to encode a message for the client
unsigned char *encode_message_client(Packet packet, size_t total_size)
{
    unsigned char *buffer = malloc(total_size);
    size_t offset = 0;

    memcpy(buffer + offset, &(packet.fixed_header), sizeof(packet.fixed_header));
    offset += sizeof(packet.fixed_header);

    memcpy(buffer + offset, &(packet.remaining_length), sizeof(packet.remaining_length));
    offset += sizeof(packet.remaining_length);

    if (packet.variable_header)
    {
        memcpy(buffer + offset, packet.variable_header, packet.remaining_length);
        offset += packet.remaining_length;
    }

    if (packet.payload)
    {
        memcpy(buffer + offset, packet.payload, packet.remaining_length);
        offset += sizeof(packet.payload);
    }

    return buffer;
    
}

// Function to decode a message from the client
Packet decode_message_client(int client_socket)
{
    Packet packet = {0};
    unsigned char buffer[10000];

    ssize_t data = read(client_socket, buffer, sizeof(buffer));

    if (data == -1)
        return packet;

    size_t offset = 0;

    memcpy(&(packet.fixed_header), buffer + offset, sizeof(packet.fixed_header));
    offset += sizeof(packet.fixed_header);

    memcpy(&(packet.remaining_length), buffer + offset, sizeof(packet.remaining_length));
    offset += sizeof(packet.remaining_length);

    packet.variable_header = malloc(packet.variable_header);
    memcpy(packet.variable_header, buffer + offset, packet.remaining_length);
    offset += packet.remaining_length;

    size_t payload_size = data - offset;
    packet.payload = malloc(sizeof(packet.payload));
    memcpy(packet.payload, buffer + offset, sizeof(packet.payload));

    return packet;
}