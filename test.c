#include <stdio.h>
#include "input.h"
#include "data_structures.h"

int main() {
    printf("Hello\n");
    TreeNodeArray *nodes = create_node_array(3);
    for (size_t i = 0; i < nodes->length; ++i) {
        printf("%p ", nodes->elements[i]);
    }

//    TreeNode *node = create_tree_node(3);
//    printf("%p ", node->children->elements[0]);
}

