global _start

section .text

_start:
	xor rax, rax
	mov al,  0x3c
	xor rdi, rdi
	mov dil, 0x31
	syscall
