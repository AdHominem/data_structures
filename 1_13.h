#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>


// DATA STRUCTURES


typedef struct node Node;

typedef struct integer_array {
    size_t length;
    int *elements;
} IntegerArray;

typedef struct node_array {
    size_t length;
    Node **elements;
} NodeArray;

struct node {
    size_t degree;
    size_t keys_head;
    NodeArray *children;
    IntegerArray *keys;
};

typedef struct tree {
    Node *root;
    size_t degree;
} Tree;


// CONSTRUCTORS AND DESTRUCTORS


// Treating integer arrays as a dedicated data structure makes keeping track of the length easier
IntegerArray *create_integer_array(size_t length) {
    IntegerArray *result = malloc(sizeof(IntegerArray));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->elements = malloc(length * sizeof(int));
    if (result->elements == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->length = length;

    return result;
}

NodeArray *create_node_array(size_t length) {
    NodeArray *result = malloc(sizeof(NodeArray));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->elements = malloc(length * sizeof(Node));
    if (result->elements == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->length = length;

    return result;
}

Node *create_node(size_t degree) {

    Node *result = malloc(sizeof(Node));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->keys = create_integer_array(degree - 1);
    result->children = create_node_array(degree);
    result->degree = degree;
    result->keys_head = 0;

    return result;
}

IntegerArray *destroy_integer_array(IntegerArray *integerArray) {
    if (integerArray) {
        free(integerArray->elements);
        free(integerArray);
    }
    return NULL;
}

NodeArray *destroy_node_array(NodeArray *nodeArray) {
    if (nodeArray) {
        free(nodeArray->elements);
        free(nodeArray);
    }
    return NULL;
}

Node *destroy_node(Node *node) {
    if (node) {
        node->keys = destroy_integer_array(node->keys);
        node->children = destroy_node_array(node->children);
        free(node);
    }
    return NULL;
}

Tree *create_tree(size_t degree) {
    assert(degree >= 2);

    Tree *result = malloc(sizeof(Tree));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->root = NULL;
    result->degree = degree;
    return result;
}

Tree *destroy_tree(Tree *tree) {
    if (tree) {
        tree->root = destroy_node(tree->root);
        free(tree);
    }
    return NULL;
}


// FUNCTIONS


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

// Prints node keys, also showing how many of the key slots are still unset
void print_node_keys(Node *node) {
    printf("[");
    for (size_t j = 0; j < node->keys->length; ++j) {
        if (j < node->keys_head) {
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

// Checks in which child node the value may fit, or if it fits inside the key array.
// Will create a new child node if necessary.
void add_internal(Node *node, int value) {

    for (size_t i = 0; i < node->degree - 1; i++) {

        if (node->keys->elements[i] >= value) {
            //printf("It seems %d is smaller than %d, will be inserted into my left child!\n", value, node->keys->elements[i]);
            if (node->children->elements[i] != NULL) {
                //printf("\tSince I have a left child, I'm going to insert there\n");
                add_internal(node->children->elements[i], value);
                return;
            } else {
                //printf("\tSeems I don't have a left child yet, creating one!\n");
                node->children->elements[i] = create_node(node->degree);
                node->children->elements[i]->keys->elements[0] = value;
                node->children->elements[i]->keys_head += 1;
                return;
            }
        } else if (i == node->keys_head) {
            //printf("It seems %d might fit into my key array!\n", value);
            node->keys->elements[i] = value;
            node->keys_head += 1;
            return;
        } else if (i == node->degree - 2){
            //printf("It seems %d is bigger than %d, will be inserted into my right child!\n", value, node->keys->elements[i]);
            if (node->children->elements[i + 1] != NULL) {
                //printf("\tSince I have a right child, I'm going to insert there\n");
                add_internal(node->children->elements[i + 1], value);
                return;
            } else {
                //printf("\tSeems I don't have a right child yet, creating one!\n");
                node->children->elements[i + 1] = create_node(node->degree);
                node->children->elements[i + 1]->keys->elements[0] = value;
                node->children->elements[i + 1]->keys_head += 1;
                return;
            }
        }
    }
}

// Adds a new value to the tree and creates a root node if the tree is empty
void add(Tree *tree, int value) {
    if (tree->root == NULL) {
        tree->root = create_node(tree->degree);
        tree->root->keys->elements[0] = value;
        tree->root->keys_head = 1;
    } else {
        add_internal(tree->root, value);
    }
}

// Prints all the nodes recursively, indenting them for better readability
void print_nodes(Node *node, size_t depth){
    if (node) {
        for (size_t key_index = 0; key_index < node->keys_head; key_index++) {
            if (node->children->elements[key_index] != NULL) {
                print_nodes(node->children->elements[key_index], depth + 1);
            }
        }

        for (size_t indentations = 1; indentations < depth; indentations++) {
            printf("\t");
        }

        print_node_keys(node);

        if (node->children->elements[node->degree - 1]) {
            print_nodes(node->children->elements[node->degree - 1], depth + 1);
        }
    }
}

void print_tree(Tree *tree) {
    print_nodes(tree->root, 1);
}

Node *search_internal(Node *node, int value) {

    for (size_t i = 0; i < node->keys_head; ++i) {

        // Check if value is in the keys
        if (node->keys->elements[i] == value) {
            return node;
        }

        // Else check if it's too small to fit, so we need to go down the left subtree
        else if (value < node->keys->elements[i]) {
            return search_internal(node->children->elements[i], value);
        }

        // Else check if we are at the last key
        else if (i == node->degree - 2) {
            // Now value is bigger than the last element and could only be in the right child
            // Only the last key in the array can have a right child
            return search_internal(node->children->elements[i + 1], value);
        }
    }
    return NULL;
}

// Returns a pointer to the containing Node or NULL, if value not in graph
Node *search(Tree *tree, int value) {
    return search_internal(tree->root, value);
}


// TESTING


int test() {

//    // Testing integer arrays and printing them
//    IntegerArray *integerArray = create_integer_array(10);
//    for (size_t i = 0; i < integerArray->length; ++i) {
//        integerArray->elements[i] = (int) (i + 5);
//    }
//    print_integer_array(integerArray);
//    integerArray = destroy_integer_array(integerArray);
//    assert (!integerArray);
//
//    // Test Node
//    Node *node = create_node(2);
//    node->keys->elements[0] = 42;
//    printf("\n%d\n", node->keys->elements[0]);
//    node = destroy_node(node);
//    assert(!node);
//
//    // Test Tree and printing them
//    Tree *tree = create_tree(2);
//    tree->root = create_node(tree->degree);
//    tree->root->keys->elements[0] = 1337;
//    print_tree(tree);
//    tree = destroy_tree(tree);
//    assert(!tree);
//
//    // Test NodeArray
//    NodeArray *nodeArray = create_node_array(2);
//    nodeArray->elements[0] = create_node(2);
//    nodeArray->elements[0]->keys->elements[0] = 42;
//    nodeArray->elements[1] = create_node(2);
//    nodeArray->elements[1]->keys->elements[0] = 33;
//    printf("\n%d, %d", nodeArray->elements[0]->keys->elements[0], nodeArray->elements[1]->keys->elements[0]);
//    nodeArray = destroy_node_array(nodeArray);
//    assert(!nodeArray);
//
//    // Test add empty
//    tree = create_tree(2);
//    add(tree, 111);
//    printf("\n%d\n", tree->root->keys->elements[0]);
//    tree = destroy_tree(tree);

    // Test add regular
    Tree *tree = create_tree(4);
    add(tree, 20);
    add(tree, 18);
    add(tree, 12);
    add(tree, 17);
    add(tree, 5);
    add(tree, 1);
    add(tree, 6);
    add(tree, 22);
    add(tree, 30);
    add(tree, 29);
    add(tree, 31);
    print_tree(tree);
    tree = destroy_tree(tree);


    return 0;
}