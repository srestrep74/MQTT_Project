#include "../../include/packet/packet.h"

void set_type(u_int8_t *fixed_header, u_int8_t type)
{
    *fixed_header = type;
}

void set_remaining_length(uint8_t *remaining_length, size_t packet_length)
{
    *remaining_length = packet_length;
}

void set_clean_session_flag(uint8_t *connect_flags)
{
    *connect_flags |= 1 << 1;
}

Packet create_connect_message(const char *id)
{
    Packet connect;
    set_type(&(connect.fixed_header), CONNECT);
    size_t packet_length = 12 + strlen(id);
    set_remaining_length(&(connect.remaining_length), packet_length);

    connect.variable_header = malloc(packet_length);
    connect.variable_header[0] = 0x00;
    connect.variable_header[1] = 0x06;
    connect.variable_header[2] = 'M';
    connect.variable_header[3] = 'Q';
    connect.variable_header[4] = 'I';
    connect.variable_header[5] = 's';
    connect.variable_header[6] = 'd';
    connect.variable_header[7] = 'p';
    connect.variable_header[8] = PROTOCOL_VERSION;
    set_clean_session_flag(&(connect.variable_header[9]));
    connect.variable_header[10] = 0x00;
    connect.variable_header[11] = 0x3C;

    connect.payload = malloc(strlen(id));
    memcpy(connect.payload, id, strlen(id));

    return connect;
}

Packet create_publish_message(const char *topic, const char *data)
{
    Packet publish;
    set_type(&(publish.fixed_header), PUBLISH);
    size_t topic_length = strlen(topic);
    size_t data_length = strlen(data);
    size_t packet_length = 2 + data_length + topic_length;
    set_remaining_length(&(publish.remaining_length), packet_length);

    publish.variable_header = malloc(packet_length);
    publish.variable_header[0] = 0x00;
    publish.variable_header[1] = topic_length;
    memcpy(&publish.variable_header[2], topic, topic_length);

    publish.payload = malloc(data_length);
    memcpy(publish.payload, data, data_length);

    return publish;
}