	.file	"calln.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d.%s"
.LC1:
	.string	"%08d.%s "
.LC2:
	.string	"%s "
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movl	$1, %eax
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movl	%edi, %ebx
	subq	$536, %rsp
	.cfi_def_cfa_offset 576
	cmpl	$2, %edi
	jg	.L16
.L2:
	addq	$536, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L16:
	.cfi_restore_state
	movq	8(%rsi), %rdi
	movq	%rsi, %r13
	movb	$0, 272(%rsp)
	leaq	16(%r13), %r12
	call	atoi
	cmpl	$1001, %eax
	movl	%eax, %ebp
	movl	$0, %eax
	cmovae	%eax, %ebp
	subl	$3, %ebx
	leaq	24(%r13,%rbx,8), %r13
	.p2align 4,,10
.L10:
	movq	(%r12), %rdi
	leaq	16(%rsp), %rcx
	xorl	%eax, %eax
	movq	%rsp, %rdx
	movl	$.LC0, %esi
	call	sscanf
	cmpl	$2, %eax
	jne	.L4
	xorl	%ebx, %ebx
	testl	%ebp, %ebp
	je	.L6
	.p2align 4,,10
.L13:
	movl	(%rsp), %edx
	leaq	16(%rsp), %rcx
	leaq	144(%rsp), %rdi
	movl	$.LC1, %esi
	xorl	%eax, %eax
	addl	%ebx, %edx
	addl	$1, %ebx
	call	sprintf
	leaq	144(%rsp), %rsi
	leaq	272(%rsp), %rdi
	call	strcat
	cmpl	%ebp, %ebx
	jne	.L13
.L6:
	addq	$8, %r12
	cmpq	%r13, %r12
	jne	.L10
.L17:
	leaq	272(%rsp), %rdi
	call	system
	xorl	%eax, %eax
	jmp	.L2
	.p2align 4,,10
.L4:
	movq	(%r12), %rdx
	leaq	144(%rsp), %rdi
	movl	$.LC2, %esi
	xorl	%eax, %eax
	addq	$8, %r12
	call	sprintf
	leaq	144(%rsp), %rsi
	leaq	272(%rsp), %rdi
	call	strcat
	cmpq	%r13, %r12
	jne	.L10
	jmp	.L17
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (FreeBSD Ports Collection) 4.8.0 20120805 (experimental)"
	.section	.note.GNU-stack,"",@progbits
