# Homework 6: OpenMP

Submit a report containing an explanation, output screenshots, and a github link.


## Assignment 1: Parallel Histogram with Reduction

**Problem**:
You are given an array of size **N = 100,000,000** with values in **range [0, 255]**.
Compute a **histogram**: hist[256] → number of occurrences of each value.**

_Steps_:

1. Initialize: `A[i] = rand() % 256;`
2. Implement in the same program:
    - Naive parallel version (race condition expected)
    - Version with critical
    - Version using reduction
_Hint_: OpenMP supports **array reduction (OpenMP 4.5+)** `#pragma omp parallel for reduction(+:hist[:256])`


## Assignment 2: Global Minimum Distance (Closest Pair)

**Problem**:
You are given an array of **N=50,000,00** loating-point values.
Compute the minimum absolute difference between any two consecutive elements: `min⁡ | A[i] − A[i−1] |`

_Steps_:

1. Initialize: `A[i] = random double`
2. Parallelize the loop
3. Use `reduction(min: min_diff)`


## Assignment 3: Parallel Filtered Sum (Top-K Style)

**Problem**:
Given an array `A[N]`, compute: sum of all elements where `A[i]>T`, where: `T=0.8 × max(A)`

_Steps_:

1. Find `max(A)` using reduction
2. Compute threshold T
3. Compute `sum of elements > T` using reduction
