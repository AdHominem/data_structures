

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_array(int *array, size_t length) {
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
}

//unsigned partition(unsigned array[], unsigned lower_bound, unsigned upper_bound) {
//    unsigned pivot, lb, ub, t;
//
//    pivot = array[lower_bound];
//
//    lb = lower_bound;
//    ub = upper_bound + 1;
//
//    while(TRUE) {
//        do {++lb;}
//        while (array[lb] <= pivot && lb <= upper_bound );
//
//        do {--ub;}
//        while (array[ub] > pivot );
//
//        if( lb >= ub ) break;
//        t = array[lb];
//        array[lb] = array[ub];
//        array[ub] = t;
//    }
//
//    t = array[lower_bound];
//    array[lower_bound] = array[ub];
//    array[ub] = t;
//    return ub;
//}
//
//void quickSort(unsigned array[], unsigned lower_bound, unsigned upper_bound) {
//    unsigned j;
//    if( lower_bound < upper_bound ) {
//        j = partition( array, lower_bound, upper_bound);
//        quickSort( array, lower_bound, j - 1);
//        quickSort( array, j + 1, upper_bound);
//    }
//}

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

// O(n^2)
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

/// Attempts to find the pivot value by scanning for two indices i and j which are both out of place in respect to
/// the pivot value. These indices can then be swapped. Once the indiced cross each other, the algorithm stops.
/// \param array The array to be partitioned.
/// \param low The lower starting index.
/// \param high The higher starting index.
/// \return The new pivot value.
size_t partition(int *array, size_t low, size_t high) {
    int pivot = array[high];
    size_t i = low;
    size_t j = high + 1;
    printf("partition(): pivot is now %d, i is %zu and j is %zu\n", pivot, i, j);
    while (1) {
        while (array[++i] < pivot);
        //printf("i is now %zu\n", i);
        while (array[--j] > pivot);
        //printf("j is now %zu\n", j);

        if (i >= j) {
            return j;
        }

        swap(array, i, j);
    }
}

void quicksort(int *array, size_t low, size_t high) {
    size_t pivot;
    if (low < high) {
        pivot = partition(array, low, high);
        quicksort(array, low, pivot);
        //quicksort(array, pivot + 1, high);
    }
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

int main() {
    size_t size = 9;
    int array[] = {54, 26, 93, 17, 77, 31, 44, 55, 20};






    /*
     * 54, 26, 93, 17, 77, 31, 44, 55, 20
     * pivot = 20
     * i = 0
     * j = 3
     * 17, 26, 93, 54, 77, 31, 44, 55, 20
     * i = 1
     * j = 0
     * Return 0
     */

//    printf("Before: ");
//    print_array(array, size);
//
//    size_t i = partition(array, 0, size - 1);
//    printf("Pivot: %zu\n", i);
//
//    printf("After: ");
//    print_array(array, size);
//
//    printf("Before: ");
//    print_array(array, size);
//
//    i = partition(array, 0, i);
//    printf("Pivot: %zu\n", i);
//
//    printf("After: ");
//    print_array(array, size);
}