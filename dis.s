	.global _start

	.text
_start:
	mov $20, %rax
	mov $60, %rax
	syscall
	mov $60, %rax
	syscall
