#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void* print_nos(void* args) {
    for(int i = 0; i <= 5; i++) {
        printf("%d ", i);
        sleep(1);
    }
}

void* print_letters(void* args) {
    char ch = 'A';
    for(int i = 0; i <= 5; i++) {
        printf("%c ", ch+i);
        sleep(1);
    }
}

int main() {

    pthread_t t1, t2;

    pthread_create(&t1, NULL, print_nos, NULL);
    pthread_create(&t2, NULL, print_letters, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    

    return 0;
}

