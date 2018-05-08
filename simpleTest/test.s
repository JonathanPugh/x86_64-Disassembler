	.global _start

	.text
_start:
	imul %rbx
	mul %rbx
	add $60, %rax
	add %rbx, %rax
	add $60, %eax
	add %ebx, %eax
	mov $43, %ecx
	mov %rax, %rbx
	syscall
	add $60, %eax
