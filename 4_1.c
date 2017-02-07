#include "4_1.h"
#include "4_1_helper.h"
#include "4_1_functions.h"
#include <stdio.h>

int main() {

    BTree *tree = b_tree_create();

    int status = parse_config(tree);
    if (status) {
        b_tree_destroy(tree);
        printf(status == 1 ? "Error: Config file could not be opened!\n" : "Error: Malformed config file!\n");
        return 1;
    }

    b_tree_print(tree);

    b_tree_destroy(tree);
    return 0;
}



