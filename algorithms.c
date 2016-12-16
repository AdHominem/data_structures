

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
    srand((unsigned) time(NULL));
    size_t size = 100;
    double array[size];
    for (size_t i = 0; i < size; ++i) {
        array[i] = (double) (rand() - 1) / (double) RAND_MAX;
    }

    print_double_array(array, size);

    bucketsort(array, size);
    print_double_array(array, size);


}