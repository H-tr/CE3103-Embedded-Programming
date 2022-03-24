#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int g_varl = 0;
pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER; // this is a macro calling pthread_mutex_init() which set its attributes to default values

void* inc_gv() {
    int i, j;
    for (i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutexA);
        g_varl++;
        for (j = 0; j < 5000000; ++j);
        printf(" %d", g_varl);
        fflush(stdout);
        pthread_mutex_unlock(&mutexA);
    }
}

int main() {
    pthread_t TA, TB;
    int TAret, TBret;

    TAret = pthread_create(&TA, NULL, inc_gv, NULL);
    TBret = pthread_create(&TB, NULL, inc_gv, NULL);

    pthread_join(TA, NULL);
    printf("\n pthread1 completed \n");
    pthread_join(TB, NULL);
    printf("\n pthread2 completed \n");
    return 0;
}