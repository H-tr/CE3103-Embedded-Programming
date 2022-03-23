#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    FILE* fp;
    mkfifo("mypipe", 0666); // create the named pipe
    if((fp = fopen("mypipe", "w")) == NULL) {
        fprintf(stderr, "fopen caused an error\n");
        exit(1);
    }
    fputs(argv[1], fp);
    fclose(fp);
    return 0;
}