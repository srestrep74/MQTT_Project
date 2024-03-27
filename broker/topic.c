#include "topic.h"

TopicNode *add_topic_node(TopicNode *parent, const char *topic_name)
{
    TopicNode *new_node = malloc(sizeof(TopicNode));
    if (new_node == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo nodo\n");
        exit(EXIT_FAILURE);
    }
    new_node->topic_name = strdup(topic_name);
    new_node->children = NULL;
    new_node->next_sibling = NULL;
    new_node->parent = parent;

    // Agregar el nuevo nodo como hijo del nodo padre
    if (parent != NULL)
    {
        if (parent->children == NULL)
        {
            parent->children = new_node;
        }
        else
        {
            TopicNode *current_child = parent->children;
            while (current_child->next_sibling != NULL)
            {
                current_child = current_child->next_sibling;
            }
            current_child->next_sibling = new_node;
        }
    }

    return new_node;
}

TopicNode *find_topic_node(TopicNode *root, const char *topic_name)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (strcmp(root->topic_name, topic_name) == 0)
    {
        return root;
    }

    // Buscar en los hijos del nodo actual
    TopicNode *child = root->children;
    while (child != NULL)
    {
        TopicNode *result = find_topic_node(child, topic_name);
        if (result != NULL)
        {
            return result;
        }
        child = child->next_sibling;
    }

    // El nodo no se encontró en este subárbol
    return NULL;
}

void free_topic_tree(TopicNode *root)
{
    if (root == NULL)
    {
        return;
    }

    // Liberar los hijos del nodo actual
    TopicNode *child = root->children;
    while (child != NULL)
    {
        TopicNode *next_sibling = child->next_sibling;
        free_topic_tree(child);
        child = next_sibling;
    }

    // Liberar la memoria utilizada por el nodo actual
    free(root->topic_name);
    free(root);
}
