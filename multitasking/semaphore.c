// In embedded systems, semaphores can also be used to signal from an interrupt service routine to a task
// Semaphore is not part of Pthread
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

int c = 0;
sem_t sem_mutex;
void fnC(char k, int p) {
    int i, j, temp;
    for (j = 0; j < 10000000; ++j); // increase the probability for context switch
    for (i = 0; i < p; ++i) {
        sem_wait (&sem_mutex);
        temp = c;
        temp = temp + 1;
        c = temp;
        printf("%c: %d\n", k, c);
        fflush(stdout);
        sem_post(&sem_mutex);
    }
}

void* tAf() {
    fnC('A', 100);
    return NULL;
}

void* tBf() {
    fnC('B', 50);
    return NULL;
}

int main() {
    int rtA, rtB;
    pthread_t tA, tB;

    sem_init(&sem_mutex, 0, 1); // initial value = 1
    pthread_create(&tA, NULL, &tAf, NULL);
    pthread_create(&tB, NULL, &tBf, NULL);

    // wait for both threads to finish
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    sem_destroy(&sem_mutex);
    return 0;
}