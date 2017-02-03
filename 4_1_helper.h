#ifndef DATA_STRUCTURES_4_1_HELPER_H
#define DATA_STRUCTURES_4_1_HELPER_H

#include "4_1.h"
#include <unistd.h>

LinkedListNode *linked_list_node_create(BTreeNode *value);

LinkedListNode *linked_list_node_destroy_recursively(LinkedListNode *node);

LinkedList *linked_list_create();

LinkedList *linked_list_destroy(LinkedList *list);

size_t linked_list_get_length(LinkedList *list);

int linked_list_insert(LinkedList *list, BTreeNode * value, size_t index);

void linked_list_as_array(LinkedList *list, BTreeNode *array[DEGREE + 1]);

void linked_list_add(LinkedList *list, BTreeNode * value);

int compare_ints(const void *first, const void *second);

int value_in_array(int *array, size_t size, int value);

BTreeNode *b_tree_node_destroy_recursively(BTreeNode *node);

BTree *b_tree_create();

/// Destroys the tree, recursively destroying its nodes
/// \param tree The tree to destroy
/// \return A NULL pointer to clean up the tree pointer
BTree *b_tree_destroy(BTree *tree);

int string_to_integer(const char *string, int *integer);

BTreeNode *b_tree_node_create();

ssize_t get_index_for_value(int array[DEGREE], int value);

void insert_into_array(BTreeNode *array[DEGREE + 1], size_t array_size, size_t index, BTreeNode *to_insert);

void delete_from_array(void *array, size_t *size, const void *value, const data_type type);

#endif //DATA_STRUCTURES_4_1_HELPER_H
