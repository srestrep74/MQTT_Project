#ifndef CONSTANTS_H
#define CONSTANTS_H

#define RESERVED 0x00
#define CONNECT 0x01
#define CONNACK 0x02
#define PUBLISH 0x03
#define PUBACK 0x04
#define PUBREC 0x05
#define PUBREL 0x06
#define PUBCOMP 0x07
#define SUBSCRIBE 0x08
#define SUBACK 0x09
#define UNSUBSCRIBE 0x0A
#define UNSUBACK 0x0B
#define PINGREQ 0x0C
#define PINGRESP 0x0D
#define DISCONNECT 0x0E

#define TYPE_MASK 0xF0

#define PROTOCOL_VERSION 0x03

#define MAX_CLIENT_ID_LENGTH 23

#endif