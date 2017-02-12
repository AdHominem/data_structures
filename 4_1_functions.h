#ifndef DATA_STRUCTURES_4_1_FUNCTIONS_H
#define DATA_STRUCTURES_4_1_FUNCTIONS_H

#include "4_1.h"

/// This will be used for insertion, returning NULL if the value is already in the node
BTreeNode *b_tree_search(const BTree *tree, const int value);

/// This will be used for removal, returning a node only if the value is in the node
BTreeNode *b_tree_find_key(const BTree *tree, const int value);

void handle_underflow(BTreeNode *node, BTree *tree);

void b_tree_remove(BTree *tree, const int value);

/// Assumes the value is not already in the tree
int b_tree_add(BTree *tree, const int value);

int parse_config(BTree *tree);

int generate_config(const size_t maximum_size);


#endif //DATA_STRUCTURES_4_1_FUNCTIONS_H
