# Pointers and Memory Allocation

Push your source code to GitHub and submit a report with an analysis of each program. The report should include explanations of the code, how it works, and screenshots of the output.

## Assignment 1: Basics of Pointers

**Objective: Understand the basics of pointers, the address-of operator(&), and dereferencing(*).**

_Task_:

1. Declare an integer variable and initialize it with a value.
2. Declare a pointer variable that points to the integer.
3. Print the address of the integer variable using both the variable and the pointer.
4. Modify the value of the integer using the pointer and print the new value.


## Assignment 2: Pointer Arithmetic

**Objective: Learn how pointer arithmetic works.**

_Task_:

1. Declare an array of integers and initialize it with 5 values.
2. Use a pointer to traverse the array and print each element.
3. Modify the values of the array using pointer arithmetic.
4. Print the modified array using both the pointer and the array name.


## Assignment 3: Pointers and Functions

**Objective: Learn how to pass pointers to functions.**

_Task_:

1. Write a function `swap(int *a, int *b)` that swaps two integer values using pointers.
2. In the `main()` function, call `swap()` and pass the addresses of two integers.
3. Print the values of the integers before and after the swap.


## Assignment 4: Pointers to Pointers

**Objective: Work with double pointers.**

_Task_:

1. Declare an integer variable and a pointer to that variable.
2. Declare a pointer to the pointer and initialize it.
3. Print the value of the integer using both the pointer and the double-pointer.


## Assignment 5: Dynamic Memory Allocation with Pointers

**Objective: Learn how to allocate and free memory dynamically using pointers.**

_Task:_

1. Use `malloc()` to allocate memory for an integer.
2. Assign a value to the allocated memory and print it.
3. Use `malloc()` to allocate memory for an array of 5 integers.
4. Populate the array using pointer arithmetic and print the values.
5. Free all allocated memory.


## Assignment 6: String Manipulation with Pointers

**Objective: Work with strings using pointers.**

_Task:_

1. Declare a character pointer and assign it a string literal.
2. Use a pointer to traverse and print the string character by character.
3. Write a function `str_length(char *str)` that calculates the length of a string using pointer arithmetic.
4. Call `str_length()` in `main()` and print the length of a user-provided string.


## Assignment 7: Array of Pointers

**Objective: Work with an array of pointers.**

_Task:_

1. Declare an array of strings using an array of character pointers.
2. Print each string using pointer notation.
3. Modify one of the strings and print the updated array.