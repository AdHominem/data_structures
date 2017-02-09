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
int b_tree_contains(BTree *tree, int value);
BTree *b_tree_create();

/// Destroys the tree, recursively destroying its nodes
/// \param tree The tree to destroy
/// \return A NULL pointer to clean up the tree pointer
BTree *b_tree_destroy(BTree *tree);

int string_to_integer(const char *string, int *integer);

BTreeNode *b_tree_node_create();

ssize_t get_index_for_value(int array[DEGREE], int value);

void array_insert(BTreeNode **array, size_t *array_size, size_t index, BTreeNode *to_insert);
void array_add(BTreeNode *array[DEGREE + 1], size_t *array_size, BTreeNode *to_insert);
void array_delete(void *array, size_t *size, const void *value, const data_type type);

/// Prints tree_node keys, also showing how many of the key slots are still unset
/// \param node The node whose keys are to be printed
void b_tree_node_keys_print(BTreeNode *node);

int array_contains(const void *array, const size_t size, const void *value, const data_type type);

void array_int_add_sorted(int *array, size_t *array_size, const int to_insert);

/// Prints all the nodes recursively, indenting them for better readability
/// \param node The node to start printing from
/// \param depth The depth of printing, this should be 1 initially
void b_tree_nodes_print(BTreeNode *node, size_t depth);

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void b_tree_print(BTree *tree);

#endif //DATA_STRUCTURES_4_1_HELPER_H
