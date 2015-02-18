global _start

section .text

_start:
    xor     eax, eax
    mov     al, 0x1
    xor     ebx, ebx
    mov     bl, 0x31
    int     0x80
