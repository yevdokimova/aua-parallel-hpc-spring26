#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 50000000
#define T_N 4
#define MIN 0
#define MAX 100

typedef struct {
	int start;
	int end;
	int* arr;
} Args;

int max = 0;

pthread_spinlock_t lock;


void* find_max(void* arg) {
	
	int maxl = 0;
	Args* args = (Args*) arg;
	for (int i = args -> start; i < args-> end; i++) {
		if (maxl < args->arr[i])
			maxl = args->arr[i];
	}

	pthread_spin_lock(&lock);
	max = maxl;
	pthread_spin_unlock(&lock);
	return NULL;
}

int main() {

	pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
	int* arr = (int*) malloc(N * sizeof(int));
	if (arr == NULL) {
		perror("malloc");
		return 1;
	}

	for (int i = 0; i < N; i++) {
		arr[i] = rand() % (MAX - MIN + 1);
	}

	int chunk = N /T_N;
	Args args[T_N];
	for (int i = 0; i < T_N; i++) {
		args[i].start = chunk * i;
		args[i].end = (i == T_N - 1) ? N : chunk * (i + 1);
		args[i].arr = arr;
	}
	pthread_t threads[T_N];

	for (int i = 0; i < T_N; i++) {
		if(pthread_create(&threads[i], NULL, find_max, &args[i]) != 0) {
			perror("create");
			free(arr);
			return 1;
		}
	}

	for (int i = 0; i < T_N; i++) {
		if(pthread_join(threads[i], NULL) != 0) {
			perror("join");
			free(arr);
			return 1;
		}
	}


	pthread_spin_destroy(&lock);
	printf("num is %d\n", max);
	return 0;
	

}