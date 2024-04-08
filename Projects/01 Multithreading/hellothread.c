#include <stdio.h>
#include <pthread.h>

void *run_func(void *arg) {
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", (char*)arg, i);
    }
    return NULL;
}

int main() {
    pthread_t t1;
    pthread_t t2;

    printf("Launching threads\n");


    int status1 = pthread_create(&t1, NULL, run_func, "thread 1");
    if (status1 != 0) {printf("thread 1 error: %d", status1);}
    int status2 = pthread_create(&t2, NULL, run_func, "thread 2");
    if (status2 != 0) {printf("thread 1 error: %d", status2);}

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Threads complete!\n");
}