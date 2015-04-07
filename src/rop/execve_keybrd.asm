global _start

section .text

_start:

    xor     eax, eax    ; make eax - NULL
    mov     edx, 0x0    ; third parameter - NULL
    mov     ebx, path   ; first parameter - path
    mov     ecx, argv1  ; second parameter - argp
    mov     al, 0xb     ; syscall number for __execve
    int     0x80        ; trap into kernel
    
section .data
    
    path db 'bin/rop/sendevent', 0x0
    arg1 db '1', 0x0    ; change this to the event number of the keyboard on your system
    arg2 db '1', 0x0
    arg3 db '30', 0x0   ; this presses A on my system
    arg4 db '1', 0x0
    arg5 db 0x0
    argv1 dd path, arg1, arg2, arg3, arg4, 0x0
    argv2 dd path, arg1, arg2, arg3, arg5, 0x0