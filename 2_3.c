#include "input.h"
#include "data_structures.h"
#include <stdbool.h>


int main() {


    printf(        "#########################\n"
                   "# Binary Tree Simulator #\n"
                   "#########################\n\n");


    BinaryTree *tree = create_binary_tree();
    printf("Created a binary tree\n\n");

    printf("Commands: \n"
                   "\t+ <number> - adds the number to the tree\n"
                   "\t- <number> - removes the number from the tree\n"
                   "\t\"exit\" - quit\n"
                   "\t\"print\" - print the tree\n");

    int value;
    char buf[BUFSIZ];
    char **tokens = malloc(2 * sizeof(char*));

    while (true) {

        get_line(buf, BUFSIZ);
        get_tokens_optional(buf, tokens, 2);
        int one_token_entered = tokens[0] && !tokens[1];
        int two_tokens_entered = tokens[0] && tokens[1];

        if (one_token_entered) {
            if (!strcmp(tokens[0], "exit")) {
                break;
            }
            else if (!strcmp(tokens[0], "print")) {
                print_binary_tree(tree);
            }
        }
        else if (two_tokens_entered) {
            if (!string_to_integer(tokens[1], &value)) {
                if (!strcmp(tokens[0], "+")) {
                    add_to_binary_tree(tree, value);
                    printf ("Added %d to the tree\n", value);
                }
                else if (!strcmp(tokens[0], "-")) {
                    if (!delete_binary_tree_node(tree, value)) {
                        printf ("Removed %d from the tree\n", value);
                    } else {
                        printf("%d is not in the tree!\n", value);
                    }
                }
            }
        }
    }

    free(tokens);
    destroy_binary_tree(tree);
}