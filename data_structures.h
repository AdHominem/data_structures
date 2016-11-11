#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Contains:
 * - Linked List
 * - Integer Array
 * - Tree
 */

/* ### LINKED LIST ### */


// DATA STRUCTURES

typedef struct llnode LinkedListNode;

struct llnode {
    int value;
    LinkedListNode *next;
};

typedef struct llist {
    LinkedListNode *head;
} LinkedList;

// CONSTRUCTORS AND DESTRUCTORS

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

LinkedListNode *destroy_nodes_recursively(LinkedListNode *node) {
    if (node) {
        node->next = destroy_nodes_recursively(node->next);
        free(node);
    }
    return NULL;
}

LinkedList *create_linked_list() {
    return malloc(sizeof(LinkedList));
}

LinkedList *destroy_linked_list(LinkedList *list) {
    if (list) {
        list->head = destroy_nodes_recursively(list->head);
        free(list);
    }
    return NULL;
}

// FUNCTIONS

size_t length_of_(LinkedList *list) {
    LinkedListNode *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

void insert_at_linked_list(LinkedList *list, int value, size_t index) {

    // Catch index out of range
    if (index > length_of_(list)) {
        perror("List index out of range!");
        exit(1);
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = create_linked_list_node(value);

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
        return;
    } else {
        LinkedListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
}

void add_to_linked_list(LinkedList *list, int value) {
    insert_at_linked_list(list, value, length_of_(list));
}

void print_linked_list(LinkedList *list) {
    LinkedListNode *head = list->head;

    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    printf("\n");
}

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

IntegerArray *destroy_integer_array(IntegerArray *integerArray) {
    if (integerArray) {
        free(integerArray->elements);
        free(integerArray);
    }
    return NULL;
}

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

TreeNodeArray *create_node_array(size_t length) {
    TreeNodeArray *result = malloc(sizeof(TreeNodeArray));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->elements = malloc(length * sizeof(TreeNode));
    if (result->elements == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->length = length;

    return result;
}

TreeNodeArray *destroy_node_array(TreeNodeArray *nodeArray) {
    if (nodeArray) {
        free(nodeArray->elements);
    }
    free(nodeArray);
    return NULL;
}

TreeNode *create_tree_node(size_t degree) {

    TreeNode *result = malloc(sizeof(TreeNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->keys = create_integer_array(degree - 1);
    result->children = create_node_array(degree);
    result->degree = degree;
    result->last_key_index = 0;

    return result;
}

TreeNode *destroy_tree_node(TreeNode *node) {
    if (node) {
        node->keys = destroy_integer_array(node->keys);
        node->children = destroy_node_array(node->children);
    }
    free(node);
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
        tree->root = destroy_tree_node(tree->root);
    }
    free(tree);
    return NULL;
}

// FUNCTIONS

// Prints tree_node keys, also showing how many of the key slots are still unset
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

// Checks in which child tree_node the value may fit, or if it fits inside the key array.
// Will create a new child tree_node if necessary.
void add_internal(TreeNode *node, int value) {

    if (!node) {
        return;
    }

    for (size_t i = 0; i < node->degree - 1; i++) {

        if (node->keys->elements[i] >= value) {
            //printf("It seems %d is smaller than %d, will be inserted into my left child!\n", value, tree_node->keys->elements[i]);
            if (node->children->elements[i] != NULL) {
                //printf("\tSince I have a left child, I'm going to insert there\n");
                add_internal(node->children->elements[i], value);
                return;
            } else {
                //printf("\tSeems I don't have a left child yet, creating one!\n");
                node->children->elements[i] = create_tree_node(node->degree);
                node->children->elements[i]->keys->elements[0] = value;
                node->children->elements[i]->last_key_index += 1;
                return;
            }
        } else if (i == node->last_key_index) {
            //printf("It seems %d might fit into my key array!\n", value);
            node->keys->elements[i] = value;
            node->last_key_index += 1;
            return;
        } else if (i == node->degree - 2){
            //printf("It seems %d is bigger than %d, will be inserted into my right child!\n", value, tree_node->keys->elements[i]);
            if (node->children->elements[i + 1] != NULL) {
                //printf("\tSince I have a right child, I'm going to insert there\n");
                add_internal(node->children->elements[i + 1], value);
                return;
            } else {
                //printf("\tSeems I don't have a right child yet, creating one!\n");
                node->children->elements[i + 1] = create_tree_node(node->degree);
                node->children->elements[i + 1]->keys->elements[0] = value;
                node->children->elements[i + 1]->last_key_index += 1;
                return;
            }
        }
    }
}

// Adds a new value to the tree and creates a root tree_node if the tree is empty
void add_to_tree(Tree *tree, int value) {
    if (tree->root == NULL) {
        tree->root = create_tree_node(tree->degree);
        tree->root->keys->elements[0] = value;
        tree->root->last_key_index = 1;
    } else {
        add_internal(tree->root, value);
    }
}

// Prints all the nodes recursively, indenting them for better readability
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

void print_tree(Tree *tree) {
    print_tree_nodes(tree->root, 1);
}

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