	.global _start

	.text
_start:
	add $60, %rax
	add %rbx, %rax
	mov $20, %rdx
	mov $43, %ecx
	mov %rax, %rbx
	syscall
	add $60, %eax
