#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <immintrin.h>   

pthread_mutex_t buffer1_lock;
pthread_mutex_t buffer2_lock;
bool isBuffer1Filled = false;
bool isBuffer2Filled = false;

typedef struct {
    float* buffer;
    pthread_mutex_t* mutex;
} ProduceData;

typedef struct {
    float* buffRead;
    float* buffWrite;
    pthread_mutex_t* mutexRead;
    pthread_mutex_t* mutexWrite;
} ConsumeData;

typedef struct {
    float* buffer;
    pthread_mutex_t* mutex;
} PrintData;

void* produce(void* arg) {

    ProduceData* data = (ProduceData*)arg;
    while(true) {
        pthread_mutex_lock(data->mutex);
        if (!isBuffer1Filled) {
            for (int i = 0; i < 8; ++i) {
                data->buffer[i] = i;
            }
            isBuffer1Filled = true;
        }
        pthread_mutex_unlock(data->mutex);
    }

    return NULL;
}


void* consume(void* arg) {

   ConsumeData* data = (ConsumeData*)arg;
    while (true) {
        pthread_mutex_lock(data->mutexRead);
        if (!isBuffer1Filled) {
            pthread_mutex_unlock(data->mutexRead);
            continue; 
        }
        __m256 va = _mm256_loadu_ps(data->buffRead);
        isBuffer1Filled = false;
        pthread_mutex_unlock(data->mutexRead);

        __m256 vresult = _mm256_add_ps(va, va);

        bool written = false;
        while (!written) {
            pthread_mutex_lock(data->mutexWrite);
            if (!isBuffer2Filled) {
                _mm256_storeu_ps(data->buffWrite, vresult);
                isBuffer2Filled = true;
                written = true;
            }
            pthread_mutex_unlock(data->mutexWrite);
        }
    }
    return NULL;
}


void* print(void* arg) {

    PrintData* data = (PrintData*)arg;
    while (true) {
        pthread_mutex_lock(data->mutex);
        if (isBuffer2Filled) {
            printf("Processed: ");
            for (int i = 0; i < 8; ++i) {
                printf("%.1f ", data->buffer[i]);
            }
            printf("\n");
            isBuffer2Filled = false;
        }
        pthread_mutex_unlock(data->mutex);
    }
    return NULL;

    return NULL;
}


int main() {

    float buffer1[8] = {0};
    float buffer2[8] = {0};

    pthread_mutex_init(&buffer1_lock, NULL);
    pthread_mutex_init(&buffer2_lock, NULL);

    pthread_t producer, consumer, printer;

    ProduceData pData  = { buffer1, &buffer1_lock };
    ConsumeData cData  = { buffer1, buffer2, &buffer1_lock, &buffer2_lock };
    PrintData   prData = { buffer2, &buffer2_lock };

    if(pthread_create(&producer, NULL, produce, &pData) != 0) {
        perror("pthread_create");
        return 1;
    }

    if(pthread_create(&consumer, NULL, consume, &cData) != 0) {
        perror("pthread_create");
        return 1;
    }

    if(pthread_create(&printer, NULL, print, &prData) != 0) {
        perror("pthread_create");
        return 1;
    }

    if(pthread_join(producer, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    if(pthread_join(consumer, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    if(pthread_join(printer, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    pthread_mutex_destroy(&buffer1_lock);
    pthread_mutex_destroy(&buffer2_lock);

    return 0;
}