#include "4_1_functions.h"
#include "4_1_helper.h"
#include <memory.h>
#include <stdio.h>

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

// does all the linking for two nodes and also updates the LEAF / NODE status
// note that this will not change the children count since you can just overwrite a child reference
void b_tree_link_child(BTreeNode *parent, BTreeNode *child, size_t index) {
    child->parent = parent;
    insert_into_array(parent->children, parent->children_count, index, child);
    parent->type_of_node = NODE;
}

void b_tree_overwrite_child(BTreeNode *parent, BTreeNode *child, size_t index) {
    child->parent = parent;
    parent->children[index] = child;
    parent->type_of_node = NODE;
}

ssize_t get_index_for_node(BTreeNode **node_array, size_t size, BTreeNode *node) {
    if (node == NULL) return -1;

    for (size_t i = 0; i < size; ++i) {
        if (node_array[i] == node) {
            return i;
        }
    }

    return -1;
}

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
        // if it doesn't fit, make a temp array to hold all the values
        int temp[DEGREE];
        memcpy(temp, keys, keys_count * sizeof(size_t));
        temp[keys_count] = value;
        qsort(temp, DEGREE, sizeof(int), compare_ints);

        // determine middle
        int middle = temp[DEGREE / 2];

        // create left child
        BTreeNode *first_child = b_tree_node_create();
        for (size_t i = 0; i < DEGREE / 2; ++i) {
            first_child->keys[i] = temp[i];
            first_child->keys_count++;
        }

        // create right child
        BTreeNode *second_child = b_tree_node_create();
        for (size_t i = DEGREE / 2 + 1; i < DEGREE; ++i) {
            second_child->keys[i - (DEGREE / 2 + 1)] = temp[i];
            second_child->keys_count++;
        }

        // if i have DEGREE + 1 children that means i need to restructure the tree
        // that means linking the children appropriately
        if (node->children_count == DEGREE + 1) {
            b_tree_node_keys_print(node);
            printf(" here, i have too many children, will restructure!\n");

            // left half of children go to first child
            for (size_t i = 0; i < (DEGREE + 1) / 2; ++i) {
                b_tree_link_child(first_child, node->children[i], i);
                first_child->children_count++;
            }

            // right half of children go to second child
            for (size_t i = (DEGREE + 1) / 2, j = 0; i < DEGREE + 1; ++i, ++j) {
                b_tree_link_child(second_child, node->children[i], j);
                second_child->children_count++;
            }
        }

        // we only need to create a new root when the old node was the root
        if (node->parent == NULL) {
            node = b_tree_node_create();
            node->keys[0] = middle;
            node->keys_count++;

            // Linking
            b_tree_link_child(node, first_child, 0);
            node->children_count++;
            b_tree_link_child(node, second_child, 1);
            node->children_count++;
            node->type_of_node = NODE;

            tree->root = node;
        } else {
            // otherwise we just have to add the middle to the parent node
            ssize_t insertion_index = get_index_for_node(node->parent->children, node->parent->children_count, node);
            if (insertion_index == -1) {
                exit(1);
            }

            // Linking
            b_tree_overwrite_child(node->parent, first_child, (size_t) insertion_index);
            b_tree_link_child(node->parent, second_child, (size_t) insertion_index + 1);
            node->parent->children_count += 1;

            b_tree_insert(node->parent, middle, tree);
            free(node);
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

        // every node except a leaf always has a last child, too
        if (node->type_of_node == NODE) {
            b_tree_nodes_print(node->children[node->keys_count], depth + 1);
        }
    }
}

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void b_tree_print(BTree *tree) {
    b_tree_nodes_print(tree->root, 1);
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
            b_tree_print(tree);
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

