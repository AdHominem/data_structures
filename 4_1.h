#ifndef DATA_STRUCTURES_4_1_H
#define DATA_STRUCTURES_4_1_H

#include <stdlib.h>

#define DEGREE 4

typedef struct b_tree_node BTreeNode;
typedef enum {NODE, LEAF} node_type;
typedef enum {int_type, node_pointer_type} data_type;
struct b_tree_node {
    node_type type_of_node;
    size_t children_count;
    size_t keys_count;
    int keys[DEGREE - 1];
    // +1 because in case of overflow the parent node can temporarily have DEGREE + 1 children
    BTreeNode *children[DEGREE + 1];
    BTreeNode *parent;
};
typedef struct b_tree {
    BTreeNode *root;
} BTree;
typedef struct linked_list_node LinkedListNode;
struct linked_list_node {
    BTreeNode * value;
    LinkedListNode *next;
};
typedef struct linked_list {
    LinkedListNode *head;
} LinkedList;

#endif //DATA_STRUCTURES_4_1_H
