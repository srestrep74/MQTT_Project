
#include <stdlib.h>
#include <stdio.h>
#include "mqtt/include/tree/trie.h"

int main()
{
    TrieNode *root = createNode();

    // Publicación de mensajes
    insertTopic(root, "topic/room1/light", "Message for room1 light");
    insertTopic(root, "topic/room2/light", "Message for room2 light");
    insertTopic(root, "topic/room2/sports", "Message for room2 sports");
    insertTopic(root, "topic/room1/temperature", "Message for room1 temperature");

    // Publicación de mensajes usando wildcard
    insertTopic(root, "topic/room1/#", "Message for room1 (wildcard)");
    insertTopic(root, "topic/+/sports", "Message for sports (wildcard)");

    // Suscripciones
    subscribeTopic(root, "topic/room1/light", "subscriber1");
    subscribeTopic(root, "topic/room2/light", "subscriber2");
    subscribeTopic(root, "topic/room1/temperature", "subscriber3");

    // Obtener suscriptores de un tema
    char **subscribers = getSubscribers(findTopicNode(root, "topic/room1/light"));
    if (subscribers)
    {
        printf("Subscribers to topic 'topic/room1/light':\n");
        for (int i = 0; subscribers[i] != NULL; ++i)
        {
            printf("- %s\n", subscribers[i]);
        }
    }
    else
    {
        printf("No subscribers found for topic 'topic/room1/light'\n");
    }

    TrieNode *node = findTopicNode(root, "topic/room2/sports");
    printf("%s\n", node->last_message);

    // Liberar memoria
    destroyTrie(root);

    return 0;
}