#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#define N 100000000
#define HIST_SIZE 256

int main() {

    // create an array and initialize it with values in range [0, 255]
    int* arr = (int* )malloc(N * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        arr[i] = rand() % HIST_SIZE;
    }

    // 1. Naive parallel version (race condition expected)
    int hist_v1[HIST_SIZE] = {0};
    double start_time_v1, run_time_v1;
    start_time_v1 = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        hist_v1[arr[i]]++;
    }
    run_time_v1 = omp_get_wtime() - start_time_v1;

    // 2. Version with critical
    int hist_v2[HIST_SIZE] = {0};
    double start_time_v2, run_time_v2;
    start_time_v2 = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp critical
        hist_v2[arr[i]]++;
    }
    run_time_v2 = omp_get_wtime() - start_time_v2;

    // 3. Version using reduction
    int hist_v3[HIST_SIZE] = {0};
    double start_time_v3, run_time_v3;
    start_time_v3 = omp_get_wtime();
    #pragma omp parallel for reduction(+:hist_v3[:HIST_SIZE])
    for (int i = 0; i < N; i++) {
        hist_v3[arr[i]]++;
    }
    run_time_v3 = omp_get_wtime() - start_time_v3;

    printf("Naive parallel version processing time is %lf seconds\n", run_time_v1);
    printf("Version with critical processing time is %lf seconds\n", run_time_v2);
    printf("Version using reduction processing time is %lf seconds\n", run_time_v3);

    free(arr);
    return 0;
}
