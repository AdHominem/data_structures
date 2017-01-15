#include <memory.h>
#include "data_structures.h"
#include <stdbool.h>

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
        //search in subtree
        for (size_t i = 0; i < node->keys_count; ++i) {
            if (node->keys[i] > value) {
                return b_tree_search_internal(node->children[i], value);
            }
        }
        return b_tree_search_internal(node->children[node->children_count - 1], value);
    }
}

BTreeNode *b_tree_search(BTree *tree, size_t value) {
    return b_tree_search_internal(tree->root, value);
}

void b_tree_add(BTree *tree, size_t value) {

    if (b_tree_search(tree, value)) {
        return;
    }

    // if root is null, add one with value as first key
    if (tree->root) {
        size_t keys_count = tree->root->keys_count;
        size_t *keys = tree->root->keys;

        // return if value in root
        for (size_t i = 0; i < keys_count; ++i) {
            if (tree->root->keys[i] == value) {
                return;
            }
        }

        // try to insert in root
        if (keys_count < DEGREE - 1) {
            keys[keys_count] = value;
            qsort(keys, keys_count + 1, sizeof(size_t), compare_size_t);
            tree->root->keys_count++;
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
            new_root->type = NODE;

            free(tree->root);

            tree->root = new_root;
        }
    } else {
        tree->root = b_tree_node_create();
        tree->root->keys[0] = value;
        tree->root->keys_count++;
    }

}

int main() {

    BTree *tree = b_tree_create();
    b_tree_add(tree, 1);
    b_tree_add(tree, 4);
    b_tree_add(tree, 40);
    b_tree_add(tree, 6);
    b_tree_add(tree, 10);
    b_tree_add(tree, 8);
    b_tree_print(tree);
    assert(b_tree_search(tree, 40));
    assert(!b_tree_search(tree, 42));

    b_tree_destroy(tree);
    return 0;
}