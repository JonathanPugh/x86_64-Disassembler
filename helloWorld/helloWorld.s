	.global _start

	.text
_start:
	# push "Hell"
	push $0x6C6C6548

	# print chars
	mov $1, %rdi 	# arg 1: file handle (stdout)
	mov %rsp, %rsi 	# arg 2: src
	mov $4, %rdx 	# arg 3: length
	mov $1, %rax 	# sys_write
	syscall

	# push "o Wo"
	push $0x6F57206F

	# print chars
	mov $1, %rdi 	# arg 1: file handle (stdout)
	mov %rsp, %rsi 	# arg 2: src
	mov $4, %rdx 	# arg 3: length
	mov $1, %rax 	# sys_write
	syscall

	# push "rld\n"
	push $0x0A646C72

	# print chars
	mov $1, %rdi 	# arg 1: file handle (stdout)
	mov %rsp, %rsi 	# arg 2: src
	mov $4, %rdx 	# arg 3: length
	mov $1, %rax 	# sys_write
	syscall

	#Exit
	mov $60, %rax
	mov $0, %rbx
	syscall
