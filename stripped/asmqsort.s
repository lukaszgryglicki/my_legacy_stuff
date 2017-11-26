.data		
dane:	
	.long 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 
CR_key:		
	.byte 0xa,0x0
str_ptr:	
	.long 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
str:			
        .asciz  "fedcbas"
        .asciz  "bacfeda"
        .asciz  "defbact"
        .asciz  "debfaca"
        .asciz  "cabfden"
        .asciz  "efbacda"
        .asciz  "abcdefs"
msg_begin:		
	.asciz "Przed sortowaniem\n\n"
msg_end:			
	.asciz "Po sortowaniu\n\n"
msg_authora:			
	.asciz "By MorgothDBMA morgothdbma@o2.pl\n\n"
.globl asm_qsort_str__FPPcUiUi
.globl qsortasm__FPvPFPCvPCv_iUiUii
.globl qsortasm			
.globl _Z8qsortasmPvPFiPKvS1_Ejji
.globl pure_start
.text		
	.align 0x4
bsd_kernel:	
	int $0x80
	ret	
	.align 0x4
init_str_table:	
	movl $0x0, %ecx	
	.align 0x4
loop_addres:	
	leal str, %eax	
	movl %ecx, %edx	
	shll $0x3, %edx	
	addl %edx, %eax	
	leal str_ptr, %ebx
	shrl $0x1, %edx	
	addl %edx, %ebx	
	movl %eax, (%ebx)
	incl %ecx	
	cmpl $0x7, %ecx	
	jl loop_addres	
	ret		
	.align 0x4
pure_start:
	.align 0x4	
	call init_str_table
	pushl $0x13	
	pushl $msg_begin
	call write_msg	
	addl $0x8, %esp	
	call write_out	
	leal str_ptr, %eax
	pushl $0x3	
	pushl $0x6	
	pushl $0x0	
	leal cmpstr, %ebx
	pushl %ebx	
	pushl %eax	
	call qsortasm__FPvPFPCvPCv_iUiUii
	addl $0x14, %esp		
	pushl $0xf	
	pushl $msg_end	
	call write_msg	
	addl $0x8, %esp	
	call write_out	
	pushl $0x22	
	pushl $msg_authora
	call write_msg	
	addl $0x8, %esp	
 	movl $0x1, %eax	
	pushl $0x0	
	call bsd_kernel
	.align 0x4
write_msg:		
	popl %edi	
	pushl $0x1	
	movl $0x4, %eax	
	call bsd_kernel	
	addl $0x4, %esp	
	pushl %edi	
	ret		
	.align 0x4
write_out:		
	.align 0x4	
	movl $0x0, %ecx	
print_line:		
	.align 0x4	
	movl %ecx, %esi	
	shll $0x2, %esi	
	addl $str_ptr, %esi
	movl $0x4, %eax	
	pushl $0x7	
	pushl (%esi)	
	pushl $0x1	
	call bsd_kernel	
	addl $0xc, %esp	
	movl $0x4, %eax	
	pushl $0x1	
	pushl $CR_key	
	pushl $0x1	
	call bsd_kernel	
	addl $0xc, %esp	
	incl %ecx	
	cmpl $0x7, %ecx	
	jl print_line	
	ret		
	.align 0x4
cmpint:			
	popl %esi	
	popl %esi	
	popl %edi	
	movl $0, %eax	
	cmpl %edi, %esi	
	jl skip_int	
	movl $0x1, %eax	
	.align 0x4
skip_int:		
	subl $0xc, %esp	
	ret		
	.align 0x4
cmpstr:		
	.align 0x4	
	popl %esi	
	popl %esi	
	popl %edi	
	.align 0x4
while_len:	
	.align 0x4	
	lodsb		
	movb %al, %ah	
	movl %esi, %edx	
	movl %edi, %esi	
	lodsb		
	movl %edx, %edi	
	xchg %esi, %edi	
	cmpb $0x0, %ah	
	je done_cmp	
	cmpb $0x0, %al	
	je done_cmp	
	cmpb %ah, %al	
	je while_len	
	.align 0x4
