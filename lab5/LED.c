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
    
    struct gpiohandle_request req_GY;   // Green and Yellow
    struct gpiohandle_data data_GY;     // for data bit

    req_GY.lines = 2;   // 2 pins in this handler
    req_GY.lineoffsets[0] = 4;  // pin 4 - Green LED
    req_GY.lineoffsets[1] = 17; // pin 17 - Yellow LED

    // set them to be output
    req_GY.flags = GPIOHANDLE_REQUEST_OUTPUT;

    // set data values
    data_GY.values[0] = 1;  // set Green LED to High (ON)
    data_GY.values[1] = 0;  // set Yellow LED to Low (OFF)

    // now get the line handler req_GY
    ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_GY);

    // Toggle the two LEDS
    for (int i = 0; i < 5; ++i) {
        ioctl(req_GY.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL,
                &data_GY); // output data bits
        // sleep for 1 second
        usleep(1000000);
        data_GY.values[0] = !data_GY.values[0]; //toggle
        data_GY.values[1] = !data_GY.values[1];
    }

    close(req_GY.fd); // release line
    close(fd0); // close the file
    printf("Done\n");
    exit(EXIT_SUCCESS);
}