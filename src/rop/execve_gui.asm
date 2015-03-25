global _start

section .text

_start:

    xor     eax, eax    ; make eax - NULL
    mov     edx, envp   ; third parameter - envp
    mov     ebx, path   ; first parameter - path
    mov     ecx, argv   ; second parameter - argp
    mov     al, 0xb     ; syscall number for __execve
    int     0x80        ; trap into kernel
    
section .data
    
    path db '/usr/bin/abiword', 0x0
    argv dd path, 0x0
    disp db 'DISPLAY=:0.0', 0x0
    envp dd disp, 0x0