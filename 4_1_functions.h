#ifndef DATA_STRUCTURES_4_1_FUNCTIONS_H
#define DATA_STRUCTURES_4_1_FUNCTIONS_H

#include "4_1.h"
#include <unistd.h>

// this will ALWAYS return a node since there is always a node where a value is expected to be
BTreeNode *b_tree_search_internal(BTreeNode *node, int value);

// Checks if a given value is in the tree and returns a pointer to the Node which is expected to contain it
BTreeNode *b_tree_search(BTree *tree, int value);

// does all the linking for two nodes and also updates the LEAF / NODE status
// note that this will not change the children count since you can just overwrite a child reference
void b_tree_link_child(BTreeNode *parent, BTreeNode *child, size_t index);

void b_tree_overwrite_child(BTreeNode *parent, BTreeNode *child, size_t index);

ssize_t get_index_for_node(BTreeNode **node_array, size_t size, BTreeNode *node);

/// Prints tree_node keys, also showing how many of the key slots are still unset
/// \param node The node whose keys are to be printed
void b_tree_node_keys_print(BTreeNode *node);

/*
 * There are 4 cases:
 * - fits into key array
 * - root node flows over (easy split, 2 children will be created)
 * - regular node flows over
 *      pass middle value to the parent.
 *      In this case the parent will ALWAYS have DEGREE + 1 children and needs to
 */
void b_tree_insert(BTreeNode *node, int value, BTree *tree);

void b_tree_add(BTree *tree, int value);

/// Prints all the nodes recursively, indenting them for better readability
/// \param node The node to start printing from
/// \param depth The depth of printing, this should be 1 initially
void b_tree_nodes_print(BTreeNode *node, size_t depth);

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void b_tree_print(BTree *tree);

int parse_config(BTree *tree);

data_type get_input_type();

void b_tree_remove(BTree *tree, int value);

#endif //DATA_STRUCTURES_4_1_FUNCTIONS_H
