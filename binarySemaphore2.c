/******************************************************************************
Binary semaphore - print thread ids N times (not in a pattern)
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define THREADS 3
#define REPEATS 4

sem_t bin_sem;

void* print_id(void* args){
    
    int id = *(int*)args;
    
    for(int i = 0; i < REPEATS; i++){
        
        sem_wait(&bin_sem);
        
        printf("Thread ID = %d\n", id);
        
        sem_post(&bin_sem);
        
        usleep(100000); // simulate some delay
    }
    
    return NULL;
    
}


int main()
{
    pthread_t threads[THREADS];
    int id[THREADS];
    
    sem_init(&bin_sem, 0, 1);
    
    for(int i = 0; i < THREADS; i++){
        id[i] = i;
        pthread_create(&threads[i], NULL, print_id, &id[i]);
    }
    
    for(int i = 0; i < THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&bin_sem);
    
	return 0;

}