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

Tree *create_tree() ;
void print_tree(Tree *tree) ;
void add(Tree *tree, int value) ;
void optimize(Tree *tree) ;
int tree_search(const Tree *tree, const int value) ;

Tree *tree_destroy(Tree *tree) ;


void tree_count_nodes(const Tree *tree, size_t *count) ;

void test_compression(size_t numbers) ;

int main() {
    // alphabet: 0,1,2,3,4,5,6,7,8,9
    Tree *tree = create_tree();
    assert(tree->root->suffix == 0);
    assert(tree->root->suffix_length == 0);
    for (int i = 0; i < 10; ++i) {
        assert(tree->root->children[i] == NULL);
    }

    add(tree, 15);
    add(tree, 315);
    add(tree, 25);

    assert(tree_search(tree, 15));
    assert(tree_search(tree, 315));
    assert(tree_search(tree, 25));
    assert(tree_search(tree, 0) == 0);
    assert(tree_search(tree, 5) == 0);
    assert(tree_search(tree, 1) == 0);

    tree = create_tree();

    add(tree, 15);
    add(tree, 315);
    add(tree, 25);
    optimize(tree);

    assert(tree_search(tree, 15));
    assert(tree_search(tree, 315));
    assert(tree_search(tree, 25));
    assert(tree_search(tree, 0) == 0);
    assert(tree_search(tree, 5) == 0);
    assert(tree_search(tree, 1) == 0);

    test_compression(100);
    test_compression(500);
    test_compression(1000);

    return 0;
}

void test_compression(size_t numbers) {
    Tree *tree = create_tree();

    // Generate an array with random values between 0 and 9999
    srand((unsigned) time(NULL));
    for (size_t i = 0; i < numbers; ++i) {
        add(tree, rand() % 100000);
    }

    size_t count = 0;
    tree_count_nodes(tree, &count);
    printf("Amount of nodes using %zu strings: %zu\n", numbers, count);

    optimize(tree);

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

int tree_search_internal(const TreeNode *node, const int original_value, int value) {

    // are we at a leaf? if yes, compare value with the leaf
    // if not, is there a fitting child node? if yes, use it
    // if not, exit

    if (node->suffix_length == 5 && node->suffix == original_value) {
        return true;
    } else {

        int child_index = value / (int) pow(10, node->suffix_length) % 10;

        TreeNode *child = node->children[child_index];

        if (child) {
            return tree_search_internal(child, original_value, value / (int) pow(10, node->suffix_length));
        } else {
            return false;
        }
    }
}

int tree_search(const Tree *tree, const int value) {
    assert(value < 100000);
    return tree_search_internal(tree->root, value, value);

}

void optimize_node(TreeNode *node) {

    if (node == NULL) {
        return;
    }

    //printf("Node optimization starts\n");

    size_t children_count = 0;
    for (size_t i = 0; i < 10; ++i) {
        if (node->children[i] != NULL) {
            ++children_count;
        }
    }
    //printf("Node %d (%zu) here, it seems my number of children is %zu\n", node->suffix, node->suffix_length, children_count);

    if (children_count == 0) {
        //printf("Node %d here, i have zero children, no need to optimize\n", node->suffix);
        //printf("Node optimization finished\n\n");
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

                //printf("New suffix %d here, len %zu\n", node->suffix, node->suffix_length);

                // call optimize on myself
                //printf("Node optimization finished\n\n");
                optimize_node(node);
                return;
            }
        }
    } else {
        //printf("I have more than one child it seems!\n");
        //printf("Node optimizazion finished\n\n");
        for (int i = 0; i < 10; ++i) {
            optimize_node(node->children[i]);
        }
    }
}

// Assumes that the root node has a suffix length of zero
void optimize(Tree *tree) {
    TreeNode *node = tree->root;

    // We dont need to optimize a root if it has already a suffix length of 5
    if (node->suffix_length >= 5) {
        return;
    }

    //printf("Root optimization starts\n");

    // do i have just one child?
    size_t children_count = 0;
    for (size_t i = 0; i < 10; ++i) {
        if (node->children[i] != NULL) {
            ++children_count;
        }
    }
    //printf("Root %d (%zu) here, it seems my number of children is %zu\n", node->suffix, node->suffix_length, children_count);
    // if no, call optimize on all of my children
    /*
     * If yes, 
     *      prepend my child's suffix to mine
     *      exchange all of my nodes with the ones of my child
     *      then call optimize on myself again
     */
    if (children_count == 0) {
        //printf("Root %d here, I dont have any children!\n", node->suffix);
        return;
    } else if (children_count == 1) {
        /*
         * Swap with child
         * call optimize on myself
         */
        for (size_t i = 0; i < 10; ++i) {
            if (node->children[i] != NULL) {
                int former_root_suffix = tree->root->suffix;
                size_t former_root_suffix_length = tree->root->suffix_length;
                tree->root = node->children[i];

                // if the child is a leaf (node suffix length will be 4), then just swap. We dont need to update
                // suffix length (its already 5) and the suffix will be okay too

                // update suffix and suffix_length
                if (former_root_suffix_length != 4) {
                    tree->root->suffix_length += former_root_suffix_length;
                    tree->root->suffix = former_root_suffix + (tree->root->suffix * (int) pow(10, former_root_suffix_length));
                }

                //printf("New suffix: %d\n", tree->root->suffix);
                //printf("Root optimizazion finished\n\n");
                return optimize(tree);
            }
        }
    } else {
        //printf("I have more than one child it seems!\n");
        //printf("Root optimizazion finished\n\n");
        for (int i = 0; i < 10; ++i) {
            optimize_node(node->children[i]);
        }
    }
}

TreeNode *create_tree_node(int suffix, size_t suffix_length) {

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

Tree *create_tree() {
    Tree *result = malloc(sizeof(Tree));
    if (result == NULL) {
        return NULL;
    }
    result->root = create_tree_node(0, 0);
    return result;
}

Tree *tree_destroy(Tree *tree) {
    if (tree) {
        tree->root = tree_node_destroy(tree->root);
    }
    free(tree);
    return NULL;
}

void add_internal(TreeNode *node, int value, size_t depth) {

    // Get the last digit of the value, according to its depth
    int last_digit = value / (int) pow(10, depth) % 10;

    if (node->children[last_digit] == NULL) {
        if (depth < STRING_SIZE - 1) {
            node->children[last_digit] = create_tree_node(last_digit, 1);
            add_internal(node->children[last_digit], value, depth + 1);
        } else {
            node->children[last_digit] = create_tree_node(value, STRING_SIZE);
        }
    } else {
        add_internal(node->children[last_digit], value, depth + 1);
    }
}

// will add to the tree only if the value is not already in the tree
void add(Tree *tree, int value) {
    if (tree_search(tree, value) == 0) {
        add_internal(tree->root, value, 0);
    }
}

void print_tree_node_keys(TreeNode *node) {
    printf("Suffix: %05d length:%zu\n", node->suffix, node->suffix_length);
}

void print_tree_nodes(TreeNode *node, size_t depth){
    if (node) {

        for (size_t indentations = 1; indentations < depth; indentations++) {
            printf("\t");
        }

        // print own
        print_tree_node_keys(node);

        // call this on all children
        for (size_t i = 0; i < 10; i++) {
            print_tree_nodes(node->children[i], depth + 1);
        }
    }
}

void print_tree(Tree *tree) {
    print_tree_nodes(tree->root, 1);
}

