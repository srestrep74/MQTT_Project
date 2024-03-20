#include "Packet.h"
#include <string.h>

/**
 * @brief Encodes the remaining length value.
 *
 * This static function encodes the given length value into a variable-length
 * representation according to the MQTT protocol and writes the encoded bytes
 * to the specified buffer.
 *
 * @param length The length value to be encoded.
 * @param buffer Buffer to write the encoded bytes to.
 *
 * @return Number of bytes written to the buffer.
 */

static uint32_t mqtt_encode_remaining_length(uint32_t length, uint8_t *buffer)
{
    uint32_t encoded_bytes = 0;
    do
    {
        uint8_t encoded_byte = length % 128;
        length /= 128;
        if (length > 0)
        {
            encoded_byte |= 0x80; // Set the continuation bit
        }
        buffer[encoded_bytes++] = encoded_byte;
    } while (length > 0);
    return encoded_bytes;
}

bool mqtt_packet_encode(mqtt_packet_t *packet, uint8_t *buffer, uint32_t buffer_len, uint32_t *bytes_written)
{
    // Check if the buffer is large enough
    if (buffer_len < 2 + packet->payload_length)
    {
        return false; // Buffer too small
    }

    uint32_t offset = 0;
    // Encode the packet type and flags in the first byte
    buffer[offset++] = (packet->type << 4) | packet->flags;

    // Encode the remaining length
    uint32_t remaining_length_bytes = mqtt_encode_remaining_length(packet->payload_length, &buffer[offset]);
    offset += remaining_length_bytes;

    // Copy the payload data to the buffer
    memcpy(&buffer[offset], packet->payload, packet->payload_length);
    offset += packet->payload_length;

    // Store the number of bytes written
    *bytes_written = offset;
    return true;
}

/**
 * @brief Decodes the remaining length value.
 *
 * This static function decodes the remaining length value from the provided buffer
 * according to the MQTT protocol. The current offset in the buffer is updated
 * after decoding, and the decoded remaining length value is returned.
 *
 * @param buffer Buffer containing the encoded remaining length.
 * @param buffer_len Length of the buffer.
 * @param offset Pointer to the current offset in the buffer (updated after decoding).
 *
 * @return Decoded remaining length value.
 */

static uint32_t mqtt_decode_remaining_length(const uint8_t *buffer, uint32_t buffer_len, uint32_t *offset)
{
    uint32_t multiplier = 1;
    uint32_t remaining_length = 0;
    uint32_t offset_start = *offset;

    do
    {
        // Check if we reached the end of the buffer
        if (*offset >= buffer_len)
        {
            return 0; // Malformed remaining length
        }
        // Decode the current byte and add it to the remaining length
        remaining_length += (buffer[(*offset)++] & 127) * multiplier;
        multiplier *= 128;
    } while ((buffer[offset_start] & 128) != 0); // Check the continuation bit

    return remaining_length;
}

bool mqtt_packet_decode(mqtt_packet_t *packet, const uint8_t *buffer, uint32_t buffer_len)
{
    // Check if the buffer is large enough
    if (buffer_len < 2)
    {
        return false; // Buffer too small
    }

    uint32_t offset = 0;
    // Decode the packet type and flags from the first byte
    packet->type = (mqtt_packet_type_t)((buffer[offset] >> 4) & 0x0F);
    packet->flags = buffer[offset++] & 0x0F;

    // Decode the remaining length
    packet->remaining_length = mqtt_decode_remaining_length(buffer, buffer_len, &offset);
    if (packet->remaining_length == 0)
    {
        return false; // Malformed remaining length
    }

    // Check if the buffer is large enough to contain the payload
    if (buffer_len < offset + packet->remaining_length)
    {
        return false; // Buffer too small
    }

    // Allocate memory for the payload and copy the data
    packet->payload_length = packet->remaining_length;
    packet->payload = malloc(packet->payload_length);
    if (packet->payload == NULL)
    {
        return false; // Out of memory
    }

    memcpy(packet->payload, &buffer[offset], packet->payload_length);
    return true;
}

void mqtt_packet_free(mqtt_packet_t *packet)
{
    // Check if the payload buffer is allocated
    if (packet->payload != NULL)
    {
        free(packet->payload);      // Free the payload buffer
        packet->payload = NULL;     // Set the pointer to NULL
        packet->payload_length = 0; // Reset the payload length
    }
}