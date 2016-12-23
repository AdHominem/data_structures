#include "data_structures.h"
#include <string.h>
#include <time.h>

size_t get_max_chain_length(int input[TABLE_SIZE], size_t (*hash_function)(const int)) {
    HashTable *hash_table = hash_table_create(hash_function);
    if (!hash_table) {
        return 0;
    }

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        hash_table_add(hash_table, input[i]);
    }

    size_t maximum_chain_length = 0;
    for (int j = 0; j < TABLE_SIZE; ++j) {
        const size_t length = length_of_(hash_table->rows[j]);
        maximum_chain_length = (length > maximum_chain_length) ? length : maximum_chain_length;
    }

    //hash_table_print(hash_table);
    hash_table_destroy(hash_table);

    return maximum_chain_length;
}

size_t hash_modulo(const int value) {
    return (size_t) (value % TABLE_SIZE);
}

// prime 491 is used because its > 128, which is the size of the hash table
size_t hash_universal(const int value) {
    const size_t a = (size_t) (rand() % 491 + 1);
    const size_t b = (size_t) (rand() % 491);
    //printf("a: %zu, b:%zu\n", a, b);
    return ((a * (size_t) value + b) % 491) % TABLE_SIZE;
}

int main() {

    srand((unsigned) time(NULL));
    size_t global_maximum_chain_length = 0;
    size_t total_maximum_chain_length = 0;
    int *maximum_chain_input;
    const size_t attempts = 100;

    // Try 100 random input arrays of size 128 each and calculate the average longest chain and the total longest chain
    for (size_t k = 0; k < attempts; ++k) {

        int input[TABLE_SIZE];
        for (size_t i = 0; i < TABLE_SIZE; ++i) {
            input[i] = rand();
        }

        size_t max_chain_length = get_max_chain_length(input, hash_modulo);
        if (!max_chain_length) {
            perror("An error occurred during memory allocation!");
            return 1;
        }

        if (max_chain_length > global_maximum_chain_length) {
            global_maximum_chain_length = max_chain_length;
            maximum_chain_input = input;
        }
        total_maximum_chain_length += max_chain_length;
    }

    printf("Average maximum chain length: %lf\n", total_maximum_chain_length / (double) attempts);
    printf("Longest chain was: %zu\n", global_maximum_chain_length);

    // Now use the input creating the longest chain and use it 100 times with universal hashing instead
    total_maximum_chain_length = 0;
    global_maximum_chain_length = 0;
    for (size_t k = 0; k < attempts; ++k) {
        size_t max_chain_length = get_max_chain_length(maximum_chain_input, hash_universal);
        if (!max_chain_length) {
            perror("An error occurred during memory allocation!");
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