#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, sock, fd, max_fd;
    fd_set input;   // buffer for input
    struct timeval timeout; // timer structure

    // Initialize the input set
    FD_ZERO(&input);    // clear the buffer
    FD_SET(fd, &input); // add the fd and socket to buffer
    FD_SET(sock, &input);
    max_fd = (sock > fd ? sock : fd) + 1;

    // Initialize the timeout structure
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    // Do the select, will typically sleep based on mask returned by the poll method, and trigger again when queue wakeup
    n = select(max_fd, &input, NULL, NULL, &timeout);

    // See if there was an error
    if (n < 0)
        perror("select failed");
    else if (n == 0)
        puts("TIMEOUT");
    else {  // n > 0 means we have input. But from which source?
        if (FD_ISSET(fd, &input)) // check the file descriptor flag
            process_fd();
        if (FD_ISSET(sock, &input))
            process_socket();
    }
    return 0;
}
