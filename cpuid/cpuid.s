.code32
.text
.globl _start
_start:
	cpuid
	movl %edx, %esi
	call print_esi
	movl $0x1, %eax
	pushl $0x0
	pushl $0x0
	int $0x80
print_esi:
	movl $0x80000000, %ecx
loop:
	movl $0x4, %eax
	pushl $0x1
	test %ecx, %esi
	jnz print_1
	pushl $zero
	jmp continue
print_1:
	pushl $one
continue:
	pushl $0x1
	pushl $0x0
	int $0x80
	addl $0x10, %esp
	shrl %ecx
	cmpl $0x0, %ecx
	jnz loop
	call nl
	call hex
	ret
nl:
	movl $0x4, %eax
	pushl $0x1
	pushl $nn
	pushl $0x1
	pushl $0x0
	int $0x80
	addl $0x10, %esp
	ret
hex:
	call hex_sign
	movl %esi, %ebx
	andl $0xF0000000, %ebx
	shr $0x1C, %ebx
	call set_bl
        call write_digit
	movl %esi, %ebx
	andl $0x0F000000, %ebx
	shr $0x18, %ebx
	call set_bl
        call write_digit
	movl %esi, %ebx
	andl $0x00F00000, %ebx
	shr $0x14, %ebx
	call set_bl
        call write_digit
	movl %esi, %ebx
	andl $0x000F0000, %ebx
	shr $0x10, %ebx
	call set_bl
        call write_digit
	movl %esi, %ebx
	andl $0x0000F000, %ebx
	shr $0x0C, %ebx
	call set_bl
	movl %esi, %ebx
        call write_digit
	andl $0x00000F00, %ebx
	shr $0x08, %ebx
	call set_bl
	movl %esi, %ebx
        call write_digit
	andl $0x000000F0, %ebx
	shr $0x04, %ebx
	call set_bl
	movl %esi, %ebx
        call write_digit
	andl $0x0000000F, %ebx
	call set_bl
        call write_digit
	call nl
	ret
write_digit:
	movl $0x4, %eax
	pushl $0x1
	pushl $digit
	pushl $0x1
	pushl $0x0
	int $0x80
	addl $0x10, %esp
	ret
set_bl:
	cmpl $0xa, %ebx
	jge set_hex
	addl $0x30, %ebx
	movb %bl, (digit)
	ret
set_hex:
	addl $0x37, %ebx
	movb %bl, (digit)
	ret
hex_sign:
	movl $0x4, %eax
	pushl $0x2
	pushl $hxsgn
	pushl $0x1
	pushl $0x0
	int $0x80
	addl $0x10, %esp
	ret
.data
hxsgn:
	.byte 0x30,0x78,0x0
digit:
	.byte 0x0,0x0
one:
	.byte 0x31,0x0
zero:
	.byte 0x30,0x0
nn:
	.byte 0xa,0x0
