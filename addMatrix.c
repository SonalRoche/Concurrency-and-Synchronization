/******************************************************************************

This code adds two 4x4 matrices using 4 threads.

*******************************************************************************/
#include <stdio.h>
#include <pthread.h>

#define CAPACITY 4

int matA[CAPACITY][CAPACITY] = {{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};;
int matB[CAPACITY][CAPACITY] = {{2,2,2,2},{1,1,1,1},{4,4,4,4},{3,3,3,3}};;
int result[CAPACITY][CAPACITY];

void* addMatrix(void* args){

    int row = *(int*)args;

    for(int i = 0; i < CAPACITY; i++){
        result[row][i] = matA[row][i] + matB[row][i];
    }

}

int main(){

    pthread_t t1, t2, t3, t4;
    int id0 = 0, id1 = 1, id2 = 2, id3 = 3;

    pthread_create(&t1, NULL, addMatrix, &id0);
    pthread_create(&t2, NULL, addMatrix, &id1);
    pthread_create(&t3, NULL, addMatrix, &id2);
    pthread_create(&t4, NULL, addMatrix, &id3);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    for(int i = 0; i < CAPACITY; i++){
        for(int j = 0; j < CAPACITY; j++){
            printf("%d ",result[i][j]);
        }
        printf("\n");
    }

    return 0;
} 