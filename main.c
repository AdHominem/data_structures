#include "1_13.h"


int main() {

    Tree *tree = create_tree(2);
    add(tree, 42);
    printf("%d", tree->root->keys[11212]);

    Node *node2 = create_node(2);
    Node *node3 = create_node(2);
    return 0;
}