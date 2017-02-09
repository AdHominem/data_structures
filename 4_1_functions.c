#include "4_1_functions.h"
#include "4_1_helper.h"
#include <memory.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

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

// returns NULL in case there is no such key
BTreeNode *b_tree_find_key_internal(BTreeNode *node, int value) {

    if (array_contains(node->keys, node->keys_count, &value, int_type)) {
        return node;
    } else {
        // if we are at a leaf, return. we can not go down further!
        if (node->type_of_node == LEAF) {
            return NULL;
        } else {
            //search in left subtrees
            for (size_t i = 0; i < node->keys_count; ++i) {
                if (node->keys[i] > value) {
                    return b_tree_find_key_internal(node->children[i], value);
                }
            }
            // if the value is too big, try searching it in the right child node
            return b_tree_find_key_internal(node->children[node->keys_count], value);
        }
    }
}

// Checks if a given value is in the tree and returns a pointer to the Node which is expected to contain it
// note that this will assume you want to insert a new value. if you want to find a node with a specific key, use find
BTreeNode *b_tree_search(BTree *tree, int value) {
    return b_tree_search_internal(tree->root, value);
}

// returns a node containing a defined key
BTreeNode *b_tree_find_key(BTree *tree, int value) {
    return b_tree_find_key_internal(tree->root, value);
}

void b_tree_link_child_insert(BTreeNode *parent, BTreeNode *child, size_t index) {
    child->parent = parent;
    array_insert(parent->children, &parent->children_count, index, child);
    parent->type_of_node = NODE;
}

void b_tree_link_child_add(BTreeNode *parent, BTreeNode *child) {
    child->parent = parent;
    array_add(parent->children, &parent->children_count, child);
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

        // always print the key or a blank space
        if (j < node->keys_count) {
            printf("%d", node->keys[j]);
        } else {
            printf(" ");
        }

        // print a comma except during the last iteration
        if (j != DEGREE - 2) {
            printf(",");
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
        size_t middle_index = DEGREE / 2;
        int middle = temp[middle_index];

        // create left child
        BTreeNode *first_child = b_tree_node_create();
        for (size_t i = 0; i < middle_index; ++i) {
            first_child->keys[i] = temp[i];
            first_child->keys_count++;
        }

        // create right child
        BTreeNode *second_child = b_tree_node_create();
        for (size_t i = middle_index + 1; i < DEGREE; ++i) {
            second_child->keys[i - (middle_index + 1)] = temp[i];
            second_child->keys_count++;
        }

        // if i have DEGREE + 1 children that means i need to restructure the tree
        // that means linking the children appropriately
        if (node->children_count == DEGREE + 1) {

            // left half of children go to first child
            for (size_t i = 0; i < (DEGREE + 1) / 2; ++i) {
                b_tree_link_child_insert(first_child, node->children[i], i);
            }

            // right half of children go to second child
            for (size_t i = (DEGREE + 1) / 2, j = 0; i < DEGREE + 1; ++i, ++j) {
                b_tree_link_child_insert(second_child, node->children[i], j);
            }
        }

        // we only need to create a new root when the old node was the root
        if (node->parent == NULL) {
            node = b_tree_node_create();
            node->keys[0] = middle;
            node->keys_count++;

            // Linking
            b_tree_link_child_insert(node, first_child, 0);
            b_tree_link_child_insert(node, second_child, 1);

            tree->root = node;
        } else {
            // otherwise we just have to add the middle to the parent node
            ssize_t insertion_index = get_index_for_node(node->parent->children, node->parent->children_count, node);
            if (insertion_index == -1) {
                exit(1);
            }

            // Linking
            b_tree_overwrite_child(node->parent, first_child, (size_t) insertion_index);
            b_tree_link_child_insert(node->parent, second_child, (size_t) insertion_index + 1);

            b_tree_insert(node->parent, middle, tree);
            free(node);
        }
    }
}

BTreeNode *b_tree_find_next_largest_value(BTreeNode *node, int value) {

    // go right subtree of this key (every key has one, index will be key index + 1)
    BTreeNode *current = node->children[get_index_for_value(node->keys, value) + 1];

    // then go very left as long as there are nodes and return
    while (current->type_of_node != LEAF) {
        current = current->children[0];
    }

    return current;
}

