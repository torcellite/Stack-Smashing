/**
		System information:
		Architecture: 
		$(uname -a): 

		Note: 
			Stacks on x86 processors are always fourbyte aligned.
			Stacks on x64 processors are always eightbyte aligned.

		Stack representation:
		
		-----------------------------------------------------------------------------------
		| buffer2  | buffer 1 | Slack	| EBP     | RET		| int a   | int b   | int c   |
		| 10 bytes | 5 bytes  | 8 bytes	| 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes |
		-----------------------------------------------------------------------------------

		For more information on this look into Function Calling Sequence in the
		official ABI documentation. 
		http://www.sco.com/developers/devspecs/abi386-4.pdf
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
