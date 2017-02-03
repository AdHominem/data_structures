#include "4_1_helper.h"
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

LinkedListNode *linked_list_node_create(BTreeNode *value) {
    LinkedListNode *result = malloc(sizeof(LinkedListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

LinkedListNode *linked_list_node_destroy_recursively(LinkedListNode *node) {
    if (node) {
        node->next = linked_list_node_destroy_recursively(node->next);
    }
    free(node);
    return NULL;
}

LinkedList *linked_list_create() {
    return calloc(1, sizeof(LinkedList));
}

LinkedList *linked_list_destroy(LinkedList *list) {
    if (list) {
        list->head = linked_list_node_destroy_recursively(list->head);
    }
    free(list);
    return NULL;
}

size_t linked_list_get_length(LinkedList *list) {
    LinkedListNode *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

int linked_list_insert(LinkedList *list, BTreeNode * value, size_t index) {

    // Catch index out of range
    if (index > linked_list_get_length(list)) {
        perror("List index out of range!");
        return 2;
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = linked_list_node_create(value);
    if (to_insert == NULL) {
        return 1;
    }

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
    } else {
        LinkedListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
    return 0;
}

void linked_list_as_array(LinkedList *list, BTreeNode *array[DEGREE + 1]) {

    LinkedListNode *node = list->head;

    for (size_t i = 0; i < linked_list_get_length(list); ++i) {
        array[i] = node->value;
        node = node->next;
    }
}

void linked_list_add(LinkedList *list, BTreeNode * value) {
    linked_list_insert(list, value, linked_list_get_length(list));
}

int compare_ints(const void *first, const void *second) {
    int a = *(int*) first;
    int b = *(int*) second;
    return a < b ? -1 : a > b;
}

int value_in_array(int *array, size_t size, int value) {
    for (size_t i = 0; i < size; ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

BTreeNode *b_tree_node_destroy_recursively(BTreeNode *node) {
    if (node) {
        for (size_t i = 0; i < node->children_count; ++i) {
            b_tree_node_destroy_recursively(node->children[i]);
        }
    }
    free(node);
    return NULL;
}

BTree *b_tree_create() {

    BTree *result = malloc(sizeof(BTree));
    if (result == NULL) {
        return NULL;
    }

    result->root = NULL;
    return result;
}

/// Destroys the tree, recursively destroying its nodes
/// \param tree The tree to destroy
/// \return A NULL pointer to clean up the tree pointer
BTree *b_tree_destroy(BTree *tree) {
    if (tree) {
        tree->root = b_tree_node_destroy_recursively(tree->root);
    }
    free(tree);
    return NULL;
}

int string_to_integer(const char *string, int *integer) {
    char *p;
    const long value = strtol(string, &p, 10);
    if (!(p == string || LONG_MIN == value || LONG_MAX == value || value > INT_MAX || value < INT_MIN)) {
        *integer = (int) value;
        return 0;
    }
    return 1;
}

BTreeNode *b_tree_node_create() {

    BTreeNode *result = malloc(sizeof(BTreeNode));
    if (result == NULL) {
        return NULL;
    }

    result->type_of_node = LEAF;
    result->children_count = 0;
    result->keys_count = 0;
    result->parent = NULL;
    return result;
}

ssize_t get_index_for_value(int array[DEGREE], int value) {
    for (size_t i = 0; i < DEGREE; ++i) {
        if (array[i] == value) {
            return i;
        }
    }
    return -1;
}

void insert_into_array(BTreeNode *array[DEGREE + 1], size_t array_size, size_t index, BTreeNode *to_insert) {
    LinkedList *list = linked_list_create();
    for (size_t i = 0; i < array_size; ++i) {
        linked_list_add(list, array[i]);
    }
    linked_list_insert(list, to_insert, index);
    linked_list_as_array(list, array);
    linked_list_destroy(list);
}

// assumes that all values are unique in the array
// note that this updates the size
void delete_from_array(void *array, size_t *size, const void *value, const data_type type) {

    if (type == int_type) {
        int *cast_array = (int *) array;
        int cast_value = *((int*) value);
        int replace = false;
        for (size_t i = 0; i < *size; ++i) {
            if (cast_array[i] == cast_value) {
                replace = true;
                --*size;
            }

            if (replace) {
                cast_array[i] = cast_array[i + 1];
            }
        }
    }
}

int compare(const void *first, const void *second, const data_type type) {
    if (type == int_type) {
        int a = *(int*) first;
        int b = *(int*) second;
        return a < b ? -1 : a > b;
    }

    return -2;
}