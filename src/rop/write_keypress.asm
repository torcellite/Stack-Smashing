global _start               ; global entry point export for ld

section .text

_start:

    ; sys_open(filename, file access mode, file permissions)

    xor     eax, eax        ; clear eax
    add     al, 0x5         ; sys_open syscall number
    mov     ebx, filename   ; address of "/dev/input/event1"
    xor     ecx, ecx        ; clear ecx
    add     cl, 0x2         ; read and write access
    xor     edx, edx        ; clear edx
    int     0x80            ; trap into the kernel - the file descriptor will be stored in eax

    ; sys_write(fd, message, length)

    xor     ebx, ebx        ; clear ebx
    mov     ebx, eax        ; load file descriptor into ebx
    mov     eax, 0x4        ; sys_write
    mov     ecx, struct     ; message address
    xor     edx, edx        ; clear edx
    mov     edx, 0x10       ; message string length
    int     0x80            ; trap into the kernel

    ; sys_close(fd)

    xor     eax, eax        ; clear eax
    add     eax, 0x6        ; close file
    int     0x80            ; trap into the kernel

    ; sys_exit(return_code)
  
    xor     eax, eax        ; clear eax
    mov     al, 0x1         ; sys_exit
    mov     ebx, 0x1f       ; exit code
    int     0x80            ; trap into the kernel

section .data

    filename db '/dev/input/event1', 0x0 ; event number differs from system to system
    type_ db 1, 0x0
    code  db 31, 0x0
    value db 1, 0x0, 0x0, 0x0
    struct equ type_-8                   ; Structure alignment is below

    ; struct input_event diagram

    ; -----------------------------------------
    ; | time    | type    | code    | value   |
    ; | 8 bytes | 2 bytes | 2 bytes | 4 bytes |
    ; -----------------------------------------
 