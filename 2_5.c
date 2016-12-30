#include <time.h>
#include "data_structures.h"

/// Randomly links two nodes in both the adjacency list and the matrix
/// This relies on the list having NODES_COUNT entries!
/// \param list The adjacency list
/// \param matrix The matrix
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

/// Pretty prints a matrix with rows an columns numbered 0 to exclusive 20
/// \param matrix The matrix
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

    printf("Created a random graph!\n\n");

    AdjacencyList *graph = create_alist();
    if (graph == NULL) {
        perror("Could not allocate memory!");
        return 1;
    }

    int matrix[NODES_COUNT][NODES_COUNT];
    for (int i = 0; i < NODES_COUNT; ++i) {
        for (int j = 0; j < NODES_COUNT; ++j) {
            matrix[i][j] = 0;
        }
    }

    link_random(graph, matrix);

    printf("Adjacency list: \n");
    print_alist(graph);
    destroy_alist(graph);

    printf("Adjacency matrix: \n");
    print_matrix(matrix);
}