/******************************************************************************
Creates M threads (M < 26), each responsible for printing one line of a repeating pattern:
Example: ABCDEABCDEABCDE for M = 5 and N = 3
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5
#define LIMIT 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 0;

void* print_pattern(void* args){
    
    int myturn = *(int*)args;
    
    for(int i = 0; i < LIMIT; i++){
        pthread_mutex_lock(&mutex);
        while(turn != myturn){
            pthread_cond_wait(&cond, &mutex);
        }
        printf(" %c ", 65 + myturn);
        turn = (myturn + 1) % NUM_THREADS;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t thread[NUM_THREADS];
    int ids[NUM_THREADS];
    
    for(int i = 0; i < NUM_THREADS; i++){
        ids[i] = i;
        pthread_create(&thread[i], NULL, print_pattern, &ids[i]);
    }
    
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(thread[i], NULL);
    }
    
    return 0;
    
}