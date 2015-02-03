/**
		System information:
		Architecture: 64-bit Intel i5 M 480
		$(uname -a): Linux torcellite-vm 3.11.0-26-generic #45-Ubuntu SMP
					 Tue Jul 15 04:02:06 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux 

		Note: 
			Stacks on x86 processors are always fourbyte aligned.
			Stacks on x64 processors are always eightbyte aligned.

		The stack is created on x64 machines only if they're initialized.
		At least that's what happens with respect to this example and my system.
		This is the reason for the memset() call.

		Stack representation:
		
		---------------------------------------------------------------------------------------------------------	
		| Padding | int c   | int b   | int a   | buffer 2  | Padding | buffer 1 | Padding  | RBP     | RET     |
		| 4 bytes | 4 bytes | 4 bytes | 4 bytes | 10 bytes  | 6 bytes | 5 bytes  | 11 bytes | 8 bytes | 8 bytes |
		---------------------------------------------------------------------------------------------------------

		If memset() isn't called the parameters and function's variables
		are not pushed onto the stack i.e. the callee doesn't push anything
		onto the stack.
		---------------------
		| RBP     | RET     |
		| 8 bytes |	8 bytes |	
		---------------------

		For more information on this look into Function Calling Sequence in the
		official ABI documentation. 
		http://www.x86-64.org/documentation/abi.pdf
**/

void function(int a, int b, int c) {
	char buffer1[5];
	char buffer2[10];
	memset(buffer1, 'a', sizeof(buffer1));
	memset(buffer2, 'b', sizeof(buffer2));
}

int main() {
	function(1, 2, 3);
	return 0;
}
