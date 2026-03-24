#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <arm_neon.h>

#define NUM_THREADS 16 //since result of sysctl -a | grep machdep.cpu is machdep.cpu.thread_count: 16
#define DNA_SIZE (1024 * 1024 * 1024)

typedef struct {
    const char *start;
    size_t      len;
    long        countA;
    long        countC;
    long        countG;
    long        countT;
} Args;

long global_countA = 0;
long global_countC = 0;
long global_countG = 0;
long global_countT = 0;

pthread_mutex_t lock;


// Time calculation helper
double elapsed_ms(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 1000.0
         + (end->tv_nsec - start->tv_nsec) / 1e6;
}

// DNA sequence generator helper
void generate_dna(char *buff, size_t len) {
    char base[] = {'A', 'C', 'G', 'T'};
    srand(time(NULL));
    for (size_t i = 0; i < len; i++)
        buff[i] = base[rand() % 4];
}


// Scalar implementation
void count_scalar(Args *arg) {

    arg->countA = 0;
    arg->countC = 0;
    arg->countG = 0;
    arg->countT = 0;

     for (size_t i = 0; i < arg->len; i++) {
        if (arg->start[i] == 'A')
            arg->countA++;
        else if (arg->start[i] == 'C')
            arg->countC++;
        else if (arg->start[i] == 'G')
            arg->countG++;
        else if (arg->start[i] == 'T')
            arg->countT++;
    }
}

// Multithreaded implementation
void* count_mt(void* arg) {

    Args *args = (Args *)arg;

    count_scalar(args);

    pthread_mutex_lock(&lock);
    global_countA += args->countA;
    global_countC += args->countC;
    global_countG += args->countG;
    global_countT += args->countT;
    pthread_mutex_unlock(&lock);

    return NULL;
}

// SIMD implementation
void count_simd(Args *arg) {
    arg->countA = 0;
    arg->countC = 0;
    arg->countG = 0;
    arg->countT = 0;
 
    uint8x16_t vA = vdupq_n_u8('A');
    uint8x16_t vC = vdupq_n_u8('C');
    uint8x16_t vG = vdupq_n_u8('G');
    uint8x16_t vT = vdupq_n_u8('T');
 
    uint8x16_t sumA = vdupq_n_u8(0);
    uint8x16_t sumC = vdupq_n_u8(0);
    uint8x16_t sumG = vdupq_n_u8(0);
    uint8x16_t sumT = vdupq_n_u8(0);
 
    size_t i = 0;
    size_t block = arg->len - (arg->len % 16);
    int inner = 0;
 
    for (i = 0; i < block; i += 16) {
        uint8x16_t data = vld1q_u8((const uint8_t *)(arg->start + i));
 
        uint8x16_t cmpA = vceqq_u8(data, vA);
        uint8x16_t cmpC = vceqq_u8(data, vC);
        uint8x16_t cmpG = vceqq_u8(data, vG);
        uint8x16_t cmpT = vceqq_u8(data, vT);
 
        sumA = vsubq_u8(sumA, cmpA);
        sumC = vsubq_u8(sumC, cmpC);
        sumG = vsubq_u8(sumG, cmpG);
        sumT = vsubq_u8(sumT, cmpT);
 
        inner++;

        if (inner == 255) {
            arg->countA += vaddlvq_u8(sumA);
            arg->countC += vaddlvq_u8(sumC);
            arg->countG += vaddlvq_u8(sumG);
            arg->countT += vaddlvq_u8(sumT);
 
            sumA = vdupq_n_u8(0);
            sumC = vdupq_n_u8(0);
            sumG = vdupq_n_u8(0);
            sumT = vdupq_n_u8(0);
            inner = 0;
        }
    }
 
    if (inner > 0) {
        arg->countA += vaddlvq_u8(sumA);
        arg->countC += vaddlvq_u8(sumC);
        arg->countG += vaddlvq_u8(sumG);
        arg->countT += vaddlvq_u8(sumT);
    }

    if (i < arg->len) {
        Args tail;
        tail.start = arg->start + i;
        tail.len   = arg->len - i;
        count_scalar(&tail);
        arg->countA += tail.countA;
        arg->countC += tail.countC;
        arg->countG += tail.countG;
        arg->countT += tail.countT;
    }
}

// SIMD and Multithreaded implementation
void *count_simd_mt(void *arg) {
    Args *args = (Args *)arg;

    count_simd(args);

    pthread_mutex_lock(&lock);
    global_countA += args->countA;
    global_countC += args->countC;
    global_countG += args->countG;
    global_countT += args->countT;
    pthread_mutex_unlock(&lock);

    return NULL;
}


int main() {

    size_t dna_size = DNA_SIZE;

    char *buff = (char *)malloc(dna_size * sizeof(char));
    if (buff == NULL) {
        perror("malloc");
        return 1;
    }

    generate_dna(buff, dna_size);

    // scalar
    struct timespec start_scalar, end_scalar;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_scalar);
    Args args_scalar;
    args_scalar.start = buff;
    args_scalar.len = dna_size;
    count_scalar(&args_scalar);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_scalar);

    // multithreaded
    struct timespec start_mt, end_mt;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_mt);
    pthread_mutex_init(&lock, NULL);
    pthread_t threads[NUM_THREADS];
    Args args_mt[NUM_THREADS];
    int chunk = dna_size / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        args_mt[i].start = buff + i * chunk;
        args_mt[i].len   = (i == NUM_THREADS - 1) ? (dna_size - i * chunk) : chunk;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, count_mt, &args_mt[i]) != 0) {
            perror("pthread_create");
            free(buff);
            return 1;
        }
    }
 
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            free(buff);
            return 1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_mt);

    // simd
    struct timespec start_simd, end_simd;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_simd);
    Args args_simd;
    args_simd.start = buff;
    args_simd.len   = dna_size;
    count_simd(&args_simd);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_simd);

    // simd and multithreaded
    global_countA = 0;
    global_countC = 0;
    global_countG = 0;
    global_countT = 0;
    struct timespec start_simd_mt, end_simd_mt;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_simd_mt);
    pthread_t threads_simd[NUM_THREADS];
    Args args_simd_mt[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args_simd_mt[i].start = buff + i * chunk;
        args_simd_mt[i].len   = (i == NUM_THREADS - 1) ? (dna_size - i * chunk) : chunk;
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, count_simd_mt, &args_simd_mt[i]) != 0) {
            perror("pthread_create");
            free(buff);
            return 1;
        }
    }
 
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            free(buff);
            return 1;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_simd_mt);


    printf("DNA size: %zu MB\n", dna_size / (1024 * 1024));
    printf("Threads used: %d\n\n", NUM_THREADS);
    printf("Counts (A C G T):\n");
    printf("%ld %ld %ld %ld\n\n", args_scalar.countA, args_scalar.countC, args_scalar.countG, args_scalar.countT);
    
    printf("Scalar time:                %.3f ms\n", elapsed_ms(&start_scalar, &end_scalar));
    printf("Multithreading time:        %.3f ms\n", elapsed_ms(&start_mt, &end_mt));
    printf("SIMD time:                  %.3f ms\n", elapsed_ms(&start_simd, &end_simd));
    printf("SIMD + Multithreading time: %.3f ms\n", elapsed_ms(&start_simd_mt, &end_simd_mt));
 

    pthread_mutex_destroy(&lock);
    free(buff);
    buff = NULL;
    return 0;
}
