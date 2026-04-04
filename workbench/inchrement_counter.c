#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define T_NUM 4
#define M 10000

long long counter = 0;
pthread_spinlock_t lock;


void* increment_counter(void* arg) {

	for (int i = 0;  i < M; i++) {
		pthread_spin_lock(&lock);
		counter += 1;
		pthread_spin_unlock(&lock);
	}
	return NULL;
}

int main() {

	pthread_t threads[T_NUM];
	pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
	
	for (int i = 0; i < T_NUM; i++) {
		if(pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
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

	pthread_spin_destroy(&lock);

	printf("Initial num is %d\n",  T_NUM * M);
	printf("Result num is %lld\n", counter);

	return 0;
}
