#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFF_SIZE 100

typedef struct {
    int buf[BUFF_SIZE];
    int in;
    int out;
    sem_t buffer;
    sem_t data;
} sbuf_t;

sbuf_t shared;  // 这东西还就得是全局变量

void Put(char item) {
    // write item to shared buffer; If there is no empty buffer, wait
    sem_wait(&shared.buffer);
    shared.buf[shared.in] = item;
    // increment the nuber of data now available in buffer
    shared.in = (shared.in + 1) % BUFF_SIZE;

    // generate signal
    sem_post(&shared.data);
}

void Get() {
    int item;
    // Prepare to get data from buffer;
    // if there is no data, wait
    sem_wait(&shared.data);
    item = shared.buf[shared.out];
    // Increment the buffer now available
    shared.out = (shared.out + 1) % BUFF_SIZE;
    sem_post(&shared.buffer);
}

int main() {
    pthread_t tP, tC;
    sem_init(&shared.data, 0, 0);
    sem_init(&shared.buffer, 0, BUFF_SIZE);

    pthread_create(&tP, NULL, &Put, NULL);
    pthread_create(&tC, NULL, &Get, NULL);

    pthread_join(tP, NULL);
    pthread_join(tC, NULL);

    sem_destroy(&shared.data);
    sem_destroy(&shared.buffer);
}