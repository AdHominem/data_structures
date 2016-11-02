#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NODES_COUNT 20


// ### LINKED LIST ###


typedef struct llnode LinkedListNode;

struct llnode {
    int value;
    LinkedListNode *next;
};

LinkedListNode *create_node(int value) {
    LinkedListNode *result = malloc(sizeof(LinkedListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

LinkedListNode *destroy_nodes_recursively(LinkedListNode *node) {
    if (node) {
        node->next = destroy_nodes_recursively(node->next);
        free(node);
    }
    return NULL;
}

typedef struct llist {
    LinkedListNode *head;
} LinkedList;

LinkedList *create_linked_list() {
    return malloc(sizeof(LinkedList));
}

LinkedList *destroy_linked_list(LinkedList *list) {
    if (list) {
        list->head = destroy_nodes_recursively(list->head);
        free(list);
    }
    return NULL;
}

size_t size_of_(LinkedList *list) {
    LinkedListNode *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

void insert_at_index(LinkedList *list, int value, size_t index) {

    // Catch index out of range
    if (index > size_of_(list)) {
        perror("List index out of range!");
        exit(1);
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = create_node(value);

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
        return;
    } else {
        LinkedListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
}

void add(LinkedList *list, int value) {
    insert_at_index(list, value, size_of_(list));
}

void print_linked_list(LinkedList *list) {
    LinkedListNode *head = list->head;

    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    printf("\n");
}

int list_contains(LinkedList *list, int value) {
    LinkedListNode *current = list->head;
    while (current) {
        if (current->value == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}


// ### ADJACENCY LIST ###


typedef struct alnode AdjacencyListNode;

// An adjacency node contains a value and a dynamic list with its predecessors
struct alnode {
    int value;
    LinkedList *successors;
};

AdjacencyListNode *create_alnode(int value) {

    AdjacencyListNode *result = malloc(sizeof(AdjacencyListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->successors = create_linked_list();

    return result;
}

AdjacencyListNode *destroy_alnode(AdjacencyListNode *node) {
    if (node) {
        node->successors = destroy_linked_list(node->successors);
        free(node);
    }
    return NULL;
}

void link_alnodes(AdjacencyListNode *from, AdjacencyListNode *to) {
    if (from && to && !list_contains(from->successors, to->value)) {
        add(from->successors, to->value);
    }
}

// An adjacency list contains n nodes
typedef struct alist {
    AdjacencyListNode *nodes[NODES_COUNT];
} AdjacencyList;

// The adjacency list comes pre initialized with nodes of value 0-19
AdjacencyList *create_alist() {
    AdjacencyList *result = malloc(sizeof(AdjacencyList));
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < NODES_COUNT; ++i) {
        result->nodes[i] = create_alnode(i);
    }

    return result;
}

AdjacencyList *destroy_alist(AdjacencyList *list) {
    if (list) {
        for (int i = 0; i < NODES_COUNT; ++i) {
            list->nodes[i] = destroy_alnode(list->nodes[i]);
        }
        free(list);
    }
    return NULL;
}

void print_alist(AdjacencyList *list) {
    for (int i = 0; i < NODES_COUNT; ++i) {
        printf("%d: ", i);
        print_linked_list(list->nodes[i]->successors);
    }
    printf("\n");
}

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