#include <stdio.h>
#include <stdlib.h>

typedef struct node Node;

struct node {
    int value;
    Node *next;
};

Node *create_node(int value) {
    Node *result = malloc(sizeof(Node));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

Node *destroy_nodes_recursively(Node *node) {
    if (node) {
        node->next = destroy_nodes_recursively(node->next);
        free(node);
    }
    return NULL;
}

typedef struct list {
    Node *head;
} List;

List *create_list() {
    return malloc(sizeof(List));
}

List *destroy_list(List *list) {
    if (list) {
        list->head = destroy_nodes_recursively(list->head);
        free(list);
    }
    return NULL;
}

size_t size_of_(List *list) {
    Node *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

void insert_at_index(List *list, int value, size_t index) {

    // Catch index out of range
    if (index > size_of_(list)) {
        perror("List index out of range!");
        exit(1);
    }

    Node *head = list->head;
    Node *to_insert = create_node(value);

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
        return;
    } else {
        Node *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
}

void add(List *list, int value) {
    insert_at_index(list, value, size_of_(list));
}

void print_list(List *list) {
    Node *head = list->head;

    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    printf("\n");
}