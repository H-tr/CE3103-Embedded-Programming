#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int n, duration = 5;
    fd_set input;
    struct timeval timeout;

    if (argc != 2) {
        printf("Usage: %s 'second'\n", argv[0]);
        return 1;
    } else {
        duration = atoi(argv[1]);
        printf("Timeout in %d sec\n", duration);
    }

    // initialize the input set
    FD_ZERO(&input);
    FD_SET(STDIN_FILENO, &input); // use stdin file descriptor

    // initialize the timeout structure
    timeout.tv_sec = duration;
    timeout.tv_usec = 0;

    // Do the select, will typically sleep based on mask returned by poll mehtod, and trigger again when queue wakeup
    n = select(FD_SETSIZE, &input, NULL, NULL, &timeout);

    if (n < 0) // an error
        perror("Timeout failed");
    else if (n == 0) // Timeout
        puts("TIMEOUT");
    else // n > 0, input from stdin
        printf("Timeout terminated\n");
}