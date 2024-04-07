#include "../../include/actions/utils.h"
#include <stdint.h>
#include <pthread.h>

uint16_t get_packet_id() {
    static uint16_t packet_id;
    static bool init = false;

    if(init == false)
    {
        packet_id = 0;
        init = true;
    }
    else
    {
        packet_id++;
    }

    if (packet_id > 65535) {
        packet_id = 1;
    }

    uint16_t msb = (packet_id >> 8) & 0xFF;
    uint16_t lsb = packet_id & 0xFF;
    return (msb << 8) | lsb;
}