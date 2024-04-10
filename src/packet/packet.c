#include "../../include/packet/packet.h"

// Function to get the type from the fixed header
uint8_t get_type(uint8_t *fixed_header)
{
    return (*fixed_header >> 4) & 0b00001111;
}

// Function to set the remaining length in the packet
void set_remaining_length(uint8_t *remaining_length, size_t packet_length)
{
    *remaining_length = packet_length;
}

// Function to set the clean session flag in the connect flags
void set_clean_session_flag(uint8_t *connect_flags)
{
    *connect_flags |= 1 << 1;
}

// Function to get the topic from the publish packet
char *get_topic(Packet *publish)
{
    size_t topic_length = publish->variable_header[1];
    char *topic = malloc(topic_length + 1);
    memcpy(topic, publish->variable_header + 2, topic_length);
    topic[topic_length] = '\0';
    return topic;
}

// Function to create a connect message packet
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

// Function to create a connack message packet
Packet create_connack_message(uint8_t return_code)
{
    Packet connack;

    set_type(&(connack.fixed_header), CONNACK);

    set_remaining_length(&(connack.remaining_length), 2);

    connack.variable_header = malloc(2);
    if (connack.variable_header == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for CONNACK variable header.\n");
        exit(EXIT_FAILURE);
    }

    connack.variable_header[0] = 0x00;
    connack.variable_header[1] = return_code;

    return connack;
}

// Function to create a publish message packet
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

// Function to create a disconnect message packet
Packet create_disconnect_message()
{
    Packet disconnect;
    set_type(&(disconnect.fixed_header), DISCONNECT);
    set_qos(&disconnect.fixed_header, 0);

    disconnect.variable_header = NULL;
    disconnect.payload = NULL;

    return disconnect;
}

// Function to create a subscribe message packet
Packet create_subscribe_message(char **topics, int num_topics)
{
    Packet sub;
    printf("%d\n", num_topics);
    set_type(&sub.fixed_header, SUBSCRIBE);

    int payload_size = 0;
    for (int i = 0; i < num_topics; i++)
    {
        payload_size += strlen(topics[i]) + 2;
    }

    sub.payload = (unsigned char *)malloc(payload_size);
    int offset = 0;
    for (int i = 0; i < num_topics; i++)
    {
        int topic_length = strlen(topics[i]);
        sub.payload[offset++] = (unsigned char)topic_length;
        memcpy(&sub.payload[offset], topics[i], topic_length);
        sub.payload[offset + topic_length] = 0x00;
        offset += topic_length + 1;
    }

    sub.payload[payload_size - 1] = 0x00;
    sub.remaining_length = 2 + payload_size;

    return sub;
}

// Function to set the QoS in the fixed header
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
        printf("QoS not valid \n");
        break;
    }
}

// Function to set the type in the fixed header
void set_type(u_int8_t *fixed_header, u_int8_t type)
{
    *fixed_header &= ~(0b1111 << 4);
    *fixed_header |= (type << 4) & (0b1111 << 4);
}

// Function to get the QoS from the fixed header
uint8_t get_qos(uint8_t *fixed_header)
{
    return (*fixed_header >> 1) & 0b11;
}