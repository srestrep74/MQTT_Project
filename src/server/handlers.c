#include "../../include/server/handlers.h"

// Function to handle client requests
bool client_handler(int client_socket, Packet client_packet)
{
    if (client_packet.fixed_header == 0)
    {
        fprintf(stderr, "Error decoding the client\n");
        close(client_socket);
        return false;
    }

    if (get_type(&(client_packet.fixed_header)) != CONNECT)
    {
        printf("Message is not CONNECT type\n");

        Packet connack_error = create_connack_message(CONNACK_REFUSED_NOT_AUTHORIZED);
        send_packet(client_socket, connack_error);

        close(client_socket);
        return false;
    }

    Packet connack_success = create_connack_message(CONNACK_CONNECTION_ACCEPTED);
    send_packet(client_socket, connack_success);
    return true;
}

// Function to handle publishing messages
void publish_handler(Packet packet, TopicNode *root, const char *topic, const char *message)
{
    printf("\033[1m This is the actual tree: \033[0m\n");

    TopicNode *node = getChildNode(root, topic);

    publishMessage(node, message);
    int numsubs = 0;
    int **subs = getSubscribers(node, &numsubs);

    for (int i = 0; i < numsubs; i++)
    {
        write(subs[i], message, strlen(message));
    }

    printTree(root, 0);
}

// Function to handle subscribing to topics
void subscribe_handler(Packet packet, TopicNode *root, const char **topics, int client_socket, int numTopics)
{
    subscribeToTopics(root, topics, numTopics, client_socket);
}

// Function to handle unsubscribing from topics
void unsubscribe_handler(Packet packet, TopicNode *root, const char **topics, int client_socket, int num_topics)
{
    unsubscribeToTopics(root, topics, num_topics, client_socket);
}
