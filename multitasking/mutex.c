#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int c = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // this is a macro calling pthread_mutex_init() which set its attributes to default values
void* fnC() {
    int i, j;
    for (i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutex1);
        ++c;
        for (j = 0; j < 5000000; ++j);
        printf(" %d", c);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t tA, tB;
    int rtA, rtB;
    // Create two threads
    if (rtA = pthread_create(&tA, NULL, fnC, NULL))    // If create successfully, the function will return 0
        printf("Thread creation failed: %d\n", rtA);
    if (rtB = pthread_create(&tB, NULL, fnC, NULL))
        printf("Thread creation failed: %d\n", rtB);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_mutex_destroy(&mutex1); // when the mutex is no longer needed, it can be destroyed by calling this function
    printf("\n");
    return 0;
}