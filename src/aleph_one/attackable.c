#include <stdio.h>
#include <string.h>

#define NOP 0x90

/**
    Buffer
    -----------------------------------------------------------------------
    | Buffer -   | Canary bytes - | EBP -   | RET -   | argc -  | argv -  |
    | 80 bytes   | 8 bytes        | 4 bytes | 4 bytes | 4 bytes | 4 bytes |
    -----------------------------------------------------------------------
**/

int main(int argc, char **argv) {

    char buffer[80];
    memset(buffer, NOP, sizeof(buffer));   
    if (argc >= 2) {
        // This line is to check if the shellcode has any white space characters
        printf("argc: %d\n", argc);
        strcpy(buffer, argv[1]);
    }
    printf("Address of buffer: %010p\n", &buffer);
    printf("I haven't been hacked yet. Going to exit program normally.\n");

    return 0;
}