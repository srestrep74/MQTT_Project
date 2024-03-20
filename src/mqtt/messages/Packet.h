#ifndef MQTT_PACKET_H
#define MQTT_PACKET_H

#include <stdint.h>
#include <stdbool.h>

// Enumeration to represent the different types of MQTT control packets
typedef enum
{
    MQTT_CONNECT = 1,
    MQTT_CONNACK = 2,
    MQTT_PUBLISH = 3,
    MQTT_PUBREC = 5,
    MQTT_PUBREL = 6,
    MQTT_PUBCOMP = 7,
    MQTT_SUBSCRIBE = 8,
    MQTT_SUBACK = 9,
    MQTT_UNSUBSCRIBE = 10,
    MQTT_UNSUBACK = 11,
    MQTT_PINGREQ = 12,
    MQTT_PINGRESP = 13,
    MQTT_DISCONNECT = 14
} mqtt_packet_type_t;

// Structure to represent an MQTT control packet
typedef struct
{
    mqtt_packet_type_t type;
    uint8_t flags;
    uint32_t remaining_length;
    uint8_t *payload;
    uint32_t payload_length;
} mqtt_packet_t;

/**
 * @brief Encodes an MQTT control packet.
 *
 * This function encodes an MQTT control packet provided in the mqtt_packet_t struct
 * into the specified buffer.
 *
 * @param packet Pointer to the mqtt_packet_t struct to be encoded.
 * @param buffer Pointer to the buffer to write the encoded packet to.
 * @param buffer_len Length of the buffer.
 * @param bytes_written Pointer to store the number of bytes written to the buffer.
 *                     This parameter will be updated with the actual number of bytes written.
 *
 * @return true if encoding was successful, false otherwise.
 */
bool mqtt_packet_encode(mqtt_packet_t *packet, uint8_t *buffer, uint32_t buffer_len, uint32_t *bytes_written);

/**
 * @brief Decodes an MQTT control packet.
 *
 * This function decodes an MQTT control packet from the provided buffer
 * and stores it in the specified mqtt_packet_t struct.
 *
 * @param packet Pointer to the mqtt_packet_t struct to store the decoded packet.
 * @param buffer Buffer containing the MQTT control packet.
 * @param buffer_len Length of the buffer.
 *
 * @return true if decoding was successful, false otherwise.
 */
bool mqtt_packet_decode(mqtt_packet_t *packet, const uint8_t *buffer, uint32_t buffer_len);

/**
 * @brief Frees the dynamically allocated payload buffer in an MQTT packet.
 *
 * This function is responsible for freeing the dynamically allocated payload buffer
 * within the provided mqtt_packet_t struct to prevent memory leaks.
 *
 * @param packet Pointer to the mqtt_packet_t struct.
 */

void mqtt_packet_free(mqtt_packet_t *packet);

#endif