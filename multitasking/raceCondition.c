#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int c = 0;  // global variable
void* funC() {
    int i, j;
    for (i = 0; i < 10; ++i) {
        c++;    // increment the global variable
        for (j = 0; j < 5000000; ++j); //long delay
        printf(" %d", c);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t tA, tB;
    int rtA, rtB;
    // Create two threads
    if (rtA = pthread_create(&tA, NULL, funC, NULL))    // If create successfully, the function will return 0
        printf("Thread creation failed: %d\n", rtA);
    if (rtB = pthread_create(&tB, NULL, funC, NULL))
        printf("Thread creation failed: %d\n", rtB);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    printf("\n");
    return 0;
}