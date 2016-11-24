

#include <stddef.h>
#include <stdio.h>


void print_array(int *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%d ", array[i]);
    }
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

size_t divide(int *array, size_t left, size_t right) {
    size_t i = left;
    size_t  j = right - 1;
    int pivot = array[right];

    while (i < j) {
        while (array[i] <= pivot && i < right) {
            ++i;
            printf("i is now %zu and array[i] now points to %d\t", i, array[i]);
            //print_array(array, right);
            printf("\n");
        }
        while (array[j] >= pivot && j > left) {
            ++j;
            printf("j is now %zu and array[j] now points to %d\t", j, array[j]);
            //print_array(array, right);
            printf("\n");
        }
        if (i < j) {
            swap(array, i, j);
        }
    }
    return i;
}

int main() {
    size_t size = 5;
    int array[] = {2, 4, 1, 9, 3};
    //bubblesort(array, size - 1);
    //quickSort(array, 0, 5);
    size_t i = divide(array, 0, size - 1);
    print_array(array, size);

}