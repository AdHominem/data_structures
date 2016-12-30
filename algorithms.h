#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#define TYPE_INT "TYPE_INT"
#define TYPE_DOUBLE "TYPE_DOUBLE"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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
int print_array(const void *array, const size_t length, const char *type) {

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
void max_heapify(int *array, size_t index, size_t heap_size) {
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
        max_heapify(array, maximum, heap_size);
    }

}

/// Constructs a binary heap out of an arbitrary array.
/// \param array The array to build the heap of
/// \param size The size of the array
void build_max_heap(int *array, size_t size) {
    if (size < 2) return;

    // Note that it is only necessary to heapify the top (size / 2 - 1) elements,
    // since the lower half will only contain leafs
    for (size_t i = size / 2; i-- != 0;) {
        //printf("Size in build_max_heap is %zu\n", i);
        max_heapify(array, i, size);
    }
}

void heapsort(int *array, size_t size) {
    if (size < 2) return;

    build_max_heap(array, size);
    //print_array(array, size, TYPE_INT);
    while (--size) {
        //printf("Size is now %zu, swapping %d and %d\n", size, array[0], array[size]);
        swap(array, 0, size);
        max_heapify(array, 0, size);
    }
}

void merge(int *array, size_t size, size_t middle) {
    int *temp = malloc(size * sizeof (int));
    print_array(array, size, TYPE_INT);

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

//O(n)
int bucketsort(double *array, size_t length) {

    typedef struct bucket {
        size_t count;
        double *values;
    } Bucket;

    Bucket buckets[length];

    for (size_t i = 0; i < length; i++) {
        buckets[i].count = 0;
        buckets[i].values = malloc(length * sizeof(double));
        if (!buckets[i].values) {
            for (size_t j = 0; j < i; ++j) {
                free(buckets[j].values);
            }
            return 1;
        }
    }

    for (size_t i = 0; i < length; i++) {
        Bucket *relevant_bucket = &buckets[(size_t) (length * array[i])];
        relevant_bucket->values[relevant_bucket->count] = array[i];
        ++relevant_bucket->count;
    }

    size_t array_index = 0;
    for (size_t bucket_index = 0; bucket_index < length; ++bucket_index) {
        Bucket *current_bucket = &buckets[bucket_index];
        bubblesort_d(current_bucket->values, current_bucket->count);

        for (size_t value_index = 0; value_index < current_bucket->count; value_index++) {
            array[array_index + value_index] = current_bucket->values[value_index];
        }

        array_index += current_bucket->count;
        free(current_bucket->values);
    }

    return 0;
}

int binary_search(const int *array, const size_t size, const int number) {

    size_t last = size;
    size_t first = 0;

    while (first <= last) {
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

int interpolation_search(int *array, size_t size, int key) {
    size_t low = 0;
    size_t high = size - 1;
    size_t middle;

    while (array[high] != array[low] && key >= array[low] && key <= array[high]) {
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

#endif