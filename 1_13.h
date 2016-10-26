#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

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

    result->root = create_node(degree);
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

///**
//* Checks whether a new value is smaller than the current key i.
//* If it is, it will be inserted at subtree i.
//* Else check if the current key is maybe zero.
//* If it is, we can safely place the new value here.
//* Else check if we are at the last key.
//* If so, we need to either insert value in the right subtree i+1, or create a new one if it didn't exist.
//*/
//void add_internal(Node *node, int value) {
//
//    for (size_t i = 0; i < node->degree - 1; i++) {
//
//        if (node->keys[i] >= value) {
//            if (node->children[i] != NULL) {
//                add_internal(node->children[i], value);
//            } else {
//                node->children[i] = create_node(node->degree);
//                node->children[i]->keys[0] = value;
//                node->children[i]->keys_head += 1;
//                return;
//            }
//        } else if (i == node->keys_head) {
//            node->keys[i] = value;
//            node->keys_head += 1;
//            return;
//        } else if (i == node->degree - 2){
//            if (node->children[i + 1] != NULL) {
//                add_internal(node->children[i + 1], value);
//            } else {
//                node->children[i + 1] = create_node(node->degree);
//                node->children[i + 1]->keys[0] = value;
//                node->children[i + 1]->keys_head += 1;
//                return;
//            }
//        }
//    }
//}
//
//
//void add(Tree *tree, int value) {
//    if (tree->root == NULL) {
//        tree->root = create_node(tree->degree);
//        tree->root->keys[0] = value;
//        tree->root->keys_head = 1;
//    } else {
//        add_internal(tree->root, value);
//    }
//}
//
//void destroy_nodes_recursively(Node *node) {
//    if (node->children) {
//
//    }
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

