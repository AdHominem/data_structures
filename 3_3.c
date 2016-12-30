#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_INT "TYPE_INT"
#define TYPE_DOUBLE "TYPE_DOUBLE"

// Data types
typedef struct llnode LinkedListNode;
struct llnode {
    double value;
    LinkedListNode *next;
};
typedef struct llist {
    LinkedListNode *head;
} LinkedList;

// Functions
int print_array(const void *array, const size_t length, const char *type) ;
int bucketsort_modified(double *array, const size_t length) ;
LinkedList *create_linked_list() ;
LinkedList *destroy_linked_list(LinkedList *list) ;
void add_to_linked_list(LinkedList *list, double value) ;
size_t length_of_(LinkedList *list) ;
void linked_list_as_array(LinkedList *list, double *array) ;
void bubblesort(double *array, size_t length) ;

int main() {
    srand((unsigned) time(NULL));
    const size_t size = 100;
    double array[size];
    for (size_t i = 0; i < size; ++i) {
        // Values must be < 1 and >= 0
        array[i] = rand() / ((double) RAND_MAX + 1);
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

int bucketsort_modified(double *array, const size_t length) {

    // Initialize buckets
    LinkedList *buckets[length];
    for (size_t i = 0; i < length; i++) {
        buckets[i] = create_linked_list();
        if (buckets[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                destroy_linked_list(buckets[j]);
            }
            return 1;
        }
    }

    // Put elements in buckets
    for (size_t i = 0; i < length; i++) {
        LinkedList *relevant_bucket = buckets[(size_t) (length * array[i])];
        add_to_linked_list(relevant_bucket, array[i]);
    }

    // Sort all buckets and insert them into the array
    size_t array_index = 0;
    size_t biggest_bucket = 0;
    for (size_t bucket_index = 0; bucket_index < length; ++bucket_index) {

        // Get bucket as array and sort it
        LinkedList *current_bucket = buckets[bucket_index];
        size_t bucket_size = length_of_(current_bucket);
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
        destroy_linked_list(current_bucket);
    }

    printf("Biggest bucket: %zu\n", biggest_bucket);
    return 0;
}

// UTILITY

LinkedListNode *create_linked_list_node(double value) {
    LinkedListNode *result = malloc(sizeof(LinkedListNode));
    if (result == NULL) {
        perror("Could not allocate memory!");
        return NULL;
    }

    result->value = value;
    result->next = NULL;

    return result;
}

LinkedListNode *destroy_nodes_recursively(LinkedListNode *node) {
    if (node) {
        node->next = destroy_nodes_recursively(node->next);
    }
    free(node);
    return NULL;
}

LinkedList *create_linked_list() {
    return calloc(1, sizeof(LinkedList));
}

LinkedList *destroy_linked_list(LinkedList *list) {
    if (list) {
        list->head = destroy_nodes_recursively(list->head);
    }
    free(list);
    return NULL;
}

size_t length_of_(LinkedList *list) {
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

int insert_at_linked_list(LinkedList *list, double value, size_t index) {

    // Catch index out of range
    if (index > length_of_(list)) {
        perror("List index out of range!");
        return 2;
    }

    LinkedListNode *head = list->head;
    LinkedListNode *to_insert = create_linked_list_node(value);
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

void linked_list_as_array(LinkedList *list, double *array) {

    LinkedListNode *node = list->head;

    for (size_t i = 0; i < length_of_(list); ++i) {
        array[i] = node->value;
        node = node->next;
    }
}

void add_to_linked_list(LinkedList *list, double value) {
    insert_at_linked_list(list, value, length_of_(list));
}

void print_linked_list(LinkedList *list) {
    LinkedListNode *head = list->head;

    while (head != NULL) {
        printf("%lf ", head->value);
        head = head->next;
    }

    printf("\n");
}

void swap(double *array, size_t a, size_t b) {
    double temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void bubblesort(double *array, size_t length) {
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
