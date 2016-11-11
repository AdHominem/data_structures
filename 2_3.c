#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <time.h>


// DATA STRUCTURES


typedef struct node Node;

struct node {
    int value;
    Node *left;
    Node *right;
    Node *parent;
};

typedef struct tree {
    Node *root;
} Tree;


// CONSTRUCTORS AND DESTRUCTORS


// Treating integer arrays as a dedicated data structure makes keeping track of the length easier

Node *create_node(int value) {

    Node *result = malloc(sizeof(Node));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->left = NULL;
    result->right = NULL;
    result->parent = NULL;
    result->value = value;

    return result;
}

Node *destroy_node(Node *node) {
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

Node *destroy_nodes_recursively(Node *node) {
    if (node) {
        node->left = destroy_nodes_recursively(node->left);
        node->right = destroy_nodes_recursively(node->right);
    }
    free(node);
    return NULL;
}

Tree *create_tree() {

    Tree *result = malloc(sizeof(Tree));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->root = NULL;
    return result;
}

Tree *destroy_tree(Tree *tree) {
    if (tree) {
        tree->root = destroy_nodes_recursively(tree->root);
    }
    free(tree);
    return NULL;
}


// FUNCTIONS


// Checks in which child node the value may fit, or if it fits inside the key array.
// Will create a new child node if necessary.
void add_internal(Node *node, int value) {

    if (node) {
        if (node->value >= value) {
            if (node->left) {
                add_internal(node->left, value);
            } else {
                node->left = create_node(value);
                node->left->parent = node;
            }
        } else {
            if (node->right) {
                add_internal(node->right, value);
            } else {
                node->right = create_node(value);
                node->right->parent = node;
            }
        }
    }
}

// Adds a new value to the tree and creates a root node if the tree is empty
void add(Tree *tree, int value) {
    if (tree->root == NULL) {
        tree->root = create_node(value);
    } else {
        add_internal(tree->root, value);
    }
}

// Prints all the nodes recursively, indenting them for better readability
void print_nodes(Node *node, size_t depth){
    if (node) {
        if (node->left) {
            print_nodes(node->left, depth + 1);
        }

        for (size_t indentations = 1; indentations < depth; indentations++) {
            printf("\t");
        }

        printf("%d\n", node->value);

        if (node->right) {
            print_nodes(node->right, depth + 1);
        }
    }
}

void print_tree(Tree *tree) {
    print_nodes(tree->root, 1);
    printf("\n");
}

Node *search_internal(Node *node, int value) {

    if (node) {
        if (node->value == value) {
            return node;
        } else if (value < node->value) {
            return search_internal(node->left, value);
        } else {
            return search_internal(node->right, value);
        }
    }
    return NULL;
}

// Returns a pointer to the containing Node or NULL, if value not in graph
Node *search(Tree *tree, int value) {
    return search_internal(tree->root, value);
}

Node *rightmost_node_of_(Node *node) {
    return node->right ? rightmost_node_of_(node->right) : node;
}

Node *leftmost_node_of_(Node *node) {
    return node->left ? leftmost_node_of_(node->left) : node;
}

void delete_node_internal(Node *node, int value) {
    Node *target = search_internal(node, value);
    Node *replacement;
    if (target) {
        if (!target->left && !target->right) {
            destroy_node(target);
            return;
        } else {
            replacement = target->left ? rightmost_node_of_(target->left) : leftmost_node_of_(target->right);
        }
        if (!replacement->right && !replacement->left) {
            target->value = replacement->value;
            destroy_node(replacement);
        } else {
            target->value = replacement->value;
            delete_node_internal(replacement, replacement->value);
        }
    }
}

void delete_node(Tree *tree, int value) {
    delete_node_internal(tree->root, value);
}


// TESTING


// calling rand() multiple times per millisecond can yield identical results.
// to increase entropy, a feedback is included
void delete_random_nodes(Tree *tree) {
    srand((unsigned int) time(NULL));
    size_t deletions = (size_t) (rand() % 3 + 1);

    int to_delete, random, old;
    size_t deleted = deletions;
    do {
        random = rand();
        old = random;
        to_delete = (random + old) % 13;
        if (search(tree, to_delete)) {
            printf("Deleting %d...\n", to_delete);
            delete_node(tree, to_delete);
            --deleted;
        }
    } while (deleted);
}

void add_random_nodes(Tree *tree) {
    srand((unsigned int) time(NULL));
    size_t insertions = (size_t) (rand() % 10 + 3);
    printf("Adding ");

    int random, old, to_add;
    for (size_t i = 0; i < insertions; ++i) {
        random = rand();
        old = random;
        to_add = (random + old) % 13;
        add(tree, to_add);
        printf("%d ", to_add);
    }
    printf("\n");
}

int main() {


    printf("#########################\n"
                   "# Binary Tree Simulator #\n"
                   "#########################\n\n");


    Tree *tree = create_tree();
    printf("Created a binary tree\n\n");
    printf("Commands: \n"
                   "\t+ <number> - adds the number to the tree\n"
                   "\t- <number> - removes the number from the tree\n"
                   "\t\"exit\" - quit\n"
                   "\t\"print\" - print the tree\n");
    printf("Warning: Expecting integers!\n");

    int value;
    char buf[BUFSIZ];
    char *p;

    do {
        fgets(buf, sizeof(buf), stdin);
        if (!strncmp(buf, "print", 5)) {
            print_tree(tree);
        } else if (buf[1] == ' ' && (buf[0] == '-' || buf[0] == '+')) {
            char number[BUFSIZ];
            memmove(number, buf + 2, strlen(buf));
            value = (int) strtol(number, &p, 10);

            if (*p == '\n' || *p == '\0') {
                if (buf[0] == '+') {
                    add(tree, value);
                    printf ("Added %d to the tree\n", value);
                } else if (buf[0] == '-') {
                    delete_node(tree, value);
                    printf ("Removed %d from the tree\n", value);
                }
            } else {
                printf ("Invalid number entered\n");
            }
        }
    } while (strncmp(buf, "exit", 4) != 0);

    print_tree(tree);
    destroy_tree(tree);

    return 0;
}