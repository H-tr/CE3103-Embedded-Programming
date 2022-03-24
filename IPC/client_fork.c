#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>  // define hostent struct
#include <time.h>

void error(char* m) {   // print out error message
    perror(m);
}

int client(char* addr, int port, int key) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buff[256];

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   // Define a TCP package
    if (sockfd < 0)
        error("ERROR opening socket\n");
    server = gethostbyname(addr);    // get server's address
    if (server == NULL)
        error("ERROR, no such host\n");
    // start with a clean address structure
    memset(&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET; // internet socket
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr,
            server->h_length);  // assign server's address to socket
    serv_addr.sin_port = htons(port);   // server's port

    if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0) // connect an available socket on a server. Since this is a stream socket, 
        error("ERROR connecting\n");

    sprintf(buff, "%d", key);

    n = write(sockfd, buff, strlen(buff));
    if (n < 0)
        error("ERROR writing to socket\n");

    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket\n");

    printf("%s\n", buff);
    return 0;
}

int main(int argc, char* argv[]) {
    srand(time(0));
    pid_t ch_pid, ch2_pid, ch3_pid;
    int status;
    char* addr = argv[1];
    int port1 = atoi(argv[2]);   // convert a string to int
    int port2 = atoi(argv[3]);   // convert a string to int
    int port3 = atoi(argv[4]);   // convert a string to int

    printf("\nThis program demonstrates the fork\n");

    ch_pid = fork();
    if (ch_pid != 0) { // progress 1
        srand(time(0));
        int key = rand()%20;
        client(addr, port1, key);
        wait(&status);  // parent wait for child to terminate
    }
    else {
        // ch_pid == 0 means this is child process
        ch2_pid = fork();
        if (ch2_pid != 0) { // progress 2
            srand(time(0));
            int key = rand()%20;
            client(addr, port2, key);
            wait(&status); // I don't know what will happen. Let's see!
        }
        else {  // progress 3
            srand(time(0));
            int key = rand()%20;
            client(addr, port3, key);
            return 20;
        }
        return 20; // child terminated itself here
    }
    return 0;
}