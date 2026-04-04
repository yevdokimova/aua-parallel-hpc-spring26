#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N_THREADS 4
#define N_LOGS 20000
#define PAD 16

typedef struct {
    long request_id;
    long user_id;
    double response_time_ms;
} Log;

typedef enum {
    FAST,
    MEDIUM,
    SLOW,
    N_LOG_TYPE
} LogType;


int main() {

    Log* logs = (Log* )malloc(N_LOGS * sizeof(Log));
    if(logs == NULL) {
        perror("malloc");
        return 1;
    }
    int log_count[N_THREADS][PAD] = {0};
    int log_count_results[N_LOG_TYPE] = {0};

    #pragma omp parallel num_threads(N_THREADS)
    {
        int id = omp_get_thread_num(); 

        // Step 1. Initialize logs
        #pragma omp single nowait
        for (int i = 0; i < N_LOGS; i++) {
                logs[i].request_id = i + 100;
                logs[i].user_id = i;
                logs[i].response_time_ms = (double)rand() / RAND_MAX * 500.0;
            }
        #pragma omp barrier //using barrier since it was requested by requirement, otherwise single with nowait has implicit barrier at the end

        // Step 2. Process logs in parallel with for
        #pragma omp for
        for(int i = 0; i < N_LOGS; i++) {
            if(logs[i].response_time_ms < 100) {
                log_count[id][FAST]++;
            }
            else if (logs[i].response_time_ms <= 300) {
                log_count[id][MEDIUM]++;
            }
            else {
                log_count[id][SLOW]++;
            }
        }

        #pragma omp barrier
        
        // Step 3. Produce final result
        #pragma omp single
        {   
            for (int i = 0; i < N_THREADS; i++) {
                log_count_results[FAST] += log_count[i][FAST];
                log_count_results[MEDIUM] += log_count[i][MEDIUM];
                log_count_results[SLOW] += log_count[i][SLOW];
            }
            printf("Results of logs distribution by types:\n");
            printf("FAST: %d\n", log_count_results[FAST]);
            printf("MEDIUM: %d\n", log_count_results[MEDIUM]);
            printf("SLOW: %d\n", log_count_results[SLOW]);
        } 

    }

    free(logs);
    return 0;
}