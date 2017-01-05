#include "algorithms.h"
#include "data_structures.h"
#include <stdio.h>

/// Tests if the sorting algorithm works with arbitrary arrays, zero length arrays and already sorted ones.
/// \param sort The sorting algorithm
/// \return 1 in case of a mistake
int test_sorting(void (*sort)(int *, size_t)) {
    size_t size = 7;
    int ints_sorted[] = {-4, 0, 1, 4, 7, 12, 18};
    int ints[] = {4,7,18,-4,0,12,1};

    sort(ints, size);

    for (size_t i = 0; i < size; ++i) {
        if (ints[i] != ints_sorted[i]) {
            printf("Test failed!\n");
            print_array(ints, size, TYPE_INT);
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
            print_array(ints_sorted_again, size, TYPE_INT);
            return 1;
        }
    }

    printf("All tests successful!\n");

    return 0;
}

int main() {

    size_t size = 7;
    int ints[] = {-4, 0, 1, 4, 7, 12, 18};


    LinkedList *list = create_linked_list();

    print_linked_list(list);

    size_t length = length_of_(list);
    int array[length];
    print_array(array, length, TYPE_INT);
    linked_list_as_array(list, array);
    print_array(array, length, TYPE_INT);

    destroy_linked_list(list);

    //test_sorting(mergesort);

}

