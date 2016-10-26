#include "1_13.h"


int main() {

    // Testing integer arrays
    IntegerArray *integerArray = create_integer_array(10);
    for (size_t i = 0; i < integerArray->length; ++i) {
        integerArray->elements[i] = (int) (i + 5);
    }
    for (size_t i = 0; i < integerArray->length; ++i) {
        printf("%d ", integerArray->elements[i]);
    }
    integerArray = destroy_integer_array(integerArray);
    assert (!integerArray);


    // Test Node
    Node *node = create_node(2);
    node->keys->elements[0] = 42;
    printf("\n%d", node->keys->elements[0]);
    node = destroy_node(node);
    assert(!node);

    // Test Tree
    Tree *tree = create_tree(2);
    tree->root->keys->elements[0] = 1337;
    printf("\n%d", tree->root->keys->elements[0]);
    tree = destroy_tree(tree);
    assert(!tree);


    return 0;
}

