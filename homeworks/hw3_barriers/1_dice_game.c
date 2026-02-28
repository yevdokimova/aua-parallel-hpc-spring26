#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define P 4
#define R 9

pthread_barrier_t barrier;
int scores[P][R];
int wins[P] = {0};

void* roll_dice(void* arg) {
    int player_index = (int)(long)arg;
    int max = 0;
    int num;

    for (int round = 0; round < R; round++)  {
        num = (rand() % 6 + 1);
        scores[player_index][round] = num;
        pthread_barrier_wait(&barrier);

        if(player_index == 0) {
            max = 0;
            for(int i = 0; i < P; i++) {
                if(scores[i][round] > scores[max][round]) {
                    max = i;  
                }
            }
            wins[max]++;
            for (int i = 0; i < P; i++) {
                printf("Round %d: Player %d rolled %d\n", round + 1, i + 1, scores[i][round]);
            }

            printf("Round %d winner: Player %d with score %d\n", round + 1, max + 1, scores[max][round]);
        }

        pthread_barrier_wait(&barrier);
    }
    return NULL;

}

int main() {
    srand(time(NULL));
    pthread_barrier_init(&barrier, NULL, P);
    pthread_t threads[P];

    for (int i = 0; i < P; i++) {
        if(pthread_create(&threads[i], NULL, roll_dice, (void*)(long)i) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < P; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    int max = 0;
    for (int i = 0; i < P; i++) {
        if(wins[i] > wins[max]) {
            max = i;
        }
    }

    printf("Player %d is the winner with the most wins of %d\n", max + 1, wins[max]);

    pthread_barrier_destroy(&barrier);

    return 0;

}