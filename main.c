#include "messages/base/message.h"
#include "encoders/fixed_header.h"
#include "encoders/utf8.h"
#include "encoders/packet.h"
#include "broker/trie.h"
#include <stdlib.h>
#include <stdio.h>
#include "broker/log.h"

int main()
{

    message msg;
    msg.type = PUBLISH;
    initialize_message(&msg);
    msg.set_fixed_header(&msg);
    msg.set_variable_header(&msg);
    msg.set_payload(&msg);
    msg.set_topic(&msg, "topic1");
    msg.set_data(&msg, "Data del topic1");

    printf("%s\n", msg.variable_header->publish_variable_h.topic_name);
    printf("%s\n", msg.payload->publish_payload.data);

    message msg1;
    msg1.type = SUBSCRIBE;
    initialize_message(&msg1);
    msg1.set_fixed_header(&msg1);
    msg1.set_variable_header(&msg1);
    msg1.set_payload(&msg1);
    char *topics[] = {"topic1", "topic2", "topic3", "topic4"};
    msg1.set_topics(&msg1, topics, 4);

    for (int i = 0; i < 4; i++)
        printf("%s\n", msg1.payload->sub_payload.subscribe_list[i].subscribe_topic_name);

    /*

TopicNode *root = malloc(sizeof(TopicNode));
root->topic_name = strdup("/");
root->children = NULL;
root->next_sibling = NULL;
root->parent = NULL;

char *topic1 = "topic1";
char *topic2 = "topic2";
TopicNode *node1 = add_topic_node(root, topic1);
TopicNode *node2 = add_topic_node(root, topic2);
TopicNode *subtopic1 = add_topic_node(node1, "subtopic1");

print_topic_tree(root);

*/

    // Mirar bien jerarquia
    // Filtrados

    TrieNode *root = createNode();

    // Publicar tópicos y suscribirse
    insertTopic(root, "topic/sensores", "Data 1");
    insertTopic(root, "topic/actuadores", "Data 2");
    subscribeTopic(root, "topic/sensores", "cliente1");
    subscribeTopic(root, "topic/sensores", "cliente2");
    subscribeTopic(root, "topic/actuadores", "cliente1");

    // Buscar suscriptores de un tópico
    TrieNode *topicNode = findTopicNode(root, "topic/sensores");
    char **subscribers = getSubscribers(topicNode);
    if (subscribers)
    {
        printf("%s\n", topicNode->last_message);
        printf("Suscriptores de topic/sensores:\n");
        for (int i = 0; i < topicNode->num_subscribers; i++)
        {
            printf("%s\n", subscribers[i]);
        }
    }
    else
    {
        printf("No hay suscriptores para el tópico especificado.\n");
    }

    insertTopic(root, "topic/sensores", "Data 2");
    topicNode = findTopicNode(root, "topic/sensores");
    printf("%s\n", topicNode->last_message);

    register_request("127.0.0.1", "CONNECT", "CONNACK");

    return 0;
}