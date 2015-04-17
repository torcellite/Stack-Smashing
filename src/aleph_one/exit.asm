global _start

section .text

_start:
    xor     eax, eax    ; clear eax
    mov     al, 0x1     ; exit syscall number
    xor     ebx, ebx    ; clear ebx
    mov     bl, 0x31    ; exit code
    int     0x80        ; trap into kernel