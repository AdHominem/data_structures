#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#define TYPE_INT "TYPE_INT"
#define TYPE_DOUBLE "TYPE_DOUBLE"

#include "data_structures.h"
#include <stddef.h>
#include <string.h>


// ### UTILITY ###


void print_int(const void *number) {
    int result = *((int*) number);
    printf("%d ", result);
}

void print_double(const void *number) {
    double result = *((double*)number);
    printf("%f ", result);
}

/// Prints arrays of arbitrary type
/// \param array The array to print
/// \param length The length of the array
/// \param type The data type xxx of the array elements in the form of "TYPE_XXX"
/// \return 1 in case of an invalid data type
int array_print(const void *array, const size_t length, const char *type) {

    void (*function)(const void *);
    void *cast_array;
    size_t size;

    if (!strcmp(type, TYPE_INT)) {
        function = print_int;
        cast_array = (int *) array;
        size = sizeof(int);
    } else if (!strcmp(type, TYPE_DOUBLE)) {
        function = print_double;
        cast_array = (double *) array;
        size = sizeof(double);
    } else {
        return 1;
    }

    for (size_t i = 0; i < length; ++i) {
        function(cast_array + i * size);
    }
    printf("\n");
    return 0;
}

/*void print_array(int *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void print_double_array(double *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%.3f ", array[i]);
    }
    printf("\n");
}*/

