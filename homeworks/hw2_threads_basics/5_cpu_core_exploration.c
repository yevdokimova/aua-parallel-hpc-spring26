#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>

#define T_NUM 4

void* cpu_explore(void* arg) {

    volatile long long sum = 0;
    pthread_t tid = pthread_self();
    int init_cpu = sched_getcpu();
    printf("Thread %lu is running on CPU ID %d \n", (unsigned long)tid, init_cpu);

    for (long long i = 0; i < 1000000000; i++) {
        sum += i;
        int cpu = sched_getcpu();

        if (cpu != init_cpu) {
            printf("Thread %lu moved to CPU %d\n", (unsigned long)tid, cpu);
            init_cpu = cpu;
        }
    }
    return NULL;
}

int main() {

    struct timespec start, end;
    double elapsed_time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    pthread_t threads[T_NUM];
    for(int i = 0; i < T_NUM; i++) {
        if(pthread_create(&threads[i], NULL, cpu_explore, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for(int i = 0; i < T_NUM; i++) {
        if(pthread_join(threads[i], NULL) != 0 ){
            perror("pthread_join");
            return 1;
        } 
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1e6;
    printf("Elapsed time is %f ms\n", elapsed_time);

    return 0;
}