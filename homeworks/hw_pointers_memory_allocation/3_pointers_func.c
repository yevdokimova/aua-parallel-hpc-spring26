#include <stdio.h>

void swap(int *a, int *b) {

    int temp;

    temp = *a;
    *a = *b;
    *b = temp;

    return;
}

int main() {

    int a = 10;
    int b = 20;

    printf("The values of the variables before modification are : %d and %d\n", a, b);

    swap(&a, &b);

    printf("The values of the variables after modification are : %d and %d\n", a, b);

    return 0;
}