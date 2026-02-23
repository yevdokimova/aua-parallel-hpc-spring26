# Homework 2: Threads Basics

Submit a PDF document that includes the following:
- Git Repository Link: Include the link to your Git repository containing all source code files at the top of the PDF.
- Program Outputs: Add clear, labeled screenshots showing the output of each program. Ensure each screenshot is associated with the corresponding exercise or program name for easy reference.


## Assignment 1: Basic Thread Creation

**Objective: Familiarize yourself with creating threads.**

_Task_:

1. Write a C program that creates three threads.
2. Each thread should print a message, including its thread ID, to indicate it is running (e.g., "Thread X is running").
3. Ensure the main thread waits for each of the threads to complete using `pthread_join`.

**Expected output:** Expected Output: Each thread prints a message, and the program exits only after all threads are complete.


## Assignment 2: Parallel Array Sum

**Objective: Create threads, split work across threads, and avoid shared writes.**

_Task_:

1. Write a C program that creates a large array of integers (e.g., size = 50 million).
2. Fill the array with random numbers.
3. Compute the total sum:
    - sequentially
    - using N threads (No global shared counter, No mutex, No atomic variables. Each thread works on a separate index range)
4. Each thread should:
    - Process a separate chunk of the array
    - Store its partial sum in a thread-local variable
    - Return the result to the main thread
5. Main threadshould:
    - Collect all partial sums
    - Compute the final result
    - Print execution time for both versions

**Expected output:** Compute and print the final result, and print execution time for both versions


## Assignment 3: Parallel Maximum Search

**Objective: Parallel reduction without synchronization.**

_Task_:

1. Write a C program that creates a large array of integers (e.g., size = 50 million).
2. Fill the array with random numbers.
3. Find the maximum value sequentially.
4. Find the maximum using **4 threads**.
5. Each thread:
    - Searches maximum in its own chunk.
    - Stores local max in a struct passed to the thread.
    - Returns result to main.
6. Main thread:
    - Finds the global maximum from 4 local maximums.

**Expected output:** Compute and print the final result and print execution time for both versions (sequential and threads).


## Assignment 4: Parallel Prime Counting

**Objective: Parallel CPU-bound workload.**

_Task_:

1. Count the number of prime numbers from 1 to 20 million.
2. Sequential version first.
3. Parallel version:
    - Divide the numeric range into equal intervals
    - Each thread counts primes in its interval
    - Store result locally
4. Combine in the main.

**Expected output:** Print the final result and execution time for both versions


## Assignment 5: CPU Core Exploration

**Objective: Understand scheduling behavior.**

_Task:_

1. Create N threads.
2. Each thread runs a heavy loop for several seconds. Just have some big number for iterations.
3. Each thread prints:
    - Thread ID
    - CPU core ID (`sched_getcpu()`)
4. Observe:
    - Do threads run on different CPUs?
    - Does OS migrate threads?
5. Try running with `taskset`.
    - taskset -c 0 ./my_program
    - If you are using your own laptop with multiple cores, you can also trydifferent combinations: 
        - taskset -c 0,1,2,3 ./my_program
        - taskset -c 0,2 ./my_program
        - taskset -c 0-3 ./my_program
        - taskset -c 0-2,5,7 ./my_program

**Expected output:** Print the thread IDs and CPU numbers.
