global _start

section .text

    _start:
	jmp MESSAGE ;
	
    GOBACK:
        
	mov eax, 0x4
	mov ebx, 0x1
	pop rcx
	push 0x1
	pop rax
	mov rdi, rax
        syscall	

        ; exit from the application here
        xor     rdi,rdi
        push    0x3c
        pop     rax
        syscall
   MESSAGE:
	call GOBACK
	db "Hello, World!", 0dh, 0ah

section .data
