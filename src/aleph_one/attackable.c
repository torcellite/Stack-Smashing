#include <stdio.h>
#include <string.h>

#define NOP 0x90

int main(int argc, char **argv) {

    // this line is to check if the shellcode has any CR, LF characters
    // printf("Number of arguments: %d\n", argc);
    char buffer[80];
    memset(buffer, NOP, sizeof(buffer));
    int i;
    for (i = 1; i < argc; i++) {
        // printf("%s\nLength: %d\n", argv[i], strlen(argv[i]));
    }    
    strcpy(buffer, argv[1]);
    printf("%p\n", &buffer);
    printf("I haven't been hacked yet. Going to exit program normally.\n");
    return 0;

}
