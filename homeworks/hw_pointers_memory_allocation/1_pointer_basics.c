#include <stdio.h>

int main() {

    int num = 42;
    int* ptr_num = &num;

    printf("Initial value of the variable is %d\n", num);

    printf("The address of the integer using variable is %p\n", &num);
    printf("The address of the integer using pointer is %p\n", ptr_num);

    *ptr_num = 7;

    printf("The modified value using variable is %d\n", num);
    printf("The modified value using pointer is %d\n", *ptr_num);


    return 0;
}
