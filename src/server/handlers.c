#include "../../include/server/handlers.h"

// Function to handle client requests
bool client_handler(int client_socket, Packet client_packet)
{
    // Check if the client packet is valid
    if (client_packet.fixed_header == 0)
    {
        fprintf(stderr, "Error al decodificar el mensaje del cliente\n");
        close(client_socket);
        return false;
    }

    // Check if the packet type is CONNECT
    if (get_type(&(client_packet.fixed_header)) != CONNECT)
    {
        printf("Mensaje recibido no es del tipo CONNECT\n");

        // Create and send a connack message with connection refused status
        Packet connack_error = create_connack_message(CONNACK_REFUSED_NOT_AUTHORIZED);
        send_packet(client_socket, connack_error);

        close(client_socket);
        return false;
    }

    // Create and send a connack message with connection accepted status
    Packet connack_success = create_connack_message(CONNACK_CONNECTION_ACCEPTED);
    send_packet(client_socket, connack_success);
    return true;
}

// Function to handle publishing messages
void publish_handler(Packet packet, TopicNode *root, const char *topic, const char *message)
{
    printf("\033[1m This is the actual tree: \033[0m\n"); 

    // Get the node corresponding to the topic
    TopicNode *node = getChildNode(root, topic);

    // Publish the message to the node
    publishMessage(node, message);

    // Get the subscribers of the node
    int numsubs = 0;
    int **subs = getSubscribers(node, &numsubs);

    // Write the message to each subscriber
    for (int i = 0; i < numsubs; i++)
    {
        write(subs[i], message, strlen(message));
    }

    // Print the tree
    printTree(root, 0);
}

// Function to handle subscribing to topics
void subscribe_handler(Packet packet, TopicNode *root, const char **topics, int client_socket)
{
    // Calculate the number of topics
    int num_topics = sizeof(topics) / sizeof(topics[0]);

    // Subscribe to the topics
    subscribeToTopics(root, topics, num_topics, client_socket);
}