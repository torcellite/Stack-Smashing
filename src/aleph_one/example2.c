/**
		System information:
		Architecture: 64-bit Intel i5 M 480
		$(uname -a): Linux torcellite-vm 3.11.0-26-generic #45-Ubuntu SMP
					 Tue Jul 15 04:02:06 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux 

		Note: 
			Stacks on x86 processors are always fourbyte aligned.
			Stacks on x64 processors are always eightbyte aligned.

		Stack representation:
		
		---------------------------------------------------------------------------	
		| Padding | str     | buffer   | RBP     | RET     | Memory that contains |
		| 8 bytes | 8 bytes | 16 bytes | 8 bytes | 8 bytes | 'A' x 255 times      |
		---------------------------------------------------------------------------

		For more information on this look into Function Calling Sequence in the
		official ABI documentation. 
		http://www.x86-64.org/documentation/abi.pdf
**/

void function(char *str) {
	char buffer[16];
	strcpy(buffer, str);
}

void main() {
	char large_string[256];
	int i;
	for(i = 0; i < 255; i++)
		large_string[i] = 'A';
	function(large_string);
}
