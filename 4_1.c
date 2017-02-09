#include "4_1.h"
#include "4_1_helper.h"
#include "4_1_functions.h"
#include <stdio.h>
#include <assert.h>

void test() {
    BTree *tree = b_tree_create();

    b_tree_add(tree, 1);
    b_tree_add(tree, 2);
    b_tree_add(tree, 3);
    b_tree_add(tree, 4);
    b_tree_add(tree, 5);
    b_tree_add(tree, 6);
    b_tree_add(tree, 7);
    b_tree_add(tree, 8);
    b_tree_add(tree, 9);
    b_tree_add(tree, 10);
    b_tree_add(tree, 11);
    b_tree_print(tree);
    b_tree_remove(tree, 11);
    //TODO Code breaks here:
    //b_tree_remove(tree, 3);



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



