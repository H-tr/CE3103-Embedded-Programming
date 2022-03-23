#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[]) {
    pid_t ch_pid, ch2_pid;
    int status;

    printf("\nThis program demonstrates the fork\n");

    ch_pid = fork();
    if (ch_pid != 0) {
        printf("Hello, this is parent process and we need to wait for child process to finish!\n");
        printf("parent pid is: %d\n", ch_pid);
        wait(&status);  // parent wait for child to terminate
    }
    else {
        // ch_pid == 0 means this is child process
        printf("Hello! I'm child process!\n");
        printf("Child pid is %d\n", ch_pid);
        ch2_pid = fork();
        if (ch2_pid != 0) {
            printf("This is the parent in child2, the pid is: %d\n", ch2_pid);
            wait(&status); // I don't know what will happen. Let's see!
        }
        else {
            printf("This is suppose to be child of child. The pid is: %d\n", ch2_pid);
            return 20;
        }
        printf("Child1 back!\n");

        return 20; // child terminated itself here
    }
    printf("Parent comes back!\n");
    return 0;
}