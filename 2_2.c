#include "input.h"
#include "data_structures.h"
#include <stdbool.h>


int main() {

    printf(        "########################\n"
                   "# N-Ary Tree Simulator #\n"
                   "########################\n\n"
                   "Enter degree: \n");

    size_t degree;
    do {
        get_size_t(&degree);
        if (degree < 2) {
            fprintf(stderr, "Degree must be a valid size_t >= 2\n");
        }
    } while (degree < 2);

    Tree *tree = create_tree(degree);
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
            add_to_tree(tree, value);
            printf ("Added %d to the tree\n", value);
        }
    }

    destroy_tree(tree);
}