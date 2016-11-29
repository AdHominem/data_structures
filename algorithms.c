

#include <stddef.h>
#include <stdio.h>


void print_array(int *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}


//void bubblesort(unsigned array[], int lower_bound, int upper_bound) {
//
//    for (int i = upper_bound - 1; i >= lower_bound; --i) {
//        for (int j = lower_bound; j <= i; ++j) {
//            if (array[j] > array[j + 1]) {
//                unsigned temp = array[j];
//                array[j] = array[j + 1];
//                array[j + 1] = temp;
//             }
//        }
//    }
//}
//
//
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
    printf("Swapping array[%zu] = %d with array[%zu] = %d\n", a, array[a], b, array[b]);
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void bubblesort(int *array, size_t length) {
    for (size_t end_index = length - 1; end_index > 0; --end_index) {
        for (size_t start_index = 0; start_index < end_index; ++start_index) {
            if (array[start_index] > array[end_index]) {
                swap(array, start_index, end_index);
            }
        }
    }
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

    printf("Before: ");
    print_array(array, size);

    size_t i = partition(array, 0, size - 1);
    printf("Pivot: %zu\n", i);

    printf("After: ");
    print_array(array, size);

    printf("Before: ");
    print_array(array, size);

    i = partition(array, 0, i);
    printf("Pivot: %zu\n", i);

    printf("After: ");
    print_array(array, size);
}