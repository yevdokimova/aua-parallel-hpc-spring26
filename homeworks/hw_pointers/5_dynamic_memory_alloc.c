#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

int main() {

    int *ptr = (int *)malloc(sizeof(int));
    if (ptr == NULL) {
        perror("malloc");
        exit(1);
    }
    *ptr = 42;

    printf("The number is %d\n", *ptr);

    free(ptr);
    ptr = NULL;

    int* ptr_arr = (int *)malloc(SIZE * sizeof(int));
    if(ptr_arr == NULL) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        *(ptr_arr + i) = i;
    }

    printf("Values of the array after initialization:\n");
    for (int i = 0; i < SIZE; i ++) {
        printf("%d ", *(ptr_arr + i));
    }
    printf("\n");
    
    free(ptr_arr);
    ptr_arr = NULL;

    return 0;
}