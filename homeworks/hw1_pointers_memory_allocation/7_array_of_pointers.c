#include <stdio.h>
#include <stdlib.h>

#define SIZE 3

int main() {

    char* arr[] = {"Hello", "world", "42"};

    printf("Initial string: \n");
    for (int i = 0; i < SIZE; i++) {
        printf("%s ", *(arr + i));
    }
    printf("\n");

    *(arr + 1) = "humanity";

    printf("Modified string: \n");
    for (int i = 0; i < SIZE; i++) {
        printf("%s ", *(arr + i));
    }
    printf("\n");

    return 0;
}