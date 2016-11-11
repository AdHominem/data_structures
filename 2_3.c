#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include "data_structures.h"

// TESTING

// calling rand() multiple times per millisecond can yield identical results.
// to increase entropy, a feedback is included
void delete_random_binary_tree_nodes(BinaryTree *tree) {
    srand((unsigned int) time(NULL));
    size_t deletions = (size_t) (rand() % 3 + 1);

    int to_delete, random, old;
    size_t deleted = deletions;
    do {
        random = rand();
        old = random;
        to_delete = (random + old) % 13;
        if (search_in_binary_tree(tree, to_delete)) {
            printf("Deleting %d...\n", to_delete);
            delete_binary_tree_node(tree, to_delete);
            --deleted;
        }
    } while (deleted);
}

void add_random_binary_tree_nodes(BinaryTree *tree) {
    srand((unsigned int) time(NULL));
    size_t insertions = (size_t) (rand() % 10 + 3);
    printf("Adding ");

    int random, old, to_add;
    for (size_t i = 0; i < insertions; ++i) {
        random = rand();
        old = random;
        to_add = (random + old) % 13;
        add_to_binary_tree(tree, to_add);
        printf("%d ", to_add);
    }
    printf("\n");
}

int main() {


    printf("#########################\n"
                   "# Binary Tree Simulator #\n"
                   "#########################\n\n");


    BinaryTree *tree = create_binary_tree();
    printf("Created a binary binary_tree\n\n");
    printf("Commands: \n"
                   "\t+ <number> - adds the number to the tree\n"
                   "\t- <number> - removes the number from the tree\n"
                   "\t\"exit\" - quit\n"
                   "\t\"print\" - print the tree\n");
    printf("Warning: Expecting integers!\n");

    int value;
    char buf[BUFSIZ];
    char *p;

    do {
        fgets(buf, sizeof(buf), stdin);
        if (!strncmp(buf, "print", 5)) {
            print_binary_tree(tree);
        } else if (buf[1] == ' ' && (buf[0] == '-' || buf[0] == '+')) {
            char number[BUFSIZ];
            memmove(number, buf + 2, strlen(buf));
            value = (int) strtol(number, &p, 10);

            if (*p == '\n' || *p == '\0') {
                if (buf[0] == '+') {
                    add_to_binary_tree(tree, value);
                    printf ("Added %d to the tree\n", value);
                } else if (buf[0] == '-') {
                    delete_binary_tree_node(tree, value);
                    printf ("Removed %d from the tree\n", value);
                }
            } else {
                printf ("Invalid number entered\n");
            }
        }
    } while (strncmp(buf, "exit", 4) != 0);

    print_binary_tree(tree);
    destroy_binary_tree(tree);

    return 0;
}