void handle_underflow(BTreeNode *node, BTree *tree) {
    // UNDERFLOW DETECTION
    if (node != tree->root && node->keys_count < ceil(DEGREE / 2)) {
        // Leaf contains not enough keys so we have to somehow adjust it
        printf("Underflow detected!\n");

        size_t index_in_parent = (size_t) get_index_for_node(node->parent->children, node->parent->children_count, node);
        BTreeNode *sibling_who_can_compensate = NULL;
        BTreeNode *left_sibling = NULL;
        BTreeNode *right_sibling = NULL;
        BTreeNode *parent = node->parent;
        int right = false;

        if (index_in_parent < parent->keys_count) {
            // have right
            right_sibling = parent->children[index_in_parent + 1];
            if (right_sibling->keys_count > ceil(DEGREE / 2)) {
                sibling_who_can_compensate = right_sibling;
            }
            right = true;
        } else if (index_in_parent > 0) {
            // have left
            left_sibling = parent->children[index_in_parent - 1];
            if (left_sibling->keys_count > ceil(DEGREE / 2)) {
                sibling_who_can_compensate = left_sibling;
            }
        }

        // if siblings can compensate
        if (sibling_who_can_compensate) {
            int to_insert;
            // if left, then the value to insert will be the rightmost
            // else it will be the leftmost
            if (right) {
                to_insert = sibling_who_can_compensate->keys[0];
            } else {
                to_insert = sibling_who_can_compensate->keys[sibling_who_can_compensate->keys_count - 1];
            }
            printf("One sibling can compensate with %d!\n", to_insert);

            // pass that value to parent
            array_delete(sibling_who_can_compensate->keys, &sibling_who_can_compensate->keys_count, &to_insert,
                         int_type);
            b_tree_insert(parent, to_insert, tree);

            // if right, move the leftmost value of parent into node
            // if left, move the rightmost value of parent into node
            size_t index_to_insert = right ? 0 : parent->keys_count - 1;
            b_tree_insert(node, parent->keys[index_to_insert], tree);
            array_delete(parent->keys, &parent->keys_count, &parent->keys[index_to_insert], int_type);

        } else {
            // else
            // move the extreme key from parent down
            printf("I have to merge with one sibling!\n");
            BTreeNode *sibling = right ? right_sibling : left_sibling;

            // insert all keys of node into this sibling
            for (size_t i = 0; i < node->keys_count; ++i) {
                array_int_add_sorted(sibling->keys, &sibling->keys_count, node->keys[i]);
            }

            // add all children of node into this sibling
            // order matters, so:
            // if sibling is right, we append all children from right to left into index 0
            // if sibling is left, we append all children from left to right at the end
            if (node->type_of_node == NODE) {
                if (right) {
                    for (size_t j = node->children_count - 1; j < node->children_count; --j) {
                        b_tree_link_child_insert(sibling, node->children[j], 0);
                    }
                } else {
                    for (size_t i = 0; i < node->children_count; ++i) {
                        b_tree_link_child_add(sibling, node->children[i]);
                    }
                }
            }

            // get siblings index
            size_t index = (size_t) get_index_for_node(parent->children, parent->children_count, sibling);

            // if merge with right, then move the index-1-th key into sibling
            // else move the index-th key into sibling
            if (right) --index;
            b_tree_insert(sibling, parent->keys[index], tree);

            // remove old node (which is empty anyways)
            array_delete(parent->children, &parent->children_count, node, node_pointer_type);

            // remove the moved key from parent
            array_delete(parent->keys, &parent->keys_count, &parent->keys[index], int_type);

            // if parent has zero keys now, swap
//            if (parent->keys_count == 0 && parent->parent == NULL) {
//                tree->root = sibling;
//                sibling->parent = NULL;
//            }

            // finally check if parent underflows now
            handle_underflow(parent, tree);
        }
    }
}

void b_tree_remove_internal(BTreeNode *node, int value, BTree *tree) {

    // DELETION
    if (node->type_of_node == LEAF) {
        // LEAF
        array_delete(node->keys, &node->keys_count, &value, int_type);
    } else {
        // NODE
        // Internal node contains the value
        // replace value with the next largest value
        BTreeNode *node_with_next_largest_value = b_tree_find_next_largest_value(node, value);
        ssize_t index_to_replace = get_index_for_value(node->keys, value);
        int next_largest_value = node_with_next_largest_value->keys[0];
        node->keys[index_to_replace] = next_largest_value;
        // delete value
        // note that we do not actually perform the swap here but instead just delete the next largest which would be
        // replaced by the target value anyways
        array_delete(node_with_next_largest_value->keys, &node_with_next_largest_value->keys_count,
                     &next_largest_value, int_type);
        node = node_with_next_largest_value;
    }

    handle_underflow(node, tree);
}

void b_tree_remove(BTree *tree, int value) {
    if (tree->root) {
        // get the fitting node
        BTreeNode *node = b_tree_find_key(tree, value);

        // remove if the value is inside
        if (node && value_in_array(node->keys, node->keys_count, value)) {
            // if the node is the root, we need to link a bit different. That's why we need to pass the tree
            b_tree_remove_internal(node, value, tree);
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
            b_tree_remove(tree, input);
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

