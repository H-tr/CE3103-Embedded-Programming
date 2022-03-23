#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void main(void) {
    FILE* fp;
    char buf[20];
    // Create the FIFO if it does not exist
    umask(0); // set user mask permission for file creation
    mknod("mypipe", S_IFIFO|0666, 0);   // Here we use different function to show that both are ok
    while(1) {
        fp = fopen("mypipe", "r");
        fgets(buf, 20, fp);
        printf("We received: %s\n", buf);
        fclose(fp);
    }
}