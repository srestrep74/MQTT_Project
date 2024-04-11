#include "../../include/actions/publish.h"

// Publishes a message to a topic.
void publishMessage(TopicNode *node, const char *message)
{
    if (node == NULL || message == NULL)
        return;

    if (node->last_message != NULL)
    {
        free(node->last_message);
    }

    node->last_message = strdup(message);
}
