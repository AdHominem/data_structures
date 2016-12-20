#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Contains:
 *
 * - Linked List
 *      A basic linked list with integer nodes
 * - Integer Array
 *      A wrapper class for an integer array plus its size
 * - Tree
 *      A generic n-ary tree without rebalancing and node removal
 * - Binary Tree
 *      A non-balancing binary tree
 */

/* ### LINKED LIST ### */


// DATA STRUCTURES

typedef struct llnode LinkedListNode;

struct llnode {
    int value;
    LinkedListNode *next;
};

/// Linked list is just a wrapper for the nodes
typedef struct llist {
    LinkedListNode *head;
} LinkedList;

// CONSTRUCTORS AND DESTRUCTORS

/// Creates a new linked list node
/// \param value The node's value
/// \return A pointer to the new node or NULL if memory allocation failed
LinkedListNode *create_linked_list_node(int value) {
    LinkedListNode *result = malloc(sizeof(LinkedListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

/// Destroys all linked nodes starting at node. This is usually the root of a linked list
/// \param linkedlistnode The node to destroy
/// \return A NULL pointer to be used for overwriting the object pointer
LinkedListNode *destroy_nodes_recursively(LinkedListNode *node) {
    if (node) {
        node->next = destroy_nodes_recursively(node->next);
    }
    free(node);
    return NULL;
}

/// Creates a linked list
/// \return A pointer to the list or NULL if memory allocation failed
LinkedList *create_linked_list() {
    return calloc(1, sizeof(LinkedList));
}

/// Destroys the given list, freeing its memory
/// \param list The list to destroy
/// \return A NULL pointer to be used for overwriting the object pointer
LinkedList *destroy_linked_list(LinkedList *list) {
    if (list) {
        list->head = destroy_nodes_recursively(list->head);
    }
    free(list);
    return NULL;
}

// FUNCTIONS

/// Calculates the length of the linked list dynamically
/// \param list The target list
/// \return The length
size_t length_of_(LinkedList *list) {
    LinkedListNode *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

/// Inserts a value at a given index
/// \param list The list to insert into
/// \param value The value to insert
/// \param index The index at which the insertion takes place
/// \return 0 if insertion was successful, 2 if index was out of range and 1 if memory allocation failed
int insert_at_linked_list(LinkedList *list, int value, size_t index) {

    // Catch index out of range
    if (index > length_of_(list)) {
        perror("List index out of range!");
        return 2;
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = create_linked_list_node(value);
    if (to_insert == NULL) {
        return 1;
    }

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
    } else {
        LinkedListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
    return 0;
}

/// Wrapper for insert_at_linked_list() to append a value to the end
/// \param list The target list
/// \param value The value to append
void add_to_linked_list(LinkedList *list, int value) {
    insert_at_linked_list(list, value, length_of_(list));
}

/// Prints the linked list as a one line blank delimited string
/// \param list The list to print
void print_linked_list(LinkedList *list) {
    LinkedListNode *head = list->head;

    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    printf("\n");
}

/// Returns 1 if the list contains a specified value
/// \param list The list to search in
/// \param value The value to search for
/// \return 1 if the list contains that value, else 0
int linked_list_contains(LinkedList *list, int value) {
    LinkedListNode *current = list->head;
    while (current) {
        if (current->value == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}


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

/// Checks if a given value is contained by a node or its child nodes
/// \param node The node to start searching at
/// \param value The value to search for
/// \return A pointer to the found node or NULL if the value is not found
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

/// Checks if a given value is in a tree
/// \param tree The tree in which to search
/// \param value The value to search for
/// \return A pointer to the found node or NULL if the value is not found
TreeNode *search_in_tree(Tree *tree, int value) {
    return search_in_tree_internal(tree->root, value);
}


/* ### BINARY TREE ###
 *
 * This is separate from Tree since it includes tracking the parent element and removal
 *
 * */


// DATA STRUCTURES

typedef struct binary_tree_node BinaryTreeNode;

struct binary_tree_node {
    int value;
    BinaryTreeNode *left;
    BinaryTreeNode *right;
    BinaryTreeNode *parent;
};

typedef struct binary_tree {
    BinaryTreeNode *root;
} BinaryTree;

// CONSTRUCTORS AND DESTRUCTORS

/// Creates a new binary tree node
/// \param value The value of the new node
/// \return A pointer to the new node or NULL in case of memory error
BinaryTreeNode *create_binary_tree_node(int value) {

    BinaryTreeNode *result = malloc(sizeof(BinaryTreeNode));
    if (result == NULL) {
        return NULL;
    }

    result->left = NULL;
    result->right = NULL;
    result->parent = NULL;
    result->value = value;

    return result;
}

BinaryTreeNode *destroy_binary_tree_node(BinaryTreeNode *node) {
    if (node) {
        if (node->parent) {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }
    }
    free(node);
    return NULL;
}

BinaryTreeNode *destroy_binary_tree_nodes_recursively(BinaryTreeNode *node) {
    if (node) {
        node->left = destroy_binary_tree_nodes_recursively(node->left);
        node->right = destroy_binary_tree_nodes_recursively(node->right);
    }
    free(node);
    return NULL;
}

BinaryTree *create_binary_tree() {

    BinaryTree *result = malloc(sizeof(BinaryTree));
    if (result == NULL) {
        return NULL;
    }

    result->root = NULL;
    return result;
}

BinaryTree *destroy_binary_tree(BinaryTree *tree) {
    if (tree) {
        tree->root = destroy_binary_tree_nodes_recursively(tree->root);
    }
    free(tree);
    return NULL;
}

// FUNCTIONS

/// Adds a new node with give value to the tree
/// \param node The node at which the insertion starts
/// \param value The value to insert
/// \return 0 if successful, else 1 in case of memory error
int add_to_binary_tree_internal(BinaryTreeNode *node, int value) {

    if (node) {
        if (node->value >= value) {
            if (node->left) {
                add_to_binary_tree_internal(node->left, value);
            } else {
                node->left = create_binary_tree_node(value);
                if (node->left == NULL) {
                    return 1;
                }
                node->left->parent = node;
            }
        } else {
            if (node->right) {
                add_to_binary_tree_internal(node->right, value);
            } else {
                node->right = create_binary_tree_node(value);
                if (node->right == NULL) {
                    return 1;
                }
                node->right->parent = node;
            }
        }
    }
    return 0;
}

/// Adds a new value to the tree and creates a root node if the tree is empty
/// \param tree The tree to extend
/// \param value The value to insert
/// \return 0 if successful, else 1 in case of memory error
int add_to_binary_tree(BinaryTree *tree, int value) {
    if (tree->root == NULL) {
        tree->root = create_binary_tree_node(value);
        if (tree->root == NULL) {
            return 1;
        }
    } else {
        if (add_to_binary_tree_internal(tree->root, value)) {
            return 1;
        }
    }
    return 0;
}

/// Prints all the nodes recursively, indenting them for better readability
/// \param node The subtree which is printed
/// \param depth The depth of indentation. This parameter is used in recursion and should remain 1
void print_binary_tree_nodes(BinaryTreeNode *node, size_t depth){
    if (node) {
        if (node->left) {
            print_binary_tree_nodes(node->left, depth + 1);
        }

        for (size_t indentations = 1; indentations < depth; indentations++) {
            printf("\t");
        }

        printf("%d\n", node->value);

        if (node->right) {
            print_binary_tree_nodes(node->right, depth + 1);
        }
    }
}

/// Prints all the nodes of the tree recursively, indenting them for better readability
/// \param tree The tree to print
void print_binary_tree(BinaryTree *tree) {
    print_binary_tree_nodes(tree->root, 1);
    printf("\n");
}

/// Searches a value inside a subtree and fetches the respective node if found
/// \param node The subtree to search in
/// \param value The value to search
/// \return A pointer to the containing BinaryTreeNode or NULL, if the subtree doesn't contain the value
BinaryTreeNode *search_in_binary_tree_internal(BinaryTreeNode *node, int value) {

    if (node) {
        if (node->value == value) {
            return node;
        } else if (value < node->value) {
            return search_in_binary_tree_internal(node->left, value);
        } else {
            return search_in_binary_tree_internal(node->right, value);
        }
    }
    return NULL;
}

/// Searches a value inside a tree and fetches the respective node if found
/// \param tree The tree to search in
/// \param value The value to search
/// \return A pointer to the containing BinaryTreeNode or NULL, if the tree doesn't contain the value
BinaryTreeNode *search_in_binary_tree(BinaryTree *tree, int value) {
    return search_in_binary_tree_internal(tree->root, value);
}

/// Returns the rightmost node of a given node
/// \param node The node from where to start searching
/// \return The rightmost node or the node itself if it has no right node
BinaryTreeNode *rightmost_node_of_(BinaryTreeNode *node) {
    return node->right ? rightmost_node_of_(node->right) : node;
}

/// Returns the leftmost node of a given node
/// \param node The node from where to start searching
/// \return The leftmost node or the node itself if it has no left node
BinaryTreeNode *leftmost_node_of_(BinaryTreeNode *node) {
    return node->left ? leftmost_node_of_(node->left) : node;
}

/// Deletes the node with value from the subtree starting at node
/// \param node The root from which to start looking
/// \param value The value of the node to be deleted
/// \return 0 if the node was successfully deleted, else 1 if the given value was not in the subtree
int delete_binary_tree_node_internal(BinaryTreeNode *node, int value) {
    BinaryTreeNode *target = search_in_binary_tree_internal(node, value);
    if (target == NULL) {
        return 1;
    } else {
        BinaryTreeNode *replacement;
        if (!target->left && !target->right) {
            destroy_binary_tree_node(target);
            return 0;
        } else {
            replacement = target->left ? rightmost_node_of_(target->left) : leftmost_node_of_(target->right);
        }
        if (!replacement->right && !replacement->left) {
            target->value = replacement->value;
            destroy_binary_tree_node(replacement);
        } else {
            target->value = replacement->value;
            delete_binary_tree_node_internal(replacement, replacement->value);
        }
        return 0;
    }
}

int delete_binary_tree_node(BinaryTree *tree, int value) {
    return delete_binary_tree_node_internal(tree->root, value);
}


/*
 * ADJACENCY LIST
 *
 * This specific data structure creates adjacency lists of fixed size
 * TODO: Make it accept an arbitrary size
 */


// DATA STRUCTURES

#define NODES_COUNT 20

typedef struct alnode AdjacencyListNode;

/// An adjacency node contains a value and a dynamic list with its predecessors
struct alnode {
    int value;
    LinkedList *successors;
};

typedef struct alist {
    AdjacencyListNode *nodes[NODES_COUNT];
} AdjacencyList;

// CONSTRUCTORS AND DESTRUCTORS

/// Destroy an adjacency list node
/// \param node The node to destroy
/// \return NULL
AdjacencyListNode *destroy_alnode(AdjacencyListNode *node) {
    if (node) {
        node->successors = destroy_linked_list(node->successors);
    }
    free(node);
    return NULL;
}

/// Creates a new node for an adjacency list
/// \param value The node's integer value
/// \return A pointer to the node or NULL if memory allocation failed
AdjacencyListNode *create_alnode(int value) {

    AdjacencyListNode *result = malloc(sizeof(AdjacencyListNode));
    if (result == NULL) {
        return NULL;
    }

    result->value = value;

    result->successors = create_linked_list();
    if (result->successors == NULL) {
        return destroy_alnode(result);
    }

    return result;
}

/// Destroys an adjacency list
/// \param list The list to destroy
/// \return NULL
AdjacencyList *destroy_alist(AdjacencyList *list) {
    if (list) {
        for (int i = 0; i < NODES_COUNT; ++i) {
            list->nodes[i] = destroy_alnode(list->nodes[i]);
        }
    }
    free(list);
    return NULL;
}

/// Creates a new adjacency list pre initialized with nodes of value 0-19
/// \return A pointer to the list or NULL if memory allocation failed
AdjacencyList *create_alist() {
    AdjacencyList *result = malloc(sizeof(AdjacencyList));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < NODES_COUNT; ++i) {
        result->nodes[i] = create_alnode(i);
        if (result->nodes[i] == NULL) {
            return destroy_alist(result);
        }
    }

    return result;
}

// FUNCTIONS

/// Links two given adjaceny list nodes
/// \param from The node from wich the edge starts
/// \param to The node to which the edge goes
void link_alnodes(AdjacencyListNode *from, AdjacencyListNode *to) {
    if (from && to && !linked_list_contains(from->successors, to->value)) {
        add_to_linked_list(from->successors, to->value);
    }
}

void print_alist(AdjacencyList *list) {

    for (int i = 0; i < NODES_COUNT; ++i) {
        printf("%d: ", i);
        print_linked_list(list->nodes[i]->successors);
    }
    printf("\n");
}


/* ### B-TREE ### */


// DATA STRUCTURES

typedef struct b_tree_node BTreeNode;

typedef struct b_tree_node_array {
    size_t length;
    BTreeNode **elements;
} BTreeNodeArray;

struct b_tree_node {
    size_t degree;
    size_t last_key_index;
    BTreeNodeArray *children;
    IntegerArray *keys;
};

typedef struct b_tree {
    BTreeNode *root;
    size_t degree;
} BTree;

// CONSTRUCTORS AND DESTRUCTORS

/// Destroy a given node array
/// \param nodeArray The node array to destroy
/// \return NULL
BTreeNodeArray *b_tree_node_array_destroy(BTreeNodeArray *nodeArray) {
    if (nodeArray) {
        free(nodeArray->elements);
    }
    free(nodeArray);
    return NULL;
}

/// Creates an array of nodes with a given length
/// \param length The length of the array
/// \return A pointer to the new array or NULL if memory allocation failed
BTreeNodeArray *b_tree_node_array_create(size_t length) {
    BTreeNodeArray *result = malloc(sizeof(BTreeNodeArray));
    if (result == NULL) {
        return NULL;
    }

    result->elements = calloc(length, sizeof(BTreeNode));
    if (result->elements == NULL) {
        return b_tree_node_array_destroy(result);
    }

    result->length = length;

    return result;
}

/// Destroys a node and its child nodes recursively
/// \param node The node to destroy
/// \return A NULL pointer to clean up the node pointer
BTreeNode *b_tree_node_array_destroy_recursively(BTreeNode *node) {
    if (node) {
        node->keys = destroy_integer_array(node->keys);
        node->children = b_tree_node_array_destroy(node->children);
    }
    free(node);
    return NULL;
}

/// Creates a new node of given degree
/// \param degree The degree of the node
/// \return A pointer to the new node or NULL in case of memory allocation failure
BTreeNode *b_tree_node_create(size_t degree) {

    BTreeNode *result = malloc(sizeof(BTreeNode));
    if (result == NULL) {
        return NULL;
    }

    result->keys = create_integer_array(degree - 1);
    result->children = b_tree_node_array_create(degree);
    if (result->keys == NULL || result->children == NULL) {
        return b_tree_node_array_destroy_recursively(result);
    }

    result->degree = degree;
    result->last_key_index = 0;

    return result;
}

/// Creates a new n-ary tree
/// \param degree The degree
/// \return A pointer to the new tree or NULL in case of memory error or if the degree is invalid (lower than 2)
BTree *b_tree_create(size_t degree) {
    if (degree < 2) {
        return NULL;
    }

    BTree *result = malloc(sizeof(BTree));
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
BTree *b_tree_destroy(BTree *tree) {
    if (tree) {
        tree->root = b_tree_node_array_destroy_recursively(tree->root);
    }
    free(tree);
    return NULL;
}

// FUNCTIONS

/// Prints tree_node keys, also showing how many of the key slots are still unset
/// \param node The node whose keys are to be printed
void b_tree_node_keys_print(BTreeNode *node) {
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
int b_tree_add_internal(BTreeNode *node, int value) {

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
                return b_tree_add_internal(node->children->elements[i], value);
            }
            else {
                node->children->elements[i] = b_tree_node_create(node->degree);
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
                return b_tree_add_internal(node->children->elements[i + 1], value);
            } else {
                node->children->elements[i + 1] = b_tree_node_create(node->degree);
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
int b_tree_add(BTree *tree, int value) {
    if (tree->root == NULL) {
        tree->root = b_tree_node_create(tree->degree);
        if (tree->root == NULL) {
            return 1;
        }
        tree->root->keys->elements[0] = value;
        tree->root->last_key_index = 1;
    } else {
        if (b_tree_add_internal(tree->root, value)) {
            return 1;
        }
    }
    return 0;
}

/// Prints all the nodes recursively, indenting them for better readability
/// \param node The node to start printing from
/// \param depth The depth of printing, this should be 1 initially
void b_tree_nodes_print(BTreeNode *node, size_t depth){
    if (node) {
        for (size_t key_index = 0; key_index < node->last_key_index; key_index++) {
            if (node->children->elements[key_index] != NULL) {
                b_tree_nodes_print(node->children->elements[key_index], depth + 1);
            }
        }

        for (size_t indentations = 1; indentations < depth; indentations++) {
            printf("\t");
        }

        b_tree_node_keys_print(node);

        if (node->children->elements[node->degree - 1]) {
            b_tree_nodes_print(node->children->elements[node->degree - 1], depth + 1);
        }
    }
}

/// Prints all the nodes of a tree recursively, indenting them for better readability
/// \param tree The tree to print
void b_tree_print(BTree *tree) {
    b_tree_nodes_print(tree->root, 1);
}

///
/// \param node
/// \param value
/// \return
BTreeNode *b_tree_search_internal(BTreeNode *node, int value) {

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
            return b_tree_search_internal(node->children->elements[i], value);
        }

            // Else check if we are at the last key
        else if (i == node->degree - 2) {
            // Now value is bigger than the last element and could only be in the right child
            // Only the last key in the array can have a right child
            return b_tree_search_internal(node->children->elements[i + 1], value);
        }
    }
    return NULL;
}

// Returns a pointer to the containing TreeNode or NULL, if value not in graph
BTreeNode *b_tree_search(BTree *tree, int value) {
    return b_tree_search_internal(tree->root, value);
}


#endif