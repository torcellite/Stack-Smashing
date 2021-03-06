/**
        System information:
        Architecture: 32 bit

        Note: 
            Stacks on x86 processors are always fourbyte aligned.
            Stacks on x64 processors are always eightbyte aligned.

        Stack representation:
        
        -------------------------------------------------------------------------------------------
        | buffer   | Canary bytes | EBP     | RET     | str     | Random   | Memory that contains |
        | 16 bytes | 8 bytes      | 4 bytes | 4 bytes | 4 bytes | 24 bytes | 'A' x 255 times      |
        -------------------------------------------------------------------------------------------

        For more information on this look into Function Calling Sequence in the
        official ABI documentation. 
        http://www.sco.com/developers/devspecs/abi386-4.pdf
**/

void function(char *str) {
    char buffer[16];
    memset(buffer, 'A', 16);
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
