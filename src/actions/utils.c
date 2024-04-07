#include "../../include/actions/utils.h"
#include <stdint.h>
#include <pthread.h>

static uint16_t packet_id = 0;

uint16_t get_packet_id() {

    packet_id++;

    if (packet_id > 65535) {
        packet_id = 1;
    }

    uint16_t msb = (packet_id >> 8) & 0xFF;
    uint16_t lsb = packet_id & 0xFF;
    // Retornar los bytes MSB y LSB como un solo valor uint16_t
    return (msb << 8) | lsb;
}