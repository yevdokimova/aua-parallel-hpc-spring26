#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N_THREADS 4
#define N_DELIVERY 10000

typedef struct {
    int order_id;
    int distance_km;
    int priority;
} Order;

typedef enum {
    HIGH,
    NORMAL
} PriorityType;

int main() {

    int threshold;
    int high_priority_count = 0;
    // observed ~1.5x speedup with padding on this workload, but since the assignment requires thread_high_count[4] kept the solution without padding
    int thread_high_count[N_THREADS] = {0}; 
    double start_time, run_time;
    Order* orders = (Order* )malloc(N_DELIVERY * sizeof(Order));
    if (orders == NULL) {
        perror("malloc");
        return 1;
    }
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(N_THREADS)
    {   
        
        int id = omp_get_thread_num();
        unsigned int seed = id + time(NULL);
        int nthreads = omp_get_num_threads();

        // Step 1. initialize threshold
        #pragma omp single
        threshold = 20;

        // Step 2. initialize orders with proper order priority
        #pragma omp for
        for (int i = 0; i < N_DELIVERY; i++) {
            orders[i].order_id = i;
            orders[i].distance_km = rand_r(&seed) % 50;
            if (orders[i].distance_km < threshold) {
                orders[i].priority = HIGH;
            }
            else {
                orders[i].priority = NORMAL;
            }
        }
        #pragma omp barrier

        #pragma omp single
        printf("Assignment of order priority is completed!\n");

        // Step 3. count orders with high priority
        #pragma omp for
        for (int i = 0; i < N_DELIVERY; i++) {
            if (orders[i].priority == HIGH) {
                thread_high_count[id]++;
            }
        }

        #pragma omp barrier
        // Step 4. process the results 
        #pragma omp single
        {
            for (int i = 0; i < nthreads; i++) {
                printf("Thread %d counted %d orders with high priority\n", i,thread_high_count[i]);
                high_priority_count += thread_high_count[i];
            }
            printf("Total number of orders with high priority is %d\n", high_priority_count);
        }
    }
    run_time = omp_get_wtime() - start_time;
    printf("Processing time is %lf seconds\n", run_time);

    free(orders);
    return 0;
}