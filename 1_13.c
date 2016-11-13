#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <limits.h>
#include <errno.h>
#include "data_structures.h"
#include "input.h"

int main() {

    printf("########################\n"
                   "# N-Ary Tree Simulator #\n"
                   "########################\n\n"
                   "Warning: Expecting integers > 2. \n"
                   "Wrong input will lead to undefined behavior!\n\n"
                   "Enter degree: \n");

    size_t degree;
    get_size_t(&degree);

    Tree *tree = create_tree(degree);
    printf("Created a tree of size %zu\n\n", degree);
    printf("Commands: \n"
                   "\t<number> - adds the number to the tree\n"
                   "\t\"exit\" - quit\n"
                   "\t\"print\" - print the tree\n");
    printf("Warning: Expecting integers. Long input is not remediated\n");

    char buf[BUFSIZ];
    char *p;

    do {
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            perror("Could not read from stdin!");
            exit(EXIT_FAILURE);
        }

        if (!strncmp(buf, "print", 5)) {
            print_tree(tree);
        } else {
            long value = strtol(buf, &p, 10);
            if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))) {
                printf("Value out of range!");
                exit(EXIT_FAILURE);
            }
            else if (errno != 0 && value == 0) {
                perror("Error during long conversion with strtol");
                exit(EXIT_FAILURE);
            }
            else if (buf[0] != '\n' && (*p == '\n' || *p == '\0')) {
                add_to_tree(tree, (int) value);
                printf ("Added %ld to the tree\n", value);
            }
        }
    } while (strncmp(buf, "exit", 4) != 0);

    print_tree(tree);
    destroy_tree(tree);

    return EXIT_SUCCESS;
}