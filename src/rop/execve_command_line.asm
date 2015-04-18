global _start

section .text

_start:

    xor     eax, eax    ; clear eax
    mov     edx, 0x0    ; third parameter - NULL
    mov     ebx, path   ; first parameter - path
    mov     ecx, argv   ; second parameter - argp
    mov     al, 0xb     ; syscall number for __execve
    int     0x80        ; trap into kernel
    
section .data
    
    path db '/bin/ls', 0x0
    arg1 db '-Rlaxo', 0x0
    argv dd path, arg1,  0x0