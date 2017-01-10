#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

void priority_queue_insert(int *queue, const int value, size_t *size) ;
int priority_queue_remove_max(int *queue, size_t *size) ;
void priority_queue_increase(int *queue, size_t index, const int increment) ;
int verify(const int *heap, const size_t size);

int main() {
//    int priority_queue[10] = {2,1,3,5,4};
//    size_t size = 5;
//    build_max_heap(priority_queue, size);
//
//    assert(priority_queue_maximum(priority_queue) == 5);
//
//    assert(priority_queue_remove_max(priority_queue, &size) == 5);
//    assert(size == 4);
//
//
//    priority_queue_increase(priority_queue, 3, 4);
//    assert(priority_queue_maximum(priority_queue) == 5);
//
//
//    priority_queue_insert(priority_queue, 10, &size);
//    assert(priority_queue_maximum(priority_queue) == 10);
//    assert(size == 5);

    const size_t queue_limit = 200;
    int priority_queue[queue_limit];

    // fill queue with 100 random values between 1 and 100
    srand((unsigned) time(NULL));
    size_t queue_size = 0;
    for (size_t i = 0; i < 100; ++i) {
        priority_queue_insert(priority_queue, rand() % 100 + 1, &queue_size);
    }

    assert(verify(priority_queue, queue_size));

    // perform 100 random operations on the queue
    for (size_t j = 0; j < 100; ++j) {
        int random = rand() % 3;

        if (random == 0) {
            priority_queue_remove_max(priority_queue, &queue_size);
        } else if (random == 1) {
            priority_queue_increase(priority_queue, rand() % queue_size, 20);
        } else {
            priority_queue_insert(priority_queue, rand() % 100 + 1, &queue_size);
        }
        assert(verify(priority_queue, queue_size));
    }

    return 0;
}

// Assumes that queue is big enough to fit the additional value!
void priority_queue_insert(int *queue, const int value, size_t *size) {
    queue[*size] = value;
    // increase is used to align the new value properly in the queue
    priority_queue_increase(queue, *size, 0);
    ++*size;
}

int priority_queue_maximum(const int *queue) {
    return queue[0];
}

/// Swap the first with the last element and restored the heap on the reduced queue
/// This virtually truncates the queue by one, since size is decremented
/// \param queue The queue whose largest element is to remove
/// \param size A pointer to the queue's size
/// \return The maximum value
int priority_queue_remove_max(int *queue, size_t *size) {
    int max = queue[0];
    queue[0] = queue[*size - 1];
    --*size;
    max_heapify(queue, 0, *size);
    return max;
}

void priority_queue_increase(int *queue, size_t index, const int increment) {
    size_t parent_index = (size_t) ceil(index / 2.0) - 1;
    queue[index] += increment;
    while (index > 0 && queue[parent_index] < queue[index]) {
//        printf("%d (index %zu) here it seems my parent is %d (index %zu)\n", queue[index], index,
//               queue[parent_index], parent_index);
        swap(queue, index, parent_index);
        index = parent_index;
        parent_index = (size_t) ceil(index / 2.0) - 1;
    }

}

int verify(const int *heap, const size_t size) {
    // loop over all leafs
    for (size_t i = size / 2; i < size; ++i) {
        size_t index = i;
        size_t parent_index = (size_t) ceil(index / 2.0) - 1;
        while (index > 0) {
            //printf("%d (%zu) here my parent is %d (%zu) \n", heap[index], index, heap[parent_index], parent_index);

            if (heap[index] > heap[parent_index]) {
                return false;
            }

            index = parent_index;
            parent_index = (size_t) ceil(index / 2.0) - 1;
        }
    }
    return true;
}

void array_print_segment(int *array, size_t start, size_t end, char delimiter, size_t delimiter_count) {
    for (size_t i = start; i < end; ++i) {
        for (size_t j = 0; j < delimiter_count; ++j) {
            printf("%c", delimiter);
        }
        printf("%d", array[i]);
    }
    printf("\n");
}

void heap_print_internal(int *array, size_t size, size_t start, size_t end) {

    // calc indentation
    size_t levels = (size_t) ceil(log2(size));

    // print values
    // in the last recursion, just print up to size and return afterwards
    while (end < size) {

        array_print_segment(array, start, end, '\t', levels);
        --levels;

        // call print_heap with new start = old end and new end = twice the end index
        start = end;
        end = 2 * end + 1;
    }

    array_print_segment(array, start, size, '\t', levels);
}

void heap_print(int *array, size_t size) {
    heap_print_internal(array, size, 0, 1);
}

void swap(int *array, size_t a, size_t b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

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