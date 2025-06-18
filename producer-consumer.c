/******************************************************************************
Basic Producer-Consumer problem. (Bounded buffer)
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

//Define parameters
#define BUFFER_SIZE 5
#define ITEMS 10
#define PRODUCERS 2
#define CONSUMERS 2

//Keep track of resources
int buffer[BUFFER_SIZE];
int count = 0; // number of items in buffer
int in = 0; // next write index
int out = 0; // next read index

//Init mutex and 2 cv
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

//Producer code
void* producer(void* args){
    
    int id = *(int*)args;
    
    for(int i = 0; i < ITEMS; i++){
        int item = id * 100 + i;
        pthread_mutex_lock(&mutex);
        
        //wait while the buffer is full
        while(count == BUFFER_SIZE){
            pthread_cond_wait(&notFull, &mutex);
        }
        
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        printf("Producer %d produced %d\n", id, item);
        
        pthread_cond_signal(&notEmpty);
        pthread_mutex_unlock(&mutex);
        
        usleep(rand() % 100000);  // Simulate work
    }
    
    return NULL;
    
}

//Consumer code
void* consumer(void* args){
    
    int id = *(int*)args;
    
    for(int i = 0 ; i < ITEMS; i++){
        pthread_mutex_lock(&mutex);
        
        // wait while buffer is empty
        while(count == 0){
            pthread_cond_wait(&notEmpty, &mutex);
        }
        
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        printf("Consumer %d consumed %d\n", id, item);
        
        pthread_cond_signal(&notFull);
        pthread_mutex_unlock(&mutex);
        
        usleep(rand() % 150000);  // Simulate work
    }
    
    return NULL;
}


int main()
{
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int ids[PRODUCERS + CONSUMERS];
    
    for(int i = 0; i < PRODUCERS; i++){
        ids[i] = i;
        pthread_create(&prod[i], NULL, producer, &ids[i]);
    }
    
    for(int i = 0; i < CONSUMERS; i++){
        ids[PRODUCERS + i] = i;
        pthread_create(&cons[i], NULL, consumer, &ids[PRODUCERS + i]);
    }
    
    for(int i = 0; i < PRODUCERS; i++){
        pthread_join(prod[i], NULL);
    }
    
    for(int i = 0; i < CONSUMERS; i++){
        pthread_join(cons[i], NULL);
    }

	return 0;

}