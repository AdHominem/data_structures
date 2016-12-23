#include "algorithms.h"
#include <time.h>

#define SIZE 1000
#define SIZE_TO_SEARCH 100

int binary_search_modified(const int *array, const size_t size, const int number, size_t *iterations) {

    size_t last = size;
    size_t first = 0;

    while (first <= last) {
        ++*iterations;
        size_t middle = first + (last - first) / 2;

        if (number == array[middle]) {
            return 1;
        } else if (number > array[middle]) {
            first = middle + 1;
        } else {
            last = middle - 1;
        }
    }

    return 0;
}

int interpolation_search_modified(const int *array, const size_t size, const int key, size_t *iterations) {
    size_t low = 0;
    size_t high = size - 1;
    size_t middle;

    while (array[high] != array[low] && key >= array[low] && key <= array[high]) {
        ++*iterations;
        middle = low + ((key - array[low]) * (high - low) / (array[high] - array[low]));
        if (array[middle] < key) {
            low = middle + 1;
        } else if (key < array[middle]) {
            high = middle - 1;
        } else {
            return 1;
        }
    }

    return key == array[low];
}

/// Uses the specified search algorithm so look for a set of values in a larger array
/// \param algorithm The search algorithm to use
/// \return The amount of iterations necessary for the search
size_t test_searching_algorithm(const int array[SIZE], const int to_search[SIZE_TO_SEARCH],
                                int (*algorithm)(const int*, const size_t, const int, size_t*)) {
    size_t iterations = 0;
    for (size_t l = 0; l < SIZE_TO_SEARCH; ++l) {
        algorithm(array, SIZE, to_search[l], &iterations);
    }

    return iterations;
}

int main() {

    // Generate an array with random values between 0 and 9999
    srand((unsigned) time(NULL));
    int array[SIZE];
    for (size_t i = 0; i < SIZE; ++i) {
        array[i] = rand() % 10000;
    }

    // Determine 100 random values to search for (random because the array is yet to be sorted)
    int to_search[SIZE_TO_SEARCH];
    for (size_t k = 0; k < SIZE_TO_SEARCH; ++k) {
        to_search[k] = array[k];
    }

    quicksort(array, SIZE);

    size_t iterations = test_searching_algorithm(array, to_search, binary_search_modified);

    printf("\nBinary Search:\n");
    printf("Iterations: %zu\n", iterations);
    printf("Average: %f\n", iterations / (double) SIZE_TO_SEARCH);

    iterations = test_searching_algorithm(array, to_search, interpolation_search_modified);

    printf("\nInterpolation:\n");
    printf("Iterations: %zu\n", iterations);
    printf("Average: %f\n", iterations / (double) SIZE_TO_SEARCH);

    return 0;
}