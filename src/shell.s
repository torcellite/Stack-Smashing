.globl _start
_start:
	jmp offset-to-call
	pop  %rsi
	movq %rsi, -0x10(%rsi)
	movq $0x0, -0x8(%rbp)
	mov  -0x10(%rbp), %rax
	lea  -0x10(%rbp), %rcx
	mov  $0x0, %edx
	mov  %rcx, %rsi
	mov  %rax, %rdi
	syscall
	movq $0x1
