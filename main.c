#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "include/server/topic_tree/topic_tree.h"
/*#include "include/actions/subscribe.h"*/
#include "include/packet/packet.h"

Packet create_sub(char **topics, int num_topics)
{
    Packet sub;
    set_type(&sub.fixed_header, SUBSCRIBE);

    int payload_size = 0;
    for (int i = 0; i < num_topics; i++)
    {
        payload_size += strlen(topics[i]) + 2;
    }

    sub.payload = (unsigned char *)malloc(payload_size);
    int offset = 0;
    for (int i = 0; i < num_topics; i++)
    {
        int topic_length = strlen(topics[i]);
        sub.payload[offset++] = (unsigned char)topic_length;
        memcpy(&sub.payload[offset], topics[i], topic_length);
        sub.payload[offset + topic_length] = 0x00;
        offset += topic_length + 1;
    }

    sub.payload[payload_size - 1] = 0x00;
    sub.remaining_length = 2 + payload_size;

    return sub;
}

void log_transaction(const char *log_file, const char *client_ip, const char *query, const char *response_ip)
{
    FILE *file = fopen(log_file, "a"); // Abrir el archivo en modo de añadir (append)
    if (file == NULL)
    {
        perror("Error al abrir el archivo de registro");
        return;
    }

    // Obtener la fecha y hora actual
    time_t current_time;
    struct tm *local_time;
    time(&current_time);
    local_time = localtime(&current_time);

    // Escribir la información de la transacción en el archivo de registro
    fprintf(file, "%d-%02d-%02d %02d:%02d:%02d %s %s %s\n",
            local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
            local_time->tm_hour, local_time->tm_min, local_time->tm_sec,
            client_ip, query, response_ip);

    fclose(file);
}

/*int main()
{

    const char *topics[] = {"topic1", "topic2", "topic3"};
    int num_topics = sizeof(topics) / sizeof(topics[0]);

    printf("%d\n", num_topics);
    Packet sub = create_sub(topics, num_topics);
    int offset = 0;
    while (offset < sub.remaining_length)
    {
        // Obtener la longitud del tópico
        int topic_length = sub.payload[offset++];

        // Reservar memoria para el tópico y copiarlo
        char *topic = (char *)malloc(topic_length + 1); // +1 para el carácter nulo
        memcpy(topic, &sub.payload[offset], topic_length);
        topic[topic_length] = '\0'; // Agregar el carácter nulo al final

        // Imprimir el tópico
        printf("Topic: %s\n", topic);

        // Liberar la memoria del tópico
        free(topic);

        // Incrementar el desplazamiento
        offset += topic_length + 1;
    }

    return 0;
}*/

int main()
{

    /*if (argc != 2)
    {
        printf("Using : %s </path/log.log>\n", argv[0]);
        return 1;
    }

    const char *log_file = argv[1];
    char client_ip[16]; // Para almacenar direcciones IPv4
    char query[20];     // Suponiendo que el tipo de mensaje MQTT no excede los 20 caracteres
    char response_ip[16];

    strcpy(client_ip, "192.168.1.100");
    strcpy(query, "PUBLISH");
    strcpy(response_ip, "192.168.1.1");
    log_transaction(log_file, client_ip, query, response_ip);*/

    // Ejemplo de uso
    TopicNode *root = createTopicNode("/");

    const char *topic = "sensors";
    TopicNode *node1 = getChildNode(root, topic);
    topic = "sensors/temperature/wet";
    TopicNode *node2 = getChildNode(root, topic);
    topic = "sensors/humidity";
    TopicNode *node3 = getChildNode(root, topic);
    topic = "ejemplo/wet";
    TopicNode *node4 = getChildNode(root, topic);
    topic = "sensors";
    TopicNode *node5 = getChildNode(root, topic);

    const char *topics[] = {"sensors/#"};
    const int numTopics = sizeof(topics) / sizeof(topics[0]);
    int subscriber = 10;

    subscribeToTopics(root, topics, numTopics, subscriber);

    int numSubscribers = 0;
    TopicNode *test = getChildNode(root, "sensors/temperature/wet");
    int **subscribers = getSubscribers(test, &numSubscribers);
    if (subscribers != NULL)
    {
        printf("Los suscriptores del tópico sensors/temperature/wet: \n");
        for (int i = 0; i < numSubscribers; i++)
        {
            printf("- %d\n", subscribers[i]);
        }
    }
    topic = "sensors/sebas";
    TopicNode *node88 = getChildNode(root, topic);
    numSubscribers = 0;
    TopicNode *testt = getChildNode(root, "sensors/sebas");
    int **subscriberss = getSubscribers(testt, &numSubscribers);
    if (subscriberss != NULL)
    {
        printf("Los suscriptores del tópico sensors/sebas : \n");
        for (int i = 0; i < numSubscribers; i++)
        {
            printf("- %d\n", subscriberss[i]);
        }
    }

    // Liberar memoria utilizada por el árbol de tópicos
    // freeTopicTree(root);
    /*char *client_id = generate_client_id();
    printf("ID  Antes: %s\n", client_id);
    Packet connect = create_connect(client_id);

    char *id = utf8_decode(connect.payload);
    printf("ID  Despues: %s\n", id);

    return 0;*/
    return 0;
}
