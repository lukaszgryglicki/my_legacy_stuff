	.file	"fjpeg.c"
	.text
	.type	my_error_exit, @function
my_error_exit:
.LFB23:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	(%rdi), %rbx
	.cfi_offset 3, -16
	call	*16(%rbx)
	leaq	168(%rbx), %rdi
	movl	$1, %esi
	call	__longjmp14
	.cfi_endproc
.LFE23:
	.size	my_error_exit, .-my_error_exit
.globl alloc_cache
	.type	alloc_cache, @function
alloc_cache:
.LFB24:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$0, ncache(%rip)
	movslq	%edi, %rdi
	leaq	(%rdi,%rdi,2), %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, icache(%rip)
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	alloc_cache, .-alloc_cache
.globl lookup_cache
	.type	lookup_cache, @function
lookup_cache:
.LFB25:
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
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, %rbx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%rsi, %rbp
	movq	%rdx, %r12
	movq	%rcx, 8(%rsp)
	movl	ncache(%rip), %eax
	movl	%eax, 4(%rsp)
	testl	%eax, %eax
	jle	.L7
	movq	icache(%rip), %r13
	movl	%eax, %r15d
	subl	$1, %r15d
	andl	$7, %r15d
	movq	0(%r13), %rsi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	movl	$1, %r14d
	addq	$24, %r13
	cmpl	$1, 4(%rsp)
	jle	.L7
	testl	%r15d, %r15d
	je	.L6
	cmpl	$1, %r15d
	je	.L47
	cmpl	$2, %r15d
	je	.L48
	cmpl	$3, %r15d
	je	.L49
	cmpl	$4, %r15d
	je	.L50
	cmpl	$5, %r15d
	je	.L51
	cmpl	$6, %r15d
	je	.L52
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$1, %r14d
	addq	$24, %r13
.L52:
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$1, %r14d
	addq	$24, %r13
.L51:
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$1, %r14d
	addq	$24, %r13
.L50:
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$1, %r14d
	addq	$24, %r13
.L49:
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$1, %r14d
	addq	$24, %r13
.L48:
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$1, %r14d
	addq	$24, %r13
.L47:
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$1, %r14d
	addq	$24, %r13
	cmpl	%r14d, 4(%rsp)
	jle	.L7
