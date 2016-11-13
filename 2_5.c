#include <time.h>
#include "data_structures.h"

void link_random(AdjacencyList *list, int matrix[NODES_COUNT][NODES_COUNT]) {
    srand((unsigned int) time(NULL));

    // For any given node, there is a small chance it links to a different target node
    for (int from = 0; from < NODES_COUNT; ++from) {
        for (int to = 0; to < NODES_COUNT; ++to) {
            if (from != to && !(rand() % 7)) {
                link_alnodes(list->nodes[from], list->nodes[to]);
                matrix[from][to] = 1;
            }
        }
    }
}

void print_matrix(int matrix[NODES_COUNT][NODES_COUNT]) {
    printf("    ");
    for (int i = 0; i < NODES_COUNT; ++i) {
        printf("%3d ", i);
    }
    printf("\n");
    for (int row = 0; row < NODES_COUNT; ++row) {
        printf("%3d ", row);
        for (int column = 0; column < NODES_COUNT; ++column) {
            printf("%3d ", matrix[row][column]);
        }
        printf("\n");

    }
}

int main() {

    printf("Created a random graph!\n");

    AdjacencyList *graph = create_alist();

    int matrix[NODES_COUNT][NODES_COUNT];
    for (int i = 0; i < NODES_COUNT; ++i) {
        for (int j = 0; j < NODES_COUNT; ++j) {
            matrix[i][j] = 0;
        }
    }

    link_random(graph, matrix);
    print_alist(graph);
    destroy_alist(graph);
    print_matrix(matrix);

    return 0;
}