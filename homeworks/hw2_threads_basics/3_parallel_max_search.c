#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARR_SIZE 50000000
#define T_NUM 4
#define MIN 0
#define MAX 1000

typedef struct {
    int* arr;
    int start;
    int end;
    int max;
} ArrArgs;


void* find_max(void* arg) {
    ArrArgs* arr_args = (ArrArgs*) arg;
    int max = 0;

    for (int i = arr_args -> start; i < arr_args -> end; i++) {
        if (arr_args -> arr[i] > max) {
            max = arr_args -> arr[i];
        }
    }

    arr_args -> max = max;

    return NULL;
}


int main() {

    srand(time(NULL));
    int max_seq = 0;
    int max_th = 0;

    // create an array
    int *arr = (int *)malloc(ARR_SIZE * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    // init array with random values
    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = (rand() % (MAX - MIN + 1)) + MIN;
    }

    // process max value sequantially and calculate elapsed time
    struct timespec start_seq, end_seq;
    double elapsed_time_seq;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_seq);
    for (int i = 0; i < ARR_SIZE; i++) {
        if (arr[i] > max_seq) {
            max_seq = arr[i];
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_seq);


    // process max value with threads and calculate elapsed time
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
        args[i].max = 0;
    }

    for (int i = 0; i < T_NUM; i++) {
        if(pthread_create(&threads[i], NULL, find_max, &args[i]) != 0){
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
            if (args[i].max > max_th) {
                max_th = args[i].max;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_th);


    // print results for both versions (sequential and threads)
    elapsed_time_seq = (end_seq.tv_sec - start_seq.tv_sec) * 1000.0 + (end_seq.tv_nsec - start_seq.tv_nsec) / 1e6;
    printf("Max calculated sequentially is %d and elapsed time is %f ms\n", max_seq, elapsed_time_seq);

    elapsed_time_th = (end_th.tv_sec - start_th.tv_sec) * 1000.0 + (end_th.tv_nsec - start_th.tv_nsec) / 1e6;
    printf("Max calculated with threads is %d and elapsed time is %f ms\n", max_th, elapsed_time_th);

    free(arr);
    arr = NULL;
    return 0;

}