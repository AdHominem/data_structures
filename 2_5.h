#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <time.h>

#define NODES_COUNT 20

// Each node has a value and for each existing node the binary pointers array
// marks a link (1) or no link (0) to another node
typedef struct node {
    int value;
    int pointers[NODES_COUNT];
} Node;

// This way, a graph only consists of nodes!
typedef struct graph {
    Node *nodes[NODES_COUNT];
} Graph;

// Initially, a node always has a value and no links
Node *create_node(int value) {
    Node *result = malloc(sizeof(Node));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    for (int i = 0; i < NODES_COUNT; ++i) {
        result->pointers[i] = 0;
    }
    return result;
}

Node *destroy_node(Node *node) {
    if (node) {
        free(node);
    }
    return 0;
}

Graph *create_random_graph() {
    Graph *result = malloc(sizeof(Graph));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    for (int i = 0; i < NODES_COUNT; ++i) {
        result->nodes[i] = create_node(i);
    }

    srand((unsigned int) time(NULL));

    // For any given node, there is a small chance it links to a different target node
    for (int node = 0; node < NODES_COUNT; ++node) {
        for (int target = 0; target < NODES_COUNT; ++target) {
            if (node != target && !(rand() % 7)) {
                result->nodes[node]->pointers[target] = 1;
            }
        }
    }

    return result;
}

Graph *destroy_graph(Graph *graph) {
    if (graph) {
        for (int i = 0; i < NODES_COUNT; ++i) {
            graph->nodes[i] = destroy_node(graph->nodes[i]);
        }
        free(graph);
    }
    return NULL;
}

void print_graph_as_matrix(Graph *graph) {
    printf("    ");
    for (int i = 0; i < NODES_COUNT; ++i) {
        printf("%3d ", i);
    }
    printf("\n");
    for (int row = 0; row < NODES_COUNT; ++row) {
        printf("%3d ", row);
        for (int column = 0; column < NODES_COUNT; ++column) {
            printf("%3d ", graph->nodes[row]->pointers[column]);
        }
        printf("\n");

    }
}

void print_graph_as_list(Graph *graph) {
    for (int i = 0; i < NODES_COUNT; ++i) {
        printf("%d: ", i);
        for (int j = 0; j < NODES_COUNT; ++j) {
            if (graph->nodes[i]->pointers[j]) {
                printf("--> %d ", j);
            }
        }
        printf("\n");
    }
}

int test() {
    Graph *graph = create_random_graph();
    print_graph_as_list(graph);
    printf("\n");
    print_graph_as_matrix(graph);
    graph = destroy_graph(graph);

    return 0;
}