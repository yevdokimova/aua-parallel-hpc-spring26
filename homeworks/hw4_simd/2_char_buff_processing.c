#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <arm_neon.h>
#include <string.h>

#define NUM_THREADS 4
#define BUF_SIZE (256 * 1024 * 1024)

typedef struct {
    char *start;
    size_t len;
} Args;


// Time calculation helper
double elapsed_ms(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 1000.0
         + (end->tv_nsec - start->tv_nsec) / 1e6;
}

// Buffer generator helper
void generate_buffer(char *buff, size_t len) {
    srand(time(NULL));
    for (size_t i = 0; i < len; i++)
        buff[i] = (rand() % 95) + 32;
}


// Scalar implementation
void to_upper_scalar(Args *arg) {
    for (size_t i = 0; i < arg->len; i++) {
        if (arg->start[i] >= 'a' && arg->start[i] <= 'z')
            arg->start[i] -= 32;
    }
}

// Multithreaded implementation
void *to_upper_mt(void *arg) {
    Args *args = (Args *)arg;
    to_upper_scalar(args);

    return NULL;
}

// SIMD implementation
void to_upper_simd(Args *arg) {
    uint8x16_t vA = vdupq_n_u8('a');
    uint8x16_t vZ = vdupq_n_u8('z');
    uint8x16_t vDiff = vdupq_n_u8(32);

    size_t i = 0;
    size_t block = arg->len - (arg->len % 16);

    for (i = 0; i < block; i += 16) {
        uint8x16_t data = vld1q_u8((uint8_t *)(arg->start + i));

        uint8x16_t ge_a = vcgeq_u8(data, vA);
        uint8x16_t le_z = vcleq_u8(data, vZ);
        uint8x16_t mask = vandq_u8(ge_a, le_z);

        uint8x16_t sub = vandq_u8(mask, vDiff);
        data = vsubq_u8(data, sub);

        vst1q_u8((uint8_t *)(arg->start + i), data);
    }

    if (i < arg->len) {
        Args tail;
        tail.start = arg->start + i;
        tail.len = arg->len - i;
        to_upper_scalar(&tail);
    }
}

// SIMD and Multithreaded implementation
void *to_upper_simd_mt(void *arg) {
    Args *args = (Args *)arg;
    to_upper_simd(args);

    return NULL;
}


int main()
{
    size_t buf_size = BUF_SIZE;

    char *original = (char *)malloc(buf_size * sizeof(char));
    if (original == NULL) {
        perror("malloc");
        return 1;
    }

    generate_buffer(original, buf_size);

    // create separate copies for each implementation
    char *buf_scalar = (char *)malloc(buf_size * sizeof(char));
    char *buf_mt = (char *)malloc(buf_size * sizeof(char));
    char *buf_simd = (char *)malloc(buf_size * sizeof(char));
    char *buf_simd_mt = (char *)malloc(buf_size * sizeof(char));

    if (!buf_scalar || !buf_mt || !buf_simd || !buf_simd_mt) {
        perror("malloc");
        return 1;
    }

    memcpy(buf_mt, original, buf_size);
    memcpy(buf_simd, original, buf_size);
    memcpy(buf_simd_mt, original, buf_size);

    int chunk = buf_size / NUM_THREADS;

    // multithreaded
    struct timespec start_mt, end_mt;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_mt);
    pthread_t threads[NUM_THREADS];
    Args args_mt[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args_mt[i].start = buf_mt + i * chunk;
        args_mt[i].len = (i == NUM_THREADS - 1) ? (buf_size - i * chunk) : chunk;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, to_upper_mt, &args_mt[i]) != 0) {
            perror("pthread_create");
            free(original);
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            free(original);
            return 1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_mt);

    // simd
    struct timespec start_simd, end_simd;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_simd);
    Args args_simd;
    args_simd.start = buf_simd;
    args_simd.len = buf_size;
    to_upper_simd(&args_simd);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_simd);

    // simd and multithreaded
    struct timespec start_simd_mt, end_simd_mt;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_simd_mt);
    pthread_t threads_simd[NUM_THREADS];
    Args args_simd_mt[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args_simd_mt[i].start = buf_simd_mt + i * chunk;
        args_simd_mt[i].len = (i == NUM_THREADS - 1) ? (buf_size - i * chunk) : chunk;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, to_upper_simd_mt, &args_simd_mt[i]) != 0) {
            perror("pthread_create");
            free(original);
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            free(original);
            return 1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_simd_mt);


    printf("Buffer size: %zu MB\n", buf_size / (1024 * 1024));
    printf("Threads used: %d\n\n", NUM_THREADS);

    printf("Multithreading time:        %.3f ms\n", elapsed_ms(&start_mt, &end_mt));
    printf("SIMD time:                  %.3f ms\n", elapsed_ms(&start_simd, &end_simd));
    printf("SIMD + Multithreading time: %.3f ms\n", elapsed_ms(&start_simd_mt, &end_simd_mt));

    free(original);
    free(buf_mt);
    free(buf_simd);
    free(buf_simd_mt);
    return 0;
}