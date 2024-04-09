#include "../../include/client/utils.h"

char *generate_client_id()
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    pid_t pid = getpid();

    unsigned int seed = (unsigned int)(tv.tv_sec * 1000000 + tv.tv_usec + pid);
    
    srand(seed);

    char *client_id = NULL;
    int length = 0;
    do {
        if (client_id != NULL) {
            free(client_id);
        }

        length = rand() % (23 + 1);
        client_id = (char *)malloc(length + 1);
        if (client_id == NULL)
        {
            fprintf(stderr, "Error: No se pudo asignar memoria para el identificador del cliente.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < length; i++)
        {
            client_id[i] = charset[rand() % (sizeof(charset) - 1)];
        }
        client_id[length] = '\0';

    } while (length == 0);

    return client_id;
}

void display_menu() {
    printf("\n");
    printf("===========================================\n");
    printf("                  Menu                     \n");
    printf("===========================================\n");
    printf("1. Subscriber\n");
    printf("2. Publisher\n");
    printf("3. Disconnect\n");
    printf("===========================================\n");
    printf("Enter the number corresponding to your choice: ");
}
