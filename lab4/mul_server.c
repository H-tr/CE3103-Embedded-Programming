#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>

struct arg_struct {
    int arg1;
    char* arg2[];
};

void error(char* m) {
    perror(m);
}

void* server(char* argv[]) {
    int sockfd, newsockfd, port, clilen, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    port = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   // initial the socket
    if (sockfd < 0)
        error("ERROR opening socket\n");
    
    memset((char*) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);   // host to network

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR bind to socket\n");

    while (1) {
        listen(sockfd, 2);
        clilen = sizeof(cli_addr);

        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);   // accept the socket and create a socket backward
        if (newsockfd < 0)
            error("ERROR on accept\n");
        n = read(newsockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Message received: %s\n", buffer);
        int num = 5 * atoi(buffer);
        char message[200];
        sprintf(message, "Message acknewledge: num * 5 = %d", num);

        n = write(newsockfd, message, 200);
        if (n < 0)
            error("ERROR writing back to socket\n");
    }

    return 0;
}

int main(int argc, char* argv[]) {
    pthread_t T1, T2, T3; // This is thread identifier
    int i = 0;
    if (argc < 4) 
        error("ERROR, no port provided\n");
    
    pthread_create(&T1, NULL, server, (void*) &argv[0]);
    pthread_create(&T2, NULL, server, (void*) &argv[1]);
    pthread_create(&T3, NULL, server, (void*) &argv[2]);
    pthread_join(T1, NULL);
    pthread_join(T2, NULL);
    pthread_join(T3, NULL);
    return 0;
}