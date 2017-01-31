#define _GNU_SOURCE
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <values.h>
#include <asm/errno.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>


#define DEGREE 3

typedef struct b_tree_node BTreeNode;
typedef enum {NODE, LEAF} node_type;
typedef enum {int_type, float_type, string_type, invalid} data_type;
struct b_tree_node {
    node_type type_of_node;
    size_t children_count;
    size_t keys_count;
    int keys[DEGREE - 1];
    BTreeNode *children[DEGREE];
    BTreeNode *parent;
};

typedef struct b_tree {
    BTreeNode *root;
} BTree;

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

// this will ALWAYS return a node since there is always a node where a value is expected to be
BTreeNode *b_tree_search_internal(BTreeNode *node, int value) {

    // if we are at a leaf, return. we can not go down further!
    if (node->type_of_node == LEAF) {
        return node;
    } else {
        //search in left subtrees
        for (size_t i = 0; i < node->keys_count; ++i) {
            if (node->keys[i] > value) {
                return b_tree_search_internal(node->children[i], value);
            }
        }
        // if the value is too big, try fitting it in the right child node
        return b_tree_search_internal(node->children[node->keys_count], value);
    }
}

// Checks if a given value is in the tree and returns a pointer to the Node which is expected to contain it
BTreeNode *b_tree_search(BTree *tree, int value) {
    return b_tree_search_internal(tree->root, value);
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
    for (int i = 0; i < DEGREE; ++i) {
        if (array[i] == value) {
            return i;
        }
    }
    return -1;
}

void b_tree_link_child(BTreeNode *parent, BTreeNode *child, ssize_t index) {
    child->parent = parent;
    parent->children[index] = child;
}

/*
 * There are 4 cases:
 * - fits into key array
 * - root node flows over (easy split, 2 children will be created)
 * - regular node flows over
 *      pass middle value to the parent.
 *      In this case the parent will ALWAYS have DEGREE + 1 children and needs to
 */
void b_tree_insert(BTreeNode *node, int value, BTree *tree) {
    size_t keys_count = node->keys_count;
    int *keys = node->keys;

    // try to insert in node
    if (keys_count < DEGREE - 1) {
        keys[keys_count] = value;
        qsort(keys, keys_count + 1, sizeof(int), compare_ints);
        node->keys_count++;
    } else {
        // if it doesn't fit, insert virtually
        int temp[DEGREE];
        memcpy(temp, keys, keys_count * sizeof(size_t));
        temp[keys_count] = value;
        qsort(temp, DEGREE, sizeof(int), compare_ints);

        // determine middle
        int middle = temp[DEGREE / 2];

        // extract middle, create new root and split old one
        BTreeNode *first_child = b_tree_node_create();
        for (size_t i = 0; i < DEGREE / 2; ++i) {
            first_child->keys[i] = temp[i];
            first_child->keys_count++;
        }

        BTreeNode *second_child = b_tree_node_create();
        for (size_t i = DEGREE / 2 + 1; i < DEGREE; ++i) {
            second_child->keys[i - (DEGREE / 2 + 1)] = temp[i];
            second_child->keys_count++;
        }

        // we only need to create a new root when the old node was the root
        if (node->parent == NULL) {
            node = b_tree_node_create();
            node->keys[0] = middle;
            node->keys_count++;

            // Linking
            first_child->parent = node;
            second_child->parent = node;
            b_tree_link_child(node, first_child, 0);
            b_tree_link_child(node, second_child, 1);
            node->children_count = 2;
            node->type_of_node = NODE;

            tree->root = node;
        } else {
            // otherwise we just have to add the middle to the parent node
            // careful: linking here depends on the position of the middle element
            ssize_t middle_index = get_index_for_value(temp, middle);
            if (middle_index == -1) exit(1);

            // Linking
            b_tree_link_child(node->parent, first_child, middle_index);
            b_tree_link_child(node->parent, second_child, middle_index + 1);
            node->parent->children_count += 1;

            b_tree_insert(node->parent, middle, tree);
        }
    }
}

