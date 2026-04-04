#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 4
static long num_steps = 1000000000;

int main() {

    double start_time, run_time;
    double pi = 0.0;
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;

	start_time = omp_get_wtime();

    /*
    same as
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp for reduction(+:sum)
        for (int i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    }
    */

    #pragma omp parallel num_threads(NUM_THREADS)
    #pragma omp for reduction(+:sum)
    for (int i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    
    pi = sum * step;
	run_time = omp_get_wtime() - start_time;
	printf("OMP pi with %ld steps is %lf in %lf seconds\n", num_steps, pi, run_time);
    
    return 0;
}