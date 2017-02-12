#include "4_1_functions.h"
#include "4_1_helper.h"
#include <memory.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

BTreeNode *b_tree_search_internal(const BTreeNode *node, const int value) {
    if (array_contains(node->keys, node->keys_count, &value, int_type)) {
        return NULL;
    } else if (node->type_of_node == LEAF) {
        // if we are at a leaf, return. we can not go down further!
        return (BTreeNode *) node;
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

BTreeNode *b_tree_find_key_internal(const BTreeNode *node, const int value) {
    if (array_contains(node->keys, node->keys_count, &value, int_type)) {
        return (BTreeNode *) node;
    } else if (node->type_of_node == LEAF) {
        // if we are at a leaf, return. we can not go down further!
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

/// This will be used for insertion, returning NULL if the value is already in the node
BTreeNode *b_tree_search(const BTree *tree, const int value) {
    return b_tree_search_internal(tree->root, value);
}

/// This will be used for removal, returning a node only if the value is in the node
BTreeNode *b_tree_find_key(const BTree *tree, const int value) {
    return b_tree_find_key_internal(tree->root, value);
}

void b_tree_insert(BTreeNode *node, const int value, BTree *tree) {
    size_t keys_count = node->keys_count;
    int *keys = node->keys;

    // Case 1: Try to insert in node
    if (keys_count < DEGREE - 1) {
        keys[keys_count] = value;
        qsort(keys, keys_count + 1, sizeof(int), compare_ints);
        node->keys_count++;
    } else {
        // Case 2: If it doesn't fit, make a temp array to hold all the values
        int temp[DEGREE];
        memcpy(temp, keys, keys_count * sizeof(int));
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

        // if i have DEGREE + 1 children that means one of my children had an overflow and I need to restructure the tree
        if (node->children_count == DEGREE + 1) {

            // distribute child nodes to the newly created two children.
            // each child gets keys_count + 1 children
            // left half of children go to first child
            for (size_t i = 0; i < first_child->keys_count + 1; ++i) {
                b_tree_link_child_insert(first_child, node->children[i], i);
            }

            // right half of children go to second child
            for (size_t i = first_child->keys_count + 1, j = 0; i < DEGREE + 1; ++i, ++j) {
                b_tree_link_child_insert(second_child, node->children[i], j);
            }
        }

        // we only need to create a new root when the old node was the root
        if (node->parent == NULL) {
            free(node);
            node = b_tree_node_create();
            node->keys[0] = middle;
            node->keys_count++;

            // Linking
            b_tree_link_child_insert(node, first_child, 0);
            b_tree_link_child_insert(node, second_child, 1);

            tree->root = node;
        } else {
            // otherwise we just have to add the middle to the parent node
            ssize_t insertion_index = b_tree_node_get_index((const BTreeNode **) node->parent->children, node->parent->children_count, node);
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

void handle_underflow(BTreeNode *node, BTree *tree) {
    if (node != tree->root && node->keys_count < ceil(DEGREE / 2.0) - 1) {
        size_t index_in_parent = (size_t) b_tree_node_get_index((const BTreeNode **) node->parent->children, node->parent->children_count, node);
        BTreeNode *compensator = NULL;
        BTreeNode *left_sibling = NULL;
        BTreeNode *right_sibling = NULL;
        BTreeNode *parent = node->parent;
        int right = false;

        if (index_in_parent < parent->keys_count) {
            right_sibling = parent->children[index_in_parent + 1];
            if (right_sibling->keys_count > ceil(DEGREE / 2)) {
                compensator = right_sibling;
            }
            right = true;
        } else if (index_in_parent > 0) {
            left_sibling = parent->children[index_in_parent - 1];
            if (left_sibling->keys_count > ceil(DEGREE / 2)) {
                compensator = left_sibling;
            }
        }

        // Case 1: One siblings can compensate
        if (compensator) {
            int to_insert;
            // if left, then the value to insert will be the rightmost, else leftmost
            if (right) {
                to_insert = compensator->keys[0];
            } else {
                to_insert = compensator->keys[compensator->keys_count - 1];
            }

            // pass that value to parent
            // note that parent keys can overflow here, so we need to perform this on a temp array and update parent later
            array_delete(compensator->keys, &compensator->keys_count, &to_insert, int_type);
            size_t temp_size = parent->keys_count + 1;
            int temp[temp_size];
            memcpy(temp, parent->keys, parent->keys_count * sizeof(int));
            temp[temp_size - 1] = to_insert;
            qsort(temp, temp_size, sizeof(int), compare_ints);

            // if right, move the leftmost value of temp into node, else rightmost
            size_t index_to_insert = right ? 0 : temp_size - 1;
            b_tree_insert(node, temp[index_to_insert], tree);

            // update temp
            array_delete(temp, &temp_size, &temp[index_to_insert], int_type);

            // now update the parent
            for (size_t i = 0; i < parent->keys_count; ++i) {
                parent->keys[i] = temp[i];
            }

            // now transfer the child from sibling to node
            if (compensator->type_of_node == NODE) {
                if (right) {
                    // take the leftmost child of sibling
                    BTreeNode *child = compensator->children[0];
                    b_tree_link_child_add(node, child);
                    array_delete(compensator->children, &compensator->children_count,
                                 child, node_pointer_type);
                } else {
                    // take the rightmost one
                    BTreeNode *child = compensator->children[compensator->children_count - 1];
                    b_tree_link_child_insert(node, child, 0);
                    array_delete(compensator->children, &compensator->children_count,
                                 child, node_pointer_type);
                }
            }
        } else {
            // Case 2: Merge with sibling
            // move the extreme key from parent down
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
            size_t index = (size_t) b_tree_node_get_index((const BTreeNode **) parent->children, parent->children_count, sibling);

            // if merge with right, then move the index-1-th key into sibling
            // else move the index-th key into sibling
            if (right) --index;
            b_tree_insert(sibling, parent->keys[index], tree);

            // remove old node (which is empty anyways)
            array_delete(parent->children, &parent->children_count, node, node_pointer_type);
            free(node);

            // remove the moved key from parent
            array_delete(parent->keys, &parent->keys_count, &parent->keys[index], int_type);

            // finally check if parent underflows now
            handle_underflow(parent, tree);
        }
    } else if (node->keys_count == 0 && node->children_count == 1) {
        // This happens when the root node has an underflow and one child.
        // Note that we do not need to transfer any children because root can only underflow with 0 keys
        free(tree->root);
        tree->root = node->children[0];
        node->children[0]->parent = NULL;
    }
}

void b_tree_remove_internal(BTreeNode *node, const int value, BTree *tree) {

    if (node->type_of_node == LEAF) {
        // Case 1: LEAF
        array_delete(node->keys, &node->keys_count, &value, int_type);
    } else {
        // Case 2: NODE
        // Internal node contains the value
        // replace value with the next largest value
        BTreeNode *node_with_next_largest_value = b_tree_find_next_largest_value(node, value);
        ssize_t index_to_replace = array_get_index(node->keys, value);
        int next_largest_value = node_with_next_largest_value->keys[0];
        node->keys[index_to_replace] = next_largest_value;

        // note that we do not actually perform the swap here but instead just delete the next largest which would be
        // replaced by the target value anyways
        array_delete(node_with_next_largest_value->keys, &node_with_next_largest_value->keys_count,
                     &next_largest_value, int_type);
        node = node_with_next_largest_value;
    }

    handle_underflow(node, tree);
}

void b_tree_remove(BTree *tree, const int value) {
    if (tree->root) {
        // get the fitting node
        BTreeNode *node = b_tree_find_key(tree, value);

        // remove if the value is inside
        if (node && array_contains(node->keys, node->keys_count, &value, int_type)) {
            b_tree_remove_internal(node, value, tree);
        }
    }
}

/// Assumes the value is not already in the tree
int b_tree_add(BTree *tree, const int value) {
    if (tree->root) {
        // get the fitting node. This will return NULL if the value already happens to be inside that node
        BTreeNode *node = b_tree_search(tree, value);

        if (node) {
            b_tree_insert(node, value, tree);
        } else {
            return 1;
        }
    } else {
        // if root is null, add a new one with value as first key
        tree->root = b_tree_node_create();
        tree->root->keys[0] = value;
        tree->root->keys_count++;
    }
    return 0;
}

int parse_config(BTree *tree) {
    FILE *config = fopen("b_tree.config", "r");
    if (config == NULL) return 1;

    char line[BUFSIZ];
    while (fgets(line, BUFSIZ, config)) {
        int input;

        if (*line == '-' && !string_to_integer(line + 1, &input)) {
            b_tree_remove(tree, input);
        } else if (!string_to_integer(line, &input)) {
            b_tree_add(tree, input);
        }
    }

    fclose(config);
    return 0;
}

int generate_config(const size_t maximum_size) {
    FILE *config = fopen("b_tree.config", "w");
    if (config == NULL) return -1;

    srand((unsigned int) time(NULL));
    int operations = rand() % (int) maximum_size;
    if (operations < 3) operations = 3;

    fprintf(config, "i\n");

    for (int i = 0; i < operations; ++i) {
        char *operation = rand() % 2 ? "%d\n" : "-%d\n";
        fprintf(config, operation, rand() % operations);
    }

    fclose(config);
    return operations;
}

