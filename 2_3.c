#include "input.h"
#include "data_structures.h"
#include <stdbool.h>
#include <time.h>

int main() {


    printf(        "#########################\n"
                   "# Binary Tree Simulator #\n"
                   "#########################\n\n");


    BinaryTree *tree = create_binary_tree();
    printf("Created a binary tree\n\n");

    printf("Commands: \n"
                   "\t<+> - Add a random node to the tree\n"
                   "\t<-> - Removes a random node from the tree\n"
                   "\t<x> / <exit> - Exit\n"
                   "\t<p> / <print> - Print the current tree\n");

    char buf[BUFSIZ];
    char *tokens;
    srand((unsigned int) time(NULL));

    while (true) {

        prompt_for_tokens(buf, &tokens, 1);
        int to_delete;

        if (!strcmp(tokens, "exit") || !strcmp(tokens, "x")) {
            break;
        }
        else if (!strcmp(tokens, "print") || !strcmp(tokens, "p")) {
            print_binary_tree(tree);
        }
        else if (!strcmp(tokens, "+")) {
            int to_add = rand() % 100 - 50;
            add_to_binary_tree(tree, to_add);
            printf ("Added %d to the tree\n", to_add);
        }
        else if (!strcmp(tokens, "-")) {
            do {
                to_delete = rand() % 100 - 50;
            } while (delete_binary_tree_node(tree, to_delete));
            printf ("Removed %d from the tree\n", to_delete);
        }
    }

    destroy_binary_tree(tree);
}