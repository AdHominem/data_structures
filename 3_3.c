#include "algorithms.h"
#include <time.h>

int bucketsort_modified(double *array, size_t length) {

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

    size_t biggest_bucket = 0;

    for (size_t i = 0; i < length; i++) {
        Bucket *relevant_bucket = &buckets[(size_t) (length * array[i])];
        relevant_bucket->values[relevant_bucket->count] = array[i];
        ++relevant_bucket->count;
        if (relevant_bucket->count > biggest_bucket) {
            biggest_bucket = relevant_bucket->count;
        }
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

    printf("Biggest bucket: %zu\n", biggest_bucket);
    return 0;
}

int main() {
    srand((unsigned) time(NULL));
    size_t size = 100;
    double array[size];
    for (size_t i = 0; i < size; ++i) {
        array[i] = (double) (rand() - 1) / (double) RAND_MAX;
    }

    printf("Generating a list of random doubles in [0, 1):\n");
    print_array(array, size, TYPE_DOUBLE);

    if (bucketsort_modified(array, size)) {
        perror("An error occurred during memory allocation!");
        return 1;
    }

    printf("Sorted:\n");
    print_array(array, size, TYPE_DOUBLE);

    return 0;
}