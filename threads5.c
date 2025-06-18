/******************************************************************************
Write a thread-safe counter struct in C that supports increment(), decrement(),
and get() methods.Each function must use internal locking. Provide sample test
code with threads calling these functions.
*******************************************************************************/
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 6
#define LIMIT 100

typedef struct {
	int value;
	pthread_mutex_t lock;
} Counter;

void init(Counter* c) {
	c->value = 0;
	pthread_mutex_init(&c->lock, NULL);
}

void increment(Counter* c) {
	pthread_mutex_lock(&c->lock);
	c->value++;
	pthread_mutex_unlock(&c->lock);
}

void decrement(Counter* c){
    pthread_mutex_lock(&c->lock);
    c->value--;
    pthread_mutex_unlock(&c->lock);
}

int get(Counter* c){
    pthread_mutex_lock(&c->lock);
    int res = c->value;
    pthread_mutex_unlock(&c->lock);
    return res;
}

void* worker_inc(void* args){
    Counter* c = (Counter*)args;
    for(int i = 0; i < LIMIT; i++){
        increment(c);
    }
    return NULL;
}

void* worker_dec(void* args){
    Counter* c = (Counter*)args;
    for(int i = 0; i < LIMIT; i++){
        decrement(c);
    }
    return NULL;
}

int main()
{
    Counter c;
    init(&c);
    
    pthread_t threads[NUM_THREADS];
    
    for(int i = 0; i < NUM_THREADS/2; i++){
        pthread_create(&threads[i], NULL, worker_inc, &c);
    }
    
    for(int i = NUM_THREADS / 2; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, worker_dec, &c);
    }
    
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    
    printf("Final value of counter = %d\n", get(&c));

	return 0;

}