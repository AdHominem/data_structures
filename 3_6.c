#include "data_structures.h"
#include <time.h>

size_t hash_modulo(const int *parameters) {
    return (size_t) (parameters[0] % TABLE_SIZE);
}

// prime 491 is used because its > 128, which is the size of the hash table
size_t hash_universal(const int *parameters) {
    return ((parameters[1] * (size_t) parameters[0] + parameters[2]) % 491) % TABLE_SIZE;
}

size_t get_max_chain_length(int input[TABLE_SIZE], size_t (*hash_function)(const int *), int *hash_parameters) {
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
        const size_t length = length_of_(hash_table->rows[j]);
        maximum_chain_length = (length > maximum_chain_length) ? length : maximum_chain_length;
    }

    //hash_table_print(hash_table);
    hash_table_destroy(hash_table);

    return maximum_chain_length;
}

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