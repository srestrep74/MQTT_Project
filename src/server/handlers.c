#include "../../include/server/handlers.h"

bool client_handler(int client_socket, Packet client_packet)
{

    if (client_packet.fixed_header == 0)
    {
        fprintf(stderr, "Error al decodificar el mensaje del cliente\n");
        close(client_socket);
        return false;
    }

    if (get_type(&(client_packet.fixed_header)) != CONNECT)
    {
        printf("Mensaje recibido no es del tipo CONNECT\n");

        Packet connack_error = create_connack_message(CONNACK_REFUSED_NOT_AUTHORIZED);
        send_packet(client_socket, connack_error);

        close(client_socket);
        return false;
    }

    Packet connack_success = create_connack_message(CONNACK_CONNECTION_ACCEPTED);
    send_packet(client_socket, connack_success);
    return true;
}

void publish_handler(Packet packet, TopicNode *root, const char *topic, const char *message)
{
    TopicNode *node = getChildNode(root, topic);
    publishMessage(node, message);
    int numsubs = 0;
    int **subs = getSubscribers(node, &numsubs);
    printf("hodho");
    for (int i = 0; i < numsubs; i++)
    {

        printf("BUENOSD AIS%d\n", subs[i]);
        write(subs[i], message, strlen(message));
    }
    free(subs);
    printTree(root, 0);
}

void subscribe_handler(Packet packet, TopicNode *root, const char *topic, int client_socket)
{
    printf("%d\n", client_socket);
    char *topics[] = {topic};
    int num_topics = sizeof(topics) / sizeof(topics[0]);
    subscribeToTopics(root, topics, num_topics, client_socket);
    TopicNode *node = getChildNode(root, topic);
}