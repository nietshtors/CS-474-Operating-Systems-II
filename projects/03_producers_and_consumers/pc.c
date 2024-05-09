#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "eventbuf.h"


// Global variable initialization
int num_producers;
int num_consumers;
int num_events;
int num_outstanding;

// Initialize global event buffer
struct eventbuf *eb;

// Initialize semaphores
sem_t *mutex;
sem_t *items;
sem_t *spaces;


sem_t *sem_open_temp(const char *name, int value) {
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}


void *producer_run(void *arg) {
    int *id = arg;

    for (int i = 0; i < num_events; i++) {
        int event = *id * 100 + i;
        sem_wait(spaces);
        sem_wait(mutex);
        printf("P%d: adding event %d\n", *id, event);
        eventbuf_add(eb, event);
        sem_post(mutex);
        sem_post(items);
    }
    
    printf("P%d: exiting\n", *id);
    return NULL;
}


void *consumer_run(void *arg) {
    int *id = arg;

    while (1) {
        sem_wait(items);
        sem_wait(mutex);
        if (eventbuf_empty(eb)) {
            sem_post(mutex);
            break;
        }
        int event = eventbuf_get(eb);
        printf("C%d: got event %d\n", *id, event);
        sem_post(mutex);
        sem_post(spaces);
    }
    
    printf("C%d: exiting\n", *id);
    return NULL;
}


int main(int argc, char *argv[]) {
    // Parse command line
    if (argc != 5) {
        fprintf(stderr, "usage: pc producers consumers num_events num_outstanding_events\n");
        exit(1);
    }

    num_producers = atoi(argv[1]);
    num_consumers = atoi(argv[2]);
    num_events = atoi(argv[3]);
    num_outstanding = atoi(argv[4]);

    mutex = sem_open_temp("mutex", 1);
    items = sem_open_temp("items", 0);
    spaces = sem_open_temp("spaces", num_outstanding);

    eb = eventbuf_create();

    // Allocate thread handle array for all producers
    pthread_t *p_thread = calloc(num_producers, sizeof *p_thread);

    // Allocate thread handle array for all consumers
    pthread_t *c_thread = calloc(num_consumers, sizeof *c_thread);

    // Allocate thread ID array for all producers
    int *p_thread_id = calloc(num_producers, sizeof *p_thread_id);

    // Allocate thread ID array for all consumers
    int *c_thread_id = calloc(num_consumers, sizeof *c_thread_id);

    // Launch all producers
    for (int i = 0; i < num_producers; i++) {
        p_thread_id[i] = i;
        pthread_create(p_thread + i, NULL, producer_run, p_thread_id + i);
    }

    // Launch all consumers
    for (int i = 0; i < num_consumers; i++) {
        c_thread_id[i] = i;
        pthread_create(c_thread + i, NULL, consumer_run, c_thread_id + i);
    }
    
    // Wait for all producers to finish
    for (int i = 0; i < num_producers; i++) {
        pthread_join(p_thread[i], NULL);
    }

    // Signal consumer threads
    for (int i = 0; i < num_consumers; i++) {
        sem_post(items);
    }
    
    // Wait for all consumers to finish
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(c_thread[i], NULL);
    }

    eventbuf_free(eb);
}