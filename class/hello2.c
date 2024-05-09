#include <stdio.h>
#include <pthread.h>

void *run(void *arg)
{
    (void)arg;
    puts("child: hello");
    puts("child: goodbye");

    return NULL;
}

int main(void)
{
    pthread_t thread;
    int status;

    puts("parent: hello");

    status = pthread_create(&thread, NULL, run, NULL);

    if (status == -1)
        perror("pthread_create");

    status = pthread_join(thread, NULL);

    if (status == -1)
        perror("pthread_join");

    puts("parent: goodbye");
}
