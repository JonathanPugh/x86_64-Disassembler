# This test program doesn't do anything interesting, but shows 
# some of the instructions the disassembler is capable of processing.
	.global _start

	.text
_start:
	mov %eax, %esi
	mov $0x935, %eax 
	idiv %ebx
	imul %rbx
	div %rdx
	mul %edx
	add $0x60, %rax
	add %rbx, %rax
	add $0x60, %eax
	add %ebx, %eax
	mov $0x43, %ecx
	mov %rax, %rbx
	add $0x60, %eax
	push $0xa874f
	syscall
