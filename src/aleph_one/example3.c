/**
		System information:
		Architecture: 32 bit

		Note: 
			Stacks on x86 processors are always fourbyte aligned.
			Stacks on x64 processors are always eightbyte aligned.

		Stack representation:
		
		---------------------------------------------------------------------------------
		| buffer1 | ret     | Slack   | EBP     | RET     | int a   | int b   | int c   |
		| 5 bytes | 4 bytes | 8 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes |
		---------------------------------------------------------------------------------

		For more information on this look into Function Calling Sequence in the
		official ABI documentation. 
		http://www.sco.com/developers/devspecs/abi386-4.pdf
**/

#include <stdio.h>

#define NOP 0x90

void function(int a, int b, int c) {
	char buffer1[5];
	// Set buffer contents to NOP for reference sake
	memset(buffer1, NOP, sizeof(buffer1));
	int *ret;
	/** 
		From buffer1 we need to move the pointer to RET,
		so buffer1 + 5 + 4 + 8 + 4 points to RET
	**/
	ret = buffer1 + 21;
	/**
		We now need to increment the return address by the difference
		between the addresses of x = 1; and printf("%d\n", x);,
		so that it points to printf("%d\n", x); rather than
		x = 1;
		Increment the return address so that it skips the assigment x=1;
	**/
	(*ret) += 1;
}

int main(void) {
	int x;
	x = 0;
	function(1, 2, 3);
	x = 1;
	printf("%d\n", x);
	return 0;
}
