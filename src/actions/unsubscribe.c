#include "../../include/actions/unsubscribe.h"

void unsubscribeToTopics(TopicNode *root, char **topics, int num_topics, int client_id)
{
    for (int i = 0; i < num_topics; i++)
    {
        TopicNode *node = getChildNode(root, topics[i]);
        if (node != NULL)
        {
            int **subscribers = node->subscribers;
            int num_subscribers = node->num_subscribers;
            for (int j = 0; j < num_subscribers; j++)
            {
                if (subscribers[j] == client_id)
                {
                    for (int k = j; k < num_subscribers - 1; k++)
                    {
                        subscribers[k] = subscribers[k + 1];
                    }
                    node->num_subscribers--;
                    break;
                }
            }
        }
    }
}