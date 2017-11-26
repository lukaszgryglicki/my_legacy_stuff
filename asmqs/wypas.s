.data
dane:
	.long 0x0,0x0,0x0,0x0,0x0
tabl:
	.long 0x0,0x4,0x3,0x2,0x5,0x1,0x3,0x6,0x5,0x7,0xa,0x4,0x0,0x7
.globl qsortasm__FiiPi
.globl _start
.text
_start:
	.align 0x4
	leal (tabl+0x4), %eax
	pushl %eax
	pushl $0xc
	pushl $0x0
	call qsortasm__FiiPi
	addl $0xc, %esp
 	movl $0x1, %eax
	pushl $0x0
	pushl %eax
	int $0x80
	ret
qsortasm__FiiPi:
	.align 0x4
	popl dane+0x14
	popl %eax
	popl %ebx
	popl dane
	movl %eax, dane+0x4
	movl %ebx, dane+0x8
	incl dane+0x8
	movl (dane+0x4), %ecx
	shll $0x2, %ecx
	addl (dane), %ecx
	movl %ecx, dane+0xc
	movl %ecx, dane+0x10
while_loop:
	.align 0x4
	movl (dane+0x10), %ecx
	movl (dane+0xc), %edx
loop_i:
	.align 0x4
	incl dane+0x4
	addl $0x4, %ecx
	#cmpl (dane+0x4), %ebx
	#jl end_loop_i
	movl (%ecx), %edi
	cmpl (%edx), %edi
	jl loop_i
end_loop_i:
	.align 0x4
	movl %ecx, dane+0x10
	movl (dane+0x8), %ecx
	shll $0x2, %ecx
	addl (dane), %ecx
loop_j:
	.align 0x4
	decl dane+0x8
	subl $0x4, %ecx
	#cmpl (dane+0x8), %eax
	#jg end_loop_j
	movl (%ecx), %edi
	cmpl (%edx), %edi
	jg loop_j
end_loop_j:
	.align 0x4
 	movl (dane+0x4), %esi
	cmpl (dane+0x8), %esi
	jl swap
	jmp continue
swap:
	.align 0x4
	movl (%ecx), %edx
	movl (dane+0x10), %edi
	movl (%edi), %edi
	movl %edi, (%ecx)
	movl (dane+0x10), %edi
	movl %edx, (%edi)
continue:
	.align 0x4
	movl (dane+0x4), %edi
	cmpl (dane+0x8), %edi
	jl while_loop
	movl (%ecx), %edx
	movl (dane+0xc), %edi
	movl (%edi), %edi
	movl %edi, (%ecx)
	movl (dane+0xc), %edi
	movl %edx, (%edi)
recurse:
	.align 0x4
	decl dane+0x8
	cmpl (dane+0x8), %eax
	jl qsort_left
ret_from_left:
	.align 0x4
	addl $0x2, dane+0x8
	cmpl (dane+0x8), %ebx
	jg qsort_right
	jmp endcode
qsort_left:
	.align 0x4
	pushl %eax
	pushl %ebx
	pushl (dane+0x8)
	pushl (dane+0x14)
	pushl (dane)
	pushl (dane+0x8)
	pushl %eax
	call qsortasm__FiiPi
	addl $0xc, %esp
	popl dane+0x14
	popl dane+0x8
	popl %ebx
	popl %eax
	jmp ret_from_left
qsort_right:
	.align 0x4
	pushl (dane+0x14)
	pushl (dane)
	pushl %ebx
	pushl (dane+0x8)
	call qsortasm__FiiPi
	addl $0xc, %esp
	popl dane+0x14
endcode:
	.align 0x4
	subl $0xc, %esp
	pushl (dane+0x14)
	ret
