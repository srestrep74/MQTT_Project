#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdbool.h>

#include "../packet/packet.h"
#include "../constants.h"
#include "server.h"

bool client_handler(int, Packet);

#endif