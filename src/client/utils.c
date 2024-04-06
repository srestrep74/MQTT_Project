#include "../../include/client/utils.h"

char *generate_client_id()
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *client_id = (char *)malloc(23 + 1);
    if (client_id == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el identificador del cliente.\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int length = rand() % (23 + 1);
    for (int i = 0; i < length; i++)
    {
        client_id[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    client_id[length] = '\0';

    return client_id;
}