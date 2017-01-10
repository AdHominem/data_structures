#include <stdlib.h>
#include <stdio.h>

typedef struct heap_node HeapNode;

struct heap_node {
    int value;
    HeapNode *left;
    HeapNode *right;
};

void print_array(int *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void swap(int *array, size_t a, size_t b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void max_heapify(int *array, size_t i, size_t heap_size) {
    size_t left = 2 * i;
    size_t right = 2 * i + 1;
    size_t max;
    if ((left <= heap_size) && (array[left] > array[right])) {
        max = left;
    } else {
        max = i;
    }
    if ((right <= heap_size) && (array[right] > array[max])) {
        max = right;
    }
    if (max != i) {
        swap(array, i, max);
        max_heapify(array, max, heap_size);
    }
}

void build_max_heap(int *array, size_t heap_size) {
    for (size_t i = (size_t) (heap_size / 2); i < heap_size / 2; --i) {
        max_heapify(array, i, heap_size);
    }
}

int main() {
    int array[] = {1,2,3,4,5,6,7,8};
    build_max_heap(array, 8);
    print_array(array, 8);
    return 0;
}

/*
 *
 * max-heapify (A,i,heap-groesse) {
l = lkind(i);
r = rkind(i);
if ((l <= heap-groesse) && (A[l] > A[i]))
max = l;
else max = i;
if { (r <= heap-groesse) && (A[r] > A[max])
max = r;
if ( max != i) {
vertausche A[i] und A[max];
max-heapify(A,max,heap-groesse);
}
}
 * */