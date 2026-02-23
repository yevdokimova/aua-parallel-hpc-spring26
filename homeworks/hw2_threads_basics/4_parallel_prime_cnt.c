#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define T_NUM 8
#define N 20000000

typedef struct {
    int start;
    int end;
    long long prime_cnt;
} ArrArgs;


void* count_prime(void* arg) {

    ArrArgs* args = (ArrArgs*)arg;
    long long prime_cnt = 0;

    for (int i = args->start; i <= args->end; i++) {
        if (i < 2) continue;
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        } 

        if (is_prime)
            prime_cnt++;
    }

    args->prime_cnt = prime_cnt;

    return NULL;
}

int main() {

    long long prime_seq = 0;
    long long prime_th = 0;

    // process prime counting sequentially and calculate elapsed time
    struct timespec start_seq, end_seq;
    double elapsed_time_seq;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_seq);
    for (int i = 1; i <= N; i++) {
        if (i < 2) continue;
        int is_prime = 1;

        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime)
            prime_seq++;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_seq);


    // process prime counting with threads and calculate elapsed time
    struct timespec start_th, end_th;
    double elapsed_time_th;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_th);
    pthread_t threads[T_NUM];
    ArrArgs args[T_NUM];
    int chunk = N / T_NUM;
    for (int i = 0; i < T_NUM; i++) {
        args[i].start = i * chunk + 1;
        args[i].end   = (i == T_NUM - 1) ? N : (i + 1) * chunk;
        args[i].prime_cnt = 0;
    }

    for (int i = 0; i < T_NUM; i++) {
        if(pthread_create(&threads[i], NULL, count_prime, &args[i]) != 0){
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < T_NUM; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
        else {
            prime_th += args[i].prime_cnt;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_th);


    // print results for both versions (sequential and threads)
    elapsed_time_seq = (end_seq.tv_sec - start_seq.tv_sec) * 1000.0 + (end_seq.tv_nsec - start_seq.tv_nsec) / 1e6;
    printf("Prime count calculated sequentially is %lld and elapsed time is %f ms\n", prime_seq, elapsed_time_seq);

    elapsed_time_th = (end_th.tv_sec - start_th.tv_sec) * 1000.0 + (end_th.tv_nsec - start_th.tv_nsec) / 1e6;
    printf("Prime count calculated with %d threads is %lld and elapsed time is %f ms\n", T_NUM, prime_th, elapsed_time_th);

    return 0;

}