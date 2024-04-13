#include "../../include/log/log.h"

void log_activity(const char *filename, const char *client_ip, const char *query, const char *server_ip)
{
    FILE *fp = fopen(filename, "a");
    if (!fp)
    {
        perror("Failed to open log file");
        return;
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date_time[64];
    strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", tm);

    fprintf(fp, "%s - %s - %s - %s\n", date_time, client_ip, query, server_ip);
    fclose(fp);
}