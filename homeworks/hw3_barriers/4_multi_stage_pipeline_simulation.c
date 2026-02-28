#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define M 5

pthread_barrier_t barrier;

void* perform_exec(void* arg) {
    int id = (int)(long)arg;

    printf("Thread %d: Stage 1\n", id);
    sleep(1);
    pthread_barrier_wait(&barrier);

    printf("Thread %d: Stage 2\n", id);
    sleep(2);
    pthread_barrier_wait(&barrier);

    printf("Thread %d: Stage 3\n", id);
    sleep(3);
    pthread_barrier_wait(&barrier);
    
    return NULL;
}

int main() {
    pthread_t threads[M];
    pthread_barrier_init(&barrier, NULL, M);


     for (int i = 0; i < M; i++) {
        if(pthread_create(&threads[i], NULL, perform_exec, (void*)(long)i) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < M; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    pthread_barrier_destroy(&barrier);
    return 0;
}