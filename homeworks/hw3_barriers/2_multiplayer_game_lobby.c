#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 8

pthread_barrier_t barrier;

void* wait_at_barrier(void* arg) {

    pthread_t tid = pthread_self();

    printf("Thread %lu is getting ready...\n", (unsigned long)tid);
    sleep(7);
    pthread_barrier_wait(&barrier);
    return NULL;

}

int main() {

    pthread_barrier_init(&barrier, NULL, N+1);
    pthread_t threads[N];

    for (int i = 0; i < N; i++) {
        if(pthread_create(&threads[i], NULL, wait_at_barrier, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    pthread_barrier_wait(&barrier);
    printf("Game Started!\n");

     for (int i = 0; i < N; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}