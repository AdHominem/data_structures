#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_INT "TYPE_INT"
#define TYPE_DOUBLE "TYPE_DOUBLE"

typedef struct linked_list_node LinkedListNode;
struct linked_list_node {
    double value;
    LinkedListNode *next;
};
typedef struct linked_list {
    LinkedListNode *head;
} LinkedList;

int array_print(const void *array, const size_t length, const char *type) ;
int bucketsort_modified(double *array, const size_t length) ;
LinkedList *linked_list_create() ;
LinkedList *linked_list_destroy(LinkedList *list) ;
void linked_list_add(LinkedList *list, double value) ;
void bubblesort(double *array, size_t length) ;
size_t linked_list_get_length(const LinkedList *list) ;
void linked_list_as_array(const LinkedList *list, double *array) ;

int main() {
    srand((unsigned) time(NULL));
    const size_t size = 100;
    double array[size];
    for (size_t i = 0; i < size; ++i) {
        // Values must be < 1 and >= 0
        array[i] = rand() / ((double) RAND_MAX + 1);
    }

    printf("Generating a list of 100 random doubles in [0, 1):\n");
    array_print(array, size, TYPE_DOUBLE);

    if (bucketsort_modified(array, size)) {
        perror("An error occurred during memory allocation!");
        return 1;
    }

    printf("\nSorted:\n");
    array_print(array, size, TYPE_DOUBLE);

    return 0;
}

int bucketsort_modified(double *array, const size_t length) {

    // Initialize buckets and handle memory errors
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
        double bucket_as_array[bucket_size];
        linked_list_as_array(current_bucket, bucket_as_array);
        bubblesort(bucket_as_array, bucket_size);

        // Update biggest bucket
        biggest_bucket = (biggest_bucket < bucket_size) ? bucket_size : biggest_bucket;

        // Append the sorted bucket to the array
        for (size_t value_index = 0; value_index < bucket_size; value_index++) {
            array[array_index + value_index] = bucket_as_array[value_index];
        }

        array_index += bucket_size;
        linked_list_destroy(current_bucket);
    }

    printf("\nBiggest bucket: %zu\n", biggest_bucket);
    return 0;
}

// UTILITY

LinkedListNode *linked_list_node_create(const double value) {
    LinkedListNode *result = malloc(sizeof(LinkedListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

LinkedListNode *linked_list_node_destroy_recursively(LinkedListNode *node) {
    if (node) {
        node->next = linked_list_node_destroy_recursively(node->next);
    }
    free(node);
    return NULL;
}

LinkedList *linked_list_create() {
    return calloc(1, sizeof(LinkedList));
}

LinkedList *linked_list_destroy(LinkedList *list) {
    if (list) {
        list->head = linked_list_node_destroy_recursively(list->head);
    }
    free(list);
    return NULL;
}

size_t linked_list_get_length(const LinkedList *list) {
    size_t result = 0;

    if (list) {
        LinkedListNode *head = list->head;

        while (head != NULL) {
            head = head->next;
            ++result;
        }
    }

    return result;
}

int linked_list_insert(LinkedList *list, const double value, const size_t index) {

    // Catch index out of range
    if (index > linked_list_get_length(list)) {
        perror("List index out of range!");
        return 2;
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = linked_list_node_create(value);
    if (to_insert == NULL) {
        return 1;
    }

    // Catch append to front case
    // Else insert normally
    if (index == 0) {
        to_insert->next = head;
        list->head = to_insert;
    } else {
        LinkedListNode *before = head;

        for (size_t i = 0; i < index; ++i) {
            before = head;
            head = head->next;
        }

        before->next = to_insert;
        to_insert->next = head;
    }
    return 0;
}

void linked_list_as_array(const LinkedList *list, double *array) {

    LinkedListNode *node = list->head;

    for (size_t i = 0; i < linked_list_get_length(list); ++i) {
        array[i] = node->value;
        node = node->next;
    }
}

void linked_list_add(LinkedList *list, const double value) {
    linked_list_insert(list, value, linked_list_get_length(list));
}

void swap(double *array, const size_t a, const size_t b) {
    double temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void bubblesort(double *array, const size_t length) {
    if (!length) return;

    for (size_t end_index = length - 1; end_index > 0; --end_index) {
        for (size_t start_index = 0; start_index < end_index; ++start_index) {
            if (array[start_index] > array[end_index]) {
                swap(array, start_index, end_index);
            }
        }
    }
}

void print_double(const void *number) {
    double result = *((double*)number);
    printf("%f ", result);
}

void print_int(const void *number) {
    int result = *((int*) number);
    printf("%d ", result);
}

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
