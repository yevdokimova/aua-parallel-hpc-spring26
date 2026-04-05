#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 100000000


int main() {

    // Step 0. create and initizalize array with random values
    srand(time(NULL));
    int* arr = (int* )malloc(N * sizeof(int));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100;
    }

    int maxv = 0;
    double threshold = 0;
    int sum = 0;

    #pragma omp parallel
    {   
        // Step 1. compute max
        #pragma omp for reduction(max:maxv)
        for (int i = 0; i < N; i++) {
            if (arr[i] > maxv)
                maxv = arr[i];
        }

        // Step 2. compute threshold
        #pragma omp single
        {
            threshold = 0.8 * maxv;
        }

        // Step 3. compute sum for elements > threshold
        #pragma omp for reduction(+:sum)
        for (int i = 0; i < N; i++) {
            if (arr[i] > threshold) {
                sum += arr[i];
            }
        }
    }

    printf("The threshold is %f\n", threshold);
    printf("The sum is %d\n", sum);

    free(arr);
    return 0;
}