void b_tree_add(BTree *tree, int value) {
    if (tree->root) {
        // get the fitting node
        BTreeNode *node = b_tree_search(tree, value);

        // add if the value is not already inside
        if (!value_in_array(node->keys, node->keys_count, value)) {
            // if the node is the root, we need to link a bit different. That's why we need to pass the tree
            b_tree_insert(node, value, tree);
        }
    } else {
        // if root is null, add a new one with value as first key
        tree->root = b_tree_node_create();
        tree->root->keys[0] = value;
        tree->root->keys_count++;
    }

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

// FUNCTIONS

/// Prints tree_node keys, also showing how many of the key slots are still unset
/// \param node The node whose keys are to be printed
void b_tree_node_keys_print(BTreeNode *node) {
    printf("[");
    for (size_t j = 0; j < DEGREE - 1; ++j) {
        if (j < node->keys_count) {
            printf("%d", node->keys[j]);
        } else {
            printf(" ");
        }

        if (j != node->keys_count - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
//
///// Checks in which child tree_node the value may fit, or if it fits inside the key array.
///// Will create a new child tree_node if necessary.
///// \param node The node from which to start insertion
///// \param value The value to insert
///// \return 0 if insertion was successful or 1 if creating a new node was impossible due to memory error and 2 if the
///// given node was NULL and 3 if the function terminates unexpectedly
//int b_tree_add_internal(BTreeNode *node, int value) {
//
//    if (!node) {
//        return 2;
//    }
//
//    for (size_t i = 0; i < node->degree - 1; i++) {
//
//        if (i == node->last_key_index) {
//            node->keys->elements[i] = value;
//            node->last_key_index += 1;
//            return 0;
//        }
//        else if (node->keys->elements[i] >= value) {
//            if (node->children->elements[i] != NULL) {
//                return b_tree_add_internal(node->children->elements[i], value);
//            }
//            else {
//                node->children->elements[i] = b_tree_node_create(node->degree);
//                if (node->children->elements[i] == NULL) {
//                    return 1;
//                }
//                node->children->elements[i]->keys->elements[0] = value;
//                node->children->elements[i]->last_key_index += 1;
//                return 0;
//            }
//        }
//        else if (i == node->degree - 2){
//            if (node->children->elements[i + 1] != NULL) {
//                return b_tree_add_internal(node->children->elements[i + 1], value);
//            } else {
//                node->children->elements[i + 1] = b_tree_node_create(node->degree);
//                if (node->children->elements[i + 1] == NULL) {
//                    return 1;
//                }
//                node->children->elements[i + 1]->keys->elements[0] = value;
//                node->children->elements[i + 1]->last_key_index += 1;
//                return 0;
//            }
//        }
//    }
//
//    return 3;
//}
//
///// Adds a new value to the tree and creates a root tree_node if the tree is empty
///// \param tree The tree to extend
///// \param value The value to insert
///// \return 0 if insertion was successful, else 1 in case of memory error
//int b_tree_add(BTree *tree, int value) {
//    if (tree->root == NULL) {
//        tree->root = b_tree_node_create(tree->degree);
//        if (tree->root == NULL) {
//            return 1;
//        }
//        tree->root->keys->elements[0] = value;
//        tree->root->last_key_index = 1;
//    } else {
//        if (b_tree_add_internal(tree->root, value)) {
//            return 1;
//        }
//    }
//    return 0;
//}

/// Prints all the nodes recursively, indenting them for better readability
/// \param node The node to start printing from
/// \param depth The depth of printing, this should be 1 initially
void b_tree_nodes_print(BTreeNode *node, size_t depth){
    if (node) {

        // call this on all child nodes except for the last
        for (size_t i = 0; i+1 < node->children_count; i++) {
            b_tree_nodes_print(node->children[i], depth + 1);
        }

        for (size_t indentations = 1; indentations < depth; indentations++) {
            printf("\t");
        }

        b_tree_node_keys_print(node);

        // print the last child
        b_tree_nodes_print(node->children[node->keys_count], depth + 1);
    }
}

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void b_tree_print(BTree *tree) {
    b_tree_nodes_print(tree->root, 1);
}

//BTreeNode *b_tree_search_internal(BTreeNode *node, int value) {
//
//    if (!node) {
//        return NULL;
//    }
//
//    for (size_t i = 0; i < node->last_key_index; ++i) {
//
//        // Check if value is in the keys
//        if (node->keys->elements[i] == value) {
//            return node;
//        }
//
//            // Else check if it's too small to fit, so we need to go down the left subtree
//        else if (value < node->keys->elements[i]) {
//            return b_tree_search_internal(node->children->elements[i], value);
//        }
//
//            // Else check if we are at the last key
//        else if (i == node->degree - 2) {
//            // Now value is bigger than the last element and could only be in the right child
//            // Only the last key in the array can have a right child
//            return b_tree_search_internal(node->children->elements[i + 1], value);
//        }
//    }
//    return NULL;
//}
//
//// Returns a pointer to the containing TreeNode or NULL, if value not in graph
//BTreeNode *b_tree_search(BTree *tree, int value) {
//    return b_tree_search_internal(tree->root, value);
//}

int string_to_integer(const char *string, int *integer) {
    char *p;
    const long value = strtol(string, &p, 10);
    if (!(p == string || LONG_MIN == value || LONG_MAX == value || value > INT_MAX || value < INT_MIN)) {
        *integer = (int) value;
        return 0;
    }
    return 1;
}

int parse_config(BTree *tree) {
    FILE *config = fopen("b_tree.config", "r");
    if (config == NULL) return 1;

    char line[BUFSIZ];
    while (fgets(line, BUFSIZ, config)) {
        int input;
        if (*line == '-' && !string_to_integer(line + 1, &input)) {
            printf("Removing %d\n", input);
            //b_tree_remove(tree, input);
        } else if (!string_to_integer(line, &input)) {
            printf("Adding %d\n", input);
            b_tree_add(tree, input);
        }
    }

    fclose(config);
    return 0;
}

data_type get_input_type() {
    FILE *config = fopen("b_tree.config", "r");
    if (config == NULL)
        return invalid;

    int identifier = fgetc(config);
    fclose(config);

    return identifier == 'i' ? int_type
          : identifier == 'f' ? float_type
          : identifier == 's' ? string_type
          : invalid;
}

int main() {

    BTree *tree = b_tree_create();

    int status = parse_config(tree);
    if (status) {
        b_tree_destroy(tree);
        printf(status == 1 ? "Error: Config file could not be opened!\n" : "Error: Malformed config file!\n");
        return 1;
    }

    b_tree_print(tree);

    b_tree_destroy(tree);
    return 0;
}

