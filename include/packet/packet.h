#ifndef PACKET_H
#define PACKET_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../constants.h"
#include "../encoders/utf8.h"
#include "../client/utils.h"
#include "../actions/subscribe.h"

typedef struct
{
    u_int8_t fixed_header;
    u_int8_t remaining_length;
    u_int8_t *variable_header;
    u_int8_t *payload;
} Packet;

typedef struct
{
    const char *topic_name;
    uint8_t qos;
} TopicQoS;

Packet create_connect_message();
Packet create_publish_message(const char *, const char *);
Packet create_subscribe_message();
Packet create_connack_message(uint8_t);
Packet create_subscribe_message(const char *);
Packet create_disconnect_message();

void set_remaining_length(uint8_t *, size_t);
void set_clean_session_flag(uint8_t *);
uint8_t get_type(uint8_t *fixed_header);
char *get_topic(Packet *);
void set_type(u_int8_t *, u_int8_t);
void set_qos(uint8_t *, int);

#endif