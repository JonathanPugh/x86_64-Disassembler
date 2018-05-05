	.global _start

	.text
_start:
	mov $20, %rax
	mov %rax, %rbx
	mov $60, %rax
	syscall
