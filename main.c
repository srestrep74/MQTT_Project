#include "messages/base/message.h"
#include "encoders/fixed_header.h"
#include "encoders/utf8.h"
#include "encoders/packet.h"
#include "broker/topic.h"
#include <stdlib.h>
#include <stdio.h>

// Función auxiliar para imprimir la estructura del árbol de tópicos recursivamente
void print_topic_tree_helper(TopicNode *node, int depth)
{
    // Imprimir el nombre del tópico con sangría según la profundidad
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("|-- %s\n", node->topic_name);

    // Recorrer los hijos del nodo actual
    TopicNode *child = node->children;
    while (child != NULL)
    {
        // Llamar recursivamente a esta función para imprimir los hijos
        print_topic_tree_helper(child, depth + 1);
        child = child->next_sibling;
    }
}

// Función para imprimir la estructura del árbol de tópicos
void print_topic_tree(TopicNode *root)
{
    printf("Estructura del árbol de tópicos:\n");
    // Comenzar el recorrido desde el nodo raíz
    if (root != NULL)
    {
        print_topic_tree_helper(root, 0);
    }
    else
    {
        printf("El árbol de tópicos está vacío.\n");
    }
}

int main()
{

    message msg;
    msg.type = CONNECT;
    initialize_message(&msg);
    msg.set_fixed_header(&msg);
    msg.set_variable_header(&msg);
    msg.set_payload(&msg);

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

    return 0;
}