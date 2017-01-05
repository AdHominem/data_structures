#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 1000
#define SIZE_TO_SEARCH 100

void measure_performance(const char *name, const int *array, const int *to_search,
                         int (*function)(const int *, const size_t, const int, size_t *)) ;
int binary_search_modified(const int *array, const size_t size, const int number, size_t *iterations) ;
int interpolation_search_modified(const int *array, const size_t size, const int key, size_t *iterations) ;
size_t test_searching_algorithm(const int array[SIZE], const int to_search[SIZE_TO_SEARCH],
                                int (*algorithm)(const int*, const size_t, const int, size_t*)) ;
void quicksort(int *array, size_t size) ;
void swap(int *array, size_t a, size_t b) ;

int main() {

    // Generate an array with random values between 0 and 9999
    srand((unsigned) time(NULL));
    int array[SIZE];
    for (size_t i = 0; i < SIZE; ++i) {
        array[i] = rand() % 10000;
    }

    // Determine 100 random values to search for out of the array
    // (chose the first 100, they are random because the array is unsorted)
    int to_search[SIZE_TO_SEARCH];
    for (size_t k = 0; k < SIZE_TO_SEARCH; ++k) {
        to_search[k] = array[k];
    }

    quicksort(array, SIZE);

    measure_performance("Binary Search", array, to_search, binary_search_modified);
    measure_performance("Interpolation", array, to_search, interpolation_search_modified);

    // Determine 100 real random values to search for
    for (size_t k = 0; k < SIZE_TO_SEARCH; ++k) {
        to_search[k] = rand() % 10000;
    }

    measure_performance("Binary Search Random", array, to_search, binary_search_modified);
    measure_performance("Interpolation Random", array, to_search, interpolation_search_modified);

    return 0;
}

void measure_performance(const char *name, const int *array, const int *to_search,
                         int (*function)(const int *, const size_t, const int, size_t *)) {
    size_t iterations = test_searching_algorithm(array, to_search, function);

    printf("\n%s:\n", name);
    printf("Iterations: %zu\n", iterations);
    printf("Average: %f\n", iterations / (double) SIZE_TO_SEARCH);
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

int binary_search_modified(const int *array, const size_t size, const int number, size_t *iterations) {

    int upper_bound = (int) size - 1;
    int lower_bound = 0;

    while (lower_bound <= upper_bound) {
        ++*iterations;

        int middle = lower_bound + (upper_bound - lower_bound) / 2;

        if (number == array[middle]) {
            return 1;
        } else if (number > array[middle]) {
            lower_bound = middle + 1;
        } else /*if (number < array[middle]) */ {
            upper_bound = middle - 1;
        }
    }

    return 0;
}

int interpolation_search_modified(const int *array, const size_t size, const int key, size_t *iterations) {

    if (size == 0) {
        return 0;
    }

    int lower_bound = 0;
    int upper_bound = (int) size - 1;
    int middle;

    while (array[upper_bound] != array[lower_bound]
           && key >= array[lower_bound]
           && key <= array[upper_bound]) {
        ++*iterations;
        middle = lower_bound + ((key - array[lower_bound]) * (upper_bound - lower_bound)
                                / (array[upper_bound] - array[lower_bound]));
        if (array[middle] < key) {
            lower_bound = middle + 1;
        } else if (key < array[middle]) {
            upper_bound = middle - 1;
        } else {
            return 1;
        }
    }

    return key == array[lower_bound];
}

void quicksort(int *array, size_t size) {
    if (size < 2) return;

    // This part determines the pivot element and places all array elements to the LEFT of the pivot index
    // which are smaller and all the ones which are bigger to the RIGHT
    int pivot = array[size / 2];
    size_t i, j;
    for (i = 0, j = size - 1; ; i++, j--) {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        if (i >= j) break;
        swap(array, i, j);
    }

    quicksort(array, i);
    quicksort(array + i, size - i);
}

void swap(int *array, size_t a, size_t b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}



