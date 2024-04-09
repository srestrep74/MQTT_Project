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

    const char *topic = "hola";
    TopicNode *node1 = getChildNode(root, topic);
    const char *topic2 = "hola/hola";
    TopicNode *node2 = getChildNode(root, topic2);
    const char *topic22 = "sensors/humidity";
    TopicNode *node3 = getChildNode(root, topic22);

    /*const char *topics[] = {"sensors/+/hot", "sensors/#"};
    const int numTopics = sizeof(topics) / sizeof(topics[0]);
    const char *subscriber = "new_subscriber";

    subscribeToTopics(root, topics, numTopics, subscriber);

    int numSubscribers = 0;
    TopicNode *test = getChildNode(root, "sensors/humidity/hot");
    char **subscribers = getSubscribers(test, &numSubscribers);
    if (subscribers != NULL)
    {
        printf("Los suscriptores del tópico : \n");
        for (int i = 0; i < numSubscribers; i++)
        {
            printf("- %s\n", subscribers[i]);
        }
    }*/

    printTree(root, 0);

    // Liberar memoria utilizada por el árbol de tópicos
    freeTopicTree(root);

    Packet pub = create_publish_message("data", "data");

    printf("type : %d\n", get_type(&pub.fixed_header));
    printf("type : %d\n", get_qos(&pub.fixed_header));

    /*char *client_id = generate_client_id();
    printf("ID  Antes: %s\n", client_id);
    Packet connect = create_connect(client_id);

    char *id = utf8_decode(connect.payload);
    printf("ID  Despues: %s\n", id);*/

    return 0;
}
