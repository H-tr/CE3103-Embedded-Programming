#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[]) {
    printf("Hello world!\n");
    fork();
    fork();
    printf("Hello world!\n");
    return 0;
}