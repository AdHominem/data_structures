#include "4_1.h"
#include "4_1_helper.h"
#include "4_1_functions.h"
#include <stdio.h>

int main() {

    int config_status = generate_config(DEGREE * 40);
    if (config_status == -1) {
        printf("Error: Config file could not be created!\n");
        return 1;
    }

    printf("Created a config files with %d operations\n", config_status);

    printf("Generating a tree of degree %d: \n", DEGREE);
    BTree *tree = b_tree_create();

    int status = parse_config(tree);
    if (status) {
        b_tree_destroy(tree);
        printf("Error: Config file could not be opened!\n");
        return 1;
    }

    printf("\nResulting tree: \n");
    b_tree_print(tree);

    b_tree_destroy(tree);
    return 0;
}



