#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define STRING_SIZE 5

typedef struct node TreeNode;
struct node {
    int suffix;
    size_t suffix_length;
    TreeNode *children[10];
};
typedef struct tree {
    TreeNode *root;
} Tree;

Tree *tree_create() ;
void tree_add(Tree *tree, const int value) ;
void tree_optimize(Tree *tree) ;
Tree *tree_destroy(Tree *tree) ;
void tree_count_nodes(const Tree *tree, size_t *count) ;
void test_compression(size_t numbers) ;

int main() {
    test_compression(100);
    test_compression(500);
    test_compression(1000);
    return 0;
}

void test_compression(const size_t numbers) {
    Tree *tree = tree_create();

    // Add random values between 0 and 9999 to the tree
    srand((unsigned) time(NULL));
    for (size_t i = 0; i < numbers; ++i) {
        tree_add(tree, rand() % 100000);
    }

    size_t count = 0;
    tree_count_nodes(tree, &count);
    printf("Amount of nodes using %zu strings: %zu\n", numbers, count);

    tree_optimize(tree);

    count = 0;
    tree_count_nodes(tree, &count);
    printf("Amount of nodes using %zu strings after optimization: %zu\n\n", numbers, count);

    tree_destroy(tree);
}

void tree_count_nodes_internal(const TreeNode *node, size_t *count) {
    if (node) {
        ++*count;
        for (int i = 0; i < 10; ++i) {
            tree_count_nodes_internal(node->children[i], count);
        }
    }
}

void tree_count_nodes(const Tree *tree, size_t *count) {
    return tree_count_nodes_internal(tree->root, count);
}

int tree_search_internal(const TreeNode *node, const int original_value, const int value) {

    // Are we at a leaf? If yes, compare value with the leaf
    // Else, is there a fitting child node? If yes, use it
    // Else exit

    if (node->suffix_length == 5 && node->suffix == original_value) {
        return true;
    } else {
        int child_index = value / (int) pow(10, node->suffix_length) % 10;
        TreeNode *child = node->children[child_index];
        return (child) ? tree_search_internal(child, original_value, value / (int) pow(10, node->suffix_length))
                       : false;
    }
}

int tree_search(const Tree *tree, const int value) {
    return tree_search_internal(tree->root, value, value);
}

void tree_node_optimize(TreeNode *node) {
    if (node == NULL) {
        return;
    }

    size_t children_count = 0;
    for (size_t i = 0; i < 10; ++i) {
        if (node->children[i] != NULL) {
            ++children_count;
        }
    }

    // Zero children: No need to optimize
    // One child: Swap with child
    // More than two: Can't optimize, just call optimize on all children

    if (children_count == 0) {
        return;
    } else if (children_count == 1) {
        for (size_t i = 0; i < 10; ++i) {
            if (node->children[i] != NULL) {
                TreeNode *child = node->children[i];

                // copy all children of child
                for (size_t j = 0; j < 10; ++j) {
                    node->children[j] = child->children[j];
                }

                // if child is a leaf, just swap with the child (basically copy)
                // update suffix and suffix_length
                if (child->suffix_length == 5) {
                    node->suffix = child->suffix;
                    node->suffix_length = child->suffix_length;
                } else {
                    node->suffix = node->suffix + (child->suffix * (int) pow(10, node->suffix_length));
                    node->suffix_length += child->suffix_length;
                }

                // free child (not its children!)
                free(child);

                // call optimize on myself
                tree_node_optimize(node);
                return;
            }
        }
    } else {
        for (int i = 0; i < 10; ++i) {
            tree_node_optimize(node->children[i]);
        }
    }
}

// Assumes that the root node has a suffix length of zero
void tree_optimize(Tree *tree) {
    TreeNode *node = tree->root;

    // We dont need to optimize a root if it has already a suffix length of 5
    if (node->suffix_length >= 5) {
        return;
    }

    // do i have just one child?
    size_t children_count = 0;
    for (size_t i = 0; i < 10; ++i) {
        if (node->children[i] != NULL) {
            ++children_count;
        }
    }

    // leaves need not to be optimized
    if (children_count == 0) {
        return;
    } else if (children_count == 1) {
        // Swap with child and call optimize on myself
        for (size_t i = 0; i < 10; ++i) {
            if (node->children[i] != NULL) {
                int former_root_suffix = tree->root->suffix;
                size_t former_root_suffix_length = tree->root->suffix_length;
                tree->root = node->children[i];

                // if the child is a leaf (node suffix length will be 4), then just swap. We don't need to update
                // suffix length (it's already 5) and the suffix will be okay too
                if (former_root_suffix_length != 4) {
                    tree->root->suffix_length += former_root_suffix_length;
                    tree->root->suffix = former_root_suffix + (tree->root->suffix *
                            (int) pow(10, former_root_suffix_length));
                }

                return tree_optimize(tree);
            }
        }
    } else {
        // node is already optimal, call optimize on all children
        for (int i = 0; i < 10; ++i) {
            tree_node_optimize(node->children[i]);
        }
    }
}

TreeNode *tree_node_create(const int suffix, const size_t suffix_length) {

    TreeNode *result = malloc(sizeof(TreeNode));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < 10; ++i) {
        result->children[i] = NULL;
    }

    result->suffix = suffix;
    result->suffix_length = suffix_length;

    return result;
}

TreeNode *tree_node_destroy(TreeNode *node) {
    if (node) {
        for (int i = 0; i < 10; ++i) {
            node->children[i] = tree_node_destroy(node->children[i]);
        }
    }
    free(node);
    return NULL;
}

Tree *tree_create() {
    Tree *result = malloc(sizeof(Tree));
    if (result == NULL) {
        return NULL;
    }
    result->root = tree_node_create(0, 0);
    return result;
}

Tree *tree_destroy(Tree *tree) {
    if (tree) {
        tree->root = tree_node_destroy(tree->root);
    }
    free(tree);
    return NULL;
}

void tree_add_internal(TreeNode *node, const int value, const size_t depth) {

    // Get the last digit of the value, according to its depth
    int last_digit = value / (int) pow(10, depth) % 10;

    if (node->children[last_digit] == NULL) {
        if (depth < STRING_SIZE - 1) {
            node->children[last_digit] = tree_node_create(last_digit, 1);
            tree_add_internal(node->children[last_digit], value, depth + 1);
        } else {
            node->children[last_digit] = tree_node_create(value, STRING_SIZE);
        }
    } else {
        tree_add_internal(node->children[last_digit], value, depth + 1);
    }
}

// will add to the tree only if the value is not already in the tree
void tree_add(Tree *tree, const int value) {
    if (tree_search(tree, value) == 0) {
        tree_add_internal(tree->root, value, 0);
    }
}
