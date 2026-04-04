#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 4
static long num_steps = 1000000000;

int main() {

    double start_time, run_time;
    double pi = 0.0;
    double step = 1.0 / (double)num_steps;

	start_time = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        /*
        loop-carried dependence (read-after-write (RAW) also called true dependence), must be declared outside the loop;
        local to each thread;
        parallelization is still possible because addition is associative and commutative
        */
        double sum = 0.0;

        #pragma omp for  // runtime handles distribution automatically regardless of thread count
        for (int i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;  // not used across iterations, so it can be declared and destroyed each iteration
            sum += 4.0 / (1.0 + x * x);
        }

        sum *= step;
        #pragma omp atomic 
        pi += sum;
    }
    
	run_time = omp_get_wtime() - start_time;
	printf("OMP pi with %ld steps is %lf in %lf seconds\n", num_steps, pi, run_time);
    
    return 0;
}