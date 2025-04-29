/******************************************************************************

This code creates 2 threads that alternately increment the count of a 
static variable 'c' 1000 times each. The final value of c should be 2000.

*******************************************************************************/
#include <stdio.h>
#include <pthread.h>

#define LIMIT 1000

static int c = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 0; // 0 for thread 1, 1 for thread 2

void* increment(void* args){
    int my_turn = *(int*)args;
    for (int i = 0; i < LIMIT; i++) {
        pthread_mutex_lock(&mutex);
        while (turn != my_turn)
            pthread_cond_wait(&cond, &mutex);
        c++;
        
        turn = 1 - my_turn; // Switch turn
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int id1 = 0, id2 = 1;
    
    pthread_create(&t1, NULL, increment, &id1);
    pthread_create(&t2, NULL, increment, &id2);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Final value of c = %d", c);

    return 0;
}
