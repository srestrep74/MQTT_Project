#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void register_request(const char *clientIP, const char *query, const char *responseIP);

#endif