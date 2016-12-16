#include "algorithms.h"
#include <time.h>

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

int interpolation_search_modified(int *array, size_t size, int key, size_t *iterations) {
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

int main() {
    size_t size = 1000;
    size_t size_to_search = size / 10;
    int array[size];
    srand((unsigned) time(NULL));
    for (size_t i = 0; i < size; ++i) {
        array[i] = rand() % 10000;
    }

    // Chose 100 random values (random because the array is yet to be sorted)
    int to_search[size_to_search];
    for (size_t k = 0; k < size_to_search; ++k) {
        to_search[k] = array[k];
    }

    quicksort(array, size);

    size_t iterations = 0;
    for (size_t l = 0; l < size_to_search; ++l) {
        binary_search_modified(array, size, to_search[l], &iterations);
    }

    printf("\nBinary Search:\n");
    printf("Iterations: %zu\n", iterations);
    printf("Average: %f\n", iterations / (double) size_to_search);

    // Chose 100 random values (random because the array is yet to be sorted)
    for (size_t k = 0; k < size_to_search; ++k) {
        to_search[k] = rand() % 10000;
    }

    iterations = 0;
    for (size_t l = 0; l < size_to_search; ++l) {
        interpolation_search_modified(array, size, to_search[l], &iterations);
    }

    printf("\nInterpolation:\n");
    printf("Iterations: %zu\n", iterations);
    printf("Average: %f\n", iterations / (double) size_to_search);

    return 0;
}