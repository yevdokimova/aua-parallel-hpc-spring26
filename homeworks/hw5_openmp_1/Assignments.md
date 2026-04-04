# Homework 5: Intro to OpenMP

Submit a report containing a simple explanation, output screenshots, and a github link.


## Assignment 1: Parallel Log Processing

**Objective: Implement a program that processes 20000 log entries in parallel.**

_Task_:

Each log entry contains:
- `request_id`
- `user_id`
- `response_time_ms`

Inside a parallel region with **4 threads**:
1. Use `single` to initialize all logs.
2. Use a `barrier` to ensure all threads wait until logs are fully initialized
3. Use for to process logs in parallel:
    - classify each log as:
        - FAST (<100 ms)
        - MEDIUM (100–300 ms)
        - SLOW (>300 ms)
4. Use another `barrier` to ensure all threads finish processing before the summary
5. Use `single` to compute and print:
    - number of FAST / MEDIUM / SLOW logs (sequentially, no reduction)


## Assignment 2: Delivery Priority Update

**Objective: Implement a program for 10000 delivery orders.**

_Task_:

Each order has:
- `order_id`
- `distance_km`
- `priority`

Inside a parallel region with **4 threads**:
1. Use `single` to set one common rule for all orders (for example, a distance threshold such as 20 km).
2. Use `for` to process orders in parallel:
    - if `distance_km < threshold` set priority to `HIGH`
    - otherwise set priority to `NORMAL`
3. Use a `barrier` to ensure all priorities are assigned
4. Use `single` to print a message that the priority assignment is finished
5. Use another `for` to process orders again in parallel:
    - count how many orders in each thread have `HIGH` priority
    - store the result in an array `thread_high_count[4]`
5. Use another `barrier` ensures all per-thread counts are ready
6. Use `single` to print the values from `thread_high_count[ ]` and the `total number of HIGH priority orders`