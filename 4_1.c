#define _GNU_SOURCE
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>

#define DEGREE 3

typedef struct b_tree_node BTreeNode;
typedef enum {NODE, LEAF} node_type;
typedef enum {int_type, float_type, string_type, invalid} data_type;
struct b_tree_node {
    node_type type_of_node;
    size_t children_count;
    size_t keys_count;
    size_t keys[DEGREE - 1];
    BTreeNode *children[DEGREE];
    BTreeNode *parent;
};

typedef struct b_tree {
    BTreeNode *root;
} BTree;

int compare_size_t(const void *first, const void *second) {
    size_t a = *(size_t*) first;
    size_t b = *(size_t*) second;
    return a < b ? -1 : a > b;
}

int value_in_array(size_t *array, size_t size, size_t value) {
    for (size_t i = 0; i < size; ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

BTreeNode *b_tree_search_internal(BTreeNode *node, size_t value) {
    if (node == NULL) {
        return NULL;
    } else if (value_in_array(node->keys, node->keys_count, value)) {
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

BTreeNode *b_tree_search(BTree *tree, size_t value) {
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

void b_tree_insert(BTreeNode *node, size_t value) {
    size_t keys_count = node->keys_count;
    size_t *keys = node->keys;

    // try to insert in root
    if (keys_count < DEGREE - 1) {
        keys[keys_count] = value;
        qsort(keys, keys_count + 1, sizeof(size_t), compare_size_t);
        node->keys_count++;
    } else {
        // insert virtually
        size_t temp[DEGREE];
        memcpy(temp, keys, keys_count * sizeof(size_t));
        temp[keys_count] = value;
        qsort(temp, DEGREE, sizeof(size_t), compare_size_t);
        // determine middle
        size_t middle = temp[DEGREE / 2];

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

        BTreeNode *new_root = b_tree_node_create();
        new_root->keys[0] = middle;
        new_root->keys_count++;

        // Linking
        first_child->parent = new_root;
        second_child->parent = new_root;
        new_root->children[0] = first_child;
        new_root->children[1] = second_child;
        new_root->children_count = 2;
        new_root->type_of_node = NODE;

        free(node);

        node = new_root;
    }
}

void b_tree_add(BTree *tree, size_t value) {

    if (b_tree_search(tree, value)) {
        return;
    }

    // if root is null, add a new one with value as first key
    if (tree->root) {
        b_tree_insert(tree->root, value);
    } else {
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
            printf("%zu", node->keys[j]);
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

int parse_config(BTree *tree) {
    FILE *config = fopen("b_tree.config", "r");
    if (config == NULL) return 1;

    char *line = NULL;
    size_t line_length = 0;

    while (getline(&line, &line_length, config) != -1) {
        char *end;
        size_t input = (size_t) strtol(line + 1, &end, 10);
        if (*line == '+') {
            printf("Adding %zu\n", input);
            b_tree_add(tree, input);
        } else if (*line == '-') {
            printf("Removing %zu\n", input);
        }
    }

    fclose(config);
    free(line);
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

