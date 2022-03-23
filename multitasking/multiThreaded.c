#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// When conpiling this file, we need to add -lpthread to link pthread
// gcc -o multiThreaded multiThreaded -lpthread

void* print_msg(void* ptr) {
    char* text;
    text = (char*) ptr;
    printf("%s\n", text);
    fflush(stdout); // it fludhes the output of a stream to print the value immediately
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t thread1, thread2; // thread identifiers
    char* str1 = "I am thread 1";
    char* str2 = "I am thread 2";
    int T1ret, T2ret;
    // Create two threads
    T1ret = pthread_create(&thread1, NULL,
            print_msg, (void*) str1);
    T2ret = pthread_create(&thread2, NULL, 
            print_msg, (void*) str2);
    // The main program (thread 0) now waits for both
    // threads to finish before it continue
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Bye!\n");
    return 0;
}
