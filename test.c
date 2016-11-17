#include <stdio.h>
#include "input.h"
#include "data_structures.h"

typedef struct my {
    size_t length;
    char copy;
    char name[5];
    int *array;
} MY;

MY *create_my(size_t length, char name[5]) {
    MY *result = malloc(sizeof(MY));
    result->length = length;
    result->copy = 'a';
    result->array = calloc(length, sizeof(int));
    memset(result->array, 42, length * sizeof(int));
    result->name[0] = 'h';
    return result;
}

int main() {
    //MY *my = create_my(5, "Mine!");
    int a = 10;
    char b = 'a';
    char string[] = "Hello";
    int array[] = {1,2,3};
    return 1;


}

