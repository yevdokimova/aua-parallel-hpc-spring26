#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARR_SIZE 100000000
#define NUM_THREADS 4

int main()
{
    int *arr = (int *)malloc(ARR_SIZE * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < ARR_SIZE; i++)
        arr[i] = i + 1;

    long partial_sums[NUM_THREADS];
    long total_sum = 0;

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int id = omp_get_thread_num();
        int chunk = ARR_SIZE / NUM_THREADS;
        int start_idx = id * chunk;
        int end_idx = (id == NUM_THREADS - 1) ? ARR_SIZE : (id + 1) * chunk;

        long local_sum = 0;
        for (int i = start_idx; i < end_idx; i++)
            local_sum += arr[i];

        partial_sums[id] = local_sum;
    }

    for (int i = 0; i < NUM_THREADS; i++)
        total_sum += partial_sums[i];

    printf("Array size: %d\n", ARR_SIZE);
    printf("Threads used: %d\n", NUM_THREADS);
    printf("Total sum: %ld\n", total_sum);

    free(arr);
    arr = NULL;
    return 0;
}