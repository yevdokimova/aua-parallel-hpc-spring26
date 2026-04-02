#include <stdio.h>
#include <stdlib.h>


int str_length(char *str) {

    int len = 0;

    while(*str != '\0') {
        len++;
        str++;
    }

    return len;
}

int main() {

    char *ptr = "Hello World";
    char *str = ptr;

    while(*str != '\0') {
        printf("%c", *str);
        str++;

    }
    printf("\n");
    int len = str_length(ptr);
    printf("The lenght of the string is %d\n", len);

    return 0;
}