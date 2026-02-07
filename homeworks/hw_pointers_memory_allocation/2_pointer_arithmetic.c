#include <stdio.h>

#define SIZE 5

int main() {

    int arr[] = {1, 2, 3, 4, 5};

    int* ptr = arr;

    printf("The values of the array:\n");
    
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        *(ptr + i) = *(ptr + i) + i * 2;
    }

    printf("The values of the modified array using pointer:\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", *(ptr + i));
    }
    printf("\n");

    printf("The values of the modified array using the array name:\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}