void swap(int *array, size_t a, size_t b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void swap_d(double *array, size_t a, size_t b) {
    double temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}


// ### ALGORITHMS ###

/// Checks the heap property for an element at given index, compares it with its two child nodes. If one of its children
/// is bigger, it will swap them and call this function again on the child index.
/// \param array The heap to check as an array
/// \param index The index to start checking at
/// \param heap_size The size of the heap
void heap_max_heapify(int *array, const size_t index, const size_t heap_size) {
    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;
//    printf("\tIncoming: \n\t");
//    print_array(array, heap_size, TYPE_INT);

//    printf("I am %d (%zu), my left child is %d (%zu) and my right child is %d (%zu)\n", array[index], index,
//           array[left_child_index], left_child_index, array[right_child_index], right_child_index);

    size_t maximum = ((left_child_index < heap_size) && (array[left_child_index] > array[index]))
                     ? left_child_index : index;

    if ((right_child_index < heap_size) && (array[right_child_index] > array[maximum])) {
        maximum = right_child_index;
    }
    if (maximum != index) {
        swap(array, index, maximum);
//        printf("\tAfter swapping: \n\t");
//        print_array(array, heap_size, TYPE_INT);
        heap_max_heapify(array, maximum, heap_size);
    }

}

/// Constructs a binary heap out of an arbitrary array.
/// \param array The array to build the heap of
/// \param size The size of the array
void heap_build_max(int *array, const size_t size) {
    if (size < 2) return;

    // Note that it is only necessary to heapify the top (size / 2 - 1) elements,
    // since the lower half will only contain leafs
    for (size_t i = size / 2; i-- != 0;) {
        //printf("Size in build_max_heap is %zu\n", i);
        heap_max_heapify(array, i, size);
    }
}

void heapsort(int *array, size_t size) {
    if (size < 2) return;

    heap_build_max(array, size);
    //print_array(array, size, TYPE_INT);
    while (--size) {
        //printf("Size is now %zu, swapping %d and %d\n", size, array[0], array[size]);
        swap(array, 0, size);
        heap_max_heapify(array, 0, size);
    }
}

void merge(int *array, size_t size, size_t middle) {
    int *temp = malloc(size * sizeof (int));
    array_print(array, size, TYPE_INT);

    for (size_t lpos = 0, rpos = middle, i = 0; i < size; i++) {
        int value;
        if (rpos == size) {
            value = array[lpos];
            ++lpos;
        } else if (lpos == middle || array[rpos] < array[lpos]) {
            value = array[rpos];
            ++rpos;
        } else {
            value = array[lpos];
            ++lpos;
        }
        temp[i] = value;
    }

    // copy tmp to array
    for (size_t i = 0; i < size; i++) {
        array[i] = temp[i];
    }

    free(temp);
}

void mergesort(int *array, size_t size) {
    if (size < 2) return;

    size_t middle = size / 2;
    mergesort(array, middle);
    mergesort(array + middle, size - middle);
    merge(array, size, middle);
}

void minsort(int *array, size_t length) {

    for (size_t i = 0; i < length; ++i) {
        size_t smallest_index = i;
        int smallest = array[i];
        for (size_t j = i + 1; j < length; ++j) {
            if (array[j] < smallest) {
                swap(array, j, smallest_index);
                smallest = array[j];
                smallest_index = j;
            }
        }
    }
}

void bubblesort(int *array, size_t length) {
    if (!length) return;

    for (size_t end_index = length - 1; end_index > 0; --end_index) {
        for (size_t start_index = 0; start_index < end_index; ++start_index) {
            if (array[start_index] > array[end_index]) {
                swap(array, start_index, end_index);
            }
        }
    }
}

void bubblesort_d(double *array, size_t length) {
    if (!length) return;

    for (size_t end_index = length - 1; end_index > 0; --end_index) {
        for (size_t start_index = 0; start_index < end_index; ++start_index) {
            if (array[start_index] > array[end_index]) {
                swap_d(array, start_index, end_index);
            }
        }
    }
}

int bucketsort(int *array, const size_t length) {

    // Initialize buckets
    LinkedList *buckets[length];
    for (size_t i = 0; i < length; i++) {
        buckets[i] = linked_list_create();
        if (buckets[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                linked_list_destroy(buckets[j]);
            }
            return 1;
        }
    }

    // Put elements in buckets
    for (size_t i = 0; i < length; i++) {
        LinkedList *relevant_bucket = buckets[(size_t) (length * array[i])];
        linked_list_add(relevant_bucket, array[i]);
    }

    // Sort all buckets and insert them into the array
    size_t array_index = 0;
    size_t biggest_bucket = 0;
    for (size_t bucket_index = 0; bucket_index < length; ++bucket_index) {

        // Get bucket as array and sort it
        LinkedList *current_bucket = buckets[bucket_index];
        size_t bucket_size = linked_list_get_length(current_bucket);
        int bucket_as_array[bucket_size];
        linked_list_as_array(current_bucket, bucket_as_array);
        bubblesort(bucket_as_array, bucket_size);

        // Append the sorted bucket to the array
        for (size_t value_index = 0; value_index < bucket_size; value_index++) {
            array[array_index + value_index] = bucket_as_array[value_index];
        }

        array_index += bucket_size;
        linked_list_destroy(current_bucket);
    }
    return 0;
}

int binary_search(const int *array, const size_t size, const int number) {

    int upper_bound = (int) size - 1;
    int lower_bound = 0;

    while (lower_bound <= upper_bound) {

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

int interpolation_search(const int *array, const size_t size, const int key) {

    if (size == 0) {
        return 0;
    }

    int lower_bound = 0;
    int upper_bound = (int) size - 1;
    int middle;

    while (array[upper_bound] != array[lower_bound]
           && key >= array[lower_bound]
           && key <= array[upper_bound]) {
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


/* TESTING */

void test_search_algorithm(int (*algorithm)(const int *, const size_t, const int, size_t *)) {
    int a[] = {0,1,2,3,4,5,6,7,8,9};
    size_t iter;
    assert(algorithm(a, 10, -1, &iter) == 0);
    assert(algorithm(a, 10, 10, &iter) == 0);
    assert(algorithm(a, 10, 0, &iter));
    assert(algorithm(a, 10, 9, &iter));

    int b[] = {0};
    assert(algorithm(b, 1, -1, &iter) == 0);
    assert(algorithm(b, 1, 2, &iter) == 0);
    assert(algorithm(b, 1, 0, &iter));

    int c[] = {};
    assert(algorithm(b, 0, -1, &iter) == 0);
    assert(algorithm(b, 0, 0, &iter) == 0);

    int d[] = {0,1,2,2,4};
    assert(algorithm(d, 5, 2, &iter));
}

/// Tests if the sorting algorithm works with arbitrary arrays, zero length arrays and already sorted ones.
/// \param sort The sorting algorithm
/// \return 1 in case of a mistake
int test_sorting_algorithm(void (*sort)(int *, size_t)) {
    size_t size = 7;
    int ints_sorted[] = {-4, 0, 1, 4, 7, 12, 18};
    int ints[] = {4,7,18,-4,0,12,1};

    sort(ints, size);

    for (size_t i = 0; i < size; ++i) {
        if (ints[i] != ints_sorted[i]) {
            printf("Test failed!\n");
            array_print(ints, size, TYPE_INT);
            return 1;
        }
    }
    printf("Test arbitrary array successful!\n");

    size = 0;
    int ints_empty[] = {};

    sort(ints_empty, size);
    printf("Test zero length array successful!\n");


    size = 7;
    int ints_sorted_again[] = {-4, 0, 1, 4, 7, 12, 18};
    int ints_sorted_too[] = {-4, 0, 1, 4, 7, 12, 18};

    sort(ints_sorted_again, size);

    for (size_t i = 0; i < size; ++i) {
        if (ints_sorted_again[i] != ints_sorted_too[i]) {
            printf("Test already sorted failed!\n");
            array_print(ints_sorted_again, size, TYPE_INT);
            return 1;
        }
    }

    printf("All tests successful!\n");

    return 0;
}

#endif