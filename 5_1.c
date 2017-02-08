#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdbool.h>


typedef struct linked_list_node LinkedListNode;
typedef struct adjacency_list_node AdjacencyListNode;
typedef struct adjacency_list {
    AdjacencyListNode *head;
} AdjacencyList;
typedef struct linked_list {
    LinkedListNode *head;
} LinkedList;
struct linked_list_node {
    int value;
    LinkedListNode *next;
};
struct adjacency_list_node {
    int value;
    size_t order;
    LinkedList *successors;
    AdjacencyListNode *next;
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
    size_t result = 0;
    if (list) {
        LinkedListNode *head = list->head;

        while (head != NULL) {
            head = head->next;
            ++result;
        }
    }

    return result;
}

int linked_list_insert(LinkedList *list, int value, size_t index) {

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

void linked_list_add(LinkedList *list, int value) {
    if (list->head) {
        linked_list_insert(list, value, linked_list_get_length(list));
    } else {
        list->head = linked_list_node_create(value);
    }
}

void linked_list_print(LinkedList *list) {
    if (list) {
        LinkedListNode *head = list->head;

        while (head != NULL) {
            printf("%d ", head->value);
            head = head->next;
        }

        printf("\n");
    }
}

int linked_list_contains(LinkedList *list, int value) {
    if (list) {
        LinkedListNode *current = list->head;
        while (current) {
            if (current->value == value) {
                return 1;
            }
            current = current->next;
        }
    }
    return 0;
}

void linked_list_remove(LinkedList *list, int value) {
    LinkedListNode *current = list->head;
    LinkedListNode *last = NULL;
    while (current) {
        if (current->value == value) {
            if (last == NULL) {
                list->head = current->next;
            } else if (current->next == NULL) {
                last->next = NULL;
            } else {
                last->next = current->next;
            }
            free(current);
            return;
        }
        last = current;
        current = current->next;
    }
}


AdjacencyListNode *adjacency_list_node_create(int value) {
    AdjacencyListNode *result = malloc(sizeof(AdjacencyListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->successors = linked_list_create();
    result->next = NULL;

    return result;
}

AdjacencyListNode *adjacency_list_node_destroy_recursively(AdjacencyListNode *node) {
    if (node) {
        node->next = adjacency_list_node_destroy_recursively(node->next);
        node->successors = linked_list_destroy(node->successors);
    }
    free(node);
    return NULL;
}

AdjacencyListNode *adjacency_list_node_destroy(AdjacencyListNode *node) {
    if (node) {
        node->successors = linked_list_destroy(node->successors);
    }
    free(node);
    return NULL;
}

AdjacencyList *adjacency_list_create() {
    return calloc(1, sizeof(AdjacencyList));
}

AdjacencyList *adjacency_list_destroy(AdjacencyList *list) {
    if (list) {
        list->head = adjacency_list_node_destroy_recursively(list->head);
    }
    free(list);
    return NULL;
}

size_t adjacency_list_get_length(AdjacencyList *list) {
    AdjacencyListNode *head = list->head;
    size_t result = 0;

    while (head != NULL) {
        head = head->next;
        ++result;
    }

    return result;
}

AdjacencyListNode *adjacency_list_insert(AdjacencyList *list, int value, size_t index) {

    // Catch index out of range
    if (index > adjacency_list_get_length(list)) {
        perror("List index out of range!");
        return NULL;
    }

    AdjacencyListNode *head = list->head;
    AdjacencyListNode *to_insert = adjacency_list_node_create(value);
    if (to_insert == NULL) {
        return NULL;
    }

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
    } else {
        AdjacencyListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
    return to_insert;
}

//int adjacency_list_add_sorted(AdjacencyList *list, int value) {
//
//    LinkedListNode *to_insert = adjacency_list_node_create(value);
//    if (to_insert == NULL) {
//        return 1;
//    }
//
//    LinkedListNode *current_node = list->head;
//
//    // Catch empty list and append to front case
//    if (current_node == NULL || current_node->value > value) {
//        to_insert->next = current_node;
//        list->head = to_insert;
//    } else {
//        LinkedListNode *precursor = NULL;
//
//        for (size_t i = 0; i < adjacency_list_get_length(list); ++i) {
//
//            precursor = current_node;
//            current_node = current_node->next;
//
//            // current node is NULL? Then we are at the end and the value to insert is the largest in the list
//            // or is the current value larger than our value? then we can insert too
//            if (current_node == NULL || current_node->value > value) {
//                to_insert->next = current_node;
//                precursor->next = to_insert;
//                break;
//            }
//            // if not, move further in the list
//        }
//    }
//    return 0;
//}
//
//void adjacency_list_as_array(AdjacencyList *list, int *array) {
//
//    LinkedListNode *node = list->head;
//
//    for (size_t i = 0; i < adjacency_list_get_length(list); ++i) {
//        array[i] = node->value;
//        node = node->next;
//    }
//}

int adjacency_list_contains_value(AdjacencyList *list, int value) {
    if (list) {
        AdjacencyListNode *current = list->head;
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
    }
    return false;
}

AdjacencyListNode *adjacency_list_add(AdjacencyList *list, int value) {
    if (list->head && !adjacency_list_contains_value(list, value)) {
        return adjacency_list_insert(list, value, adjacency_list_get_length(list));
    } else {
        AdjacencyListNode *to_insert = adjacency_list_node_create(value);
        list->head = to_insert;
        return to_insert;
    }
}

void adjacency_list_link_nodes(AdjacencyListNode *from, AdjacencyListNode *to) {
    if (from && to && !linked_list_contains(from->successors, to->value)) {
        linked_list_add(from->successors, to->value);
    }
}

AdjacencyListNode *adjacency_list_search(AdjacencyList *list, int value) {
    if (list) {
        AdjacencyListNode *current = list->head;
        while (current) {
            if (current->value == value) {
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}

void adjacency_list_link_nodes_by_value(AdjacencyList *list, int from_value, int to_value) {
    AdjacencyListNode *from = adjacency_list_search(list, from_value);
    AdjacencyListNode *to = adjacency_list_search(list, to_value);

    if (from && to && !linked_list_contains(from->successors, to->value)) {
        linked_list_add(from->successors, to->value);
    }
}

void adjacency_list_remove(AdjacencyList *list, int value) {
    AdjacencyListNode *current = list->head;
    AdjacencyListNode *last = NULL;
    while (current) {
        if (current->value == value) {
            if (last == NULL) {
                list->head = current->next;
            } else if (current->next == NULL) {
                last->next = NULL;
            } else {
                last->next = current->next;
            }
            free(current);
        } else {
            linked_list_remove(current->successors, value);
        }

        last = current;
        current = current->next;
    }
}

int adjacency_list_contains(AdjacencyList *list, AdjacencyListNode *node) {
    if (list) {
        AdjacencyListNode *current = list->head;
        while (current) {
            if (current == node) {
                return 1;
            }
            current = current->next;
        }
    }
    return 0;
}


AdjacencyList *adjacency_list_copy(const AdjacencyList *source) {
    AdjacencyList *destination = adjacency_list_create();

    AdjacencyListNode *current = source->head;
    while (current) {
        AdjacencyListNode *copied_node = adjacency_list_add(destination, current->value);
        LinkedListNode *current_successor = current->successors->head;
        while (current_successor) {

            linked_list_add(copied_node->successors, current_successor->value);
            current_successor = current_successor->next;
        }
        current = current->next;
    }
    return destination;
}


size_t indeg(const int value, const AdjacencyList *graph) {
    size_t result = 0;

    AdjacencyListNode *current = graph->head;
    while (current) {
        if (linked_list_contains(current->successors, value)) {
            ++result;
        }
        current = current->next;
    }

    return result;
}

void adjacency_list_print(AdjacencyList *list) {

    AdjacencyListNode *current = list->head;
    while (current) {
        printf("%d\t(%zu):\t", current->value, current->order);
        linked_list_print(current->successors);
        current = current->next;
    }
}

/// Randomly links two nodes in both the adjacency list and the matrix
/// This relies on the list having NODES_COUNT entries!
/// \param list The adjacency list
/// \param matrix The matrix
//TODO The amount of edges is supposed to be variable
void link_random(AdjacencyList *list, int chance_not_to_link) {
    srand((unsigned int) time(NULL));

    // For any given node, there is a small chance it links to a different target node
    AdjacencyListNode *from = list->head;
    AdjacencyListNode *to = list->head;
    while (from) {
        while (to) {
            if (from != to && !(rand() % chance_not_to_link)) {
                adjacency_list_link_nodes(from, to);
            }
            to = to->next;
        }
        to = list->head;
        from = from->next;
    }
}

int topological_sort(AdjacencyList *graph) {
    AdjacencyList *copy = adjacency_list_copy(graph);
    size_t order = 1;

    // graph is cyclic if one full loop does not lead to a change
    size_t changed = true;
    while (copy->head && changed) {
        changed = false;
        AdjacencyListNode *current = copy->head;
        while (current) {
            // if indegree == 0, then assign a fresh number and remove
            if (indeg(current->value, copy) == 0) {
                adjacency_list_search(graph, current->value)->order = order++;
                adjacency_list_remove(copy, current->value);
                changed = true;
            }
            current = current->next;
        }
    }

    adjacency_list_destroy(copy);

    return changed ? 0 : 1;
}

void add_random(AdjacencyList *list, int limit) {
    srand((unsigned int) time(NULL));

    int random = rand() % limit;
    int nodes = random < 3 ? 3 : random;
    for (int i = 0; i < nodes; ++i) {
        adjacency_list_add(list, i);
    }
}

int main() {
    AdjacencyList *list = adjacency_list_create();

    add_random(list, 15);
    link_random(list, 10);
    int status = topological_sort(list);

    printf("This graph is");
    printf(status ? " cyclic\n" : " acyclic\n");
    adjacency_list_print(list);

    adjacency_list_destroy(list);
}