#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 1000000
#define NUM_THREADS 4

pthread_mutex_t lock;
int arr[N];
int hist[10] = {0};


typedef struct {
	int start;
	int end;
} Args;

void* update_hist(void* arg) {
	
	Args* args = (Args*) arg;
	int hist_l[10] = {0};

	for (int i = args->start; i < args->end; i++) {
		int d = arr[i];
		hist_l[d]++;
	}

	pthread_mutex_lock(&lock);
	for (int i = 0; i < 10; i++) {
		hist[i] += hist_l[i];
	}
	pthread_mutex_unlock(&lock);

	return NULL;
}

int main() {

	for (int i = 0; i < N; i++) {
		arr[i] = i % 10; 
	}

	pthread_mutex_init(&lock, NULL);
	
	pthread_t threads[NUM_THREADS];
	Args thread_args[NUM_THREADS];
	int chunk = N / NUM_THREADS;

	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i].start = i * chunk;
		thread_args[i].end = (i == NUM_THREADS - 1) ? N : (i + 1) * chunk;
		//thread_args[i].end = i * chunk + chunk - 1;		
	}

	for (int i = 0; i < NUM_THREADS; i++ ) {
		if(pthread_create(&threads[i], NULL, update_hist, &thread_args[i]) != 0) {
			perror("pthread_create");
			return 1;
		}
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		if(pthread_join(threads[i], NULL) != 0) {
			perror("pthread_join");
			return 1;
		}
	}

	pthread_mutex_destroy(&lock);



	for (int i = 0; i < 10; i++) {
		printf("Hist[%d] = %d, expected (10000)\n", i, hist[i]);
	}
	

	return 0;
}