.L6:
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	leaq	24(%r13), %r15
	movq	%r15, %r13
	movq	(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addq	$24, %r13
	movq	0(%r13), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	leaq	48(%r15), %r13
	movq	48(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	leaq	72(%r15), %r13
	movq	72(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	leaq	96(%r15), %r13
	movq	96(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	leaq	120(%r15), %r13
	movq	120(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	leaq	144(%r15), %r13
	movq	144(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L53
	addl	$8, %r14d
	leaq	168(%r15), %r13
	cmpl	%r14d, 4(%rsp)
	jg	.L6
.L7:
	movl	$1, %eax
	addq	$24, %rsp
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
.L53:
	.cfi_restore_state
	movq	8(%r13), %rbx
	movq	%rbx, 0(%rbp)
	movl	16(%r13), %ecx
	movl	%ecx, (%r12)
	movl	20(%r13), %eax
	movq	8(%rsp), %rdx
	movl	%eax, (%rdx)
	xorl	%eax, %eax
	addq	$24, %rsp
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
	.cfi_endproc
.LFE25:
	.size	lookup_cache, .-lookup_cache
.globl insert_cache
	.type	insert_cache, @function
insert_cache:
.LFB26:
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
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, %rbx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%rsi, %rbp
	movl	%edx, %r15d
	movl	%ecx, %r14d
	movl	ncache(%rip), %r13d
	movslq	%r13d, %rdx
	leaq	(%rdx,%rdx,2), %r12
	salq	$3, %r12
	addq	icache(%rip), %r12
	xorl	%eax, %eax
	movq	$-1, %rcx
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, (%r12)
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	%r15d, 16(%r12)
	movl	%r14d, 20(%r12)
	movq	%rbp, 8(%r12)
	addl	$1, %r13d
	movl	%r13d, ncache(%rip)
	addq	$8, %rsp
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
	.cfi_endproc
.LFE26:
	.size	insert_cache, .-insert_cache
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"fstat"
.LC1:
	.string	"mmap"
	.text
.globl fmmf
	.type	fmmf, @function
fmmf:
.LFB27:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	subq	$160, %rsp
	.cfi_def_cfa_offset 192
	movq	%rsi, %rbx
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movq	%rdx, %rbp
	movq	$0, (%rsi)
	movq	$0, (%rdx)
	call	fileno
	movl	%eax, %r12d
	movq	%rsp, %rsi
	movl	%eax, %edi
	call	__fstat50
	testl	%eax, %eax
	js	.L61
	xorl	%r9d, %r9d
	movl	%r12d, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	movq	112(%rsp), %rsi
	xorl	%edi, %edi
	call	mmap
	cmpq	$-1, %rax
	je	.L62
	movq	%rax, (%rbx)
	movq	112(%rsp), %rax
	movq	%rax, 0(%rbp)
	xorl	%eax, %eax
	addq	$160, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L61:
	.cfi_restore_state
	movl	$.LC0, %edi
	call	perror
	movl	$1, %eax
	addq	$160, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L62:
	.cfi_restore_state
	movl	$.LC1, %edi
	call	perror
	movl	$2, %eax
	addq	$160, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE27:
	.size	fmmf, .-fmmf
.globl RGB
	.type	RGB, @function
RGB:
.LFB28:
	.cfi_startproc
	sall	$8, %esi
	addl	%edx, %esi
	sall	$16, %edi
	leal	(%rsi,%rdi), %edi
	movslq	%edi, %rax
	ret
	.cfi_endproc
.LFE28:
	.size	RGB, .-RGB
.globl dRGB
	.type	dRGB, @function
dRGB:
.LFB29:
	.cfi_startproc
	movq	%rcx, %r8
	shrq	$16, %r8
	movb	%r8b, (%rdi)
	movzbl	%ch, %eax
	movb	%al, (%rsi)
	movb	%cl, (%rdx)
	ret
	.cfi_endproc
.LFE29:
	.size	dRGB, .-dRGB
.globl set_rows_array
	.type	set_rows_array, @function
set_rows_array:
.LFB30:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	testl	%r9d, %r9d
	je	.L66
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %esi
	jle	.L65
.L74:
	movl	-16(%rsp), %eax
	cltq
	movq	(%rdx,%rax,8), %r10
	movl	$0, -20(%rsp)
	movl	-20(%rsp), %ebx
	cmpl	%ebx, %ecx
	jg	.L75
	jmp	.L68
.L84:
	movzbl	1(%r10), %r11d
	movl	%r11d, -24(%rsp)
	addq	$2, %r10
	movl	-16(%rsp), %ebx
	movl	-20(%rsp), %ebp
	movl	-24(%rsp), %r9d
	movl	-24(%rsp), %eax
	movl	-24(%rsp), %r11d
	movl	%edi, %r12d
	subl	%ebx, %r12d
	movslq	%r12d, %rbx
	movslq	%ebp, %rbp
	movq	(%r8,%rbx,8), %r12
	sall	$16, %r9d
	sall	$8, %eax
	leal	(%r9,%rax), %eax
	addl	%r11d, %eax
	cltq
	movq	%rax, (%r12,%rbp,8)
	movl	-20(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, -20(%rsp)
	movl	-20(%rsp), %r11d
	cmpl	%ecx, %r11d
	jge	.L68
.L75:
	movzbl	(%r10), %ebp
	movl	%ebp, -24(%rsp)
	movl	-16(%rsp), %ebx
	movl	-20(%rsp), %r11d
	movl	-24(%rsp), %r9d
	movl	-24(%rsp), %eax
	movl	-24(%rsp), %ebp
	movl	%edi, %r12d
	subl	%ebx, %r12d
	movslq	%r12d, %rbx
	movslq	%r11d, %r11
	movq	(%r8,%rbx,8), %r12
	sall	$16, %r9d
	sall	$8, %eax
	leal	(%r9,%rax), %eax
	addl	%ebp, %eax
	cltq
	movq	%rax, (%r12,%r11,8)
	movl	-20(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, -20(%rsp)
	movl	-20(%rsp), %ebp
	cmpl	%ecx, %ebp
	jl	.L84
.L68:
	movl	-16(%rsp), %eax
	addl	$1, %eax
	movl	%eax, -16(%rsp)
	movl	-16(%rsp), %r10d
	cmpl	%esi, %r10d
	jl	.L74
.L65:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L66:
	.cfi_restore_state
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %r9d
	cmpl	%r9d, %esi
	jle	.L65
.L76:
	movl	-16(%rsp), %eax
	cltq
	movq	(%rdx,%rax,8), %rax
	movl	$0, -20(%rsp)
	movl	-20(%rsp), %r11d
	cmpl	%r11d, %ecx
	jg	.L77
	jmp	.L71
.L85:
	movzbl	3(%rax), %ebx
	sall	$16, %ebx
	movl	%ebx, -24(%rsp)
	movl	-24(%rsp), %ebp
	movzbl	1(%r10), %r9d
	sall	$8, %r9d
	addl	%ebp, %r9d
	movl	%r9d, -24(%rsp)
	movl	-16(%rsp), %r12d
	movl	-20(%rsp), %ebp
	movl	-24(%rsp), %r9d
	movl	%edi, %r11d
	subl	%r12d, %r11d
	movslq	%r11d, %r12
	movslq	%ebp, %rbp
	movq	(%r8,%r12,8), %r12
	movzbl	2(%r10), %r10d
	leal	(%r10,%r9), %r11d
	movslq	%r11d, %rbx
	movq	%rbx, (%r12,%rbp,8)
	addq	$6, %rax
	movl	-20(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, -20(%rsp)
	movl	-20(%rsp), %r10d
	cmpl	%r10d, %ecx
	jle	.L71
.L77:
	movzbl	(%rax), %r10d
	sall	$16, %r10d
	movl	%r10d, -24(%rsp)
	movl	-24(%rsp), %ebp
	movzbl	1(%rax), %r12d
	sall	$8, %r12d
	addl	%ebp, %r12d
	movl	%r12d, -24(%rsp)
	movl	-16(%rsp), %ebx
	movl	-20(%rsp), %r10d
	movl	-24(%rsp), %r12d
	movl	%edi, %r11d
	subl	%ebx, %r11d
	movslq	%r11d, %rbp
	movslq	%r10d, %r10
	movq	(%r8,%rbp,8), %rbp
	movzbl	2(%rax), %ebx
	leal	(%rbx,%r12), %r11d
	movslq	%r11d, %r9
	movq	%r9, 0(%rbp,%r10,8)
	leaq	3(%rax), %r10
	movl	-20(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, -20(%rsp)
	movl	-20(%rsp), %r12d
	cmpl	%r12d, %ecx
	jg	.L85
.L71:
	movl	-16(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %esi
	jg	.L76
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE30:
	.size	set_rows_array, .-set_rows_array
	.section	.rodata.str1.1
.LC2:
	.string	"slow"
	.text
.globl set_rows
	.type	set_rows, @function
set_rows:
.LFB31:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	pushq	%r12
	.cfi_def_cfa_offset 24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	movl	%edi, %ebx
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movl	%edx, %ebp
	movq	%rcx, %r13
	movq	(%rsi), %r12
	movl	$.LC2, %edi
	call	puts
	movl	$0, 12(%rsp)
	movl	12(%rsp), %eax
	cmpl	%eax, %ebp
	jle	.L86
	movslq	%ebx, %rbx
	movq	0(%r13), %rdx
	movq	(%rdx,%rbx,8), %rbx
	jmp	.L88
.L91:
	movzbl	3(%r12), %esi
	movl	%esi, 8(%rsp)
	movzbl	1(%rcx), %edx
	movl	%edx, 4(%rsp)
	movzbl	2(%rcx), %ecx
	movl	%ecx, (%rsp)
	addq	$6, %r12
	movl	12(%rsp), %r13d
	movl	(%rsp), %r9d
	movl	4(%rsp), %r10d
	movl	8(%rsp), %r11d
	movslq	%r13d, %r8
	sall	$16, %r11d
	sall	$8, %r10d
	leal	(%r11,%r10), %eax
	addl	%r9d, %eax
	cltq
	movq	%rax, (%rbx,%r8,8)
	movl	12(%rsp), %eax
	addl	$1, %eax
	movl	%eax, 12(%rsp)
	movl	12(%rsp), %edi
	cmpl	%ebp, %edi
	jge	.L86
.L88:
	movzbl	(%r12), %esi
	movl	%esi, 8(%rsp)
	movzbl	1(%r12), %edx
	movl	%edx, 4(%rsp)
	movzbl	2(%r12), %ecx
	movl	%ecx, (%rsp)
	leaq	3(%r12), %rcx
	movl	12(%rsp), %r13d
	movl	(%rsp), %r9d
	movl	4(%rsp), %r10d
	movl	8(%rsp), %r11d
	movslq	%r13d, %r8
	sall	$16, %r11d
	sall	$8, %r10d
	leal	(%r11,%r10), %eax
	addl	%r9d, %eax
	cltq
	movq	%rax, (%rbx,%r8,8)
	movl	12(%rsp), %edi
	addl	$1, %edi
	movl	%edi, 12(%rsp)
	movl	12(%rsp), %esi
	cmpl	%ebp, %esi
	jl	.L91
.L86:
	addq	$24, %rsp
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
	.cfi_endproc
.LFE31:
	.size	set_rows, .-set_rows
.globl set_pngrow_pointers
	.type	set_pngrow_pointers, @function
set_pngrow_pointers:
.LFB32:
	.cfi_startproc
	movl	%esi, -40(%rsp)
	movl	$0, -24(%rsp)
	movl	-24(%rsp), %eax
	cmpl	%eax, %esi
	jle	.L92
.L97:
	movl	-40(%rsp), %r9d
	subl	$1, %r9d
	movl	%r9d, -40(%rsp)
	movl	-24(%rsp), %eax
	cltq
	movq	(%rdx,%rax,8), %rax
	movl	$0, -28(%rsp)
	movl	-28(%rsp), %r8d
	cmpl	%r8d, %edi
	jle	.L94
.L98:
	movl	-28(%rsp), %r8d
	movl	-40(%rsp), %r9d
	movl	-28(%rsp), %r10d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %r11
	movslq	%r9d, %r9
	movslq	%r10d, %r8
	movq	(%rcx,%r9,8), %r9
	movq	(%r9,%r8,8), %r10
	movb	%r10b, 2(%rax,%r11)
	movl	-28(%rsp), %r8d
	movl	-40(%rsp), %r9d
	movl	-28(%rsp), %r10d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %r11
	movslq	%r9d, %r9
	movslq	%r10d, %r10
	movq	(%rcx,%r9,8), %r8
	movzbl	1(%r8,%r10,8), %r10d
	movb	%r10b, 1(%rax,%r11)
	movl	-28(%rsp), %r9d
	movl	-40(%rsp), %r10d
	movl	-28(%rsp), %r11d
	leal	(%r9,%r9,2), %r8d
	movslq	%r8d, %r8
	movslq	%r10d, %r9
	movslq	%r11d, %r10
	movq	(%rcx,%r9,8), %r11
	movzbl	2(%r11,%r10,8), %r9d
	movb	%r9b, (%rax,%r8)
	movl	-28(%rsp), %r11d
	addl	$1, %r11d
	movl	%r11d, -28(%rsp)
	movl	-28(%rsp), %r10d
	cmpl	%edi, %r10d
	jl	.L98
.L94:
	movl	-24(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -24(%rsp)
	movl	-24(%rsp), %eax
	cmpl	%eax, %esi
	jg	.L97
.L92:
	ret
	.cfi_endproc
.LFE32:
	.size	set_pngrow_pointers, .-set_pngrow_pointers
.globl set_pngrows_array
	.type	set_pngrows_array, @function
set_pngrows_array:
.LFB33:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movl	%esi, -32(%rsp)
	cmpl	$4, %r8d
	je	.L132
	.cfi_offset 3, -16
	cmpl	$3, %r8d
	je	.L133
	cmpl	$1, %r8d
	je	.L134
.L101:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	.p2align 4,,2
	ret
.L132:
	.cfi_restore_state
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %esi
	jle	.L101
.L114:
	movl	-32(%rsp), %r8d
	subl	$1, %r8d
	movl	%r8d, -32(%rsp)
	movl	-16(%rsp), %eax
	cltq
	movq	(%rdx,%rax,8), %rax
	movl	$0, -20(%rsp)
	movl	-20(%rsp), %ebx
	cmpl	%ebx, %edi
	jg	.L115
	jmp	.L104
.L129:
	movl	-20(%rsp), %r11d
	sall	$2, %r11d
	movslq	%r11d, %r8
	movzbl	(%rax,%r8), %r9d
	sall	$16, %r9d
	movl	%r9d, -24(%rsp)
	movl	-20(%rsp), %ebx
	movl	-24(%rsp), %r11d
	sall	$2, %ebx
	movslq	%ebx, %r10
	movzbl	1(%rax,%r10), %r8d
	sall	$8, %r8d
	addl	%r11d, %r8d
	movl	%r8d, -24(%rsp)
	movl	-20(%rsp), %r9d
	movl	-24(%rsp), %r11d
	sall	$2, %r9d
	movslq	%r9d, %rbx
	movzbl	2(%rax,%rbx), %r10d
	leal	(%r10,%r11), %r8d
	movl	%r8d, -24(%rsp)
	movl	-32(%rsp), %r9d
	movl	-20(%rsp), %ebx
	movl	-24(%rsp), %r11d
	movslq	%r9d, %r10
	movslq	%ebx, %r9
	movq	(%rcx,%r10,8), %rbx
	movslq	%r11d, %r8
	movq	%r8, (%rbx,%r9,8)
	movl	-20(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -20(%rsp)
	movl	-20(%rsp), %r11d
	cmpl	%edi, %r11d
	jge	.L104
.L115:
	movl	-20(%rsp), %r10d
	sall	$2, %r10d
	movslq	%r10d, %r8
	movzbl	(%rax,%r8), %ebx
	sall	$16, %ebx
	movl	%ebx, -24(%rsp)
	movl	-20(%rsp), %r9d
	movl	-24(%rsp), %r10d
	sall	$2, %r9d
	movslq	%r9d, %r11
	movzbl	1(%rax,%r11), %r8d
	sall	$8, %r8d
	addl	%r10d, %r8d
	movl	%r8d, -24(%rsp)
	movl	-20(%rsp), %ebx
	movl	-24(%rsp), %r10d
	sall	$2, %ebx
	movslq	%ebx, %r9
	movzbl	2(%rax,%r9), %r11d
	leal	(%r11,%r10), %r8d
	movl	%r8d, -24(%rsp)
	movl	-32(%rsp), %ebx
	movl	-20(%rsp), %r9d
	movl	-24(%rsp), %r10d
	movslq	%ebx, %r11
	movslq	%r9d, %rbx
	movq	(%rcx,%r11,8), %r11
	movslq	%r10d, %r8
	movq	%r8, (%r11,%rbx,8)
	movl	-20(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -20(%rsp)
	movl	-20(%rsp), %r9d
	cmpl	%edi, %r9d
	jl	.L129
.L104:
	movl	-16(%rsp), %r11d
	addl	$1, %r11d
	movl	%r11d, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %esi
	jg	.L114
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L133:
	.cfi_restore_state
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %r9d
	cmpl	%r9d, %esi
	jle	.L101
.L116:
	movl	-32(%rsp), %r8d
	subl	$1, %r8d
	movl	%r8d, -32(%rsp)
	movl	-16(%rsp), %eax
	cltq
	movq	(%rdx,%rax,8), %rax
	movl	$0, -20(%rsp)
	movl	-20(%rsp), %ebx
	cmpl	%ebx, %edi
	jg	.L117
	jmp	.L108
.L130:
	movl	-20(%rsp), %r8d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %rbx
	movzbl	(%rax,%rbx), %r10d
	sall	$16, %r10d
	movl	%r10d, -24(%rsp)
	movl	-20(%rsp), %r9d
	movl	-24(%rsp), %ebx
	leal	(%r9,%r9,2), %r8d
	movslq	%r8d, %r11
	movzbl	1(%rax,%r11), %r10d
	sall	$8, %r10d
	addl	%ebx, %r10d
	movl	%r10d, -24(%rsp)
	movl	-20(%rsp), %r9d
	movl	-24(%rsp), %r10d
	leal	(%r9,%r9,2), %r8d
	movslq	%r8d, %r11
	movzbl	2(%rax,%r11), %ebx
	leal	(%rbx,%r10), %r9d
	movl	%r9d, -24(%rsp)
	movl	-32(%rsp), %r8d
	movl	-20(%rsp), %r11d
	movl	-24(%rsp), %r10d
	movslq	%r8d, %rbx
	movslq	%r11d, %r8
	movq	(%rcx,%rbx,8), %r11
	movslq	%r10d, %r9
	movq	%r9, (%r11,%r8,8)
	movl	-20(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, -20(%rsp)
	movl	-20(%rsp), %r10d
	cmpl	%r10d, %edi
	jle	.L108
.L117:
	movl	-20(%rsp), %ebx
	leal	(%rbx,%rbx,2), %r11d
	movslq	%r11d, %r10
	movzbl	(%rax,%r10), %r8d
	sall	$16, %r8d
	movl	%r8d, -24(%rsp)
	movl	-20(%rsp), %r9d
	movl	-24(%rsp), %r10d
	leal	(%r9,%r9,2), %ebx
	movslq	%ebx, %r11
	movzbl	1(%rax,%r11), %r8d
	sall	$8, %r8d
	addl	%r10d, %r8d
	movl	%r8d, -24(%rsp)
	movl	-20(%rsp), %r9d
	movl	-24(%rsp), %r10d
	leal	(%r9,%r9,2), %ebx
	movslq	%ebx, %r11
	movzbl	2(%rax,%r11), %r8d
	addl	%r10d, %r8d
	movl	%r8d, -24(%rsp)
	movl	-32(%rsp), %r9d
	movl	-20(%rsp), %ebx
	movl	-24(%rsp), %r10d
	movslq	%r9d, %r11
	movslq	%ebx, %r9
	movq	(%rcx,%r11,8), %rbx
	movslq	%r10d, %r8
	movq	%r8, (%rbx,%r9,8)
	movl	-20(%rsp), %r11d
	addl	$1, %r11d
	movl	%r11d, -20(%rsp)
	movl	-20(%rsp), %r10d
	cmpl	%r10d, %edi
	jg	.L130
.L108:
	movl	-16(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %esi
	jg	.L116
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L134:
	.cfi_restore_state
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %r8d
	cmpl	%r8d, %esi
	jle	.L101
.L118:
	movl	-32(%rsp), %r11d
	subl	$1, %r11d
	movl	%r11d, -32(%rsp)
	movl	-16(%rsp), %eax
	cltq
	movq	(%rdx,%rax,8), %r9
	movl	$0, -20(%rsp)
	movl	-20(%rsp), %r10d
	cmpl	%r10d, %edi
	jg	.L119
	jmp	.L111
.L131:
	movl	-20(%rsp), %eax
	cltq
	movzbl	(%r9,%rax), %r11d
	movl	%r11d, -24(%rsp)
	movl	-32(%rsp), %ebx
	movl	-20(%rsp), %r10d
	movl	-24(%rsp), %r8d
	movl	-24(%rsp), %eax
	movl	-24(%rsp), %r11d
	movslq	%ebx, %rbx
	movslq	%r10d, %r10
	movq	(%rcx,%rbx,8), %rbx
	sall	$16, %r8d
	sall	$8, %eax
	leal	(%r8,%rax), %eax
	addl	%r11d, %eax
	cltq
	movq	%rax, (%rbx,%r10,8)
	movl	-20(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -20(%rsp)
	movl	-20(%rsp), %eax
	cmpl	%eax, %edi
	jle	.L111
.L119:
	movl	-20(%rsp), %eax
	cltq
	movzbl	(%r9,%rax), %ebx
	movl	%ebx, -24(%rsp)
	movl	-32(%rsp), %r10d
	movl	-20(%rsp), %r11d
	movl	-24(%rsp), %r8d
	movl	-24(%rsp), %eax
	movl	-24(%rsp), %ebx
	movslq	%r10d, %r10
	movslq	%r11d, %r11
	movq	(%rcx,%r10,8), %r10
	sall	$16, %r8d
	sall	$8, %eax
	leal	(%r8,%rax), %eax
	addl	%ebx, %eax
	cltq
	movq	%rax, (%r10,%r11,8)
	movl	-20(%rsp), %r8d
	addl	$1, %r8d
	movl	%r8d, -20(%rsp)
	movl	-20(%rsp), %ebx
	cmpl	%ebx, %edi
	jg	.L131
.L111:
	movl	-16(%rsp), %r8d
	addl	$1, %r8d
	movl	%r8d, -16(%rsp)
	movl	-16(%rsp), %r9d
	cmpl	%r9d, %esi
	jg	.L118
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE33:
	.size	set_pngrows_array, .-set_pngrows_array
.globl init_bmp
	.type	init_bmp, @function
init_bmp:
.LFB34:
	.cfi_startproc
	movb	$66, (%rdi)
	movb	$77, 1(%rdi)
	movl	$0, 4(%rdi)
	movl	$0, 8(%rdi)
	movl	$56, 12(%rdi)
	movl	$32, 24(%rdi)
	movl	$32, 20(%rdi)
	movl	$40, 16(%rdi)
	movw	$24, 30(%rdi)
	movw	$1, 28(%rdi)
	movl	$0, 32(%rdi)
	movl	$3072, 36(%rdi)
	movl	$0, 40(%rdi)
	movl	$0, 44(%rdi)
	movl	$0, 48(%rdi)
	movl	$0, 52(%rdi)
	ret
	.cfi_endproc
.LFE34:
	.size	init_bmp, .-init_bmp
	.section	.rodata.str1.1
.LC3:
	.string	"load_bmp: cannot open: %s\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC4:
	.string	"load_bmp: warning: mmap file failed, using normal buffered I/O: %s\n"
	.section	.rodata.str1.1
.LC5:
	.string	"%c%c"
	.section	.rodata.str1.8
	.align 8
.LC6:
	.string	"load_bmp: file %s is truncated, skipping\n"
	.align 8
.LC7:
	.string	"load_bmp: file %s doesn't start with BM - not recognized as a bitmap\n"
	.align 8
.LC8:
	.string	"Only 8,24,32 BPP BMPs suported: current bpp: %d\n"
	.section	.rodata.str1.1
.LC9:
	.string	"%c"
.LC10:
	.string	"%c%c%c"
.LC11:
	.string	"%c%c%c%c"
.LC12:
	.string	"rb"
	.text
.globl load_bmp_file
	.type	load_bmp_file, @function
load_bmp_file:
.LFB35:
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
	subq	$360, %rsp
	.cfi_def_cfa_offset 416
	movq	%rdi, %r12
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%rsi, %r14
	movq	%rdx, %r13
	movq	%rcx, %rbx
	movl	ncache(%rip), %eax
	movl	%eax, 32(%rsp)
	testl	%eax, %eax
	jle	.L137
	movq	icache(%rip), %rbp
	movl	%eax, %edx
	subl	$1, %edx
	andl	$7, %edx
	movq	0(%rbp), %rsi
	movq	%rcx, %rdi
	movl	%edx, 8(%rsp)
	call	strcmp
	testl	%eax, %eax
	movl	8(%rsp), %edx
	je	.L308
	movl	$1, %r15d
	addq	$24, %rbp
	cmpl	32(%rsp), %r15d
	jge	.L137
	testl	%edx, %edx
	je	.L328
	cmpl	$1, %edx
	je	.L302
	cmpl	$2, %edx
	.p2align 4,,3
	je	.L303
	cmpl	$3, %edx
	.p2align 4,,2
	je	.L304
	cmpl	$4, %edx
	.p2align 4,,2
	je	.L305
	cmpl	$5, %edx
	.p2align 4,,2
	je	.L306
	cmpl	$6, %edx
	.p2align 4,,2
	je	.L307
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L308
	addl	$1, %r15d
	addq	$24, %rbp
.L307:
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L308
	addl	$1, %r15d
	addq	$24, %rbp
.L306:
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L308
	addl	$1, %r15d
	addq	$24, %rbp
.L305:
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L308
	addl	$1, %r15d
	addq	$24, %rbp
.L304:
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L308
	addl	$1, %r15d
	addq	$24, %rbp
.L303:
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L308
	addl	$1, %r15d
	addq	$24, %rbp
.L302:
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L308
	addl	$1, %r15d
	addq	$24, %rbp
	cmpl	32(%rsp), %r15d
	jge	.L137
.L328:
	movq	%r13, 40(%rsp)
	movq	%r12, %r13
	movl	%r15d, %r12d
.L140:
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	leaq	24(%rbp), %r15
	movq	%r15, %rbp
	movq	(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	addq	$24, %rbp
	movq	0(%rbp), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	leaq	48(%r15), %rbp
	movq	48(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	leaq	72(%r15), %rbp
	movq	72(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	leaq	96(%r15), %rbp
	movq	96(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	leaq	120(%r15), %rbp
	movq	120(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	leaq	144(%r15), %rbp
	movq	144(%r15), %rsi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L334
	addl	$8, %r12d
	leaq	168(%r15), %rbp
	cmpl	32(%rsp), %r12d
	jl	.L140
	movq	%r13, %r12
	movq	40(%rsp), %r13
.L137:
	movl	$.LC12, %esi
	movq	%rbx, %rdi
	call	fopen
	movq	%rax, %rbp
	testq	%rax, %rax
	je	.L335
	movq	%rbp, %rdi
	call	fileno
	movl	%eax, %r15d
	leaq	96(%rsp), %rsi
	movl	%eax, %edi
	call	__fstat50
	testl	%eax, %eax
	js	.L336
	xorl	%r9d, %r9d
	movl	%r15d, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	movq	208(%rsp), %rsi
	xorl	%edi, %edi
	call	mmap
	movq	%rax, %r15
	cmpq	$-1, %rax
	je	.L337
	movq	208(%rsp), %rsi
	movq	%rsi, 32(%rsp)
.L144:
	movb	$66, 256(%rsp)
	movb	$77, 257(%rsp)
	movl	$0, 260(%rsp)
	movl	$0, 264(%rsp)
	movl	$56, 268(%rsp)
	movl	$32, 280(%rsp)
	movl	$32, 276(%rsp)
	movl	$40, 272(%rsp)
	movw	$24, 286(%rsp)
	movw	$1, 284(%rsp)
	movl	$0, 288(%rsp)
	movl	$3072, 292(%rsp)
	movl	$0, 296(%rsp)
	movl	$0, 300(%rsp)
	movl	$0, 304(%rsp)
	movl	$0, 308(%rsp)
	leaq	320(%rsp), %rcx
	leaq	336(%rsp), %rdx
	movl	$.LC5, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	cmpl	$2, %eax
	je	.L145
	movq	%rbx, %rsi
	movl	$.LC6, %edi
	xorl	%eax, %eax
	call	printf
	testq	%r15, %r15
	je	.L146
	movq	32(%rsp), %rsi
	movq	%r15, %rdi
	call	munmap
.L146:
	movq	%rbp, %rdi
	call	fclose
	movl	$2, %eax
.L139:
	addq	$360, %rsp
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
.L334:
	.cfi_restore_state
	movq	%r13, %r12
	movq	40(%rsp), %r13
.L308:
	movq	8(%rbp), %rbx
	movq	%rbx, (%r12)
	movl	16(%rbp), %ecx
	movl	%ecx, (%r14)
	movl	20(%rbp), %eax
	movl	%eax, 0(%r13)
	xorl	%eax, %eax
	jmp	.L139
.L145:
	cmpb	$66, 336(%rsp)
	je	.L338
.L147:
	movq	%rbx, %rsi
	movl	$.LC7, %edi
	xorl	%eax, %eax
	call	printf
	testq	%r15, %r15
	je	.L149
	movq	32(%rsp), %rsi
	movq	%r15, %rdi
	call	munmap
.L149:
	movq	%rbp, %rdi
	call	fclose
	movl	$3, %eax
	jmp	.L139
.L336:
	movl	$.LC0, %edi
	call	perror
.L142:
	movq	%rbx, %rsi
	movl	$.LC4, %edi
	xorl	%eax, %eax
	call	printf
	xorl	%r15d, %r15d
	movq	$0, 32(%rsp)
	jmp	.L144
.L335:
	movq	%rbx, %rsi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movl	$1, %eax
	jmp	.L139
.L337:
	movl	$.LC1, %edi
	call	perror
	jmp	.L142
.L338:
	cmpb	$77, 320(%rsp)
	jne	.L147
	leaq	256(%rsp), %rdi
	addq	$4, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	256(%rsp), %rdi
	addq	$8, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	256(%rsp), %rdi
	addq	$12, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	256(%rsp), %rdi
	addq	$16, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	256(%rsp), %rdi
	addq	$20, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	256(%rsp), %rdi
	addq	$24, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	256(%rsp), %rdi
	addq	$28, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	leaq	256(%rsp), %rdi
	addq	$30, %rdi
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movswl	286(%rsp), %edi
	movl	%edi, 48(%rsp)
	cmpl	$8, %edi
	je	.L150
	cmpl	$24, %edi
	je	.L150
	cmpl	$32, %edi
	je	.L150
	movl	%edi, %esi
	movl	$.LC8, %edi
	xorl	%eax, %eax
	call	printf
	testq	%r15, %r15
	je	.L151
	movq	32(%rsp), %rsi
	movq	%r15, %rdi
	call	munmap
.L151:
	movq	%rbp, %rdi
	call	fclose
	movl	$4, %eax
	jmp	.L139
.L150:
	movslq	268(%rsp), %rsi
	xorl	%edx, %edx
	movq	%rbp, %rdi
	call	fseek
	movl	276(%rsp), %r8d
	movl	%r8d, 40(%rsp)
	movl	280(%rsp), %r9d
	movl	%r9d, 68(%rsp)
	movslq	%r9d, %rdi
	movl	$8, %esi
	call	calloc
	movq	%rax, (%r12)
	cmpl	$0, 68(%rsp)
	jle	.L152
	movslq	40(%rsp), %r10
	movq	%r10, 56(%rsp)
	movl	68(%rsp), %r11d
	subl	$1, %r11d
	andl	$7, %r11d
	movl	$8, %esi
	movq	%r10, %rdi
	movl	%r11d, 24(%rsp)
	movq	%rax, 16(%rsp)
	call	calloc
	movq	16(%rsp), %r8
	movq	%rax, (%r8)
	movl	$1, %edx
	leaq	8(%r8), %r9
	cmpl	$1, 68(%rsp)
	movl	24(%rsp), %ecx
	jle	.L152
	testl	%ecx, %ecx
	je	.L326
	cmpl	$1, %ecx
	je	.L295
	cmpl	$2, %ecx
	.p2align 4,,3
	je	.L296
	cmpl	$3, %ecx
	.p2align 4,,2
	je	.L297
	cmpl	$4, %ecx
	.p2align 4,,2
	je	.L298
	cmpl	$5, %ecx
	.p2align 4,,2
	je	.L299
	cmpl	$6, %ecx
	.p2align 4,,2
	je	.L300
	movl	$8, %esi
	movq	56(%rsp), %rdi
	movq	%r8, 16(%rsp)
	movq	%r9, 24(%rsp)
	call	calloc
	movq	24(%rsp), %r8
	movq	%rax, (%r8)
	movl	$2, %edx
	movq	16(%rsp), %rdi
	leaq	16(%rdi), %r9
.L300:
	movl	$8, %esi
	movq	56(%rsp), %rdi
	movl	%edx, 8(%rsp)
	movq	%r9, 24(%rsp)
	call	calloc
	movq	24(%rsp), %r9
	movq	%rax, (%r9)
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %r9
.L299:
	movl	$8, %esi
	movq	56(%rsp), %rdi
	movl	%edx, 8(%rsp)
	movq	%r9, 24(%rsp)
	call	calloc
	movq	24(%rsp), %r9
	movq	%rax, (%r9)
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %r9
.L298:
	movl	$8, %esi
	movq	56(%rsp), %rdi
	movl	%edx, 8(%rsp)
	movq	%r9, 24(%rsp)
	call	calloc
	movq	24(%rsp), %r9
	movq	%rax, (%r9)
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %r9
.L297:
	movl	$8, %esi
	movq	56(%rsp), %rdi
	movl	%edx, 8(%rsp)
	movq	%r9, 24(%rsp)
	call	calloc
	movq	24(%rsp), %r9
	movq	%rax, (%r9)
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %r9
.L296:
	movl	$8, %esi
	movq	56(%rsp), %rdi
	movl	%edx, 8(%rsp)
	movq	%r9, 24(%rsp)
	call	calloc
	movq	24(%rsp), %r9
	movq	%rax, (%r9)
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %r9
.L295:
	movl	$8, %esi
	movq	56(%rsp), %rdi
	movl	%edx, 8(%rsp)
	movq	%r9, 24(%rsp)
	call	calloc
	movq	24(%rsp), %r9
	movq	%rax, (%r9)
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %r9
	cmpl	%edx, 68(%rsp)
	jle	.L152
.L326:
	movq	%r13, %rcx
	movq	%r12, %r13
	movq	%rbp, %r12
	movq	56(%rsp), %rbp
	movq	%rbx, %r8
	movq	%r9, %rbx
.L153:
	movl	$8, %esi
	movq	%rbp, %rdi
	movl	%edx, 8(%rsp)
	movq	%rcx, 24(%rsp)
	movq	%r8, 16(%rsp)
	call	calloc
	movq	%rax, (%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 8(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 16(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 24(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 32(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 40(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 48(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 56(%rbx)
	movl	8(%rsp), %edx
	addl	$8, %edx
	addq	$64, %rbx
	cmpl	%edx, 68(%rsp)
	movq	24(%rsp), %rcx
	movq	16(%rsp), %r8
	jg	.L153
	movq	%r12, %rbp
	movq	%r13, %r12
	movq	%rcx, %r13
	movq	%r8, %rbx
.L152:
	testq	%r15, %r15
	je	.L154
	movq	%rbp, %rdi
	call	fclose
	movslq	268(%rsp), %rdi
	leaq	(%r15,%rdi), %rax
	cmpl	$8, 48(%rsp)
	je	.L339
	cmpl	$24, 48(%rsp)
	je	.L340
	cmpl	$32, 48(%rsp)
	je	.L341
.L158:
	movl	40(%rsp), %edi
	movl	%edi, (%r14)
	movl	68(%rsp), %ebp
	movl	%ebp, 0(%r13)
	testq	%r15, %r15
	je	.L179
	movq	32(%rsp), %rsi
	movq	%r15, %rdi
	call	munmap
.L179:
	movl	0(%r13), %ecx
	movl	(%r14), %edx
	movq	(%r12), %rsi
	movq	%rbx, %rdi
	call	insert_cache
	xorl	%eax, %eax
	jmp	.L139
.L341:
	xorl	%ebp, %ebp
	xorl	%r10d, %r10d
	movl	40(%rsp), %r9d
	subl	$1, %r9d
	leaq	4(,%r9,4), %rsi
.L164:
	cmpl	68(%rsp), %r10d
	jge	.L158
	cmpl	$0, 40(%rsp)
	jle	.L166
	movq	%rbp, %rcx
	addq	(%r12), %rcx
	leaq	-4(%rsi), %rdi
	shrq	$2, %rdi
	andl	$3, %edi
	movzbl	1(%rax), %r9d
	movb	%r9b, 319(%rsp)
	movzbl	2(%rax), %edx
	movb	%dl, 320(%rsp)
	movzbl	3(%rax), %r8d
	movb	%r8b, 336(%rsp)
	movq	(%rcx), %r11
	movzbl	%r8b, %r8d
	sall	$16, %r8d
	movzbl	%dl, %edx
	sall	$8, %edx
	leal	(%r8,%rdx), %r8d
	movzbl	%r9b, %r9d
	addl	%r9d, %r8d
	movslq	%r8d, %rdx
	movq	%rdx, (%r11)
	movl	$4, %edx
	cmpq	%rsi, %rdx
	je	.L311
	testq	%rdi, %rdi
	je	.L322
	cmpq	$1, %rdi
	je	.L291
	cmpq	$2, %rdi
	je	.L292
	movzbl	5(%rax), %r8d
	movb	%r8b, 319(%rsp)
	movzbl	6(%rax), %r11d
	movb	%r11b, 320(%rsp)
	movzbl	7(%rax), %edx
	movb	%dl, 336(%rsp)
	movq	(%rcx), %r9
	movzbl	%dl, %edi
	sall	$16, %edi
	movzbl	%r11b, %edx
	sall	$8, %edx
	leal	(%rdi,%rdx), %r11d
	movzbl	%r8b, %r8d
	addl	%r8d, %r11d
	movslq	%r11d, %rdi
	movq	%rdi, 8(%r9)
	movl	$8, %edx
.L292:
	movzbl	1(%rax,%rdx), %r9d
	movb	%r9b, 319(%rsp)
	movzbl	2(%rax,%rdx), %edi
	movb	%dil, 320(%rsp)
	movzbl	3(%rax,%rdx), %r8d
	movb	%r8b, 336(%rsp)
	movq	(%rcx), %r11
	movzbl	%r8b, %r8d
	sall	$16, %r8d
	movzbl	%dil, %edi
	sall	$8, %edi
	leal	(%r8,%rdi), %r8d
	movzbl	%r9b, %edi
	addl	%edi, %r8d
	movslq	%r8d, %r9
	movq	%r9, (%r11,%rdx,2)
	addq	$4, %rdx
.L291:
	movzbl	1(%rax,%rdx), %r9d
	movb	%r9b, 319(%rsp)
	movzbl	2(%rax,%rdx), %edi
	movb	%dil, 320(%rsp)
	movzbl	3(%rax,%rdx), %r8d
	movb	%r8b, 336(%rsp)
	movq	(%rcx), %r11
	movzbl	%r8b, %r8d
	sall	$16, %r8d
	movzbl	%dil, %edi
	sall	$8, %edi
	leal	(%r8,%rdi), %edi
	movzbl	%r9b, %r8d
	addl	%r8d, %edi
	movslq	%edi, %r9
	movq	%r9, (%r11,%rdx,2)
	addq	$4, %rdx
	cmpq	%rsi, %rdx
	je	.L311
.L322:
	movq	%rbx, 56(%rsp)
.L165:
	movzbl	1(%rax,%rdx), %r11d
	movb	%r11b, 319(%rsp)
	movzbl	2(%rax,%rdx), %r9d
	movb	%r9b, 320(%rsp)
	movzbl	3(%rax,%rdx), %edi
	movb	%dil, 336(%rsp)
	movq	(%rcx), %rbx
	movzbl	%dil, %r8d
	sall	$16, %r8d
	movzbl	%r9b, %edi
	sall	$8, %edi
	leal	(%r8,%rdi), %r8d
	movzbl	%r11b, %r9d
	addl	%r9d, %r8d
	movslq	%r8d, %rdi
	movq	%rdi, (%rbx,%rdx,2)
	leaq	4(%rdx), %rdi
	movzbl	1(%rax,%rdi), %r11d
	movb	%r11b, 48(%rsp)
	movb	%r11b, 319(%rsp)
	movzbl	2(%rax,%rdi), %ebx
	movb	%bl, 320(%rsp)
	movzbl	3(%rax,%rdi), %r8d
	movb	%r8b, 336(%rsp)
	movzbl	%r8b, %r9d
	sall	$16, %r9d
	movzbl	%bl, %r8d
	sall	$8, %r8d
	leal	(%r9,%r8), %r8d
	movzbl	%r11b, %ebx
	addl	%ebx, %r8d
	movslq	%r8d, %r11
	movq	(%rcx), %r9
	movq	%r11, (%r9,%rdi,2)
	leaq	8(%rdx), %rdi
	movzbl	1(%rax,%rdi), %ebx
	movb	%bl, 48(%rsp)
	movb	%bl, 319(%rsp)
	movzbl	2(%rax,%rdi), %r11d
	movb	%r11b, 320(%rsp)
	movzbl	3(%rax,%rdi), %r8d
	movb	%r8b, 336(%rsp)
	movzbl	%r8b, %r9d
	sall	$16, %r9d
	movzbl	%r11b, %r8d
	sall	$8, %r8d
	leal	(%r9,%r8), %r8d
	movzbl	%bl, %r11d
	addl	%r11d, %r8d
	movslq	%r8d, %r8
	movq	(%rcx), %r9
	movq	%r8, (%r9,%rdi,2)
	leaq	12(%rdx), %rdi
	movzbl	1(%rax,%rdi), %ebx
	movb	%bl, 48(%rsp)
	movb	%bl, 319(%rsp)
	movzbl	2(%rax,%rdi), %r8d
	movb	%r8b, 320(%rsp)
	movzbl	3(%rax,%rdi), %r11d
	movb	%r11b, 336(%rsp)
	movzbl	%r11b, %r9d
	sall	$16, %r9d
	movzbl	%r8b, %r11d
	sall	$8, %r11d
	leal	(%r9,%r11), %r8d
	movzbl	%bl, %r9d
	addl	%r9d, %r8d
	movslq	%r8d, %rbx
	movq	(%rcx), %r11
	movq	%rbx, (%r11,%rdi,2)
	addq	$16, %rdx
	cmpq	%rsi, %rdx
	jne	.L165
	movq	56(%rsp), %rbx
.L311:
	addq	%rdx, %rax
.L166:
	addl	$1, %r10d
	addq	$8, %rbp
	jmp	.L164
.L340:
	xorl	%r9d, %r9d
	xorl	%r11d, %r11d
	movl	40(%rsp), %esi
	subl	$1, %esi
	addq	$1, %rsi
	leaq	0(,%rsi,8), %rdi
	leaq	(%rsi,%rsi,2), %rcx
	movq	%rcx, 72(%rsp)
.L161:
	cmpl	68(%rsp), %r11d
	jge	.L158
	cmpl	$0, 40(%rsp)
	jle	.L163
	movq	%r9, %rsi
	addq	(%r12), %rsi
	leaq	-8(%rdi), %rbp
	shrq	$3, %rbp
	andl	$1, %ebp
	movzbl	(%rax), %r8d
	movb	%r8b, 48(%rsp)
	movb	%r8b, 319(%rsp)
	movzbl	1(%rax), %ecx
	movb	%cl, 56(%rsp)
	movl	%ecx, %edx
	movb	%cl, 320(%rsp)
	movzbl	2(%rax), %r10d
	movb	%r10b, 336(%rsp)
	leaq	3(%rax), %r8
	movq	%r8, %rcx
	movzbl	%r10b, %r10d
	sall	$16, %r10d
	movzbl	%dl, %edx
	sall	$8, %edx
	leal	(%r10,%rdx), %edx
	movzbl	48(%rsp), %r10d
	addl	%r10d, %edx
	movslq	%edx, %rdx
	movq	(%rsi), %r10
	movq	%rdx, (%r10)
	movl	$8, %edx
	cmpq	%rdi, %rdx
	je	.L310
	testq	%rbp, %rbp
	je	.L320
	movzbl	(%r8), %ebp
	movb	%bpl, 319(%rsp)
	movzbl	1(%r8), %r10d
	movb	%r10b, 320(%rsp)
	movzbl	2(%r8), %edx
	movb	%dl, 336(%rsp)
	leaq	6(%rax), %rcx
	movq	(%rsi), %r8
	movzbl	%dl, %edx
	sall	$16, %edx
	movzbl	%r10b, %r10d
	sall	$8, %r10d
	addl	%r10d, %edx
	movzbl	%bpl, %ebp
	addl	%ebp, %edx
	movslq	%edx, %r10
	movq	%r10, 8(%r8)
	movl	$16, %edx
	cmpq	%rdi, %rdx
	je	.L310
.L320:
	movq	%rax, 56(%rsp)
.L162:
	movzbl	(%rcx), %eax
	movb	%al, 319(%rsp)
	movzbl	1(%rcx), %ebp
	movb	%bpl, 320(%rsp)
	movzbl	2(%rcx), %r8d
	movb	%r8b, 336(%rsp)
	leaq	3(%rcx), %r10
	movzbl	%r8b, %r8d
	sall	$16, %r8d
	movzbl	%bpl, %ebp
	sall	$8, %ebp
	leal	(%r8,%rbp), %ebp
	movzbl	%al, %eax
	addl	%eax, %ebp
	movslq	%ebp, %rax
	movq	(%rsi), %r8
	movq	%rax, (%r8,%rdx)
	movzbl	3(%rcx), %eax
	movb	%al, 319(%rsp)
	movzbl	1(%r10), %r8d
	movb	%r8b, 320(%rsp)
	movzbl	2(%r10), %ebp
	movb	%bpl, 336(%rsp)
	addq	$6, %rcx
	movzbl	%bpl, %r10d
	sall	$16, %r10d
	movzbl	%r8b, %ebp
	sall	$8, %ebp
	leal	(%r10,%rbp), %r8d
	movzbl	%al, %ebp
	leal	(%r8,%rbp), %eax
	movslq	%eax, %r8
	movq	(%rsi), %r10
	movq	%r8, 8(%r10,%rdx)
	addq	$16, %rdx
	cmpq	%rdi, %rdx
	jne	.L162
	movq	56(%rsp), %rax
.L310:
	addq	72(%rsp), %rax
.L163:
	addl	$1, %r11d
	addq	$8, %r9
	jmp	.L161
.L339:
	xorl	%r8d, %r8d
	xorl	%r9d, %r9d
	movl	40(%rsp), %r10d
	subl	$1, %r10d
	addq	$1, %r10
.L156:
	cmpl	68(%rsp), %r9d
	jge	.L158
	cmpl	$0, 40(%rsp)
	jle	.L159
	movq	%r8, %rcx
	addq	(%r12), %rcx
	leaq	(%rax,%r10), %rsi
	movq	%rax, %rdx
	notq	%rdx
	leaq	(%rsi,%rdx), %rdi
	andl	$3, %edi
	movzbl	(%rax), %ebp
	movb	%bpl, 336(%rsp)
	leaq	1(%rax), %rdx
	movzbl	%bpl, %ebp
	movl	%ebp, %r11d
	sall	$16, %r11d
	movl	%r11d, 48(%rsp)
	movl	%ebp, %r11d
	sall	$8, %r11d
	movl	%r11d, 56(%rsp)
	movl	48(%rsp), %r11d
	addl	56(%rsp), %r11d
	leal	(%r11,%rbp), %ebp
	movslq	%ebp, %rbp
	movq	(%rcx), %r11
	movq	%rbp, (%r11)
	movl	$8, %ebp
	cmpq	%rsi, %rdx
	je	.L309
	testq	%rdi, %rdi
	je	.L157
	cmpq	$1, %rdi
	je	.L289
	cmpq	$2, %rdi
	je	.L290
	movzbl	(%rdx), %edi
	movb	%dil, 336(%rsp)
	leaq	2(%rax), %rdx
	movzbl	%dil, %eax
	movq	(%rcx), %rdi
	movl	%eax, %r11d
	sall	$16, %r11d
	movl	%eax, %ebp
	sall	$8, %ebp
	leal	(%r11,%rbp), %r11d
	leal	(%r11,%rax), %eax
	cltq
	movq	%rax, 8(%rdi)
	movl	$16, %ebp
.L290:
	movzbl	(%rdx), %eax
	movb	%al, 336(%rsp)
	addq	$1, %rdx
	movzbl	%al, %eax
	movl	%eax, %r11d
	sall	$16, %r11d
	movl	%eax, %edi
	sall	$8, %edi
	leal	(%r11,%rdi), %edi
	leal	(%rdi,%rax), %eax
	cltq
	movq	(%rcx), %r11
	movq	%rax, (%r11,%rbp)
	addq	$8, %rbp
.L289:
	movzbl	(%rdx), %eax
	movb	%al, 336(%rsp)
	addq	$1, %rdx
	movzbl	%al, %eax
	movl	%eax, %r11d
	sall	$16, %r11d
	movl	%eax, %edi
	sall	$8, %edi
	leal	(%r11,%rdi), %r11d
	leal	(%r11,%rax), %eax
	cltq
	movq	(%rcx), %rdi
	movq	%rax, (%rdi,%rbp)
	addq	$8, %rbp
	cmpq	%rsi, %rdx
	je	.L309
.L157:
	movzbl	(%rdx), %eax
	movb	%al, 336(%rsp)
	movzbl	%al, %eax
	movl	%eax, %r11d
	sall	$16, %r11d
	movl	%eax, %edi
	sall	$8, %edi
	leal	(%r11,%rdi), %r11d
	leal	(%r11,%rax), %eax
	cltq
	movq	(%rcx), %rdi
	movq	%rax, (%rdi,%rbp)
	movzbl	1(%rdx), %eax
	movb	%al, 336(%rsp)
	movzbl	%al, %eax
	movl	%eax, %r11d
	sall	$16, %r11d
	movl	%eax, %edi
	sall	$8, %edi
	leal	(%r11,%rdi), %r11d
	leal	(%r11,%rax), %eax
	cltq
	movq	(%rcx), %rdi
	movq	%rax, 8(%rdi,%rbp)
	movzbl	2(%rdx), %eax
	movb	%al, 336(%rsp)
	movzbl	%al, %eax
	movl	%eax, %r11d
	sall	$16, %r11d
	movl	%eax, %edi
	sall	$8, %edi
	leal	(%r11,%rdi), %r11d
	leal	(%r11,%rax), %eax
	cltq
	movq	(%rcx), %rdi
	movq	%rax, 16(%rdi,%rbp)
	movzbl	3(%rdx), %eax
	movb	%al, 336(%rsp)
	addq	$4, %rdx
	movzbl	%al, %eax
	movl	%eax, %r11d
	sall	$16, %r11d
	movl	%eax, %edi
	sall	$8, %edi
	leal	(%r11,%rdi), %edi
	leal	(%rdi,%rax), %eax
	cltq
	movq	(%rcx), %r11
	movq	%rax, 24(%r11,%rbp)
	addq	$32, %rbp
	cmpq	%rsi, %rdx
	jne	.L157
.L309:
	movq	%rsi, %rax
.L159:
	addl	$1, %r9d
	addq	$8, %r8
	jmp	.L156
.L154:
	cmpl	$8, 48(%rsp)
	je	.L342
	cmpl	$24, 48(%rsp)
	je	.L343
	cmpl	$32, 48(%rsp)
	je	.L344
.L170:
	movq	%rbp, %rdi
	call	fclose
	jmp	.L158
.L344:
	xorl	%edi, %edi
	xorl	%r10d, %r10d
	movq	%r15, 56(%rsp)
	movq	%r14, 72(%rsp)
	movq	%r13, %r14
	movq	%rbx, %r13
	movq	%rdi, %rbx
.L176:
	cmpl	68(%rsp), %r10d
	jge	.L345
	xorl	%edx, %edx
	movl	$0, 48(%rsp)
	movq	%r14, %r15
	movq	%r13, %r14
	movq	%rbp, %r13
	movq	%rbx, %rbp
	movq	%rdx, %rbx
	jmp	.L178
.L177:
	leaq	336(%rsp), %r9
	leaq	320(%rsp), %r8
	leaq	319(%rsp), %rcx
	leaq	318(%rsp), %rdx
	movl	$.LC11, %esi
	movq	%r13, %rdi
	xorl	%eax, %eax
	movl	%r10d, 24(%rsp)
	call	fscanf
	movq	(%r12), %r8
	movq	(%r8,%rbp), %r10
	movzbl	336(%rsp), %r9d
	sall	$16, %r9d
	movzbl	320(%rsp), %ecx
	sall	$8, %ecx
	leal	(%r9,%rcx), %eax
	movzbl	319(%rsp), %r11d
	addl	%r11d, %eax
	cltq
	movq	%rax, (%r10,%rbx)
	addl	$1, 48(%rsp)
	addq	$8, %rbx
	movl	24(%rsp), %r10d
.L178:
	movl	40(%rsp), %esi
	cmpl	%esi, 48(%rsp)
	jl	.L177
	movq	%rbp, %rbx
	movq	%r13, %rbp
	movq	%r14, %r13
	movq	%r15, %r14
	addl	$1, %r10d
	addq	$8, %rbx
	jmp	.L176
.L345:
	movq	56(%rsp), %r15
	movq	%r13, %rbx
	movq	%r14, %r13
	movq	72(%rsp), %r14
	jmp	.L170
.L343:
	xorl	%edi, %edi
	xorl	%r9d, %r9d
	movq	%r15, 48(%rsp)
	movq	%r14, 56(%rsp)
	movq	%r13, 72(%rsp)
	movq	%rbx, %r13
	movq	%rdi, %rbx
.L173:
	cmpl	68(%rsp), %r9d
	jge	.L346
	xorl	%r15d, %r15d
	xorl	%esi, %esi
	movq	%rbp, %r14
	movq	%r15, %rbp
	movq	%r13, %r15
	movq	%rbx, %r13
	movl	%esi, %ebx
	jmp	.L175
.L174:
	leaq	336(%rsp), %r8
	leaq	320(%rsp), %rcx
	leaq	319(%rsp), %rdx
	movl	$.LC10, %esi
	movq	%r14, %rdi
	xorl	%eax, %eax
	movl	%r9d, 24(%rsp)
	call	fscanf
	movq	(%r12), %rdx
	movq	(%rdx,%r13), %r11
	movzbl	336(%rsp), %r8d
	sall	$16, %r8d
	movzbl	320(%rsp), %r9d
	sall	$8, %r9d
	leal	(%r8,%r9), %eax
	movzbl	319(%rsp), %ecx
	addl	%ecx, %eax
	cltq
	movq	%rax, (%r11,%rbp)
	addl	$1, %ebx
	addq	$8, %rbp
	movl	24(%rsp), %r9d
.L175:
	cmpl	40(%rsp), %ebx
	jl	.L174
	movq	%r14, %rbp
	movq	%r13, %rbx
	movq	%r15, %r13
	addl	$1, %r9d
	addq	$8, %rbx
	jmp	.L173
.L346:
	movq	48(%rsp), %r15
	movq	56(%rsp), %r14
	movq	%r13, %rbx
	movq	72(%rsp), %r13
	jmp	.L170
.L342:
	xorl	%esi, %esi
	movl	$0, 48(%rsp)
	movq	%r15, 56(%rsp)
	movq	%r14, 72(%rsp)
	movq	%rsi, %r14
	movq	%r13, 80(%rsp)
	movq	%rbx, 88(%rsp)
.L168:
	movl	68(%rsp), %r13d
	cmpl	%r13d, 48(%rsp)
	jge	.L316
	cmpl	$0, 40(%rsp)
	jle	.L171
	movl	40(%rsp), %ebx
	subl	$1, %ebx
	andl	$3, %ebx
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %r15d
	movq	(%r12), %r11
	movq	(%r11,%r14), %r13
	movl	%r15d, %edi
	sall	$16, %edi
	movl	%r15d, %ecx
	sall	$8, %ecx
	leal	(%rdi,%rcx), %r10d
	leal	(%r10,%r15), %eax
	cltq
	movq	%rax, 0(%r13)
	movl	$1, %r15d
	movl	$8, %r13d
	cmpl	$1, 40(%rsp)
	jle	.L171
	testl	%ebx, %ebx
	je	.L169
	cmpl	$1, %ebx
	je	.L293
	cmpl	$2, %ebx
	.p2align 4,,3
	je	.L294
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %r15d
	movq	(%r12), %r10
	movq	(%r10,%r14), %r13
	movl	%r15d, %esi
	sall	$16, %esi
	movl	%r15d, %ecx
	sall	$8, %ecx
	leal	(%rsi,%rcx), %ebx
	leal	(%rbx,%r15), %eax
	cltq
	movq	%rax, 8(%r13)
	movl	$2, %r15d
	movl	$16, %r13d
.L294:
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %r11d
	movq	(%r12), %rbx
	movq	(%rbx,%r14), %rdx
	movl	%r11d, %r8d
	sall	$16, %r8d
	movl	%r11d, %edi
	sall	$8, %edi
	leal	(%r8,%rdi), %r9d
	leal	(%r9,%r11), %eax
	cltq
	movq	%rax, (%rdx,%r13)
	addl	$1, %r15d
	addq	$8, %r13
.L293:
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %r10d
	movq	(%r12), %r9
	movq	(%r9,%r14), %rsi
	movl	%r10d, %r11d
	sall	$16, %r11d
	movl	%r10d, %edx
	sall	$8, %edx
	leal	(%r11,%rdx), %ecx
	leal	(%rcx,%r10), %eax
	cltq
	movq	%rax, (%rsi,%r13)
	addl	$1, %r15d
	addq	$8, %r13
	cmpl	%r15d, 40(%rsp)
	jle	.L171
.L169:
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %r8d
	movq	(%r12), %r10
	movq	(%r10,%r14), %r9
	movl	%r8d, %esi
	sall	$16, %esi
	movl	%r8d, %edx
	sall	$8, %edx
	leal	(%rsi,%rdx), %ebx
	leal	(%rbx,%r8), %eax
	cltq
	movq	%rax, (%r9,%r13)
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %esi
	movq	(%r12), %rcx
	movq	(%rcx,%r14), %rdx
	movl	%esi, %r11d
	sall	$16, %r11d
	movl	%esi, %edi
	sall	$8, %edi
	leal	(%r11,%rdi), %r10d
	leal	(%r10,%rsi), %eax
	cltq
	movq	%rax, 8(%rdx,%r13)
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %r11d
	movq	(%r12), %rbx
	movq	(%rbx,%r14), %rdi
	movl	%r11d, %r8d
	sall	$16, %r8d
	movl	%r11d, %r9d
	sall	$8, %r9d
	leal	(%r8,%r9), %ecx
	leal	(%rcx,%r11), %eax
	cltq
	movq	%rax, 16(%rdi,%r13)
	leaq	336(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	336(%rsp), %r8d
	movq	(%r12), %r10
	movq	(%r10,%r14), %r9
	movl	%r8d, %esi
	sall	$16, %esi
	movl	%r8d, %edx
	sall	$8, %edx
	leal	(%rsi,%rdx), %ebx
	leal	(%rbx,%r8), %eax
	cltq
	movq	%rax, 24(%r9,%r13)
	addl	$4, %r15d
	addq	$32, %r13
	cmpl	%r15d, 40(%rsp)
	jg	.L169
.L171:
	addl	$1, 48(%rsp)
	addq	$8, %r14
	jmp	.L168
.L316:
	movq	56(%rsp), %r15
	movq	72(%rsp), %r14
	movq	80(%rsp), %r13
	movq	88(%rsp), %rbx
	jmp	.L170
	.cfi_endproc
.LFE35:
	.size	load_bmp_file, .-load_bmp_file
	.section	.rodata.str1.8
	.align 8
.LC13:
	.string	"pngio: file %s could not be opened for reading"
	.align 8
.LC14:
	.string	"load_png: warning: mmap file failed, using normal buffered I/O: %s\n"
	.align 8
.LC15:
	.string	"pngio: File %s is not recognized as a PNG file"
	.section	.rodata.str1.1
.LC16:
	.string	"1.4.8"
	.section	.rodata.str1.8
	.align 8
.LC17:
	.string	"pngio: png_create_read_struct failed"
	.align 8
.LC18:
	.string	"pngio: png_create_info_struct failed"
	.section	.rodata.str1.1
.LC19:
	.string	"pngio: error during init_io"
	.section	.rodata.str1.8
	.align 8
.LC20:
	.string	"pngio: error during read_image"
	.align 8
.LC21:
	.string	"pngio: unsupported color type: %d\n"
	.text
.globl load_png_file
	.type	load_png_file, @function
load_png_file:
.LFB36:
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
	subq	$312, %rsp
	.cfi_def_cfa_offset 368
	movq	%rdi, 24(%rsp)
	movq	%rsi, 16(%rsp)
	movq	%rdx, 32(%rsp)
	movq	%rcx, (%rsp)
	movl	ncache(%rip), %r13d
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	testl	%r13d, %r13d
	jle	.L348
	movq	icache(%rip), %rbx
	leal	-1(%r13), %r12d
	andl	$7, %r12d
	movq	(%rbx), %rsi
	movq	%rcx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	movl	$1, %ebp
	addq	$24, %rbx
	cmpl	%r13d, %ebp
	jge	.L348
	testl	%r12d, %r12d
	je	.L525
	cmpl	$1, %r12d
	je	.L508
	cmpl	$2, %r12d
	je	.L509
	cmpl	$3, %r12d
	je	.L510
	cmpl	$4, %r12d
	je	.L511
	cmpl	$5, %r12d
	je	.L512
	cmpl	$6, %r12d
	je	.L513
	movq	(%rbx), %rsi
	movq	(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$1, %ebp
	addq	$24, %rbx
.L513:
	movq	(%rbx), %rsi
	movq	(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$1, %ebp
	addq	$24, %rbx
.L512:
	movq	(%rbx), %rsi
	movq	(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$1, %ebp
	addq	$24, %rbx
.L511:
	movq	(%rbx), %rsi
	movq	(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$1, %ebp
	addq	$24, %rbx
.L510:
	movq	(%rbx), %rsi
	movq	(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$1, %ebp
	addq	$24, %rbx
.L509:
	movq	(%rbx), %rsi
	movq	(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$1, %ebp
	addq	$24, %rbx
.L508:
	movq	(%rbx), %rsi
	movq	(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$1, %ebp
	addq	$24, %rbx
	cmpl	%r13d, %ebp
	jge	.L348
.L525:
	movq	(%rsp), %r14
.L351:
	movq	(%rbx), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	leaq	24(%rbx), %r12
	movq	%r12, %rbx
	movq	(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addq	$24, %rbx
	movq	(%rbx), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	leaq	48(%r12), %rbx
	movq	48(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	leaq	72(%r12), %rbx
	movq	72(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	leaq	96(%r12), %rbx
	movq	96(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	leaq	120(%r12), %rbx
	movq	120(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	leaq	144(%r12), %rbx
	movq	144(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L514
	addl	$8, %ebp
	leaq	168(%r12), %rbx
	cmpl	%r13d, %ebp
	jl	.L351
.L348:
	movl	$.LC12, %esi
	movq	(%rsp), %rdi
	call	fopen
	movq	%rax, 8(%rsp)
	testq	%rax, %rax
	je	.L532
	movq	%rax, %rdi
	call	fileno
	movl	%eax, %ebx
	leaq	128(%rsp), %rsi
	movl	%eax, %edi
	call	__fstat50
	testl	%eax, %eax
	js	.L533
	xorl	%r9d, %r9d
	movl	%ebx, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	movq	240(%rsp), %rsi
	xorl	%edi, %edi
	call	mmap
	movq	%rax, 40(%rsp)
	cmpq	$-1, %rax
	je	.L534
	movq	240(%rsp), %rbx
	movq	%rbx, 48(%rsp)
.L355:
	leaq	288(%rsp), %r12
	movq	8(%rsp), %rcx
	movl	$8, %edx
	movl	$1, %esi
	movq	%r12, %rdi
	call	fread
	movl	$8, %edx
	xorl	%esi, %esi
	movq	%r12, %rdi
	call	png_sig_cmp
	testl	%eax, %eax
	je	.L356
	movq	(%rsp), %rsi
	movl	$.LC15, %edi
	xorl	%eax, %eax
	call	printf
	cmpq	$0, 40(%rsp)
	je	.L357
	movq	48(%rsp), %rsi
	movq	40(%rsp), %rdi
	call	munmap
.L357:
	movq	8(%rsp), %rdi
	call	fclose
	movl	$2, %eax
.L350:
	addq	$312, %rsp
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
.L514:
	.cfi_restore_state
	movq	8(%rbx), %rdi
	movq	24(%rsp), %rbp
	movq	%rdi, 0(%rbp)
	movl	16(%rbx), %ecx
	movq	16(%rsp), %rsi
	movl	%ecx, (%rsi)
	movl	20(%rbx), %eax
	movq	32(%rsp), %rdx
	movl	%eax, (%rdx)
	xorl	%eax, %eax
	jmp	.L350
.L356:
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	$.LC16, %edi
	call	png_create_read_struct
	movq	%rax, 56(%rsp)
	testq	%rax, %rax
	je	.L535
	movq	%rax, %rdi
	call	png_create_info_struct
	movq	%rax, 64(%rsp)
	testq	%rax, %rax
	je	.L536
	movl	$88, %edx
	movl	$__longjmp14, %esi
	movq	56(%rsp), %rdi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L362
	movl	$.LC19, %edi
	xorl	%eax, %eax
	call	printf
	cmpq	$0, 40(%rsp)
	je	.L363
	movq	48(%rsp), %rsi
	movq	40(%rsp), %rdi
	call	munmap
.L363:
	movq	8(%rsp), %rdi
	call	fclose
	movl	$5, %eax
	jmp	.L350
.L533:
	movl	$.LC0, %edi
	call	perror
.L353:
	movq	(%rsp), %rsi
	movl	$.LC14, %edi
	xorl	%eax, %eax
	call	printf
	movq	$0, 40(%rsp)
	movq	$0, 48(%rsp)
	jmp	.L355
.L532:
	movq	(%rsp), %rsi
	movl	$.LC13, %edi
	xorl	%eax, %eax
	call	printf
	movl	$1, %eax
	jmp	.L350
.L362:
	movq	8(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	png_init_io
	movl	$8, %esi
	movq	56(%rsp), %rdi
	call	png_set_sig_bytes
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	png_read_info
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	png_get_image_width
	movl	%eax, 100(%rsp)
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	png_get_image_height
	movl	%eax, 76(%rsp)
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	png_get_color_type
	movb	%al, 99(%rsp)
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	png_get_bit_depth
	movq	56(%rsp), %rdi
	call	png_set_interlace_handling
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	png_read_update_info
	movl	$88, %edx
	movl	$__longjmp14, %esi
	movq	56(%rsp), %rdi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	__setjmp14
	testl	%eax, %eax
	jne	.L537
	cmpb	$2, 99(%rsp)
	je	.L376
	cmpb	$6, 99(%rsp)
	je	.L377
	cmpb	$0, 99(%rsp)
	jne	.L538
	movl	$1, %ebx
	movl	$1, 92(%rsp)
.L366:
	movl	76(%rsp), %r14d
	movslq	%r14d, %r8
	movq	%r8, 104(%rsp)
	leaq	0(,%r8,8), %rdi
	call	malloc
	movq	%rax, %r15
	cmpl	$0, 76(%rsp)
	jle	.L368
	movslq	100(%rsp), %r12
	imulq	%rbx, %r12
	movq	%rax, 112(%rsp)
	movl	76(%rsp), %r13d
	subl	$1, %r13d
	andl	$7, %r13d
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%r15)
	movl	$1, %ebp
	leaq	8(%r15), %rbx
	cmpl	$1, 76(%rsp)
	jle	.L515
	testl	%r13d, %r13d
	je	.L369
	cmpl	$1, %r13d
	je	.L501
	cmpl	$2, %r13d
	je	.L502
	cmpl	$3, %r13d
	je	.L503
	cmpl	$4, %r13d
	je	.L504
	cmpl	$5, %r13d
	je	.L505
	cmpl	$6, %r13d
	je	.L506
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	movl	$2, %ebp
	leaq	16(%r15), %rbx
.L506:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L505:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L504:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L503:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L502:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L501:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
	cmpl	%ebp, %r14d
	jle	.L515
.L369:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 8(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 16(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 24(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 32(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 40(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 48(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 56(%rbx)
	addl	$8, %ebp
	addq	$64, %rbx
	cmpl	%ebp, %r14d
	jg	.L369
.L515:
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	png_read_image
	movq	8(%rsp), %rdi
	call	fclose
	movl	100(%rsp), %esi
	movq	16(%rsp), %rcx
	movl	%esi, (%rcx)
	movl	76(%rsp), %edx
	movq	32(%rsp), %rax
	movl	%edx, (%rax)
	movl	$8, %esi
	movq	104(%rsp), %rdi
	call	calloc
	movq	%rax, 80(%rsp)
	movq	24(%rsp), %r13
	movq	%rax, 0(%r13)
	movq	16(%rsp), %r11
	movl	(%r11), %r10d
	movl	%r10d, 124(%rsp)
	movslq	%r10d, %r12
	leal	-1(%r14), %r13d
	andl	$7, %r13d
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	80(%rsp), %r9
	movq	%rax, (%r9)
	movl	$1, %ebp
	movq	%r9, %rbx
	addq	$8, %rbx
	cmpl	$1, %r14d
	jle	.L516
	testl	%r13d, %r13d
	je	.L370
	cmpl	$1, %r13d
	je	.L495
	cmpl	$2, %r13d
	je	.L496
	cmpl	$3, %r13d
	je	.L497
	cmpl	$4, %r13d
	je	.L498
	cmpl	$5, %r13d
	je	.L499
	cmpl	$6, %r13d
	je	.L500
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	movl	$2, %ebp
	movq	80(%rsp), %rbx
	addq	$16, %rbx
.L500:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L499:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L498:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L497:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L496:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L495:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
	cmpl	%ebp, %r14d
	jle	.L516
.L370:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%rbx)
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, 8(%rbx)
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, 16(%rbx)
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, 24(%rbx)
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, 32(%rbx)
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, 40(%rbx)
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, 48(%rbx)
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, 56(%rbx)
	addl	$8, %ebp
	addq	$64, %rbx
	cmpl	%ebp, %r14d
	jg	.L370
.L516:
	movl	92(%rsp), %r8d
	movq	80(%rsp), %rcx
	movq	%r15, %rdx
	movl	76(%rsp), %esi
	movl	124(%rsp), %edi
	call	set_pngrows_array
	leal	-1(%r14), %r12d
	andl	$7, %r12d
	movq	112(%rsp), %rdi
	movq	(%rdi), %rdi
	call	free
	movl	$1, %ebp
	movq	112(%rsp), %rbx
	addq	$8, %rbx
	cmpl	$1, %r14d
	jle	.L375
	testl	%r12d, %r12d
	je	.L371
	cmpl	$1, %r12d
	je	.L489
	cmpl	$2, %r12d
	je	.L490
	cmpl	$3, %r12d
	je	.L491
	cmpl	$4, %r12d
	je	.L492
	cmpl	$5, %r12d
	je	.L493
	cmpl	$6, %r12d
	je	.L494
	movq	(%rbx), %rdi
	call	free
	movl	$2, %ebp
	movq	112(%rsp), %rbx
	addq	$16, %rbx
.L494:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L493:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L492:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L491:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L490:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L489:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
	cmpl	%ebp, %r14d
	jle	.L375
.L371:
	movq	(%rbx), %rdi
	call	free
	movq	8(%rbx), %rdi
	call	free
	movq	16(%rbx), %rdi
	call	free
	movq	24(%rbx), %rdi
	call	free
	movq	32(%rbx), %rdi
	call	free
	movq	40(%rbx), %rdi
	call	free
	movq	48(%rbx), %rdi
	call	free
	movq	56(%rbx), %rdi
	call	free
	addl	$8, %ebp
	addq	$64, %rbx
	cmpl	%ebp, %r14d
	jg	.L371
.L375:
	movq	%r15, %rdi
	call	free
	cmpq	$0, 40(%rsp)
	je	.L372
	movq	48(%rsp), %rsi
	movq	40(%rsp), %rdi
	call	munmap
.L372:
	movq	32(%rsp), %r10
	movl	(%r10), %r13d
	movq	16(%rsp), %r9
	movl	(%r9), %r14d
	movq	24(%rsp), %r8
	movq	(%r8), %r12
	movl	ncache(%rip), %r15d
	movslq	%r15d, %rbx
	imulq	$24, %rbx, %rbp
	addq	icache(%rip), %rbp
	xorl	%eax, %eax
	orq	$-1, %rcx
	movq	(%rsp), %rdi
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	movq	(%rsp), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	%r14d, 16(%rbp)
	movl	%r13d, 20(%rbp)
	movq	%r12, 8(%rbp)
	addl	$1, %r15d
	movl	%r15d, ncache(%rip)
	xorl	%eax, %eax
	jmp	.L350
.L534:
	movl	$.LC1, %edi
	call	perror
	jmp	.L353
.L535:
	movl	$.LC17, %edi
	call	printf
	cmpq	$0, 40(%rsp)
	je	.L359
	movq	48(%rsp), %rsi
	movq	40(%rsp), %rdi
	call	munmap
.L359:
	movq	8(%rsp), %rdi
	call	fclose
	movl	$3, %eax
	jmp	.L350
.L536:
	movl	$.LC18, %edi
	call	printf
	cmpq	$0, 40(%rsp)
	je	.L361
	movq	48(%rsp), %rsi
	movq	40(%rsp), %rdi
	call	munmap
.L361:
	movq	8(%rsp), %rdi
	call	fclose
	movl	$4, %eax
	jmp	.L350
.L537:
	movl	$.LC20, %edi
	xorl	%eax, %eax
	call	printf
	cmpq	$0, 40(%rsp)
	je	.L365
	movq	48(%rsp), %rsi
	movq	40(%rsp), %rdi
	call	munmap
.L365:
	movq	8(%rsp), %rdi
	call	fclose
	movl	$6, %eax
	jmp	.L350
.L538:
	movzbl	99(%rsp), %esi
	movl	$.LC21, %edi
	xorl	%eax, %eax
	call	printf
	cmpq	$0, 40(%rsp)
	je	.L367
	movq	48(%rsp), %rsi
	movq	40(%rsp), %rdi
	call	munmap
.L367:
	movq	8(%rsp), %rdi
	call	fclose
	movl	$7, %eax
	jmp	.L350
.L377:
	movl	$4, %ebx
	movl	$4, 92(%rsp)
	jmp	.L366
.L368:
	movq	%rax, %rsi
	movq	56(%rsp), %rdi
	call	png_read_image
	movq	8(%rsp), %rdi
	call	fclose
	movl	100(%rsp), %r13d
	movq	16(%rsp), %r12
	movl	%r13d, (%r12)
	movl	76(%rsp), %ebp
	movq	32(%rsp), %rsi
	movl	%ebp, (%rsi)
	movl	$8, %esi
	movq	104(%rsp), %rdi
	call	calloc
	movq	24(%rsp), %rdx
	movq	%rax, (%rdx)
	movl	92(%rsp), %r8d
	movq	%rax, %rcx
	movq	%r15, %rdx
	movl	76(%rsp), %esi
	movq	16(%rsp), %r11
	movl	(%r11), %edi
	call	set_pngrows_array
	jmp	.L375
.L376:
	movl	$3, %ebx
	movl	$3, 92(%rsp)
	jmp	.L366
	.cfi_endproc
.LFE36:
	.size	load_png_file, .-load_png_file
	.section	.rodata.str1.1
.LC22:
	.string	"wb"
	.section	.rodata.str1.8
	.align 8
.LC23:
	.string	"save_bmp: error writing file: %s\n"
	.text
.globl save_bmp_file
	.type	save_bmp_file, @function
save_bmp_file:
.LFB37:
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
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	movq	%rdi, %r13
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movl	%esi, %r14d
	movl	%edx, 12(%rsp)
	movq	%rcx, %rbx
	movb	$66, 16(%rsp)
	movb	$77, 17(%rsp)
	movl	$0, 20(%rsp)
	movl	$0, 24(%rsp)
	movl	$56, 28(%rsp)
	movl	$32, 40(%rsp)
	movl	$32, 36(%rsp)
	movl	$40, 32(%rsp)
	movw	$24, 46(%rsp)
	movw	$1, 44(%rsp)
	movl	$0, 48(%rsp)
	movl	$3072, 52(%rsp)
	movl	$0, 56(%rsp)
	movl	$0, 60(%rsp)
	movl	$0, 64(%rsp)
	movl	$0, 68(%rsp)
	movl	$.LC22, %esi
	movq	%rcx, %rdi
	call	fopen
	movq	%rax, %r12
	testq	%rax, %rax
	je	.L561
	movl	$77, %ecx
	movl	$66, %edx
	movl	$.LC5, %esi
	movq	%rax, %rdi
	xorl	%eax, %eax
	call	fprintf
	movl	12(%rsp), %ecx
	movl	%ecx, 40(%rsp)
	movl	%r14d, 36(%rsp)
	leal	(%r14,%r14,2), %ebp
	imull	%ecx, %ebp
	leal	56(%rbp), %edx
	movl	%edx, 20(%rsp)
	leaq	16(%rsp), %rbx
	leaq	4(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	8(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	movl	$56, 28(%rsp)
	movw	$1, 44(%rsp)
	movw	$24, 46(%rsp)
	movl	%ebp, 52(%rsp)
	leaq	12(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	16(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	20(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	24(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	28(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fwrite
	leaq	30(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fwrite
	leaq	32(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	36(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	40(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	44(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	48(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	52(%rbx), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	movslq	28(%rsp), %rsi
	xorl	%edx, %edx
	movq	%r12, %rdi
	call	fseek
	movl	12(%rsp), %eax
	testl	%eax, %eax
	jle	.L542
	movq	%r13, %rbp
	movl	$0, 8(%rsp)
.L543:
	testl	%r14d, %r14d
	jle	.L546
	leal	-1(%r14), %r15d
	andl	$3, %r15d
	movq	0(%rbp), %rsi
	movq	(%rsi), %rax
	movq	%rax, %r8
	andl	$16711680, %r8d
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	movl	$1, %r13d
	movl	$8, %ebx
	cmpl	$1, %r14d
	jle	.L546
	testl	%r15d, %r15d
	je	.L544
	cmpl	$1, %r15d
	je	.L559
	cmpl	$2, %r15d
	je	.L560
	movq	0(%rbp), %r13
	movq	8(%r13), %rbx
	movq	%rbx, %r8
	andl	$16711680, %r8d
	movzbl	%bh, %ecx
	movzbl	%bl, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	movl	$2, %r13d
	movl	$16, %ebx
.L560:
	movq	0(%rbp), %r15
	movq	(%r15,%rbx), %rdx
	movq	%rdx, %r8
	andl	$16711680, %r8d
	movzbl	%dh, %ecx
	movzbl	%dl, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	addl	$1, %r13d
	addq	$8, %rbx
.L559:
	movq	0(%rbp), %rcx
	movq	(%rcx,%rbx), %rax
	movq	%rax, %r8
	andl	$16711680, %r8d
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	addl	$1, %r13d
	addq	$8, %rbx
	cmpl	%r13d, %r14d
	jle	.L546
.L544:
	movq	0(%rbp), %r10
	movq	(%r10,%rbx), %rax
	movq	%rax, %r8
	andl	$16711680, %r8d
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	movq	0(%rbp), %r9
	movq	8(%r9,%rbx), %rdx
	movq	%rdx, %r8
	andl	$16711680, %r8d
	movzbl	%dh, %ecx
	movzbl	%dl, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	movq	0(%rbp), %r8
	movq	16(%r8,%rbx), %rax
	movq	%rax, %r8
	andl	$16711680, %r8d
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	movq	0(%rbp), %rdi
	movq	24(%rdi,%rbx), %rdx
	movq	%rdx, %r8
	andl	$16711680, %r8d
	movzbl	%dh, %ecx
	movzbl	%dl, %edx
	shrq	$16, %r8
	movl	$.LC10, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	call	fprintf
	addl	$4, %r13d
	addq	$32, %rbx
	cmpl	%r13d, %r14d
	jg	.L544
.L546:
	addl	$1, 8(%rsp)
	addq	$8, %rbp
	movl	8(%rsp), %r11d
	cmpl	%r11d, 12(%rsp)
	jg	.L543
.L542:
	movq	%r12, %rdi
	call	fclose
	xorl	%eax, %eax
	addq	$88, %rsp
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
.L561:
	.cfi_restore_state
	movq	%rbx, %rsi
	movl	$.LC23, %edi
	xorl	%eax, %eax
	call	printf
	movl	$1, %eax
	addq	$88, %rsp
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
	.cfi_endproc
.LFE37:
	.size	save_bmp_file, .-save_bmp_file
	.section	.rodata.str1.8
	.align 8
.LC24:
	.string	"pngio: File %s could not be opened for writing"
	.align 8
.LC25:
	.string	"pngio: png_create_write_struct failed"
	.section	.rodata.str1.1
.LC26:
	.string	"pngio: Error during init_io"
	.section	.rodata.str1.8
	.align 8
.LC27:
	.string	"pngio: Error during writing header"
	.align 8
.LC28:
	.string	"pngio: Error during writing bytes"
	.align 8
.LC29:
	.string	"pngio: Error during end of write"
	.text
.globl save_png_file
	.type	save_png_file, @function
save_png_file:
.LFB38:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	pushq	%r12
	.cfi_def_cfa_offset 24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$88, %rsp
	.cfi_def_cfa_offset 128
	movq	%rdi, 64(%rsp)
	movl	%esi, 60(%rsp)
	movl	%edx, 56(%rsp)
	movq	%rcx, %rbx
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movl	$.LC22, %esi
	movq	%rcx, %rdi
	call	fopen
	movq	%rax, 40(%rsp)
	testq	%rax, %rax
	je	.L635
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	$.LC16, %edi
	call	png_create_write_struct
	movq	%rax, 32(%rsp)
	testq	%rax, %rax
	je	.L636
	movq	%rax, %rdi
	call	png_create_info_struct
	movq	%rax, 48(%rsp)
	testq	%rax, %rax
	je	.L637
	movl	$88, %edx
	movl	$__longjmp14, %esi
	movq	32(%rsp), %rdi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L567
	movl	$.LC26, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$4, %eax
.L564:
	addq	$88, %rsp
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
.L636:
	.cfi_restore_state
	movl	$.LC25, %edi
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$2, %eax
	addq	$88, %rsp
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
.L637:
	.cfi_restore_state
	movl	$.LC18, %edi
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$3, %eax
	addq	$88, %rsp
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
.L567:
	.cfi_restore_state
	movq	40(%rsp), %rsi
	movq	32(%rsp), %rdi
	call	png_init_io
	movl	$88, %edx
	movl	$__longjmp14, %esi
	movq	32(%rsp), %rdi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L568
	movl	$.LC27, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$5, %eax
	addq	$88, %rsp
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
.L635:
	.cfi_restore_state
	movq	%rbx, %rsi
	movl	$.LC24, %edi
	call	printf
	movl	$1, %eax
	addq	$88, %rsp
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
.L568:
	.cfi_restore_state
	movl	$0, 16(%rsp)
	movl	$0, 8(%rsp)
	movl	$0, (%rsp)
	movl	$2, %r9d
	movl	$8, %r8d
	movl	56(%rsp), %ecx
	movl	60(%rsp), %edx
	movq	48(%rsp), %rsi
	movq	32(%rsp), %rdi
	call	png_set_IHDR
	movq	48(%rsp), %rsi
	movq	32(%rsp), %rdi
	call	png_write_info
	movl	$88, %edx
	movl	$__longjmp14, %esi
	movq	32(%rsp), %rdi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L569
	movl	$.LC28, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$6, %eax
	jmp	.L564
.L569:
	movslq	56(%rsp), %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, 72(%rsp)
	movl	56(%rsp), %ecx
	testl	%ecx, %ecx
	jle	.L570
	movl	60(%rsp), %eax
	addl	$9, %eax
	cltq
	leaq	(%rax,%rax,2), %r12
	movl	56(%rsp), %r13d
	subl	$1, %r13d
	andl	$7, %r13d
	movq	%r12, %rdi
	call	malloc
	movq	72(%rsp), %rdx
	movq	%rax, (%rdx)
	movl	$1, %ebp
	movq	%rdx, %rbx
	addq	$8, %rbx
	cmpl	$1, 56(%rsp)
	jle	.L570
	testl	%r13d, %r13d
	je	.L571
	cmpl	$1, %r13d
	je	.L629
	cmpl	$2, %r13d
	je	.L630
	cmpl	$3, %r13d
	je	.L631
	cmpl	$4, %r13d
	je	.L632
	cmpl	$5, %r13d
	je	.L633
	cmpl	$6, %r13d
	je	.L634
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L634:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L633:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L632:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L631:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L630:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
.L629:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addl	$1, %ebp
	addq	$8, %rbx
	cmpl	%ebp, 56(%rsp)
	jle	.L570
.L571:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 8(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 16(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 24(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 32(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 40(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 48(%rbx)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 56(%rbx)
	addl	$8, %ebp
	addq	$64, %rbx
	cmpl	%ebp, 56(%rsp)
	jg	.L571
.L570:
	movq	64(%rsp), %rcx
	movq	72(%rsp), %rdx
	movl	56(%rsp), %esi
	movl	60(%rsp), %edi
	call	set_pngrow_pointers
	movq	72(%rsp), %rsi
	movq	32(%rsp), %rdi
	call	png_write_image
	movl	$88, %edx
	movl	$__longjmp14, %esi
	movq	32(%rsp), %rdi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L572
	movl	$.LC29, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$7, %eax
	jmp	.L564
.L572:
	xorl	%esi, %esi
	movq	32(%rsp), %rdi
	call	png_write_end
	movl	56(%rsp), %ebx
	testl	%ebx, %ebx
	jle	.L573
	movl	56(%rsp), %r12d
	subl	$1, %r12d
	andl	$7, %r12d
	movq	72(%rsp), %rsi
	movq	(%rsi), %rdi
	call	free
	movl	$1, %ebp
	movq	72(%rsp), %rbx
	addq	$8, %rbx
	cmpl	$1, 56(%rsp)
	jle	.L573
	testl	%r12d, %r12d
	je	.L574
	cmpl	$1, %r12d
	je	.L623
	cmpl	$2, %r12d
	je	.L624
	cmpl	$3, %r12d
	je	.L625
	cmpl	$4, %r12d
	je	.L626
	cmpl	$5, %r12d
	je	.L627
	cmpl	$6, %r12d
	je	.L628
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L628:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L627:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L626:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L625:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L624:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
.L623:
	movq	(%rbx), %rdi
	call	free
	addl	$1, %ebp
	addq	$8, %rbx
	cmpl	%ebp, 56(%rsp)
	jle	.L573
.L574:
	movq	(%rbx), %rdi
	call	free
	movq	8(%rbx), %rdi
	call	free
	movq	16(%rbx), %rdi
	call	free
	movq	24(%rbx), %rdi
	call	free
	movq	32(%rbx), %rdi
	call	free
	movq	40(%rbx), %rdi
	call	free
	movq	48(%rbx), %rdi
	call	free
	movq	56(%rbx), %rdi
	call	free
	addl	$8, %ebp
	addq	$64, %rbx
	cmpl	%ebp, 56(%rsp)
	jg	.L574
.L573:
	movq	72(%rsp), %rdi
	call	free
	movq	40(%rsp), %rdi
	call	fclose
	xorl	%eax, %eax
	jmp	.L564
	.cfi_endproc
.LFE38:
	.size	save_png_file, .-save_png_file
	.section	.rodata.str1.8
	.align 8
.LC30:
	.string	"load_jpg: warning: mmap file failed, using normal buffered I/O: %s\n"
	.section	.rodata.str1.1
.LC31:
	.string	"setjmp error"
	.text
.globl load_jpeg_file
	.type	load_jpeg_file, @function
load_jpeg_file:
.LFB39:
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
	subq	$1160, %rsp
	.cfi_def_cfa_offset 1216
	movq	%rdi, 24(%rsp)
	movq	%rsi, 40(%rsp)
	movq	%rdx, 48(%rsp)
	movq	%rcx, 16(%rsp)
	movl	ncache(%rip), %r13d
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	testl	%r13d, %r13d
	jle	.L639
	movq	icache(%rip), %rbx
	leal	-1(%r13), %r12d
	andl	$7, %r12d
	movq	(%rbx), %rsi
	movq	%rcx, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	movl	$1, %ebp
	addq	$24, %rbx
	cmpl	$1, %r13d
	jle	.L639
	testl	%r12d, %r12d
	je	.L748
	cmpl	$1, %r12d
	je	.L737
	cmpl	$2, %r12d
	je	.L738
	cmpl	$3, %r12d
	je	.L739
	cmpl	$4, %r12d
	je	.L740
	cmpl	$5, %r12d
	je	.L741
	cmpl	$6, %r12d
	je	.L742
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$1, %ebp
	addq	$24, %rbx
.L742:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$1, %ebp
	addq	$24, %rbx
.L741:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$1, %ebp
	addq	$24, %rbx
.L740:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$1, %ebp
	addq	$24, %rbx
.L739:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$1, %ebp
	addq	$24, %rbx
.L738:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$1, %ebp
	addq	$24, %rbx
.L737:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$1, %ebp
	addq	$24, %rbx
	cmpl	%ebp, %r13d
	jle	.L639
.L748:
	movq	16(%rsp), %r14
.L642:
	movq	(%rbx), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	leaq	24(%rbx), %r12
	movq	%r12, %rbx
	movq	(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addq	$24, %rbx
	movq	(%rbx), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	leaq	48(%r12), %rbx
	movq	48(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	leaq	72(%r12), %rbx
	movq	72(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	leaq	96(%r12), %rbx
	movq	96(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	leaq	120(%r12), %rbx
	movq	120(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	leaq	144(%r12), %rbx
	movq	144(%r12), %rsi
	movq	%r14, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L743
	addl	$8, %ebp
	leaq	168(%r12), %rbx
	cmpl	%ebp, %r13d
	jg	.L642
.L639:
	movl	$.LC12, %esi
	movq	16(%rsp), %rdi
	call	fopen
	movq	%rax, 32(%rsp)
	testq	%rax, %rax
	je	.L755
	movq	%rax, %rdi
	call	fileno
	movl	%eax, %ebx
	leaq	992(%rsp), %rsi
	movl	%eax, %edi
	call	__fstat50
	testl	%eax, %eax
	js	.L756
	xorl	%r9d, %r9d
	movl	%ebx, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	movq	1104(%rsp), %rsi
	xorl	%edi, %edi
	call	mmap
	movq	%rax, 56(%rsp)
	cmpq	$-1, %rax
	je	.L757
	movq	1104(%rsp), %rbx
	movq	%rbx, 64(%rsp)
.L646:
	leaq	736(%rsp), %r12
	movq	%r12, %rdi
	call	jpeg_std_error
	movq	%rax, 80(%rsp)
	movq	$my_error_exit, 736(%rsp)
	movq	%r12, errptr(%rip)
	leaq	168(%r12), %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L647
	leaq	80(%rsp), %rdi
	call	jpeg_destroy_decompress
	cmpq	$0, 56(%rsp)
	je	.L648
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L648:
	movq	32(%rsp), %rdi
	call	fclose
	movl	$.LC31, %edi
	call	puts
	movl	$2, %eax
.L641:
	addq	$1160, %rsp
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
.L743:
	.cfi_restore_state
	movq	8(%rbx), %rdi
	movq	24(%rsp), %rbp
	movq	%rdi, 0(%rbp)
	movl	16(%rbx), %ecx
	movq	40(%rsp), %rsi
	movl	%ecx, (%rsi)
	movl	20(%rbx), %eax
	movq	48(%rsp), %rdx
	movl	%eax, (%rdx)
	xorl	%eax, %eax
	jmp	.L641
.L755:
	movl	$1, %eax
	jmp	.L641
.L647:
	leaq	80(%rsp), %rbx
	movl	$656, %edx
	movl	$80, %esi
	movq	%rbx, %rdi
	call	jpeg_CreateDecompress
	cmpq	$0, 56(%rsp)
	je	.L649
	movq	64(%rsp), %rdx
	movq	56(%rsp), %rsi
	movq	%rbx, %rdi
	call	jpeg_mem_src
	movq	32(%rsp), %rdi
	call	fclose
	movq	$0, 32(%rsp)
.L650:
	leaq	80(%rsp), %rdi
	movl	$1, %esi
	call	jpeg_read_header
	cmpl	$1, 140(%rsp)
	je	.L658
	movl	$0, 188(%rsp)
	xorl	%r15d, %r15d
.L651:
	leaq	80(%rsp), %rdi
	call	jpeg_start_decompress
	movl	216(%rsp), %r11d
	movl	%r11d, 76(%rsp)
	movq	40(%rsp), %r10
	movl	%r11d, (%r10)
	movl	220(%rsp), %r13d
	movq	48(%rsp), %r9
	movl	%r13d, (%r9)
	movslq	%r13d, %rdi
	movl	$8, %esi
	call	calloc
	movq	24(%rsp), %r8
	movq	%rax, (%r8)
	testl	%r13d, %r13d
	jle	.L652
	movq	40(%rsp), %r14
	movslq	(%r14), %rbp
	leal	-1(%r13), %r14d
	andl	$7, %r14d
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdx
	movq	%rax, (%rdx)
	movl	$1, %r12d
	leaq	8(%rdx), %rbx
	cmpl	$1, %r13d
	jle	.L652
	testl	%r14d, %r14d
	je	.L653
	cmpl	$1, %r14d
	je	.L730
	cmpl	$2, %r14d
	je	.L731
	cmpl	$3, %r14d
	je	.L732
	cmpl	$4, %r14d
	je	.L733
	cmpl	$5, %r14d
	je	.L734
	cmpl	$6, %r14d
	je	.L735
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	%rax, (%rbx)
	movl	$2, %r12d
	movq	8(%rsp), %rbx
	leaq	16(%rbx), %rbx
.L735:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %r12d
	addq	$8, %rbx
.L734:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %r12d
	addq	$8, %rbx
.L733:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %r12d
	addq	$8, %rbx
.L732:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %r12d
	addq	$8, %rbx
.L731:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %r12d
	addq	$8, %rbx
.L730:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, (%rbx)
	addl	$1, %r12d
	addq	$8, %rbx
	cmpl	%r12d, %r13d
	jle	.L652
.L653:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, (%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 8(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 16(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 24(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 32(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 40(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 48(%rbx)
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 56(%rbx)
	addl	$8, %r12d
	addq	$64, %rbx
	cmpl	%r12d, %r13d
	jg	.L653
.L652:
	movl	76(%rsp), %edx
	imull	228(%rsp), %edx
	leaq	80(%rsp), %rbp
	movl	$32, %ecx
	movl	$1, %esi
	movq	%rbp, %rdi
	movq	88(%rsp), %rbx
	call	*16(%rbx)
	movq	%rax, %rbx
	movq	48(%rsp), %r13
	movl	0(%r13), %r13d
	movl	248(%rsp), %eax
	cmpl	%eax, 220(%rsp)
	jbe	.L654
	subl	$1, %r13d
	movq	40(%rsp), %r14
.L655:
	movl	$32, %edx
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	jpeg_read_scanlines
	movl	%eax, %r12d
	movl	%r15d, %r9d
	movq	24(%rsp), %rcx
	movq	(%rcx), %r8
	movl	(%r14), %ecx
	movq	%rbx, %rdx
	movl	%eax, %esi
	movl	%r13d, %edi
	call	set_rows_array
	subl	%r12d, %r13d
	movl	220(%rsp), %edx
	cmpl	%edx, 248(%rsp)
	jae	.L654
	movl	$32, %edx
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	jpeg_read_scanlines
	movl	%eax, %r12d
	movl	%r15d, %r9d
	movq	24(%rsp), %rdx
	movq	(%rdx), %r8
	movl	(%r14), %ecx
	movq	%rbx, %rdx
	movl	%eax, %esi
	movl	%r13d, %edi
	call	set_rows_array
	subl	%r12d, %r13d
	movl	220(%rsp), %r11d
	cmpl	%r11d, 248(%rsp)
	jae	.L654
	movl	$32, %edx
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	jpeg_read_scanlines
	movl	%eax, %r12d
	movl	%r15d, %r9d
	movq	24(%rsp), %rax
	movq	(%rax), %r8
	movl	(%r14), %ecx
	movq	%rbx, %rdx
	movl	%r12d, %esi
	movl	%r13d, %edi
	call	set_rows_array
	subl	%r12d, %r13d
	movl	220(%rsp), %ecx
	cmpl	%ecx, 248(%rsp)
	jae	.L654
	movl	$32, %edx
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	jpeg_read_scanlines
	movl	%eax, %r12d
	movl	%r15d, %r9d
	movq	24(%rsp), %rsi
	movq	(%rsi), %r8
	movl	(%r14), %ecx
	movq	%rbx, %rdx
	movl	%eax, %esi
	movl	%r13d, %edi
	call	set_rows_array
	subl	%r12d, %r13d
	movl	220(%rsp), %edi
	cmpl	%edi, 248(%rsp)
	jb	.L655
.L654:
	leaq	80(%rsp), %r15
	movq	%r15, %rdi
	call	jpeg_finish_decompress
	movq	%r15, %rdi
	call	jpeg_destroy_decompress
	cmpq	$0, 56(%rsp)
	je	.L656
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L656:
	movq	48(%rsp), %r10
	movl	(%r10), %r12d
	movq	40(%rsp), %r9
	movl	(%r9), %r13d
	movq	24(%rsp), %r8
	movq	(%r8), %r14
	movl	ncache(%rip), %ebp
	movslq	%ebp, %rsi
	leaq	(%rsi,%rsi,2), %rbx
	salq	$3, %rbx
	addq	icache(%rip), %rbx
	xorl	%eax, %eax
	movq	$-1, %rcx
	movq	16(%rsp), %rdi
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, (%rbx)
	movq	16(%rsp), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	%r13d, 16(%rbx)
	movl	%r12d, 20(%rbx)
	movq	%r14, 8(%rbx)
	addl	$1, %ebp
	movl	%ebp, ncache(%rip)
	cmpq	$0, 32(%rsp)
	je	.L659
	movq	32(%rsp), %rdi
	call	fclose
	xorl	%eax, %eax
	jmp	.L641
.L756:
	movl	$.LC0, %edi
	call	perror
.L644:
	movq	16(%rsp), %rsi
	movl	$.LC30, %edi
	xorl	%eax, %eax
	call	printf
	movq	$0, 56(%rsp)
	movq	$0, 64(%rsp)
	jmp	.L646
.L757:
	movl	$.LC1, %edi
	call	perror
	jmp	.L644
.L658:
	movl	$1, %r15d
	jmp	.L651
.L659:
	xorl	%eax, %eax
	.p2align 4,,3
	jmp	.L641
.L649:
	movq	32(%rsp), %rsi
	movq	%rbx, %rdi
	call	jpeg_stdio_src
	jmp	.L650
	.cfi_endproc
.LFE39:
	.size	load_jpeg_file, .-load_jpeg_file
.globl jpeg_compute_pixels
	.type	jpeg_compute_pixels, @function
jpeg_compute_pixels:
.LFB40:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movl	%ecx, -48(%rsp)
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %ecx
	jle	.L758
	.cfi_offset 3, -16
.L763:
	movl	-48(%rsp), %r8d
	subl	$1, %r8d
	movl	%r8d, -48(%rsp)
	movl	$0, -32(%rsp)
	movl	-32(%rsp), %ebx
	cmpl	%ebx, %edx
	jg	.L764
	jmp	.L760
.L768:
	movl	-16(%rsp), %eax
	movl	-32(%rsp), %r10d
	cltq
	movslq	%r10d, %rbx
	movq	(%rsi,%rax,8), %rax
	movq	(%rax,%rbx,8), %rax
	movl	-48(%rsp), %r8d
	movl	-32(%rsp), %r9d
	imull	%edx, %r8d
	addl	%r9d, %r8d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %r10
	movq	%rax, %rbx
	shrq	$16, %rbx
	movb	%bl, (%rdi,%r10)
	movl	-48(%rsp), %r8d
	movl	-32(%rsp), %r9d
	imull	%edx, %r8d
	addl	%r9d, %r8d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %r10
	movzbl	%ah, %ebx
	movb	%bl, 1(%rdi,%r10)
	movl	-48(%rsp), %r8d
	movl	-32(%rsp), %r9d
	imull	%edx, %r8d
	addl	%r9d, %r8d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %r10
	movb	%al, 2(%rdi,%r10)
	movl	-32(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, -32(%rsp)
	movl	-32(%rsp), %r9d
	cmpl	%edx, %r9d
	jge	.L760
.L764:
	movl	-16(%rsp), %eax
	movl	-32(%rsp), %r10d
	cltq
	movslq	%r10d, %rbx
	movq	(%rsi,%rax,8), %rax
	movq	(%rax,%rbx,8), %rax
	movl	-48(%rsp), %r8d
	movl	-32(%rsp), %r9d
	imull	%edx, %r8d
	addl	%r9d, %r8d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %r10
	movq	%rax, %rbx
	shrq	$16, %rbx
	movb	%bl, (%rdi,%r10)
	movl	-48(%rsp), %r8d
	movl	-32(%rsp), %r9d
	imull	%edx, %r8d
	addl	%r9d, %r8d
	leal	(%r8,%r8,2), %r11d
	movslq	%r11d, %r10
	movzbl	%ah, %ebx
	movb	%bl, 1(%rdi,%r10)
	movl	-48(%rsp), %r8d
	movl	-32(%rsp), %r9d
	imull	%edx, %r8d
	addl	%r9d, %r8d
	leal	(%r8,%r8,2), %ebx
	movslq	%ebx, %r11
	movb	%al, 2(%rdi,%r11)
	movl	-32(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -32(%rsp)
	movl	-32(%rsp), %r9d
	cmpl	%edx, %r9d
	jl	.L768
.L760:
	movl	-16(%rsp), %r8d
	addl	$1, %r8d
	movl	%r8d, -16(%rsp)
	movl	-16(%rsp), %r11d
	cmpl	%r11d, %ecx
	jg	.L763
.L758:
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	jpeg_compute_pixels, .-jpeg_compute_pixels
.globl init_pixels
	.type	init_pixels, @function
init_pixels:
.LFB41:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	pushq	%r12
	.cfi_def_cfa_offset 24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, %rbx
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movq	%rsi, %rbp
	movl	%edx, %r12d
	movl	%ecx, %r13d
	leal	(%r12,%r12,2), %edi
	imull	%ecx, %edi
	addl	$9, %edi
	movslq	%edi, %rdi
	call	malloc
	movq	%rax, (%rbx)
	movl	ualoop(%rip), %ebx
	testl	%ebx, %ebx
	jne	.L771
	movl	%r13d, %ecx
	movl	%r12d, %edx
	movq	%rbp, %rsi
	movq	%rax, %rdi
	addq	$8, %rsp
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
	jmp	jpeg_compute_pixels
.L771:
	.cfi_restore_state
	movl	%r13d, %ecx
	movl	%r12d, %edx
	movq	%rbp, %rsi
	movq	%rax, %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	compute_asm
	.cfi_endproc
.LFE41:
	.size	init_pixels, .-init_pixels
.globl free_pixels
	.type	free_pixels, @function
free_pixels:
.LFB42:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movq	(%rdi), %rdi
	call	free
	movq	$0, (%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE42:
	.size	free_pixels, .-free_pixels
	.section	.rodata.str1.8
	.align 8
.LC33:
	.string	"Failed to write jpeg scanlines: written: %d/%d\n"
	.text
.globl save_jpeg_file
	.type	save_jpeg_file, @function
save_jpeg_file:
.LFB43:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$880, %rsp
	.cfi_def_cfa_offset 896
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	%esi, 4(%rsp)
	movl	%edx, (%rsp)
	movl	$.LC22, %esi
	movq	%rcx, %rdi
	call	fopen
	movq	%rax, 8(%rsp)
	testq	%rax, %rax
	je	.L786
	movl	4(%rsp), %ecx
	leal	(%rcx,%rcx,2), %eax
	movl	%eax, 28(%rsp)
	movl	(%rsp), %edx
	imull	%eax, %edx
	addl	$9, %edx
	movslq	%edx, %rdi
	call	malloc
	movq	%rax, 16(%rsp)
	movl	ualoop(%rip), %edi
	testl	%edi, %edi
	jne	.L830
	movq	%rax, %rdi
	movl	(%rsp), %ecx
	movl	4(%rsp), %edx
	movq	%rbx, %rsi
	call	jpeg_compute_pixels
.L776:
	leaq	624(%rsp), %rbx
	movq	%rbx, %rdi
	call	jpeg_std_error
	movq	%rax, 32(%rsp)
	movq	$my_error_exit, 624(%rsp)
	movq	%rbx, errptr(%rip)
	leaq	168(%rbx), %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L777
	leaq	32(%rsp), %rdi
	call	jpeg_destroy_compress
	movq	8(%rsp), %rdi
	call	fclose
	movl	$2, %eax
	addq	$880, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L830:
	.cfi_restore_state
	movl	(%rsp), %ecx
	movl	4(%rsp), %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	compute_asm
	jmp	.L776
.L777:
	leaq	32(%rsp), %rbx
	movl	$584, %edx
	movl	$80, %esi
	movq	%rbx, %rdi
	call	jpeg_CreateCompress
	movq	8(%rsp), %rsi
	movq	%rbx, %rdi
	call	jpeg_stdio_dest
	movl	4(%rsp), %r9d
	movl	%r9d, 80(%rsp)
	movl	(%rsp), %r8d
	movl	%r8d, 84(%rsp)
	movl	$3, 88(%rsp)
	movl	$2, 92(%rsp)
	movq	%rbx, %rdi
	call	jpeg_set_defaults
	xorl	%edx, %edx
	movl	jqual(%rip), %esi
	movq	%rbx, %rdi
	call	jpeg_set_quality
	movl	$1, %esi
	movq	%rbx, %rdi
	call	jpeg_start_compress
	movslq	(%rsp), %rdi
	salq	$3, %rdi
	call	malloc
	movl	(%rsp), %esi
	testl	%esi, %esi
	jle	.L778
	movq	%rax, %rdx
	andl	$15, %edx
	shrq	$3, %rdx
	cmpl	%edx, (%rsp)
	cmovbe	(%rsp), %edx
	mov	%edx, %ecx
	testl	%edx, %edx
	je	.L787
	movq	16(%rsp), %r10
	movq	%r10, (%rax)
	cmpl	%edx, (%rsp)
	je	.L778
	movl	$1, %edi
.L779:
	movl	(%rsp), %r9d
	subl	%edx, %r9d
	movl	%r9d, %ebx
	shrl	$2, %ebx
	movl	%ebx, %r10d
	sall	$2, %r10d
	je	.L781
	leal	1(%rdi), %r11d
	leal	2(%rdi), %esi
	leal	3(%rdi), %edx
	movd	%esi, %xmm3
	movd	%edx, %xmm2
	punpckldq	%xmm2, %xmm3
	movd	%edi, %xmm2
	movd	%r11d, %xmm1
	punpckldq	%xmm1, %xmm2
	punpcklqdq	%xmm3, %xmm2
	movdqa	.LC32(%rip), %xmm3
	movd	28(%rsp), %xmm0
	pshufd	$0, %xmm0, %xmm1
	movq	16(%rsp), %xmm4
	punpcklqdq	%xmm4, %xmm4
	movdqa	%xmm4, %xmm0
	leaq	(%rax,%rcx,8), %rdx
	xorl	%ecx, %ecx
	pxor	%xmm4, %xmm4
	leal	-1(%rbx), %esi
	andl	$3, %esi
	je	.L782
	movdqa	%xmm1, %xmm13
	pmuludq	%xmm2, %xmm13
	movdqa	%xmm1, %xmm14
	psrldq	$4, %xmm14
	movdqa	%xmm2, %xmm12
	psrldq	$4, %xmm12
	pmuludq	%xmm14, %xmm12
	pshufd	$8, %xmm13, %xmm5
	pshufd	$8, %xmm12, %xmm11
	punpckldq	%xmm11, %xmm5
	movdqa	%xmm4, %xmm9
	pcmpgtd	%xmm5, %xmm9
	movdqa	%xmm5, %xmm10
	punpckldq	%xmm9, %xmm10
	paddq	%xmm0, %xmm10
	movdqa	%xmm10, (%rdx)
	punpckhdq	%xmm9, %xmm5
	paddq	%xmm0, %xmm5
	movdqa	%xmm5, 16(%rdx)
	movl	$1, %ecx
	addq	$32, %rdx
	paddd	%xmm3, %xmm2
	cmpl	$1, %esi
	je	.L782
	cmpl	$2, %esi
	je	.L828
	movdqa	%xmm1, %xmm5
	pmuludq	%xmm2, %xmm5
	movdqa	%xmm1, %xmm10
	psrldq	$4, %xmm10
	movdqa	%xmm2, %xmm9
	psrldq	$4, %xmm9
	pmuludq	%xmm10, %xmm9
	pshufd	$8, %xmm5, %xmm15
	pshufd	$8, %xmm9, %xmm7
	punpckldq	%xmm7, %xmm15
	movdqa	%xmm4, %xmm6
	pcmpgtd	%xmm15, %xmm6
	movdqa	%xmm15, %xmm8
	punpckldq	%xmm6, %xmm8
	paddq	%xmm0, %xmm8
	movdqa	%xmm8, (%rdx)
	punpckhdq	%xmm6, %xmm15
	paddq	%xmm0, %xmm15
	movdqa	%xmm15, 16(%rdx)
	movb	$2, %cl
	addq	$32, %rdx
	paddd	%xmm3, %xmm2
.L828:
	movdqa	%xmm1, %xmm6
	pmuludq	%xmm2, %xmm6
	movdqa	%xmm1, %xmm8
	psrldq	$4, %xmm8
	movdqa	%xmm2, %xmm15
	psrldq	$4, %xmm15
	pmuludq	%xmm8, %xmm15
	pshufd	$8, %xmm6, %xmm11
	pshufd	$8, %xmm15, %xmm14
	punpckldq	%xmm14, %xmm11
	movdqa	%xmm4, %xmm12
	pcmpgtd	%xmm11, %xmm12
	movdqa	%xmm11, %xmm13
	punpckldq	%xmm12, %xmm13
	paddq	%xmm0, %xmm13
	movdqa	%xmm13, (%rdx)
	punpckhdq	%xmm12, %xmm11
	paddq	%xmm0, %xmm11
	movdqa	%xmm11, 16(%rdx)
	addl	$1, %ecx
	addq	$32, %rdx
	paddd	%xmm3, %xmm2
	jmp	.L782
.L788:
	movdqa	%xmm5, %xmm6
	paddd	%xmm3, %xmm6
	movdqa	%xmm1, %xmm11
	pmuludq	%xmm5, %xmm11
	movdqa	%xmm1, %xmm12
	psrldq	$4, %xmm12
	psrldq	$4, %xmm5
	pmuludq	%xmm12, %xmm5
	pshufd	$8, %xmm11, %xmm2
	pshufd	$8, %xmm5, %xmm10
	punpckldq	%xmm10, %xmm2
	movdqa	%xmm4, %xmm8
	pcmpgtd	%xmm2, %xmm8
	movdqa	%xmm2, %xmm9
	punpckldq	%xmm8, %xmm9
	paddq	%xmm0, %xmm9
	movdqa	%xmm9, 32(%rdx)
	punpckhdq	%xmm8, %xmm2
	paddq	%xmm0, %xmm2
	movdqa	%xmm2, 16(%r8)
	movdqa	%xmm6, %xmm2
	paddd	%xmm3, %xmm2
	movdqa	%xmm1, %xmm5
	pmuludq	%xmm6, %xmm5
	movdqa	%xmm1, %xmm7
	psrldq	$4, %xmm7
	psrldq	$4, %xmm6
	pmuludq	%xmm7, %xmm6
	pshufd	$8, %xmm5, %xmm12
	pshufd	$8, %xmm6, %xmm15
	punpckldq	%xmm15, %xmm12
	movdqa	%xmm4, %xmm13
	pcmpgtd	%xmm12, %xmm13
	movdqa	%xmm12, %xmm14
	punpckldq	%xmm13, %xmm14
	paddq	%xmm0, %xmm14
	movdqa	%xmm14, 64(%rdx)
	punpckhdq	%xmm13, %xmm12
	paddq	%xmm0, %xmm12
	movdqa	%xmm12, 80(%rdx)
	movdqa	%xmm1, %xmm10
	pmuludq	%xmm2, %xmm10
	movdqa	%xmm1, %xmm11
	psrldq	$4, %xmm11
	movdqa	%xmm2, %xmm9
	psrldq	$4, %xmm9
	pmuludq	%xmm11, %xmm9
	pshufd	$8, %xmm10, %xmm5
	pshufd	$8, %xmm9, %xmm8
	punpckldq	%xmm8, %xmm5
	movdqa	%xmm4, %xmm6
	pcmpgtd	%xmm5, %xmm6
	movdqa	%xmm5, %xmm7
	punpckldq	%xmm6, %xmm7
	paddq	%xmm0, %xmm7
	movdqa	%xmm7, 96(%rdx)
	punpckhdq	%xmm6, %xmm5
	paddq	%xmm0, %xmm5
	movdqa	%xmm5, 112(%rdx)
	addl	$4, %ecx
	subq	$-128, %rdx
	paddd	%xmm3, %xmm2
.L782:
	movdqa	%xmm2, %xmm5
	paddd	%xmm3, %xmm5
	movdqa	%xmm1, %xmm7
	pmuludq	%xmm2, %xmm7
	movdqa	%xmm1, %xmm8
	psrldq	$4, %xmm8
	psrldq	$4, %xmm2
	pmuludq	%xmm2, %xmm8
	pshufd	$8, %xmm7, %xmm13
	pshufd	$8, %xmm8, %xmm6
	punpckldq	%xmm6, %xmm13
	movdqa	%xmm4, %xmm14
	pcmpgtd	%xmm13, %xmm14
	movdqa	%xmm13, %xmm15
	punpckldq	%xmm14, %xmm15
	paddq	%xmm0, %xmm15
	movdqa	%xmm15, (%rdx)
	punpckhdq	%xmm14, %xmm13
	paddq	%xmm0, %xmm13
	movdqa	%xmm13, 16(%rdx)
	leal	1(%rcx), %r11d
	leaq	32(%rdx), %r8
	cmpl	%ebx, %r11d
	jb	.L788
	addl	%r10d, %edi
	cmpl	%r10d, %r9d
	je	.L778
.L781:
	movslq	28(%rsp), %rsi
	movl	28(%rsp), %ecx
	imull	%edi, %ecx
	movslq	%ecx, %rcx
	addq	16(%rsp), %rcx
	movslq	%edi, %rbx
	leaq	(%rax,%rbx,8), %r9
	movl	%edi, %r8d
	notl	%r8d
	addl	(%rsp), %r8d
	andl	$7, %r8d
	movq	%rcx, (%r9)
	leal	1(%rdi), %ebx
	addq	%rsi, %rcx
	leaq	8(%r9), %rdx
	cmpl	%ebx, (%rsp)
	jle	.L778
	testl	%r8d, %r8d
	je	.L784
	cmpl	$1, %r8d
	je	.L822
	cmpl	$2, %r8d
	je	.L823
	cmpl	$3, %r8d
	je	.L824
	cmpl	$4, %r8d
	je	.L825
	cmpl	$5, %r8d
	je	.L826
	cmpl	$6, %r8d
	je	.L827
	movq	%rcx, (%rdx)
	leal	2(%rdi), %ebx
	addq	%rsi, %rcx
	leaq	16(%r9), %rdx
.L827:
	movq	%rcx, (%rdx)
	addl	$1, %ebx
	addq	%rsi, %rcx
	addq	$8, %rdx
.L826:
	movq	%rcx, (%rdx)
	addl	$1, %ebx
	addq	%rsi, %rcx
	addq	$8, %rdx
.L825:
	movq	%rcx, (%rdx)
	addl	$1, %ebx
	addq	%rsi, %rcx
	addq	$8, %rdx
.L824:
	movq	%rcx, (%rdx)
	addl	$1, %ebx
	addq	%rsi, %rcx
	addq	$8, %rdx
.L823:
	movq	%rcx, (%rdx)
	addl	$1, %ebx
	addq	%rsi, %rcx
	addq	$8, %rdx
.L822:
	movq	%rcx, (%rdx)
	addl	$1, %ebx
	addq	%rsi, %rcx
	addq	$8, %rdx
	cmpl	%ebx, (%rsp)
	jle	.L778
.L784:
	movq	%rcx, (%rdx)
	addq	%rsi, %rcx
	movq	%rcx, 8(%rdx)
	addq	%rsi, %rcx
	movq	%rcx, 16(%rdx)
	addq	%rsi, %rcx
	movq	%rcx, 24(%rdx)
	addq	%rsi, %rcx
	movq	%rcx, 32(%rdx)
	addq	%rsi, %rcx
	movq	%rcx, 40(%rdx)
	addq	%rsi, %rcx
	movq	%rcx, 48(%rdx)
	addq	%rsi, %rcx
	movq	%rcx, 56(%rdx)
	addl	$8, %ebx
	addq	%rsi, %rcx
	addq	$64, %rdx
	cmpl	%ebx, (%rsp)
	jg	.L784
.L778:
	leaq	32(%rsp), %rbx
	movl	(%rsp), %edx
	movq	%rax, %rsi
	movq	%rbx, %rdi
	call	jpeg_write_scanlines
	cmpl	%eax, (%rsp)
	jg	.L831
	movq	%rbx, %rdi
	call	jpeg_finish_compress
	movq	%rbx, %rdi
	call	jpeg_destroy_compress
	movq	8(%rsp), %rdi
	call	fclose
	xorl	%eax, %eax
	addq	$880, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L786:
	.cfi_restore_state
	movl	$5, %eax
	addq	$880, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L831:
	.cfi_restore_state
	movl	(%rsp), %edx
	movl	%eax, %esi
	movl	$.LC33, %edi
	xorl	%eax, %eax
	call	printf
	movq	8(%rsp), %rdi
	call	fclose
	movl	$1, %eax
	addq	$880, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L787:
	.cfi_restore_state
	xorl	%edi, %edi
	jmp	.L779
	.cfi_endproc
.LFE43:
	.size	save_jpeg_file, .-save_jpeg_file
.globl save_jpeg_file_old
	.type	save_jpeg_file_old, @function
save_jpeg_file_old:
.LFB44:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	subq	$896, %rsp
	.cfi_def_cfa_offset 928
	movq	%rdi, %rbx
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movl	%esi, 12(%rsp)
	movl	%edx, 24(%rsp)
	movl	$.LC22, %esi
	movq	%rcx, %rdi
	call	fopen
	movq	%rax, 16(%rsp)
	testq	%rax, %rax
	je	.L839
	movl	12(%rsp), %edx
	leal	(%rdx,%rdx,2), %eax
	movl	%eax, 28(%rsp)
	movl	24(%rsp), %edi
	imull	%eax, %edi
	addl	$9, %edi
	movslq	%edi, %rdi
	call	malloc
	movq	%rax, (%rsp)
	movl	ualoop(%rip), %ebp
	testl	%ebp, %ebp
	jne	.L847
	movq	%rax, %rdi
	movl	24(%rsp), %ecx
	movl	12(%rsp), %edx
	movq	%rbx, %rsi
	call	jpeg_compute_pixels
.L835:
	leaq	624(%rsp), %rbx
	movq	%rbx, %rdi
	call	jpeg_std_error
	movq	%rax, 32(%rsp)
	movq	$my_error_exit, 624(%rsp)
	movq	%rbx, errptr(%rip)
	leaq	168(%rbx), %rdi
	call	__setjmp14
	testl	%eax, %eax
	je	.L836
	leaq	32(%rsp), %rdi
	call	jpeg_destroy_compress
	movq	16(%rsp), %rdi
	call	fclose
	movl	$2, %eax
	addq	$896, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L847:
	.cfi_restore_state
	movl	24(%rsp), %ecx
	movl	12(%rsp), %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	compute_asm
	jmp	.L835
.L836:
	leaq	32(%rsp), %rbx
	movl	$584, %edx
	movl	$80, %esi
	movq	%rbx, %rdi
	call	jpeg_CreateCompress
	movq	16(%rsp), %rsi
	movq	%rbx, %rdi
	call	jpeg_stdio_dest
	movl	12(%rsp), %esi
	movl	%esi, 80(%rsp)
	movl	24(%rsp), %ecx
	movl	%ecx, 84(%rsp)
	movl	$3, 88(%rsp)
	movl	$2, 92(%rsp)
	movq	%rbx, %rdi
	call	jpeg_set_defaults
	xorl	%edx, %edx
	movl	jqual(%rip), %esi
	movq	%rbx, %rdi
	call	jpeg_set_quality
	movl	$1, %esi
	movq	%rbx, %rdi
	call	jpeg_start_compress
	movl	372(%rsp), %eax
	cmpl	%eax, 84(%rsp)
	jbe	.L837
	movl	28(%rsp), %r12d
	leaq	880(%rsp), %rbp
.L838:
	imull	%r12d, %eax
	addq	(%rsp), %rax
	movq	%rax, 880(%rsp)
	movl	$1, %edx
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	jpeg_write_scanlines
	movl	372(%rsp), %eax
	cmpl	84(%rsp), %eax
	jae	.L837
	imull	%r12d, %eax
	addq	(%rsp), %rax
	movq	%rax, 880(%rsp)
	movl	$1, %edx
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	jpeg_write_scanlines
	movl	372(%rsp), %eax
	cmpl	84(%rsp), %eax
	jae	.L837
	imull	%r12d, %eax
	addq	(%rsp), %rax
	movq	%rax, 880(%rsp)
	movl	$1, %edx
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	jpeg_write_scanlines
	movl	372(%rsp), %eax
	cmpl	84(%rsp), %eax
	jae	.L837
	imull	%r12d, %eax
	addq	(%rsp), %rax
	movq	%rax, 880(%rsp)
	movl	$1, %edx
	movq	%rbp, %rsi
	movq	%rbx, %rdi
	call	jpeg_write_scanlines
	movl	372(%rsp), %eax
	cmpl	84(%rsp), %eax
	jb	.L838
.L837:
	leaq	32(%rsp), %r12
	movq	%r12, %rdi
	call	jpeg_finish_compress
	movq	%r12, %rdi
	call	jpeg_destroy_compress
	movq	16(%rsp), %rdi
	call	fclose
	xorl	%eax, %eax
	addq	$896, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L839:
	.cfi_restore_state
	movl	$5, %eax
	addq	$896, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE44:
	.size	save_jpeg_file_old, .-save_jpeg_file_old
	.section	.rodata.str1.1
.LC34:
	.string	"%d.%06ds "
	.text
.globl translate_jpeg_file
	.type	translate_jpeg_file, @function
translate_jpeg_file:
.LFB45:
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
	subq	$216, %rsp
	.cfi_def_cfa_offset 272
	movl	%edi, %ebp
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%rsi, %rbx
	movq	%rdx, %r13
	movl	%ecx, %r12d
	movl	%r8d, 44(%rsp)
	movl	%r9d, 68(%rsp)
	movl	280(%rsp), %r14d
	testl	%r14d, %r14d
	je	.L849
	movl	ualoop(%rip), %r8d
	testl	%r8d, %r8d
	je	.L850
	cmpl	$4, %r14d
	je	.L851
	cmpl	$11, %r14d
	je	.L851
	cmpl	$12, %r14d
	je	.L851
.L850:
	movl	44(%rsp), %eax
	addl	%r12d, %eax
	sarl	%eax
	movl	68(%rsp), %ecx
	movl	%ecx, 192(%rsp)
	movl	192(%rsp), %edx
	cmpl	%edx, 44(%rsp)
	jle	.L848
	cltq
	movq	%rax, 128(%rsp)
	leal	-1(%r12), %eax
	movl	%eax, 56(%rsp)
	movl	44(%rsp), %esi
	subl	$1, %esi
	movl	%esi, 68(%rsp)
.L950:
	movl	192(%rsp), %eax
	cmpl	$4, %r14d
	je	.L1462
	cmpl	$11, %r14d
	je	.L1463
	cmpl	$12, %r14d
	je	.L1464
	cmpl	$1, %r14d
	je	.L1465
	cmpl	$2, %r14d
	je	.L1466
	cmpl	$3, %r14d
	je	.L1467
	cmpl	$5, %r14d
	je	.L1468
	cmpl	$6, %r14d
	je	.L1469
	cmpl	$7, %r14d
	je	.L1470
	cmpl	$8, %r14d
	je	.L1471
	cmpl	$9, %r14d
	je	.L1472
	cmpl	$10, %r14d
	jne	.L855
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
	leaq	8(%rbx), %r15
	leaq	16(%rbx), %rcx
	leaq	40(%rbx), %rbp
	leaq	32(%rbx), %r10
	movq	%r10, 96(%rsp)
	leaq	24(%rbx), %r11
	movq	%r11, 104(%rsp)
	movl	%r14d, 140(%rsp)
	movl	%r12d, %r14d
	movq	%rcx, %r12
.L903:
	movl	192(%rsp), %eax
	movl	176(%rsp), %r11d
	cltq
	movslq	%r11d, %r11
	salq	$3, %r11
	addq	0(%r13,%rax,8), %r11
	movl	192(%rsp), %eax
	movl	176(%rsp), %r8d
	movl	192(%rsp), %edi
	movl	176(%rsp), %r9d
	movl	192(%rsp), %ecx
	movl	176(%rsp), %esi
	movl	192(%rsp), %edx
	movl	176(%rsp), %r10d
	movl	%r10d, 48(%rsp)
	movl	192(%rsp), %r10d
	movl	%r10d, 72(%rsp)
	movl	176(%rsp), %r10d
	movl	%r10d, 80(%rsp)
	movl	192(%rsp), %r10d
	movl	%r10d, 84(%rsp)
	movl	176(%rsp), %r10d
	movl	%r10d, 88(%rsp)
	cltq
	movq	0(%rbp), %r10
	movslq	%r8d, %r8
	movq	(%r10,%rax,8), %rax
	movslq	%edi, %rdi
	movq	96(%rsp), %r10
	movq	(%r10), %r10
	movslq	%r9d, %r9
	movq	%r9, 112(%rsp)
	movq	(%r10,%rdi,8), %r9
	movq	%r9, 120(%rsp)
	movslq	%ecx, %r10
	movq	104(%rsp), %rcx
	movq	(%rcx), %rdi
	movslq	%esi, %r9
	movq	(%rdi,%r10,8), %rcx
	movq	(%rcx,%r9,8), %rcx
	movslq	%edx, %r10
	movq	(%r12), %rdi
	movslq	48(%rsp), %rsi
	movq	(%rdi,%r10,8), %rdx
	movq	(%rdx,%rsi,8), %rdx
	movslq	72(%rsp), %r10
	movq	(%r15), %r9
	movslq	80(%rsp), %rdi
	movq	(%r9,%r10,8), %rsi
	movq	(%rsi,%rdi,8), %rsi
	movslq	84(%rsp), %rdi
	movq	(%rbx), %r10
	movslq	88(%rsp), %r9
	movq	(%r10,%rdi,8), %rdi
	movq	(%rdi,%r9,8), %rdi
	movq	(%rax,%r8,8), %r9
	movq	112(%rsp), %r10
	movq	120(%rsp), %rax
	movq	(%rax,%r10,8), %r8
	movq	%r11, 32(%rsp)
	call	udfi6
	movq	32(%rsp), %rsi
	movq	%rax, (%rsi)
	movl	176(%rsp), %edx
	addl	$1, %edx
	movl	%edx, 176(%rsp)
	movl	176(%rsp), %r8d
	cmpl	%r8d, %r14d
	jg	.L903
	movl	%r14d, %r12d
	movl	140(%rsp), %r14d
.L855:
	movl	192(%rsp), %ebp
	addl	272(%rsp), %ebp
	movl	%ebp, 192(%rsp)
	movl	192(%rsp), %r15d
	cmpl	%r15d, 44(%rsp)
	jg	.L950
.L848:
	addq	$216, %rsp
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
.L1462:
	.cfi_restore_state
	movl	$0, 176(%rsp)
	movl	176(%rsp), %edi
	cmpl	%edi, %r12d
	jg	.L951
	jmp	.L855
.L1205:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ecx
	cltq
	movslq	%ecx, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %edi
	movl	176(%rsp), %r11d
	movslq	%edi, %r8
	movq	(%rbx), %rsi
	movslq	%r11d, %rdx
	movq	(%rsi,%r8,8), %r10
	movq	(%r10,%rdx,8), %rdi
	call	asmf1
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
.L951:
	movl	192(%rsp), %eax
	movl	176(%rsp), %edi
	cltq
	movslq	%edi, %r15
	salq	$3, %r15
	addq	0(%r13,%rax,8), %r15
	movl	192(%rsp), %esi
	movl	176(%rsp), %edx
	movslq	%esi, %rcx
	movq	(%rbx), %r8
	movslq	%edx, %r11
	movq	(%r8,%rcx,8), %r10
	movq	(%r10,%r11,8), %rdi
	call	asmf1
	movq	%rax, (%r15)
	movl	176(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, 176(%rsp)
	movl	176(%rsp), %ebp
	cmpl	%ebp, %r12d
	jg	.L1205
	jmp	.L855
.L1463:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %ebp
	cmpl	%ebp, %r12d
	jle	.L855
	leaq	8(%rbx), %r15
	jmp	.L858
.L1206:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebp
	cltq
	movslq	%ebp, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %edx
	movl	176(%rsp), %r10d
	movl	192(%rsp), %r11d
	movl	176(%rsp), %eax
	movslq	%edx, %rdi
	movq	(%r15), %r9
	movslq	%r10d, %rcx
	movq	(%r9,%rdi,8), %rsi
	movq	(%rsi,%rcx,8), %rsi
	movslq	%r11d, %rdx
	movq	(%rbx), %r8
	cltq
	movq	(%r8,%rdx,8), %r10
	movq	(%r10,%rax,8), %rdi
	call	asmf2
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %ebp
	addl	$1, %ebp
	movl	%ebp, 176(%rsp)
	movl	176(%rsp), %r9d
	cmpl	%r9d, %r12d
	jle	.L855
.L858:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebp
	cltq
	movslq	%ebp, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %r8d
	movl	176(%rsp), %r11d
	movl	192(%rsp), %edx
	movl	176(%rsp), %eax
	movslq	%r8d, %rdi
	movq	(%r15), %r10
	movslq	%r11d, %r9
	movq	(%r10,%rdi,8), %rsi
	movq	(%rsi,%r9,8), %rsi
	movslq	%edx, %r8
	movq	(%rbx), %rcx
	cltq
	movq	(%rcx,%r8,8), %r11
	movq	(%r11,%rax,8), %rdi
	call	asmf2
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, 176(%rsp)
	movl	176(%rsp), %r9d
	cmpl	%r9d, %r12d
	jg	.L1206
	jmp	.L855
.L1464:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
	leaq	8(%rbx), %rcx
	leaq	16(%rbx), %rdx
	movl	%r14d, 48(%rsp)
	movl	%r12d, %r14d
	movq	%rbx, %r12
	movq	%rdx, %rbx
	movq	%rcx, %r15
	jmp	.L860
.L1207:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebp
	cltq
	movslq	%ebp, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %edx
	movl	176(%rsp), %ecx
	movl	192(%rsp), %edi
	movl	176(%rsp), %esi
	movl	192(%rsp), %r10d
	movl	176(%rsp), %eax
	movslq	%edx, %r11
	movq	(%rbx), %r8
	movslq	%ecx, %rdx
	movq	(%r8,%r11,8), %r9
	movq	(%r9,%rdx,8), %rdx
	movslq	%edi, %r11
	movq	(%r15), %rcx
	movslq	%esi, %rsi
	movq	(%rcx,%r11,8), %r8
	movq	(%r8,%rsi,8), %rsi
	movslq	%r10d, %r9
	movq	(%r12), %rdi
	cltq
	movq	(%rdi,%r9,8), %rcx
	movq	(%rcx,%rax,8), %rdi
	call	asmf3
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %r11d
	addl	$1, %r11d
	movl	%r11d, 176(%rsp)
	movl	176(%rsp), %r8d
	cmpl	%r8d, %r14d
	jle	.L1461
.L860:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebp
	cltq
	movslq	%ebp, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %edx
	movl	176(%rsp), %r10d
	movl	192(%rsp), %edi
	movl	176(%rsp), %r11d
	movl	192(%rsp), %ecx
	movl	176(%rsp), %eax
	movslq	%edx, %r9
	movq	(%rbx), %r8
	movslq	%r10d, %rdx
	movq	(%r8,%r9,8), %rsi
	movq	(%rsi,%rdx,8), %rdx
	movslq	%edi, %r9
	movq	(%r15), %r10
	movslq	%r11d, %rsi
	movq	(%r10,%r9,8), %r8
	movq	(%r8,%rsi,8), %rsi
	movslq	%ecx, %r11
	movq	(%r12), %rdi
	cltq
	movq	(%rdi,%r11,8), %r10
	movq	(%r10,%rax,8), %rdi
	call	asmf3
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, 176(%rsp)
	movl	176(%rsp), %esi
	cmpl	%esi, %r14d
	jg	.L1207
.L1461:
	movq	%r12, %rbx
	movl	%r14d, %r12d
	movl	48(%rsp), %r14d
	jmp	.L855
.L1465:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jg	.L866
	jmp	.L855
.L1473:
	cvttsd2siq	%xmm0, %r15
	movq	%r15, 0(%rbp)
.L865:
	movl	176(%rsp), %edi
	addl	$1, %edi
	movl	%edi, 176(%rsp)
	movl	176(%rsp), %ebp
	cmpl	%ebp, %r12d
	jle	.L855
.L866:
	movl	192(%rsp), %eax
	movl	176(%rsp), %r10d
	cltq
	movslq	%r10d, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %r9d
	movl	176(%rsp), %r8d
	movslq	%r9d, %rsi
	movq	(%rbx), %rdi
	movslq	%r8d, %r11
	movq	(%rdi,%rsi,8), %rcx
	movq	(%rcx,%r11,8), %rax
	testq	%rax, %rax
	js	.L862
	cvtsi2sdq	%rax, %xmm0
.L863:
	call	udf1
	comisd	.LC35(%rip), %xmm0
	jb	.L1473
	subsd	.LC35(%rip), %xmm0
	cvttsd2siq	%xmm0, %r11
	movq	%r11, 0(%rbp)
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, 0(%rbp)
	jmp	.L865
.L862:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L863
.L851:
	leaq	160(%rsp), %rdi
	xorl	%esi, %esi
	call	__gettimeofday50
	movq	%r13, 8(%rsp)
	movq	%rbx, (%rsp)
	movl	272(%rsp), %r9d
	movl	68(%rsp), %r8d
	movl	44(%rsp), %ecx
	movl	%r12d, %edx
	movl	%ebp, %esi
	movl	%r14d, %edi
	call	loop_asm
	leaq	144(%rsp), %rdi
	xorl	%esi, %esi
	call	__gettimeofday50
	movq	144(%rsp), %rsi
	subq	160(%rsp), %rsi
	movl	152(%rsp), %edx
	subl	168(%rsp), %edx
	js	.L1474
.L852:
	movl	$.LC34, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L848
.L1466:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %esi
	cmpl	%esi, %r12d
	jle	.L855
	leaq	8(%rbx), %r15
	jmp	.L874
.L1475:
	cvttsd2siq	%xmm0, %rcx
	movq	%rcx, 0(%rbp)
.L873:
	movl	176(%rsp), %edx
	addl	$1, %edx
	movl	%edx, 176(%rsp)
	movl	176(%rsp), %ebp
	cmpl	%ebp, %r12d
	jle	.L855
.L874:
	movl	192(%rsp), %eax
	movl	176(%rsp), %edx
	cltq
	movslq	%edx, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %edi
	movl	176(%rsp), %ecx
	movl	192(%rsp), %edx
	movl	176(%rsp), %eax
	movslq	%edi, %r11
	movq	(%r15), %r10
	movslq	%ecx, %r8
	movq	(%r10,%r11,8), %r9
	movq	(%r9,%r8,8), %rcx
	testq	%rcx, %rcx
	js	.L868
	cvtsi2sdq	%rcx, %xmm1
.L869:
	movslq	%edx, %r9
	movq	(%rbx), %r10
	cltq
	movq	(%r10,%r9,8), %r8
	movq	(%r8,%rax,8), %rax
	testq	%rax, %rax
	js	.L870
	cvtsi2sdq	%rax, %xmm0
.L871:
	call	udf2
	comisd	.LC35(%rip), %xmm0
	jb	.L1475
	subsd	.LC35(%rip), %xmm0
	cvttsd2siq	%xmm0, %rax
	movq	%rax, 0(%rbp)
	movabsq	$-9223372036854775808, %rdi
	xorq	%rdi, 0(%rbp)
	jmp	.L873
.L870:
	movq	%rax, %r11
	shrq	%r11
	andl	$1, %eax
	orq	%rax, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L871
.L868:
	movq	%rcx, %rsi
	shrq	%rsi
	andl	$1, %ecx
	orq	%rcx, %rsi
	cvtsi2sdq	%rsi, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L869
.L1467:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
	leaq	8(%rbx), %rsi
	leaq	16(%rbx), %r15
	movl	%r14d, 48(%rsp)
	movl	%r12d, %r14d
	movq	%rbx, %r12
	movq	%rsi, %rbx
	jmp	.L884
.L1476:
	cvttsd2siq	%xmm0, %rax
	movq	%rax, 0(%rbp)
.L883:
	movl	176(%rsp), %edi
	addl	$1, %edi
	movl	%edi, 176(%rsp)
	movl	176(%rsp), %ebp
	cmpl	%ebp, %r14d
	jle	.L1461
.L884:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ecx
	cltq
	movslq	%ecx, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %r10d
	movl	176(%rsp), %edi
	movl	192(%rsp), %esi
	movl	176(%rsp), %ecx
	movl	192(%rsp), %edx
	movl	176(%rsp), %eax
	movslq	%r10d, %r9
	movq	(%r15), %r11
	movslq	%edi, %r10
	movq	(%r11,%r9,8), %r8
	movq	(%r8,%r10,8), %rdi
	testq	%rdi, %rdi
	js	.L876
	cvtsi2sdq	%rdi, %xmm2
.L877:
	movslq	%esi, %r9
	movq	(%rbx), %rdi
	movslq	%ecx, %r11
	movq	(%rdi,%r9,8), %rsi
	movq	(%rsi,%r11,8), %rcx
	testq	%rcx, %rcx
	js	.L878
	cvtsi2sdq	%rcx, %xmm1
.L879:
	movslq	%edx, %r8
	movq	(%r12), %rcx
	cltq
	movq	(%rcx,%r8,8), %rdx
	movq	(%rdx,%rax,8), %rax
	testq	%rax, %rax
	js	.L880
	cvtsi2sdq	%rax, %xmm0
.L881:
	call	udf3
	comisd	.LC35(%rip), %xmm0
	jb	.L1476
	subsd	.LC35(%rip), %xmm0
	cvttsd2siq	%xmm0, %r9
	movq	%r9, 0(%rbp)
	movabsq	$-9223372036854775808, %rsi
	xorq	%rsi, 0(%rbp)
	jmp	.L883
.L880:
	movq	%rax, %r11
	shrq	%r11
	andl	$1, %eax
	orq	%rax, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L881
.L878:
	movq	%rcx, %r10
	shrq	%r10
	andl	$1, %ecx
	orq	%rcx, %r10
	cvtsi2sdq	%r10, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L879
.L876:
	movq	%rdi, %r8
	shrq	%r8
	andl	$1, %edi
	orq	%rdi, %r8
	cvtsi2sdq	%r8, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L877
.L849:
	movl	$72, %edi
	call	malloc
	movq	%rax, %r14
	movl	$0, 192(%rsp)
	movl	192(%rsp), %edx
	cmpl	$8, %edx
	jg	.L905
	leal	1(%rbp), %esi
	movslq	%esi, %r15
	salq	$3, %r15
	movl	%r12d, 48(%rsp)
	movq	%rbx, %r12
.L906:
	movl	192(%rsp), %ebx
	movq	%r15, %rdi
	call	malloc
	movslq	%ebx, %r9
	movq	%rax, (%r14,%r9,8)
	movl	192(%rsp), %ecx
	addl	$1, %ecx
	movl	%ecx, 192(%rsp)
	movl	192(%rsp), %ebx
	cmpl	$8, %ebx
	jg	.L1460
	movl	192(%rsp), %ebx
	movq	%r15, %rdi
	call	malloc
	movslq	%ebx, %rdx
	movq	%rax, (%r14,%rdx,8)
	movl	192(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, 192(%rsp)
	movl	192(%rsp), %esi
	cmpl	$8, %esi
	jg	.L1460
	movl	192(%rsp), %ebx
	movq	%r15, %rdi
	call	malloc
	movslq	%ebx, %rdi
	movq	%rax, (%r14,%rdi,8)
	movl	192(%rsp), %eax
	addl	$1, %eax
	movl	%eax, 192(%rsp)
	movl	192(%rsp), %ecx
	cmpl	$8, %ecx
	jg	.L1460
	movl	192(%rsp), %ebx
	movq	%r15, %rdi
	call	malloc
	movslq	%ebx, %r11
	movq	%rax, (%r14,%r11,8)
	movl	192(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, 192(%rsp)
	movl	192(%rsp), %r8d
	cmpl	$8, %r8d
	jle	.L906
.L1460:
	movq	%r12, %rbx
	movl	48(%rsp), %r12d
.L905:
	cvtsi2sd	%ebp, %xmm5
	movsd	%xmm5, 72(%rsp)
	movslq	%ebp, %r15
	leaq	0(,%r15,8), %r11
	movq	%r11, 48(%rsp)
	movq	%r11, %rax
	addq	(%r14), %rax
	movq	%rax, 32(%rsp)
	call	rand
	movl	$16777216, %r10d
	cltd
	idivl	%r10d
	cvtsi2sd	%edx, %xmm4
	movq	32(%rsp), %r8
	movsd	%xmm4, (%r8)
	leaq	16(%r14), %rdi
	movq	%rdi, 104(%rsp)
	movq	16(%r14), %r9
	cvtsi2sd	%r12d, %xmm3
	movsd	%xmm3, (%r9,%r15,8)
	leaq	32(%r14), %rcx
	movq	%rcx, 112(%rsp)
	movq	32(%r14), %rsi
	cvtsi2sd	44(%rsp), %xmm2
	movsd	%xmm2, (%rsi,%r15,8)
	leaq	40(%r14), %rdx
	movq	%rdx, 120(%rsp)
	movq	40(%r14), %r11
	cvtsi2sd	68(%rsp), %xmm1
	movsd	%xmm1, (%r11,%r15,8)
	leaq	48(%r14), %rax
	movq	%rax, 128(%rsp)
	movq	48(%r14), %r10
	cvtsi2sd	272(%rsp), %xmm0
	movsd	%xmm0, (%r10,%r15,8)
	movl	68(%rsp), %r8d
	movl	%r8d, 192(%rsp)
	movl	192(%rsp), %edi
	cmpl	%edi, 44(%rsp)
	jle	.L907
	leaq	24(%r14), %r9
	movq	%r9, 96(%rsp)
	movl	44(%rsp), %ecx
	subl	$1, %ecx
	movl	%ecx, 80(%rsp)
	leal	-1(%r12), %esi
	movl	%esi, 84(%rsp)
	leal	-1(%rbp), %r15d
	leaq	8(,%r15,8), %r15
	movq	%r13, 88(%rsp)
.L944:
	movl	192(%rsp), %eax
	movl	192(%rsp), %edx
	movq	96(%rsp), %r10
	movq	(%r10), %rdi
	cvtsi2sd	%edx, %xmm6
	movq	48(%rsp), %r8
	movsd	%xmm6, (%rdi,%r8)
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r13d
	cmpl	%r13d, %r12d
	jle	.L908
	leaq	8(%r14), %rax
	movq	%rax, 56(%rsp)
.L943:
	movl	192(%rsp), %r9d
	addl	$1, %r9d
	movl	176(%rsp), %ecx
	addl	$1, %ecx
	movl	192(%rsp), %eax
	subl	$1, %eax
	movl	176(%rsp), %edx
	subl	$1, %edx
	cmpl	44(%rsp), %r9d
	movl	$0, %r11d
	cmove	%r11d, %r9d
	cmpl	%r12d, %ecx
	cmove	%r11d, %ecx
	cmpl	$-1, %eax
	cmove	80(%rsp), %eax
	cmpl	$-1, %edx
	cmove	84(%rsp), %edx
	testl	%ebp, %ebp
	jle	.L1477
	movq	(%r14), %rsi
	leaq	-8(%r15), %r8
	shrq	$3, %r8
	andl	$3, %r8d
	movl	192(%rsp), %edi
	movl	176(%rsp), %r13d
	movslq	%edi, %r11
	movq	(%rbx), %r10
	movslq	%r13d, %r13
	movq	(%r10,%r11,8), %rdi
	movq	(%rdi,%r13,8), %rdi
	testq	%rdi, %rdi
	js	.L1478
	cvtsi2sdq	%rdi, %xmm0
.L1426:
	movsd	%xmm0, (%rsi)
	movl	$8, %edi
	cmpq	%r15, %rdi
	je	.L1427
	testq	%r8, %r8
	je	.L916
	cmpq	$1, %r8
	je	.L1252
	cmpq	$2, %r8
	je	.L1253
	movl	192(%rsp), %r11d
	movl	176(%rsp), %r8d
	movslq	%r11d, %r10
	movq	8(%rbx), %r13
	movslq	%r8d, %r8
	movq	0(%r13,%r10,8), %r11
	movq	(%r11,%r8,8), %r8
	testq	%r8, %r8
	js	.L1479
	cvtsi2sdq	%r8, %xmm0
.L1429:
	movsd	%xmm0, 8(%rsi)
	addq	$8, %rdi
.L1253:
	movl	192(%rsp), %r10d
	movl	176(%rsp), %r8d
	movslq	%r10d, %r11
	movq	(%rbx,%rdi), %r13
	movslq	%r8d, %r8
	movq	0(%r13,%r11,8), %r10
	movq	(%r10,%r8,8), %r8
	testq	%r8, %r8
	js	.L1480
	cvtsi2sdq	%r8, %xmm0
.L1431:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
.L1252:
	movl	192(%rsp), %r11d
	movl	176(%rsp), %r8d
	movslq	%r11d, %r13
	movq	(%rbx,%rdi), %r10
	movslq	%r8d, %r8
	movq	(%r10,%r13,8), %r11
	movq	(%r11,%r8,8), %r8
	testq	%r8, %r8
	js	.L1481
	cvtsi2sdq	%r8, %xmm0
.L1433:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
	cmpq	%r15, %rdi
	jne	.L916
	jmp	.L1427
.L1485:
	cvtsi2sdq	%r8, %xmm0
.L915:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
	movl	192(%rsp), %r11d
	movl	176(%rsp), %r8d
	movslq	%r11d, %r13
	movq	(%rbx,%rdi), %r10
	movslq	%r8d, %r8
	movq	(%r10,%r13,8), %r11
	movq	(%r11,%r8,8), %r8
	testq	%r8, %r8
	js	.L1482
	cvtsi2sdq	%r8, %xmm0
.L1434:
	movsd	%xmm0, (%rsi,%rdi)
	leaq	8(%rdi), %r8
	movl	192(%rsp), %r11d
	movl	176(%rsp), %r10d
	movslq	%r11d, %r13
	movq	(%rbx,%r8), %r11
	movslq	%r10d, %r10
	movq	(%r11,%r13,8), %r11
	movq	(%r11,%r10,8), %r10
	testq	%r10, %r10
	js	.L1483
	cvtsi2sdq	%r10, %xmm0
.L1436:
	movsd	%xmm0, (%rsi,%r8)
	leaq	16(%rdi), %r8
	movl	192(%rsp), %r11d
	movl	176(%rsp), %r10d
	movslq	%r11d, %r13
	movq	(%rbx,%r8), %r11
	movslq	%r10d, %r10
	movq	(%r11,%r13,8), %r11
	movq	(%r11,%r10,8), %r10
	testq	%r10, %r10
	js	.L1484
	cvtsi2sdq	%r10, %xmm0
.L1438:
	movsd	%xmm0, (%rsi,%r8)
	addq	$24, %rdi
	cmpq	%r15, %rdi
	je	.L1427
.L916:
	movl	192(%rsp), %r10d
	movl	176(%rsp), %r8d
	movslq	%r10d, %r11
	movq	(%rbx,%rdi), %r13
	movslq	%r8d, %r8
	movq	0(%r13,%r11,8), %r10
	movq	(%r10,%r8,8), %r8
	testq	%r8, %r8
	jns	.L1485
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L915
.L1427:
	movq	56(%rsp), %rsi
	movq	(%rsi), %rsi
	cltq
	salq	$3, %rax
	movslq	%edx, %rdx
	salq	$3, %rdx
	leaq	-8(%r15), %r8
	shrq	$3, %r8
	andl	$7, %r8d
	movq	(%rbx), %r10
	movq	(%r10,%rax), %rdi
	movq	(%rdi,%rdx), %rdi
	testq	%rdi, %rdi
	js	.L1486
	cvtsi2sdq	%rdi, %xmm0
.L1397:
	movsd	%xmm0, (%rsi)
	movl	$8, %edi
	cmpq	%r15, %rdi
	je	.L1398
	testq	%r8, %r8
	je	.L919
	cmpq	$1, %r8
	je	.L1245
	cmpq	$2, %r8
	je	.L1246
	cmpq	$3, %r8
	je	.L1247
	cmpq	$4, %r8
	je	.L1248
	cmpq	$5, %r8
	je	.L1249
	cmpq	$6, %r8
	je	.L1250
	movq	8(%rbx), %r10
	movq	(%r10,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1487
	cvtsi2sdq	%r8, %xmm0
.L1400:
	movsd	%xmm0, 8(%rsi)
	addq	$8, %rdi
.L1250:
	movq	(%rbx,%rdi), %r8
	movq	(%r8,%rax), %r11
	movq	(%r11,%rdx), %r8
	testq	%r8, %r8
	js	.L1488
	cvtsi2sdq	%r8, %xmm0
.L1402:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
.L1249:
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%rax), %r13
	movq	0(%r13,%rdx), %r8
	testq	%r8, %r8
	js	.L1489
	cvtsi2sdq	%r8, %xmm0
.L1404:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
.L1248:
	movq	(%rbx,%rdi), %r13
	movq	0(%r13,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1490
	cvtsi2sdq	%r8, %xmm0
.L1406:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
.L1247:
	movq	(%rbx,%rdi), %r8
	movq	(%r8,%rax), %r10
	movq	(%r10,%rdx), %r8
	testq	%r8, %r8
	js	.L1491
	cvtsi2sdq	%r8, %xmm0
.L1408:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
.L1246:
	movq	(%rbx,%rdi), %r10
	movq	(%r10,%rax), %r11
	movq	(%r11,%rdx), %r8
	testq	%r8, %r8
	js	.L1492
	cvtsi2sdq	%r8, %xmm0
.L1410:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
.L1245:
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1493
	cvtsi2sdq	%r8, %xmm0
.L1412:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
	cmpq	%r15, %rdi
	jne	.L919
	jmp	.L1398
.L1501:
	cvtsi2sdq	%r8, %xmm0
.L918:
	movsd	%xmm0, (%rsi,%rdi)
	addq	$8, %rdi
	movq	(%rbx,%rdi), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rdx), %r8
	testq	%r8, %r8
	js	.L1494
	cvtsi2sdq	%r8, %xmm0
.L1413:
	movsd	%xmm0, (%rsi,%rdi)
	leaq	8(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1495
	cvtsi2sdq	%r8, %xmm0
.L1415:
	movsd	%xmm0, (%rsi,%r10)
	leaq	16(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1496
	cvtsi2sdq	%r8, %xmm0
.L1417:
	movsd	%xmm0, (%rsi,%r10)
	leaq	24(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1497
	cvtsi2sdq	%r8, %xmm0
.L1419:
	movsd	%xmm0, (%rsi,%r10)
	leaq	32(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1498
	cvtsi2sdq	%r8, %xmm0
.L1421:
	movsd	%xmm0, (%rsi,%r10)
	leaq	40(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1499
	cvtsi2sdq	%r8, %xmm0
.L1423:
	movsd	%xmm0, (%rsi,%r10)
	leaq	48(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1500
	cvtsi2sdq	%r8, %xmm0
.L1425:
	movsd	%xmm0, (%rsi,%r10)
	addq	$56, %rdi
	cmpq	%r15, %rdi
	je	.L1398
.L919:
	movq	(%rbx,%rdi), %r8
	movq	(%r8,%rax), %r13
	movq	0(%r13,%rdx), %r8
	testq	%r8, %r8
	jns	.L1501
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L918
.L1398:
	movq	104(%rsp), %rdi
	movq	(%rdi), %rdi
	leaq	-8(%r15), %r10
	shrq	$3, %r10
	andl	$3, %r10d
	movl	176(%rsp), %r8d
	movq	(%rbx), %r13
	movslq	%r8d, %r11
	movq	0(%r13,%rax), %r8
	movq	(%r8,%r11,8), %r8
	testq	%r8, %r8
	js	.L1502
	cvtsi2sdq	%r8, %xmm0
.L1384:
	movsd	%xmm0, (%rdi)
	movl	$8, %r8d
	cmpq	%r15, %r8
	je	.L1385
	testq	%r10, %r10
	je	.L922
	cmpq	$1, %r10
	je	.L1242
	cmpq	$2, %r10
	je	.L1243
	movl	176(%rsp), %r11d
	movq	8(%rbx), %r10
	movslq	%r11d, %r13
	movq	(%r10,%rax), %r10
	movq	(%r10,%r13,8), %r10
	testq	%r10, %r10
	js	.L1503
	cvtsi2sdq	%r10, %xmm0
.L1387:
	movsd	%xmm0, 8(%rdi)
	addq	$8, %r8
.L1243:
	movl	176(%rsp), %r10d
	movq	(%rbx,%r8), %r13
	movslq	%r10d, %r11
	movq	0(%r13,%rax), %r10
	movq	(%r10,%r11,8), %r10
	testq	%r10, %r10
	js	.L1504
	cvtsi2sdq	%r10, %xmm0
.L1389:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
.L1242:
	movl	176(%rsp), %r13d
	movq	(%rbx,%r8), %r10
	movslq	%r13d, %r11
	movq	(%r10,%rax), %r13
	movq	0(%r13,%r11,8), %r10
	testq	%r10, %r10
	js	.L1505
	cvtsi2sdq	%r10, %xmm0
.L1391:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
	cmpq	%r15, %r8
	jne	.L922
	jmp	.L1385
.L1509:
	cvtsi2sdq	%r10, %xmm0
.L921:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
	movl	176(%rsp), %r13d
	movq	(%rbx,%r8), %r10
	movslq	%r13d, %r11
	movq	(%r10,%rax), %r13
	movq	0(%r13,%r11,8), %r10
	testq	%r10, %r10
	js	.L1506
	cvtsi2sdq	%r10, %xmm0
.L1392:
	movsd	%xmm0, (%rdi,%r8)
	leaq	8(%r8), %r11
	movl	176(%rsp), %r13d
	movq	(%rbx,%r11), %r10
	movslq	%r13d, %r13
	movq	(%r10,%rax), %r10
	movq	(%r10,%r13,8), %r10
	testq	%r10, %r10
	js	.L1507
	cvtsi2sdq	%r10, %xmm0
.L1394:
	movsd	%xmm0, (%rdi,%r11)
	leaq	16(%r8), %r11
	movl	176(%rsp), %r13d
	movq	(%rbx,%r11), %r10
	movslq	%r13d, %r13
	movq	(%r10,%rax), %r10
	movq	(%r10,%r13,8), %r10
	testq	%r10, %r10
	js	.L1508
	cvtsi2sdq	%r10, %xmm0
.L1396:
	movsd	%xmm0, (%rdi,%r11)
	addq	$24, %r8
	cmpq	%r15, %r8
	je	.L1385
.L922:
	movl	176(%rsp), %r10d
	movq	(%rbx,%r8), %r13
	movslq	%r10d, %r11
	movq	0(%r13,%rax), %r10
	movq	(%r10,%r11,8), %r10
	testq	%r10, %r10
	jns	.L1509
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L921
.L1385:
	movq	96(%rsp), %rdi
	movq	(%rdi), %rdi
	movslq	%ecx, %rcx
	salq	$3, %rcx
	leaq	-8(%r15), %r10
	shrq	$3, %r10
	andl	$7, %r10d
	movq	(%rbx), %r11
	movq	(%r11,%rax), %r8
	movq	(%r8,%rcx), %r8
	testq	%r8, %r8
	js	.L1510
	cvtsi2sdq	%r8, %xmm0
.L1355:
	movsd	%xmm0, (%rdi)
	movl	$8, %r8d
	cmpq	%r15, %r8
	je	.L1356
	testq	%r10, %r10
	je	.L925
	cmpq	$1, %r10
	je	.L1235
	cmpq	$2, %r10
	je	.L1236
	cmpq	$3, %r10
	je	.L1237
	cmpq	$4, %r10
	je	.L1238
	cmpq	$5, %r10
	je	.L1239
	cmpq	$6, %r10
	je	.L1240
	movq	8(%rbx), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1511
	cvtsi2sdq	%r10, %xmm0
.L1358:
	movsd	%xmm0, 8(%rdi)
	addq	$8, %r8
.L1240:
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1512
	cvtsi2sdq	%r10, %xmm0
.L1360:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
.L1239:
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1513
	cvtsi2sdq	%r10, %xmm0
.L1362:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
.L1238:
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1514
	cvtsi2sdq	%r10, %xmm0
.L1364:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
.L1237:
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1515
	cvtsi2sdq	%r10, %xmm0
.L1366:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
.L1236:
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1516
	cvtsi2sdq	%r10, %xmm0
.L1368:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
.L1235:
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1517
	cvtsi2sdq	%r10, %xmm0
.L1370:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
	cmpq	%r15, %r8
	jne	.L925
	jmp	.L1356
.L1525:
	cvtsi2sdq	%r10, %xmm0
.L924:
	movsd	%xmm0, (%rdi,%r8)
	addq	$8, %r8
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1518
	cvtsi2sdq	%r10, %xmm0
.L1371:
	movsd	%xmm0, (%rdi,%r8)
	leaq	8(%r8), %r11
	movq	(%rbx,%r11), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1519
	cvtsi2sdq	%r10, %xmm0
.L1373:
	movsd	%xmm0, (%rdi,%r11)
	leaq	16(%r8), %r11
	movq	(%rbx,%r11), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1520
	cvtsi2sdq	%r10, %xmm0
.L1375:
	movsd	%xmm0, (%rdi,%r11)
	leaq	24(%r8), %r11
	movq	(%rbx,%r11), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1521
	cvtsi2sdq	%r10, %xmm0
.L1377:
	movsd	%xmm0, (%rdi,%r11)
	leaq	32(%r8), %r11
	movq	(%rbx,%r11), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1522
	cvtsi2sdq	%r10, %xmm0
.L1379:
	movsd	%xmm0, (%rdi,%r11)
	leaq	40(%r8), %r11
	movq	(%rbx,%r11), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1523
	cvtsi2sdq	%r10, %xmm0
.L1381:
	movsd	%xmm0, (%rdi,%r11)
	leaq	48(%r8), %r11
	movq	(%rbx,%r11), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	js	.L1524
	cvtsi2sdq	%r10, %xmm0
.L1383:
	movsd	%xmm0, (%rdi,%r11)
	addq	$56, %r8
	cmpq	%r15, %r8
	je	.L1356
.L925:
	movq	(%rbx,%r8), %r13
	movq	0(%r13,%rax), %r10
	movq	(%r10,%rcx), %r10
	testq	%r10, %r10
	jns	.L1525
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L924
.L1356:
	movq	112(%rsp), %rax
	movq	(%rax), %rax
	leaq	-8(%r15), %r8
	shrq	$3, %r8
	andl	$3, %r8d
	movl	192(%rsp), %r13d
	movslq	%r13d, %r10
	movq	(%rbx), %r11
	movq	(%r11,%r10,8), %rdi
	movq	(%rdi,%rdx), %rdi
	testq	%rdi, %rdi
	js	.L1526
	cvtsi2sdq	%rdi, %xmm0
.L1342:
	movsd	%xmm0, (%rax)
	movl	$8, %edi
	cmpq	%r15, %rdi
	je	.L1343
	testq	%r8, %r8
	je	.L928
	cmpq	$1, %r8
	je	.L1232
	cmpq	$2, %r8
	je	.L1233
	movl	192(%rsp), %r10d
	movslq	%r10d, %r13
	movq	8(%rbx), %r11
	movq	(%r11,%r13,8), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1527
	cvtsi2sdq	%r8, %xmm0
.L1345:
	movsd	%xmm0, 8(%rax)
	addq	$8, %rdi
.L1233:
	movl	192(%rsp), %r11d
	movslq	%r11d, %r10
	movq	(%rbx,%rdi), %r13
	movq	0(%r13,%r10,8), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1528
	cvtsi2sdq	%r8, %xmm0
.L1347:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
.L1232:
	movl	192(%rsp), %r13d
	movslq	%r13d, %r10
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%r10,8), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1529
	cvtsi2sdq	%r8, %xmm0
.L1349:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
	cmpq	%r15, %rdi
	jne	.L928
	jmp	.L1343
.L1533:
	cvtsi2sdq	%r8, %xmm0
.L927:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
	movl	192(%rsp), %r11d
	movslq	%r11d, %r10
	movq	(%rbx,%rdi), %r13
	movq	0(%r13,%r10,8), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1530
	cvtsi2sdq	%r8, %xmm0
.L1350:
	movsd	%xmm0, (%rax,%rdi)
	leaq	8(%rdi), %r10
	movl	192(%rsp), %r8d
	movslq	%r8d, %r11
	movq	(%rbx,%r10), %r13
	movq	0(%r13,%r11,8), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1531
	cvtsi2sdq	%r8, %xmm0
.L1352:
	movsd	%xmm0, (%rax,%r10)
	leaq	16(%rdi), %r10
	movl	192(%rsp), %r11d
	movslq	%r11d, %r13
	movq	(%rbx,%r10), %r8
	movq	(%r8,%r13,8), %r11
	movq	(%r11,%rdx), %r8
	testq	%r8, %r8
	js	.L1532
	cvtsi2sdq	%r8, %xmm0
.L1354:
	movsd	%xmm0, (%rax,%r10)
	addq	$24, %rdi
	cmpq	%r15, %rdi
	je	.L1343
.L928:
	movl	192(%rsp), %r13d
	movslq	%r13d, %r10
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%r10,8), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	jns	.L1533
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L927
.L1343:
	movq	120(%rsp), %rax
	movq	(%rax), %rax
	leaq	-8(%r15), %r8
	shrq	$3, %r8
	andl	$3, %r8d
	movl	192(%rsp), %r11d
	movslq	%r11d, %r10
	movq	(%rbx), %r13
	movq	0(%r13,%r10,8), %rdi
	movq	(%rdi,%rcx), %rdi
	testq	%rdi, %rdi
	js	.L1534
	cvtsi2sdq	%rdi, %xmm0
.L1329:
	movsd	%xmm0, (%rax)
	movl	$8, %edi
	cmpq	%r15, %rdi
	je	.L1330
	testq	%r8, %r8
	je	.L931
	cmpq	$1, %r8
	je	.L1229
	cmpq	$2, %r8
	je	.L1230
	movl	192(%rsp), %r10d
	movslq	%r10d, %r13
	movq	8(%rbx), %r11
	movq	(%r11,%r13,8), %r8
	movq	(%r8,%rcx), %r8
	testq	%r8, %r8
	js	.L1535
	cvtsi2sdq	%r8, %xmm0
.L1332:
	movsd	%xmm0, 8(%rax)
	addq	$8, %rdi
.L1230:
	movl	192(%rsp), %r11d
	movslq	%r11d, %r10
	movq	(%rbx,%rdi), %r13
	movq	0(%r13,%r10,8), %r8
	movq	(%r8,%rcx), %r8
	testq	%r8, %r8
	js	.L1536
	cvtsi2sdq	%r8, %xmm0
.L1334:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
.L1229:
	movl	192(%rsp), %r13d
	movslq	%r13d, %r10
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%r10,8), %r8
	movq	(%r8,%rcx), %r8
	testq	%r8, %r8
	js	.L1537
	cvtsi2sdq	%r8, %xmm0
.L1336:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
	cmpq	%r15, %rdi
	jne	.L931
	jmp	.L1330
.L1541:
	cvtsi2sdq	%r8, %xmm0
.L930:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
	movl	192(%rsp), %r13d
	movslq	%r13d, %r10
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%r10,8), %r8
	movq	(%r8,%rcx), %r8
	testq	%r8, %r8
	js	.L1538
	cvtsi2sdq	%r8, %xmm0
.L1337:
	movsd	%xmm0, (%rax,%rdi)
	leaq	8(%rdi), %r10
	movl	192(%rsp), %r8d
	movslq	%r8d, %r11
	movq	(%rbx,%r10), %r13
	movq	0(%r13,%r11,8), %r8
	movq	(%r8,%rcx), %r8
	testq	%r8, %r8
	js	.L1539
	cvtsi2sdq	%r8, %xmm0
.L1339:
	movsd	%xmm0, (%rax,%r10)
	leaq	16(%rdi), %r10
	movl	192(%rsp), %r11d
	movslq	%r11d, %r13
	movq	(%rbx,%r10), %r8
	movq	(%r8,%r13,8), %r11
	movq	(%r11,%rcx), %r8
	testq	%r8, %r8
	js	.L1540
	cvtsi2sdq	%r8, %xmm0
.L1341:
	movsd	%xmm0, (%rax,%r10)
	addq	$24, %rdi
	cmpq	%r15, %rdi
	je	.L1330
.L931:
	movl	192(%rsp), %r11d
	movslq	%r11d, %r10
	movq	(%rbx,%rdi), %r13
	movq	0(%r13,%r10,8), %r8
	movq	(%r8,%rcx), %r8
	testq	%r8, %r8
	jns	.L1541
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L930
.L1330:
	movq	128(%rsp), %rax
	movq	(%rax), %rax
	movslq	%r9d, %r9
	salq	$3, %r9
	leaq	-8(%r15), %r8
	shrq	$3, %r8
	andl	$7, %r8d
	movq	(%rbx), %r11
	movq	(%r11,%r9), %rdi
	movq	(%rdi,%rdx), %rdi
	testq	%rdi, %rdi
	js	.L1542
	cvtsi2sdq	%rdi, %xmm0
.L1300:
	movsd	%xmm0, (%rax)
	movl	$8, %edi
	cmpq	%r15, %rdi
	je	.L1301
	testq	%r8, %r8
	je	.L934
	cmpq	$1, %r8
	je	.L1222
	cmpq	$2, %r8
	je	.L1223
	cmpq	$3, %r8
	je	.L1224
	cmpq	$4, %r8
	je	.L1225
	cmpq	$5, %r8
	je	.L1226
	cmpq	$6, %r8
	je	.L1227
	movq	8(%rbx), %r10
	movq	(%r10,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1543
	cvtsi2sdq	%r8, %xmm0
.L1303:
	movsd	%xmm0, 8(%rax)
	addq	$8, %rdi
.L1227:
	movq	(%rbx,%rdi), %r8
	movq	(%r8,%r9), %r11
	movq	(%r11,%rdx), %r8
	testq	%r8, %r8
	js	.L1544
	cvtsi2sdq	%r8, %xmm0
.L1305:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
.L1226:
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%r9), %r13
	movq	0(%r13,%rdx), %r8
	testq	%r8, %r8
	js	.L1545
	cvtsi2sdq	%r8, %xmm0
.L1307:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
.L1225:
	movq	(%rbx,%rdi), %r13
	movq	0(%r13,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1546
	cvtsi2sdq	%r8, %xmm0
.L1309:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
.L1224:
	movq	(%rbx,%rdi), %r8
	movq	(%r8,%r9), %r10
	movq	(%r10,%rdx), %r8
	testq	%r8, %r8
	js	.L1547
	cvtsi2sdq	%r8, %xmm0
.L1311:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
.L1223:
	movq	(%rbx,%rdi), %r10
	movq	(%r10,%r9), %r11
	movq	(%r11,%rdx), %r8
	testq	%r8, %r8
	js	.L1548
	cvtsi2sdq	%r8, %xmm0
.L1313:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
.L1222:
	movq	(%rbx,%rdi), %r11
	movq	(%r11,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1549
	cvtsi2sdq	%r8, %xmm0
.L1315:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
	cmpq	%r15, %rdi
	jne	.L934
	jmp	.L1301
.L1557:
	cvtsi2sdq	%r8, %xmm0
.L933:
	movsd	%xmm0, (%rax,%rdi)
	addq	$8, %rdi
	movq	(%rbx,%rdi), %r8
	movq	(%r8,%r9), %r11
	movq	(%r11,%rdx), %r8
	testq	%r8, %r8
	js	.L1550
	cvtsi2sdq	%r8, %xmm0
.L1316:
	movsd	%xmm0, (%rax,%rdi)
	leaq	8(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1551
	cvtsi2sdq	%r8, %xmm0
.L1318:
	movsd	%xmm0, (%rax,%r10)
	leaq	16(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1552
	cvtsi2sdq	%r8, %xmm0
.L1320:
	movsd	%xmm0, (%rax,%r10)
	leaq	24(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1553
	cvtsi2sdq	%r8, %xmm0
.L1322:
	movsd	%xmm0, (%rax,%r10)
	leaq	32(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1554
	cvtsi2sdq	%r8, %xmm0
.L1324:
	movsd	%xmm0, (%rax,%r10)
	leaq	40(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1555
	cvtsi2sdq	%r8, %xmm0
.L1326:
	movsd	%xmm0, (%rax,%r10)
	leaq	48(%rdi), %r10
	movq	(%rbx,%r10), %r11
	movq	(%r11,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	js	.L1556
	cvtsi2sdq	%r8, %xmm0
.L1328:
	movsd	%xmm0, (%rax,%r10)
	addq	$56, %rdi
	cmpq	%r15, %rdi
	je	.L1301
.L934:
	movq	(%rbx,%rdi), %r10
	movq	(%r10,%r9), %r8
	movq	(%r8,%rdx), %r8
	testq	%r8, %r8
	jns	.L1557
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L933
.L1301:
	movq	56(%r14), %rax
	leaq	-8(%r15), %rdi
	shrq	$3, %rdi
	andl	$3, %edi
	movl	176(%rsp), %r11d
	movq	(%rbx), %r13
	movslq	%r11d, %r10
	movq	0(%r13,%r9), %rdx
	movq	(%rdx,%r10,8), %rdx
	testq	%rdx, %rdx
	js	.L1558
	cvtsi2sdq	%rdx, %xmm0
.L1287:
	movsd	%xmm0, (%rax)
	movl	$8, %edx
	cmpq	$8, %r15
	je	.L1288
	testq	%rdi, %rdi
	je	.L937
	cmpq	$1, %rdi
	je	.L1219
	cmpq	$2, %rdi
	je	.L1220
	movl	176(%rsp), %r13d
	movq	8(%rbx), %r11
	movslq	%r13d, %r8
	movq	(%r11,%r9), %rdi
	movq	(%rdi,%r8,8), %rdi
	testq	%rdi, %rdi
	js	.L1559
	cvtsi2sdq	%rdi, %xmm0
.L1290:
	movsd	%xmm0, 8(%rax)
	addq	$8, %rdx
.L1220:
	movl	176(%rsp), %r13d
	movq	(%rbx,%rdx), %r11
	movslq	%r13d, %r8
	movq	(%r11,%r9), %rdi
	movq	(%rdi,%r8,8), %rdi
	testq	%rdi, %rdi
	js	.L1560
	cvtsi2sdq	%rdi, %xmm0
.L1292:
	movsd	%xmm0, (%rax,%rdx)
	addq	$8, %rdx
.L1219:
	movl	176(%rsp), %r13d
	movq	(%rbx,%rdx), %r11
	movslq	%r13d, %r8
	movq	(%r11,%r9), %rdi
	movq	(%rdi,%r8,8), %rdi
	testq	%rdi, %rdi
	js	.L1561
	cvtsi2sdq	%rdi, %xmm0
.L1294:
	movsd	%xmm0, (%rax,%rdx)
	addq	$8, %rdx
	cmpq	%rdx, %r15
	jne	.L937
	jmp	.L1288
.L1565:
	cvtsi2sdq	%rdi, %xmm0
.L936:
	movsd	%xmm0, (%rax,%rdx)
	addq	$8, %rdx
	movl	176(%rsp), %r13d
	movq	(%rbx,%rdx), %r10
	movslq	%r13d, %r8
	movq	(%r10,%r9), %rdi
	movq	(%rdi,%r8,8), %rdi
	testq	%rdi, %rdi
	js	.L1562
	cvtsi2sdq	%rdi, %xmm0
.L1295:
	movsd	%xmm0, (%rax,%rdx)
	leaq	8(%rdx), %r8
	movl	176(%rsp), %r10d
	movq	(%rbx,%r8), %r13
	movslq	%r10d, %r11
	movq	0(%r13,%r9), %rdi
	movq	(%rdi,%r11,8), %rdi
	testq	%rdi, %rdi
	js	.L1563
	cvtsi2sdq	%rdi, %xmm0
.L1297:
	movsd	%xmm0, (%rax,%r8)
	leaq	16(%rdx), %r8
	movl	176(%rsp), %r10d
	movq	(%rbx,%r8), %r11
	movslq	%r10d, %r13
	movq	(%r11,%r9), %rdi
	movq	(%rdi,%r13,8), %rdi
	testq	%rdi, %rdi
	js	.L1564
	cvtsi2sdq	%rdi, %xmm0
.L1299:
	movsd	%xmm0, (%rax,%r8)
	addq	$24, %rdx
	cmpq	%rdx, %r15
	je	.L1288
.L937:
	movl	176(%rsp), %r13d
	movq	(%rbx,%rdx), %r10
	movslq	%r13d, %r8
	movq	(%r10,%r9), %rdi
	movq	(%rdi,%r8,8), %rdi
	testq	%rdi, %rdi
	jns	.L1565
	movq	%rdi, %r11
	shrq	%r11
	andl	$1, %edi
	orq	%rdi, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L936
.L1288:
	movq	64(%r14), %rax
	leal	-1(%rbp), %edi
	andl	$3, %edi
	movq	(%rbx), %r11
	movq	(%r11,%r9), %rdx
	movq	(%rdx,%rcx), %rdx
	testq	%rdx, %rdx
	js	.L1566
	cvtsi2sdq	%rdx, %xmm0
.L1275:
	movsd	%xmm0, (%rax)
	movl	$1, %r10d
	movl	$8, %edx
	cmpl	$1, %ebp
	jle	.L913
	testl	%edi, %edi
	je	.L940
	cmpl	$1, %edi
	je	.L1216
	cmpl	$2, %edi
	.p2align 4,,3
	je	.L1217
	movq	8(%rbx), %r8
	movq	(%r8,%r9), %rdi
	movq	(%rdi,%rcx), %rdi
	testq	%rdi, %rdi
	js	.L1567
	cvtsi2sdq	%rdi, %xmm0
.L1277:
	movsd	%xmm0, 8(%rax)
	addl	$1, %r10d
	addq	$8, %rdx
.L1217:
	movq	(%rbx,%rdx), %rdi
	movq	(%rdi,%r9), %r11
	movq	(%r11,%rcx), %rdi
	testq	%rdi, %rdi
	js	.L1568
	cvtsi2sdq	%rdi, %xmm0
.L1279:
	movsd	%xmm0, (%rax,%rdx)
	addl	$1, %r10d
	addq	$8, %rdx
.L1216:
	movq	(%rbx,%rdx), %r11
	movq	(%r11,%r9), %r13
	movq	0(%r13,%rcx), %rdi
	testq	%rdi, %rdi
	js	.L1569
	cvtsi2sdq	%rdi, %xmm0
.L1281:
	movsd	%xmm0, (%rax,%rdx)
	addl	$1, %r10d
	addq	$8, %rdx
	cmpl	%r10d, %ebp
	jg	.L940
	jmp	.L913
.L1573:
	cvtsi2sdq	%rdi, %xmm0
.L939:
	movsd	%xmm0, (%rax,%rdx)
	addl	$1, %r10d
	addq	$8, %rdx
	movq	(%rbx,%rdx), %rdi
	movq	(%rdi,%r9), %r11
	movq	(%r11,%rcx), %rdi
	testq	%rdi, %rdi
	js	.L1570
	cvtsi2sdq	%rdi, %xmm0
.L1282:
	movsd	%xmm0, (%rax,%rdx)
	leaq	8(%rdx), %r8
	movq	(%rbx,%r8), %r11
	movq	(%r11,%r9), %rdi
	movq	(%rdi,%rcx), %rdi
	testq	%rdi, %rdi
	js	.L1571
	cvtsi2sdq	%rdi, %xmm0
.L1284:
	movsd	%xmm0, (%rax,%r8)
	leaq	16(%rdx), %r8
	movq	(%rbx,%r8), %r11
	movq	(%r11,%r9), %rdi
	movq	(%rdi,%rcx), %rdi
	testq	%rdi, %rdi
	js	.L1572
	cvtsi2sdq	%rdi, %xmm0
.L1286:
	movsd	%xmm0, (%rax,%r8)
	addl	$3, %r10d
	addq	$24, %rdx
	cmpl	%r10d, %ebp
	jle	.L913
.L940:
	movq	(%rbx,%rdx), %r8
	movq	(%r8,%r9), %rdi
	movq	(%rdi,%rcx), %rdi
	testq	%rdi, %rdi
	jns	.L1573
	movq	%rdi, %r13
	shrq	%r13
	andl	$1, %edi
	orq	%rdi, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L939
.L1477:
	movq	56(%rsp), %rsi
	movq	(%rsi), %rsi
.L913:
	movl	176(%rsp), %edx
	cvtsi2sd	%edx, %xmm7
	movq	48(%rsp), %r10
	movsd	%xmm7, (%rsi,%r10)
	movl	192(%rsp), %eax
	movl	176(%rsp), %r9d
	cltq
	movslq	%r9d, %r13
	salq	$3, %r13
	movq	88(%rsp), %rcx
	addq	(%rcx,%rax,8), %r13
	movq	%r14, %rsi
	movsd	72(%rsp), %xmm0
	movl	68(%rsp), %edi
	call	fpar_f
	comisd	.LC35(%rip), %xmm0
	jae	.L941
	cvttsd2siq	%xmm0, %rsi
	movq	%rsi, 0(%r13)
.L942:
	movl	176(%rsp), %edi
	addl	$1, %edi
	movl	%edi, 176(%rsp)
	movl	176(%rsp), %r13d
	cmpl	%r13d, %r12d
	jg	.L943
.L908:
	movl	192(%rsp), %ecx
	addl	272(%rsp), %ecx
	movl	%ecx, 192(%rsp)
	movl	192(%rsp), %eax
	cmpl	%eax, 44(%rsp)
	jg	.L944
.L907:
	movl	$0, 192(%rsp)
	movl	192(%rsp), %r12d
	cmpl	$8, %r12d
	jg	.L945
.L953:
	movl	192(%rsp), %eax
	cltq
	movq	(%r14,%rax,8), %rdi
	call	free
	movl	192(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, 192(%rsp)
	movl	192(%rsp), %ebp
	cmpl	$8, %ebp
	jg	.L945
	movl	192(%rsp), %eax
	cltq
	movq	(%r14,%rax,8), %rdi
	call	free
	movl	192(%rsp), %r12d
	addl	$1, %r12d
	movl	%r12d, 192(%rsp)
	movl	192(%rsp), %r13d
	cmpl	$8, %r13d
	jg	.L945
	movl	192(%rsp), %eax
	cltq
	movq	(%r14,%rax,8), %rdi
	call	free
	movl	192(%rsp), %ebp
	addl	$1, %ebp
	movl	%ebp, 192(%rsp)
	movl	192(%rsp), %r15d
	cmpl	$8, %r15d
	jg	.L945
	movl	192(%rsp), %eax
	cltq
	movq	(%r14,%rax,8), %rdi
	call	free
	movl	192(%rsp), %ecx
	addl	$1, %ecx
	movl	%ecx, 192(%rsp)
	movl	192(%rsp), %r11d
	cmpl	$8, %r11d
	jle	.L953
.L945:
	movq	%r14, %rdi
	call	free
	jmp	.L848
.L941:
	subsd	.LC35(%rip), %xmm0
	cvttsd2siq	%xmm0, %r11
	movq	%r11, 0(%r13)
	movabsq	$-9223372036854775808, %r8
	xorq	%r8, 0(%r13)
	jmp	.L942
.L1572:
	movq	%rdi, %r13
	shrq	%r13
	andl	$1, %edi
	orq	%rdi, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1286
.L1571:
	movq	%rdi, %r13
	shrq	%r13
	andl	$1, %edi
	orq	%rdi, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1284
.L1570:
	movq	%rdi, %r13
	shrq	%r13
	andl	$1, %edi
	orq	%rdi, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1282
.L1563:
	movq	%rdi, %r11
	shrq	%r11
	andl	$1, %edi
	orq	%rdi, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1297
.L1562:
	movq	%rdi, %r8
	shrq	%r8
	andl	$1, %edi
	orq	%rdi, %r8
	cvtsi2sdq	%r8, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1295
.L1564:
	movq	%rdi, %r13
	shrq	%r13
	andl	$1, %edi
	orq	%rdi, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1299
.L1539:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1339
.L1538:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1337
.L1540:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1341
.L1532:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1354
.L1531:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1352
.L1530:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1350
.L1484:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1438
.L1483:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1436
.L1482:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1434
.L1508:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1396
.L1507:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1394
.L1506:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1392
.L1500:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1425
.L1499:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1423
.L1498:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1421
.L1497:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1419
.L1496:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1417
.L1495:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1415
.L1494:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1413
.L1556:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1328
.L1555:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1326
.L1554:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1324
.L1553:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1322
.L1552:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1320
.L1551:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1318
.L1550:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1316
.L1524:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1383
.L1523:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1381
.L1522:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1379
.L1521:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1377
.L1520:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1375
.L1519:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1373
.L1518:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1371
.L1510:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1355
.L1478:
	movq	%rdi, %r10
	shrq	%r10
	andl	$1, %edi
	orq	%rdi, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1426
.L1502:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1384
.L1558:
	movq	%rdx, %r10
	shrq	%r10
	andl	$1, %edx
	orq	%rdx, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1287
.L1486:
	movq	%rdi, %r11
	shrq	%r11
	andl	$1, %edi
	orq	%rdi, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1397
.L1534:
	movq	%rdi, %r10
	shrq	%r10
	andl	$1, %edi
	orq	%rdi, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1329
.L1526:
	movq	%rdi, %r10
	shrq	%r10
	andl	$1, %edi
	orq	%rdi, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1342
.L1566:
	movq	%rdx, %r10
	shrq	%r10
	andl	$1, %edx
	orq	%rdx, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1275
.L1542:
	movq	%rdi, %r11
	shrq	%r11
	andl	$1, %edi
	orq	%rdi, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1300
.L1543:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1303
.L1568:
	movq	%rdi, %r8
	shrq	%r8
	andl	$1, %edi
	orq	%rdi, %r8
	cvtsi2sdq	%r8, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1279
.L1547:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1311
.L1505:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1391
.L1511:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1358
.L1515:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1366
.L1514:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1364
.L1512:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1360
.L1513:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1362
.L1569:
	movq	%rdi, %r8
	shrq	%r8
	andl	$1, %edi
	orq	%rdi, %r8
	cvtsi2sdq	%r8, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1281
.L1567:
	movq	%rdi, %r13
	shrq	%r13
	andl	$1, %edi
	orq	%rdi, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1277
.L1560:
	movq	%rdi, %r10
	shrq	%r10
	andl	$1, %edi
	orq	%rdi, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1292
.L1561:
	movq	%rdi, %r10
	shrq	%r10
	andl	$1, %edi
	orq	%rdi, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1294
.L1545:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1307
.L1546:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1309
.L1544:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1305
.L1548:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1313
.L1528:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1347
.L1527:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1345
.L1529:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1349
.L1535:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1332
.L1549:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1315
.L1559:
	movq	%rdi, %r10
	shrq	%r10
	andl	$1, %edi
	orq	%rdi, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1290
.L1536:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1334
.L1537:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1336
.L1503:
	movq	%r10, %r13
	shrq	%r13
	andl	$1, %r10d
	orq	%r10, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1387
.L1492:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1410
.L1493:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1412
.L1479:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1429
.L1480:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1431
.L1504:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1389
.L1516:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1368
.L1517:
	movq	%r10, %r11
	shrq	%r11
	andl	$1, %r10d
	orq	%r10, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1370
.L1481:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1433
.L1487:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1400
.L1489:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1404
.L1488:
	movq	%r8, %r10
	shrq	%r10
	andl	$1, %r8d
	orq	%r8, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1402
.L1490:
	movq	%r8, %r11
	shrq	%r11
	andl	$1, %r8d
	orq	%r8, %r11
	cvtsi2sdq	%r11, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1406
.L1491:
	movq	%r8, %r13
	shrq	%r13
	andl	$1, %r8d
	orq	%r8, %r13
	cvtsi2sdq	%r13, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1408
.L1468:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
.L952:
	movl	192(%rsp), %eax
	movl	176(%rsp), %edi
	cltq
	movslq	%edi, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %r9d
	movl	176(%rsp), %esi
	movslq	%r9d, %rcx
	movq	(%rbx), %rax
	movslq	%esi, %r11
	movq	(%rax,%rcx,8), %r8
	movq	(%r8,%r11,8), %rdi
	call	udfi1
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %edx
	addl	$1, %edx
	movl	%edx, 176(%rsp)
	movl	176(%rsp), %r10d
	cmpl	%r10d, %r12d
	jle	.L855
	movl	192(%rsp), %eax
	movl	176(%rsp), %edx
	cltq
	movslq	%edx, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %ecx
	movl	176(%rsp), %r11d
	movslq	%ecx, %r8
	movq	(%rbx), %r9
	movslq	%r11d, %rdi
	movq	(%r9,%r8,8), %rsi
	movq	(%rsi,%rdi,8), %rdi
	call	udfi1
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jg	.L952
	jmp	.L855
.L1469:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
	leaq	8(%rbx), %r15
.L888:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebp
	cltq
	movslq	%ebp, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %r11d
	movl	176(%rsp), %r8d
	movl	192(%rsp), %r9d
	movl	176(%rsp), %eax
	movslq	%r11d, %rdi
	movq	(%r15), %rdx
	movslq	%r8d, %r10
	movq	(%rdx,%rdi,8), %rsi
	movq	(%rsi,%r10,8), %rsi
	movslq	%r9d, %r11
	movq	(%rbx), %rcx
	cltq
	movq	(%rcx,%r11,8), %r8
	movq	(%r8,%rax,8), %rdi
	call	udfi2
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %edx
	addl	$1, %edx
	movl	%edx, 176(%rsp)
	movl	176(%rsp), %r10d
	cmpl	%r10d, %r12d
	jle	.L855
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebp
	cltq
	movslq	%ebp, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movl	192(%rsp), %edi
	movl	176(%rsp), %r9d
	movl	192(%rsp), %r11d
	movl	176(%rsp), %eax
	movslq	%edi, %r10
	movq	(%r15), %rdx
	movslq	%r9d, %rcx
	movq	(%rdx,%r10,8), %rsi
	movq	(%rsi,%rcx,8), %rsi
	movslq	%r11d, %r9
	movq	(%rbx), %r8
	cltq
	movq	(%r8,%r9,8), %rdi
	movq	(%rdi,%rax,8), %rdi
	call	udfi2
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %ebp
	addl	$1, %ebp
	movl	%ebp, 176(%rsp)
	movl	176(%rsp), %r10d
	cmpl	%r10d, %r12d
	jg	.L888
	jmp	.L855
.L1472:
	movl	192(%rsp), %r15d
	movl	68(%rsp), %eax
	testl	%r15d, %r15d
	jle	.L899
	movl	192(%rsp), %eax
	subl	$1, %eax
.L899:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %edi
	cmpl	%edi, %r12d
	jle	.L855
	cltq
	leaq	0(,%rax,8), %r15
	jmp	.L902
.L1574:
	movl	176(%rsp), %edi
	subl	$1, %edi
.L901:
	movl	192(%rsp), %eax
	movl	176(%rsp), %r9d
	cltq
	movslq	%r9d, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movq	(%rbx), %r10
	movl	192(%rsp), %edx
	movl	176(%rsp), %r8d
	movslq	%edi, %rdi
	movq	(%r10,%r15), %rsi
	movq	(%rsi,%rdi,8), %rsi
	movslq	%edx, %rax
	movslq	%r8d, %r11
	movq	(%r10,%rax,8), %rcx
	movq	(%rcx,%r11,8), %rdi
	movq	128(%rsp), %rdx
	call	udfi3
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %r9d
	addl	$1, %r9d
	movl	%r9d, 176(%rsp)
	movl	176(%rsp), %edx
	cmpl	%edx, %r12d
	jle	.L855
.L902:
	movl	176(%rsp), %r8d
	testl	%r8d, %r8d
	jg	.L1574
	movl	56(%rsp), %edi
	jmp	.L901
.L1474:
	subq	$1, %rsi
	addl	$1000000, %edx
	jmp	.L852
.L1470:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
	leaq	8(%rbx), %r15
	leaq	16(%rbx), %rbp
	movl	%r14d, 48(%rsp)
	movl	%r12d, %r14d
	movq	%rbx, %r12
	jmp	.L890
.L1210:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebx
	cltq
	movslq	%ebx, %rbx
	salq	$3, %rbx
	addq	0(%r13,%rax,8), %rbx
	movl	192(%rsp), %edx
	movl	176(%rsp), %r8d
	movl	192(%rsp), %r10d
	movl	176(%rsp), %ecx
	movl	192(%rsp), %r11d
	movl	176(%rsp), %eax
	movslq	%edx, %r9
	movq	0(%rbp), %rdi
	movslq	%r8d, %rdx
	movq	(%rdi,%r9,8), %rsi
	movq	(%rsi,%rdx,8), %rdx
	movslq	%r10d, %rdi
	movq	(%r15), %r8
	movslq	%ecx, %rsi
	movq	(%r8,%rdi,8), %r9
	movq	(%r9,%rsi,8), %rsi
	movslq	%r11d, %rcx
	movq	(%r12), %r10
	cltq
	movq	(%r10,%rcx,8), %r11
	movq	(%r11,%rax,8), %rdi
	call	udfi3
	movq	%rax, (%rbx)
	movl	176(%rsp), %r8d
	addl	$1, %r8d
	movl	%r8d, 176(%rsp)
	movl	176(%rsp), %ebx
	cmpl	%ebx, %r14d
	jle	.L1461
.L890:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebx
	cltq
	movslq	%ebx, %rbx
	salq	$3, %rbx
	addq	0(%r13,%rax,8), %rbx
	movl	192(%rsp), %r9d
	movl	176(%rsp), %edx
	movl	192(%rsp), %edi
	movl	176(%rsp), %r10d
	movl	192(%rsp), %r11d
	movl	176(%rsp), %eax
	movslq	%r9d, %rcx
	movq	0(%rbp), %r8
	movslq	%edx, %rdx
	movq	(%r8,%rcx,8), %rsi
	movq	(%rsi,%rdx,8), %rdx
	movslq	%edi, %rcx
	movq	(%r15), %r9
	movslq	%r10d, %rsi
	movq	(%r9,%rcx,8), %r8
	movq	(%r8,%rsi,8), %rsi
	movslq	%r11d, %r10
	movq	(%r12), %rdi
	cltq
	movq	(%rdi,%r10,8), %r9
	movq	(%r9,%rax,8), %rdi
	call	udfi3
	movq	%rax, (%rbx)
	movl	176(%rsp), %ecx
	addl	$1, %ecx
	movl	%ecx, 176(%rsp)
	movl	176(%rsp), %ebx
	cmpl	%ebx, %r14d
	jg	.L1210
	jmp	.L1461
.L1471:
	movl	192(%rsp), %ebp
	movl	68(%rsp), %eax
	testl	%ebp, %ebp
	jle	.L893
	movl	192(%rsp), %eax
	subl	$1, %eax
.L893:
	movl	$0, 176(%rsp)
	movl	176(%rsp), %r15d
	cmpl	%r15d, %r12d
	jle	.L855
	cltq
	leaq	0(,%rax,8), %r15
	jmp	.L896
.L1575:
	movl	176(%rsp), %edi
	subl	$1, %edi
.L895:
	movl	192(%rsp), %eax
	movl	176(%rsp), %ebp
	cltq
	movslq	%ebp, %rbp
	salq	$3, %rbp
	addq	0(%r13,%rax,8), %rbp
	movq	(%rbx), %r9
	movl	192(%rsp), %r11d
	movl	176(%rsp), %eax
	movslq	%edi, %r10
	movq	(%r9,%r15), %rsi
	movq	(%rsi,%r10,8), %rsi
	movslq	%r11d, %rcx
	movslq	%eax, %rdx
	movq	(%r9,%rcx,8), %r8
	movq	(%r8,%rdx,8), %rdi
	call	udfi2
	movq	%rax, 0(%rbp)
	movl	176(%rsp), %edi
	addl	$1, %edi
	movl	%edi, 176(%rsp)
	movl	176(%rsp), %r10d
	cmpl	%r10d, %r12d
	jle	.L855
.L896:
	movl	176(%rsp), %r11d
	testl	%r11d, %r11d
	jg	.L1575
	movl	56(%rsp), %edi
	jmp	.L895
	.cfi_endproc
.LFE45:
	.size	translate_jpeg_file, .-translate_jpeg_file
.globl convert_thread
	.type	convert_thread, @function
convert_thread:
.LFB47:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movl	12(%rdi), %r9d
	movl	8(%rdi), %r8d
	movl	4(%rdi), %ecx
	movq	32(%rdi), %rdx
	movq	24(%rdi), %rsi
	movl	20(%rdi), %edi
	movl	%edi, 8(%rsp)
	movl	16(%rbx), %eax
	movl	%eax, (%rsp)
	movl	(%rbx), %edi
	call	translate_jpeg_file
	movq	%rbx, %rdi
	call	free
	xorl	%eax, %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE47:
	.size	convert_thread, .-convert_thread
	.section	.rodata.str1.1
.LC37:
	.string	"Function not accepted."
.LC38:
	.string	"f(%f) = %f\n"
	.text
.globl tests
	.type	tests, @function
tests:
.LFB46:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	pushq	%r12
	.cfi_def_cfa_offset 24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$1048, %rsp
	.cfi_def_cfa_offset 1088
	leaq	16(%rsp), %r13
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movabsq	$3564984146619542833, %r9
	movq	%r9, 16(%rsp)
	movw	$41, 24(%rsp)
	movq	$-1, %r12
	xorl	%ebp, %ebp
	movq	%r12, %rcx
	movq	%r13, %rdi
	movl	%ebp, %eax
	repnz scasb
	notq	%rcx
	leaq	2(%rcx), %rdi
	call	malloc
	movq	%rax, %rbx
	movq	%r13, %rsi
	movq	%rax, %rdi
	call	strcpy
	movq	%r12, %rcx
	movq	%rbx, %rdi
	movl	%ebp, %eax
	repnz scasb
	notq	%rcx
	movw	$10, -1(%rbx,%rcx)
	movl	$1, %edi
	call	fpar_init
	movq	%rbx, %rcx
	xorl	%edx, %edx
	movl	$1, %esi
	xorl	%edi, %edi
	call	fpar_function
	xorl	%edi, %edi
	call	fpar_ok
	testl	%eax, %eax
	je	.L1588
	movl	$8, %edi
	call	malloc
	movq	%rax, %rbx
	movl	$8, %edi
	call	malloc
	movq	%rax, (%rbx)
	xorl	%ebp, %ebp
	movq	%rbp, (%rax)
	movq	%rbx, %rsi
	movd	%rbp, %xmm0
	xorl	%edi, %edi
	call	fpar_f
	movapd	%xmm0, %xmm1
	movd	%rbp, %xmm0
	movl	$.LC38, %edi
	movl	$2, %eax
	call	printf
	movsd	.LC39(%rip), %xmm2
	movl	$21, %r12d
	movq	(%rbx), %rax
	jmp	.L1581
.L1589:
	movq	(%rbx), %r10
	movsd	%xmm2, (%r10)
	movq	%rbx, %rsi
	movd	%rbp, %xmm0
	xorl	%edi, %edi
	movsd	%xmm2, (%rsp)
	call	fpar_f
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm7
	movapd	%xmm7, %xmm0
	movl	$.LC38, %edi
	movl	$2, %eax
	call	printf
	movsd	(%rsp), %xmm6
	addsd	.LC39(%rip), %xmm6
	movq	(%rbx), %r8
	movsd	%xmm6, (%r8)
	movq	%rbx, %rsi
	movd	%rbp, %xmm0
	xorl	%edi, %edi
	movsd	%xmm6, (%rsp)
	call	fpar_f
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm5
	movapd	%xmm5, %xmm0
	movl	$.LC38, %edi
	movl	$2, %eax
	call	printf
	movsd	(%rsp), %xmm4
	addsd	.LC39(%rip), %xmm4
	movq	(%rbx), %rsi
	movsd	%xmm4, (%rsi)
	movq	%rbx, %rsi
	movd	%rbp, %xmm0
	xorl	%edi, %edi
	movsd	%xmm4, (%rsp)
	call	fpar_f
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm3
	movapd	%xmm3, %xmm0
	movl	$.LC38, %edi
	movl	$2, %eax
	call	printf
	movsd	(%rsp), %xmm1
	addsd	.LC39(%rip), %xmm1
	movq	(%rbx), %rdx
	movsd	%xmm1, (%rdx)
	movq	%rbx, %rsi
	movd	%rbp, %xmm0
	xorl	%edi, %edi
	movsd	%xmm1, (%rsp)
	call	fpar_f
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm0
	movl	$.LC38, %edi
	movl	$2, %eax
	call	printf
	movsd	(%rsp), %xmm2
	addsd	.LC39(%rip), %xmm2
	subl	$5, %r12d
	movq	(%rbx), %rax
.L1581:
	movsd	%xmm2, (%rax)
	movq	%rbx, %rsi
	movd	%rbp, %xmm0
	xorl	%edi, %edi
	movsd	%xmm2, (%rsp)
	call	fpar_f
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm2
	movapd	%xmm2, %xmm0
	movl	$.LC38, %edi
	movl	$2, %eax
	call	printf
	movsd	(%rsp), %xmm2
	addsd	.LC39(%rip), %xmm2
	cmpl	$1, %r12d
	jne	.L1589
	movq	(%rbx), %rdi
	call	free
	movq	%rbx, %rdi
	call	free
	xorl	%eax, %eax
	call	fpar_deinit
	addq	$1048, %rsp
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
.L1588:
	.cfi_restore_state
	movl	$.LC37, %edi
	call	puts
	addq	$1048, %rsp
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
	.cfi_endproc
.LFE46:
	.size	tests, .-tests
	.section	.rodata.str1.1
.LC40:
	.string	"No work threads, exiting"
.LC41:
	.string	"none"
.LC42:
	.string	"asmf1"
.LC43:
	.string	"asmf2"
.LC44:
	.string	"asmf3"
.LC45:
	.string	"udf1"
.LC46:
	.string	"udfi1"
.LC47:
	.string	"udf2"
.LC48:
	.string	"udfi2"
.LC49:
	.string	"udf3"
.LC50:
	.string	"udfi3"
.LC51:
	.string	"udfpi2"
.LC52:
	.string	"udfpi3"
.LC53:
	.string	"udfi6"
	.section	.rodata.str1.8
	.align 8
.LC54:
	.string	"To use asmf2 fast mode, you need to set at least 2 input images"
	.align 8
.LC55:
	.string	"To use asmf3 fast mode, you need to set at least 3 input images"
	.align 8
.LC56:
	.string	"To use udf2 fast mode, you need to set at least 2 input images"
	.align 8
.LC57:
	.string	"To use udf3 fast mode, you need to set at least 3 input images"
	.align 8
.LC58:
	.string	"To use udfi2 fast mode, you need to set at least 2 input images"
	.align 8
.LC59:
	.string	"To use udfi3 fast mode, you need to set at least 3 input images"
	.align 8
.LC60:
	.string	"To use udfi6 fast mode, you need to set at least 6 input images"
	.section	.rodata.str1.1
.LC61:
	.string	"r"
	.section	.rodata.str1.8
	.align 8
.LC62:
	.string	"File %s exists, not overwriting\n"
	.section	.rodata.str1.1
.LC63:
	.string	".jpg"
.LC64:
	.string	".jpeg"
.LC65:
	.string	".png"
.LC66:
	.string	".bmp"
	.section	.rodata.str1.8
	.align 8
.LC67:
	.string	"Unknown file type: %s, supported are JPG, PNG, BMP\n"
	.section	.rodata.str1.1
.LC68:
	.string	"Cannot load %s\n"
	.section	.rodata.str1.8
	.align 8
.LC69:
	.string	"\nWarning X-resolution downgrade: %d --> %d\n"
	.align 8
.LC70:
	.string	"\nWarning X-resolution bigger than actual: %d > %d\n"
	.align 8
.LC71:
	.string	"\nWarning Y-resolution downgrade: %d --> %d\n"
	.align 8
.LC72:
	.string	"\nWarning Y-resolution bigger than actual: %d > %d\n"
	.align 8
.LC73:
	.string	"Failed to create %d/%d thread\n"
	.section	.rodata.str1.1
.LC74:
	.string	"Cannot save %s\n"
	.text
.globl fjpg
	.type	fjpg, @function
fjpg:
.LFB48:
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
	subq	$136, %rsp
	.cfi_def_cfa_offset 192
	movq	%rdi, 88(%rsp)
	movq	%rsi, %rbx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movl	%edx, 68(%rsp)
	movq	%rcx, %r13
	movl	%r8d, 72(%rsp)
	testl	%r8d, %r8d
	jle	.L1839
	movl	$0, ncache(%rip)
	movslq	68(%rsp), %r12
	leaq	(%r12,%r12,2), %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, icache(%rip)
	xorl	%eax, %eax
	movq	$-1, %rcx
	movq	%rbx, %rdi
	repnz scasb
	notq	%rcx
	leaq	2(%rcx), %rdi
	call	malloc
	movq	%rax, %rbp
	leaq	0(,%r12,8), %rdi
	call	malloc
	movq	%rax, 80(%rsp)
	salq	$2, %r12
	movq	%r12, %rdi
	call	malloc
	movq	%rax, %r15
	movq	%r12, %rdi
	call	malloc
	movq	%rax, %r14
	movslq	72(%rsp), %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, 104(%rsp)
	movl	$5, %ecx
	movl	$.LC41, %edi
	movq	%rbx, %rsi
	repz cmpsb
	jne	.L1840
	movl	$13, 76(%rsp)
.L1593:
	movl	ualoop(%rip), %r12d
	testl	%r12d, %r12d
	je	.L1609
	cmpl	$4, 76(%rsp)
	je	.L1665
	cmpl	$11, 76(%rsp)
	je	.L1665
	cmpl	$12, 76(%rsp)
	movl	$1, %edx
	cmovne	72(%rsp), %edx
	movl	%edx, 72(%rsp)
.L1609:
	movl	$.LC61, %esi
	movq	88(%rsp), %rdi
	call	fopen
	movq	%rax, %r12
	testq	%rax, %rax
	je	.L1610
	movl	overwrite_mode(%rip), %r8d
	testl	%r8d, %r8d
	je	.L1611
	movq	%rax, %rdi
	call	fclose
.L1610:
	movl	76(%rsp), %r9d
	testl	%r9d, %r9d
	je	.L1612
.L1616:
	movl	68(%rsp), %ebx
	testl	%ebx, %ebx
	jle	.L1841
	movq	%r13, %r12
	movl	$65535, %ebx
	movl	$65535, %ebp
	movl	68(%rsp), %r11d
	subl	$1, %r11d
	andl	$1, %r11d
	movl	%r11d, 100(%rsp)
	movl	$.LC63, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1842
.L1723:
	movq	0(%r13), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	80(%rsp), %rdi
	call	load_jpeg_file
.L1808:
	testl	%eax, %eax
	jne	.L1622
	movl	(%r15), %esi
	cmpl	$65535, %esi
	jl	.L1810
	jg	.L1843
.L1728:
	movl	(%r14), %esi
	cmpl	%esi, %ebx
	jg	.L1811
	.p2align 4,,6
	jl	.L1844
.L1731:
	movl	$1, 64(%rsp)
	leaq	8(%r13), %r12
	leaq	4(%r14), %r13
	addq	$4, %r15
	movq	80(%rsp), %rsi
	addq	$8, %rsi
	movq	%rsi, 56(%rsp)
	cmpl	$1, 68(%rsp)
	jle	.L1812
	movl	100(%rsp), %edx
	testl	%edx, %edx
	je	.L1834
	movl	$.LC63, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1813
.L1734:
	movq	(%r12), %rcx
	movq	%r13, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_jpeg_file
.L1814:
	testl	%eax, %eax
	jne	.L1622
	movl	(%r15), %esi
	cmpl	%ebp, %esi
	jl	.L1816
	.p2align 4,,4
	jg	.L1845
.L1739:
	movl	0(%r13), %esi
	cmpl	%esi, %ebx
	jg	.L1818
.L1854:
	.p2align 4,,5
	jl	.L1846
.L1742:
	addl	$1, 64(%rsp)
	addq	$8, %r12
	addq	$4, %r13
	addq	$4, %r15
	addq	$8, 56(%rsp)
	movl	64(%rsp), %eax
	cmpl	%eax, 68(%rsp)
	jle	.L1812
.L1834:
	movq	%r13, %r14
.L1630:
	movl	$.LC63, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1847
.L1617:
	movq	(%r12), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_jpeg_file
.L1619:
	testl	%eax, %eax
	jne	.L1622
	movl	(%r15), %esi
	cmpl	%ebp, %esi
	jge	.L1624
	cmpl	$65534, %ebp
	jle	.L1848
.L1625:
	movl	%esi, %ebp
.L1626:
	movl	(%r14), %esi
	cmpl	%esi, %ebx
	jle	.L1627
	cmpl	$65534, %ebx
	jle	.L1849
.L1628:
	movl	%esi, %ebx
.L1629:
	movl	64(%rsp), %r13d
	addl	$1, %r13d
	movl	%r13d, 64(%rsp)
	leaq	8(%r12), %r13
	movq	%r13, %r12
	addq	$4, %r14
	addq	$4, %r15
	movq	56(%rsp), %rsi
	addq	$8, %rsi
	movq	%rsi, 56(%rsp)
	movl	$.LC63, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1850
.L1746:
	movq	0(%r13), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_jpeg_file
.L1820:
	testl	%eax, %eax
	jne	.L1622
	movl	(%r15), %esi
	cmpl	%ebp, %esi
	jl	.L1822
	.p2align 4,,4
	jg	.L1851
.L1751:
	movl	(%r14), %esi
	cmpl	%esi, %ebx
	jg	.L1824
	.p2align 4,,6
	jl	.L1852
.L1754:
	movl	64(%rsp), %r8d
	addl	$1, %r8d
	movl	%r8d, 64(%rsp)
	leaq	8(%r13), %r12
	addq	$4, %r14
	addq	$4, %r15
	movq	56(%rsp), %r13
	addq	$8, %r13
	movq	%r13, 56(%rsp)
	cmpl	%r8d, 68(%rsp)
	jg	.L1630
.L1812:
	movslq	%ebx, %rdi
	movl	$8, %esi
	call	calloc
	movq	%rax, %r13
	testl	%ebx, %ebx
	jle	.L1631
	movslq	%ebp, %r15
.L1632:
	leal	-1(%rbx), %r9d
	andl	$7, %r9d
	movl	$8, %esi
	movq	%r15, %rdi
	movl	%r9d, 16(%rsp)
	call	calloc
	movq	%rax, 0(%r13)
	movl	$1, %r14d
	leaq	8(%r13), %r12
	cmpl	%ebx, %r14d
	movl	16(%rsp), %edx
	jge	.L1631
	testl	%edx, %edx
	je	.L1633
	cmpl	$1, %edx
	je	.L1789
	cmpl	$2, %edx
	.p2align 4,,3
	je	.L1790
	cmpl	$3, %edx
	.p2align 4,,2
	je	.L1791
	cmpl	$4, %edx
	.p2align 4,,2
	je	.L1792
	cmpl	$5, %edx
	.p2align 4,,2
	je	.L1793
	cmpl	$6, %edx
	.p2align 4,,2
	je	.L1794
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	movl	$2, %r14d
	leaq	16(%r13), %r12
.L1794:
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	addl	$1, %r14d
	addq	$8, %r12
.L1793:
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	addl	$1, %r14d
	addq	$8, %r12
.L1792:
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	addl	$1, %r14d
	addq	$8, %r12
.L1791:
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	addl	$1, %r14d
	addq	$8, %r12
.L1790:
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	addl	$1, %r14d
	addq	$8, %r12
.L1789:
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	addl	$1, %r14d
	addq	$8, %r12
	cmpl	%ebx, %r14d
	jge	.L1631
.L1633:
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, (%r12)
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, 8(%r12)
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, 16(%r12)
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, 24(%r12)
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, 32(%r12)
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, 40(%r12)
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, 48(%r12)
	movl	$8, %esi
	movq	%r15, %rdi
	call	calloc
	movq	%rax, 56(%r12)
	addl	$8, %r14d
	addq	$64, %r12
	cmpl	%ebx, %r14d
	jl	.L1633
.L1631:
	cmpl	$1, 72(%rsp)
	je	.L1634
	cmpl	$13, 76(%rsp)
	je	.L1635
	xorl	%r12d, %r12d
	movl	72(%rsp), %r15d
	subl	$1, %r15d
	andl	$1, %r15d
	movl	$40, %edi
	call	malloc
	movl	68(%rsp), %r9d
	movl	%r9d, (%rax)
	movl	%ebp, 4(%rax)
	movl	%ebx, 8(%rax)
	movl	$0, 12(%rax)
	movl	72(%rsp), %r8d
	movl	%r8d, 16(%rax)
	movl	76(%rsp), %edx
	movl	%edx, 20(%rax)
	movq	80(%rsp), %r14
	movq	%r14, 24(%rax)
	movq	%r13, 32(%rax)
	movq	%rax, %rcx
	movl	$convert_thread, %edx
	xorl	%esi, %esi
	movq	104(%rsp), %rdi
	call	pthread_create
	testl	%eax, %eax
	jne	.L1806
	movl	$1, %r12d
	movq	104(%rsp), %r14
	addq	$8, %r14
	cmpl	$1, 72(%rsp)
	jle	.L1807
	testl	%r15d, %r15d
	je	.L1830
	movl	$40, %edi
	call	malloc
	movl	68(%rsp), %r11d
	movl	%r11d, (%rax)
	movl	%ebp, 4(%rax)
	movl	%ebx, 8(%rax)
	movl	$1, 12(%rax)
	movl	72(%rsp), %edi
	movl	%edi, 16(%rax)
	movl	76(%rsp), %ecx
	movl	%ecx, 20(%rax)
	movq	80(%rsp), %r10
	movq	%r10, 24(%rax)
	movq	%r13, 32(%rax)
	movq	%rax, %rcx
	movl	$convert_thread, %edx
	xorl	%esi, %esi
	movq	%r14, %rdi
	call	pthread_create
	testl	%eax, %eax
	jne	.L1806
	movb	$2, %r12b
	movq	104(%rsp), %r14
	addq	$16, %r14
	cmpl	$2, 72(%rsp)
	jle	.L1807
.L1830:
	movl	72(%rsp), %r15d
	jmp	.L1637
.L1636:
	addl	$1, %r12d
	leaq	8(%r14), %rax
	movq	%rax, 56(%rsp)
	movl	$40, %edi
	call	malloc
	movl	68(%rsp), %edx
	movl	%edx, (%rax)
	movl	%ebp, 4(%rax)
	movl	%ebx, 8(%rax)
	movl	%r12d, 12(%rax)
	movl	%r15d, 16(%rax)
	movl	76(%rsp), %esi
	movl	%esi, 20(%rax)
	movq	80(%rsp), %r11
	movq	%r11, 24(%rax)
	movq	%r13, 32(%rax)
	movq	%rax, %rcx
	movl	$convert_thread, %edx
	xorl	%esi, %esi
	movq	56(%rsp), %rdi
	call	pthread_create
	testl	%eax, %eax
	jne	.L1806
	addl	$1, %r12d
	addq	$16, %r14
	cmpl	%r12d, %r15d
	jle	.L1807
.L1637:
	movl	$40, %edi
	call	malloc
	movl	68(%rsp), %edi
	movl	%edi, (%rax)
	movl	%ebp, 4(%rax)
	movl	%ebx, 8(%rax)
	movl	%r12d, 12(%rax)
	movl	%r15d, 16(%rax)
	movl	76(%rsp), %ecx
	movl	%ecx, 20(%rax)
	movq	80(%rsp), %r10
	movq	%r10, 24(%rax)
	movq	%r13, 32(%rax)
	movq	%rax, %rcx
	movl	$convert_thread, %edx
	xorl	%esi, %esi
	movq	%r14, %rdi
	call	pthread_create
	testl	%eax, %eax
	je	.L1636
.L1806:
	leal	1(%r12), %esi
	movl	72(%rsp), %edx
	movl	$.LC73, %edi
	xorl	%eax, %eax
	call	printf
	movl	$8, %eax
	jmp	.L1592
.L1840:
	movl	$6, %eax
	movl	$.LC42, %edi
	movq	%rbx, %rsi
	movq	%rax, %rcx
	repz cmpsb
	je	.L1650
	movl	$.LC43, %edi
	movq	%rbx, %rsi
	movq	%rax, %rcx
	repz cmpsb
	seta	%dl
	setb	%r8b
	subb	%r8b, %dl
	movsbl	%dl, %r12d
	testl	%r12d, %r12d
	jne	.L1594
	cmpl	$1, 68(%rsp)
	jle	.L1594
	movl	$11, 76(%rsp)
	jmp	.L1593
.L1849:
	movl	%esi, %edx
	movl	%ebx, %esi
	movl	$.LC71, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%r14), %esi
	jmp	.L1628
.L1848:
	movl	%esi, %edx
	movl	%ebp, %esi
	movl	$.LC69, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%r15), %esi
	jmp	.L1625
.L1627:
	jge	.L1629
	movl	%ebx, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1629
.L1624:
	jle	.L1626
	movl	%ebp, %edx
	movl	$.LC70, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1626
.L1824:
	cmpl	$65534, %ebx
	jle	.L1825
	movl	%esi, %ebx
	.p2align 4,,3
	jmp	.L1754
.L1825:
	movl	%esi, %edx
	movl	%ebx, %esi
	movl	$.LC71, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%r14), %esi
	movl	%esi, %ebx
	jmp	.L1754
.L1822:
	cmpl	$65534, %ebp
	jle	.L1823
	movl	%esi, %ebp
	jmp	.L1751
.L1823:
	movl	%esi, %edx
	movl	%ebp, %esi
	movl	$.LC69, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%r15), %esi
	movl	%esi, %ebp
	jmp	.L1751
.L1847:
	movl	$.LC64, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1617
	movl	$.LC65, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1620
	movq	(%r12), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_png_file
	jmp	.L1619
.L1850:
	movl	$.LC64, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1746
	movl	$.LC65, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1853
	movq	0(%r13), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_png_file
	jmp	.L1820
.L1852:
	movl	%ebx, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1754
.L1851:
	movl	%ebp, %edx
	movl	$.LC70, %edi
	call	printf
	jmp	.L1751
.L1611:
	movq	88(%rsp), %rsi
	movl	$.LC62, %edi
	xorl	%eax, %eax
	call	printf
	movq	%r12, %rdi
	call	fclose
	movl	$1, %eax
.L1592:
	addq	$136, %rsp
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
.L1621:
	.cfi_restore_state
	movq	(%r12), %rsi
	movl	$.LC67, %edi
	xorl	%eax, %eax
	call	printf
.L1622:
	movq	(%r12), %rsi
	movl	$.LC68, %edi
	xorl	%eax, %eax
	call	printf
	movl	$4, %eax
	jmp	.L1592
.L1811:
	movl	%esi, %ebx
	jmp	.L1731
.L1810:
	movl	%esi, %ebp
	.p2align 4,,2
	jmp	.L1728
.L1650:
	movl	$4, 76(%rsp)
	jmp	.L1593
.L1816:
	cmpl	$65534, %ebp
	jle	.L1817
	movl	%esi, %ebp
.L1856:
	movl	0(%r13), %esi
	cmpl	%esi, %ebx
	jle	.L1854
.L1818:
	cmpl	$65534, %ebx
	jle	.L1819
	movl	%esi, %ebx
	jmp	.L1742
.L1594:
	movl	$6, %ecx
	movl	$.LC44, %edi
	movq	%rbx, %rsi
	repz cmpsb
	seta	%r9b
	setb	%r10b
	subb	%r10b, %r9b
	movsbl	%r9b, %edx
	testl	%edx, %edx
	jne	.L1595
	cmpl	$2, 68(%rsp)
	jle	.L1595
	movl	$12, 76(%rsp)
	jmp	.L1593
.L1634:
	cmpl	$13, 76(%rsp)
	je	.L1635
	movl	76(%rsp), %r14d
	movl	%r14d, 8(%rsp)
	movl	$1, (%rsp)
	xorl	%r9d, %r9d
	movl	%ebx, %r8d
	movl	%ebp, %ecx
	movq	%r13, %rdx
	movq	80(%rsp), %rsi
	movl	68(%rsp), %edi
	call	translate_jpeg_file
.L1639:
	movl	76(%rsp), %r8d
	testl	%r8d, %r8d
	jne	.L1641
	xorl	%eax, %eax
	call	fpar_deinit
.L1641:
	movl	$.LC63, %esi
	movq	88(%rsp), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1855
.L1642:
	movq	88(%rsp), %rcx
	movl	%ebx, %edx
	movl	%ebp, %esi
	movq	%r13, %rdi
	call	save_jpeg_file
.L1644:
	testl	%eax, %eax
	je	.L1648
.L1647:
	movq	88(%rsp), %rsi
	movl	$.LC74, %edi
	xorl	%eax, %eax
	call	printf
	movl	$16, %eax
	jmp	.L1592
.L1844:
	movl	$65535, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1731
.L1843:
	movl	$65535, %edx
	movl	$.LC70, %edi
	call	printf
	jmp	.L1728
.L1817:
	movl	%esi, %edx
	movl	%ebp, %esi
	movl	$.LC69, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%r15), %esi
	movl	%esi, %ebp
	jmp	.L1856
.L1595:
	movl	$5, %ecx
	movl	$.LC45, %edi
	movq	%rbx, %rsi
	repz cmpsb
	jne	.L1596
	movl	68(%rsp), %r11d
	testl	%r11d, %r11d
	jle	.L1596
	movl	$1, 76(%rsp)
	jmp	.L1593
.L1596:
	movl	$6, %ecx
	movl	$.LC46, %edi
	movq	%rbx, %rsi
	repz cmpsb
	je	.L1857
.L1597:
	movl	$5, %ecx
	movl	$.LC47, %edi
	movq	%rbx, %rsi
	repz cmpsb
	seta	%r8b
	setb	%r9b
	subb	%r9b, %r8b
	movsbl	%r8b, %r8d
	testl	%r8d, %r8d
	jne	.L1598
	cmpl	$1, 68(%rsp)
	jle	.L1598
	movl	$2, 76(%rsp)
	jmp	.L1593
.L1819:
	movl	%esi, %edx
	movl	%ebx, %esi
	movl	$.LC71, %edi
	xorl	%eax, %eax
	call	printf
	movl	0(%r13), %esi
	movl	%esi, %ebx
	jmp	.L1742
.L1665:
	movl	$1, 72(%rsp)
	jmp	.L1609
.L1839:
	movl	$.LC40, %edi
	call	puts
	xorl	%eax, %eax
	jmp	.L1592
.L1612:
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	strcpy
	xorl	%eax, %eax
	movq	$-1, %rcx
	movq	%rbp, %rdi
	repnz scasb
	notq	%rcx
	movw	$10, -1(%rbp,%rcx)
	movl	72(%rsp), %edi
	call	fpar_init
	movl	68(%rsp), %r12d
	addl	$1, %r12d
	movl	72(%rsp), %r10d
	subl	$1, %r10d
	andl	$7, %r10d
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	xorl	%edi, %edi
	movl	%r10d, 24(%rsp)
	call	fpar_function
	movl	$1, %ebx
	cmpl	$1, 72(%rsp)
	movl	24(%rsp), %r8d
	jle	.L1803
	testl	%r8d, %r8d
	je	.L1615
	cmpl	$1, %r8d
	je	.L1796
	cmpl	$2, %r8d
	je	.L1797
	cmpl	$3, %r8d
	je	.L1798
	cmpl	$4, %r8d
	je	.L1799
	cmpl	$5, %r8d
	je	.L1800
	cmpl	$6, %r8d
	je	.L1801
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	$1, %edi
	call	fpar_function
	movl	$2, %ebx
.L1801:
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	%ebx, %edi
	call	fpar_function
	addl	$1, %ebx
.L1800:
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	%ebx, %edi
	call	fpar_function
	addl	$1, %ebx
.L1799:
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	%ebx, %edi
	call	fpar_function
	addl	$1, %ebx
.L1798:
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	%ebx, %edi
	call	fpar_function
	addl	$1, %ebx
.L1797:
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	%ebx, %edi
	call	fpar_function
	addl	$1, %ebx
.L1796:
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	%ebx, %edi
	call	fpar_function
	addl	$1, %ebx
	cmpl	%ebx, 72(%rsp)
	jle	.L1803
.L1615:
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	movl	%ebx, %edi
	call	fpar_function
	leal	1(%rbx), %edi
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	call	fpar_function
	leal	2(%rbx), %edi
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	call	fpar_function
	leal	3(%rbx), %edi
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	call	fpar_function
	leal	4(%rbx), %edi
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	call	fpar_function
	leal	5(%rbx), %edi
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	call	fpar_function
	leal	6(%rbx), %edi
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	call	fpar_function
	leal	7(%rbx), %edi
	movq	%rbp, %rcx
	movl	$8, %edx
	movl	%r12d, %esi
	call	fpar_function
	addl	$8, %ebx
	cmpl	%ebx, 72(%rsp)
	jg	.L1615
.L1803:
	xorl	%edi, %edi
	call	fpar_ok
	testl	%eax, %eax
	jne	.L1616
	movl	$.LC37, %edi
	call	puts
	movl	$2, %eax
	jmp	.L1592
.L1842:
	movl	$.LC64, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1723
	movl	$.LC65, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1858
	movq	0(%r13), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	80(%rsp), %rdi
	call	load_png_file
	jmp	.L1808
.L1807:
	movl	72(%rsp), %r15d
	subl	$1, %r15d
	andl	$7, %r15d
	xorl	%esi, %esi
	movq	104(%rsp), %r12
	movq	(%r12), %rdi
	call	pthread_join
	movl	$1, %r14d
	movq	104(%rsp), %r12
	addq	$8, %r12
	cmpl	$1, 72(%rsp)
	jle	.L1639
	testl	%r15d, %r15d
	je	.L1638
	cmpl	$1, %r15d
	je	.L1782
	cmpl	$2, %r15d
	je	.L1783
	cmpl	$3, %r15d
	je	.L1784
	cmpl	$4, %r15d
	je	.L1785
	cmpl	$5, %r15d
	je	.L1786
	cmpl	$6, %r15d
	je	.L1787
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	movl	$2, %r14d
	movq	104(%rsp), %r12
	addq	$16, %r12
.L1787:
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	addl	$1, %r14d
	addq	$8, %r12
.L1786:
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	addl	$1, %r14d
	addq	$8, %r12
.L1785:
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	addl	$1, %r14d
	addq	$8, %r12
.L1784:
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	addl	$1, %r14d
	addq	$8, %r12
.L1783:
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	addl	$1, %r14d
	addq	$8, %r12
.L1782:
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	addl	$1, %r14d
	addq	$8, %r12
	cmpl	%r14d, 72(%rsp)
	jle	.L1639
.L1638:
	xorl	%esi, %esi
	movq	(%r12), %rdi
	call	pthread_join
	xorl	%esi, %esi
	movq	8(%r12), %rdi
	call	pthread_join
	xorl	%esi, %esi
	movq	16(%r12), %rdi
	call	pthread_join
	xorl	%esi, %esi
	movq	24(%r12), %rdi
	call	pthread_join
	xorl	%esi, %esi
	movq	32(%r12), %rdi
	call	pthread_join
	xorl	%esi, %esi
	movq	40(%r12), %rdi
	call	pthread_join
	xorl	%esi, %esi
	movq	48(%r12), %rdi
	call	pthread_join
	xorl	%esi, %esi
	movq	56(%r12), %rdi
	call	pthread_join
	addl	$8, %r14d
	addq	$64, %r12
	cmpl	%r14d, 72(%rsp)
	jg	.L1638
	jmp	.L1639
.L1813:
	movl	$.LC64, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1734
	movl	$.LC65, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1859
	movq	(%r12), %rcx
	movq	%r13, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_png_file
	jmp	.L1814
.L1648:
	leaq	112(%rsp), %rdi
	xorl	%esi, %esi
	call	__gettimeofday50
	movq	88(%rsp), %rdi
	call	puts
	xorl	%eax, %eax
	jmp	.L1592
.L1845:
	movl	%ebp, %edx
	movl	$.LC70, %edi
	call	printf
	jmp	.L1739
.L1846:
	movl	%ebx, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1742
.L1635:
	movq	80(%rsp), %r15
	movq	(%r15), %r13
	jmp	.L1641
.L1857:
	movl	68(%rsp), %eax
	testl	%eax, %eax
	jle	.L1597
	movl	$5, 76(%rsp)
	jmp	.L1593
.L1855:
	movl	$.LC64, %esi
	movq	88(%rsp), %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1642
	movl	$.LC65, %esi
	movq	88(%rsp), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1645
	movq	88(%rsp), %rcx
	movl	%ebx, %edx
	movl	%ebp, %esi
	movq	%r13, %rdi
	call	save_png_file
	jmp	.L1644
.L1598:
	movl	$6, %ecx
	movl	$.LC48, %edi
	movq	%rbx, %rsi
	repz cmpsb
	seta	%r10b
	setb	%r11b
	subb	%r11b, %r10b
	movsbl	%r10b, %r9d
	testl	%r9d, %r9d
	jne	.L1599
	cmpl	$1, 68(%rsp)
	jle	.L1599
	movl	$6, 76(%rsp)
	jmp	.L1593
.L1841:
	movl	$8, %esi
	movl	$65535, %edi
	call	calloc
	movq	%rax, %r13
	movl	$65535, %r15d
	movl	$65535, %ebx
	movl	$65535, %ebp
	jmp	.L1632
.L1599:
	movl	$5, %ecx
	movl	$.LC49, %edi
	movq	%rbx, %rsi
	repz cmpsb
	seta	%r10b
	setb	%al
	subb	%al, %r10b
	movsbl	%r10b, %r10d
	testl	%r10d, %r10d
	jne	.L1600
	cmpl	$2, 68(%rsp)
	jle	.L1600
	movl	$3, 76(%rsp)
	jmp	.L1593
.L1600:
	movl	$6, %ecx
	movl	$.LC50, %edi
	movq	%rbx, %rsi
	repz cmpsb
	seta	%r11b
	setb	%al
	subb	%al, %r11b
	movsbl	%r11b, %r11d
	testl	%r11d, %r11d
	jne	.L1601
	cmpl	$2, 68(%rsp)
	jle	.L1601
	movl	$7, 76(%rsp)
	jmp	.L1593
.L1601:
	movl	$7, %ecx
	movl	$.LC51, %edi
	movq	%rbx, %rsi
	repz cmpsb
	je	.L1659
	movl	$.LC52, %esi
	movq	%rbx, %rdi
	movl	%edx, 16(%rsp)
	movl	%r8d, 24(%rsp)
	movl	%r9d, 48(%rsp)
	movl	%r10d, 40(%rsp)
	movl	%r11d, 32(%rsp)
	call	strcmp
	movl	$9, 76(%rsp)
	testl	%eax, %eax
	je	.L1593
	movl	$.LC53, %esi
	movq	%rbx, %rdi
	call	strcmp
	testl	%eax, %eax
	movl	16(%rsp), %edx
	movl	24(%rsp), %r8d
	movl	48(%rsp), %r9d
	movl	40(%rsp), %r10d
	movl	32(%rsp), %r11d
	jne	.L1602
	movl	$10, 76(%rsp)
	cmpl	$5, 68(%rsp)
	jg	.L1593
.L1602:
	testl	%r12d, %r12d
	jne	.L1603
	cmpl	$1, 68(%rsp)
	jle	.L1860
.L1603:
	testl	%edx, %edx
	jne	.L1604
	cmpl	$2, 68(%rsp)
	jle	.L1861
.L1604:
	testl	%r8d, %r8d
	jne	.L1605
	cmpl	$1, 68(%rsp)
	jle	.L1862
.L1605:
	testl	%r10d, %r10d
	jne	.L1606
	cmpl	$2, 68(%rsp)
	jle	.L1863
.L1606:
	testl	%r9d, %r9d
	jne	.L1607
	cmpl	$1, 68(%rsp)
	jle	.L1864
.L1607:
	testl	%r11d, %r11d
	jne	.L1608
	cmpl	$2, 68(%rsp)
	jle	.L1865
.L1608:
	movl	$0, 76(%rsp)
	testl	%eax, %eax
	jne	.L1593
	movl	%eax, 76(%rsp)
	cmpl	$5, 68(%rsp)
	jg	.L1593
	movl	$.LC60, %edi
	call	puts
	movl	$64, %eax
	jmp	.L1592
.L1659:
	movl	$8, 76(%rsp)
	jmp	.L1593
.L1620:
	movl	$.LC66, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1621
	movq	(%r12), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_bmp_file
	jmp	.L1619
.L1853:
	movl	$.LC66, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1621
	movq	0(%r13), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_bmp_file
	jmp	.L1820
.L1859:
	movl	$.LC66, %esi
	movq	(%r12), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1621
	movq	(%r12), %rcx
	movq	%r13, %rdx
	movq	%r15, %rsi
	movq	56(%rsp), %rdi
	call	load_bmp_file
	jmp	.L1814
.L1861:
	movl	$.LC55, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1592
.L1858:
	movl	$.LC66, %esi
	movq	0(%r13), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1621
	movq	0(%r13), %rcx
	movq	%r14, %rdx
	movq	%r15, %rsi
	movq	80(%rsp), %rdi
	call	load_bmp_file
	jmp	.L1808
.L1645:
	movl	$.LC66, %esi
	movq	88(%rsp), %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1646
	movq	88(%rsp), %rcx
	movl	%ebx, %edx
	movl	%ebp, %esi
	movq	%r13, %rdi
	call	save_bmp_file
	jmp	.L1644
.L1860:
	movl	$.LC54, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1592
.L1646:
	movq	88(%rsp), %rsi
	movl	$.LC67, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1647
.L1862:
	movl	$.LC56, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1592
.L1863:
	movl	$.LC57, %edi
	call	puts
	movl	$64, %eax
	jmp	.L1592
.L1864:
	movl	$.LC58, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1592
.L1865:
	movl	$.LC59, %edi
	call	puts
	movl	$64, %eax
	jmp	.L1592
	.cfi_endproc
.LFE48:
	.size	fjpg, .-fjpg
.globl separate_options
	.type	separate_options, @function
separate_options:
.LFB49:
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
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	movl	%edi, 20(%rsp)
	movq	%rsi, %rbx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%rdx, %r12
	movq	%rcx, 24(%rsp)
	movq	%r8, %rbp
	movq	%r9, 32(%rsp)
	movslq	%edi, %rdi
	addq	%rdi, %rdi
	call	malloc
	movq	%rax, %r15
	movl	$1, 0(%rbp)
	movl	$1, (%r12)
	cmpl	$1, 20(%rsp)
	jle	.L1879
	movl	20(%rsp), %r9d
	subl	$2, %r9d
	movq	$-1, %rdx
	xorl	%eax, %eax
	leaq	2(%r15,%r9,2), %r11
	andl	$3, %r9d
	movq	8(%rbx), %rsi
	movq	%rdx, %rcx
	movq	%rsi, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1945
	cmpb	$45, (%rsi)
	je	.L1991
.L1945:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r15)
.L1894:
	leaq	8(%rbx), %rsi
	leaq	2(%r15), %r8
	cmpq	%r11, %r8
	je	.L1957
	testq	%r9, %r9
	je	.L1990
	cmpq	$1, %r9
	je	.L1943
	cmpq	$2, %r9
	je	.L1944
	movq	8(%rsi), %r9
	movq	%rdx, %rcx
	movq	%r9, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1958
	cmpb	$45, (%r9)
	je	.L1992
.L1958:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r8)
.L1901:
	addq	$8, %rsi
	addq	$2, %r8
.L1944:
	movq	8(%rsi), %r9
	movq	%rdx, %rcx
	movq	%r9, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1963
	cmpb	$45, (%r9)
	je	.L1993
.L1963:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r8)
.L1909:
	addq	$8, %rsi
	addq	$2, %r8
.L1943:
	movq	8(%rsi), %r9
	movq	%rdx, %rcx
	movq	%r9, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1968
	cmpb	$45, (%r9)
	je	.L1994
.L1968:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r8)
.L1917:
	addq	$8, %rsi
	addq	$2, %r8
	cmpq	%r11, %r8
	je	.L1957
.L1990:
	movq	%rbx, %r10
	jmp	.L1874
.L1873:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r8)
.L1869:
	addq	$8, %rsi
	addq	$2, %r8
	movq	8(%rsi), %rbx
	movq	%rdx, %rcx
	movq	%rbx, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1946
	cmpb	$45, (%rbx)
	je	.L1995
.L1946:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r8)
.L1925:
	leaq	2(%r8), %r9
	movq	16(%rsi), %rbx
	movq	%rdx, %rcx
	movq	%rbx, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1977
	cmpb	$45, (%rbx)
	je	.L1996
.L1977:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r9)
.L1932:
	leaq	4(%r8), %r9
	movq	24(%rsi), %rbx
	movq	%rdx, %rcx
	movq	%rbx, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1982
	cmpb	$45, (%rbx)
	je	.L1997
.L1982:
	addl	$1, 0(%rbp)
	movw	$0, 2(%r9)
.L1939:
	addq	$24, %rsi
	addq	$6, %r8
	cmpq	%r11, %r8
	je	.L1998
.L1874:
	movq	8(%rsi), %rbx
	movq	%rdx, %rcx
	movq	%rbx, %rdi
	repnz scasb
	notq	%rcx
	subl	$1, %ecx
	cmpl	$1, %ecx
	jle	.L1873
	cmpb	$45, (%rbx)
	jne	.L1873
	movzbl	1(%rbx), %edi
	leal	-97(%rdi), %r9d
	cmpb	$22, %r9b
	jbe	.L1871
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1869
.L1871:
	cmpl	$2, %ecx
	je	.L1872
	movzbl	2(%rbx), %ecx
	cmpb	$47, %cl
	jle	.L1873
	cmpb	$57, %cl
	jg	.L1873
.L1872:
	addl	$1, (%r12)
	movw	$1, 2(%r8)
	jmp	.L1869
.L1998:
	movq	%r10, %rbx
.L1957:
	movslq	(%r12), %r12
	salq	$3, %r12
.L1867:
	movslq	0(%rbp), %rdi
	salq	$3, %rdi
	call	malloc
	movq	32(%rsp), %r10
	movq	%rax, (%r10)
	movq	%r12, %rdi
	call	malloc
	movq	%rax, %r13
	movq	24(%rsp), %r8
	movq	%rax, (%r8)
	movq	32(%rsp), %rsi
	movq	(%rsi), %r12
	movq	(%rbx), %rdi
	movq	$-1, %rbp
	xorl	%r14d, %r14d
	movq	%rbp, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, (%r12)
	movq	(%rbx), %rdi
	movq	%rbp, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, 0(%r13)
	movq	(%rbx), %rsi
	movq	(%r12), %rdi
	call	strcpy
	movq	24(%rsp), %rdx
	movq	(%rdx), %rax
	movq	(%rbx), %rsi
	movq	(%rax), %rdi
	call	strcpy
	cmpl	$1, 20(%rsp)
	jle	.L1866
	movl	$1, %r14d
	movl	$1, %r13d
	movl	20(%rsp), %r11d
	andl	$1, %r11d
	movl	%r11d, 44(%rsp)
	cmpw	$0, 2(%r15)
	jne	.L1999
	movl	$8, %r12d
	movq	32(%rsp), %r13
	addq	0(%r13), %r12
	movq	8(%rbx), %rdi
	movq	$-1, %rcx
	xorl	%eax, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, (%r12)
	movq	8(%rbx), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	$2, %r13d
.L1949:
	movl	$2, %r12d
	movl	$2, %ebp
	cmpl	$2, 20(%rsp)
	jle	.L1866
	movl	44(%rsp), %r9d
	testl	%r9d, %r9d
	je	.L1878
	cmpw	$0, 2(%r15,%rbp)
	jne	.L1948
.L1887:
	movslq	%r13d, %rsi
	salq	$3, %rsi
	movq	32(%rsp), %r8
	addq	(%r8), %rsi
	movq	8(%rbx,%rbp,4), %rdi
	movq	$-1, %rcx
	xorl	%eax, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movq	%rsi, 8(%rsp)
	call	malloc
	movq	8(%rsp), %rdx
	movq	%rax, (%rdx)
	movq	8(%rbx,%rbp,4), %rsi
	movq	%rax, %rdi
	call	strcpy
	addl	$1, %r13d
	addl	$1, %r12d
	addq	$2, %rbp
	cmpl	%r12d, 20(%rsp)
	jle	.L1866
.L1878:
	cmpw	$0, 2(%r15,%rbp)
	je	.L1876
	movslq	%r14d, %rdx
	salq	$3, %rdx
	movq	24(%rsp), %rsi
	addq	(%rsi), %rdx
	movq	8(%rbx,%rbp,4), %rdi
	movq	$-1, %rcx
	xorl	%eax, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movq	%rdx, 8(%rsp)
	call	malloc
	movq	8(%rsp), %r9
	movq	%rax, (%r9)
	movq	8(%rbx,%rbp,4), %rsi
	movq	%rax, %rdi
	call	strcpy
	addl	$1, %r14d
.L1877:
	addl	$1, %r12d
	addq	$2, %rbp
	cmpw	$0, 2(%r15,%rbp)
	je	.L1887
.L1948:
	movslq	%r14d, %r11
	salq	$3, %r11
	movq	24(%rsp), %r9
	addq	(%r9), %r11
	movq	8(%rbx,%rbp,4), %rdi
	movq	$-1, %rcx
	xorl	%eax, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movq	%r11, 8(%rsp)
	call	malloc
	movq	8(%rsp), %r10
	movq	%rax, (%r10)
	movq	8(%rbx,%rbp,4), %rsi
	movq	%rax, %rdi
	call	strcpy
	addl	$1, %r14d
	addl	$1, %r12d
	addq	$2, %rbp
	cmpl	%r12d, 20(%rsp)
	jg	.L1878
.L1866:
	addq	$56, %rsp
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
.L1999:
	.cfi_restore_state
	movl	$8, %r14d
	movq	24(%rsp), %rbp
	addq	0(%rbp), %r14
	movq	8(%rbx), %rdi
	movq	$-1, %rcx
	xorl	%eax, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, (%r14)
	movq	8(%rbx), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	$2, %r14d
	jmp	.L1949
.L1876:
	movslq	%r13d, %r10
	salq	$3, %r10
	movq	32(%rsp), %r11
	addq	(%r11), %r10
	movq	8(%rbx,%rbp,4), %rdi
	movq	$-1, %rcx
	xorl	%eax, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movq	%r10, 8(%rsp)
	call	malloc
	movq	8(%rsp), %r8
	movq	%rax, (%r8)
	movq	8(%rbx,%rbp,4), %rsi
	movq	%rax, %rdi
	call	strcpy
	addl	$1, %r13d
	jmp	.L1877
.L1997:
	movzbl	1(%rbx), %edi
	leal	-97(%rdi), %r13d
	cmpb	$22, %r13b
	jbe	.L1936
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1939
.L1936:
	cmpl	$2, %ecx
	je	.L1937
	movzbl	2(%rbx), %ecx
	cmpb	$47, %cl
	jle	.L1982
	cmpb	$57, %cl
	jg	.L1982
.L1937:
	addl	$1, (%r12)
	movw	$1, 2(%r9)
	jmp	.L1939
.L1996:
	movzbl	1(%rbx), %edi
	leal	-97(%rdi), %r14d
	cmpb	$22, %r14b
	jbe	.L1929
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1932
.L1929:
	cmpl	$2, %ecx
	je	.L1930
	movzbl	2(%rbx), %ecx
	cmpb	$47, %cl
	jle	.L1977
	cmpb	$57, %cl
	jg	.L1977
.L1930:
	addl	$1, (%r12)
	movw	$1, 2(%r9)
	jmp	.L1932
.L1995:
	movzbl	1(%rbx), %edi
	leal	-97(%rdi), %r9d
	cmpb	$22, %r9b
	jbe	.L1922
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1925
.L1922:
	cmpl	$2, %ecx
	je	.L1923
	movzbl	2(%rbx), %ecx
	cmpb	$47, %cl
	jle	.L1946
	cmpb	$57, %cl
	jg	.L1946
.L1923:
	addl	$1, (%r12)
	movw	$1, 2(%r8)
	jmp	.L1925
.L1991:
	movzbl	1(%rsi), %edi
	leal	-97(%rdi), %r13d
	cmpb	$22, %r13b
	jbe	.L1891
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1894
.L1891:
	cmpl	$2, %ecx
	jne	.L1955
.L1892:
	addl	$1, (%r12)
	movw	$1, 2(%r15)
	jmp	.L1894
.L1992:
	movzbl	1(%r9), %edi
	leal	-97(%rdi), %r14d
	cmpb	$22, %r14b
	jbe	.L1898
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1901
.L1898:
	cmpl	$2, %ecx
	jne	.L1961
.L1899:
	addl	$1, (%r12)
	movw	$1, 2(%r8)
	jmp	.L1901
.L1993:
	movzbl	1(%r9), %edi
	leal	-97(%rdi), %r10d
	cmpb	$22, %r10b
	jbe	.L1906
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1909
.L1906:
	cmpl	$2, %ecx
	jne	.L1966
.L1907:
	addl	$1, (%r12)
	movw	$1, 2(%r8)
	jmp	.L1909
.L1994:
	movzbl	1(%r9), %edi
	leal	-97(%rdi), %r13d
	cmpb	$22, %r13b
	jbe	.L1914
	subb	$65, %dil
	cmpb	$22, %dil
	ja	.L1917
.L1914:
	cmpl	$2, %ecx
	jne	.L1971
.L1915:
	addl	$1, (%r12)
	movw	$1, 2(%r8)
	jmp	.L1917
.L1879:
	movl	$8, %r12d
	jmp	.L1867
.L1955:
	movzbl	2(%rsi), %ecx
	cmpb	$47, %cl
	jle	.L1945
	cmpb	$57, %cl
	jg	.L1945
	jmp	.L1892
.L1966:
	movzbl	2(%r9), %ecx
	cmpb	$47, %cl
	jle	.L1963
	cmpb	$57, %cl
	jg	.L1963
	jmp	.L1907
.L1971:
	movzbl	2(%r9), %ecx
	cmpb	$47, %cl
	jle	.L1968
	cmpb	$57, %cl
	jg	.L1968
	jmp	.L1915
.L1961:
	movzbl	2(%r9), %ecx
	cmpb	$47, %cl
	jle	.L1958
	cmpb	$57, %cl
	jg	.L1958
	jmp	.L1899
	.cfi_endproc
.LFE49:
	.size	separate_options, .-separate_options
	.section	.rodata.str1.8
	.align 8
.LC75:
	.string	"%s outfile.jpg 'equation' n_threads infile(s).jpg [...] [-a|-n|-q val]\n"
	.align 8
.LC76:
	.string	"\t-a\tdisable faster but singlethread assembler loop processing"
	.align 8
.LC77:
	.string	"\t-n\tdisable automatic output file overwrite"
	.section	.rodata.str1.1
.LC78:
	.string	"\t-q\tjpeg quality[100]"
	.text
.globl usage
	.type	usage, @function
usage:
.LFB50:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	%rdi, %rsi
	movl	$.LC75, %edi
	xorl	%eax, %eax
	call	printf
	movl	$.LC76, %edi
	call	puts
	movl	$.LC77, %edi
	call	puts
	movl	$.LC78, %edi
	call	puts
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	fpar_info
	.cfi_endproc
.LFE50:
	.size	usage, .-usage
	.section	.rodata.str1.1
.LC79:
	.string	"Unrecognized option"
.LC80:
	.string	"hanq:"
	.text
.globl main
	.type	main, @function
main:
.LFB51:
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
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	leaq	40(%rsp), %rcx
	leaq	60(%rsp), %rdx
	leaq	48(%rsp), %r9
	leaq	64(%rsp), %r8
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	call	separate_options
.L2029:
	movl	$.LC80, %edx
	movq	40(%rsp), %rsi
	movl	60(%rsp), %edi
	call	getopt
	cmpb	$-1, %al
	je	.L2030
	cmpb	$104, %al
	je	.L2005
	.p2align 4,,7
	jg	.L2008
	cmpb	$97, %al
	.p2align 4,,7
	je	.L2031
.L2003:
	movl	$.LC79, %edi
	call	puts
	movl	$1, %eax
.L2010:
	addq	$88, %rsp
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
.L2008:
	.cfi_restore_state
	cmpb	$110, %al
	je	.L2006
	cmpb	$113, %al
	jne	.L2003
	movq	optarg(%rip), %rdi
	call	atoi
	movl	%eax, jqual(%rip)
	jmp	.L2029
.L2006:
	movl	$0, overwrite_mode(%rip)
	jmp	.L2029
.L2031:
	movl	$0, ualoop(%rip)
	jmp	.L2029
.L2005:
	movq	48(%rsp), %rax
	movq	(%rax), %rsi
	movl	$.LC75, %edi
	xorl	%eax, %eax
	call	printf
	movl	$.LC76, %edi
	call	puts
	movl	$.LC77, %edi
	call	puts
	movl	$.LC78, %edi
	call	puts
	xorl	%eax, %eax
	call	fpar_info
	xorl	%eax, %eax
	addq	$88, %rsp
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
.L2030:
	.cfi_restore_state
	movl	64(%rsp), %r13d
	cmpl	$3, %r13d
	jg	.L2012
	movq	48(%rsp), %rdx
	movq	(%rdx), %rsi
	movl	$.LC75, %edi
	xorl	%eax, %eax
	call	printf
	movl	$.LC76, %edi
	call	puts
	movl	$.LC77, %edi
	call	puts
	movl	$.LC78, %edi
	call	puts
	xorl	%eax, %eax
	call	fpar_info
	movl	$32, %eax
	jmp	.L2010
.L2012:
	leal	-4(%r13), %ecx
	movl	%ecx, 28(%rsp)
	movslq	%ecx, %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, %rbx
	cmpl	$4, %r13d
	je	.L2013
	movq	48(%rsp), %r12
	movq	$-1, %r15
	xorl	%r14d, %r14d
	leal	-5(%r13), %r8d
	andl	$3, %r8d
	movq	32(%r12), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movl	%r8d, 16(%rsp)
	call	malloc
	movq	%rax, (%rbx)
	movq	32(%r12), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	$5, %edx
	movl	$8, %ebp
	cmpl	$5, %r13d
	movl	16(%rsp), %r8d
	jle	.L2013
	testl	%r8d, %r8d
	je	.L2014
	cmpl	$1, %r8d
	je	.L2027
	cmpl	$2, %r8d
	je	.L2028
	movq	40(%r12), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, 8(%rbx)
	movq	40(%r12), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	$6, %edx
	movl	$16, %ebp
.L2028:
	movq	32(%r12,%rbp), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movl	%edx, 8(%rsp)
	call	malloc
	movq	%rax, (%rbx,%rbp)
	movq	32(%r12,%rbp), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %rbp
.L2027:
	movq	32(%r12,%rbp), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movl	%edx, 8(%rsp)
	call	malloc
	movq	%rax, (%rbx,%rbp)
	movq	32(%r12,%rbp), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	8(%rsp), %edx
	addl	$1, %edx
	addq	$8, %rbp
	cmpl	%edx, %r13d
	jle	.L2013
.L2014:
	movq	32(%r12,%rbp), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movl	%edx, 8(%rsp)
	call	malloc
	movq	%rax, (%rbx,%rbp)
	movq	32(%r12,%rbp), %rsi
	movq	%rax, %rdi
	call	strcpy
	leaq	8(%rbp), %r8
	movq	32(%r12,%r8), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movq	%r8, 16(%rsp)
	call	malloc
	movq	16(%rsp), %rdx
	movq	%rax, (%rbx,%rdx)
	movq	32(%r12,%rdx), %rsi
	movq	%rax, %rdi
	call	strcpy
	leaq	16(%rbp), %r11
	movq	32(%r12,%r11), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movq	%r11, 16(%rsp)
	call	malloc
	movq	16(%rsp), %r10
	movq	%rax, (%rbx,%r10)
	movq	32(%r12,%r10), %rsi
	movq	%rax, %rdi
	call	strcpy
	leaq	24(%rbp), %r9
	movq	32(%r12,%r9), %rdi
	movq	%r15, %rcx
	movl	%r14d, %eax
	repnz scasb
	notq	%rcx
	movq	%rcx, %rdi
	movq	%r9, 16(%rsp)
	call	malloc
	movq	16(%rsp), %rsi
	movq	%rax, (%rbx,%rsi)
	movq	32(%r12,%rsi), %rsi
	movq	%rax, %rdi
	call	strcpy
	movl	8(%rsp), %edx
	addl	$4, %edx
	addq	$32, %rbp
	cmpl	%edx, %r13d
	jg	.L2014
.L2013:
	movq	48(%rsp), %r12
	movq	24(%r12), %rdi
	call	atoi
	movl	%eax, %r8d
	movq	48(%rsp), %rbp
	movq	16(%rbp), %rsi
	movq	8(%rbp), %rdi
	movq	%rbx, %rcx
	movl	28(%rsp), %edx
	call	fjpg
	jmp	.L2010
	.cfi_endproc
.LFE51:
	.size	main, .-main
.globl jqual
	.data
	.align 4
	.type	jqual, @object
	.size	jqual, 4
jqual:
	.long	100
.globl ualoop
	.align 16
	.type	ualoop, @object
	.size	ualoop, 4
ualoop:
	.long	1
.globl overwrite_mode
	.align 16
	.type	overwrite_mode, @object
	.size	overwrite_mode, 4
overwrite_mode:
	.long	1
	.comm	icache,8,16
	.comm	ncache,4,16
	.local	errptr
	.comm	errptr,8,8
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC32:
	.long	4
	.long	4
	.long	4
	.long	4
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC35:
	.long	0
	.long	1138753536
	.align 8
.LC39:
	.long	2576980378
	.long	1068079513
	.ident	"GCC: (NetBSD nb2 20110806) 4.5.3"
