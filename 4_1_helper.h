#ifndef DATA_STRUCTURES_4_1_HELPER_H
#define DATA_STRUCTURES_4_1_HELPER_H

#include "4_1.h"
#include <unistd.h>

int compare_ints(const void *first, const void *second);

BTree *b_tree_create();

/// Destroys the tree, recursively destroying its nodes
/// \param tree The tree to destroy
/// \return A NULL pointer to clean up the tree pointer
BTree *b_tree_destroy(BTree *tree);

int string_to_integer(const char *string, int *integer);

BTreeNode *b_tree_node_create();

ssize_t array_get_index(const int *array, const int value);

// note that this updates the size
void array_insert(BTreeNode **array, size_t *array_size, const size_t index, BTreeNode *to_insert);

// note that this updates the size
// assumes this fits
void array_add(BTreeNode *array[DEGREE + 1], size_t *array_size, BTreeNode *to_insert);

// note that this updates the size
// ATTENTION: This assumes the array can actually fit an additional value
void array_int_add_sorted(int *array, size_t *array_size, const int to_insert);

int array_contains(const void *array, const size_t size, const void *value, const data_type type);

// assumes that all values are unique in the array
// note that this updates the size
// this simulates a dynamic data structure
void array_delete(void *array, size_t *size, const void *value, const data_type type);

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void b_tree_print(const BTree *tree);

ssize_t b_tree_node_get_index(const BTreeNode **node_array, const size_t size, const BTreeNode *node);

/// Inserts a new child at a specific position into parent's children
void b_tree_link_child_insert(BTreeNode *parent, BTreeNode *child, const size_t index);

/// Links two nodes, appending the child to the parent's list
void b_tree_link_child_add(BTreeNode *parent, BTreeNode *child);

void b_tree_overwrite_child(BTreeNode *parent, BTreeNode *child, const size_t index);

/// This one is used for deletion of inner nodes
BTreeNode *b_tree_find_next_largest_value(BTreeNode *node, const int value);

#endif //DATA_STRUCTURES_4_1_HELPER_H
