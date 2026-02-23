#include <pthread.h>
#include <stdio.h>

#define T_NUM 3

void* print_thread_id(void *arg) {
    pthread_t tid = pthread_self();
    printf("Thread %lu is running\n", (unsigned long)tid);

    return NULL;
}

int main() {

    pthread_t threads[T_NUM];

    for (int i = 0; i < T_NUM; i++) {
        if(pthread_create(&threads[i], NULL, print_thread_id, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < T_NUM; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    return 0;
}
