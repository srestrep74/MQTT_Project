#include "../../include/actions/subscribe.h"

// Subscribes to MQTT topics and manages subscribers.
void subscribeToTopics(TopicNode *root, const char **topics, int numTopics, const int subscriber)
{
    if (root == NULL || topics == NULL)
        return;

    for (int i = 0; i < numTopics; ++i)
    {
        const char *topic = topics[i];

        if (strstr(topic, "#") != NULL)
        {
            subscribeToChildrenBeforeWildcard(root, topic, subscriber);
        }
        else if (strstr(topic, "+") != NULL)
        {
            subscribeToWildcardPlus(root, topic, subscriber);
        }
        else
        {
            TopicNode *node = getChildNode(root, topic);
            if (node != NULL)
            {
                addSubscriber(node, subscriber);
            }
        }
    }
}

// Subscribes to MQTT topics with wildcard '+'.
void subscribeToWildcardPlus(TopicNode *node, const char *pattern, const int subscriber)
{
    if (node == NULL || pattern == NULL)
        return;

    char *patternCopy = strdup(pattern);
    if (patternCopy == NULL)
        return;

    char *token = strtok(patternCopy, "/");
    char *partialPattern = (char *)malloc(strlen(pattern) + 1);
    partialPattern[0] = '\0';

    while (token != NULL)
    {
        strcat(partialPattern, token); 
        strcat(partialPattern, "/");  

        if (strcmp(token, "+") == 0)
        {
            TopicNode *child = node->children;
            while (child != NULL)
            {
                subscribeToWildcardPlus(child, pattern + strlen(partialPattern), subscriber);
                child = child->next_sibling;
            }
            break; 
        }

        node = getChildNodeHelper(node, token);
        if (node == NULL)
            break;

        token = strtok(NULL, "/");
    }

    if (token == NULL && node != NULL)
    {
        addSubscriber(node, subscriber);
    }

    free(patternCopy);
    free(partialPattern);
}

// Subscribes to MQTT topics with wildcard '#'.
void subscribeToChildrenBeforeWildcard(TopicNode *root, const char *topic, const int subscriber)
{
    if (root == NULL || topic == NULL )
        return;

    char *topicCopy = strdup(topic);
    if (topicCopy == NULL)
        return;

    char *token = strtok(topicCopy, "/");
    TopicNode *currentNode = root;
    char *partialTopic = (char *)malloc(strlen(topic) + 1);
    partialTopic[0] = '\0'; 

    while (token != NULL && strcmp(token, "#") != 0)
    {
        strcat(partialTopic, token);
        strcat(partialTopic, "/"); 
        currentNode = getChildNodeHelper(currentNode, token);
        token = strtok(NULL, "/");
    }

    if (currentNode != NULL)
    {
        subscribeToChildren(currentNode, subscriber);
    }

    free(topicCopy);
    free(partialTopic);
}

// Subscribes to children nodes of a given topic node.
void subscribeToChildren(TopicNode *node, const int subscriber)
{
    if (node == NULL )
        return;

    addSubscriber(node, subscriber);

    TopicNode *child = node->children;
    while (child != NULL)
    {
        subscribeToChildren(child, subscriber);
        child = child->next_sibling;
    }
}

// Adds a subscriber to a topic node.
void addSubscriber(TopicNode *node, const int subscriber)
{
    if (node == NULL )
        return;

    for (int i = 0; i < node->num_subscribers; ++i)
    {
        if (node->subscribers[i] == subscriber)
            return; 
    }
    node->subscribers = realloc(node->subscribers, (node->num_subscribers + 1) * sizeof(char *));
    node->subscribers[node->num_subscribers] = subscriber;
    node->num_subscribers++;
}

// Generates a unique packet ID for MQTT communication.
uint16_t get_packet_id() {
    static uint16_t packet_id;
    static bool init = false;

    if(init == false)
    {
        packet_id = 0;
        init = true;
    }
    else
    {
        packet_id++;
    }

    if (packet_id > 65535) {
        packet_id = 1;
    }

    uint16_t msb = (packet_id >> 8) & 0xFF;
    uint16_t lsb = packet_id & 0xFF;
    return (msb << 8) | lsb;
}