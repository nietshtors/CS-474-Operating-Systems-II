#include <stdio.h>
#include <pthread.h>

#define THREADS 8

int arr[10000] = {0};
pthread_mutex_t lock;

void *run(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 10000; i++)
        arr[i]++;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void)
{
    pthread_t thread[THREADS];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(thread + i, NULL, run, NULL);
    }

    for (int i = 0; i < THREADS; i++)
        pthread_join(thread[i], NULL);

    int expected = THREADS;
    
    for (int i = 0; i < 10000; i++)
        if (arr[i] != expected)
            printf("arr[%d] = %d, expected %d\n", i, arr[i], expected);
    
    pthread_mutex_destroy(&lock);
}