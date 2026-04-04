#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 4
static long num_steps = 1000000000;

int main() {

    double start_time, run_time;
    double pi;
    double step = 1.0 / (double)num_steps;
    
    omp_set_num_threads(NUM_THREADS);  //requesting given number of threads, may receive <= to NUM_THREADS

	start_time = omp_get_wtime();
    #pragma omp parallel
    {
        int i, id, nthrds;
        double x;
        double sum;
        
        id = omp_get_thread_num();  // each thread gets its own ID (0, 1, 2, 3); thread 0 is the master thread
        nthrds = omp_get_num_threads(); // check how many threads were actually allocated and use that number
        
        for (i = id, sum = 0.0; i < num_steps; i += nthrds) {  // uses cyclic (round-robin) distribution, as opposed to block distribution
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }

        /*
        Equivalent to:
        #pragma omp critical
        pi += sum * step
        */
        sum = sum * step;
        #pragma omp atomic 
        pi += sum;
    }
    
	run_time = omp_get_wtime() - start_time;
	printf("OMP pi with %ld steps is %lf in %lf seconds\n", num_steps, pi, run_time);
    
    return 0;
}