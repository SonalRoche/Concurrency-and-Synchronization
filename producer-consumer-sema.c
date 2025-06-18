/******************************************************************************
Basic Producer-Consumer problem with semaphores. (Bounded Buffer)
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

//Define parameters
#define BUFFER_SIZE 5
#define ITEMS 10
#define PRODUCERS 2
#define CONSUMERS 2

//Keep track of resources
int buffer[BUFFER_SIZE];
int in = 0; // next write idx
int out = 0; // next read idx

//Init mutex and 2 semaphores
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty; //tracks how many empty slots are in the buffer.
sem_t full; //tracks how many filled slots are in the buffer.

//Producer code
void* producer(void* args){
    
    int id = *(int*)args;
    
    for(int i = 0; i < ITEMS; i++){
        int item = id * 100 + i;
        
        sem_wait(&empty); // wait for empty slot
        pthread_mutex_lock(&mutex);
        
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        
        printf("Producer %d produced %d\n", id, item);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // signal item produced
        
        usleep(rand() % 100000); // simulate delay
    }
    
    return NULL;
    
}

//Consumer code
void* consumer(void* args){
    
    int id = *(int*)args;
    
    for(int i = 0; i < ITEMS; i++){
        
        sem_wait(&full); //wait for item
        pthread_mutex_lock(&mutex);
        
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        
        printf("Consumer %d consumed %d\n", id, item);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        
        usleep(rand() % 150000); // simulate delay
        
    }
    
    return NULL;
}


int main()
{
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int ids[PRODUCERS + CONSUMERS];
    
    //init semaphores
    sem_init(&empty, 0, BUFFER_SIZE); // BUFFER_SIZE empty slots initially
    sem_init(&full, 0, 0); // 0 full slots initially
    
    for (int i = 0; i < PRODUCERS; i++) {
        ids[i] = i;
        pthread_create(&prod[i], NULL, producer, &ids[i]);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        ids[PRODUCERS + i] = i;
        pthread_create(&cons[i], NULL, consumer, &ids[PRODUCERS + i]);
    }
    
    for (int i = 0; i < PRODUCERS; i++) {
        pthread_join(prod[i], NULL);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        pthread_join(cons[i], NULL);
    }
    
    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

	return 0;

}