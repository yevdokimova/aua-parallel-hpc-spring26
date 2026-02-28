#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SENSORS 6
#define MIN 20.0
#define MAX 25.5

double data[SENSORS];
pthread_barrier_t barrier;

void* collect_data(void* arg) {

    int id = (int)(long)arg;
    data[id] = MIN + ((double)rand() / RAND_MAX) * (MAX - MIN);
    printf("Sensor %d collected data: %.2f\n", id + 1, data[id]);
    pthread_barrier_wait(&barrier);
    return NULL;

}

int main() {

    srand(time(NULL));
    pthread_barrier_init(&barrier, NULL, SENSORS+1);
    pthread_t threads[SENSORS];

    for (int i = 0; i < SENSORS; i++) {
        if(pthread_create(&threads[i], NULL, collect_data, (void *)(long)i) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    pthread_barrier_wait(&barrier);
    int sum = 0;

    for (int i = 0; i < SENSORS; i++) {
        sum += data[i];
    }
    double average = (double)sum / SENSORS;
    printf("Average temperature from all %d sensors is %.2f Celsius\n", SENSORS, average);

    for (int i = 0; i < SENSORS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}