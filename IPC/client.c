#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>  // define hostent struct

void error(char* m) {   // print out error message
    perror(m);
}

int main(int argc, char* argv[]) {
    int sockfd, port, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buff[256];

    if (argc < 3)
        error("usage client [hostname] [port]\n");
    port = atoi(argv[2]);   // convert a string to int
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   // Define a TCP package
    if (sockfd < 0)
        error("ERROR opening socket\n");
    server = gethostbyname(argv[1]);    // get server's address
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

    printf("Please enter the messsage: ");
    fgets(buff, 255, stdin);

    n = write(sockfd, buff, strlen(buff));
    if (n < 0)
        error("ERROR writing to socket\n");

    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket\n");

    printf("%s\n", buff);
    return 0;
}