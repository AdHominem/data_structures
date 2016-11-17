#include "input.h"
#include "data_structures.h"
#include <stdbool.h>


int main() {

    printf(        "########################\n"
                   "# N-Ary Tree Simulator #\n"
                   "########################\n\n"
                   "Enter degree: \n");

    size_t degree;
    int status;
    do {
        status = get_size_t(&degree);
        if (status != 0 || degree < 2) {
            fprintf(stderr, "Degree must be a valid size_t >= 2\n");
        }
    } while (status || degree < 2);

    Tree *tree = create_tree(degree);
    if (tree == NULL) {
        fprintf(stderr, "Could not create a tree!\n");
        return 1;
    }

    printf("Created a tree of degree %zu\n\n", degree);

    printf("Commands: \n"
                   "\t<number> - Adds the number to the tree\n"
                   "\t<x> / <exit> - Exit\n"
                   "\t<p> / <print> - Print the tree\n");

    char buf[BUFSIZ];
    char *tokens;
    int value;

    while (true) {

        prompt_for_tokens(buf, &tokens, 1);

        if (!strcmp(tokens, "exit") || !strcmp(tokens, "x")) {
            break;
        }
        else if (!strcmp(tokens, "print") || !strcmp(tokens, "p")) {
            print_tree(tree);
        }
        else if (!string_to_integer(tokens, &value)) {
            if (add_to_tree(tree, value)) {
                fprintf(stderr, "Memory Error: Could not add to tree!\n");
                destroy_tree(tree);
                return 1;
            }
            printf ("Added %d to the tree\n", value);
        }
    }

    destroy_tree(tree);
}