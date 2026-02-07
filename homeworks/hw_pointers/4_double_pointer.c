#include <stdio.h>

int main() {

    int num = 42;
    int *ptr = &num;
    int **dptr = &ptr;

    printf("The value of the integer using variable: %d\n", num);
    printf("The value of the integer using pointer: %d\n", *ptr);
    printf("The value of the integer using double-pointer: %d\n", **dptr);

    return 0;
}