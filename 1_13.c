#include "input.h"
#include "data_structures.h"

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
                   "\t<number> - adds the number to the tree\n"
                   "\t\"exit\" - quit\n"
                   "\t\"print\" - print the tree\n");

    char buf[BUFSIZ];
    char **tokens = malloc(sizeof(char*));
    int value;

    while (TRUE) {

        prompt_for_tokens(buf, tokens, 1);

        if (!strcmp(tokens[0], "exit")) {
            break;
        }
        else if (!strcmp(tokens[0], "print")) {
            print_tree(tree);
        }
        else if (!string_to_integer(tokens[0], &value)) {
            add_to_tree(tree, value);
            printf ("Added %d to the tree\n", value);
        }
    }

    free(tokens);
    destroy_tree(tree);

    return EXIT_SUCCESS;
}