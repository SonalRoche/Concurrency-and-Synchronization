/******************************************************************************

Multi-threaded Producer-Consumer problem using mutex and condition variables 
with a bounded buffer.

*******************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFFER_SIZE];
int count = 0; // items in buffer
int in = 0, out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void* producer (void* args){
    int id = *(int*)args;
    for(int i = 0 ; i < PRODUCE_COUNT; i++){
        pthread_mutex_lock(&mutex);
        while(count == BUFFER_SIZE){
            pthread_cond_wait(&not_full, &mutex);
        }
        
        // Produce item
        int item  = id * 100 + i;
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        printf("Producer %d produced item %d\n", id, item);
        
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 10000); // Random sleep to simulate work
        }
        
            return NULL;
    }
    

void* consumer (void* args){
    int id  = *(int*) args;
    for(int i = 0 ; i < PRODUCE_COUNT; i++){
        pthread_mutex_lock(&mutex);
        while(count == 0){
            pthread_cond_wait(&not_empty, &mutex);
        }
        
        // Consume item
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        printf("Consumer %d consumed item %d\n", id, item);
        
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 150000); // Random sleep to simulate work
    }
    
    return NULL;
}

int main()
{
    pthread_t prod1, prod2, cons1, cons2;
    int id1 = 1, id2 = 2;
    
    pthread_create(&prod1, NULL, producer, &id1);
    pthread_create(&prod2, NULL, producer, &id2);
    pthread_create(&cons1, NULL, consumer, &id1);
    pthread_create(&cons2, NULL, consumer, &id2);
    
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);

    return 0;
}
