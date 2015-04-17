/**
        System information:
        Architecture: 32 bit

        Note: 
            Stacks on x86 processors are always fourbyte aligned.
            Stacks on x64 processors are always eightbyte aligned.

        Stack representation:
        
        -----------------------------------------------------------------------------------------
        | buffer1 | int *ret | Canary bytes   | EBP     | RET     | int a   | int b   | int c   |
        | 5 bytes | 4 bytes  | 8 bytes        | 4 bytes | 4 bytes | 4 bytes | 4 bytes | 4 bytes |
        -----------------------------------------------------------------------------------------

        For more information on this look into Function Calling Sequence in the
        official ABI documentation. 
        http://www.sco.com/developers/devspecs/abi386-4.pdf
**/
#define NOP 0x90

void function(int a, int b, int c, int offset) {
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
        Increment the return value by the difference between statements' addresses.
        (*ret) += 8; Flow of control is returned to line 53
        (*ret) += 36; Flow of control is returned to line 55
        (*ret) += 64; Flow of control is returned to line 57
        (*ret) += 92; Flow of control is returned to line 59
    **/
    (*ret) += offset;
}

int main(int argc, char *argv[]) {
    int offset = 0;
    if (argc >= 2) {
        offset = atoi(argv[1]);
    }
    int x;
    x = 0;
    function(1, 2, 3, offset);
    // Size of assignment statement = 8 bytes
    x = 4;
    // Size of printf statement = 20 bytes
    printf("%d\n", x);
    x = 3;
    printf("%d\n", x);
    x = 2;
    printf("%d\n", x);
    x = 1;
    printf("%d\n", x);
    return 0;
}
