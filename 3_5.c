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

void add_internal(TreeNode *node, int suffix, size_t suffix_length) {
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
            add_internal(child, suffix, suffix_length);
        } else {
            printf("Need to refactor!\n");
        }
    } else {
        // if not, insert value as a new node with suffix of value
        node->children[last] = create_tree_node(suffix, suffix_length);
    }
}

void add(Tree *tree, int value, size_t value_length) {
    add_internal(tree->root, value, value_length);
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

    add(tree, 15, 2);
    assert(tree->root->children[5]->suffix == 15);

    size_t length;
    assert(longest_common_suffix(15, 315, &length) == 15);
    assert(length == 2);
    assert(longest_common_suffix(25, 315, &length) == 5);
    assert(length == 1);
    assert(longest_common_suffix(17, 315, &length) == -1);
    assert(length == 0);

    add(tree, 315, 3);

    return 0;
}
