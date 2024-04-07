#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "include/server/topic_tree/topic_tree.h"
#include "include/actions/subscribe.h"
#include "include/packet/packet.h"


int main() {
    printf("jsoa");
    
    TopicQoS topics[] = {
        {"topic/a", 1},
        {"topic/b", 2},
        {"topic/c", 0}
    };
    
    size_t num_topics = sizeof(topics) / sizeof(topics[0]);
    
    size_t payload_length;
    printf("%zu\n", payload_length);

    Packet subscribe_message = create_subscribe_message(topics, num_topics, &payload_length);

    if (subscribe_message.payload == NULL) {
        printf("Error: No se pudo crear el mensaje de suscripción.\n");
        return EXIT_FAILURE;
    }

    printf("Mensaje de suscripción:\n");
    printf("Tipo: %d\n", get_type(&(subscribe_message.fixed_header)));
    printf("Packet ID: %d\n", get_packet_id(&(subscribe_message.variable_header)));
    printf("Remaining Length: %lu\n", subscribe_message.remaining_length);
    printf("Payload Length: %lu\n", payload_length);

    printf("Payload Content:\n");
    for (size_t i = 0; i < payload_length; ++i) {
        printf("%02X ", subscribe_message.payload[i]); 
    }
    printf("\n");
    free(subscribe_message.payload);

    return EXIT_SUCCESS;

}