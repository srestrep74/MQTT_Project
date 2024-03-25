#ifndef PAYLOAD_H
#define PAYLOAD_H

#include "../../encoders/utf8.h"
#include <stdint.h>

union payload
{
    struct connect_payload
    {
        utf8_string client_identifier;
        utf8_string will_topic;
        utf8_string will_message;
        utf8_string user_name;
        utf8_string password;
    } connect_payload;

    struct publish_payload
    {
        char *data;
    } publish_payload;

    struct subscribe_list_payload
    {
        struct subscribe
        {
            int8_t subscribe_length_MSB;
            int8_t subscribe_length_LSB;
            char *subscribe_topic_name;
            int8_t requested_qos;
        };
        struct subscribe *subscribe_list;
        int subscribe_size;
    };

    struct suback_list_payload
    {
        struct suback
        {
            int8_t granted_qos;
        };
        struct suback *suback_list;
        int suback_size;
    };

    struct unsubscribe_list_payload
    {
        struct unsubscribe
        {
            int8_t unsubscribe_length_MSB;
            int8_t unsubscribe_length_LSB;
            char *unsubscribe_topic_name;
        };
        struct unsubscribe *unsubscribe_list;
        int unsubscribe_size;
    };
};

#endif