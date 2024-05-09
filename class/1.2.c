#include <stdio.h>
#include <pthread.h>

#define THREADS 8

int arr[10000] = {0};


void *run(void *arg)
{
    (void)arg;

    for (int i = 0; i < 10000; i++)
        arr[i]++;
    
    return NULL;
}

int main(void)
{
    pthread_t thread[THREADS];

    for (int i = 0; i < THREADS; i++) {
        pthread_create(thread + i, NULL, run, NULL);
    }

    for (int i = 0; i < THREADS; i++)
        pthread_join(thread[i], NULL);

    int expected = THREADS;
    
    for (int i = 0; i < 10000; i++)
        if (arr[i] != expected)
            printf("arr[%d] = %d, expected %d\n", i, arr[i], expected);

}