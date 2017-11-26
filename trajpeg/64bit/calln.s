	.file	"calln.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d.%s"
.LC1:
	.string	"%08d.%s "
.LC2:
	.string	"%s "
	.text
.globl main
	.type	main, @function
main:
.LFB16:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r13
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	subq	$552, %rsp
	.cfi_def_cfa_offset 608
	movl	%edi, 12(%rsp)
	movq	%rsi, %r14
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	cmpl	$2, %edi
	jg	.L13
	movl	$1, %eax
.L2:
	addq	$552, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L13:
	.cfi_restore_state
	movq	8(%rsi), %rdi
	call	atoi
	movl	%eax, %r13d
	cmpl	$1001, %eax
	movl	$0, %eax
	cmovae	%eax, %r13d
	movl	$2, 8(%rsp)
	leaq	400(%rsp), %r12
	leaq	16(%rsp), %r15
	leaq	272(%rsp), %rbp
.L7:
	movq	16(%r14), %rdi
	movq	%r12, %rcx
	leaq	540(%rsp), %rdx
	movl	$.LC0, %esi
	xorl	%eax, %eax
	call	sscanf
	cmpl	$2, %eax
	jne	.L4
	testl	%r13d, %r13d
	je	.L5
	xorl	%ebx, %ebx
.L6:
	movl	540(%rsp), %edx
	addl	%ebx, %edx
	movq	%r12, %rcx
	movl	$.LC1, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	sprintf
	movq	%rbp, %rsi
	movq	%r15, %rdi
	call	strcat
	addl	$1, %ebx
	cmpl	%ebx, %r13d
	jg	.L6
.L5:
	addl	$1, 8(%rsp)
	addq	$8, %r14
	movl	8(%rsp), %eax
	cmpl	%eax, 12(%rsp)
	jg	.L7
	movq	%r15, %rdi
	call	system
	xorl	%eax, %eax
	jmp	.L2
.L4:
	movq	16(%r14), %rdx
	movl	$.LC2, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	sprintf
	movq	%rbp, %rsi
	movq	%r15, %rdi
	call	strcat
	jmp	.L5
	.cfi_endproc
.LFE16:
	.size	main, .-main
	.ident	"GCC: (NetBSD nb2 20110806) 4.5.3"
