#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char temp1[12] = "HTR yyds!";   // message to send
    char temp2[12];
    int fd, i;
    printf("opening file /dev/myDriver\n");
    fd = open("/dev/myDriver", O_RDWR);
    if (fd < 0) {
        printf("Open failed\n");
        exit(-1);
    } else 
        printf("open successfully!\n");

    i = write(fd, temp1, 10);
    printf("number of char write = %d\n", i);
    i = read(fd, temp2, 10);
    printf("number of char read = %i\n", i);
    if (i < 0) {
        printf("Read error\n");
        close(fd);
        exit(-1);
    } else
        printf("Value read = %s\n", temp2);
    close(fd);
    exit(0);
    return 0;
}