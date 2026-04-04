#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 4

static long num_steps = 1000000000;
double step;

int main() {

    double start_time, run_time;
    int nthreads;
    int i;
    double pi;
    double sum[NUM_THREADS][8];  //scalar expansion or scalar privatization via array expansion with padding to avoid false sharing
    
    step = 1.0 / (double)num_steps;
    
    omp_set_num_threads(NUM_THREADS);  //requesting given number of threads, may receive <= to NUM_THREADS

	start_time = omp_get_wtime();
    
    #pragma omp parallel
    {
        int i, id, nthrds;
        double x;
        
        id = omp_get_thread_num();  // each thread gets its own ID (0, 1, 2, 3); thread 0 is the master thread
        nthrds = omp_get_num_threads(); // check how many threads were actually allocated
        /*
        Ensure all threads have the same nthrds value
        Only thread 0 writes to the shared variable
        equivalent to #pragma omp single nowait, #pragma omp master or #pragma omp masked
        */
        if (id == 0)
            nthreads = nthrds;  // store in shared variable for use after the parallel region
        
        for (i = id, sum[id][0] = 0.0; i < num_steps; i += nthrds) {  // uses cyclic (round-robin) distribution, as opposed to block distribution
            x = (i + 0.5) * step;
            sum[id][0] += 4.0 / (1.0 + x * x);
        }
    }
    
    for (i = 0, pi = 0.0; i < nthreads; i++)
        pi += sum[i][0] * step;
    
	run_time = omp_get_wtime() - start_time;
	printf("OMP pi with %ld steps is %lf in %lf seconds\n", num_steps, pi, run_time);
    
    return 0;
}