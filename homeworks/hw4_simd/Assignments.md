# Homework 4: SIMD and Multithreading

Submit source codes to git and a short report which includes:
- description of your implementations
- explanation of how the buffer was divided among threads
- explanation of SIMD processing
- performance measurements
- analysis of results
- screenshots of the outputs


## Task 1: DNA Nucleotide Statistics

### Objective

Practice:

- Data-level parallelism using SIMD
- Task-level parallelism using multithreading
- Combining SIMD and multithreading
- Parallel reduction / aggregation of partial results

You will implement several versions of a program that analyzes a large DNA sequence and counts how many times each nucleotide appears.

### Problem Description

In bioinformatics, one of the basic tasks is to compute simple statistics for a DNA sequence.

A DNA sequence consists of the following nucleotide symbols: `A`, `C`, `G`, `T`

Given a DNA sequence, write a program that counts how many times each of these symbols occurs. The final output must contain four integers representing the number of occurrences of: **A C G T**

**Example:**

```
Input:
AGCTTTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGTGGATTAAAAAAAGAGTGTCTGATAGCAGC

Output:
20 12 17 21
```

### DNA Buffer Preparation

- The buffer must contain only the characters: `A`, `C`, `G`, `T`
- The sequence size must be large enough for performance measurements (100 MB, 256 MB, 512 MB, …)
- You may prepare the DNA sequence by generating it randomly

### Implementations

#### Scalar Reference Version

- Process the DNA sequence one character at a time.
- Use this version as a reference implementation.
- Use it to verify correctness of the optimized versions.

#### Multithreading Implementation

- Use POSIX threads (`pthread`).
- Divide the DNA buffer into chunks.
- Each thread must process one chunk and compute in a global variable for each character.
- **You need to protect it using a mutex or spinlock!**

#### SIMD Implementation

- Use SIMD intrinsics available on your platform (SSE / AVX2 on x86, NEON on ARM).
- Process multiple characters in parallel.
- Compare vector elements against the four nucleotide symbols: `A`, `C`, `G`, `T`.
- Accumulate the counts correctly.
- Handle any remaining elements that do not fit into the SIMD vector width.

#### SIMD + Multithreading Implementation

- Divide the DNA buffer among multiple threads.
- Each thread must use SIMD to process its own chunk.
- Each thread must maintain local counts for: A, C, G, T.
- After all threads finish, merge the partial counts into final totals.

### Performance Measurement

Measure execution time for all four versions using a high-resolution timer such as `clock_gettime()`:

1. Scalar
2. Multithreading
3. SIMD
4. SIMD + Multithreading

### Program Output

Your program should print results in a format similar to:

```
DNA size: 256 MB
Threads used: 4

Counts (A C G T):
67108864 67108864 67108864 67108864

Scalar time:                0.220 sec
Multithreading time:        0.095 sec
SIMD time:                  0.080 sec
SIMD + Multithreading time: 0.028 sec
```

---

## Task 2: Character Buffer Processing

### Objective

Practice:

- Data-level parallelism using SIMD
- Task-level parallelism using multithreading
- Combining SIMD and multithreading for performance optimization

You will implement several versions of a program that processes a large buffer of characters and converts lowercase letters to uppercase.

### Problem Description

Given a large buffer of characters, write a program that scans the buffer and converts every lowercase English letter into its uppercase equivalent. Only characters in the range `a`–`z` must be converted. All other characters must remain unchanged.

**Example:**

```
Input:  "Hello world! a1b2"
Output: "HELLO WORLD! A1B2"
```

### Buffer Preparation

- Allocate a buffer of at least **200 MB**.
- Fill the buffer with characters that include: lowercase letters, uppercase letters, digits, punctuation, spaces.
- Create separate copies of the buffer so each implementation works on identical data.

### Implementations

#### Multithreading Implementation

- Use POSIX threads (`pthread`).
- Divide the buffer into equal-sized chunks.
- Each thread processes one chunk of the buffer.
- Each thread converts lowercase letters to uppercase in its region.

#### SIMD Implementation

- Use SIMD intrinsics available on your platform (SSE / AVX2 on x86, NEON on ARM).
- Process multiple characters per iteration using vector registers.
- Detect lowercase ASCII letters and convert them to uppercase.
- Handle remaining characters that do not fit into the SIMD vector width.

#### SIMD + Multithreading Implementation

- Divide the buffer among multiple threads.
- Each thread processes its chunk using SIMD operations.
- Ensure that thread boundaries are handled correctly.

### Performance Measurement

Measure the execution time of each implementation using a high-resolution timer.

### Program Output

Your program should print results similar to:

```
Buffer size: 256 MB
Threads used: 4

Multithreading time:      0.145 sec
SIMD time:                0.098 sec
SIMD + Multithreading:    0.041 sec
```

---

## Task 3: Grayscale Image Conversion

### Objective

Practice:

- Scalar image processing
- Data-level parallelism using SIMD
- Task-level parallelism using multithreading
- Combining SIMD and multithreading
- Performance comparison of different implementations

You will implement several versions of a program that converts a color image to grayscale.

### Problem Description

A color image contains pixels represented by three color channels: **R, G, B**. Your task is to convert a color image into a grayscale image.

For each pixel, compute the grayscale intensity using the formula:

```
gray = 0.299 * R + 0.587 * G + 0.114 * B
```

The resulting grayscale value must then be stored for all three channels of the output pixel: `(gray, gray, gray)` so that the output image is still a valid color image, but visually grayscale.

### Image Format

For simplicity, use the **PPM image format**. You may use P3 or P6 (P6 is recommended because it is simpler and more efficient for performance experiments).

References:

- https://paulbourke.net/dataformats/ppm/
- https://cs.berea.edu/courses/csc226/tasks/L3.ppm.html
- https://netpbm.sourceforge.net/doc/ppm.html
- https://en.wikipedia.org/wiki/Netpbm

Your program must be able to: 
- read the input PPM image,
- convert it to grayscale, 
- save the resulting grayscale image as another PPM image.

### Implementations

#### Scalar Implementation

- Process the image pixel by pixel.
- For each pixel: read R, G, B → compute grayscale intensity → write `(gray, gray, gray)` to the output buffer.
- Use this version as the reference implementation.

#### SIMD Implementation

- Use SIMD intrinsics available on your platform (SSE / AVX2 on x86, NEON on ARM).
- Process multiple pixels in parallel.
- Compute grayscale values using vector operations.
- Store the result back into the output buffer.
- Handle remaining pixels that do not fit into the SIMD vector width.

#### Multithreading Implementation

- Use POSIX threads (`pthread`).
- Divide the image into chunks.
- Each thread processes one part of the image using a scalar approach.
- Each thread writes results only into its assigned output region.

#### Multithreading + SIMD Implementation

- Divide the image among multiple threads.
- Each thread must process its own chunk using SIMD.
- Merge results implicitly by writing each chunk into the correct output region.

### PPM Image Reading and Writing

- Read the image header correctly: format (P3 or P6), width, height, maximum color value.
- Load the pixel data into memory.
- Store the image as a linear buffer.
- Save the processed result into an output PPM image.

### Performance Measurement

Measure execution time for all four versions using a high-resolution timer such as `clock_gettime()`:

1. Scalar
2. SIMD
3. Multithreading
4. Multithreading + SIMD

### Program Output

Your program should print results in a format similar to:

```
Image size: 3840 x 2160
Threads used: 4

Scalar time:                0.085 sec
SIMD time:                  0.041 sec
Multithreading time:        0.030 sec
Multithreading + SIMD time: 0.014 sec

Verification: PASSED
Output image: gray_output.ppm
```