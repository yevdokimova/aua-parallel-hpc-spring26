#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define T_NUM 4
#define SIZE 50000000
#define MIN 0
#define MAX 1000

typedef struct {
	int* arr;
	int start;
	int end;
} Args;

long long sum = 0;
pthread_mutex_t lock;

void* calculate_sum(void* arg) {
	
	Args* args = (Args*) arg;

	int lsum = 0;
	for(int i = args -> start; i < args-> end; i++) {
		lsum += args->arr[i];	
	}

	pthread_mutex_lock(&lock);
	sum += lsum;
	pthread_mutex_unlock(&lock);

	return NULL;
}


int main() {

	int *arr = (int *) malloc(SIZE * sizeof(int));
	if(arr == NULL) {
		perror("malloc");
		return 1;
	}

	for (int i = 0; i < SIZE; i++) {
		arr[i] = rand() % (MAX - MIN + 1);
	}

	pthread_t threads[T_NUM];
	Args args[T_NUM];
	int chunk = SIZE / T_NUM;
	pthread_mutex_init(&lock, NULL);

	for (int i = 0; i < T_NUM; i++) {
		args[i].start = i + chunk;
		args[i].end = (i == T_NUM - 1) ? SIZE : (i + 1) * chunk;
		args[i].arr = arr;
	}

	for (int i = 0; i < T_NUM; i++) {
		if(pthread_create(&threads[i], NULL, calculate_sum, &args[i]) != 0 ) {
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
	}

        printf("num is %lld\n", sum);	
	pthread_mutex_destroy(&lock);

	free(arr);
	return 0;
}