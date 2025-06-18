/******************************************************************************
Creates N threads that print "Hello from thread X" n times,
where X is the thread ID.
Mutex is required if we want the same numbers consecutively. eg. 1122335544
Without mutex we might get 1123325445
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5
#define LIMIT 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* print_tid(void* args){
    
    int id = *(int*)args;
    
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < LIMIT; i++){
        printf("Hello from thread %d\n", id);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_t thread[NUM_THREADS];
    int ids[NUM_THREADS];
    
    for(int i = 0; i < NUM_THREADS; i++){
        ids[i] = i + 1;
        pthread_create(&thread[i], NULL, print_tid, &ids[i]);
    }
    
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(thread[i], NULL);
    }
    
    return 0;
    
}