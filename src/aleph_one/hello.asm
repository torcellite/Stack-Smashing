global _start			   ; global entry point export for ld

section .text

_start:

	jmp message

mystart:

	; sys_write(stdout, message, length)
	; xor registers to clear them and then push the values only to the lowest bits to avoid nulls
	xor   rax, rax
	mov   al, 0x1		   ; sys_write
	xor   rdi, rdi		  
	mov   dil, 0x1	 	   ; stdout
	pop   rsi		       ; message address
	xor   rdx, rdx
	mov   dl, 0xd		   ; message string length
	syscall

	; sys_exit(return_code)
  
	xor   rax, rax
	mov   al, 0x3c		   ; sys_exit
	syscall

message: 

	call mystart 		       ; push the address of the string onto the stack
	db 'Hello, world!' 		   ; message

section .data
