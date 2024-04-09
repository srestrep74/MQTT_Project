#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "include/server/topic_tree/topic_tree.h"
/*#include "include/actions/subscribe.h"*/
#include "include/packet/packet.h"

int main()
{
    // Ejemplo de uso
    TopicNode *root = createTopicNode("/");

    /*const char *topic = "sensors";
    TopicNode *node1 = getChildNode(root, topic);
    topic = "sensors/temperature/wet";
    TopicNode *node2 = getChildNode(root, topic);
    topic = "sensors/humidity";
    TopicNode *node3 = getChildNode(root, topic);
    topic = "ejemplo/wet";
    TopicNode *node4 = getChildNode(root, topic);
    topic = "sensors";
    TopicNode *node5 = getChildNode(root, topic);

    const char *topics[] = {"sensors/+/wet"};
    const int numTopics = sizeof(topics) / sizeof(topics[0]);
    int subscriber = 10;

    subscribeToTopics(root, topics, numTopics, subscriber);

    int numSubscribers = 0;
    TopicNode *test = getChildNode(root, "sensors/temperature/wet");
    int **subscribers = getSubscribers(test, &numSubscribers);
    if (subscribers != NULL)
    {
        printf("Los suscriptores del tópico : \n");
        for (int i = 0; i < numSubscribers; i++)
        {
            printf("- %d\n", subscribers[i]);
        }
    }*/

    char topic[100], message[100];
    printf("Enter the topic\n");
    fgets(topic, sizeof(topic), stdin);
    topic[strcspn(topic, "\n")] = '\0';
    printf("Enter the message\n");
    fgets(message, sizeof(message), stdin);
    Packet pub = create_publish_message(topic, message);
    TopicNode *node = getChildNode(root, topic);
    publishMessage(node, message);
    printTree(root, 0);

    char topic1[100], message1[100];
    printf("Enter the topic\n");
    fgets(topic1, sizeof(topic1), stdin);
    topic1[strcspn(topic1, "\n")] = '\0';
    printf("Enter the message\n");
    fgets(message1, sizeof(message1), stdin);
    Packet pub1 = create_publish_message(topic1, message1);
    TopicNode *node1 = getChildNode(root, topic1);
    publishMessage(node1, message1);
    printTree(root, 0);

    // Liberar memoria utilizada por el árbol de tópicos
    freeTopicTree(root);
    /*char *client_id = generate_client_id();
    printf("ID  Antes: %s\n", client_id);
    Packet connect = create_connect(client_id);

    char *id = utf8_decode(connect.payload);
    printf("ID  Despues: %s\n", id);*/

    return 0;
}
