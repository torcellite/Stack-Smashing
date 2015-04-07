#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/**
    Example usage:
        #!/bin/bash
        
        bin/rop/sendevent 1 1 31 1
        bin/rop/sendevent 1 1 31 0

    This presses the S key.

    You can find the event number and mapping of key using evtest.
    sudo apt-get install evtest
    The program will work only when it is executed as superuser.
**/

int main(int argc, char *argv[]) {
    int fd;
    ssize_t ret;
    int version;
    struct input_event event;
    char event_file[20] = "/dev/input/event";

    if(argc != 5) {
        fprintf(stderr, "Use: %s device type code value. Only found %d arguments.\n", argv[0], argc);
        return 1;
    }

    printf("argc: %d, argv[1]: %s, argv[2]: %d, argv[3]: %d, argv[4]: %d\n", argc, argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

    strcat(event_file, argv[1]);
    event_file[strlen(event_file)] = '\0';

    fd = open(event_file, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "Could not open %s, %s\n", event_file, strerror(errno));
        return 1;
    }
    if (ioctl(fd, EVIOCGVERSION, &version)) {
        fprintf(stderr, "Could not get driver version for %s, %s\n", event_file, strerror(errno));
        return 1;
    }
    memset(&event, 0, sizeof(event));
    event.type = atoi(argv[2]);
    event.code = atoi(argv[3]);
    event.value = atoi(argv[4]);
    printf("type: %d, code: %d, value: %d\n", event.type, event.code, event.value);
    ret = write(fd, &event, sizeof(event));
    if(ret < (ssize_t) sizeof(event)) {
        fprintf(stderr, "Write event failed, %s\n", strerror(errno));
        return -1;
    }

    close(fd);
    exit(0);
    return 0;
}