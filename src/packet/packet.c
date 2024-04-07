#include "../../include/packet/packet.h"

uint8_t get_type(uint8_t *fixed_header) {
    return (*fixed_header >> 4) & 0b00001111;
}

void set_remaining_length(uint8_t *remaining_length, size_t packet_length)
{
    *remaining_length = packet_length;
}

void set_clean_session_flag(uint8_t *connect_flags)
{
    *connect_flags |= 1 << 1;
}

char *get_topic(Packet *publish)
{
    size_t topic_length = publish->variable_header[1];
    char *topic = malloc(topic_length + 1);
    memcpy(topic, publish->variable_header + 2, topic_length);
    topic[topic_length] = '\0';
    return topic;
}

Packet create_connect_message()
{
    Packet connect;

    char *client_id = generate_client_id();
    uint8_t *id = utf8_encode(client_id);
    size_t id_length = strlen(client_id);

    set_type(&(connect.fixed_header), CONNECT);
    size_t packet_length = 12 + id_length;
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

    connect.payload = malloc(id_length + 1);
    memcpy(connect.payload, id, id_length + 1);
    free(id);

    return connect;
}

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

Packet create_publish_message(const char *topic, const char *data)
{
    Packet publish;
    set_type(&(publish.fixed_header), PUBLISH);
    set_qos(&publish.fixed_header, 0);

    uint8_t *topic_encoded = utf8_encode(topic);
    uint8_t *data_encoded = utf8_encode(data);

    size_t topic_length = strlen(topic) + 2;
    size_t data_length = strlen(data) + 2;

    size_t packet_length = 2 + data_length + topic_length;
    set_remaining_length(&(publish.remaining_length), packet_length);

    publish.variable_header = malloc(packet_length);
    publish.variable_header[0] = 0x00;
    publish.variable_header[1] = topic_length;
    memcpy(&publish.variable_header[2], topic_encoded, packet_length);

    publish.payload = malloc(data_length + 1);
    memcpy(publish.payload, data_encoded, data_length + 1);

    return publish;
}

Packet create_subscribe_message(const char *topic)
{
    Packet subscribe;

    uint16_t id = get_packet_id();
    set_type(&(subscribe.fixed_header), SUBSCRIBE);
    set_qos(&(subscribe.fixed_header), 1);
    
    size_t id_length = sizeof(id); 

    subscribe.variable_header = malloc(id_length);
    
    memcpy(subscribe.variable_header, &id, id_length);


    uint8_t *topic_encoded = utf8_encode(topic);

    size_t topic_length = strlen(topic) + 2;

    size_t packet_length = 2 + topic_length;
    set_remaining_length(&(subscribe.remaining_length), packet_length);

    subscribe.payload = malloc(topic_length + 1);
    memcpy(subscribe.payload, topic_encoded, topic_length + 1);

    return subscribe;

}

void set_qos(uint8_t *fixed_header, int qos)
{
    *fixed_header &= ~(0b11 << 1);

    switch (qos)
    {
    case 0:
        break;
    case 1:
        *fixed_header |= (1 << 1);
        break;
    case 2:
        *fixed_header |= (2 << 1);
        break;
    default:
        printf("Qos not valid \n");
        break;
    }
}

void set_type(u_int8_t *fixed_header, u_int8_t type)
{
    *fixed_header &= ~(0b1111 << 4);
    *fixed_header |= (type << 4) & (0b1111 << 4);
}

uint8_t get_qos(uint8_t *fixed_header) {
    return (*fixed_header >> 1) & 0b11; 
}