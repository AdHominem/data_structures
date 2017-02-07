#include <time.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct linked_list_node LinkedListNode;

typedef struct alnode AdjacencyListNode;

struct linked_list_node {
    AdjacencyListNode *value;
    LinkedListNode *next;
};

typedef struct alist {
    AdjacencyListNode **nodes;
    size_t size;
} AdjacencyList;

typedef struct linked_list {
    LinkedListNode *head;
} LinkedList;

struct alnode {
    int value;
    LinkedList *successors;
};


LinkedListNode *linked_list_node_create(int value) {
    LinkedListNode *result = malloc(sizeof(LinkedListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

LinkedListNode *linked_list_node_destroy_recursively(LinkedListNode *node) {
    if (node) {
        node->next = linked_list_node_destroy_recursively(node->next);
    }
    free(node);
    return NULL;
}

LinkedList *linked_list_create() {
    return calloc(1, sizeof(LinkedList));
}

LinkedList *linked_list_destroy(LinkedList *list) {
    if (list) {
        list->head = linked_list_node_destroy_recursively(list->head);
    }
    free(list);
    return NULL;
}

size_t linked_list_get_length(LinkedList *list) {
    LinkedListNode *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

int insert_at_linked_list(LinkedList *list, int value, size_t index) {

    // Catch index out of range
    if (index > linked_list_get_length(list)) {
        perror("List index out of range!");
        return 2;
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = linked_list_node_create(value);
    if (to_insert == NULL) {
        return 1;
    }

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
    } else {
        LinkedListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
    return 0;
}

int linked_list_add_sorted(LinkedList *list, int value) {

    LinkedListNode *to_insert = linked_list_node_create(value);
    if (to_insert == NULL) {
        return 1;
    }

    LinkedListNode *current_node = list->head;

    // Catch empty list and append to front case
    if (current_node == NULL || current_node->value > value) {
        to_insert->next = current_node;
        list->head = to_insert;
    } else {
        LinkedListNode *precursor = NULL;

        for (size_t i = 0; i < linked_list_get_length(list); ++i) {

            precursor = current_node;
            current_node = current_node->next;

            // current node is NULL? Then we are at the end and the value to insert is the largest in the list
            // or is the current value larger than our value? then we can insert too
            if (current_node == NULL || current_node->value > value) {
                to_insert->next = current_node;
                precursor->next = to_insert;
                break;
            }
            // if not, move further in the list
        }
    }
    return 0;
}

void linked_list_as_array(LinkedList *list, int *array) {

    LinkedListNode *node = list->head;

    for (size_t i = 0; i < linked_list_get_length(list); ++i) {
        array[i] = node->value;
        node = node->next;
    }
}

void add_to_linked_list(LinkedList *list, int value) {
    insert_at_linked_list(list, value, linked_list_get_length(list));
}

void linked_list_print(LinkedList *list) {
    LinkedListNode *head = list->head;

    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    printf("\n");
}

int linked_list_contains(LinkedList *list, int value) {
    LinkedListNode *current = list->head;
    while (current) {
        if (current->value == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}


typedef struct linked_list_node_integer LinkedListNodeInteger;

struct linked_list_node_integer {
    int value;
    LinkedListNodeInteger *next;
};

typedef struct linked_list_integer {
    LinkedListNodeInteger *head;
} LinkedListInteger;

LinkedListNodeInteger *ilinked_list_node_create(int value) {
    LinkedListNodeInteger *result = malloc(sizeof(LinkedListNodeInteger));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

LinkedListNodeInteger *ilinked_list_node_destroy_recursively(LinkedListNodeInteger *node) {
    if (node) {
        node->next = ilinked_list_node_destroy_recursively(node->next);
    }
    free(node);
    return NULL;
}

LinkedListInteger *ilinked_list_create() {
    return calloc(1, sizeof(LinkedListInteger));
}

LinkedListInteger *ilinked_list_destroy(LinkedListInteger *list) {
    if (list) {
        list->head = ilinked_list_node_destroy_recursively(list->head);
    }
    free(list);
    return NULL;
}

size_t ilinked_list_get_length(LinkedListInteger *list) {
    LinkedListNodeInteger *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

int iinsert_at_linked_list(LinkedListInteger *list, int value, size_t index) {

    // Catch index out of range
    if (index > ilinked_list_get_length(list)) {
        perror("List index out of range!");
        return 2;
    }

    LinkedListNodeInteger *head = list->head;
    LinkedListNodeInteger *to_insert = ilinked_list_node_create(value);
    if (to_insert == NULL) {
        return 1;
    }

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
    } else {
        LinkedListNodeInteger *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
    return 0;
}

int ilinked_list_add_sorted(LinkedListInteger *list, int value) {

    LinkedListNodeInteger *to_insert = ilinked_list_node_create(value);
    if (to_insert == NULL) {
        return 1;
    }

    LinkedListNodeInteger *current_node = list->head;

    // Catch empty list and append to front case
    if (current_node == NULL || current_node->value > value) {
        to_insert->next = current_node;
        list->head = to_insert;
    } else {
        LinkedListNodeInteger *precursor = NULL;

        for (size_t i = 0; i < ilinked_list_get_length(list); ++i) {

            precursor = current_node;
            current_node = current_node->next;

            // current node is NULL? Then we are at the end and the value to insert is the largest in the list
            // or is the current value larger than our value? then we can insert too
            if (current_node == NULL || current_node->value > value) {
                to_insert->next = current_node;
                precursor->next = to_insert;
                break;
            }
            // if not, move further in the list
        }
    }
    return 0;
}

void ilinked_list_as_array(LinkedListInteger *list, int *array) {

    LinkedListNodeInteger *node = list->head;

    for (size_t i = 0; i < ilinked_list_get_length(list); ++i) {
        array[i] = node->value;
        node = node->next;
    }
}

void iadd_to_linked_list(LinkedListInteger *list, int value) {
    iinsert_at_linked_list(list, value, ilinked_list_get_length(list));
}

void ilinked_list_print(LinkedListInteger *list) {
    LinkedListNodeInteger *head = list->head;

    while (head != NULL) {
        printf("%d ", head->value);
        head = head->next;
    }

    printf("\n");
}

int ilinked_list_contains(LinkedListInteger *list, int value) {
    LinkedListNodeInteger *current = list->head;
    while (current) {
        if (current->value == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}




// DATA STRUCTURES



// CONSTRUCTORS AND DESTRUCTORS

/// Destroy an adjacency list node
/// \param node The node to destroy
/// \return NULL
AdjacencyListNode *destroy_alnode(AdjacencyListNode *node) {
    if (node) {
        node->successors = linked_list_destroy(node->successors);
    }
    free(node);
    return NULL;
}

/// Creates a new node for an adjacency list
/// \param value The node's integer value
/// \return A pointer to the node or NULL if memory allocation failed
AdjacencyListNode *adjacency_list_node_create(int value) {

    AdjacencyListNode *result = malloc(sizeof(AdjacencyListNode));
    if (result == NULL) {
        return NULL;
    }

    result->value = value;

    result->successors = linked_list_create();
    if (result->successors == NULL) {
        return destroy_alnode(result);
    }

    return result;
}

/// Destroys an adjacency list
/// \param list The list to destroy
/// \return NULL
AdjacencyList *adjacency_list_destroy(AdjacencyList *list) {
    if (list) {
        for (size_t i = 0; i < list->size; ++i) {
            list->nodes[i] = destroy_alnode(list->nodes[i]);
        }
        free(list->nodes);
    }
    free(list);
    return NULL;
}

/// Creates a new adjacency list pre initialized with nodes of value 0-19
/// \return A pointer to the list or NULL if memory allocation failed
AdjacencyList *adjacency_list_create(size_t size) {

    AdjacencyList *result = malloc(sizeof(AdjacencyList));
    if (result == NULL) {
        return NULL;
    }

    result->size = size;

    result->nodes = malloc(sizeof(AdjacencyListNode*) * size);
    if (result->nodes == NULL) {
        return adjacency_list_destroy(result);
    }

    for (size_t i = 0; i < size; ++i) {
        result->nodes[i] = adjacency_list_node_create(i);
        if (result->nodes[i] == NULL) {
            return adjacency_list_destroy(result);
        }
    }

    return result;
}

// FUNCTIONS

/// Links two given adjaceny list nodes
/// \param from The node from wich the edge starts
/// \param to The node to which the edge goes
void link_alnodes(AdjacencyListNode *from, AdjacencyListNode *to) {
    if (from && to && !linked_list_contains(from->successors, to->value)) {
        add_to_linked_list(from->successors, to->value);
    }
}

void adjacency_list_remove(AdjacencyList *list, AdjacencyListNode *node) {

}

size_t indeg(AdjacencyListNode *node, AdjacencyList *graph) {
    size_t result = 0;
    for (size_t i = 0; i < graph->size; ++i) {
        if (linked_list_contains(graph->nodes[i]->successors, node->value)) {
            ++result;
        }
    }
    return result;
}

void print_alist(AdjacencyList *list) {

    for (size_t i = 0; i < list->size; ++i) {
        printf("%zu: ", i);
        linked_list_print(list->nodes[i]->successors);
    }
    printf("\n");
}

/// Randomly links two nodes in both the adjacency list and the matrix
/// This relies on the list having NODES_COUNT entries!
/// \param list The adjacency list
/// \param matrix The matrix
//TODO The amount of edges is supposed to be variable
void link_random(AdjacencyList *list) {
    srand((unsigned int) time(NULL));
    int matrix[list->size][list->size];

    // For any given node, there is a small chance it links to a different target node
    for (size_t from = 0; from < list->size; ++from) {
        for (size_t to = 0; to < list->size; ++to) {
            if (from != to && !(rand() % 7)) {
                link_alnodes(list->nodes[from], list->nodes[to]);
                matrix[from][to] = 1;
            }
        }
    }
}

int main() {
    AdjacencyList *graph = adjacency_list_create(10);

    link_random(graph);
    link_alnodes(graph->nodes[0], graph->nodes[1]);
    printf("%zu\n", indeg(graph->nodes[1], graph));
    print_alist(graph);

    adjacency_list_destroy(graph);
}