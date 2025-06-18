#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int count = 0;
sem_t bin_sem;

void* counter(void* args) {
    
    for (int i = 0; i < 100; i++) {
        sem_wait(&bin_sem);
        count++;
        sem_post(&bin_sem);
    }
    
    return NULL;

}

int main() {
    
    pthread_t t1, t2;
    
    sem_init(&bin_sem, 0, 1);
    
    pthread_create(&t1, NULL, counter, NULL);
    pthread_create(&t2, NULL, counter, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Final counter value: %d\n", count);

    sem_destroy(&bin_sem);
    return 0;

}

