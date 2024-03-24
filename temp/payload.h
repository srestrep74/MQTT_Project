#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdint.h>

union payload
{
    struct connect_payload
    {
        char *client_identifier;
        char *will_topic;
        char *will_message;
        char *user_name;
        char *password;
    } connect_payload;

    struct publish_payload
    {
        char *data;
    };

    struct subscribe_list_payload
    {
        struct subscribe
        {
            int8_t length_MSB;
            int8_t length_LSB;
            char *topic_name;
            int8_t requested_qos;
        };
        struct subscribe *list;
        int size;
    };

    struct suback_list_payload
    {
        struct suback
        {
            int8_t granted_qos;
        };
        struct suback *list;
        int size;
    };

    struct unsubscribe_list_payload
    {
        struct unsubscribe
        {
            int8_t length_MSB;
            int8_t length_LSB;
            char *topic_name;
        };
        struct unsubscribe *list;
        int size;
    };
};

#endif