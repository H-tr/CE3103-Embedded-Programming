#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <linux/gpio.h>
#include <pthread.h>

int fd0;
struct gpioevent_request req_pb;   // for push button
struct gpioevent_data event;

struct gpiohandle_request req_LED;   // LED
struct gpiohandle_data data_LED;     // for data bit

void* Button(void* ptr) {
    while(1) {
        ioctl (fd0, GPIO_GET_LINEEVENT_IOCTL, &req_pb);
        ioctl (fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_LED);
        read(req_pb.fd, &event, sizeof(event));
        data_LED.values[2] = !data_LED.values[2]; //toggle
        printf("Event occur @ %llu \n", event.timestamp);
    }
    return NULL;
}

void* LED(void* ptr) {
    // Toggle the two LEDS
    while (1) {
        ioctl(req_LED.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL,
                &data_LED); // output data bits
        // sleep for 1 second
        usleep(1000000);
        data_LED.values[0] = !data_LED.values[0]; //toggle
        data_LED.values[1] = !data_LED.values[1];
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    int status;
    fd0 = open("/dev/gpiochip0", O_RDWR);
    struct gpiochip_info cinfo;
    ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo);
    fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n",
            cinfo.name, cinfo.label, cinfo.lines);

    req_LED.lines = 3;
    req_LED.lineoffsets[0] = 4;
    req_LED.lineoffsets[1] = 17;
    req_LED.lineoffsets[2] = 27;
    req_LED.flags = GPIOHANDLE_REQUEST_OUTPUT;
    data_LED.values[0] = 1;
    data_LED.values[1] = 0;
    data_LED.values[2] = 0;

    req_pb.lineoffset = 11;
    req_pb.handleflags = GPIOHANDLE_REQUEST_INPUT;

    req_pb.eventflags = GPIOEVENT_REQUEST_RISING_EDGE;
    strcpy(req_pb.consumer_label, "Push Button Event");

    pthread_t thread1, thread2; // thread identifiers
    int T1ret, T2ret;
    // Create two threads
    T1ret = pthread_create(&thread1, NULL,
            LED, NULL);
    T2ret = pthread_create(&thread2, NULL, 
            Button, NULL);
    // The main program (thread 0) now waits for both
    // threads to finish before it continue
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    close(req_pb.fd); // release line
    close(fd0); // close the file
    printf("Done\n");
    return 0;
}