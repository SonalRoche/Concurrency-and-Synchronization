/******************************************************************************

This code creates N threads that alternately print their Thread ID
10 times each. The final output looks like: 012012012012 for N = 3.

*******************************************************************************/
#include <stdio.h>
#include <pthread.h>

#define LIMIT 10
#define NUMTHREADS 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 0;

void* print_num(void* args){
    int my_turn = *(int*)args;
    for(int i = 0; i < LIMIT; i++){
        pthread_mutex_lock(&mutex);
        while(my_turn != turn)
            pthread_cond_wait(&cond, &mutex);
        printf("%d", my_turn);
        turn = (my_turn + 1) % NUMTHREADS;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}


int main()
{
    pthread_t thread[NUMTHREADS];
    int ids[NUMTHREADS];
    
    for(int i = 0 ; i  < NUMTHREADS; i++){
        ids[i] = i;
        pthread_create(&thread[i], NULL, print_num, &ids[i]);
    }
    
    for(int i = 0 ; i  < NUMTHREADS; i++){
        pthread_join(thread[i], NULL);
    }

    return 0;
}
