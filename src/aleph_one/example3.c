/**
		System information:
		Architecture: 64-bit Intel i5 M 480
		$(uname -a): Linux torcellite-vm 3.11.0-26-generic #45-Ubuntu SMP
					 Tue Jul 15 04:02:06 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux 

		Note: 
			Stacks on x86 processors are always fourbyte aligned.
			Stacks on x64 processors are always eightbyte aligned.

		Stack representation:
				
		------------------------------------------------------------------------------------------------------
		| Padding | int c   | int b   | int a   | buffer1 | Padding  | ret     | Padding | RBP     | RET     |
		| 4 bytes | 4 bytes | 4 bytes | 4 bytes | 5 bytes | 3 bytes  | 4 bytes | 4 bytes | 8 bytes | 8 bytes |
		------------------------------------------------------------------------------------------------------

		For more information on this look into Function Calling Sequence in the
		official ABI documentation. 
		http://www.x86-64.org/documentation/abi.pdf
**/

#include <stdio.h>

#define NOP 0x90

void function(int a, int b, int c) {
	char buffer1[5];
	memset(buffer1, NOP, sizeof(buffer1));
	int *ret;
	/** 
		From buffer1 we need to move the pointer to RET,
		so buffer1 + 5 + 3 + 4 + 4 + 8 points to RET
	**/
	ret = buffer1 + 24;
	(*ret) += 7;
}

void main() {
	int x;
	x = 0;
	function(1, 2, 3);
	x = 1;
	printf("%d\n", x);
}
