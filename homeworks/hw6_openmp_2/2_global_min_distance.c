#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define N 50000000
#define MAX 1000.0

int main() {

    double minv = MAX;

    // create and initizalize array with random values
    srand(time(NULL));
    double* arr = (double* )malloc(N * sizeof(double));
    if (arr == NULL) {
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < N; i++) {
        arr[i] = (double)rand() / RAND_MAX * MAX;
    }

    // process min distance with reduction
    #pragma omp parallel for reduction(min:minv)
    for (int i = 1; i < N; i++) {
        double diff = fabs(arr[i] - arr[i - 1]);
        if (diff < minv) {
            minv = diff;
        }
    }

    printf("The minimum value is %f\n", minv);
    free(arr);
    return 0;
}