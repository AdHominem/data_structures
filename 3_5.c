#include <stdio.h>
#include <stdlib.h>

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

typedef struct tree_node TreeNode;

typedef struct tree_node_array {
    size_t length;
    TreeNode **elements;
} TreeNodeArray;

struct tree_node {
    size_t degree;
    size_t last_key_index;
    TreeNodeArray *children;
    IntegerArray *keys;
};

typedef struct tree {
    TreeNode *root;
    size_t degree;
} Tree;

// CONSTRUCTORS AND DESTRUCTORS

/// Destroy a given node array
/// \param nodeArray The node array to destroy
/// \return NULL
TreeNodeArray *destroy_node_array(TreeNodeArray *nodeArray) {
    if (nodeArray) {
        free(nodeArray->elements);
    }
    free(nodeArray);
    return NULL;
}

/// Creates an array of nodes with a given length
/// \param length The length of the array
/// \return A pointer to the new array or NULL if memory allocation failed
TreeNodeArray *create_node_array(size_t length) {
    TreeNodeArray *result = malloc(sizeof(TreeNodeArray));
    if (result == NULL) {
        return NULL;
    }

    result->elements = calloc(length, sizeof(TreeNode));
    if (result->elements == NULL) {
        return destroy_node_array(result);
    }

    result->length = length;

    return result;
}

/// Destroys a node and its child nodes recursively
/// \param node The node to destroy
/// \return A NULL pointer to clean up the node pointer
TreeNode *destroy_tree_nodes_recursively(TreeNode *node) {
    if (node) {
        node->keys = destroy_integer_array(node->keys);
        node->children = destroy_node_array(node->children);
    }
    free(node);
    return NULL;
}

/// Creates a new node of given degree
/// \param degree The degree of the node
/// \return A pointer to the new node or NULL in case of memory allocation failure
TreeNode *create_tree_node(size_t degree) {

    TreeNode *result = malloc(sizeof(TreeNode));
    if (result == NULL) {
        return NULL;
    }

    result->keys = create_integer_array(degree - 1);
    result->children = create_node_array(degree);
    if (result->keys == NULL || result->children == NULL) {
        return destroy_tree_nodes_recursively(result);
    }

    result->degree = degree;
    result->last_key_index = 0;

    return result;
}

/// Creates a new n-ary tree
/// \param degree The degree
/// \return A pointer to the new tree or NULL in case of memory error or if the degree is invalid (lower than 2)
Tree *create_tree(size_t degree) {
    if (degree < 2) {
        return NULL;
    }

    Tree *result = malloc(sizeof(Tree));
    if (result == NULL) {
        return NULL;
    }

    result->root = NULL;
    result->degree = degree;
    return result;
}

/// Destroys the tree, recursively destroying its nodes
/// \param tree The tree to destroy
/// \return A NULL pointer to clean up the tree pointer
Tree *destroy_tree(Tree *tree) {
    if (tree) {
        tree->root = destroy_tree_nodes_recursively(tree->root);
    }
    free(tree);
    return NULL;
}

// FUNCTIONS

