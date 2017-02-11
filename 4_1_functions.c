#include "4_1_functions.h"
#include "4_1_helper.h"
#include <memory.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// this will ALWAYS return a node since there is always a node where a value is expected to be
BTreeNode *b_tree_search_internal(BTreeNode *node, int value) {

    if (array_contains(node->keys, node->keys_count, &value, int_type)) {
        return NULL;
    }

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
        memcpy(temp, keys, keys_count * sizeof(int));
        temp[keys_count] = value;
        qsort(temp, DEGREE, sizeof(int), compare_ints);

        //determine middle
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
    if (node->keys[0] == 83) {
        b_tree_print(tree);
        printf("ho");
    }

    if (node != tree->root && node->keys_count < ceil(DEGREE / 2)) {
        // Leaf contains not enough keys so we have to somehow adjust it
        size_t index_in_parent = (size_t) get_index_for_node(node->parent->children, node->parent->children_count, node);
        BTreeNode *sibling_who_can_compensate = NULL;
        BTreeNode *left_sibling = NULL;
        BTreeNode *right_sibling = NULL;
        BTreeNode *parent = node->parent;
        int right = false;

        // It's NOT enough to check if they have enough keys!!! Also the siblings must be leafs
        // TODO: They must not be leafs but if they aren't, we need to transfer one child too
        if (index_in_parent < parent->keys_count) {
            // have right
            right_sibling = parent->children[index_in_parent + 1];
            if (right_sibling->type_of_node == LEAF && right_sibling->keys_count > ceil(DEGREE / 2)) {
                sibling_who_can_compensate = right_sibling;
            }
            right = true;
        } else if (index_in_parent > 0) {
            // have left
            left_sibling = parent->children[index_in_parent - 1];
            if (left_sibling->type_of_node == LEAF && left_sibling->keys_count > ceil(DEGREE / 2)) {
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

            // else merge with sibling!
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
            size_t index = (size_t) get_index_for_node(parent->children, parent->children_count, sibling);

            // if merge with right, then move the index-1-th key into sibling
            // else move the index-th key into sibling
            if (right) --index;
            b_tree_insert(sibling, parent->keys[index], tree);

            // remove old node (which is empty anyways)
            array_delete(parent->children, &parent->children_count, node, node_pointer_type);

            // remove the moved key from parent
            array_delete(parent->keys, &parent->keys_count, &parent->keys[index], int_type);

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
    // make sure value is not already inside
    if (tree->root) {
        // get the fitting node. This will return NULL if the value already happens to be inside that node
        BTreeNode *node = b_tree_search(tree, value);

        // if the node is the root, we need to link a bit different. That's why we need to pass the tree
        if (node) {
            b_tree_insert(node, value, tree);
        }
    } else {
        // if root is null, add a new one with value as first key
        tree->root = b_tree_node_create();
        tree->root->keys[0] = value;
        tree->root->keys_count++;
    }
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

int generate_config() {
    FILE *config = fopen("b_tree.config", "w");
    if (config == NULL) return -1;

    srand((unsigned int) time(NULL));
    int operations = rand() % 2000;

    fprintf(config, "i\n");

    for (int i = 0; i < operations; ++i) {
        char *operation = rand() % 7 ? "%d\n" : "-%d\n";
        fprintf(config, operation, rand() % operations);
    }

    fclose(config);
    return operations;
}

