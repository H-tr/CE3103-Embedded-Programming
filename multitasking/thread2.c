#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// This is the thread function
void* threadFunc(void* arg) {
    char* str;
    int i = 0;
    str = (char*) arg;
    while (i < 110) {
        usleep(1); // sleep for 1 microsecond
        printf("Thrad is running: %s\n", str);
        ++i;
    }
    return NULL;
}

int main(void) {
    pthread_t T1; // This is thread identifier
    int i = 0;
    pthread_create(&T1, NULL, threadFunc, "foo");
    while (i < 100) {
        usleep(1);
        printf("main is running...\n");
        fflush(stdout);
        ++i;
    }
    printf("main waiting for thread to terminate.\n");
    pthread_join(T1, NULL);
    return 0;
}