#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define TABLE_SIZE 128

typedef struct linked_list_node LinkedListNode;
struct linked_list_node {
    int value;
    LinkedListNode *next;
};
typedef struct linked_list {
    LinkedListNode *head;
} LinkedList;
typedef size_t (*hash_function)(const int *);
typedef struct hash_table {
    LinkedList *rows[TABLE_SIZE];
    hash_function hash_function;
    size_t argument_count;
} HashTable;

size_t hash_modulo(const int *parameters) ;
size_t hash_universal(const int *parameters) ;
HashTable *hash_table_destroy(HashTable *hash_table) ;
void hash_table_add(HashTable *hash_table, const int *parameters) ;
size_t linked_list_get_length(LinkedList *list) ;
HashTable *hash_table_create(const hash_function hash_function, const size_t argument_count) ;
size_t get_max_chain_length(int input[TABLE_SIZE], const hash_function hash_function, int *hash_parameters) ;

int main() {
    srand((unsigned) time(NULL));
    size_t maximum_chain_length = 0;
    size_t sum_of_maximum_chain_lengths = 0;
    int *maximum_chain_input;
    const size_t attempts = 100;

    // Try 100 random input arrays of size 128 each and calculate the average longest chain and the total longest chain
    for (size_t k = 0; k < attempts; ++k) {

        int input[TABLE_SIZE];
        for (size_t i = 0; i < TABLE_SIZE; ++i) {
            input[i] = rand();
        }

        size_t current_maximum_chain_length = get_max_chain_length(input, hash_modulo, NULL);
        if (!current_maximum_chain_length) {
            perror("An error occurred during memory allocation!");
            return 1;
        }

        if (current_maximum_chain_length > maximum_chain_length) {
            maximum_chain_length = current_maximum_chain_length;
            maximum_chain_input = input;
        }
        sum_of_maximum_chain_lengths += current_maximum_chain_length;
    }

    printf("Average maximum chain length: %lf\n", sum_of_maximum_chain_lengths / (double) attempts);
    printf("Longest chain was: %zu\n", maximum_chain_length);

    // Now use the input creating the longest chain and use it 100 times with universal hashing instead
    sum_of_maximum_chain_lengths = 0;
    maximum_chain_length = 0;

    int params[2];
    while (!(params[0] = rand() % 491));
    params[1] = rand() % 491;

    for (size_t k = 0; k < attempts; ++k) {
        size_t current_maximum_chain_length = get_max_chain_length(maximum_chain_input, hash_universal, params);
        if (!current_maximum_chain_length) {
            perror("An error occurred during memory allocation!");
            return 1;
        }

        maximum_chain_length = (current_maximum_chain_length > maximum_chain_length)
                               ? current_maximum_chain_length : maximum_chain_length;
        sum_of_maximum_chain_lengths += current_maximum_chain_length;
    }

    printf("Average maximum chain length (universal hashing): %lf\n",
           sum_of_maximum_chain_lengths / (double) attempts);

    printf("Longest chain was: %zu\n", maximum_chain_length);

    return 0;
}

size_t get_max_chain_length(int input[TABLE_SIZE], const hash_function hash_function, int *hash_parameters) {
    HashTable *hash_table = (hash_function == hash_modulo)
                            ? hash_table_create(hash_modulo, 0) : hash_table_create(hash_universal, 2);
    if (!hash_table) {
        return 0;
    }

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        int parameters[1 + hash_table->argument_count];
        parameters[0] = input[i];
        for (size_t j = 1; j < hash_table->argument_count; ++j) {
            parameters[j] = hash_parameters[j - 1];
        }
        hash_table_add(hash_table, parameters);
    }

    size_t maximum_chain_length = 0;
    for (int j = 0; j < TABLE_SIZE; ++j) {
        const size_t length = linked_list_get_length(hash_table->rows[j]);
        maximum_chain_length = (length > maximum_chain_length) ? length : maximum_chain_length;
    }

    hash_table_destroy(hash_table);

    return maximum_chain_length;
}

size_t hash_modulo(const int *parameters) {
    return (size_t) (parameters[0] % TABLE_SIZE);
}

size_t hash_universal(const int *parameters) {
    return ((parameters[1] * (size_t) parameters[0] + parameters[2]) % 491) % TABLE_SIZE;
}

LinkedList *linked_list_create() {
    return calloc(1, sizeof(LinkedList));
}

HashTable *hash_table_create(const hash_function hash_function, const size_t argument_count) {
    HashTable *result = malloc(sizeof(HashTable));

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        result->rows[i] = linked_list_create();
        if (!result->rows[i]) {
            return hash_table_destroy(result);
        }
    }

    result->hash_function = hash_function;
    result->argument_count = argument_count;

    return result;
}

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

void linked_list_add(LinkedList *list, int value) {
    linked_list_insert(list, value, linked_list_get_length(list));
}

HashTable *hash_table_destroy(HashTable *hash_table) {
    if (hash_table) {
        for (size_t i = 0; i < TABLE_SIZE; ++i) {
            hash_table->rows[i] = linked_list_destroy(hash_table->rows[i]);
        }
    }
    free(hash_table);
    return NULL;
}

void hash_table_add(HashTable *hash_table, const int *parameters) {
    size_t row_index = hash_table->hash_function(parameters);
    linked_list_add(hash_table->rows[row_index], parameters[0]);
}