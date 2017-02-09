#include "4_1.h"
#include "4_1_helper.h"
#include "4_1_functions.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>

void test() {
    BTree *tree = b_tree_create();

    srand(time(NULL));
    size_t number = rand() % 20 + 10;
    printf("%zu\n\n", number);

    for (size_t i = 0; i < number; ++i) {
        int value = rand() % 50;
        if (rand() % 3) {
            printf("Adding %d \n", value);
            b_tree_add(tree, value);
        } else {
            printf("Subtracting %d \n ", value);
            b_tree_remove(tree, value);
        }
    }
    b_tree_print(tree);


    b_tree_destroy(tree);
}

int main() {

    test();

//    BTree *tree = b_tree_create();
//
//    int status = parse_config(tree);
//    if (status) {
//        b_tree_destroy(tree);
//        printf(status == 1 ? "Error: Config file could not be opened!\n" : "Error: Malformed config file!\n");
//        return 1;
//    }
//
//    b_tree_print(tree);
//
//    b_tree_destroy(tree);
    return 0;
}



