#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define STRING_SIZE 5

/* ### INTEGER ARRAY ###
 *
 * This is a wrapper which keeps track of the array length
 *
 */


// DATA STRUCTURES

typedef struct integer_array {
    size_t length;
    int *elements;
} IntegerArray;

// CONSTRUCTORS AND DESTRUCTORS

/// Destroy an integer array
/// \param integerArray The integer array
/// \return NULL
IntegerArray *destroy_integer_array(IntegerArray *integerArray) {
    if (integerArray) {
        free(integerArray->elements);
        free(integerArray);
    }
    return NULL;
}

/// Creates a new integer array of given length
/// \param length The length of thr array
/// \return A pointer to the given array or NULL in case of a memory allocation failure
IntegerArray *create_integer_array(size_t length) {
    IntegerArray *result = malloc(sizeof(IntegerArray));
    if (result == NULL) {
        return NULL;
    }

    result->elements = calloc(length, sizeof(int));
    if (result->elements == NULL) {
        return destroy_integer_array(result);
    }

    result->length = length;

    return result;
}

/// Prints the integer array as a one line comma delimited string
/// \param integerArray The array to print
void print_integer_array(IntegerArray *integerArray) {
    printf("[");
    for (size_t i = 0; i < integerArray->length; ++i) {
        printf("%d", integerArray->elements[i]);
        if (i != integerArray->length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}


/* ### TREE ### */


// DATA STRUCTURES

typedef struct node TreeNode;

struct node {
    int suffix;
    size_t suffix_length;
    TreeNode *children[10];
};

typedef struct tree {
    TreeNode *root;
} Tree;

/// Creates a new node of given degree
/// \param degree The degree of the node
/// \return A pointer to the new node or NULL in case of memory allocation failure
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

/// Creates a new n-ary tree
/// \param degree The degree
/// \return A pointer to the new tree or NULL in case of memory error or if the degree is invalid (lower than 2)
Tree *create_tree() {
    Tree *result = malloc(sizeof(Tree));
    if (result == NULL) {
        return NULL;
    }
    result->root = create_tree_node(0, 0);
    return result;
}

/// For two numbers, returns the longest consecutive sequence of digits starting from the end (thus suffix)
/// The length parameter is necessary to denote trailing zeroes (1001 and 2001 have a lcs of 001 which has length 3)
/// \param a First number
/// \param b Second number
/// \param length Pointer to the length of the longest common suffix
/// \return The longest shared suffix of both numbers or -1 if they don't have any
int longest_common_suffix(int a, int b, size_t *length) {

    size_t longest_common_suffix_length = 0;

    // when they have no common suffix at all
    if (a % 10 != b % 10) {
        *length = longest_common_suffix_length;
        return -1;
    }

    int result = 0;
    size_t exponent = 0;

    while (exponent < STRING_SIZE && a % 10 == b % 10) {
        result += ((a % 10) * pow(10, exponent));
        a /= 10;
        b /= 10;
        ++longest_common_suffix_length;
        ++exponent;
    }

    *length = longest_common_suffix_length;
    return result;
}

void add_internal_optimized(TreeNode *node, int suffix, size_t suffix_length) {
    // get last digit of suffix
    int last = suffix % 10;
    // determine the responsible child node
    // see if this node exists
    TreeNode *child = node->children[last];
    if (child) {
        int suffix_of_child = child->suffix;
        size_t lcs_length;
        int lcs = longest_common_suffix(suffix, suffix_of_child, &lcs_length);
        printf("Child node exists, lcs is %d\n", lcs);

        if (lcs == suffix_of_child) {
            add_internal_optimized(child, suffix, suffix_length);
        } else {
            printf("Need to refactor!\n");
        }
    } else {
        // if not, insert value as a new node with suffix of value
        node->children[last] = create_tree_node(suffix, suffix_length);
    }
}

void add_internal(TreeNode *node, int value, size_t depth) {

    // Get the last digit of the value, according to its depth
    int last_digit = value / (int) pow(10, depth) % 10;

    if (node->children[last_digit] == NULL) {
        if (depth < 4) {
            node->children[last_digit] = create_tree_node(last_digit, 1);
            add_internal(node->children[last_digit], value, depth + 1);
        } else {
            node->children[last_digit] = create_tree_node(value, 5);
        }
    } else {
        add_internal(node->children[last_digit], value, depth + 1);
    }
}

void add(Tree *tree, int value, size_t value_length) {
    add_internal(tree->root, value, 0);
}

void add_optimized(Tree *tree, int value, size_t value_length) {
    add_internal_optimized(tree->root, value, value_length);
}

void print_tree_node_keys(TreeNode *node) {
    printf("Suffix: %05d\n", node->suffix);
}

/// Prints all the nodes recursively, indenting them for better readability
/// \param node The node to start printing from
/// \param depth The depth of printing, this should be 1 initially
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

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void print_tree(Tree *tree) {
    print_tree_nodes(tree->root, 1);
}

// height of trie is the length of the longest key
// degree is the amount of different alphabet symbols
// If a node is a leaf, it has just one key
int main() {
    // alphabet: 0,1,2,3,4,5,6,7,8,9
    Tree *tree = create_tree();
    assert(tree->root->suffix == 0);
    assert(tree->root->suffix_length == 0);
    for (int i = 0; i < 10; ++i) {
        assert(tree->root->children[i] == NULL);
    }

    add_optimized(tree, 15, 5);
    add_optimized(tree, 315, 5);
    add_optimized(tree, 415, 5);
    add_optimized(tree, 25, 5);
    add_optimized(tree, 10025, 5);
    print_tree(tree);

    size_t length;
    assert(longest_common_suffix(15, 315, &length) == 15);
    assert(length == 2);
    assert(longest_common_suffix(25, 315, &length) == 5);
    assert(length == 1);
    assert(longest_common_suffix(17, 315, &length) == -1);
    assert(length == 0);





    return 0;
}
