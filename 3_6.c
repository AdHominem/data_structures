#include "data_structures.h"
#include <string.h>
#include <time.h>

size_t get_max_chain_length(int input[TABLE_SIZE], size_t (*hash_function)(int)) {
    HashTable *hash_table = hash_table_create(hash_function);
    if (!hash_table) {
        return 0;
    }

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        hash_table_add(hash_table, input[i]);
    }

    size_t maximum_chain_length = 0;
    for (int j = 0; j < TABLE_SIZE; ++j) {
        size_t length = length_of_(hash_table->rows[j]);
        maximum_chain_length = (length > maximum_chain_length) ? length : maximum_chain_length;
    }

    //hash_table_print(hash_table);
    hash_table_destroy(hash_table);

    return maximum_chain_length;
}

size_t modulo_table_size(int value) {
    return (size_t) (value % TABLE_SIZE);
}

size_t universal_hashing(int value) {
    size_t a = (size_t) (rand() % 491 + 1);
    size_t b = (size_t) (rand() % 491);
    //printf("a: %zu, b:%zu\n", a, b);
    return ((a * value + b) % 491) % TABLE_SIZE;
}

int main() {

    srand((unsigned) time(NULL));
    size_t global_maximum_chain_length = 0;
    size_t total_maximum_chain_length = 0;
    int maximum_chain_input[TABLE_SIZE];
    size_t attempts = 100;

    // Try 100 random input arrays of size 128 each and calculate the average longest chain and the total longest chain
    for (size_t k = 0; k < attempts; ++k) {

        int input[TABLE_SIZE];
        for (size_t i = 0; i < TABLE_SIZE; ++i) {
            input[i] = rand();
        }

        size_t max_chain_length = get_max_chain_length(input, modulo_table_size);
        if (!max_chain_length) {
            return 1;
        }

        if (max_chain_length > global_maximum_chain_length) {
            global_maximum_chain_length = max_chain_length;
            memcpy(maximum_chain_input, input, TABLE_SIZE);
        }
        total_maximum_chain_length += max_chain_length;
    }

    printf("Average maximum chain length: %lf\n", total_maximum_chain_length / (double) attempts);
    printf("Longest chain was: %zu\n", global_maximum_chain_length);

    // Now use the input creating the longest chain and use it 100 times with universal hashing instead
    total_maximum_chain_length = 0;
    global_maximum_chain_length = 0;
    for (size_t k = 0; k < attempts; ++k) {
        size_t max_chain_length = get_max_chain_length(maximum_chain_input, universal_hashing);
        if (!max_chain_length) {
            return 1;
        }

        if (max_chain_length > global_maximum_chain_length) {
            global_maximum_chain_length = max_chain_length;
        }
        total_maximum_chain_length += max_chain_length;
    }

    printf("Average maximum chain length (universal hashing): %lf\n",
           total_maximum_chain_length / (double) attempts);
    printf("Longest chain was: %zu\n", global_maximum_chain_length);

    return 0;
}