

#include <stddef.h>
#include <stdio.h>
#include <ncurses.h>


void print_array(unsigned *array, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%u", array[i]);
    }
}


void bubblesort(unsigned array[], int lower_bound, int upper_bound) {

    for (int i = upper_bound - 1; i >= lower_bound; --i) {
        for (int j = lower_bound; j <= i; ++j) {
            if (array[j] > array[j + 1]) {
                unsigned temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
             }
        }
    }

}


int partition(unsigned array[], unsigned lower_bound, unsigned upper_bound) {
    unsigned pivot, lb, ub, t;

    pivot = array[lower_bound];

    lb = lower_bound;
    ub = upper_bound + 1;

    while(TRUE) {
        do {++lb;}
        while (array[lb] <= pivot && lb <= upper_bound );

        do {--ub;}
        while (array[ub] > pivot );

        if( lb >= ub ) break;
        t = array[lb];
        array[lb] = array[ub];
        array[ub] = t;
    }

    t = array[lower_bound];
    array[lower_bound] = array[ub];
    array[ub] = t;
    return ub;
}

void quickSort(unsigned array[], int lower_bound, int upper_bound)
{
    int j;
    if( lower_bound < upper_bound ) {
        j = partition( array, lower_bound, upper_bound);
        quickSort( array, lower_bound, j-1);
        quickSort( array, j+1, upper_bound);
    }
}

int main() {
    unsigned array[5] = {2, 4, 1, 5, 3};
    //bubblesort(array, 0, 5);
    quickSort(array, 0, 5);
    print_array(array, 5);
}