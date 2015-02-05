global _start			   ; global entry point export for ld

section .text

_start:

	jmp message

mystart:

	; sys_write(stdout, message, length)
	; xor registers to clear them
	; then push the values only to the lowest bits to avoid nulls
	xor   eax, eax
	mov   al, 0x4		   ; sys_write
	xor   ebx, ebx		  
	inc   ebx	 	       ; stdout, mov ebx, 0x1 gave hexcode with null in it
	pop   ecx		   	   ; message address
	xor   edx, edx
	mov   dl, 0x7		   ; message string length
	int   0x80

	; sys_exit(return_code)
  
	xor   eax, eax
	mov   al, 0x1		   ; sys_exit
	int   0x80

message: 

	call mystart 		   ; push the address of the string onto the stack
	db 'Hacked!' 	       ; message

section .data