done_cmp:		
	.align 0x4	
	cmpb %ah, %al	
	jle set_register
	xorl %eax, %eax	
	jmp ret_from_cmp
set_register:		
	.align 0x4	
	movl $0x1, %eax	
ret_from_cmp:		
	.align 0x4	
	subl $0xc, %esp	
	ret		
halt:			
	.align 0x4	
	subl $0xc, %esp	
	ret		
	.align 0x4
get_function:		
	leal cmpstr, %eax
	movl %eax, dane+0x18
	jmp have_function
	.align 0x4
get_int_function:	
	leal cmpint, %eax
	movl %eax, dane+0x18
	jmp have_function
	.align 0x4
insertion_sort:		
	movl %eax, %ecx	
	shll $0x2, %ecx	
	addl (dane), %ecx
	movl %ebx, %edx	
	shll $0x2, %edx	
	addl (dane), %edx
	pushl %ebx	
	pushl %eax	
	pushl %ecx	
	pushl %edx	
	movl %edx, %edi	
	movl %edx, %esi	
	subl %ecx, %esi	
	cmpl $0x4, %esi	
	jge loop_ins1	
	popl %edx	
	popl %ecx	
	popl %eax	
	popl %ebx	
	ret		
	.align 0x4
loop_ins1:		
	pushl %edi	
	pushl %ecx	
	pushl (%edi)	
	subl $0x4, %edi	
	pushl (%edi)	
	call *(dane+0x18)
	addl $0x8, %esp	
	popl %ecx	
	popl %edi	
	cmpl $0x0, %eax	
	jz no_exch	
	movl %edi, %esi	
	subl $0x4, %esi	
	movl (%edi), %eax
	movl (%esi), %ebx
	movl %eax, (%esi)
	movl %ebx, (%edi)
	.align 0x4
no_exch:		
	subl $0x4, %edi	
	cmpl %edi, %ecx	
	jl loop_ins1	
	popl %edx	
	popl %ecx	
	popl %eax	
	popl %ebx	
	movl %ecx, %edi	
	addl $0x8, %edi	
	movl %ebx, %ecx	
	subl %eax, %ecx	
	cmpl $0x2, %ecx	
	jge znowu_petla	
	ret		
	.align 0x4
znowu_petla:		
	movl (%edi), %ecx
	addl $0x2, %eax	
	movl %edi, %ebx	
	subl $0x4, %ebx	
	pushl %eax	
	pushl %ecx	
	pushl %edx	
	pushl %edi	
	.align 0x4
little_while:		
	pushl %ecx	
	pushl %ebx	
	pushl (%ebx)	
	pushl %ecx	
	call *(dane+0x18)
	addl $0x8, %esp	
	popl %ebx	
	popl %ecx	
	cmpl $0x0, %eax	
	jnz leave_while	
	movl (%ebx), %eax
	addl $0x4, %ebx	
	movl (%ebx), %esi
	movl %eax, (%ebx)
	subl $0x4, %ebx	
	movl %esi, (%ebx)
	subl $0x4, %ebx	
	jmp little_while
	.align 0x4
leave_while:		
	popl %edi	
	popl %edx	
	popl %ecx	
	popl %eax	
	addl $0x4, %ebx	
	movl %ecx, (%ebx)
	addl $0x4, %edi	
	cmpl %edx, %edi	
	jle znowu_petla	
	ret		
qsortasm__FPvPFPCvPCv_iUiUii:
_Z8qsortasmPvPFiPKvS1_Ejji:
qsortasm:		
	.align 0x4
	popl dane+0x14	
	popl dane	
	popl dane+0x18	
	cmpl $0x0,(dane+0x18)
	je get_function	
	cmpl $0x1,(dane+0x18)
	je get_int_function
	.align 0x4
