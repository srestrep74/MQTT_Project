#include "log.h"

void register_request(const char *clientIP, const char *query, const char *responseIP)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    FILE *archivo = fopen("broker/log.txt", "a");
    if (archivo == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo de registro.\n");
        exit(1);
    }

    fprintf(archivo, "[%04d-%02d-%02d %02d:%02d:%02d] %s %s %s\n",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
            clientIP, query, responseIP);
    fclose(archivo);
}