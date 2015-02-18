/**
        System information:
        Architecture: 64-bit

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

int main(void) {
    char large_string[256];
    int i;
    for(i = 0; i < 255; i++)
        large_string[i] = 'A';
    function(large_string);
    return 0;
}
