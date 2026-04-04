#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define S 8


pthread_barrier_t barrier;
int data[S];
int ids[S];

void* collect_data(void* arg) {
	int id = *(int*) arg;

	data[id] = 10;
	pthread_barrier_wait(&barrier);

	return NULL;
}

int main() {

	pthread_barrier_init(&barrier, NULL, S);

	pthread_t threads[S];
	for (int i = 0; i < S; i++ ) {
		ids[i] = i;
		pthread_create(&threads[i], NULL, collect_data, &ids[i]);
	}
	//pthread_barrier_wait(&barrier); if barrier includes man thread as well as in homework solution

	for (int i = 0; i < S; i++) {
		pthread_join(threads[i], NULL);
	}

	int sum = 0;
        for (int i = 0; i < S; i++) {
                sum += data[i];
        }
	printf("Data is %d\n", sum);

	pthread_barrier_destroy(&barrier);

	return 0;
}