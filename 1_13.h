#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct node Node;
typedef struct node *NodePointer;

struct node {
    Node **children;
    size_t values_border_index;
    int keys[];
};

typedef struct tree {
    Node *root;
    size_t degree;
} Tree;

Node *create_node(size_t degree) {

    Node *result = malloc(sizeof(Node) + (degree - 1) * sizeof(int) + degree * sizeof(NodePointer));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->children = NULL;
    result->values_border_index = 0;

    return result;
}

void destroy_node(Node *node) {
    if (node) {
        free(node);
    }
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

void add(Tree *tree, int value) {
    if (tree->root == NULL) {
        tree->root = create_node(tree->degree);
        tree->root->keys[0] = value;
        tree->root->values_border_index = 1;
    } else {
        //add_internal(tree->root, value);
    }
}

//void destroy_nodes_recursively(Node *node) {
//    Node *left = node->left;
//    Node *right = node->right;
//
//    if (left) {
//        destroy_nodes_recursively(left);
//    }
//
//    if (right) {
//        destroy_nodes_recursively(right);
//    }
//
//    destroy_node(node);
//}
//
//void destroy_tree(Tree *tree) {
//    if (tree != NULL) {
//        destroy_nodes_recursively(tree->root);
//        free(tree);
//    }
//}
//
//// Infix notation
//void print_nodes(Node *root) {
//    Node *left = root->left;
//    Node *right = root->right;
//
//    if (left) {
//        print_nodes(left);
//    }
//
//    printf("%d ", root->value);
//
//    if (right) {
//        print_nodes(right);
//    }
//}
//
//void print_tree(Tree *tree) {
//    if (tree->root) {
//        print_nodes(tree->root);
//        printf("\n");
//    } else {
//        printf("Tree is empty!\n");
//    }
//}
//
//// Decides whether to insert left or right and then whether here or down the subtree
//void add_internal(Node *root, int value) {
//    Node **relevant_node = (root->value >= value) ? &root->left : &root->right;
//
//    if (*relevant_node) {
//        add_internal(*relevant_node, value);
//    } else {
//        *relevant_node = create_node(value);
//    }
//}
//
//void add(Tree *tree, int value) {
//    if (tree->root) {
//        add_internal(tree->root, value);
//    } else {
//        tree->root = create_node(value);
//    }
//}

