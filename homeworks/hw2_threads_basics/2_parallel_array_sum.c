#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define T_NUM 16
#define ARR_SIZE 50000000
#define MIN 0
#define MAX 100

typedef struct {
    int* arr;
    int  start;
    int  end;
    long long result;

} ArrArgs;


void* array_sum(void *arg) {
    ArrArgs* arr_args = (ArrArgs*) arg;
    long long sum = 0;

    for (int i = arr_args -> start; i < arr_args -> end; i++) {
        sum += arr_args -> arr[i];
    }

    arr_args -> result = sum;

    return NULL;
}

int main() {

    srand(time(NULL));
    long long sum_seq = 0;
    long long sum_th = 0;
    
    // create an array
    int *arr = (int *) malloc(ARR_SIZE * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    // initialize the array with random values
    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = (rand() % (MAX - MIN + 1)) + MIN;
    }

    // process sum sequantially and calculate elapsed time
    struct timespec start_seq, end_seq;
    double elapsed_time_seq;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_seq);
    for (int i = 0; i < ARR_SIZE; i++) {
        sum_seq += arr[i];
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_seq);
    

    // process sum with threads and calculate elapsed time
    struct timespec start_th, end_th;
    double elapsed_time_th;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_th);
    pthread_t threads[T_NUM];
    ArrArgs args[T_NUM];
    int chunk = ARR_SIZE / T_NUM;
    for (int i = 0; i < T_NUM; i++) {
        args[i].arr   = arr;
        args[i].start = i * chunk;
        args[i].end   = (i == T_NUM - 1) ? ARR_SIZE : (i + 1) * chunk;
        args[i].result = 0;
    }
    
    for (int i = 0; i < T_NUM; i++) {
        if(pthread_create(&threads[i], NULL, array_sum, &args[i]) != 0) {
            perror("pthread_create");
            free(arr);
            return 1;
        }
    }

    for (int i = 0; i < T_NUM; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            free(arr);
            return 1;
        }
        else {
            sum_th += args[i].result;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_th);
    
    // print results for both versions (sequential and threads)
    elapsed_time_seq = (end_seq.tv_sec - start_seq.tv_sec) * 1000.0 + (end_seq.tv_nsec - start_seq.tv_nsec) / 1e6;
    printf("Sum calculated sequentially is %lld and elapsed time is %f ms\n", sum_seq, elapsed_time_seq);

    elapsed_time_th = (end_th.tv_sec - start_th.tv_sec) * 1000.0 + (end_th.tv_nsec - start_th.tv_nsec) / 1e6;
    printf("Sum calculated with threads is %lld and elapsed time is %f ms\n", sum_th, elapsed_time_th);

    free (arr);
    arr = NULL;
    return 0;
    
}