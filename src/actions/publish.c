#include "../../include/actions/publish.h"

void publishMessage(TopicNode *node, const char *message)
{
    if (node == NULL || message == NULL)
        return;

    // Liberar el mensaje anterior si existe
    if (node->last_message != NULL)
    {
        free(node->last_message);
    }

    // Asignar el nuevo mensaje al nodo
    node->last_message = strdup(message);
}
