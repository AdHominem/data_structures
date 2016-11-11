#include <stdio.h>
#include <stdlib.h>

// ### LINKED LIST ###


typedef struct llnode LinkedListNode;

struct llnode {
    int value;
    LinkedListNode *next;
};

typedef struct llist {
    LinkedListNode *head;
} LinkedList;

LinkedListNode *create_node(int value) {
    LinkedListNode *result = malloc(sizeof(LinkedListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

LinkedListNode *destroy_nodes_recursively(LinkedListNode *node) {
    if (node) {
        node->next = destroy_nodes_recursively(node->next);
        free(node);
    }
    return NULL;
}

LinkedList *create_linked_list() {
    return malloc(sizeof(LinkedList));
}

LinkedList *destroy_linked_list(LinkedList *list) {
    if (list) {
        list->head = destroy_nodes_recursively(list->head);
        free(list);
    }
    return NULL;
}

size_t size_of_(LinkedList *list) {
    LinkedListNode *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

void insert_at_index(LinkedList *list, int value, size_t index) {

    // Catch index out of range
    if (index > size_of_(list)) {
        perror("List index out of range!");
        exit(1);
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = create_node(value);

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
        return;
    } else {
        LinkedListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
}

void add(LinkedList *list, int value) {
    insert_at_index(list, value, size_of_(list));
}

void print_linked_list(LinkedList *list) {
    LinkedListNode *head = list->head;

    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    printf("\n");
}

int list_contains(LinkedList *list, int value) {
    LinkedListNode *current = list->head;
    while (current) {
        if (current->value == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}



typedef struct integer_array {
    size_t length;
    int *elements;
} IntegerArray;

IntegerArray *create_integer_array(size_t length) {
    IntegerArray *result = malloc(sizeof(IntegerArray));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->elements = malloc(length * sizeof(int));
    if (result->elements == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->length = length;

    return result;
}

IntegerArray *destroy_integer_array(IntegerArray *integerArray) {
    if (integerArray) {
        free(integerArray->elements);
        free(integerArray);
    }
    return NULL;
}

void print_integer_array(IntegerArray *integerArray) {
    printf("[");
    for (size_t i = 0; i < integerArray->length; ++i) {
        printf("%d", integerArray->elements[i]);
        if (i != integerArray->length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}



