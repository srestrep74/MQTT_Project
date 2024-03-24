#ifndef VARIABLE_HEADER_H
#define VARIABLE_HEADER_H

#include <stdint.h>

union variable_header
{
    struct connect_variable_h
    {
        int8_t connect_length_MSB;
        int8_t connect_length_LSB;
        char protocol_name[7];
        // int8_t protocol_name[6];
        int8_t version;
        int8_t flags;
        int8_t keep_alive_MSB;
        int8_t keep_alive_LSB;
    } connect_variable_h;

    struct connack_variable_h
    {
        int8_t session_present;
        int8_t return_code;
    };

    struct publish_variable_h
    {
        int8_t publish_length_MSB;
        int8_t publish_length_LSB;
        char *topic_name;
        int8_t message_id_MSB;
        int8_t message_id_LSB;
    };

    struct ack_variable_h
    {
        int8_t ack_message_id_MSB;
        int8_t ack_message_id_LSB;
    };

    struct subscribe_variable_h
    {
        int8_t subscribe_message_id_MSB;
        int8_t subscribe_message_id_LSB;
    };

    struct unsubscribe_variable_h
    {
        int8_t unsubscribe_message_id_MSB;
        int8_t unsubscribe_message_id_LSB;
    };
};

#endif