have_function:		
	cmpl $0x0, dane	
	je halt		
	popl %eax	
	popl %ebx	
	popl dane+0x1c	
	cmpl $0x0, (dane+0x1c)
	jg skip_resolution
	movl $0x10, (dane+0x1c) 
skip_resolution:	
	.align 0x4
	pushl (dane+0x14)
	pushl %eax	
	pushl %ebx	
	pushl %ebx	
	pushl %eax	
	call qsorta	
	addl $0x8, %esp	
	popl %ebx	
	popl %eax	
	popl dane+0x14	
	cmpl $0x1, (dane+0x1c)
	jle insertion_done
	call insertion_sort
insertion_done:		
	.align 0x4
	subl $0x10, %esp
	pushl dane+0x14	
	ret	
qsorta:		
	.align 0x4	
	popl dane+0x14	
	popl %eax	
	popl %ebx	
dispatch_insertion:	
	.align 0x4	
	movl %ebx, %esi	
	subl %eax, %esi	
	cmpl (dane+0x1c), %esi
	jge insertion_skipped
	subl $0xc, %esp	
	ret		
insertion_skipped:	
	.align 0x4
	movl %eax, dane+0x4
	movl %ebx, dane+0x8
	incl dane+0x8	
	movl (dane+0x4), %ecx
	shll $0x2, %ecx	
	addl (dane), %ecx
median_swap:		
	.align 0x4	
	movl %eax, %esi	
	addl %ebx, %esi	
	shrl $0x1, %esi	
	shll $0x2, %esi	
	addl (dane), %esi
	movl (%esi), %edi
	movl (%ecx), %edx
	movl %edx, (%esi)
	movl %edi, (%ecx)
	movl %ecx, dane+0xc
	movl %ecx, dane+0x10
while_loop:		
	.align 0x4	
	movl (dane+0x10), %ecx
	movl (dane+0xc), %edx
	pushl %eax	
loop_i:			
	.align 0x4	
	incl dane+0x4	
	addl $0x4, %ecx	
	cmpl (dane+0x4), %ebx
	jl end_loop_i	
	pushl %ecx	
	pushl %ebx	
	pushl %edx	
	pushl (%edx)	
	pushl (%ecx)	
	call *(dane+0x18)
	addl $0x8, %esp	
	movl %eax, %edi	
	popl %edx	
	popl %ebx	
	popl %ecx	
	cmpl $0x0, %edi	
	jz loop_i	
end_loop_i:		
	.align 0x4	
	popl %eax	
	movl %ecx, dane+0x10
	movl (dane+0x8), %ecx
	shll $0x2, %ecx	
	addl (dane), %ecx
	pushl %ebx	
loop_j:			
	.align 0x4	
	decl dane+0x8	
	subl $0x4, %ecx	
	cmpl (dane+0x8), %eax
	jg end_loop_j	
	pushl %eax	
	pushl %ecx	
	pushl %edx	
	pushl (%ecx)	
	pushl (%edx)	
	call *(dane+0x18)
	addl $0x8, %esp	
	movl %eax, %edi	
	popl %edx	
	popl %ecx	
	popl %eax	
	cmpl $0x0, %edi	
	jz loop_j	
end_loop_j:		
	.align 0x4	
	popl %ebx	
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
	pushl (dane+0x8)
	pushl %eax	
	call qsorta	
	addl $0x8, %esp	
	popl dane+0x14	
	popl dane+0x8	
	popl %ebx	
	popl %eax	
	jmp ret_from_left
qsort_right:		
	.align 0x4	
	pushl (dane+0x14)
	pushl %ebx	
	pushl (dane+0x8)
	call qsorta	
	addl $0x8, %esp	
	popl dane+0x14	
endcode:		
	.align 0x4	
	subl $0x8, %esp	
	pushl (dane+0x14)
	ret		