/// Prints tree_node keys, also showing how many of the key slots are still unset
/// \param node The node whose keys are to be printed
void print_tree_node_keys(TreeNode *node) {
    printf("[");
    for (size_t j = 0; j < node->keys->length; ++j) {
        if (j < node->last_key_index) {
            printf("%d", node->keys->elements[j]);
        } else {
            printf(" ");
        }

        if (j != node->keys->length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

/// Checks in which child tree_node the value may fit, or if it fits inside the key array.
/// Will create a new child tree_node if necessary.
/// \param node The node from which to start insertion
/// \param value The value to insert
/// \return 0 if insertion was successful or 1 if creating a new node was impossible due to memory error and 2 if the
/// given node was NULL and 3 if the function terminates unexpectedly
int add_to_tree_internal(TreeNode *node, int value) {

    if (!node) {
        return 2;
    }

    for (size_t i = 0; i < node->degree - 1; i++) {

        if (i == node->last_key_index) {
            node->keys->elements[i] = value;
            node->last_key_index += 1;
            return 0;
        }
        else if (node->keys->elements[i] >= value) {
            if (node->children->elements[i] != NULL) {
                return add_to_tree_internal(node->children->elements[i], value);
            }
            else {
                node->children->elements[i] = create_tree_node(node->degree);
                if (node->children->elements[i] == NULL) {
                    return 1;
                }
                node->children->elements[i]->keys->elements[0] = value;
                node->children->elements[i]->last_key_index += 1;
                return 0;
            }
        }
        else if (i == node->degree - 2){
            if (node->children->elements[i + 1] != NULL) {
                return add_to_tree_internal(node->children->elements[i + 1], value);
            } else {
                node->children->elements[i + 1] = create_tree_node(node->degree);
                if (node->children->elements[i + 1] == NULL) {
                    return 1;
                }
                node->children->elements[i + 1]->keys->elements[0] = value;
                node->children->elements[i + 1]->last_key_index += 1;
                return 0;
            }
        }
    }

    return 3;
}

/// Adds a new value to the tree and creates a root tree_node if the tree is empty
/// \param tree The tree to extend
/// \param value The value to insert
/// \return 0 if insertion was successful, else 1 in case of memory error
int add_to_tree(Tree *tree, int value) {
    if (tree->root == NULL) {
        tree->root = create_tree_node(tree->degree);
        if (tree->root == NULL) {
            return 1;
        }
        tree->root->keys->elements[0] = value;
        tree->root->last_key_index = 1;
    } else {
        if (add_to_tree_internal(tree->root, value)) {
            return 1;
        }
    }
    return 0;
}

/// Prints all the nodes recursively, indenting them for better readability
/// \param node The node to start printing from
/// \param depth The depth of printing, this should be 1 initially
void print_tree_nodes(TreeNode *node, size_t depth){
    if (node) {
        for (size_t key_index = 0; key_index < node->last_key_index; key_index++) {
            if (node->children->elements[key_index] != NULL) {
                print_tree_nodes(node->children->elements[key_index], depth + 1);
            }
        }

        for (size_t indentations = 1; indentations < depth; indentations++) {
            printf("\t");
        }

        print_tree_node_keys(node);

        if (node->children->elements[node->degree - 1]) {
            print_tree_nodes(node->children->elements[node->degree - 1], depth + 1);
        }
    }
}

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void print_tree(Tree *tree) {
    print_tree_nodes(tree->root, 1);
}

///
/// \param node
/// \param value
/// \return
TreeNode *search_in_tree_internal(TreeNode *node, int value) {

    if (!node) {
        return NULL;
    }

    for (size_t i = 0; i < node->last_key_index; ++i) {

        // Check if value is in the keys
        if (node->keys->elements[i] == value) {
            return node;
        }

            // Else check if it's too small to fit, so we need to go down the left subtree
        else if (value < node->keys->elements[i]) {
            return search_in_tree_internal(node->children->elements[i], value);
        }

            // Else check if we are at the last key
        else if (i == node->degree - 2) {
            // Now value is bigger than the last element and could only be in the right child
            // Only the last key in the array can have a right child
            return search_in_tree_internal(node->children->elements[i + 1], value);
        }
    }
    return NULL;
}

// Returns a pointer to the containing TreeNode or NULL, if value not in graph
TreeNode *search_in_tree(Tree *tree, int value) {
    return search_in_tree_internal(tree->root, value);
}

void add(TreeNode *node, int value) {
    if (!value) return;

    int digit = value % 10;
    value /= 10;

    if (!node->children->elements[digit]) {
        node->children->elements[digit] = create_tree_node(node->degree);
    }

    node->keys->elements[digit] = digit;
    printf("%d ", node->keys->elements[digit]);
    add(node->children->elements[digit], value);

}

void add_root(Tree *tree, int value) {
    tree->root = create_tree_node(tree->degree);
    add(tree->root, value);
}

// height of trie is the length of the longest key
// degree is the amount of different alphabet symbols
//
int main() {
    // alphabet: 0,1,2,3,4,5,6,7,8,9
    // height: 2
    Tree *tree = create_tree(10);
    add_root(tree, 42);
    add(tree->root, 12);
    print_tree(tree);

    return 0;
}