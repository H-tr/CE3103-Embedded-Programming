#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <linux/gpio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int fd0 = open("/dev/gpiochip0", O_RDWR);
    struct gpiochip_info cinfo;
    ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo);
    fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n",
            cinfo.name, cinfo.label, cinfo.lines);

    struct gpioevent_request req_pb;   // for push button
    struct gpioevent_data event;

    req_pb.lineoffset = 11;
    req_pb.handleflags = GPIOHANDLE_REQUEST_INPUT;

    req_pb.eventflags = GPIOEVENT_REQUEST_RISING_EDGE;
    strcpy(req_pb.consumer_label, "Push Button Event");

    ioctl (fd0, GPIO_GET_LINEEVENT_IOCTL, &req_pb);
    read(req_pb.fd, &event, sizeof(event));
    printf("Event occur @ %llu \n", event.timestamp);

    close(req_pb.fd); // release line
    close(fd0); // close the file
    printf("Done\n");
    exit(EXIT_SUCCESS);
}