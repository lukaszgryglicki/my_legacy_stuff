	.file	"fjpeg.c"
	.text
	.p2align 4,,15
	.type	my_error_exit, @function
my_error_exit:
.LFB46:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	(%rdi), %rbx
	call	*16(%rbx)
	leaq	168(%rbx), %rdi
	movl	$1, %esi
	call	longjmp
	.cfi_endproc
.LFE46:
	.size	my_error_exit, .-my_error_exit
	.p2align 4,,15
	.globl	alloc_cache
	.type	alloc_cache, @function
alloc_cache:
.LFB47:
	.cfi_startproc
	movslq	%edi, %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$0, ncache(%rip)
	leaq	(%rdi,%rdi,2), %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, icache(%rip)
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE47:
	.size	alloc_cache, .-alloc_cache
	.p2align 4,,15
	.globl	lookup_cache
	.type	lookup_cache, @function
lookup_cache:
.LFB48:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$40, %rsp
	.cfi_def_cfa_offset 96
	movl	ncache(%rip), %eax
	movq	%rsi, 8(%rsp)
	movq	%rdx, 16(%rsp)
	movq	%rcx, 24(%rsp)
	testl	%eax, %eax
	jle	.L10
	movq	icache(%rip), %rbx
	subl	$1, %eax
	movq	%rdi, %rbp
	leaq	(%rax,%rax,2), %rdx
	xorl	%r12d, %r12d
	leaq	24(%rbx), %rcx
	movq	(%rbx), %rsi
	movq	%rbx, %r15
	leaq	(%rcx,%rdx,8), %r14
	movq	%rcx, (%rsp)
	call	strcmp
	movq	%r14, %r13
	movq	(%rsp), %rsi
	subq	%rbx, %r13
	subq	$24, %r13
	shrq	$3, %r13
	leaq	0(%r13,%r13,2), %r13
	andl	$7, %r13d
	testl	%eax, %eax
	je	.L59
	cmpq	%r14, %rsi
	movq	%rsi, %r15
	je	.L10
	testq	%r13, %r13
	je	.L8
	cmpq	$1, %r13
	je	.L46
	cmpq	$2, %r13
	je	.L47
	cmpq	$3, %r13
	je	.L48
	cmpq	$4, %r13
	je	.L49
	cmpq	$5, %r13
	je	.L50
	cmpq	$6, %r13
	je	.L51
	movq	%rsi, %r12
	movq	(%rsi), %rsi
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
.L51:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
.L50:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
.L49:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
.L48:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
.L47:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
.L46:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
	cmpq	%r14, %r15
	je	.L10
.L8:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	leaq	24(%r15), %r13
	movq	%rbp, %rdi
	movq	0(%r13), %rsi
	movq	%r13, %r12
	movq	%r13, %r15
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	addq	$24, %r15
	movq	%rbp, %rdi
	movq	(%r15), %rsi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	leaq	48(%r13), %r15
	movq	48(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	leaq	72(%r13), %r15
	movq	72(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	leaq	96(%r13), %r15
	movq	96(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	leaq	120(%r13), %r15
	movq	120(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	leaq	144(%r13), %r15
	movq	144(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L59
	leaq	168(%r13), %r15
	cmpq	%r14, %r15
	jne	.L8
.L10:
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	$1, %eax
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
	.p2align 4,,10
.L59:
	.cfi_restore_state
	movq	8(%r15), %rdi
	movq	8(%rsp), %r8
	movq	16(%rsp), %r10
	movq	24(%rsp), %rax
	movq	%rdi, (%r8)
	addq	icache(%rip), %r12
	movl	16(%r12), %r9d
	movl	%r9d, (%r10)
	movl	20(%r12), %r11d
	movl	%r11d, (%rax)
	addq	$40, %rsp
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
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
.LFE48:
	.size	lookup_cache, .-lookup_cache
	.p2align 4,,15
	.globl	insert_cache
	.type	insert_cache, @function
insert_cache:
.LFB49:
	.cfi_startproc
	movq	%rbx, -48(%rsp)
	movq	%rbp, -40(%rsp)
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	movq	%r12, -32(%rsp)
	movq	%r13, -24(%rsp)
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	movq	%rsi, %r13
	movq	%r14, -16(%rsp)
	movq	%r15, -8(%rsp)
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movl	ncache(%rip), %r12d
	movl	%ecx, %r15d
	movq	icache(%rip), %rcx
	movl	%edx, %r14d
	movslq	%r12d, %rax
	addl	$1, %r12d
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rcx,%rdx,8), %rbx
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	%rbp, %rsi
	movq	%rax, (%rbx)
	movq	%rax, %rdi
	call	strcpy
	movl	%r14d, 16(%rbx)
	movl	%r15d, 20(%rbx)
	movq	%r13, 8(%rbx)
	movl	%r12d, ncache(%rip)
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	movq	40(%rsp), %r14
	movq	48(%rsp), %r15
	addq	$56, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE49:
	.size	insert_cache, .-insert_cache
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"fstat"
.LC1:
	.string	"mmap"
	.text
	.p2align 4,,15
	.globl	fmmf
	.type	fmmf, @function
fmmf:
.LFB50:
	.cfi_startproc
	movq	%rbp, -16(%rsp)
	movq	%r12, -8(%rsp)
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movq	%rdx, %rbp
	movq	%rbx, -24(%rsp)
	subq	$152, %rsp
	.cfi_def_cfa_offset 160
	.cfi_offset 3, -32
	movl	__isthreaded(%rip), %eax
	movq	$0, (%rsi)
	movq	%rsi, %r12
	movq	$0, (%rdx)
	testl	%eax, %eax
	jne	.L65
	movswl	18(%rdi), %ebx
	movq	%rsp, %rsi
	movl	%ebx, %edi
	call	fstat
	testl	%eax, %eax
	js	.L71
.L67:
	movq	72(%rsp), %rsi
	xorl	%r9d, %r9d
	xorl	%edi, %edi
	movl	%ebx, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	call	mmap
	cmpq	$-1, %rax
	je	.L72
	movq	72(%rsp), %rdx
	movq	%rax, (%r12)
	xorl	%eax, %eax
	movq	%rdx, 0(%rbp)
.L68:
	movq	128(%rsp), %rbx
	movq	136(%rsp), %rbp
	movq	144(%rsp), %r12
	addq	$152, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L65:
	.cfi_restore_state
	call	fileno
	movl	%eax, %ebx
	movq	%rsp, %rsi
	movl	%ebx, %edi
	call	fstat
	testl	%eax, %eax
	jns	.L67
.L71:
	movl	$.LC0, %edi
	call	perror
	movl	$1, %eax
	jmp	.L68
	.p2align 4,,10
.L72:
	movl	$.LC1, %edi
	call	perror
	movl	$2, %eax
	jmp	.L68
	.cfi_endproc
.LFE50:
	.size	fmmf, .-fmmf
	.p2align 4,,15
	.globl	RGB
	.type	RGB, @function
RGB:
.LFB51:
	.cfi_startproc
	sall	$16, %edi
	sall	$8, %esi
	leal	(%rdi,%rsi), %eax
	addl	%edx, %eax
	cltq
	ret
	.cfi_endproc
.LFE51:
	.size	RGB, .-RGB
	.p2align 4,,15
	.globl	dRGB
	.type	dRGB, @function
dRGB:
.LFB52:
	.cfi_startproc
	movq	%rcx, %rax
	shrq	$16, %rax
	movb	%al, (%rdi)
	movzbl	%ch, %edi
	movb	%dil, (%rsi)
	movb	%cl, (%rdx)
	ret
	.cfi_endproc
.LFE52:
	.size	dRGB, .-dRGB
	.p2align 4,,15
	.globl	set_jpeg_rows_array
	.type	set_jpeg_rows_array, @function
set_jpeg_rows_array:
.LFB53:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	testl	%r9d, %r9d
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	$0, -12(%rsp)
	movl	-12(%rsp), %eax
	je	.L76
	cmpl	%eax, %esi
	jle	.L75
	.p2align 4,,10
.L89:
	movslq	-12(%rsp), %r12
	movl	$0, -8(%rsp)
	movl	-8(%rsp), %eax
	cmpl	%eax, %ecx
	movq	(%rdx,%r12,8), %r10
	jle	.L81
	.p2align 4,,10
.L90:
	addq	$1, %r10
	movl	%edi, %r12d
	movzbl	-1(%r10), %r11d
	movl	%r11d, -4(%rsp)
	movl	-12(%rsp), %r11d
	movslq	-8(%rsp), %rbx
	movl	-4(%rsp), %r9d
	movl	-4(%rsp), %eax
	movl	-4(%rsp), %ebp
	subl	%r11d, %r12d
	sall	$16, %r9d
	movslq	%r12d, %r11
	sall	$8, %eax
	movq	(%r8,%r11,8), %r12
	addl	%r9d, %eax
	addl	%ebp, %eax
	cltq
	movq	%rax, (%r12,%rbx,8)
	movl	-8(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, -8(%rsp)
	movl	-8(%rsp), %r9d
	cmpl	%ecx, %r9d
	jl	.L90
.L81:
	movl	-12(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -12(%rsp)
	movl	-12(%rsp), %eax
	cmpl	%esi, %eax
	jl	.L89
.L75:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L76:
	.cfi_restore_state
	cmpl	%eax, %esi
	jle	.L75
	.p2align 4,,10
.L88:
	movslq	-12(%rsp), %rbx
	movl	$0, -8(%rsp)
	movl	-8(%rsp), %r9d
	cmpl	%r9d, %ecx
	movq	(%rdx,%rbx,8), %rbp
	jle	.L86
	.p2align 4,,10
.L91:
	movzbl	0(%rbp), %r10d
	movzbl	1(%rbp), %r12d
	movl	%edi, %ebx
	addq	$3, %rbp
	sall	$16, %r10d
	sall	$8, %r12d
	movl	%r10d, -4(%rsp)
	movl	-4(%rsp), %r11d
	addl	%r11d, %r12d
	movl	%r12d, -4(%rsp)
	movl	-12(%rsp), %eax
	movslq	-8(%rsp), %r10
	movl	-4(%rsp), %r11d
	subl	%eax, %ebx
	movzbl	-1(%rbp), %eax
	movslq	%ebx, %r9
	movq	(%r8,%r9,8), %r12
	addl	%eax, %r11d
	movslq	%r11d, %r11
	movq	%r11, (%r12,%r10,8)
	movl	-8(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -8(%rsp)
	movl	-8(%rsp), %ebx
	cmpl	%ebx, %ecx
	jg	.L91
.L86:
	movl	-12(%rsp), %ebp
	addl	$1, %ebp
	movl	%ebp, -12(%rsp)
	movl	-12(%rsp), %r9d
	cmpl	%r9d, %esi
	jg	.L88
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE53:
	.size	set_jpeg_rows_array, .-set_jpeg_rows_array
	.section	.rodata.str1.1
.LC2:
	.string	"slow"
	.text
	.p2align 4,,15
	.globl	set_jpeg_rows
	.type	set_jpeg_rows, @function
set_jpeg_rows:
.LFB54:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rcx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movslq	%edi, %r12
	movl	$.LC2, %edi
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movl	%edx, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	movq	(%rsi), %rbx
	call	puts
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	cmpl	%eax, %ebp
	jle	.L93
	movq	0(%r13), %rdx
	movq	(%rdx,%r12,8), %r8
	.p2align 4,,10
.L96:
	movzbl	(%rbx), %ecx
	movzbl	1(%rbx), %esi
	addq	$3, %rbx
	movzbl	-1(%rbx), %edi
	movl	%ecx, 4(%rsp)
	movl	%esi, 8(%rsp)
	movl	%edi, 12(%rsp)
	movslq	(%rsp), %r9
	movl	12(%rsp), %r10d
	movl	8(%rsp), %eax
	movl	4(%rsp), %r11d
	movl	(%rsp), %r12d
	sall	$8, %eax
	addl	$1, %r12d
	sall	$16, %r11d
	movl	%r12d, (%rsp)
	movl	(%rsp), %r13d
	addl	%r11d, %eax
	addl	%r10d, %eax
	cltq
	cmpl	%ebp, %r13d
	movq	%rax, (%r8,%r9,8)
	jl	.L96
.L93:
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
.LFE54:
	.size	set_jpeg_rows, .-set_jpeg_rows
	.p2align 4,,15
	.globl	set_pngrow_pointers
	.type	set_pngrow_pointers, @function
set_pngrow_pointers:
.LFB55:
	.cfi_startproc
	movl	%esi, -4(%rsp)
	movl	$0, -12(%rsp)
	movl	-12(%rsp), %eax
	cmpl	%eax, %esi
	jle	.L98
	.p2align 4,,10
.L106:
	movl	-4(%rsp), %r8d
	subl	$1, %r8d
	movl	%r8d, -4(%rsp)
	movslq	-12(%rsp), %r9
	movl	$0, -8(%rsp)
	movl	-8(%rsp), %r10d
	cmpl	%r10d, %edi
	movq	(%rdx,%r9,8), %r11
	jle	.L103
	.p2align 4,,10
.L107:
	movl	-8(%rsp), %eax
	movslq	-4(%rsp), %r9
	movslq	-8(%rsp), %r10
	leal	(%rax,%rax,2), %r8d
	movq	(%rcx,%r9,8), %r9
	movslq	%r8d, %rax
	movl	-8(%rsp), %r8d
	movq	(%r9,%r10,8), %r10
	leal	(%r8,%r8,2), %r9d
	movb	%r10b, 2(%r11,%rax)
	movslq	-4(%rsp), %rax
	movslq	%r9d, %r8
	movslq	-8(%rsp), %r10
	movq	(%rcx,%rax,8), %rax
	movzbl	1(%rax,%r10,8), %r10d
	movb	%r10b, 1(%r11,%r8)
	movl	-8(%rsp), %r8d
	movslq	-4(%rsp), %r9
	movslq	-8(%rsp), %rax
	leal	(%r8,%r8,2), %r10d
	movq	(%rcx,%r9,8), %r9
	movslq	%r10d, %r8
	movl	-8(%rsp), %r10d
	movzbl	2(%r9,%rax,8), %eax
	addl	$1, %r10d
	movl	%r10d, -8(%rsp)
	movb	%al, (%r11,%r8)
	movl	-8(%rsp), %r8d
	cmpl	%edi, %r8d
	jl	.L107
.L103:
	movl	-12(%rsp), %r11d
	addl	$1, %r11d
	movl	%r11d, -12(%rsp)
	movl	-12(%rsp), %r9d
	cmpl	%r9d, %esi
	jg	.L106
	rep; ret
.L98:
	rep; ret
	.cfi_endproc
.LFE55:
	.size	set_pngrow_pointers, .-set_pngrow_pointers
	.p2align 4,,15
	.globl	set_pngrows_array
	.type	set_pngrows_array, @function
set_pngrows_array:
.LFB56:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	cmpl	$4, %r8d
	movl	%esi, -4(%rsp)
	je	.L133
	cmpl	$3, %r8d
	je	.L134
	cmpl	$1, %r8d
	je	.L135
.L108:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	.p2align 4,,2
	ret
.L133:
	.cfi_restore_state
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %r9d
	cmpl	%r9d, %esi
	jle	.L108
	.p2align 4,,10
.L128:
	movl	-4(%rsp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rsp)
	movslq	-16(%rsp), %r10
	movl	$0, -12(%rsp)
	movl	-12(%rsp), %r8d
	cmpl	%r8d, %edi
	movq	(%rdx,%r10,8), %rbx
	jle	.L114
	.p2align 4,,10
.L129:
	movl	-12(%rsp), %eax
	sall	$2, %eax
	cltq
	movzbl	(%rbx,%rax), %r11d
	sall	$16, %r11d
	movl	%r11d, -8(%rsp)
	movl	-12(%rsp), %eax
	movl	-8(%rsp), %r9d
	sall	$2, %eax
	cltq
	movzbl	1(%rbx,%rax), %r10d
	sall	$8, %r10d
	addl	%r9d, %r10d
	movl	%r10d, -8(%rsp)
	movl	-12(%rsp), %r8d
	movl	-8(%rsp), %r11d
	sall	$2, %r8d
	movslq	%r8d, %rax
	movzbl	2(%rbx,%rax), %r9d
	addl	%r9d, %r11d
	movl	%r11d, -8(%rsp)
	movslq	-4(%rsp), %r8
	movslq	-12(%rsp), %r11
	movslq	-8(%rsp), %r10
	movl	-12(%rsp), %r9d
	movq	(%rcx,%r8,8), %rax
	addl	$1, %r9d
	movl	%r9d, -12(%rsp)
	movl	-12(%rsp), %r8d
	movq	%r10, (%rax,%r11,8)
	cmpl	%edi, %r8d
	jl	.L129
.L114:
	movl	-16(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, -16(%rsp)
	movl	-16(%rsp), %r11d
	cmpl	%r11d, %esi
	jg	.L128
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L134:
	.cfi_restore_state
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %r11d
	cmpl	%r11d, %esi
	jle	.L108
	.p2align 4,,10
.L122:
	movl	-4(%rsp), %r9d
	subl	$1, %r9d
	movl	%r9d, -4(%rsp)
	movslq	-16(%rsp), %r10
	movl	$0, -12(%rsp)
	movl	-12(%rsp), %ebx
	cmpl	%ebx, %edi
	movq	(%rdx,%r10,8), %r8
	jle	.L120
	.p2align 4,,10
.L130:
	movl	-12(%rsp), %eax
	leal	(%rax,%rax,2), %eax
	cltq
	movzbl	(%r8,%rax), %r11d
	sall	$16, %r11d
	movl	%r11d, -8(%rsp)
	movl	-12(%rsp), %r9d
	movl	-8(%rsp), %r10d
	leal	(%r9,%r9,2), %eax
	cltq
	movzbl	1(%r8,%rax), %ebx
	sall	$8, %ebx
	addl	%r10d, %ebx
	movl	%ebx, -8(%rsp)
	movl	-12(%rsp), %r11d
	movl	-8(%rsp), %r9d
	leal	(%r11,%r11,2), %eax
	cltq
	movzbl	2(%r8,%rax), %r10d
	addl	%r9d, %r10d
	movl	%r10d, -8(%rsp)
	movslq	-4(%rsp), %rbx
	movslq	-12(%rsp), %r11
	movslq	-8(%rsp), %r9
	movl	-12(%rsp), %r10d
	movq	(%rcx,%rbx,8), %rax
	addl	$1, %r10d
	movl	%r10d, -12(%rsp)
	movl	-12(%rsp), %ebx
	movq	%r9, (%rax,%r11,8)
	cmpl	%ebx, %edi
	jg	.L130
.L120:
	movl	-16(%rsp), %r8d
	addl	$1, %r8d
	movl	%r8d, -16(%rsp)
	movl	-16(%rsp), %r11d
	cmpl	%r11d, %esi
	jg	.L122
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L135:
	.cfi_restore_state
	movl	$0, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %esi
	jle	.L108
	.p2align 4,,10
.L127:
	movl	-4(%rsp), %ebx
	subl	$1, %ebx
	movl	%ebx, -4(%rsp)
	movslq	-16(%rsp), %r8
	movl	$0, -12(%rsp)
	movl	-12(%rsp), %r9d
	cmpl	%r9d, %edi
	movq	(%rdx,%r8,8), %rbx
	jle	.L125
	.p2align 4,,10
.L131:
	movslq	-12(%rsp), %r10
	movzbl	(%rbx,%r10), %r11d
	movl	%r11d, -8(%rsp)
	movslq	-4(%rsp), %r9
	movslq	-12(%rsp), %r10
	movl	-8(%rsp), %r8d
	movl	-8(%rsp), %eax
	movl	-8(%rsp), %r11d
	sall	$16, %r8d
	movq	(%rcx,%r9,8), %r9
	sall	$8, %eax
	addl	%r8d, %eax
	addl	%r11d, %eax
	cltq
	movq	%rax, (%r9,%r10,8)
	movl	-12(%rsp), %r10d
	addl	$1, %r10d
	movl	%r10d, -12(%rsp)
	movl	-12(%rsp), %r8d
	cmpl	%r8d, %edi
	jg	.L131
.L125:
	movl	-16(%rsp), %ebx
	addl	$1, %ebx
	movl	%ebx, -16(%rsp)
	movl	-16(%rsp), %eax
	cmpl	%eax, %esi
	jg	.L127
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE56:
	.size	set_pngrows_array, .-set_pngrows_array
	.p2align 4,,15
	.globl	init_bmp
	.type	init_bmp, @function
init_bmp:
.LFB57:
	.cfi_startproc
	movl	$24, %edx
	movl	$1, %ecx
	movb	$66, (%rdi)
	movb	$77, 1(%rdi)
	movl	$0, 4(%rdi)
	movl	$0, 8(%rdi)
	movl	$56, 12(%rdi)
	movl	$32, 24(%rdi)
	movl	$32, 20(%rdi)
	movl	$40, 16(%rdi)
	movw	%dx, 30(%rdi)
	movw	%cx, 28(%rdi)
	movl	$0, 32(%rdi)
	movl	$3072, 36(%rdi)
	movl	$0, 40(%rdi)
	movl	$0, 44(%rdi)
	movl	$0, 48(%rdi)
	movl	$0, 52(%rdi)
	ret
	.cfi_endproc
.LFE57:
	.size	init_bmp, .-init_bmp
	.section	.rodata.str1.1
.LC3:
	.string	"load_bmp: cannot open: %s\n"
.LC4:
	.string	"%c%c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"load_bmp: file %s is truncated, skipping\n"
	.align 8
.LC6:
	.string	"load_bmp: file %s doesn't start with BM - not recognized as a bitmap\n"
	.align 8
.LC7:
	.string	"Only 8,24,32 BPP BMPs suported: current bpp: %d\n"
	.section	.rodata.str1.1
.LC8:
	.string	"%c"
.LC9:
	.string	"%c%c%c"
.LC10:
	.string	"%c%c%c%c"
.LC11:
	.string	"rb"
	.section	.rodata.str1.8
	.align 8
.LC12:
	.string	"load_bmp: warning: mmap file failed, using normal buffered I/O: %s\n"
	.text
	.p2align 4,,15
	.globl	load_bmp_file
	.type	load_bmp_file, @function
load_bmp_file:
.LFB58:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rcx, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$264, %rsp
	.cfi_def_cfa_offset 320
	movl	ncache(%rip), %eax
	movq	%rdi, 24(%rsp)
	movq	%rsi, 32(%rsp)
	movq	%rdx, 40(%rsp)
	testl	%eax, %eax
	jle	.L138
	movq	icache(%rip), %rbx
	subl	$1, %eax
	movq	%rbp, %rdi
	leaq	(%rax,%rax,2), %rdx
	xorl	%r12d, %r12d
	leaq	24(%rbx), %rcx
	movq	(%rbx), %rsi
	movq	%rbx, %r15
	leaq	(%rcx,%rdx,8), %r14
	movq	%rcx, (%rsp)
	call	strcmp
	movq	%r14, %r13
	movq	(%rsp), %rsi
	subq	%rbx, %r13
	subq	$24, %r13
	shrq	$3, %r13
	leaq	0(%r13,%r13,2), %r13
	andl	$7, %r13d
	testl	%eax, %eax
	je	.L367
	cmpq	%r14, %rsi
	movq	%rsi, %r15
	je	.L138
	testq	%r13, %r13
	je	.L141
	cmpq	$1, %r13
	je	.L328
	cmpq	$2, %r13
	je	.L329
	cmpq	$3, %r13
	je	.L330
	cmpq	$4, %r13
	je	.L331
	cmpq	$5, %r13
	je	.L332
	cmpq	$6, %r13
	je	.L333
	movq	%rsi, %r12
	movq	(%rsi), %rsi
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
.L333:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
.L332:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
.L331:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
.L330:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
.L329:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
.L328:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
	cmpq	%r14, %r15
	je	.L138
.L141:
	movq	(%r15), %rsi
	movq	%r15, %r12
	movq	%rbp, %rdi
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	leaq	24(%r15), %r13
	movq	%rbp, %rdi
	movq	0(%r13), %rsi
	movq	%r13, %r12
	movq	%r13, %r15
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	addq	$24, %r15
	movq	%rbp, %rdi
	movq	(%r15), %rsi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	leaq	48(%r13), %r15
	movq	48(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	leaq	72(%r13), %r15
	movq	72(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	leaq	96(%r13), %r15
	movq	96(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	leaq	120(%r13), %r15
	movq	120(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	leaq	144(%r13), %r15
	movq	144(%r13), %rsi
	movq	%rbp, %rdi
	movq	%r15, %r12
	subq	%rbx, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L367
	leaq	168(%r13), %r15
	cmpq	%r14, %r15
	jne	.L141
.L138:
	movl	$.LC11, %esi
	movq	%rbp, %rdi
	call	fopen
	testq	%rax, %rax
	movq	%rax, %rbx
	je	.L398
	movl	__isthreaded(%rip), %edi
	testl	%edi, %edi
	jne	.L142
	movswl	18(%rbx), %r14d
.L143:
	leaq	128(%rsp), %rsi
	movl	%r14d, %edi
	call	fstat
	testl	%eax, %eax
	js	.L399
	movq	200(%rsp), %rsi
	xorl	%r9d, %r9d
	movl	%r14d, %r8d
	xorl	%edi, %edi
	movl	$2, %ecx
	movl	$1, %edx
	call	mmap
	cmpq	$-1, %rax
	movq	%rax, %r14
	movq	200(%rsp), %r13
	je	.L400
.L192:
	leaq	96(%rsp), %rcx
	leaq	80(%rsp), %rdx
	movl	$24, %r8d
	movl	$1, %r9d
	xorl	%eax, %eax
	movl	$.LC4, %esi
	movq	%rbx, %rdi
	movb	$66, 128(%rsp)
	movb	$77, 129(%rsp)
	movl	$0, 132(%rsp)
	movl	$0, 136(%rsp)
	movl	$56, 140(%rsp)
	movl	$32, 152(%rsp)
	movl	$32, 148(%rsp)
	movl	$40, 144(%rsp)
	movw	%r8w, 158(%rsp)
	movw	%r9w, 156(%rsp)
	movl	$0, 160(%rsp)
	movl	$3072, 164(%rsp)
	movl	$0, 168(%rsp)
	movl	$0, 172(%rsp)
	movl	$0, 176(%rsp)
	movl	$0, 180(%rsp)
	call	fscanf
	cmpl	$2, %eax
	je	.L147
	xorl	%eax, %eax
	movq	%rbp, %rsi
	movl	$.LC5, %edi
	call	printf
	testq	%r14, %r14
	je	.L148
	movq	%r13, %rsi
	movq	%r14, %rdi
	call	munmap
.L148:
	movq	%rbx, %rdi
	call	fclose
	movl	$2, %eax
.L368:
	addq	$264, %rsp
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
	.p2align 4,,10
.L367:
	.cfi_restore_state
	movq	8(%r15), %rbp
	movq	24(%rsp), %r15
	xorl	%eax, %eax
	movq	32(%rsp), %r13
	movq	40(%rsp), %r14
	movq	%rbp, (%r15)
	addq	icache(%rip), %r12
	movl	16(%r12), %ebx
	movl	%ebx, 0(%r13)
	movl	20(%r12), %r12d
	movl	%r12d, (%r14)
	jmp	.L368
	.p2align 4,,10
.L147:
	cmpb	$66, 80(%rsp)
	je	.L401
.L149:
	xorl	%eax, %eax
	movq	%rbp, %rsi
	movl	$.LC6, %edi
	call	printf
	testq	%r14, %r14
	je	.L151
	movq	%r13, %rsi
	movq	%r14, %rdi
	call	munmap
.L151:
	movq	%rbx, %rdi
	call	fclose
	movl	$3, %eax
	jmp	.L368
	.p2align 4,,10
.L142:
	movq	%rbx, %rdi
	call	fileno
	movl	%eax, %r14d
	jmp	.L143
	.p2align 4,,10
.L399:
	movl	$.LC0, %edi
	call	perror
.L145:
	movq	%rbp, %rsi
	movl	$.LC12, %edi
	xorl	%eax, %eax
	call	printf
	xorl	%r14d, %r14d
	xorl	%r13d, %r13d
	jmp	.L192
	.p2align 4,,10
.L398:
	movq	%rbp, %rsi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movl	$1, %eax
	jmp	.L368
	.p2align 4,,10
.L400:
	movl	$.LC1, %edi
	call	perror
	jmp	.L145
	.p2align 4,,10
.L401:
	cmpb	$77, 96(%rsp)
	jne	.L149
	leaq	132(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	136(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	140(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	144(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	148(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	152(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	leaq	156(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	leaq	158(%rsp), %rdi
	movl	$2, %esi
	movq	%rbx, %rcx
	movl	$1, %edx
	call	fread
	movzwl	158(%rsp), %r10d
	movswl	%r10w, %esi
	andl	$-17, %r10d
	cmpw	$8, %r10w
	movl	%esi, 64(%rsp)
	je	.L152
	cmpl	$32, %esi
	je	.L152
	xorl	%eax, %eax
	movl	$.LC7, %edi
	call	printf
	testq	%r14, %r14
	je	.L153
	movq	%r13, %rsi
	movq	%r14, %rdi
	call	munmap
.L153:
	movq	%rbx, %rdi
	call	fclose
	movl	$4, %eax
	jmp	.L368
.L152:
	movslq	140(%rsp), %rsi
	xorl	%edx, %edx
	movq	%rbx, %rdi
	call	fseek
	movl	152(%rsp), %r11d
	movl	$8, %esi
	movl	148(%rsp), %r15d
	movslq	%r11d, %rdi
	movl	%r11d, 52(%rsp)
	call	calloc
	movl	52(%rsp), %ecx
	movq	%rax, %rdx
	movq	24(%rsp), %rax
	testl	%ecx, %ecx
	movq	%rdx, (%rax)
	jle	.L157
	movl	52(%rsp), %r8d
	movslq	%r15d, %rsi
	xorl	%r12d, %r12d
	movq	%rsi, 56(%rsp)
	subl	$1, %r8d
	leaq	8(,%r8,8), %rdi
	andl	$7, %r8d
	movq	%rdi, 72(%rsp)
	je	.L388
	movq	56(%rsp), %rdi
	movl	$8, %esi
	movq	%rdx, 8(%rsp)
	movq	%r8, (%rsp)
	movl	$8, %r12d
	call	calloc
	movq	(%rsp), %r10
	movq	8(%rsp), %rdx
	movq	24(%rsp), %r9
	cmpq	$1, %r10
	movq	%rax, (%rdx)
	movq	(%r9), %rdx
	je	.L388
	cmpq	$2, %r10
	je	.L323
	cmpq	$3, %r10
	je	.L324
	cmpq	$4, %r10
	je	.L325
	cmpq	$5, %r10
	je	.L326
	cmpq	$6, %r10
	je	.L327
	movq	56(%rsp), %rdi
	movl	$8, %esi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %r11
	movq	%rax, (%r11,%r12)
	movq	24(%rsp), %rax
	movb	$16, %r12b
	movq	(%rax), %rdx
.L327:
	movq	56(%rsp), %rdi
	movl	$8, %esi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rcx
	movq	24(%rsp), %rsi
	movq	%rax, (%rcx,%r12)
	movq	(%rsi), %rdx
	addq	$8, %r12
.L326:
	movq	56(%rsp), %rdi
	movl	$8, %esi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %r8
	movq	24(%rsp), %rdi
	movq	%rax, (%r8,%r12)
	movq	(%rdi), %rdx
	addq	$8, %r12
.L325:
	movq	56(%rsp), %rdi
	movl	$8, %esi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdx
	movq	24(%rsp), %r9
	movq	%rax, (%rdx,%r12)
	movq	(%r9), %rdx
	addq	$8, %r12
.L324:
	movq	56(%rsp), %rdi
	movl	$8, %esi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %r10
	movq	24(%rsp), %r11
	movq	%rax, (%r10,%r12)
	movq	(%r11), %rdx
	addq	$8, %r12
.L323:
	movq	56(%rsp), %rdi
	movl	$8, %esi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rcx
	movq	%rbp, %r8
	movq	56(%rsp), %rbp
	movq	%rbx, %r9
	movq	%rax, (%rcx,%r12)
	movq	24(%rsp), %rax
	addq	$8, %r12
	movq	(%rax), %rdx
	movq	%rax, %rbx
.L158:
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rdx, 8(%rsp)
	movq	%r9, (%rsp)
	movq	%r8, 16(%rsp)
	call	calloc
	leaq	8(%r12), %rdi
	cmpq	72(%rsp), %rdi
	movq	8(%rsp), %rsi
	movq	(%rsp), %r10
	movq	16(%rsp), %r11
	movq	%rax, (%rsi,%r12)
	je	.L374
	movq	(%rbx), %r8
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%r8, 8(%rsp)
	call	calloc
	movq	8(%rsp), %r9
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 8(%r9,%r12)
	movq	(%rbx), %r10
	movq	%r10, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rsi
	movq	%rbp, %rdi
	movq	%rax, 16(%rsi,%r12)
	movq	(%rbx), %r11
	movl	$8, %esi
	movq	%r11, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rcx
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 24(%rcx,%r12)
	movq	(%rbx), %rax
	movq	%rax, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdx
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 32(%rdx,%r12)
	movq	(%rbx), %r8
	movq	%r8, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdi
	movl	$8, %esi
	movq	%rax, 40(%rdi,%r12)
	movq	(%rbx), %r9
	movq	%rbp, %rdi
	movq	%r9, 8(%rsp)
	call	calloc
	movq	8(%rsp), %r10
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 48(%r10,%r12)
	movq	(%rbx), %r11
	movq	%r11, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rsi
	movq	(%rsp), %r9
	movq	16(%rsp), %r8
	movq	%rax, 56(%rsi,%r12)
	addq	$64, %r12
	movq	(%rbx), %rdx
	jmp	.L158
.L374:
	movq	%r10, %rbx
	movq	%r11, %rbp
.L157:
	testq	%r14, %r14
	je	.L402
	movq	%rbx, %rdi
	call	fclose
	movslq	140(%rsp), %rax
	addq	%r14, %rax
	cmpl	$8, 64(%rsp)
	je	.L403
	cmpl	$24, 64(%rsp)
	je	.L404
	cmpl	$32, 64(%rsp)
	je	.L405
.L160:
	movq	32(%rsp), %rax
	movq	40(%rsp), %rbx
	movq	%r13, %rsi
	movq	%r14, %rdi
	movl	%r15d, (%rax)
	movl	52(%rsp), %r15d
	movl	%r15d, (%rbx)
	call	munmap
	movq	40(%rsp), %r13
	movq	32(%rsp), %rdx
	movl	0(%r13), %r14d
	movl	%r14d, 52(%rsp)
	movl	%r14d, %ecx
.L193:
	movq	24(%rsp), %rdi
	movl	(%rdx), %edx
	movq	(%rdi), %rsi
	movq	%rbp, %rdi
	call	insert_cache
	xorl	%eax, %eax
	jmp	.L368
.L403:
	movl	52(%rsp), %r8d
	testl	%r8d, %r8d
	jle	.L160
	movl	52(%rsp), %r11d
	leal	-1(%r15), %r8d
	movq	24(%rsp), %r10
	xorl	%edi, %edi
	addq	$1, %r8
	subl	$1, %r11d
	leaq	8(,%r11,8), %r9
.L161:
	testl	%r15d, %r15d
	jle	.L163
	movzbl	(%rax), %esi
	movq	(%r10), %rcx
	leal	-1(%r15), %r12d
	andl	$3, %r12d
	movq	(%rcx,%rdi), %rbx
	imulq	$65793, %rsi, %rdx
	cmpl	$1, %r15d
	movb	%sil, 80(%rsp)
	movq	%rdx, (%rbx)
	movl	$1, %edx
	jle	.L371
	testl	%r12d, %r12d
	je	.L164
	cmpl	$1, %r12d
	je	.L315
	cmpl	$2, %r12d
	je	.L316
	movzbl	1(%rax), %r11d
	movl	$2, %edx
	imulq	$65793, %r11, %rcx
	movb	%r11b, 80(%rsp)
	movq	%rcx, 8(%rbx)
.L316:
	movzbl	(%rax,%rdx), %r12d
	imulq	$65793, %r12, %rsi
	movb	%r12b, 80(%rsp)
	movq	%rsi, (%rbx,%rdx,8)
	addq	$1, %rdx
.L315:
	movzbl	(%rax,%rdx), %r11d
	imulq	$65793, %r11, %rcx
	movb	%r11b, 80(%rsp)
	movq	%rcx, (%rbx,%rdx,8)
	addq	$1, %rdx
	cmpl	%edx, %r15d
	jle	.L371
.L164:
	movzbl	(%rax,%rdx), %r12d
	imulq	$65793, %r12, %rsi
	movb	%r12b, 80(%rsp)
	movq	%rsi, (%rbx,%rdx,8)
	movzbl	1(%rax,%rdx), %r11d
	imulq	$65793, %r11, %rcx
	movb	%r11b, 80(%rsp)
	movq	%rcx, 8(%rbx,%rdx,8)
	movzbl	2(%rax,%rdx), %r12d
	imulq	$65793, %r12, %rsi
	movb	%r12b, 80(%rsp)
	movq	%rsi, 16(%rbx,%rdx,8)
	movzbl	3(%rax,%rdx), %r11d
	imulq	$65793, %r11, %rcx
	movb	%r11b, 80(%rsp)
	movq	%rcx, 24(%rbx,%rdx,8)
	addq	$4, %rdx
	cmpl	%edx, %r15d
	jg	.L164
.L371:
	addq	%r8, %rax
.L163:
	addq	$8, %rdi
	cmpq	%r9, %rdi
	jne	.L161
	jmp	.L160
.L402:
	cmpl	$8, 64(%rsp)
	je	.L406
	cmpl	$24, 64(%rsp)
	je	.L407
	cmpl	$32, 64(%rsp)
	je	.L408
.L176:
	movq	%rbx, %rdi
	call	fclose
	movq	32(%rsp), %rdx
	movl	52(%rsp), %ecx
	movq	40(%rsp), %rbx
	movl	%r15d, (%rdx)
	movl	%ecx, (%rbx)
	jmp	.L193
.L388:
	movq	%rbx, %r9
	movq	%rbp, %r8
	movq	24(%rsp), %rbx
	movq	56(%rsp), %rbp
	jmp	.L158
.L405:
	movl	52(%rsp), %r12d
	testl	%r12d, %r12d
	jle	.L160
	movl	52(%rsp), %edx
	leal	-1(%r15), %ecx
	movzbl	80(%rsp), %edi
	movzbl	112(%rsp), %esi
	movzbl	96(%rsp), %r12d
	xorl	%ebx, %ebx
	leaq	4(,%rcx,4), %r10
	subl	$1, %edx
	leaq	8(,%rdx,8), %r8
	movq	%r10, 56(%rsp)
	movq	%r8, 64(%rsp)
.L170:
	testl	%r15d, %r15d
	jle	.L173
	movq	24(%rsp), %rsi
	leaq	4(%rax), %r9
	movzbl	2(%rax), %r12d
	leal	-1(%r15), %r8d
	movq	%r9, %rdx
	movq	(%rsi), %rdi
	movzbl	1(%rax), %esi
	andl	$3, %r8d
	movzbl	%r12b, %r10d
	sall	$8, %r10d
	movq	(%rdi,%rbx), %rcx
	movzbl	-1(%r9), %edi
	movzbl	%dil, %r11d
	sall	$16, %r11d
	addl	%r10d, %r11d
	movzbl	%sil, %r10d
	addl	%r11d, %r10d
	cmpl	$1, %r15d
	movslq	%r10d, %r11
	movl	$1, %r10d
	movq	%r11, (%rcx)
	jle	.L372
	testl	%r8d, %r8d
	je	.L174
	cmpl	$1, %r8d
	je	.L319
	cmpl	$2, %r8d
	je	.L320
	movzbl	7(%rax), %r12d
	movzbl	2(%r9), %esi
	leaq	8(%rax), %rdx
	movzbl	1(%r9), %edi
	movl	$2, %r10d
	sall	$16, %r12d
	sall	$8, %esi
	addl	%r12d, %esi
	addl	%edi, %esi
	movslq	%esi, %r8
	movq	%r8, 8(%rcx)
.L320:
	movzbl	2(%rdx), %r11d
	movzbl	3(%rdx), %r12d
	movzbl	1(%rdx), %r9d
	addq	$4, %rdx
	sall	$16, %r12d
	sall	$8, %r11d
	addl	%r12d, %r11d
	addl	%r9d, %r11d
	movslq	%r11d, %rsi
	movq	%rsi, (%rcx,%r10,8)
	addq	$1, %r10
.L319:
	movzbl	1(%rdx), %esi
	movzbl	2(%rdx), %r12d
	addq	$4, %rdx
	movzbl	-1(%rdx), %edi
	movzbl	%r12b, %r8d
	movzbl	%sil, %r11d
	movzbl	%dil, %r9d
	sall	$8, %r8d
	sall	$16, %r9d
	addl	%r9d, %r8d
	addl	%r11d, %r8d
	movslq	%r8d, %r9
	movq	%r9, (%rcx,%r10,8)
	addq	$1, %r10
	cmpl	%r10d, %r15d
	jle	.L372
.L174:
	movzbl	3(%rdx), %r12d
	movzbl	2(%rdx), %esi
	movzbl	1(%rdx), %edi
	sall	$16, %r12d
	sall	$8, %esi
	addl	%r12d, %esi
	addl	%edi, %esi
	movslq	%esi, %r8
	movq	%r8, (%rcx,%r10,8)
	movzbl	7(%rdx), %r11d
	movzbl	6(%rdx), %r9d
	movzbl	5(%rdx), %r12d
	sall	$16, %r11d
	sall	$8, %r9d
	addl	%r11d, %r9d
	addl	%r9d, %r12d
	movslq	%r12d, %rsi
	movq	%rsi, 8(%rcx,%r10,8)
	movzbl	11(%rdx), %edi
	movzbl	10(%rdx), %r8d
	movzbl	9(%rdx), %r11d
	sall	$16, %edi
	sall	$8, %r8d
	addl	%edi, %r8d
	addl	%r8d, %r11d
	movslq	%r11d, %r9
	movq	%r9, 16(%rcx,%r10,8)
	movzbl	13(%rdx), %esi
	movzbl	14(%rdx), %r12d
	addq	$16, %rdx
	movzbl	-1(%rdx), %edi
	movzbl	%sil, %r9d
	movzbl	%r12b, %r8d
	movzbl	%dil, %r11d
	sall	$8, %r8d
	sall	$16, %r11d
	addl	%r11d, %r8d
	addl	%r9d, %r8d
	movslq	%r8d, %r11
	movq	%r11, 24(%rcx,%r10,8)
	addq	$4, %r10
	cmpl	%r10d, %r15d
	jg	.L174
.L372:
	addq	56(%rsp), %rax
.L173:
	addq	$8, %rbx
	cmpq	64(%rsp), %rbx
	jne	.L170
	movb	%dil, 80(%rsp)
	movb	%sil, 112(%rsp)
	movb	%r12b, 96(%rsp)
	jmp	.L160
.L404:
	movl	52(%rsp), %ecx
	testl	%ecx, %ecx
	jle	.L160
	movl	52(%rsp), %edx
	leal	-1(%r15), %r10d
	xorl	%ecx, %ecx
	leaq	3(%r10,%r10,2), %rbx
	subl	$1, %edx
	leaq	8(,%rdx,8), %r12
.L166:
	testl	%r15d, %r15d
	jle	.L168
	movq	24(%rsp), %rsi
	movzbl	(%rax), %r11d
	leaq	-3(%rbx), %r9
	leaq	(%rax,%rbx), %r8
	leaq	(%r9,%r9,2), %rdx
	movq	(%rsi), %rdi
	andl	$3, %edx
	movq	(%rdi,%rcx), %rsi
	movb	%r11b, 112(%rsp)
	leaq	3(%rax), %rdi
	movzbl	1(%rax), %r9d
	movq	%rdi, %rax
	movb	%r9b, 96(%rsp)
	movzbl	-1(%rdi), %r10d
	sall	$8, %r9d
	movb	%r10b, 80(%rsp)
	sall	$16, %r10d
	addl	%r10d, %r9d
	addl	%r11d, %r9d
	cmpq	%rdi, %r8
	movslq	%r9d, %r11
	movq	%r11, (%rsi)
	leaq	8(%rsi), %r11
	je	.L168
	testq	%rdx, %rdx
	je	.L169
	cmpq	$1, %rdx
	je	.L317
	cmpq	$2, %rdx
	je	.L318
	movzbl	(%rdi), %r9d
	leaq	16(%rsi), %r11
	addq	$3, %rax
	movb	%r9b, 112(%rsp)
	movzbl	1(%rdi), %edx
	movb	%dl, 96(%rsp)
	movzbl	2(%rdi), %edi
	sall	$8, %edx
	movb	%dil, 80(%rsp)
	sall	$16, %edi
	addl	%edi, %edx
	addl	%r9d, %edx
	movslq	%edx, %r10
	movq	%r10, 8(%rsi)
.L318:
	movzbl	(%rax), %r9d
	movb	%r9b, 112(%rsp)
	movzbl	1(%rax), %edx
	movb	%dl, 96(%rsp)
	movzbl	2(%rax), %esi
	sall	$8, %edx
	addq	$3, %rax
	movb	%sil, 80(%rsp)
	sall	$16, %esi
	addl	%esi, %edx
	addl	%r9d, %edx
	movslq	%edx, %rdi
	movq	%rdi, (%r11)
	addq	$8, %r11
.L317:
	movzbl	(%rax), %r10d
	movb	%r10b, 112(%rsp)
	movzbl	1(%rax), %r9d
	addq	$3, %rax
	movb	%r9b, 96(%rsp)
	movzbl	-1(%rax), %edx
	sall	$8, %r9d
	movb	%dl, 80(%rsp)
	sall	$16, %edx
	addl	%edx, %r9d
	addl	%r10d, %r9d
	movslq	%r9d, %rsi
	movq	%rsi, (%r11)
	addq	$8, %r11
	cmpq	%rax, %r8
	je	.L168
.L169:
	movzbl	(%rax), %edi
	movb	%dil, 112(%rsp)
	movzbl	1(%rax), %r10d
	movb	%r10b, 96(%rsp)
	movzbl	2(%rax), %r9d
	sall	$8, %r10d
	movb	%r9b, 80(%rsp)
	sall	$16, %r9d
	addl	%r9d, %r10d
	addl	%edi, %r10d
	movslq	%r10d, %rdx
	movq	%rdx, (%r11)
	movzbl	3(%rax), %edi
	movb	%dil, 112(%rsp)
	movzbl	4(%rax), %r10d
	movb	%r10b, 96(%rsp)
	movzbl	5(%rax), %esi
	sall	$8, %r10d
	movb	%sil, 80(%rsp)
	sall	$16, %esi
	addl	%esi, %r10d
	addl	%edi, %r10d
	movslq	%r10d, %r9
	movq	%r9, 8(%r11)
	movzbl	6(%rax), %edi
	movb	%dil, 112(%rsp)
	movzbl	7(%rax), %edx
	movb	%dl, 96(%rsp)
	movzbl	8(%rax), %r10d
	sall	$8, %edx
	movb	%r10b, 80(%rsp)
	sall	$16, %r10d
	addl	%r10d, %edx
	addl	%edi, %edx
	movslq	%edx, %rsi
	movq	%rsi, 16(%r11)
	movzbl	9(%rax), %r9d
	movb	%r9b, 112(%rsp)
	movzbl	10(%rax), %edi
	addq	$12, %rax
	movb	%dil, 96(%rsp)
	movzbl	-1(%rax), %edx
	sall	$8, %edi
	movb	%dl, 80(%rsp)
	sall	$16, %edx
	addl	%edx, %edi
	addl	%r9d, %edi
	movslq	%edi, %r10
	movq	%r10, 24(%r11)
	addq	$32, %r11
	cmpq	%rax, %r8
	jne	.L169
.L168:
	addq	$8, %rcx
	cmpq	%r12, %rcx
	jne	.L166
	jmp	.L160
.L408:
	movl	52(%rsp), %r14d
	testl	%r14d, %r14d
	jle	.L176
	movl	52(%rsp), %r13d
	leal	-1(%r15), %ecx
	movq	%rbp, 64(%rsp)
	movq	24(%rsp), %rbp
	subl	$1, %r13d
	leaq	8(,%r13,8), %r12
	leaq	8(,%rcx,8), %r13
	movq	%r12, 56(%rsp)
	xorl	%r12d, %r12d
.L186:
	testl	%r15d, %r15d
	jle	.L188
	leaq	-8(%r13), %r10
	leaq	80(%rsp), %r9
	leaq	96(%rsp), %r8
	leaq	127(%rsp), %rdx
	leaq	112(%rsp), %rcx
	movl	$.LC10, %esi
	shrq	$3, %r10
	xorl	%eax, %eax
	movq	%rbx, %rdi
	andl	$1, %r10d
	movl	$8, %r14d
	movq	%r10, 16(%rsp)
	call	fscanf
	movq	0(%rbp), %rax
	movzbl	80(%rsp), %edx
	movzbl	112(%rsp), %r8d
	movq	16(%rsp), %rsi
	movq	(%rax,%r12), %r9
	movzbl	96(%rsp), %eax
	sall	$16, %edx
	sall	$8, %eax
	addl	%edx, %eax
	addl	%r8d, %eax
	cmpq	%r13, %r14
	cltq
	movq	%rax, (%r9)
	je	.L188
	testq	%rsi, %rsi
	je	.L189
	leaq	112(%rsp), %rcx
	leaq	80(%rsp), %r9
	leaq	96(%rsp), %r8
	leaq	127(%rsp), %rdx
	movq	%rbx, %rdi
	xorl	%eax, %eax
	movl	$.LC10, %esi
	call	fscanf
	movzbl	80(%rsp), %r14d
	movzbl	96(%rsp), %eax
	movq	0(%rbp), %rdi
	movzbl	112(%rsp), %ecx
	sall	$16, %r14d
	sall	$8, %eax
	movq	(%rdi,%r12), %r11
	addl	%r14d, %eax
	movl	$16, %r14d
	addl	%ecx, %eax
	cmpq	%r13, %r14
	cltq
	movq	%rax, 8(%r11)
	je	.L188
.L189:
	leaq	80(%rsp), %r9
	leaq	96(%rsp), %r8
	leaq	112(%rsp), %rcx
	leaq	127(%rsp), %rdx
	movl	$.LC10, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	80(%rsp), %edx
	movzbl	96(%rsp), %eax
	leaq	112(%rsp), %rcx
	movq	0(%rbp), %r10
	movzbl	112(%rsp), %r8d
	movl	$.LC10, %esi
	movq	%rbx, %rdi
	sall	$16, %edx
	sall	$8, %eax
	movq	(%r10,%r12), %r9
	addl	%edx, %eax
	leaq	127(%rsp), %rdx
	addl	%r8d, %eax
	leaq	96(%rsp), %r8
	cltq
	movq	%rax, (%r9,%r14)
	leaq	80(%rsp), %r9
	xorl	%eax, %eax
	call	fscanf
	movzbl	80(%rsp), %r11d
	movzbl	96(%rsp), %ecx
	movq	0(%rbp), %rsi
	movzbl	112(%rsp), %r10d
	sall	$16, %r11d
	sall	$8, %ecx
	movq	(%rsi,%r12), %rdi
	addl	%r11d, %ecx
	addl	%r10d, %ecx
	movslq	%ecx, %r9
	movq	%r9, 8(%rdi,%r14)
	addq	$16, %r14
	cmpq	%r13, %r14
	jne	.L189
.L188:
	addq	$8, %r12
	cmpq	56(%rsp), %r12
	jne	.L186
.L379:
	movq	64(%rsp), %rbp
	jmp	.L176
.L407:
	movl	52(%rsp), %r14d
	testl	%r14d, %r14d
	jle	.L176
	movl	52(%rsp), %r12d
	leal	-1(%r15), %edx
	movq	%rbp, 64(%rsp)
	movq	24(%rsp), %rbp
	subl	$1, %r12d
	leaq	8(,%r12,8), %r13
	xorl	%r12d, %r12d
	movq	%r13, 56(%rsp)
	leaq	8(,%rdx,8), %r13
.L182:
	testl	%r15d, %r15d
	jle	.L184
	leaq	-8(%r13), %r11
	leaq	80(%rsp), %r8
	leaq	96(%rsp), %rcx
	leaq	112(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbx, %rdi
	shrq	$3, %r11
	xorl	%eax, %eax
	movl	$8, %r14d
	andl	$1, %r11d
	movq	%r11, 16(%rsp)
	call	fscanf
	movq	0(%rbp), %rax
	movzbl	80(%rsp), %esi
	movzbl	112(%rsp), %edi
	movq	16(%rsp), %rcx
	movq	(%rax,%r12), %r8
	movzbl	96(%rsp), %eax
	sall	$16, %esi
	sall	$8, %eax
	addl	%esi, %eax
	addl	%edi, %eax
	cmpq	%r13, %r14
	cltq
	movq	%rax, (%r8)
	je	.L184
	testq	%rcx, %rcx
	je	.L185
	leaq	112(%rsp), %rdx
	leaq	80(%rsp), %r8
	leaq	96(%rsp), %rcx
	xorl	%eax, %eax
	movl	$.LC9, %esi
	movq	%rbx, %rdi
	call	fscanf
	movzbl	80(%rsp), %r14d
	movzbl	96(%rsp), %eax
	movq	0(%rbp), %r10
	movzbl	112(%rsp), %edx
	sall	$16, %r14d
	sall	$8, %eax
	movq	(%r10,%r12), %r9
	addl	%r14d, %eax
	movl	$16, %r14d
	addl	%edx, %eax
	cmpq	%r13, %r14
	cltq
	movq	%rax, 8(%r9)
	je	.L184
.L185:
	leaq	80(%rsp), %r8
	leaq	96(%rsp), %rcx
	leaq	112(%rsp), %rdx
	movl	$.LC9, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	80(%rsp), %eax
	movzbl	96(%rsp), %esi
	leaq	96(%rsp), %rcx
	movq	0(%rbp), %r11
	movzbl	112(%rsp), %edi
	leaq	112(%rsp), %rdx
	sall	$8, %esi
	sall	$16, %eax
	movq	(%r11,%r12), %r8
	addl	%esi, %eax
	movl	$.LC9, %esi
	addl	%edi, %eax
	movq	%rbx, %rdi
	cltq
	movq	%rax, (%r8,%r14)
	leaq	80(%rsp), %r8
	xorl	%eax, %eax
	call	fscanf
	movzbl	80(%rsp), %r9d
	movzbl	96(%rsp), %edx
	movq	0(%rbp), %rcx
	movzbl	112(%rsp), %r11d
	sall	$16, %r9d
	sall	$8, %edx
	movq	(%rcx,%r12), %r10
	addl	%r9d, %edx
	addl	%r11d, %edx
	movslq	%edx, %r8
	movq	%r8, 8(%r10,%r14)
	addq	$16, %r14
	cmpq	%r13, %r14
	jne	.L185
.L184:
	addq	$8, %r12
	cmpq	56(%rsp), %r12
	jne	.L182
	jmp	.L379
.L406:
	movl	52(%rsp), %r14d
	testl	%r14d, %r14d
	jle	.L176
	movl	52(%rsp), %r12d
	leal	-1(%r15), %eax
	movq	24(%rsp), %r14
	movq	%rbp, 64(%rsp)
	subl	$1, %r12d
	leaq	8(,%r12,8), %r13
	xorl	%r12d, %r12d
	movq	%r13, 56(%rsp)
	leaq	8(,%rax,8), %r13
.L177:
	testl	%r15d, %r15d
	jle	.L179
	leaq	-8(%r13), %rbp
	leaq	80(%rsp), %rdx
	movl	$.LC8, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	shrq	$3, %rbp
	andl	$3, %ebp
	movq	%rbp, 16(%rsp)
	movl	$8, %ebp
	call	fscanf
	movzbl	80(%rsp), %ecx
	movq	(%r14), %rsi
	movq	16(%rsp), %r9
	movq	(%rsi,%r12), %rdi
	imulq	$65793, %rcx, %r10
	cmpq	%r13, %rbp
	movq	%r10, (%rdi)
	je	.L179
	testq	%r9, %r9
	je	.L180
	cmpq	$1, %r9
	je	.L321
	cmpq	$2, %r9
	je	.L322
	leaq	80(%rsp), %rdx
	movl	$.LC8, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	movl	$16, %ebp
	call	fscanf
	movzbl	80(%rsp), %r8d
	movq	(%r14), %rdx
	movq	(%rdx,%r12), %r11
	imulq	$65793, %r8, %rax
	movq	%rax, 8(%r11)
.L322:
	leaq	80(%rsp), %rdx
	movl	$.LC8, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	80(%rsp), %ecx
	movq	(%r14), %rsi
	movq	(%rsi,%r12), %rdi
	imulq	$65793, %rcx, %r10
	movq	%r10, (%rdi,%rbp)
	addq	$8, %rbp
.L321:
	leaq	80(%rsp), %rdx
	xorl	%eax, %eax
	movl	$.LC8, %esi
	movq	%rbx, %rdi
	call	fscanf
	movzbl	80(%rsp), %edx
	movq	(%r14), %r9
	movq	(%r9,%r12), %r11
	imulq	$65793, %rdx, %r8
	movq	%r8, (%r11,%rbp)
	addq	$8, %rbp
	cmpq	%r13, %rbp
	je	.L179
.L180:
	leaq	80(%rsp), %rdx
	movl	$.LC8, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	call	fscanf
	movzbl	80(%rsp), %edi
	movq	(%r14), %rax
	leaq	80(%rsp), %rdx
	movq	(%rax,%r12), %rsi
	xorl	%eax, %eax
	imulq	$65793, %rdi, %rcx
	movq	%rbx, %rdi
	movq	%rcx, (%rsi,%rbp)
	movl	$.LC8, %esi
	call	fscanf
	movzbl	80(%rsp), %r11d
	movq	(%r14), %r10
	movl	$.LC8, %esi
	movq	%rbx, %rdi
	xorl	%eax, %eax
	movq	(%r10,%r12), %r9
	imulq	$65793, %r11, %rdx
	movq	%rdx, 8(%r9,%rbp)
	leaq	80(%rsp), %rdx
	call	fscanf
	movzbl	80(%rsp), %esi
	movq	(%r14), %r8
	leaq	80(%rsp), %rdx
	movq	(%r8,%r12), %rax
	imulq	$65793, %rsi, %rdi
	movl	$.LC8, %esi
	movq	%rdi, 16(%rax,%rbp)
	xorl	%eax, %eax
	movq	%rbx, %rdi
	call	fscanf
	movzbl	80(%rsp), %r9d
	movq	(%r14), %rcx
	movq	(%rcx,%r12), %r10
	imulq	$65793, %r9, %r11
	movq	%r11, 24(%r10,%rbp)
	addq	$32, %rbp
	cmpq	%r13, %rbp
	jne	.L180
.L179:
	addq	$8, %r12
	cmpq	56(%rsp), %r12
	jne	.L177
	jmp	.L379
	.cfi_endproc
.LFE58:
	.size	load_bmp_file, .-load_bmp_file
	.section	.rodata.str1.8
	.align 8
.LC13:
	.string	"pngio: file %s could not be opened for reading"
	.align 8
.LC14:
	.string	"pngio: File %s is not recognized as a PNG file"
	.section	.rodata.str1.1
.LC15:
	.string	"1.5.12"
	.section	.rodata.str1.8
	.align 8
.LC16:
	.string	"pngio: png_create_read_struct failed"
	.align 8
.LC17:
	.string	"pngio: png_create_info_struct failed"
	.section	.rodata.str1.1
.LC18:
	.string	"pngio: error during init_io"
	.section	.rodata.str1.8
	.align 8
.LC19:
	.string	"pngio: error during read_image"
	.align 8
.LC20:
	.string	"pngio: unsupported color type: %d\n"
	.align 8
.LC21:
	.string	"load_png: warning: mmap file failed, using normal buffered I/O: %s\n"
	.text
	.p2align 4,,15
	.globl	load_png_file
	.type	load_png_file, @function
load_png_file:
.LFB59:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$248, %rsp
	.cfi_def_cfa_offset 304
	movl	ncache(%rip), %eax
	movq	%rdi, 32(%rsp)
	movq	%rsi, 40(%rsp)
	movq	%rdx, 48(%rsp)
	movq	%rcx, 16(%rsp)
	testl	%eax, %eax
	jle	.L410
	movq	icache(%rip), %rbp
	subl	$1, %eax
	movq	%rcx, %rdi
	leaq	(%rax,%rax,2), %rdx
	xorl	%r12d, %r12d
	leaq	24(%rbp), %r15
	movq	0(%rbp), %rsi
	movq	%rbp, %rbx
	leaq	(%r15,%rdx,8), %r14
	call	strcmp
	movq	%r14, %r13
	subq	%rbp, %r13
	subq	$24, %r13
	shrq	$3, %r13
	leaq	0(%r13,%r13,2), %r13
	andl	$7, %r13d
	testl	%eax, %eax
	je	.L570
	cmpq	%r14, %r15
	movq	%r15, %rbx
	je	.L410
	testq	%r13, %r13
	je	.L582
	cmpq	$1, %r13
	je	.L564
	cmpq	$2, %r13
	je	.L565
	cmpq	$3, %r13
	je	.L566
	cmpq	$4, %r13
	je	.L567
	cmpq	$5, %r13
	je	.L568
	cmpq	$6, %r13
	je	.L569
	movq	(%r15), %rsi
	movq	16(%rsp), %rdi
	movq	%r15, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
.L569:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
.L568:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
.L567:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
.L566:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
.L565:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
.L564:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
	cmpq	%r14, %rbx
	je	.L410
.L582:
	movq	16(%rsp), %r15
.L413:
	movq	(%rbx), %rsi
	movq	%rbx, %r12
	movq	%r15, %rdi
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	leaq	24(%rbx), %r13
	movq	%r15, %rdi
	movq	0(%r13), %rsi
	movq	%r13, %r12
	movq	%r13, %rbx
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	addq	$24, %rbx
	movq	%r15, %rdi
	movq	(%rbx), %rsi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	leaq	48(%r13), %rbx
	movq	48(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	leaq	72(%r13), %rbx
	movq	72(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	leaq	96(%r13), %rbx
	movq	96(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	leaq	120(%r13), %rbx
	movq	120(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	leaq	144(%r13), %rbx
	movq	144(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L570
	leaq	168(%r13), %rbx
	cmpq	%r14, %rbx
	jne	.L413
.L410:
	movq	16(%rsp), %rdi
	movl	$.LC11, %esi
	call	fopen
	testq	%rax, %rax
	movq	%rax, 24(%rsp)
	je	.L591
	movl	__isthreaded(%rip), %ecx
	testl	%ecx, %ecx
	jne	.L414
	movq	24(%rsp), %rsi
	movswl	18(%rsi), %ebp
.L415:
	leaq	112(%rsp), %rsi
	movl	%ebp, %edi
	call	fstat
	testl	%eax, %eax
	js	.L592
	movq	184(%rsp), %rsi
	xorl	%edi, %edi
	xorl	%r9d, %r9d
	movl	%ebp, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	call	mmap
	movq	184(%rsp), %rdi
	cmpq	$-1, %rax
	movq	%rax, 56(%rsp)
	movq	%rdi, 64(%rsp)
	je	.L593
.L440:
	movq	24(%rsp), %rcx
	leaq	112(%rsp), %rdi
	movl	$8, %edx
	movl	$1, %esi
	call	fread
	leaq	112(%rsp), %rdi
	xorl	%esi, %esi
	movl	$8, %edx
	call	png_sig_cmp
	testl	%eax, %eax
	je	.L419
	movq	16(%rsp), %rsi
	xorl	%eax, %eax
	movl	$.LC14, %edi
	call	printf
	cmpq	$0, 56(%rsp)
	je	.L420
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L420:
	movq	24(%rsp), %rdi
	call	fclose
	movl	$2, %eax
.L571:
	addq	$248, %rsp
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
	.p2align 4,,10
.L570:
	.cfi_restore_state
	movq	8(%rbx), %rbp
	movq	32(%rsp), %r15
	xorl	%eax, %eax
	movq	40(%rsp), %r14
	movq	48(%rsp), %rbx
	movq	%rbp, (%r15)
	addq	icache(%rip), %r12
	movl	16(%r12), %r13d
	movl	%r13d, (%r14)
	movl	20(%r12), %r12d
	movl	%r12d, (%rbx)
	jmp	.L571
	.p2align 4,,10
.L419:
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	$.LC15, %edi
	call	png_create_read_struct
	testq	%rax, %rax
	movq	%rax, 72(%rsp)
	je	.L594
	movq	%rax, %rdi
	call	png_create_info_struct
	testq	%rax, %rax
	movq	%rax, 80(%rsp)
	je	.L595
	movq	72(%rsp), %rdi
	movl	$96, %edx
	movl	$longjmp, %esi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	setjmp
	testl	%eax, %eax
	je	.L425
	xorl	%eax, %eax
	movl	$.LC18, %edi
	call	printf
	cmpq	$0, 56(%rsp)
	je	.L426
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L426:
	movq	24(%rsp), %rdi
	call	fclose
	movl	$5, %eax
	jmp	.L571
	.p2align 4,,10
.L414:
	movq	24(%rsp), %rdi
	call	fileno
	movl	%eax, %ebp
	jmp	.L415
	.p2align 4,,10
.L592:
	movl	$.LC0, %edi
	call	perror
.L417:
	movq	16(%rsp), %rsi
	movl	$.LC21, %edi
	xorl	%eax, %eax
	call	printf
	movq	$0, 56(%rsp)
	movq	$0, 64(%rsp)
	jmp	.L440
	.p2align 4,,10
.L591:
	movq	16(%rsp), %rsi
	movl	$.LC13, %edi
	xorl	%eax, %eax
	call	printf
	movl	$1, %eax
	jmp	.L571
	.p2align 4,,10
.L425:
	movq	24(%rsp), %rsi
	movq	72(%rsp), %rdi
	call	png_init_io
	movq	72(%rsp), %rdi
	movl	$8, %esi
	call	png_set_sig_bytes
	movq	80(%rsp), %rsi
	movq	72(%rsp), %rdi
	call	png_read_info
	movq	80(%rsp), %rsi
	movq	72(%rsp), %rdi
	call	png_get_image_width
	movq	80(%rsp), %rsi
	movq	72(%rsp), %rdi
	movl	%eax, 104(%rsp)
	call	png_get_image_height
	movq	80(%rsp), %rsi
	movq	72(%rsp), %rdi
	movl	%eax, 92(%rsp)
	call	png_get_color_type
	movq	80(%rsp), %rsi
	movq	72(%rsp), %rdi
	movb	%al, 103(%rsp)
	call	png_read_update_info
	movq	72(%rsp), %rdi
	movl	$96, %edx
	movl	$longjmp, %esi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	setjmp
	testl	%eax, %eax
	jne	.L596
	cmpb	$2, 103(%rsp)
	je	.L444
	cmpb	$6, 103(%rsp)
	je	.L445
	cmpb	$0, 103(%rsp)
	movl	$1, %r13d
	jne	.L597
.L429:
	movl	92(%rsp), %r8d
	movslq	%r8d, %r15
	movl	%r8d, 96(%rsp)
	leaq	0(,%r15,8), %rdi
	call	malloc
	movl	92(%rsp), %r9d
	movq	%rax, %rbx
	testl	%r9d, %r9d
	jle	.L431
	movslq	104(%rsp), %rcx
	movl	92(%rsp), %ebp
	movslq	%r13d, %r12
	subl	$1, %ebp
	imulq	%rcx, %r12
	leaq	8(,%rbp,8), %r14
	andl	$7, %ebp
	movq	%rbp, 8(%rsp)
	movl	$8, %ebp
	movq	%r12, %rdi
	call	malloc
	cmpq	%r14, %rbp
	movq	%rax, (%rbx)
	movq	8(%rsp), %rsi
	je	.L432
	testq	%rsi, %rsi
	je	.L433
	cmpq	$1, %rsi
	je	.L558
	cmpq	$2, %rsi
	je	.L559
	cmpq	$3, %rsi
	je	.L560
	cmpq	$4, %rsi
	je	.L561
	cmpq	$5, %rsi
	je	.L562
	cmpq	$6, %rsi
	je	.L563
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx,%rbp)
	movl	$16, %ebp
.L563:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx,%rbp)
	addq	$8, %rbp
.L562:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx,%rbp)
	addq	$8, %rbp
.L561:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx,%rbp)
	addq	$8, %rbp
.L560:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx,%rbp)
	addq	$8, %rbp
.L559:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx,%rbp)
	addq	$8, %rbp
.L558:
	movq	%r12, %rdi
	call	malloc
	movq	%rax, (%rbx,%rbp)
	addq	$8, %rbp
	cmpq	%r14, %rbp
	je	.L432
.L433:
	movq	%r12, %rdi
	call	malloc
	movq	%r12, %rdi
	movq	%rax, (%rbx,%rbp)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, 8(%rbx,%rbp)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, 16(%rbx,%rbp)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, 24(%rbx,%rbp)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, 32(%rbx,%rbp)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, 40(%rbx,%rbp)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, 48(%rbx,%rbp)
	call	malloc
	movq	%rax, 56(%rbx,%rbp)
	addq	$64, %rbp
	cmpq	%r14, %rbp
	jne	.L433
.L432:
	movq	72(%rsp), %rdi
	movq	%rbx, %rsi
	xorl	%ebp, %ebp
	call	png_read_image
	movq	24(%rsp), %rdi
	call	fclose
	movl	104(%rsp), %edi
	movq	40(%rsp), %r8
	movl	$8, %esi
	movl	92(%rsp), %r9d
	movq	48(%rsp), %r10
	movl	%edi, (%r8)
	movq	%r15, %rdi
	movl	%r9d, (%r10)
	call	calloc
	movq	32(%rsp), %r11
	movq	%rax, %r15
	leaq	-8(%r14), %rcx
	shrq	$3, %rcx
	movq	%rax, (%r11)
	movq	40(%rsp), %rax
	andl	$7, %ecx
	movl	(%rax), %edx
	movl	%edx, 108(%rsp)
	movslq	%edx, %r12
	je	.L578
	movl	$8, %esi
	movq	%r12, %rdi
	movq	%rcx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdi
	movq	32(%rsp), %rsi
	movq	%rax, (%r15,%rbp)
	movl	$8, %ebp
	cmpq	$1, %rdi
	movq	(%rsi), %r15
	je	.L578
	cmpq	$2, %rdi
	je	.L553
	cmpq	$3, %rdi
	je	.L554
	cmpq	$4, %rdi
	je	.L555
	cmpq	$5, %rdi
	je	.L556
	cmpq	$6, %rdi
	je	.L557
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%r15,%rbp)
	movq	32(%rsp), %r15
	movb	$16, %bpl
	movq	(%r15), %r15
.L557:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	32(%rsp), %r8
	movq	%rax, (%r15,%rbp)
	addq	$8, %rbp
	movq	(%r8), %r15
.L556:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	32(%rsp), %r9
	movq	%rax, (%r15,%rbp)
	addq	$8, %rbp
	movq	(%r9), %r15
.L555:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	32(%rsp), %r10
	movq	%rax, (%r15,%rbp)
	addq	$8, %rbp
	movq	(%r10), %r15
.L554:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	32(%rsp), %r11
	movq	%rax, (%r15,%rbp)
	addq	$8, %rbp
	movq	(%r11), %r15
.L553:
	movl	$8, %esi
	movq	%r12, %rdi
	call	calloc
	movq	%rax, (%r15,%rbp)
	movq	32(%rsp), %rax
	movq	%rbx, %r15
	addq	$8, %rbp
	movq	32(%rsp), %rbx
	movq	(%rax), %rdx
	movq	%rdx, %rcx
	jmp	.L441
.L434:
	movq	(%rbx), %rcx
	movl	$8, %esi
	movq	%r12, %rdi
	movq	%rcx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rsi
	movq	%r12, %rdi
	movq	%rax, 8(%rsi,%rbp)
	movq	(%rbx), %r8
	movl	$8, %esi
	movq	%r8, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdi
	movl	$8, %esi
	movq	%rax, 16(%rdi,%rbp)
	movq	(%rbx), %r9
	movq	%r12, %rdi
	movq	%r9, 8(%rsp)
	call	calloc
	movq	8(%rsp), %r10
	movl	$8, %esi
	movq	%r12, %rdi
	movq	%rax, 24(%r10,%rbp)
	movq	(%rbx), %r11
	movq	%r11, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdx
	movl	$8, %esi
	movq	%r12, %rdi
	movq	%rax, 32(%rdx,%rbp)
	movq	(%rbx), %rax
	movq	%rax, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rcx
	movl	$8, %esi
	movq	%r12, %rdi
	movq	%rax, 40(%rcx,%rbp)
	movq	(%rbx), %r8
	movq	%r8, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rsi
	movq	%r12, %rdi
	movq	%rax, 48(%rsi,%rbp)
	movq	(%rbx), %r9
	movl	$8, %esi
	movq	%r9, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rdi
	movq	%rax, 56(%rdi,%rbp)
	movq	(%rbx), %rcx
	addq	$64, %rbp
.L441:
	movl	$8, %esi
	movq	%r12, %rdi
	movq	%rcx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %rsi
	leaq	8(%rbp), %rdi
	cmpq	%rdi, %r14
	movq	%rax, (%rsi,%rbp)
	jne	.L434
	movl	%r13d, %r8d
	movq	32(%rsp), %r13
	movl	92(%rsp), %esi
	movl	108(%rsp), %edi
	movq	%r15, %rdx
	movq	%r15, %rbx
	movl	$1, %ebp
	movq	0(%r13), %rcx
	call	set_pngrows_array
	movl	96(%rsp), %r14d
	movq	(%r15), %rdi
	leal	-1(%r14), %r12d
	call	free
	andl	$7, %r12d
	cmpl	$1, 96(%rsp)
	jle	.L442
	testl	%r12d, %r12d
	je	.L443
	cmpl	$1, %r12d
	je	.L547
	cmpl	$2, %r12d
	je	.L548
	cmpl	$3, %r12d
	je	.L549
	cmpl	$4, %r12d
	je	.L550
	cmpl	$5, %r12d
	je	.L551
	cmpl	$6, %r12d
	je	.L552
	movq	(%r15,%rbp,8), %rdi
	movl	$2, %ebp
	call	free
.L552:
	movq	(%rbx,%rbp,8), %rdi
	addq	$1, %rbp
	call	free
.L551:
	movq	(%rbx,%rbp,8), %rdi
	addq	$1, %rbp
	call	free
.L550:
	movq	(%rbx,%rbp,8), %rdi
	addq	$1, %rbp
	call	free
.L549:
	movq	(%rbx,%rbp,8), %rdi
	addq	$1, %rbp
	call	free
.L548:
	movq	(%rbx,%rbp,8), %rdi
	addq	$1, %rbp
	call	free
.L547:
	movq	(%rbx,%rbp,8), %rdi
	addq	$1, %rbp
	call	free
	cmpl	%ebp, 96(%rsp)
	jle	.L442
.L443:
	movq	(%rbx,%rbp,8), %rdi
	call	free
	movq	8(%rbx,%rbp,8), %rdi
	call	free
	movq	16(%rbx,%rbp,8), %rdi
	call	free
	movq	24(%rbx,%rbp,8), %rdi
	call	free
	movq	32(%rbx,%rbp,8), %rdi
	call	free
	movq	40(%rbx,%rbp,8), %rdi
	call	free
	movq	48(%rbx,%rbp,8), %rdi
	call	free
	movq	56(%rbx,%rbp,8), %rdi
	addq	$8, %rbp
	call	free
	cmpl	%ebp, 96(%rsp)
	jg	.L443
.L442:
	movq	%rbx, %rdi
	call	free
	cmpq	$0, 56(%rsp)
	je	.L437
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L437:
	movl	ncache(%rip), %r12d
	movq	48(%rsp), %r8
	movq	40(%rsp), %r9
	movq	32(%rsp), %r10
	movq	icache(%rip), %rax
	movq	16(%rsp), %rdi
	movslq	%r12d, %r11
	movl	(%r8), %r15d
	addl	$1, %r12d
	leaq	(%r11,%r11,2), %rdx
	movl	(%r9), %r13d
	movq	(%r10), %r14
	leaq	(%rax,%rdx,8), %rbx
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	16(%rsp), %rsi
	movq	%rax, (%rbx)
	movq	%rax, %rdi
	call	strcpy
	movl	%r13d, 16(%rbx)
	movl	%r15d, 20(%rbx)
	xorl	%eax, %eax
	movq	%r14, 8(%rbx)
	movl	%r12d, ncache(%rip)
	jmp	.L571
	.p2align 4,,10
.L593:
	movl	$.LC1, %edi
	call	perror
	jmp	.L417
	.p2align 4,,10
.L594:
	movl	$.LC16, %edi
	call	printf
	cmpq	$0, 56(%rsp)
	je	.L422
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L422:
	movq	24(%rsp), %rdi
	call	fclose
	movl	$3, %eax
	jmp	.L571
	.p2align 4,,10
.L595:
	movl	$.LC17, %edi
	call	printf
	cmpq	$0, 56(%rsp)
	je	.L424
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L424:
	movq	24(%rsp), %rdi
	call	fclose
	movl	$4, %eax
	jmp	.L571
.L596:
	xorl	%eax, %eax
	movl	$.LC19, %edi
	call	printf
	cmpq	$0, 56(%rsp)
	je	.L428
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L428:
	movq	24(%rsp), %rdi
	call	fclose
	movl	$6, %eax
	jmp	.L571
.L597:
	movzbl	103(%rsp), %esi
	xorl	%eax, %eax
	movl	$.LC20, %edi
	call	printf
	cmpq	$0, 56(%rsp)
	je	.L430
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L430:
	movq	24(%rsp), %rdi
	call	fclose
	movl	$7, %eax
	jmp	.L571
.L445:
	movl	$4, %r13d
	jmp	.L429
.L578:
	movq	%r15, %rcx
	movq	%rbx, %r15
	movq	32(%rsp), %rbx
	jmp	.L441
.L444:
	movl	$3, %r13d
	jmp	.L429
.L431:
	movq	72(%rsp), %rdi
	movq	%rax, %rsi
	call	png_read_image
	movq	24(%rsp), %rdi
	call	fclose
	movq	48(%rsp), %rdx
	movl	104(%rsp), %r10d
	movq	%r15, %rdi
	movq	40(%rsp), %r11
	movl	92(%rsp), %eax
	movl	$8, %esi
	movl	%r10d, (%r11)
	movl	%eax, (%rdx)
	call	calloc
	movq	40(%rsp), %r15
	movq	32(%rsp), %r14
	movl	%r13d, %r8d
	movl	92(%rsp), %esi
	movq	%rax, %rcx
	movq	%rbx, %rdx
	movl	(%r15), %edi
	movq	%rax, (%r14)
	call	set_pngrows_array
	jmp	.L442
	.cfi_endproc
.LFE59:
	.size	load_png_file, .-load_png_file
	.section	.rodata.str1.1
.LC22:
	.string	"wb"
	.section	.rodata.str1.8
	.align 8
.LC23:
	.string	"save_bmp: error writing file: %s\n"
	.text
	.p2align 4,,15
	.globl	save_bmp_file
	.type	save_bmp_file, @function
save_bmp_file:
.LFB60:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movl	%esi, %r14d
	movl	$.LC22, %esi
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%rcx, %r13
	movl	$1, %ecx
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	movq	%r13, %rdi
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movl	%edx, %ebx
	movl	$24, %edx
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	movb	$66, 16(%rsp)
	movb	$77, 17(%rsp)
	movl	$0, 20(%rsp)
	movl	$0, 24(%rsp)
	movl	$56, 28(%rsp)
	movl	$32, 40(%rsp)
	movl	$32, 36(%rsp)
	movl	$40, 32(%rsp)
	movw	%dx, 46(%rsp)
	movw	%cx, 44(%rsp)
	movl	$0, 48(%rsp)
	movl	$3072, 52(%rsp)
	movl	$0, 56(%rsp)
	movl	$0, 60(%rsp)
	movl	$0, 64(%rsp)
	movl	$0, 68(%rsp)
	call	fopen
	testq	%rax, %rax
	movq	%rax, %r12
	je	.L623
	movq	%rax, %rdi
	movl	$77, %ecx
	movl	$66, %edx
	movl	$.LC4, %esi
	xorl	%eax, %eax
	call	fprintf
	movl	%ebx, %eax
	leaq	20(%rsp), %rdi
	movq	%r12, %rcx
	imull	%r14d, %eax
	movl	$1, %edx
	movl	%ebx, 40(%rsp)
	movl	%r14d, 36(%rsp)
	leal	(%rax,%rax,2), %r15d
	leal	56(%r15), %esi
	movl	%esi, 20(%rsp)
	movl	$4, %esi
	call	fwrite
	leaq	24(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	movl	$1, %edi
	movl	$24, %r8d
	movq	%r12, %rcx
	movw	%di, 44(%rsp)
	leaq	28(%rsp), %rdi
	movl	$1, %edx
	movl	$4, %esi
	movw	%r8w, 46(%rsp)
	movl	$56, 28(%rsp)
	movl	%r15d, 52(%rsp)
	call	fwrite
	leaq	32(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	36(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	40(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	44(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fwrite
	leaq	46(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fwrite
	leaq	48(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	52(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	56(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	60(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	64(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	leaq	68(%rsp), %rdi
	movq	%r12, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	movslq	28(%rsp), %rsi
	xorl	%edx, %edx
	movq	%r12, %rdi
	call	fseek
	testl	%ebx, %ebx
	jle	.L601
	subl	$1, %ebx
	leal	-1(%r14), %r10d
	leaq	8(%rbp,%rbx,8), %r9
	leaq	8(,%r10,8), %r13
	movq	%r9, 8(%rsp)
	.p2align 4,,10
.L602:
	testl	%r14d, %r14d
	jle	.L604
	movq	0(%rbp), %r11
	leaq	-8(%r13), %r15
	xorl	%eax, %eax
	movl	$.LC9, %esi
	movq	%r12, %rdi
	shrq	$3, %r15
	movq	(%r11), %rbx
	andl	$3, %r15d
	movq	%rbx, %r8
	movzbl	%bh, %ecx
	movzbl	%bl, %edx
	andl	$16711680, %r8d
	movl	$8, %ebx
	shrq	$16, %r8
	call	fprintf
	cmpq	%r13, %rbx
	je	.L604
	testq	%r15, %r15
	je	.L605
	cmpq	$1, %r15
	je	.L617
	cmpq	$2, %r15
	je	.L618
	movq	0(%rbp), %rdx
	movl	$.LC9, %esi
	movq	%r12, %rdi
	movl	$16, %ebx
	movq	8(%rdx), %rax
	movq	%rax, %r8
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	andl	$16711680, %r8d
	xorl	%eax, %eax
	shrq	$16, %r8
	call	fprintf
.L618:
	movq	0(%rbp), %rcx
	movl	$.LC9, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	movq	(%rcx,%rbx), %rdx
	addq	$8, %rbx
	movq	%rdx, %r8
	movzbl	%dh, %ecx
	movzbl	%dl, %edx
	andl	$16711680, %r8d
	shrq	$16, %r8
	call	fprintf
.L617:
	movq	0(%rbp), %rsi
	movq	%r12, %rdi
	movq	(%rsi,%rbx), %rax
	movl	$.LC9, %esi
	addq	$8, %rbx
	movq	%rax, %r8
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	andl	$16711680, %r8d
	xorl	%eax, %eax
	shrq	$16, %r8
	call	fprintf
	cmpq	%r13, %rbx
	je	.L604
.L605:
	movq	0(%rbp), %rdi
	movl	$.LC9, %esi
	xorl	%eax, %eax
	movq	(%rdi,%rbx), %rdx
	movq	%r12, %rdi
	movq	%rdx, %r8
	movzbl	%dh, %ecx
	movzbl	%dl, %edx
	andl	$16711680, %r8d
	shrq	$16, %r8
	call	fprintf
	movq	0(%rbp), %r8
	movl	$.LC9, %esi
	movq	%r12, %rdi
	movq	8(%r8,%rbx), %rax
	movq	%rax, %r8
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	andl	$16711680, %r8d
	xorl	%eax, %eax
	shrq	$16, %r8
	call	fprintf
	movq	0(%rbp), %r9
	movl	$.LC9, %esi
	movq	%r12, %rdi
	xorl	%eax, %eax
	movq	16(%r9,%rbx), %rdx
	movq	%rdx, %r8
	movzbl	%dh, %ecx
	movzbl	%dl, %edx
	andl	$16711680, %r8d
	shrq	$16, %r8
	call	fprintf
	movq	0(%rbp), %r10
	movl	$.LC9, %esi
	movq	%r12, %rdi
	movq	24(%r10,%rbx), %rax
	addq	$32, %rbx
	movq	%rax, %r8
	movzbl	%ah, %ecx
	movzbl	%al, %edx
	andl	$16711680, %r8d
	xorl	%eax, %eax
	shrq	$16, %r8
	call	fprintf
	cmpq	%r13, %rbx
	jne	.L605
.L604:
	addq	$8, %rbp
	cmpq	8(%rsp), %rbp
	jne	.L602
.L601:
	movq	%r12, %rdi
	call	fclose
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
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
.L623:
	.cfi_restore_state
	movq	%r13, %rsi
	movl	$.LC23, %edi
	xorl	%eax, %eax
	call	printf
	addq	$88, %rsp
	.cfi_def_cfa_offset 56
	movl	$1, %eax
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
.LFE60:
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
	.p2align 4,,15
	.globl	save_png_file
	.type	save_png_file, @function
save_png_file:
.LFB61:
	.cfi_startproc
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	movq	%rcx, %rbx
	movq	%r12, -16(%rsp)
	movq	%r13, -8(%rsp)
	subq	$120, %rsp
	.cfi_def_cfa_offset 128
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movq	%rdi, 64(%rsp)
	movl	%esi, 60(%rsp)
	movq	%rcx, %rdi
	movl	$.LC22, %esi
	movl	%edx, 56(%rsp)
	call	fopen
	testq	%rax, %rax
	movq	%rax, 40(%rsp)
	je	.L698
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	$.LC15, %edi
	call	png_create_write_struct
	testq	%rax, %rax
	movq	%rax, 32(%rsp)
	je	.L699
	movq	%rax, %rdi
	call	png_create_info_struct
	testq	%rax, %rax
	movq	%rax, 48(%rsp)
	je	.L700
	movq	32(%rsp), %rdi
	movl	$96, %edx
	movl	$longjmp, %esi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	setjmp
	testl	%eax, %eax
	je	.L629
	movl	$.LC26, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$4, %eax
.L626:
	movq	88(%rsp), %rbx
	movq	96(%rsp), %rbp
	movq	104(%rsp), %r12
	movq	112(%rsp), %r13
	addq	$120, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L699:
	.cfi_restore_state
	movl	$.LC25, %edi
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$2, %eax
	jmp	.L626
	.p2align 4,,10
.L700:
	movl	$.LC17, %edi
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$3, %eax
	jmp	.L626
	.p2align 4,,10
.L629:
	movq	40(%rsp), %rsi
	movq	32(%rsp), %rdi
	call	png_init_io
	movq	32(%rsp), %rdi
	movl	$96, %edx
	movl	$longjmp, %esi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	setjmp
	testl	%eax, %eax
	je	.L630
	movl	$.LC27, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$5, %eax
	jmp	.L626
	.p2align 4,,10
.L698:
	movq	%rbx, %rsi
	movl	$.LC24, %edi
	call	printf
	movl	$1, %eax
	jmp	.L626
	.p2align 4,,10
.L630:
	movl	56(%rsp), %ecx
	movl	60(%rsp), %edx
	movl	$2, %r9d
	movq	48(%rsp), %rsi
	movq	32(%rsp), %rdi
	movl	$8, %r8d
	movl	$0, 16(%rsp)
	movl	$0, 8(%rsp)
	movl	$0, (%rsp)
	call	png_set_IHDR
	movq	48(%rsp), %rsi
	movq	32(%rsp), %rdi
	call	png_write_info
	movq	32(%rsp), %rdi
	movl	$96, %edx
	movl	$longjmp, %esi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	setjmp
	testl	%eax, %eax
	je	.L631
	movl	$.LC28, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$6, %eax
	jmp	.L626
	.p2align 4,,10
.L631:
	movslq	56(%rsp), %rdi
	salq	$3, %rdi
	call	malloc
	movl	56(%rsp), %edi
	movq	%rax, 72(%rsp)
	testl	%edi, %edi
	jle	.L635
	movl	60(%rsp), %eax
	movl	56(%rsp), %r12d
	movq	72(%rsp), %rdx
	addl	$9, %eax
	subl	$1, %r12d
	cltq
	leaq	8(%rdx,%r12,8), %r13
	andl	$7, %r12d
	leaq	(%rax,%rax,2), %rbp
	movq	%rbp, %rdi
	call	malloc
	movq	72(%rsp), %rcx
	movq	%rcx, %rbx
	movq	%rax, (%rcx)
	addq	$8, %rbx
	cmpq	%r13, %rbx
	je	.L635
	testq	%r12, %r12
	je	.L636
	cmpq	$1, %r12
	je	.L688
	cmpq	$2, %r12
	je	.L689
	cmpq	$3, %r12
	je	.L690
	cmpq	$4, %r12
	je	.L691
	cmpq	$5, %r12
	je	.L692
	cmpq	$6, %r12
	je	.L693
	movq	%rbp, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addq	$8, %rbx
.L693:
	movq	%rbp, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addq	$8, %rbx
.L692:
	movq	%rbp, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addq	$8, %rbx
.L691:
	movq	%rbp, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addq	$8, %rbx
.L690:
	movq	%rbp, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addq	$8, %rbx
.L689:
	movq	%rbp, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addq	$8, %rbx
.L688:
	movq	%rbp, %rdi
	call	malloc
	movq	%rax, (%rbx)
	addq	$8, %rbx
	cmpq	%r13, %rbx
	je	.L635
.L636:
	movq	%rbp, %rdi
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, (%rbx)
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 8(%rbx)
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 16(%rbx)
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 24(%rbx)
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 32(%rbx)
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 40(%rbx)
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 48(%rbx)
	call	malloc
	movq	%rax, 56(%rbx)
	addq	$64, %rbx
	cmpq	%r13, %rbx
	jne	.L636
.L635:
	movq	64(%rsp), %rcx
	movq	72(%rsp), %rdx
	movl	56(%rsp), %esi
	movl	60(%rsp), %edi
	call	set_pngrow_pointers
	movq	72(%rsp), %rsi
	movq	32(%rsp), %rdi
	call	png_write_image
	movq	32(%rsp), %rdi
	movl	$96, %edx
	movl	$longjmp, %esi
	call	png_set_longjmp_fn
	movq	%rax, %rdi
	call	setjmp
	testl	%eax, %eax
	je	.L701
	movl	$.LC29, %edi
	xorl	%eax, %eax
	call	printf
	movq	40(%rsp), %rdi
	call	fclose
	movl	$7, %eax
	jmp	.L626
.L701:
	movq	32(%rsp), %rdi
	xorl	%esi, %esi
	call	png_write_end
	movl	56(%rsp), %esi
	testl	%esi, %esi
	jle	.L638
	movq	72(%rsp), %r8
	movl	56(%rsp), %ebp
	movq	(%r8), %rdi
	subl	$1, %ebp
	leaq	8(%r8,%rbp,8), %r12
	andl	$7, %ebp
	call	free
	movq	72(%rsp), %r13
	addq	$8, %r13
	cmpq	%r12, %r13
	je	.L638
	testq	%rbp, %rbp
	je	.L639
	cmpq	$1, %rbp
	je	.L682
	cmpq	$2, %rbp
	je	.L683
	cmpq	$3, %rbp
	je	.L684
	cmpq	$4, %rbp
	je	.L685
	cmpq	$5, %rbp
	je	.L686
	cmpq	$6, %rbp
	je	.L687
	movq	0(%r13), %rdi
	addq	$8, %r13
	call	free
.L687:
	movq	0(%r13), %rdi
	addq	$8, %r13
	call	free
.L686:
	movq	0(%r13), %rdi
	addq	$8, %r13
	call	free
.L685:
	movq	0(%r13), %rdi
	addq	$8, %r13
	call	free
.L684:
	movq	0(%r13), %rdi
	addq	$8, %r13
	call	free
.L683:
	movq	0(%r13), %rdi
	addq	$8, %r13
	call	free
.L682:
	movq	0(%r13), %rdi
	addq	$8, %r13
	call	free
	cmpq	%r12, %r13
	je	.L638
.L639:
	movq	0(%r13), %rdi
	call	free
	movq	8(%r13), %rdi
	call	free
	movq	16(%r13), %rdi
	call	free
	movq	24(%r13), %rdi
	call	free
	movq	32(%r13), %rdi
	call	free
	movq	40(%r13), %rdi
	call	free
	movq	48(%r13), %rdi
	call	free
	movq	56(%r13), %rdi
	addq	$64, %r13
	call	free
	cmpq	%r12, %r13
	jne	.L639
.L638:
	movq	72(%rsp), %rdi
	call	free
	movq	40(%rsp), %rdi
	call	fclose
	xorl	%eax, %eax
	jmp	.L626
	.cfi_endproc
.LFE61:
	.size	save_png_file, .-save_png_file
	.section	.rodata.str1.1
.LC30:
	.string	"setjmp error"
	.section	.rodata.str1.8
	.align 8
.LC31:
	.string	"load_jpg: warning: mmap file failed, using normal buffered I/O: %s\n"
	.text
	.p2align 4,,15
	.globl	load_jpeg_file
	.type	load_jpeg_file, @function
load_jpeg_file:
.LFB62:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$1016, %rsp
	.cfi_def_cfa_offset 1072
	movl	ncache(%rip), %eax
	movq	%rdi, 24(%rsp)
	movq	%rsi, 40(%rsp)
	movq	%rdx, 48(%rsp)
	movq	%rcx, 16(%rsp)
	testl	%eax, %eax
	jle	.L703
	movq	icache(%rip), %rbp
	subl	$1, %eax
	movq	%rcx, %rdi
	leaq	(%rax,%rax,2), %rdx
	xorl	%r12d, %r12d
	leaq	24(%rbp), %r15
	movq	0(%rbp), %rsi
	movq	%rbp, %rbx
	leaq	(%r15,%rdx,8), %r14
	call	strcmp
	movq	%r14, %r13
	subq	%rbp, %r13
	subq	$24, %r13
	shrq	$3, %r13
	leaq	0(%r13,%r13,2), %r13
	andl	$7, %r13d
	testl	%eax, %eax
	je	.L800
	cmpq	%r14, %r15
	movq	%r15, %rbx
	je	.L703
	testq	%r13, %r13
	je	.L807
	cmpq	$1, %r13
	je	.L793
	cmpq	$2, %r13
	je	.L794
	cmpq	$3, %r13
	je	.L795
	cmpq	$4, %r13
	je	.L796
	cmpq	$5, %r13
	je	.L797
	cmpq	$6, %r13
	je	.L798
	movq	(%r15), %rsi
	movq	16(%rsp), %rdi
	movq	%r15, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
.L798:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
.L797:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
.L796:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
.L795:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
.L794:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
.L793:
	movq	(%rbx), %rsi
	movq	16(%rsp), %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
	cmpq	%r14, %rbx
	je	.L703
.L807:
	movq	16(%rsp), %r15
.L706:
	movq	(%rbx), %rsi
	movq	%rbx, %r12
	movq	%r15, %rdi
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	leaq	24(%rbx), %r13
	movq	%r15, %rdi
	movq	0(%r13), %rsi
	movq	%r13, %r12
	movq	%r13, %rbx
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	addq	$24, %rbx
	movq	%r15, %rdi
	movq	(%rbx), %rsi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	leaq	48(%r13), %rbx
	movq	48(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	leaq	72(%r13), %rbx
	movq	72(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	leaq	96(%r13), %rbx
	movq	96(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	leaq	120(%r13), %rbx
	movq	120(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	leaq	144(%r13), %rbx
	movq	144(%r13), %rsi
	movq	%r15, %rdi
	movq	%rbx, %r12
	subq	%rbp, %r12
	call	strcmp
	testl	%eax, %eax
	je	.L800
	leaq	168(%r13), %rbx
	cmpq	%r14, %rbx
	jne	.L706
.L703:
	movq	16(%rsp), %rdi
	movl	$.LC11, %esi
	movl	$1, %ebp
	call	fopen
	testq	%rax, %rax
	movq	%rax, 32(%rsp)
	je	.L730
	movl	__isthreaded(%rip), %r8d
	testl	%r8d, %r8d
	jne	.L707
	movq	32(%rsp), %rcx
	movswl	18(%rcx), %r14d
.L708:
	leaq	352(%rsp), %rsi
	movl	%r14d, %edi
	call	fstat
	testl	%eax, %eax
	js	.L815
	movq	424(%rsp), %rsi
	xorl	%r9d, %r9d
	xorl	%edi, %edi
	movl	%r14d, %r8d
	movl	$2, %ecx
	movl	$1, %edx
	call	mmap
	movq	424(%rsp), %rsi
	cmpq	$-1, %rax
	movq	%rax, 56(%rsp)
	movq	%rsi, 64(%rsp)
	je	.L816
.L726:
	leaq	80(%rsp), %rdi
	call	jpeg_std_error
	leaq	80(%rsp), %rdi
	movq	%rax, 352(%rsp)
	movq	$my_error_exit, 80(%rsp)
	movq	%rdi, errptr(%rip)
	addq	$168, %rdi
	call	setjmp
	testl	%eax, %eax
	je	.L712
	leaq	352(%rsp), %rdi
	call	jpeg_destroy_decompress
	cmpq	$0, 56(%rsp)
	je	.L713
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L713:
	movq	32(%rsp), %rdi
	movl	$2, %ebp
	call	fclose
	movl	$.LC30, %edi
	call	puts
.L730:
	addq	$1016, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	%ebp, %eax
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
	.p2align 4,,10
.L800:
	.cfi_restore_state
	movq	8(%rbx), %r8
	movq	24(%rsp), %rbp
	movq	40(%rsp), %rsi
	movq	48(%rsp), %r9
	movq	%r8, 0(%rbp)
	addq	icache(%rip), %r12
	xorl	%ebp, %ebp
	movl	16(%r12), %ecx
	movl	%ecx, (%rsi)
	movl	20(%r12), %edi
	movl	%edi, (%r9)
	jmp	.L730
	.p2align 4,,10
.L712:
	leaq	352(%rsp), %rdi
	movl	$656, %edx
	movl	$80, %esi
	call	jpeg_CreateDecompress
	cmpq	$0, 56(%rsp)
	je	.L714
	movq	64(%rsp), %rdx
	movq	56(%rsp), %rsi
	leaq	352(%rsp), %rdi
	call	jpeg_mem_src
	movq	32(%rsp), %rdi
	call	fclose
	movq	$0, 32(%rsp)
.L715:
	leaq	352(%rsp), %rdi
	movl	$1, %esi
	movl	$1, %r13d
	call	jpeg_read_header
	cmpl	$1, 412(%rsp)
	je	.L716
	movl	$0, 460(%rsp)
	xorb	%r13b, %r13b
.L716:
	leaq	352(%rsp), %rdi
	call	jpeg_start_decompress
	movl	492(%rsp), %r15d
	movl	488(%rsp), %r14d
	movl	$8, %esi
	movq	40(%rsp), %r9
	movq	48(%rsp), %r10
	movslq	%r15d, %rdi
	movl	%r14d, (%r9)
	movl	%r15d, (%r10)
	call	calloc
	movq	24(%rsp), %r11
	testl	%r15d, %r15d
	movq	%rax, %r12
	movq	%rax, (%r11)
	jle	.L720
	leal	-1(%r15), %edx
	movq	40(%rsp), %rax
	xorl	%ebx, %ebx
	leaq	8(,%rdx,8), %r15
	andl	$7, %edx
	movslq	(%rax), %rbp
	je	.L805
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rdx, 8(%rsp)
	call	calloc
	movq	8(%rsp), %r8
	movq	%rax, (%r12,%rbx)
	movl	$8, %ebx
	movq	24(%rsp), %r12
	cmpq	$1, %r8
	movq	(%r12), %r12
	je	.L805
	cmpq	$2, %r8
	je	.L788
	cmpq	$3, %r8
	je	.L789
	cmpq	$4, %r8
	je	.L790
	cmpq	$5, %r8
	je	.L791
	cmpq	$6, %r8
	je	.L792
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	24(%rsp), %rcx
	movq	%rax, (%r12,%rbx)
	movb	$16, %bl
	movq	(%rcx), %r12
.L792:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	24(%rsp), %rsi
	movq	%rax, (%r12,%rbx)
	addq	$8, %rbx
	movq	(%rsi), %r12
.L791:
	movq	%rbp, %rdi
	movl	$8, %esi
	call	calloc
	movq	24(%rsp), %rdi
	movq	%rax, (%r12,%rbx)
	addq	$8, %rbx
	movq	(%rdi), %r12
.L790:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	24(%rsp), %r9
	movq	%rax, (%r12,%rbx)
	addq	$8, %rbx
	movq	(%r9), %r12
.L789:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	24(%rsp), %r10
	movq	%rax, (%r12,%rbx)
	addq	$8, %rbx
	movq	(%r10), %r12
.L788:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	24(%rsp), %r11
	movq	%rax, (%r12,%rbx)
	addq	$8, %rbx
	movl	%r13d, 76(%rsp)
	movq	(%r11), %rax
	movq	%r11, %r12
	movq	%rax, %r13
	jmp	.L721
	.p2align 4,,10
.L817:
	movq	(%r12), %r13
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 8(%r13,%rbx)
	movq	(%r12), %r13
	call	calloc
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 16(%r13,%rbx)
	movq	(%r12), %r13
	call	calloc
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 24(%r13,%rbx)
	movq	(%r12), %r13
	call	calloc
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 32(%r13,%rbx)
	movq	(%r12), %r13
	call	calloc
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 40(%r13,%rbx)
	movq	(%r12), %r13
	call	calloc
	movl	$8, %esi
	movq	%rbp, %rdi
	movq	%rax, 48(%r13,%rbx)
	movq	(%r12), %r13
	call	calloc
	movq	%rax, 56(%r13,%rbx)
	movq	(%r12), %r13
	addq	$64, %rbx
.L721:
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	leaq	8(%rbx), %rdx
	movq	%rax, 0(%r13,%rbx)
	cmpq	%r15, %rdx
	jne	.L817
	movl	76(%rsp), %r13d
.L720:
	movl	500(%rsp), %edx
	movq	360(%rsp), %rbp
	movl	$32, %ecx
	movl	$1, %esi
	leaq	352(%rsp), %rdi
	imull	%r14d, %edx
	leaq	352(%rsp), %r14
	call	*16(%rbp)
	movq	48(%rsp), %r15
	movl	520(%rsp), %r8d
	movq	%rax, %r12
	movl	(%r15), %ebx
	movq	40(%rsp), %r15
	subl	$1, %ebx
	cmpl	%r8d, 492(%rsp)
	jbe	.L719
	.p2align 4,,10
.L799:
	movl	$32, %edx
	movq	%r12, %rsi
	movq	%r14, %rdi
	call	jpeg_read_scanlines
	movq	24(%rsp), %rcx
	movl	%eax, %ebp
	movl	%eax, %esi
	movl	%ebx, %edi
	movl	%r13d, %r9d
	movq	%r12, %rdx
	subl	%ebp, %ebx
	movq	(%rcx), %r8
	movl	(%r15), %ecx
	call	set_jpeg_rows_array
	movl	492(%rsp), %esi
	cmpl	%esi, 520(%rsp)
	jb	.L799
.L719:
	leaq	352(%rsp), %rdi
	call	jpeg_finish_decompress
	leaq	352(%rsp), %rdi
	call	jpeg_destroy_decompress
	cmpq	$0, 56(%rsp)
	je	.L723
	movq	64(%rsp), %rsi
	movq	56(%rsp), %rdi
	call	munmap
.L723:
	movq	48(%rsp), %rdi
	movl	ncache(%rip), %r15d
	xorl	%ebp, %ebp
	movq	40(%rsp), %r9
	movq	24(%rsp), %r10
	movq	icache(%rip), %rax
	movslq	%r15d, %r11
	movl	(%rdi), %r13d
	movq	16(%rsp), %rdi
	leaq	(%r11,%r11,2), %rdx
	movl	(%r9), %r14d
	movq	(%r10), %r12
	addl	$1, %r15d
	leaq	(%rax,%rdx,8), %rbx
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	16(%rsp), %rsi
	movq	%rax, (%rbx)
	movq	%rax, %rdi
	call	strcpy
	cmpq	$0, 32(%rsp)
	movl	%r14d, 16(%rbx)
	movl	%r13d, 20(%rbx)
	movq	%r12, 8(%rbx)
	movl	%r15d, ncache(%rip)
	je	.L730
	movq	32(%rsp), %rdi
	call	fclose
	jmp	.L730
	.p2align 4,,10
.L707:
	movq	32(%rsp), %rdi
	call	fileno
	movl	%eax, %r14d
	jmp	.L708
	.p2align 4,,10
.L815:
	movl	$.LC0, %edi
	call	perror
.L710:
	movq	16(%rsp), %rsi
	movl	$.LC31, %edi
	xorl	%eax, %eax
	call	printf
	movq	$0, 56(%rsp)
	movq	$0, 64(%rsp)
	jmp	.L726
	.p2align 4,,10
.L816:
	movl	$.LC1, %edi
	call	perror
	jmp	.L710
	.p2align 4,,10
.L805:
	movl	%r13d, 76(%rsp)
	movq	%r12, %r13
	movq	24(%rsp), %r12
	jmp	.L721
.L714:
	movq	32(%rsp), %rsi
	leaq	352(%rsp), %rdi
	call	jpeg_stdio_src
	jmp	.L715
	.cfi_endproc
.LFE62:
	.size	load_jpeg_file, .-load_jpeg_file
	.p2align 4,,15
	.globl	compute_pixels
	.type	compute_pixels, @function
compute_pixels:
.LFB63:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%ecx, -4(%rsp)
	movl	$0, -12(%rsp)
	movl	-12(%rsp), %eax
	cmpl	%eax, %ecx
	jle	.L818
	.p2align 4,,10
.L826:
	movl	-4(%rsp), %ebx
	subl	$1, %ebx
	movl	%ebx, -4(%rsp)
	movl	$0, -8(%rsp)
	movl	-8(%rsp), %r8d
	cmpl	%r8d, %edx
	jle	.L823
	.p2align 4,,10
.L827:
	movslq	-12(%rsp), %r9
	movslq	-8(%rsp), %r10
	movl	-4(%rsp), %ebx
	movl	-8(%rsp), %r8d
	movq	(%rsi,%r9,8), %r11
	imull	%edx, %ebx
	movq	(%r11,%r10,8), %rax
	addl	%r8d, %ebx
	leal	(%rbx,%rbx,2), %r9d
	movl	-4(%rsp), %ebx
	movl	-8(%rsp), %r8d
	movq	%rax, %r11
	movslq	%r9d, %r10
	shrq	$16, %r11
	movb	%r11b, (%rdi,%r10)
	movl	-4(%rsp), %r11d
	imull	%edx, %ebx
	imull	%edx, %r11d
	addl	%r8d, %ebx
	movl	-8(%rsp), %r8d
	leal	(%rbx,%rbx,2), %r9d
	movzbl	%ah, %ebx
	addl	%r8d, %r11d
	movslq	%r9d, %r10
	leal	(%r11,%r11,2), %r9d
	movb	%bl, 1(%rdi,%r10)
	movslq	%r9d, %r10
	movb	%al, 2(%rdi,%r10)
	movl	-8(%rsp), %eax
	addl	$1, %eax
	movl	%eax, -8(%rsp)
	movl	-8(%rsp), %ebx
	cmpl	%edx, %ebx
	jl	.L827
.L823:
	movl	-12(%rsp), %r11d
	addl	$1, %r11d
	movl	%r11d, -12(%rsp)
	movl	-12(%rsp), %r8d
	cmpl	%r8d, %ecx
	jg	.L826
.L818:
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE63:
	.size	compute_pixels, .-compute_pixels
	.p2align 4,,15
	.globl	init_pixels
	.type	init_pixels, @function
init_pixels:
.LFB64:
	.cfi_startproc
	movl	%edx, %eax
	movq	%r13, -8(%rsp)
	.cfi_offset 13, -16
	movq	%rdi, %r13
	imull	%ecx, %eax
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	movq	%r12, -16(%rsp)
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	movq	%rsi, %r12
	movl	%edx, %ebx
	movl	%ecx, %ebp
	leal	9(%rax,%rax,2), %edi
	movslq	%edi, %rdi
	call	malloc
	movl	ualoop(%rip), %r9d
	movq	%rax, 0(%r13)
	movl	%ebp, %ecx
	movl	%ebx, %edx
	movq	%r12, %rsi
	movq	%rax, %rdi
	testl	%r9d, %r9d
	jne	.L832
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	compute_pixels
	.p2align 4,,10
.L832:
	.cfi_restore_state
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	jmp	compute_asm
	.cfi_endproc
.LFE64:
	.size	init_pixels, .-init_pixels
	.p2align 4,,15
	.globl	free_pixels
	.type	free_pixels, @function
free_pixels:
.LFB65:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	(%rdi), %rdi
	call	free
	movq	$0, (%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE65:
	.size	free_pixels, .-free_pixels
	.section	.rodata.str1.8
	.align 8
.LC33:
	.string	"Failed to write jpeg scanlines: written: %d/%d\n"
	.text
	.p2align 4,,15
	.globl	save_jpeg_file
	.type	save_jpeg_file, @function
save_jpeg_file:
.LFB66:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	%rcx, %rdi
	subq	$912, %rsp
	.cfi_def_cfa_offset 928
	movl	%esi, 32(%rsp)
	movl	$.LC22, %esi
	movl	%edx, 12(%rsp)
	call	fopen
	movq	%rax, 16(%rsp)
	cmpq	$0, 16(%rsp)
	movl	$5, %eax
	je	.L836
	movl	32(%rsp), %eax
	imull	12(%rsp), %eax
	leal	9(%rax,%rax,2), %edi
	movslq	%edi, %rdi
	call	malloc
	movl	ualoop(%rip), %r11d
	movq	%rax, 24(%rsp)
	testl	%r11d, %r11d
	jne	.L870
	movl	12(%rsp), %ecx
	movl	32(%rsp), %edx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	compute_pixels
.L838:
	leaq	48(%rsp), %rdi
	call	jpeg_std_error
	leaq	48(%rsp), %rdx
	movq	%rax, 320(%rsp)
	movq	$my_error_exit, 48(%rsp)
	movq	%rdx, %rdi
	movq	%rdx, errptr(%rip)
	addq	$168, %rdi
	call	setjmp
	testl	%eax, %eax
	je	.L839
	leaq	320(%rsp), %rdi
	call	jpeg_destroy_compress
	movq	16(%rsp), %rdi
	call	fclose
	movl	$2, %eax
.L836:
	addq	$912, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L870:
	.cfi_restore_state
	movl	12(%rsp), %ecx
	movl	32(%rsp), %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	compute_asm
	jmp	.L838
	.p2align 4,,10
.L839:
	leaq	320(%rsp), %rdi
	movl	$584, %edx
	movl	$80, %esi
	call	jpeg_CreateCompress
	movq	16(%rsp), %rsi
	leaq	320(%rsp), %rdi
	call	jpeg_stdio_dest
	movl	32(%rsp), %ecx
	movl	12(%rsp), %esi
	leaq	320(%rsp), %rdi
	movl	$3, 376(%rsp)
	movl	$2, 380(%rsp)
	movl	%ecx, 368(%rsp)
	movl	%esi, 372(%rsp)
	call	jpeg_set_defaults
	movl	jqual(%rip), %esi
	leaq	320(%rsp), %rdi
	xorl	%edx, %edx
	call	jpeg_set_quality
	leaq	320(%rsp), %rdi
	movl	$1, %esi
	call	jpeg_start_compress
	movslq	12(%rsp), %rdi
	movl	32(%rsp), %r8d
	leal	(%r8,%r8,2), %ebx
	salq	$3, %rdi
	call	malloc
	movl	12(%rsp), %r10d
	testl	%r10d, %r10d
	jle	.L852
	movq	%rax, %rsi
	movq	%rax, %rdi
	salq	$60, %rsi
	shrq	$63, %rsi
	cmpl	%esi, 12(%rsp)
	cmovbe	12(%rsp), %esi
	cmpl	$6, 12(%rsp)
	ja	.L871
	movl	12(%rsp), %esi
.L843:
	movq	24(%rsp), %rcx
	movslq	%ebx, %r11
	movl	%esi, %r9d
	xorl	%edx, %edx
	.p2align 4,,10
.L846:
	addl	$1, %edx
	movq	%rcx, (%rdi)
	addq	%r11, %rcx
	addq	$8, %rdi
	cmpl	%r9d, %edx
	jne	.L846
	cmpl	%esi, 12(%rsp)
	je	.L852
.L844:
	movl	12(%rsp), %r11d
	movl	%esi, %r8d
	subl	%esi, %r11d
	movl	%r11d, %esi
	shrl	$2, %esi
	leal	0(,%rsi,4), %r10d
	testl	%r10d, %r10d
	je	.L848
	leal	2(%rdx), %ecx
	leal	3(%rdx), %r9d
	movl	%edx, 8(%rsp)
	leal	1(%rdx), %edi
	movq	24(%rsp), %xmm5
	movl	%ecx, 40(%rsp)
	movl	%r9d, 44(%rsp)
	leaq	(%rax,%r8,8), %rcx
	movd	44(%rsp), %xmm1
	movl	%edi, 36(%rsp)
	leal	-1(%rsi), %edi
	movd	40(%rsp), %xmm0
	punpcklqdq	%xmm5, %xmm5
	movd	36(%rsp), %xmm2
	xorl	%r8d, %r8d
	andl	$3, %edi
	punpckldq	%xmm1, %xmm0
	movd	8(%rsp), %xmm1
	movl	%ebx, 8(%rsp)
	movdqa	%xmm5, %xmm10
	movd	8(%rsp), %xmm3
	punpckldq	%xmm2, %xmm1
	movdqa	.LC32(%rip), %xmm4
	pshufd	$0, %xmm3, %xmm9
	punpcklqdq	%xmm0, %xmm1
	pxor	%xmm11, %xmm11
	movdqa	%xmm9, %xmm6
	psrlq	$32, %xmm6
	je	.L853
	movdqa	%xmm1, %xmm7
	movl	$1, %r8d
	movdqa	%xmm9, %xmm8
	psrlq	$32, %xmm7
	pmuludq	%xmm6, %xmm7
	pshufd	$8, %xmm7, %xmm13
	pmuludq	%xmm1, %xmm8
	pshufd	$8, %xmm8, %xmm12
	movdqa	%xmm11, %xmm14
	paddd	%xmm4, %xmm1
	punpckldq	%xmm13, %xmm12
	pcmpgtd	%xmm12, %xmm14
	movdqa	%xmm12, %xmm15
	punpckhdq	%xmm14, %xmm12
	punpckldq	%xmm14, %xmm15
	paddq	%xmm5, %xmm12
	movdqa	%xmm12, 16(%rcx)
	paddq	%xmm5, %xmm15
	movdqa	%xmm15, (%rcx)
	addq	$32, %rcx
	cmpl	$1, %edi
	je	.L853
	cmpl	$2, %edi
	je	.L866
	movdqa	%xmm1, %xmm3
	movb	$2, %r8b
	movdqa	%xmm9, %xmm0
	psrlq	$32, %xmm3
	pmuludq	%xmm6, %xmm3
	pshufd	$8, %xmm3, %xmm5
	pmuludq	%xmm1, %xmm0
	pshufd	$8, %xmm0, %xmm2
	movdqa	%xmm11, %xmm7
	paddd	%xmm4, %xmm1
	punpckldq	%xmm5, %xmm2
	pcmpgtd	%xmm2, %xmm7
	movdqa	%xmm2, %xmm8
	punpckhdq	%xmm7, %xmm2
	punpckldq	%xmm7, %xmm8
	paddq	%xmm10, %xmm2
	movdqa	%xmm2, 16(%rcx)
	paddq	%xmm10, %xmm8
	movdqa	%xmm8, (%rcx)
	addq	$32, %rcx
.L866:
	movdqa	%xmm1, %xmm13
	addl	$1, %r8d
	movdqa	%xmm9, %xmm12
	psrlq	$32, %xmm13
	pmuludq	%xmm6, %xmm13
	pshufd	$8, %xmm13, %xmm15
	pmuludq	%xmm1, %xmm12
	pshufd	$8, %xmm12, %xmm14
	movdqa	%xmm11, %xmm0
	paddd	%xmm4, %xmm1
	punpckldq	%xmm15, %xmm14
	pcmpgtd	%xmm14, %xmm0
	movdqa	%xmm14, %xmm3
	punpckhdq	%xmm0, %xmm14
	punpckldq	%xmm0, %xmm3
	paddq	%xmm10, %xmm14
	movdqa	%xmm14, 16(%rcx)
	paddq	%xmm10, %xmm3
	movdqa	%xmm3, (%rcx)
	addq	$32, %rcx
	jmp	.L853
	.p2align 4,,10
.L849:
	movdqa	%xmm9, %xmm14
	addl	$4, %r8d
	movdqa	%xmm5, %xmm13
	pmuludq	%xmm5, %xmm14
	psrlq	$32, %xmm5
	pshufd	$8, %xmm14, %xmm15
	pmuludq	%xmm6, %xmm5
	pshufd	$8, %xmm5, %xmm0
	movdqa	%xmm11, %xmm3
	paddd	%xmm4, %xmm13
	movdqa	%xmm13, %xmm1
	movdqa	%xmm9, %xmm2
	movdqa	%xmm9, %xmm14
	punpckldq	%xmm0, %xmm15
	paddd	%xmm4, %xmm1
	pmuludq	%xmm13, %xmm2
	pmuludq	%xmm1, %xmm14
	pshufd	$8, %xmm2, %xmm12
	psrlq	$32, %xmm13
	pshufd	$8, %xmm14, %xmm0
	pmuludq	%xmm6, %xmm13
	pcmpgtd	%xmm15, %xmm3
	movdqa	%xmm15, %xmm5
	pshufd	$8, %xmm13, %xmm7
	punpckldq	%xmm3, %xmm5
	movdqa	%xmm11, %xmm13
	punpckhdq	%xmm3, %xmm15
	punpckldq	%xmm7, %xmm12
	paddq	%xmm10, %xmm5
	movdqa	%xmm5, 32(%rcx)
	paddq	%xmm10, %xmm15
	movdqa	%xmm11, %xmm5
	movdqa	%xmm15, 16(%rdi)
	movdqa	%xmm1, %xmm15
	paddd	%xmm4, %xmm1
	pcmpgtd	%xmm12, %xmm13
	movdqa	%xmm12, %xmm8
	psrlq	$32, %xmm15
	pmuludq	%xmm6, %xmm15
	pshufd	$8, %xmm15, %xmm3
	punpckldq	%xmm13, %xmm8
	punpckhdq	%xmm13, %xmm12
	punpckldq	%xmm3, %xmm0
	paddq	%xmm10, %xmm8
	movdqa	%xmm8, 64(%rcx)
	paddq	%xmm10, %xmm12
	pcmpgtd	%xmm0, %xmm5
	movdqa	%xmm0, %xmm2
	movdqa	%xmm12, 80(%rcx)
	punpckldq	%xmm5, %xmm2
	punpckhdq	%xmm5, %xmm0
	paddq	%xmm10, %xmm2
	movdqa	%xmm2, 96(%rcx)
	paddq	%xmm10, %xmm0
	movdqa	%xmm0, 112(%rcx)
	subq	$-128, %rcx
.L853:
	movdqa	%xmm9, %xmm2
	leal	1(%r8), %r9d
	movdqa	%xmm1, %xmm5
	leaq	32(%rcx), %rdi
	pmuludq	%xmm1, %xmm2
	psrlq	$32, %xmm1
	pshufd	$8, %xmm2, %xmm7
	pmuludq	%xmm6, %xmm1
	pshufd	$8, %xmm1, %xmm1
	movdqa	%xmm11, %xmm12
	cmpl	%esi, %r9d
	paddd	%xmm4, %xmm5
	punpckldq	%xmm1, %xmm7
	pcmpgtd	%xmm7, %xmm12
	movdqa	%xmm7, %xmm8
	punpckhdq	%xmm12, %xmm7
	punpckldq	%xmm12, %xmm8
	paddq	%xmm10, %xmm7
	movdqa	%xmm7, 16(%rcx)
	paddq	%xmm10, %xmm8
	movdqa	%xmm8, (%rcx)
	jb	.L849
	addl	%r10d, %edx
	cmpl	%r10d, %r11d
	je	.L852
.L848:
	movslq	%ebx, %r11
	movslq	%edx, %r10
	imull	%edx, %ebx
	leaq	(%rax,%r10,8), %rcx
	movslq	%ebx, %rsi
	addq	24(%rsp), %rsi
	.p2align 4,,10
.L851:
	movq	%rsi, (%rcx)
	addl	$1, %edx
	addq	%r11, %rsi
	addq	$8, %rcx
	cmpl	%edx, 12(%rsp)
	jg	.L851
.L852:
	movl	12(%rsp), %edx
	leaq	320(%rsp), %rdi
	movq	%rax, %rsi
	call	jpeg_write_scanlines
	cmpl	%eax, 12(%rsp)
	jg	.L872
	leaq	320(%rsp), %rdi
	call	jpeg_finish_compress
	leaq	320(%rsp), %rdi
	call	jpeg_destroy_compress
	movq	16(%rsp), %rdi
	call	fclose
	addq	$912, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L872:
	.cfi_restore_state
	movl	12(%rsp), %edx
	movl	%eax, %esi
	movl	$.LC33, %edi
	xorl	%eax, %eax
	call	printf
	movq	16(%rsp), %rdi
	call	fclose
	movl	$1, %eax
	jmp	.L836
.L871:
	testl	%esi, %esi
	jne	.L843
	xorl	%edx, %edx
	.p2align 4,,2
	jmp	.L844
	.cfi_endproc
.LFE66:
	.size	save_jpeg_file, .-save_jpeg_file
	.p2align 4,,15
	.globl	save_jpeg_file_old
	.type	save_jpeg_file_old, @function
save_jpeg_file_old:
.LFB67:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	movq	%rcx, %rdi
	subq	$920, %rsp
	.cfi_def_cfa_offset 944
	movl	%esi, 24(%rsp)
	movl	$.LC22, %esi
	movl	%edx, 28(%rsp)
	call	fopen
	movq	%rax, 16(%rsp)
	cmpq	$0, 16(%rsp)
	movl	$5, %eax
	je	.L874
	movl	24(%rsp), %eax
	imull	28(%rsp), %eax
	leal	9(%rax,%rax,2), %edi
	movslq	%edi, %rdi
	call	malloc
	movl	ualoop(%rip), %ebp
	movq	%rax, 8(%rsp)
	testl	%ebp, %ebp
	jne	.L884
	movl	28(%rsp), %ecx
	movl	24(%rsp), %edx
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	compute_pixels
.L876:
	leaq	48(%rsp), %rbx
	movq	%rbx, %rdi
	call	jpeg_std_error
	leaq	168(%rbx), %rdi
	movq	%rax, 320(%rsp)
	movq	$my_error_exit, 48(%rsp)
	movq	%rbx, errptr(%rip)
	call	setjmp
	testl	%eax, %eax
	je	.L877
	leaq	320(%rsp), %rdi
	call	jpeg_destroy_compress
	movq	16(%rsp), %rdi
	call	fclose
	movl	$2, %eax
.L874:
	addq	$920, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L884:
	.cfi_restore_state
	movl	28(%rsp), %ecx
	movl	24(%rsp), %edx
	movq	%rbx, %rsi
	movq	%rax, %rdi
	call	compute_asm
	jmp	.L876
	.p2align 4,,10
.L877:
	leaq	320(%rsp), %rdi
	movl	$584, %edx
	movl	$80, %esi
	leaq	320(%rsp), %rbx
	call	jpeg_CreateCompress
	movq	16(%rsp), %rsi
	leaq	320(%rsp), %rdi
	call	jpeg_stdio_dest
	movl	28(%rsp), %ecx
	movl	24(%rsp), %edx
	leaq	320(%rsp), %rdi
	movl	$3, 376(%rsp)
	movl	$2, 380(%rsp)
	movl	%ecx, 372(%rsp)
	movl	%edx, 368(%rsp)
	call	jpeg_set_defaults
	movl	jqual(%rip), %esi
	leaq	320(%rsp), %rdi
	xorl	%edx, %edx
	call	jpeg_set_quality
	leaq	320(%rsp), %rdi
	movl	$1, %esi
	call	jpeg_start_compress
	movl	660(%rsp), %r8d
	cmpl	372(%rsp), %r8d
	movl	24(%rsp), %esi
	leal	(%rsi,%rsi,2), %ebp
	jae	.L879
	.p2align 4,,10
.L882:
	imull	%ebp, %r8d
	addq	8(%rsp), %r8
	leaq	32(%rsp), %rsi
	movl	$1, %edx
	movq	%rbx, %rdi
	movq	%r8, 32(%rsp)
	call	jpeg_write_scanlines
	movl	660(%rsp), %r8d
	cmpl	372(%rsp), %r8d
	jb	.L882
.L879:
	leaq	320(%rsp), %rdi
	call	jpeg_finish_compress
	leaq	320(%rsp), %rdi
	call	jpeg_destroy_compress
	movq	16(%rsp), %rdi
	call	fclose
	addq	$920, %rsp
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE67:
	.size	save_jpeg_file_old, .-save_jpeg_file_old
	.section	.rodata.str1.1
.LC34:
	.string	"%d.%06ds "
	.text
	.p2align 4,,15
	.globl	translate_file
	.type	translate_file, @function
translate_file:
.LFB68:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movl	%edi, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rsi, %rbx
	subq	$104, %rsp
	.cfi_def_cfa_offset 160
	movl	168(%rsp), %r12d
	movq	%rdx, 40(%rsp)
	movl	%ecx, 20(%rsp)
	movl	%r8d, 24(%rsp)
	movl	%r9d, 28(%rsp)
	testl	%r12d, %r12d
	je	.L886
	movl	ualoop(%rip), %r10d
	testl	%r10d, %r10d
	jne	.L1523
.L887:
	movl	20(%rsp), %eax
	addl	24(%rsp), %eax
	movl	28(%rsp), %ecx
	sarl	%eax
	movl	%ecx, 64(%rsp)
	movl	64(%rsp), %r14d
	cmpl	%r14d, 24(%rsp)
	jle	.L885
	movl	%r12d, %r15d
	movl	20(%rsp), %ebp
	movq	40(%rsp), %r12
	cltq
	movq	%rax, 48(%rsp)
	.p2align 4,,10
.L944:
	cmpl	$4, %r15d
	je	.L1524
	cmpl	$11, %r15d
	je	.L1525
	cmpl	$12, %r15d
	je	.L1526
	cmpl	$1, %r15d
	je	.L1527
	cmpl	$2, %r15d
	je	.L1528
	cmpl	$3, %r15d
	je	.L1529
	cmpl	$5, %r15d
	je	.L1530
	cmpl	$6, %r15d
	je	.L1531
	cmpl	$7, %r15d
	je	.L1532
	cmpl	$8, %r15d
	je	.L1533
	cmpl	$9, %r15d
	je	.L1534
	cmpl	$10, %r15d
	jne	.L895
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r11d
	cmpl	%r11d, %ebp
	jle	.L895
.L943:
	movslq	64(%rsp), %rsi
	movslq	80(%rsp), %rdi
	movq	(%r12,%rsi,8), %r9
	leaq	(%r9,%rdi,8), %r13
	movslq	64(%rsp), %r9
	movslq	80(%rsp), %r10
	movslq	64(%rsp), %rax
	movslq	80(%rsp), %r8
	movslq	64(%rsp), %rcx
	movslq	80(%rsp), %r14
	movslq	64(%rsp), %r11
	movslq	80(%rsp), %rdx
	movslq	64(%rsp), %rdi
	movl	80(%rsp), %esi
	movl	%esi, 28(%rsp)
	movl	64(%rsp), %esi
	movl	%esi, 20(%rsp)
	movl	80(%rsp), %esi
	movl	%esi, 32(%rsp)
	movq	40(%rbx), %rsi
	movq	(%rsi,%r9,8), %r9
	movq	32(%rbx), %rsi
	movq	(%rsi,%rax,8), %rax
	movq	24(%rbx), %rsi
	movq	(%r9,%r10,8), %r9
	movq	(%rsi,%rcx,8), %rcx
	movslq	28(%rsp), %rsi
	movq	(%rax,%r8,8), %r8
	movq	(%rcx,%r14,8), %rcx
	movq	16(%rbx), %r14
	movq	(%r14,%r11,8), %r11
	movq	8(%rbx), %r14
	movq	(%r14,%rdi,8), %rdi
	movq	(%r11,%rdx,8), %rdx
	movslq	20(%rsp), %r11
	movslq	32(%rsp), %r14
	movq	(%rdi,%rsi,8), %rsi
	movq	(%rbx), %rdi
	movq	(%rdi,%r11,8), %r11
	movq	(%r11,%r14,8), %rdi
	call	udfi6
	movq	%rax, 0(%r13)
	movl	80(%rsp), %r13d
	addl	$1, %r13d
	movl	%r13d, 80(%rsp)
	movl	80(%rsp), %r10d
	cmpl	%r10d, %ebp
	jg	.L943
	.p2align 4,,10
.L895:
	movl	64(%rsp), %r14d
	addl	160(%rsp), %r14d
	movl	%r14d, 64(%rsp)
	movl	64(%rsp), %r10d
	cmpl	%r10d, 24(%rsp)
	jg	.L944
.L885:
	addq	$104, %rsp
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
	.p2align 4,,10
.L1524:
	.cfi_restore_state
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r14d
	cmpl	%r14d, %ebp
	jle	.L895
	.p2align 4,,10
.L1228:
	movslq	64(%rsp), %r10
	movslq	80(%rsp), %r9
	movslq	64(%rsp), %r11
	movq	(%rbx), %rdx
	movslq	80(%rsp), %r8
	movq	(%r12,%r10,8), %rcx
	movq	(%rdx,%r11,8), %rdi
	leaq	(%rcx,%r9,8), %r13
	movq	(%rdi,%r8,8), %rdi
	call	asmf1
	movl	80(%rsp), %esi
	movq	%rax, 0(%r13)
	addl	$1, %esi
	movl	%esi, 80(%rsp)
	movl	80(%rsp), %eax
	cmpl	%eax, %ebp
	jg	.L1228
	jmp	.L895
	.p2align 4,,10
.L1525:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %esi
	cmpl	%esi, %ebp
	jle	.L895
	.p2align 4,,10
.L898:
	movslq	64(%rsp), %rdi
	movslq	80(%rsp), %r13
	movslq	64(%rsp), %r10
	movslq	80(%rsp), %r9
	movslq	64(%rsp), %rcx
	movq	8(%rbx), %r8
	movslq	80(%rsp), %r11
	movq	(%r12,%rdi,8), %rax
	movq	(%rbx), %rdi
	movq	(%r8,%r10,8), %rdx
	leaq	(%rax,%r13,8), %r14
	movq	(%rdi,%rcx,8), %r13
	movq	(%rdx,%r9,8), %rsi
	movq	0(%r13,%r11,8), %rdi
	call	asmf2
	movl	80(%rsp), %esi
	movq	%rax, (%r14)
	addl	$1, %esi
	movl	%esi, 80(%rsp)
	movl	80(%rsp), %eax
	cmpl	%eax, %ebp
	jg	.L898
	jmp	.L895
	.p2align 4,,10
.L1526:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r13d
	cmpl	%r13d, %ebp
	jle	.L895
	.p2align 4,,10
.L900:
	movslq	64(%rsp), %r10
	movslq	80(%rsp), %r9
	movslq	64(%rsp), %r11
	movq	16(%rbx), %rax
	movslq	80(%rsp), %rdx
	movslq	64(%rsp), %r8
	movslq	80(%rsp), %rsi
	movslq	64(%rsp), %rdi
	movq	(%r12,%r10,8), %rcx
	movslq	80(%rsp), %r13
	movq	(%rax,%r11,8), %r10
	movq	(%rbx), %r11
	leaq	(%rcx,%r9,8), %r14
	movq	8(%rbx), %r9
	movq	(%r10,%rdx,8), %rdx
	movq	(%r9,%r8,8), %rcx
	movq	(%r11,%rdi,8), %r8
	movq	(%rcx,%rsi,8), %rsi
	movq	(%r8,%r13,8), %rdi
	call	asmf3
	movq	%rax, (%r14)
	movl	80(%rsp), %r14d
	addl	$1, %r14d
	movl	%r14d, 80(%rsp)
	movl	80(%rsp), %edx
	cmpl	%edx, %ebp
	jg	.L900
	jmp	.L895
.L1527:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r13d
	cmpl	%r13d, %ebp
	jle	.L895
	movabsq	$-9223372036854775808, %r13
	jmp	.L906
	.p2align 4,,10
.L1536:
	cvttsd2siq	%xmm0, %rax
	movq	%rax, (%r14)
.L905:
	movl	80(%rsp), %r11d
	addl	$1, %r11d
	movl	%r11d, 80(%rsp)
	movl	80(%rsp), %r14d
	cmpl	%r14d, %ebp
	jle	.L895
.L906:
	movslq	64(%rsp), %rax
	movslq	80(%rsp), %rsi
	movslq	64(%rsp), %r10
	movq	(%rbx), %rcx
	movslq	80(%rsp), %r9
	movq	(%r12,%rax,8), %r11
	movq	(%rcx,%r10,8), %rdx
	leaq	(%r11,%rsi,8), %r14
	movq	(%rdx,%r9,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm0
	js	.L1535
.L903:
	call	udf1
	comisd	.LC35(%rip), %xmm0
	jb	.L1536
	subsd	.LC35(%rip), %xmm0
	cvttsd2siq	%xmm0, %rsi
	movq	%rsi, (%r14)
	xorq	%r13, (%r14)
	jmp	.L905
	.p2align 4,,10
.L1535:
	movq	%r8, %rdi
	andl	$1, %r8d
	shrq	%rdi
	orq	%r8, %rdi
	cvtsi2sdq	%rdi, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L903
.L1523:
	cmpl	$11, %r12d
	je	.L888
	cmpl	$4, %r12d
	je	.L888
	cmpl	$12, %r12d
	jne	.L887
.L888:
	leaq	64(%rsp), %rdi
	xorl	%esi, %esi
	call	gettimeofday
	movq	40(%rsp), %r15
	movl	20(%rsp), %edx
	movl	%ebp, %esi
	movl	160(%rsp), %r9d
	movl	28(%rsp), %r8d
	movl	%r12d, %edi
	movl	24(%rsp), %ecx
	movq	%rbx, (%rsp)
	movq	%r15, 8(%rsp)
	call	loop_asm
	leaq	80(%rsp), %rdi
	xorl	%esi, %esi
	call	gettimeofday
	movq	80(%rsp), %rsi
	movq	88(%rsp), %rdx
	subq	64(%rsp), %rsi
	subq	72(%rsp), %rdx
	js	.L1537
.L889:
	movl	$.LC34, %edi
	xorl	%eax, %eax
	call	printf
	addq	$104, %rsp
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
.L1528:
	.cfi_restore_state
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r13d
	cmpl	%r13d, %ebp
	jle	.L895
	movabsq	$-9223372036854775808, %r13
	jmp	.L914
	.p2align 4,,10
.L1540:
	cvttsd2siq	%xmm0, %rdx
	movq	%rdx, (%r14)
.L913:
	movl	80(%rsp), %r14d
	addl	$1, %r14d
	movl	%r14d, 80(%rsp)
	movl	80(%rsp), %edi
	cmpl	%edi, %ebp
	jle	.L895
.L914:
	movslq	64(%rsp), %rax
	movslq	80(%rsp), %rsi
	movslq	64(%rsp), %r10
	movq	8(%rbx), %r8
	movslq	80(%rsp), %r9
	movslq	64(%rsp), %rcx
	movslq	80(%rsp), %rdx
	movq	(%r12,%rax,8), %r11
	movq	(%r8,%r10,8), %rdi
	leaq	(%r11,%rsi,8), %r14
	movq	(%rdi,%r9,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm1
	js	.L1538
.L909:
	movq	(%rbx), %r11
	movq	(%r11,%rcx,8), %r10
	movq	(%r10,%rdx,8), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm0
	js	.L1539
.L911:
	call	udf2
	comisd	.LC35(%rip), %xmm0
	jb	.L1540
	subsd	.LC35(%rip), %xmm0
	cvttsd2siq	%xmm0, %r8
	movq	%r8, (%r14)
	xorq	%r13, (%r14)
	jmp	.L913
	.p2align 4,,10
.L1539:
	movq	%r9, %rcx
	andl	$1, %r9d
	shrq	%rcx
	orq	%r9, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L911
	.p2align 4,,10
.L1538:
	movq	%rax, %rsi
	andl	$1, %eax
	shrq	%rsi
	orq	%rax, %rsi
	cvtsi2sdq	%rsi, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L909
.L1529:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r13d
	cmpl	%r13d, %ebp
	jle	.L895
	movabsq	$-9223372036854775808, %r13
	jmp	.L924
	.p2align 4,,10
.L1544:
	cvttsd2siq	%xmm0, %r9
	movq	%r9, (%r14)
.L923:
	movl	80(%rsp), %r14d
	addl	$1, %r14d
	movl	%r14d, 80(%rsp)
	movl	80(%rsp), %edi
	cmpl	%edi, %ebp
	jle	.L895
.L924:
	movslq	64(%rsp), %r9
	movslq	80(%rsp), %r11
	movq	16(%rbx), %rax
	movq	(%r12,%r9,8), %r10
	movslq	64(%rsp), %r9
	movslq	80(%rsp), %r8
	movslq	64(%rsp), %rdi
	movslq	80(%rsp), %rsi
	movslq	64(%rsp), %rcx
	leaq	(%r10,%r11,8), %r14
	movslq	80(%rsp), %rdx
	movq	(%rax,%r9,8), %r11
	movq	(%r11,%r8,8), %r10
	testq	%r10, %r10
	cvtsi2sdq	%r10, %xmm2
	js	.L1541
.L917:
	movq	8(%rbx), %r8
	movq	(%r8,%rdi,8), %rdi
	movq	(%rdi,%rsi,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm1
	js	.L1542
.L919:
	movq	(%rbx), %r11
	movq	(%r11,%rcx,8), %rcx
	movq	(%rcx,%rdx,8), %r10
	testq	%r10, %r10
	cvtsi2sdq	%r10, %xmm0
	js	.L1543
.L921:
	call	udf3
	comisd	.LC35(%rip), %xmm0
	jb	.L1544
	subsd	.LC35(%rip), %xmm0
	cvttsd2siq	%xmm0, %r8
	movq	%r8, (%r14)
	xorq	%r13, (%r14)
	jmp	.L923
	.p2align 4,,10
.L1543:
	movq	%r10, %rdx
	andl	$1, %r10d
	shrq	%rdx
	orq	%r10, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L921
	.p2align 4,,10
.L1542:
	movq	%rax, %rsi
	andl	$1, %eax
	shrq	%rsi
	orq	%rax, %rsi
	cvtsi2sdq	%rsi, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L919
	.p2align 4,,10
.L1541:
	movq	%r10, %r9
	andl	$1, %r10d
	shrq	%r9
	orq	%r10, %r9
	cvtsi2sdq	%r9, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L917
.L886:
	movl	$72, %edi
	leal	1(%rbp), %r13d
	call	malloc
	movl	$0, 64(%rsp)
	movq	%rax, %r12
	movl	64(%rsp), %eax
	movslq	%r13d, %r15
	salq	$3, %r15
	cmpl	$8, %eax
	jg	.L948
	.p2align 4,,10
.L1229:
	movq	%r15, %rdi
	movslq	64(%rsp), %r14
	call	malloc
	movl	64(%rsp), %edx
	movq	%rax, (%r12,%r14,8)
	addl	$1, %edx
	movl	%edx, 64(%rsp)
	movl	64(%rsp), %ecx
	cmpl	$8, %ecx
	jle	.L1229
.L948:
	cvtsi2sd	%ebp, %xmm0
	movslq	%ebp, %r15
	leaq	0(,%r15,8), %rsi
	movq	%rsi, %r13
	movq	%rsi, 32(%rsp)
	addq	(%r12), %r13
	movsd	%xmm0, 48(%rsp)
	call	rand
	cvtsi2sd	20(%rsp), %xmm2
	cvtsi2sd	24(%rsp), %xmm3
	movl	%eax, %edi
	movq	16(%r12), %r8
	cvtsi2sd	28(%rsp), %xmm4
	sarl	$31, %edi
	movq	32(%r12), %r9
	movq	40(%r12), %r10
	shrl	$8, %edi
	movq	48(%r12), %r11
	cvtsi2sd	160(%rsp), %xmm5
	addl	%edi, %eax
	andl	$16777215, %eax
	subl	%edi, %eax
	cvtsi2sd	%eax, %xmm1
	movl	28(%rsp), %eax
	movsd	%xmm1, 0(%r13)
	movsd	%xmm2, (%r8,%r15,8)
	movsd	%xmm3, (%r9,%r15,8)
	movsd	%xmm4, (%r10,%r15,8)
	movsd	%xmm5, (%r11,%r15,8)
	movl	%eax, 64(%rsp)
	movl	64(%rsp), %r14d
	cmpl	%r14d, 24(%rsp)
	jle	.L947
	movl	24(%rsp), %edx
	movl	20(%rsp), %ecx
	subl	$1, %edx
	subl	$1, %ecx
	movl	%edx, 56(%rsp)
	movl	%ecx, 60(%rsp)
	.p2align 4,,10
.L1231:
	movl	64(%rsp), %r15d
	movq	24(%r12), %rsi
	movq	32(%rsp), %r13
	cvtsi2sd	%r15d, %xmm6
	movsd	%xmm6, (%rsi,%r13)
	movl	$0, 80(%rsp)
	movl	80(%rsp), %edi
	cmpl	%edi, 20(%rsp)
	jle	.L997
	.p2align 4,,10
.L1309:
	movl	64(%rsp), %esi
	movl	80(%rsp), %r11d
	movl	$0, %r8d
	movl	64(%rsp), %r9d
	movl	80(%rsp), %r10d
	addl	$1, %r11d
	addl	$1, %esi
	subl	$1, %r9d
	subl	$1, %r10d
	cmpl	24(%rsp), %esi
	cmove	%r8d, %esi
	cmpl	20(%rsp), %r11d
	cmove	%r8d, %r11d
	cmpl	$-1, %r9d
	cmove	56(%rsp), %r9d
	cmpl	$-1, %r10d
	cmove	60(%rsp), %r10d
	testl	%ebp, %ebp
	jle	.L1545
	movslq	64(%rsp), %rdx
	movq	(%rbx), %r15
	leal	-1(%rbp), %r14d
	movslq	80(%rsp), %rcx
	movq	(%r12), %rax
	andl	$3, %r14d
	movq	(%r15,%rdx,8), %r13
	movq	0(%r13,%rcx,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm7
	js	.L1546
.L1488:
	cmpl	$1, %ebp
	movsd	%xmm7, (%rax)
	movl	$1, %edx
	jle	.L1212
	testl	%r14d, %r14d
	je	.L963
	cmpl	$1, %r14d
	je	.L1264
	cmpl	$2, %r14d
	je	.L1265
	movslq	64(%rsp), %r14
	movq	8(%rbx), %rcx
	movslq	80(%rsp), %r15
	movq	(%rcx,%r14,8), %r13
	movq	0(%r13,%r15,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm8
	js	.L1547
.L1490:
	movsd	%xmm8, 8(%rax)
	addq	$1, %rdx
.L1265:
	movslq	64(%rsp), %r14
	movq	(%rbx,%rdx,8), %rcx
	movslq	80(%rsp), %r15
	movq	(%rcx,%r14,8), %r13
	movq	0(%r13,%r15,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm9
	js	.L1548
.L1492:
	movsd	%xmm9, (%rax,%rdx,8)
	addq	$1, %rdx
.L1264:
	movslq	64(%rsp), %r14
	movq	(%rbx,%rdx,8), %rcx
	movslq	80(%rsp), %r15
	movq	(%rcx,%r14,8), %r13
	movq	0(%r13,%r15,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm10
	js	.L1549
.L1494:
	movsd	%xmm10, (%rax,%rdx,8)
	addq	$1, %rdx
	cmpl	%edx, %ebp
	jg	.L963
	jmp	.L1212
	.p2align 4,,10
.L961:
	movslq	64(%rsp), %r14
	movsd	%xmm11, (%rax,%rdx,8)
	addq	$1, %rdx
	movq	(%rbx,%rdx,8), %rcx
	movslq	80(%rsp), %r15
	movq	(%rcx,%r14,8), %r13
	movq	0(%r13,%r15,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm12
	js	.L1550
.L1495:
	leaq	1(%rdx), %r14
	movslq	64(%rsp), %r15
	movslq	80(%rsp), %r13
	movsd	%xmm12, (%rax,%rdx,8)
	movq	(%rbx,%r14,8), %rcx
	movq	(%rcx,%r15,8), %r8
	movq	(%r8,%r13,8), %rdi
	testq	%rdi, %rdi
	cvtsi2sdq	%rdi, %xmm13
	js	.L1551
.L1497:
	movsd	%xmm13, (%rax,%r14,8)
	leaq	2(%rdx), %r14
	movslq	64(%rsp), %r13
	movslq	80(%rsp), %r8
	movq	(%rbx,%r14,8), %rcx
	movq	(%rcx,%r13,8), %rdi
	movq	(%rdi,%r8,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm14
	js	.L1552
.L1499:
	addq	$3, %rdx
	movsd	%xmm14, (%rax,%r14,8)
	cmpl	%edx, %ebp
	jle	.L1212
.L963:
	movslq	64(%rsp), %r14
	movq	(%rbx,%rdx,8), %rcx
	movslq	80(%rsp), %r15
	movq	(%rcx,%r14,8), %r13
	movq	0(%r13,%r15,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm11
	jns	.L961
	movq	%r8, %rdi
	andl	$1, %r8d
	shrq	%rdi
	orq	%r8, %rdi
	cvtsi2sdq	%rdi, %xmm11
	addsd	%xmm11, %xmm11
	jmp	.L961
	.p2align 4,,10
.L1212:
	movq	(%rbx), %rax
	movslq	%r9d, %r9
	movslq	%r10d, %r10
	leal	-1(%rbp), %edx
	movq	8(%r12), %r13
	leaq	0(,%r9,8), %rcx
	leaq	0(,%r10,8), %rdi
	movq	(%rax,%r9,8), %r14
	andl	$7, %edx
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm15
	js	.L1553
.L1460:
	cmpl	$1, %ebp
	movsd	%xmm15, 0(%r13)
	movl	$1, %eax
	jle	.L1176
	testl	%edx, %edx
	je	.L967
	cmpl	$1, %edx
	je	.L1258
	cmpl	$2, %edx
	.p2align 4,,3
	je	.L1259
	cmpl	$3, %edx
	.p2align 4,,2
	je	.L1260
	cmpl	$4, %edx
	.p2align 4,,2
	je	.L1261
	cmpl	$5, %edx
	.p2align 4,,2
	je	.L1262
	cmpl	$6, %edx
	.p2align 4,,2
	je	.L1263
	movq	8(%rbx), %rdx
	movq	(%rdx,%r9,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm0
	js	.L1554
.L1462:
	movsd	%xmm0, 8(%r13)
	addq	$1, %rax
.L1263:
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%r9,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm1
	js	.L1555
.L1464:
	movsd	%xmm1, 0(%r13,%rax,8)
	addq	$1, %rax
.L1262:
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%r9,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm2
	js	.L1556
.L1466:
	movsd	%xmm2, 0(%r13,%rax,8)
	addq	$1, %rax
.L1261:
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%r9,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm3
	js	.L1557
.L1468:
	movsd	%xmm3, 0(%r13,%rax,8)
	addq	$1, %rax
.L1260:
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%r9,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm4
	js	.L1558
.L1470:
	movsd	%xmm4, 0(%r13,%rax,8)
	addq	$1, %rax
.L1259:
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%r9,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm5
	js	.L1559
.L1472:
	movsd	%xmm5, 0(%r13,%rax,8)
	addq	$1, %rax
.L1258:
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%r9,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm6
	js	.L1560
.L1474:
	movsd	%xmm6, 0(%r13,%rax,8)
	addq	$1, %rax
	cmpl	%eax, %ebp
	jg	.L967
	jmp	.L1176
	.p2align 4,,10
.L965:
	movsd	%xmm7, 0(%r13,%rax,8)
	addq	$1, %rax
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%rcx), %r14
	movq	(%r14,%rdi), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm8
	js	.L1561
.L1475:
	leaq	1(%rax), %r15
	movsd	%xmm8, 0(%r13,%rax,8)
	movq	(%rbx,%r15,8), %rdx
	movq	(%rdx,%rcx), %r14
	movq	(%r14,%rdi), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm9
	js	.L1562
.L1477:
	movsd	%xmm9, 0(%r13,%r15,8)
	leaq	2(%rax), %r15
	movq	(%rbx,%r15,8), %r14
	movq	(%r14,%rcx), %r8
	movq	(%r8,%rdi), %rdx
	testq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm10
	js	.L1563
.L1479:
	movsd	%xmm10, 0(%r13,%r15,8)
	leaq	3(%rax), %r15
	movq	(%rbx,%r15,8), %r8
	movq	(%r8,%rcx), %rdx
	movq	(%rdx,%rdi), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm11
	js	.L1564
.L1481:
	movsd	%xmm11, 0(%r13,%r15,8)
	leaq	4(%rax), %r15
	movq	(%rbx,%r15,8), %rdx
	movq	(%rdx,%rcx), %r8
	movq	(%r8,%rdi), %rdx
	testq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm12
	js	.L1565
.L1483:
	movsd	%xmm12, 0(%r13,%r15,8)
	leaq	5(%rax), %r15
	movq	(%rbx,%r15,8), %r8
	movq	(%r8,%rcx), %rdx
	movq	(%rdx,%rdi), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm13
	js	.L1566
.L1485:
	movsd	%xmm13, 0(%r13,%r15,8)
	leaq	6(%rax), %r15
	movq	(%rbx,%r15,8), %rdx
	movq	(%rdx,%rcx), %r8
	movq	(%r8,%rdi), %rdx
	testq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm14
	js	.L1567
.L1487:
	addq	$7, %rax
	movsd	%xmm14, 0(%r13,%r15,8)
	cmpl	%eax, %ebp
	jle	.L1176
.L967:
	movq	(%rbx,%rax,8), %rdx
	movq	(%rdx,%rcx), %r14
	movq	(%r14,%rdi), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm7
	jns	.L965
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm7
	addsd	%xmm7, %xmm7
	jmp	.L965
	.p2align 4,,10
.L1176:
	movq	(%rbx), %rax
	movslq	80(%rsp), %r8
	leal	-1(%rbp), %r15d
	movq	16(%r12), %rdx
	andl	$3, %r15d
	movq	(%rax,%r9,8), %r14
	movq	(%r14,%r8,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm15
	js	.L1568
.L1448:
	cmpl	$1, %ebp
	movsd	%xmm15, (%rdx)
	movl	$1, %eax
	jle	.L1160
	testl	%r15d, %r15d
	je	.L971
	cmpl	$1, %r15d
	je	.L1256
	cmpl	$2, %r15d
	je	.L1257
	movq	8(%rbx), %r14
	movslq	80(%rsp), %r15
	movq	(%r14,%r9,8), %r8
	movq	(%r8,%r15,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm0
	js	.L1569
.L1450:
	movsd	%xmm0, 8(%rdx)
	addq	$1, %rax
.L1257:
	movq	(%rbx,%rax,8), %r8
	movslq	80(%rsp), %r15
	movq	(%r8,%r9,8), %r14
	movq	(%r14,%r15,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm1
	js	.L1570
.L1452:
	movsd	%xmm1, (%rdx,%rax,8)
	addq	$1, %rax
.L1256:
	movq	(%rbx,%rax,8), %r15
	movslq	80(%rsp), %r14
	movq	(%r15,%r9,8), %r8
	movq	(%r8,%r14,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm2
	js	.L1571
.L1454:
	movsd	%xmm2, (%rdx,%rax,8)
	addq	$1, %rax
	cmpl	%eax, %ebp
	jg	.L971
	jmp	.L1160
	.p2align 4,,10
.L969:
	movsd	%xmm3, (%rdx,%rax,8)
	addq	$1, %rax
	movslq	80(%rsp), %r14
	movq	(%rbx,%rax,8), %r15
	movq	(%r15,%rcx), %r8
	movq	(%r8,%r14,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm4
	js	.L1572
.L1455:
	leaq	1(%rax), %r14
	movslq	80(%rsp), %r15
	movsd	%xmm4, (%rdx,%rax,8)
	movq	(%rbx,%r14,8), %r8
	movq	(%r8,%rcx), %r8
	movq	(%r8,%r15,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm5
	js	.L1573
.L1457:
	movsd	%xmm5, (%rdx,%r14,8)
	leaq	2(%rax), %r14
	movslq	80(%rsp), %r15
	movq	(%rbx,%r14,8), %r8
	movq	(%r8,%rcx), %r8
	movq	(%r8,%r15,8), %r8
	testq	%r8, %r8
	cvtsi2sdq	%r8, %xmm6
	js	.L1574
.L1459:
	addq	$3, %rax
	movsd	%xmm6, (%rdx,%r14,8)
	cmpl	%eax, %ebp
	jle	.L1160
.L971:
	movq	(%rbx,%rax,8), %r8
	movslq	80(%rsp), %r15
	movq	(%r8,%rcx), %r14
	movq	(%r14,%r15,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm3
	jns	.L969
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm3
	addsd	%xmm3, %xmm3
	jmp	.L969
	.p2align 4,,10
.L1160:
	movq	(%rbx), %rdx
	movslq	%r11d, %r11
	leal	-1(%rbp), %eax
	movq	24(%r12), %r14
	leaq	0(,%r11,8), %r8
	andl	$7, %eax
	movq	(%rdx,%r9,8), %r15
	movq	(%r15,%r11,8), %rdx
	testq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm7
	js	.L1575
.L1420:
	cmpl	$1, %ebp
	movsd	%xmm7, (%r14)
	movl	$1, %edx
	jle	.L1124
	testl	%eax, %eax
	je	.L975
	cmpl	$1, %eax
	je	.L1250
	cmpl	$2, %eax
	.p2align 4,,3
	je	.L1251
	cmpl	$3, %eax
	.p2align 4,,2
	je	.L1252
	cmpl	$4, %eax
	.p2align 4,,2
	je	.L1253
	cmpl	$5, %eax
	.p2align 4,,2
	je	.L1254
	cmpl	$6, %eax
	.p2align 4,,2
	je	.L1255
	movq	8(%rbx), %rax
	movq	(%rax,%r9,8), %r15
	movq	(%r15,%r11,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm8
	js	.L1576
.L1422:
	movsd	%xmm8, 8(%r14)
	addq	$1, %rdx
.L1255:
	movq	(%rbx,%rdx,8), %rax
	movq	(%rax,%r9,8), %r15
	movq	(%r15,%r11,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm9
	js	.L1577
.L1424:
	movsd	%xmm9, (%r14,%rdx,8)
	addq	$1, %rdx
.L1254:
	movq	(%rbx,%rdx,8), %rax
	movq	(%rax,%r9,8), %r15
	movq	(%r15,%r11,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm10
	js	.L1578
.L1426:
	movsd	%xmm10, (%r14,%rdx,8)
	addq	$1, %rdx
.L1253:
	movq	(%rbx,%rdx,8), %rax
	movq	(%rax,%r9,8), %r15
	movq	(%r15,%r11,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm11
	js	.L1579
.L1428:
	movsd	%xmm11, (%r14,%rdx,8)
	addq	$1, %rdx
.L1252:
	movq	(%rbx,%rdx,8), %rax
	movq	(%rax,%r9,8), %r15
	movq	(%r15,%r11,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm12
	js	.L1580
.L1430:
	movsd	%xmm12, (%r14,%rdx,8)
	addq	$1, %rdx
.L1251:
	movq	(%rbx,%rdx,8), %rax
	movq	(%rax,%r9,8), %r15
	movq	(%r15,%r11,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm13
	js	.L1581
.L1432:
	movsd	%xmm13, (%r14,%rdx,8)
	addq	$1, %rdx
.L1250:
	movq	(%rbx,%rdx,8), %rax
	movq	(%rax,%r9,8), %r9
	movq	(%r9,%r11,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm14
	js	.L1582
.L1434:
	movsd	%xmm14, (%r14,%rdx,8)
	addq	$1, %rdx
	cmpl	%edx, %ebp
	jg	.L975
	jmp	.L1124
	.p2align 4,,10
.L973:
	movsd	%xmm15, (%r14,%rdx,8)
	addq	$1, %rdx
	movq	(%rbx,%rdx,8), %r15
	movq	(%r15,%rcx), %rax
	movq	(%rax,%r8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm0
	js	.L1583
.L1435:
	leaq	1(%rdx), %r9
	movsd	%xmm0, (%r14,%rdx,8)
	movq	(%rbx,%r9,8), %rax
	movq	(%rax,%rcx), %r15
	movq	(%r15,%r8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm1
	js	.L1584
.L1437:
	movsd	%xmm1, (%r14,%r9,8)
	leaq	2(%rdx), %r9
	movq	(%rbx,%r9,8), %rax
	movq	(%rax,%rcx), %r15
	movq	(%r15,%r8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm2
	js	.L1585
.L1439:
	movsd	%xmm2, (%r14,%r9,8)
	leaq	3(%rdx), %r9
	movq	(%rbx,%r9,8), %rax
	movq	(%rax,%rcx), %r15
	movq	(%r15,%r8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm3
	js	.L1586
.L1441:
	movsd	%xmm3, (%r14,%r9,8)
	leaq	4(%rdx), %r9
	movq	(%rbx,%r9,8), %rax
	movq	(%rax,%rcx), %r15
	movq	(%r15,%r8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm4
	js	.L1587
.L1443:
	movsd	%xmm4, (%r14,%r9,8)
	leaq	5(%rdx), %r9
	movq	(%rbx,%r9,8), %rax
	movq	(%rax,%rcx), %r15
	movq	(%r15,%r8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm5
	js	.L1588
.L1445:
	movsd	%xmm5, (%r14,%r9,8)
	leaq	6(%rdx), %r9
	movq	(%rbx,%r9,8), %rax
	movq	(%rax,%rcx), %r15
	movq	(%r15,%r8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm6
	js	.L1589
.L1447:
	addq	$7, %rdx
	movsd	%xmm6, (%r14,%r9,8)
	cmpl	%edx, %ebp
	jle	.L1124
.L975:
	movq	(%rbx,%rdx,8), %r9
	movq	(%r9,%rcx), %r15
	movq	(%r15,%r8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm15
	jns	.L973
	movq	%rax, %r9
	andl	$1, %eax
	shrq	%r9
	orq	%rax, %r9
	cvtsi2sdq	%r9, %xmm15
	addsd	%xmm15, %xmm15
	jmp	.L973
	.p2align 4,,10
.L1124:
	movslq	64(%rsp), %r14
	movq	(%rbx), %rdx
	leal	-1(%rbp), %ecx
	movq	32(%r12), %rax
	andl	$3, %ecx
	movq	(%rdx,%r14,8), %r9
	movq	(%r9,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm7
	js	.L1590
.L1408:
	cmpl	$1, %ebp
	movsd	%xmm7, (%rax)
	movl	$1, %edx
	jle	.L1108
	testl	%ecx, %ecx
	je	.L979
	cmpl	$1, %ecx
	je	.L1248
	cmpl	$2, %ecx
	.p2align 4,,3
	je	.L1249
	movslq	64(%rsp), %r9
	movq	8(%rbx), %rcx
	movq	(%rcx,%r9,8), %r15
	movq	(%r15,%r10,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm8
	js	.L1591
.L1410:
	movsd	%xmm8, 8(%rax)
	addq	$1, %rdx
.L1249:
	movslq	64(%rsp), %r15
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r15,8), %r14
	movq	(%r14,%r10,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm9
	js	.L1592
.L1412:
	movsd	%xmm9, (%rax,%rdx,8)
	addq	$1, %rdx
.L1248:
	movslq	64(%rsp), %r14
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r14,8), %r15
	movq	(%r15,%r10,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm10
	js	.L1593
.L1414:
	movsd	%xmm10, (%rax,%rdx,8)
	addq	$1, %rdx
	cmpl	%edx, %ebp
	jg	.L979
	jmp	.L1108
	.p2align 4,,10
.L977:
	movslq	64(%rsp), %r14
	movsd	%xmm11, (%rax,%rdx,8)
	addq	$1, %rdx
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r14,8), %r15
	movq	(%r15,%rdi), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm12
	js	.L1594
.L1415:
	leaq	1(%rdx), %r15
	movslq	64(%rsp), %r14
	movsd	%xmm12, (%rax,%rdx,8)
	movq	(%rbx,%r15,8), %rcx
	movq	(%rcx,%r14,8), %r9
	movq	(%r9,%rdi), %rcx
	testq	%rcx, %rcx
	cvtsi2sdq	%rcx, %xmm13
	js	.L1595
.L1417:
	movsd	%xmm13, (%rax,%r15,8)
	leaq	2(%rdx), %r15
	movslq	64(%rsp), %r9
	movq	(%rbx,%r15,8), %rcx
	movq	(%rcx,%r9,8), %r14
	movq	(%r14,%rdi), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm14
	js	.L1596
.L1419:
	addq	$3, %rdx
	movsd	%xmm14, (%rax,%r15,8)
	cmpl	%edx, %ebp
	jle	.L1108
.L979:
	movslq	64(%rsp), %r15
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r15,8), %r14
	movq	(%r14,%rdi), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm11
	jns	.L977
	movq	%r15, %r9
	andl	$1, %r15d
	shrq	%r9
	orq	%r15, %r9
	cvtsi2sdq	%r9, %xmm11
	addsd	%xmm11, %xmm11
	jmp	.L977
	.p2align 4,,10
.L1108:
	movslq	64(%rsp), %r14
	movq	(%rbx), %rdx
	leal	-1(%rbp), %r15d
	movq	40(%r12), %rax
	andl	$3, %r15d
	movq	(%rdx,%r14,8), %r9
	movq	(%r9,%r11,8), %rcx
	testq	%rcx, %rcx
	cvtsi2sdq	%rcx, %xmm15
	js	.L1597
.L1396:
	cmpl	$1, %ebp
	movsd	%xmm15, (%rax)
	movl	$1, %edx
	jle	.L1092
	testl	%r15d, %r15d
	je	.L983
	cmpl	$1, %r15d
	je	.L1246
	cmpl	$2, %r15d
	je	.L1247
	movslq	64(%rsp), %r15
	movq	8(%rbx), %r9
	movq	(%r9,%r15,8), %rcx
	movq	(%rcx,%r11,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm0
	js	.L1598
.L1398:
	movsd	%xmm0, 8(%rax)
	addq	$1, %rdx
.L1247:
	movslq	64(%rsp), %r9
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r9,8), %r14
	movq	(%r14,%r11,8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm1
	js	.L1599
.L1400:
	movsd	%xmm1, (%rax,%rdx,8)
	addq	$1, %rdx
.L1246:
	movslq	64(%rsp), %r14
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r14,8), %r15
	movq	(%r15,%r11,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm2
	js	.L1600
.L1402:
	movsd	%xmm2, (%rax,%rdx,8)
	addq	$1, %rdx
	cmpl	%edx, %ebp
	jg	.L983
	jmp	.L1092
	.p2align 4,,10
.L981:
	movslq	64(%rsp), %r14
	movsd	%xmm3, (%rax,%rdx,8)
	addq	$1, %rdx
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r14,8), %r15
	movq	(%r15,%r8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm4
	js	.L1601
.L1403:
	leaq	1(%rdx), %r15
	movslq	64(%rsp), %r14
	movsd	%xmm4, (%rax,%rdx,8)
	movq	(%rbx,%r15,8), %rcx
	movq	(%rcx,%r14,8), %r9
	movq	(%r9,%r8), %rcx
	testq	%rcx, %rcx
	cvtsi2sdq	%rcx, %xmm5
	js	.L1602
.L1405:
	movsd	%xmm5, (%rax,%r15,8)
	leaq	2(%rdx), %r15
	movslq	64(%rsp), %r9
	movq	(%rbx,%r15,8), %rcx
	movq	(%rcx,%r9,8), %r14
	movq	(%r14,%r8), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm6
	js	.L1603
.L1407:
	addq	$3, %rdx
	movsd	%xmm6, (%rax,%r15,8)
	cmpl	%edx, %ebp
	jle	.L1092
.L983:
	movslq	64(%rsp), %r15
	movq	(%rbx,%rdx,8), %rcx
	movq	(%rcx,%r15,8), %r14
	movq	(%r14,%r8), %r15
	testq	%r15, %r15
	cvtsi2sdq	%r15, %xmm3
	jns	.L981
	movq	%r15, %r9
	andl	$1, %r15d
	shrq	%r9
	orq	%r15, %r9
	cvtsi2sdq	%r9, %xmm3
	addsd	%xmm3, %xmm3
	jmp	.L981
	.p2align 4,,10
.L1092:
	movq	(%rbx), %rax
	movslq	%esi, %rsi
	leal	-1(%rbp), %r9d
	movq	48(%r12), %r15
	leaq	0(,%rsi,8), %rdx
	andl	$7, %r9d
	movq	(%rax,%rsi,8), %r14
	movq	(%r14,%r10,8), %rcx
	testq	%rcx, %rcx
	cvtsi2sdq	%rcx, %xmm7
	js	.L1604
.L1368:
	cmpl	$1, %ebp
	movsd	%xmm7, (%r15)
	movl	$1, %ecx
	jle	.L1056
	testl	%r9d, %r9d
	je	.L987
	cmpl	$1, %r9d
	je	.L1240
	cmpl	$2, %r9d
	je	.L1241
	cmpl	$3, %r9d
	je	.L1242
	cmpl	$4, %r9d
	je	.L1243
	cmpl	$5, %r9d
	je	.L1244
	cmpl	$6, %r9d
	je	.L1245
	movq	8(%rbx), %r9
	movq	(%r9,%rsi,8), %r14
	movq	(%r14,%r10,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm8
	js	.L1605
.L1370:
	movsd	%xmm8, 8(%r15)
	addq	$1, %rcx
.L1245:
	movq	(%rbx,%rcx,8), %r14
	movq	(%r14,%rsi,8), %rax
	movq	(%rax,%r10,8), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm9
	js	.L1606
.L1372:
	movsd	%xmm9, (%r15,%rcx,8)
	addq	$1, %rcx
.L1244:
	movq	(%rbx,%rcx,8), %rax
	movq	(%rax,%rsi,8), %r9
	movq	(%r9,%r10,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm10
	js	.L1607
.L1374:
	movsd	%xmm10, (%r15,%rcx,8)
	addq	$1, %rcx
.L1243:
	movq	(%rbx,%rcx,8), %r9
	movq	(%r9,%rsi,8), %rax
	movq	(%rax,%r10,8), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm11
	js	.L1608
.L1376:
	movsd	%xmm11, (%r15,%rcx,8)
	addq	$1, %rcx
.L1242:
	movq	(%rbx,%rcx,8), %rax
	movq	(%rax,%rsi,8), %r9
	movq	(%r9,%r10,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm12
	js	.L1609
.L1378:
	movsd	%xmm12, (%r15,%rcx,8)
	addq	$1, %rcx
.L1241:
	movq	(%rbx,%rcx,8), %r9
	movq	(%r9,%rsi,8), %rax
	movq	(%rax,%r10,8), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm13
	js	.L1610
.L1380:
	movsd	%xmm13, (%r15,%rcx,8)
	addq	$1, %rcx
.L1240:
	movq	(%rbx,%rcx,8), %rax
	movq	(%rax,%rsi,8), %r9
	movq	(%r9,%r10,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm14
	js	.L1611
.L1382:
	movsd	%xmm14, (%r15,%rcx,8)
	addq	$1, %rcx
	cmpl	%ecx, %ebp
	jg	.L987
	jmp	.L1056
	.p2align 4,,10
.L985:
	movsd	%xmm15, (%r15,%rcx,8)
	addq	$1, %rcx
	movq	(%rbx,%rcx,8), %rax
	movq	(%rax,%rdx), %r9
	movq	(%r9,%rdi), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm0
	js	.L1612
.L1383:
	leaq	1(%rcx), %rax
	movsd	%xmm0, (%r15,%rcx,8)
	movq	(%rbx,%rax,8), %r9
	movq	(%r9,%rdx), %r14
	movq	(%r14,%rdi), %r10
	testq	%r10, %r10
	cvtsi2sdq	%r10, %xmm1
	js	.L1613
.L1385:
	movsd	%xmm1, (%r15,%rax,8)
	leaq	2(%rcx), %rax
	movq	(%rbx,%rax,8), %r14
	movq	(%r14,%rdx), %r10
	movq	(%r10,%rdi), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm2
	js	.L1614
.L1387:
	movsd	%xmm2, (%r15,%rax,8)
	leaq	3(%rcx), %rax
	movq	(%rbx,%rax,8), %r10
	movq	(%r10,%rdx), %r9
	movq	(%r9,%rdi), %r10
	testq	%r10, %r10
	cvtsi2sdq	%r10, %xmm3
	js	.L1615
.L1389:
	movsd	%xmm3, (%r15,%rax,8)
	leaq	4(%rcx), %rax
	movq	(%rbx,%rax,8), %r9
	movq	(%r9,%rdx), %r10
	movq	(%r10,%rdi), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm4
	js	.L1616
.L1391:
	movsd	%xmm4, (%r15,%rax,8)
	leaq	5(%rcx), %rax
	movq	(%rbx,%rax,8), %r10
	movq	(%r10,%rdx), %r9
	movq	(%r9,%rdi), %r10
	testq	%r10, %r10
	cvtsi2sdq	%r10, %xmm5
	js	.L1617
.L1393:
	movsd	%xmm5, (%r15,%rax,8)
	leaq	6(%rcx), %rax
	movq	(%rbx,%rax,8), %r9
	movq	(%r9,%rdx), %r10
	movq	(%r10,%rdi), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm6
	js	.L1618
.L1395:
	addq	$7, %rcx
	movsd	%xmm6, (%r15,%rax,8)
	cmpl	%ecx, %ebp
	jle	.L1056
.L987:
	movq	(%rbx,%rcx,8), %rax
	movq	(%rax,%rdx), %r9
	movq	(%r9,%rdi), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm15
	jns	.L985
	movq	%r14, %r10
	andl	$1, %r14d
	shrq	%r10
	orq	%r14, %r10
	cvtsi2sdq	%r10, %xmm15
	addsd	%xmm15, %xmm15
	jmp	.L985
	.p2align 4,,10
.L1056:
	movq	(%rbx), %rcx
	movslq	80(%rsp), %rax
	leal	-1(%rbp), %edi
	movq	56(%r12), %r15
	andl	$3, %edi
	movq	(%rcx,%rsi,8), %r10
	movq	(%r10,%rax,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm7
	js	.L1619
.L1356:
	cmpl	$1, %ebp
	movsd	%xmm7, (%r15)
	movl	$1, %eax
	jle	.L1040
	testl	%edi, %edi
	je	.L991
	cmpl	$1, %edi
	je	.L1238
	cmpl	$2, %edi
	.p2align 4,,3
	je	.L1239
	movq	8(%rbx), %rdi
	movslq	80(%rsp), %rcx
	movq	(%rdi,%rsi,8), %r10
	movq	(%r10,%rcx,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm8
	js	.L1620
.L1358:
	movsd	%xmm8, 8(%r15)
	addq	$1, %rax
.L1239:
	movq	(%rbx,%rax,8), %rdi
	movslq	80(%rsp), %rcx
	movq	(%rdi,%rsi,8), %r10
	movq	(%r10,%rcx,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm9
	js	.L1621
.L1360:
	movsd	%xmm9, (%r15,%rax,8)
	addq	$1, %rax
.L1238:
	movq	(%rbx,%rax,8), %rdi
	movslq	80(%rsp), %rcx
	movq	(%rdi,%rsi,8), %r10
	movq	(%r10,%rcx,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm10
	js	.L1622
.L1362:
	movsd	%xmm10, (%r15,%rax,8)
	addq	$1, %rax
	cmpl	%eax, %ebp
	jg	.L991
	jmp	.L1040
	.p2align 4,,10
.L989:
	movsd	%xmm11, (%r15,%rax,8)
	addq	$1, %rax
	movslq	80(%rsp), %rcx
	movq	(%rbx,%rax,8), %rdi
	movq	(%rdi,%rdx), %r10
	movq	(%r10,%rcx,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm12
	js	.L1623
.L1363:
	leaq	1(%rax), %rcx
	movslq	80(%rsp), %r10
	movsd	%xmm12, (%r15,%rax,8)
	movq	(%rbx,%rcx,8), %rdi
	movq	(%rdi,%rdx), %r14
	movq	(%r14,%r10,8), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm13
	js	.L1624
.L1365:
	movsd	%xmm13, (%r15,%rcx,8)
	leaq	2(%rax), %rcx
	movslq	80(%rsp), %r14
	movq	(%rbx,%rcx,8), %rdi
	movq	(%rdi,%rdx), %r9
	movq	(%r9,%r14,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm14
	js	.L1625
.L1367:
	addq	$3, %rax
	movsd	%xmm14, (%r15,%rcx,8)
	cmpl	%eax, %ebp
	jle	.L1040
.L991:
	movq	(%rbx,%rax,8), %rdi
	movslq	80(%rsp), %rcx
	movq	(%rdi,%rdx), %r10
	movq	(%r10,%rcx,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm11
	jns	.L989
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm11
	addsd	%xmm11, %xmm11
	jmp	.L989
	.p2align 4,,10
.L1040:
	movq	(%rbx), %rax
	leal	-1(%rbp), %ecx
	movq	64(%r12), %r15
	andl	$7, %ecx
	movq	(%rax,%rsi,8), %rdi
	movq	(%rdi,%r11,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm15
	js	.L1626
.L1328:
	cmpl	$1, %ebp
	movsd	%xmm15, (%r15)
	movl	$1, %r10d
	jle	.L959
	testl	%ecx, %ecx
	je	.L994
	cmpl	$1, %ecx
	je	.L1232
	cmpl	$2, %ecx
	.p2align 4,,3
	je	.L1233
	cmpl	$3, %ecx
	.p2align 4,,2
	je	.L1234
	cmpl	$4, %ecx
	.p2align 4,,2
	je	.L1235
	cmpl	$5, %ecx
	.p2align 4,,2
	je	.L1236
	cmpl	$6, %ecx
	.p2align 4,,2
	je	.L1237
	movq	8(%rbx), %rcx
	movq	(%rcx,%rsi,8), %rax
	movq	(%rax,%r11,8), %rdi
	testq	%rdi, %rdi
	cvtsi2sdq	%rdi, %xmm0
	js	.L1627
.L1330:
	movsd	%xmm0, 8(%r15)
	addq	$1, %r10
.L1237:
	movq	(%rbx,%r10,8), %r9
	movq	(%r9,%rsi,8), %rcx
	movq	(%rcx,%r11,8), %rax
	testq	%rax, %rax
	cvtsi2sdq	%rax, %xmm1
	js	.L1628
.L1332:
	movsd	%xmm1, (%r15,%r10,8)
	addq	$1, %r10
.L1236:
	movq	(%rbx,%r10,8), %r14
	movq	(%r14,%rsi,8), %r9
	movq	(%r9,%r11,8), %rcx
	testq	%rcx, %rcx
	cvtsi2sdq	%rcx, %xmm2
	js	.L1629
.L1334:
	movsd	%xmm2, (%r15,%r10,8)
	addq	$1, %r10
.L1235:
	movq	(%rbx,%r10,8), %rdi
	movq	(%rdi,%rsi,8), %r14
	movq	(%r14,%r11,8), %rcx
	testq	%rcx, %rcx
	cvtsi2sdq	%rcx, %xmm3
	js	.L1630
.L1336:
	movsd	%xmm3, (%r15,%r10,8)
	addq	$1, %r10
.L1234:
	movq	(%rbx,%r10,8), %rax
	movq	(%rax,%rsi,8), %rdi
	movq	(%rdi,%r11,8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm4
	js	.L1631
.L1338:
	movsd	%xmm4, (%r15,%r10,8)
	addq	$1, %r10
.L1233:
	movq	(%rbx,%r10,8), %r9
	movq	(%r9,%rsi,8), %rax
	movq	(%rax,%r11,8), %rdi
	testq	%rdi, %rdi
	cvtsi2sdq	%rdi, %xmm5
	js	.L1632
.L1340:
	movsd	%xmm5, (%r15,%r10,8)
	addq	$1, %r10
.L1232:
	movq	(%rbx,%r10,8), %rcx
	movq	(%rcx,%rsi,8), %rsi
	movq	(%rsi,%r11,8), %r11
	testq	%r11, %r11
	cvtsi2sdq	%r11, %xmm6
	js	.L1633
.L1342:
	movsd	%xmm6, (%r15,%r10,8)
	addq	$1, %r10
	cmpl	%r10d, %ebp
	jg	.L994
	jmp	.L959
	.p2align 4,,10
.L993:
	movsd	%xmm7, (%r15,%r10,8)
	addq	$1, %r10
	movq	(%rbx,%r10,8), %rsi
	movq	(%rsi,%rdx), %r11
	movq	(%r11,%r8), %r9
	testq	%r9, %r9
	cvtsi2sdq	%r9, %xmm8
	js	.L1634
.L1343:
	leaq	1(%r10), %rdi
	movsd	%xmm8, (%r15,%r10,8)
	movq	(%rbx,%rdi,8), %r14
	movq	(%r14,%rdx), %rcx
	movq	(%rcx,%r8), %rsi
	testq	%rsi, %rsi
	cvtsi2sdq	%rsi, %xmm9
	js	.L1635
.L1345:
	leaq	2(%r10), %rax
	movsd	%xmm9, (%r15,%rdi,8)
	movq	(%rbx,%rax,8), %r9
	movq	(%r9,%rdx), %rdi
	movq	(%rdi,%r8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm10
	js	.L1636
.L1347:
	leaq	3(%r10), %r11
	movsd	%xmm10, (%r15,%rax,8)
	movq	(%rbx,%r11,8), %rsi
	movq	(%rsi,%rdx), %rax
	movq	(%rax,%r8), %rdi
	testq	%rdi, %rdi
	cvtsi2sdq	%rdi, %xmm11
	js	.L1637
.L1349:
	leaq	4(%r10), %r14
	movsd	%xmm11, (%r15,%r11,8)
	movq	(%rbx,%r14,8), %rcx
	movq	(%rcx,%rdx), %r11
	movq	(%r11,%r8), %rsi
	testq	%rsi, %rsi
	cvtsi2sdq	%rsi, %xmm12
	js	.L1638
.L1351:
	leaq	5(%r10), %rdi
	movsd	%xmm12, (%r15,%r14,8)
	movq	(%rbx,%rdi,8), %r9
	movq	(%r9,%rdx), %r14
	movq	(%r14,%r8), %rcx
	testq	%rcx, %rcx
	cvtsi2sdq	%rcx, %xmm13
	js	.L1639
.L1353:
	leaq	6(%r10), %rax
	movsd	%xmm13, (%r15,%rdi,8)
	movq	(%rbx,%rax,8), %rsi
	movq	(%rsi,%rdx), %rdi
	movq	(%rdi,%r8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm14
	js	.L1640
.L1355:
	addq	$7, %r10
	movsd	%xmm14, (%r15,%rax,8)
	cmpl	%r10d, %ebp
	jle	.L959
.L994:
	movq	(%rbx,%r10,8), %rax
	movq	(%rax,%rdx), %rdi
	movq	(%rdi,%r8), %r14
	testq	%r14, %r14
	cvtsi2sdq	%r14, %xmm7
	jns	.L993
	movq	%r14, %rcx
	andl	$1, %r14d
	shrq	%rcx
	orq	%r14, %rcx
	cvtsi2sdq	%rcx, %xmm7
	addsd	%xmm7, %xmm7
	jmp	.L993
	.p2align 4,,10
.L1545:
	movq	8(%r12), %r13
.L959:
	movl	80(%rsp), %r8d
	movq	32(%rsp), %rdx
	movq	%r12, %rsi
	movq	40(%rsp), %r10
	movl	28(%rsp), %edi
	movsd	48(%rsp), %xmm0
	cvtsi2sd	%r8d, %xmm15
	movsd	%xmm15, 0(%r13,%rdx)
	movslq	64(%rsp), %r13
	movslq	80(%rsp), %r15
	movq	(%r10,%r13,8), %rcx
	leaq	(%rcx,%r15,8), %r14
	call	fpar_f
	comisd	.LC35(%rip), %xmm0
	jae	.L995
	cvttsd2siq	%xmm0, %r11
	movq	%r11, (%r14)
.L996:
	movl	80(%rsp), %edi
	addl	$1, %edi
	movl	%edi, 80(%rsp)
	movl	80(%rsp), %r9d
	cmpl	%r9d, 20(%rsp)
	jg	.L1309
.L997:
	movl	64(%rsp), %r8d
	addl	160(%rsp), %r8d
	movl	%r8d, 64(%rsp)
	movl	64(%rsp), %edx
	cmpl	%edx, 24(%rsp)
	jg	.L1231
.L947:
	movl	$0, 64(%rsp)
	movl	64(%rsp), %ebx
	cmpl	$8, %ebx
	jg	.L951
	.p2align 4,,10
.L1230:
	movslq	64(%rsp), %rbp
	movq	(%r12,%rbp,8), %rdi
	call	free
	movl	64(%rsp), %r13d
	addl	$1, %r13d
	movl	%r13d, 64(%rsp)
	movl	64(%rsp), %r15d
	cmpl	$8, %r15d
	jle	.L1230
.L951:
	movq	%r12, %rdi
	call	free
	addq	$104, %rsp
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
	.p2align 4,,10
.L995:
	.cfi_restore_state
	subsd	.LC35(%rip), %xmm0
	movabsq	$-9223372036854775808, %rsi
	cvttsd2siq	%xmm0, %rax
	movq	%rax, (%r14)
	xorq	%rsi, (%r14)
	jmp	.L996
	.p2align 4,,10
.L1624:
	movq	%r9, %r10
	andl	$1, %r9d
	shrq	%r10
	orq	%r9, %r10
	cvtsi2sdq	%r10, %xmm13
	addsd	%xmm13, %xmm13
	jmp	.L1365
	.p2align 4,,10
.L1623:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L1363
	.p2align 4,,10
.L1625:
	movq	%r14, %r10
	andl	$1, %r14d
	shrq	%r10
	orq	%r14, %r10
	cvtsi2sdq	%r10, %xmm14
	addsd	%xmm14, %xmm14
	jmp	.L1367
	.p2align 4,,10
.L1602:
	movq	%rcx, %r14
	andl	$1, %ecx
	shrq	%r14
	orq	%rcx, %r14
	cvtsi2sdq	%r14, %xmm5
	addsd	%xmm5, %xmm5
	jmp	.L1405
	.p2align 4,,10
.L1601:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm4
	addsd	%xmm4, %xmm4
	jmp	.L1403
	.p2align 4,,10
.L1603:
	movq	%r9, %rcx
	andl	$1, %r9d
	shrq	%rcx
	orq	%r9, %rcx
	cvtsi2sdq	%rcx, %xmm6
	addsd	%xmm6, %xmm6
	jmp	.L1407
	.p2align 4,,10
.L1596:
	movq	%r9, %rcx
	andl	$1, %r9d
	shrq	%rcx
	orq	%r9, %rcx
	cvtsi2sdq	%rcx, %xmm14
	addsd	%xmm14, %xmm14
	jmp	.L1419
	.p2align 4,,10
.L1595:
	movq	%rcx, %r14
	andl	$1, %ecx
	shrq	%r14
	orq	%rcx, %r14
	cvtsi2sdq	%r14, %xmm13
	addsd	%xmm13, %xmm13
	jmp	.L1417
	.p2align 4,,10
.L1594:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L1415
	.p2align 4,,10
.L1552:
	movq	%r15, %r13
	andl	$1, %r15d
	shrq	%r13
	orq	%r15, %r13
	cvtsi2sdq	%r13, %xmm14
	addsd	%xmm14, %xmm14
	jmp	.L1499
	.p2align 4,,10
.L1551:
	movq	%rdi, %r15
	andl	$1, %edi
	shrq	%r15
	orq	%rdi, %r15
	cvtsi2sdq	%r15, %xmm13
	addsd	%xmm13, %xmm13
	jmp	.L1497
	.p2align 4,,10
.L1550:
	movq	%r8, %rdi
	andl	$1, %r8d
	shrq	%rdi
	orq	%r8, %rdi
	cvtsi2sdq	%rdi, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L1495
	.p2align 4,,10
.L1574:
	movq	%r8, %r15
	andl	$1, %r8d
	shrq	%r15
	orq	%r8, %r15
	cvtsi2sdq	%r15, %xmm6
	addsd	%xmm6, %xmm6
	jmp	.L1459
	.p2align 4,,10
.L1573:
	movq	%r8, %r15
	andl	$1, %r8d
	shrq	%r15
	orq	%r8, %r15
	cvtsi2sdq	%r15, %xmm5
	addsd	%xmm5, %xmm5
	jmp	.L1457
	.p2align 4,,10
.L1572:
	movq	%r15, %r14
	andl	$1, %r15d
	shrq	%r14
	orq	%r15, %r14
	cvtsi2sdq	%r14, %xmm4
	addsd	%xmm4, %xmm4
	jmp	.L1455
	.p2align 4,,10
.L1589:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm6
	addsd	%xmm6, %xmm6
	jmp	.L1447
	.p2align 4,,10
.L1588:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm5
	addsd	%xmm5, %xmm5
	jmp	.L1445
	.p2align 4,,10
.L1587:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm4
	addsd	%xmm4, %xmm4
	jmp	.L1443
	.p2align 4,,10
.L1586:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm3
	addsd	%xmm3, %xmm3
	jmp	.L1441
	.p2align 4,,10
.L1585:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L1439
	.p2align 4,,10
.L1584:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L1437
	.p2align 4,,10
.L1583:
	movq	%r15, %r9
	andl	$1, %r15d
	shrq	%r9
	orq	%r15, %r9
	cvtsi2sdq	%r9, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1435
	.p2align 4,,10
.L1566:
	movq	%r8, %r14
	andl	$1, %r8d
	shrq	%r14
	orq	%r8, %r14
	cvtsi2sdq	%r14, %xmm13
	addsd	%xmm13, %xmm13
	jmp	.L1485
	.p2align 4,,10
.L1565:
	movq	%rdx, %r14
	andl	$1, %edx
	shrq	%r14
	orq	%rdx, %r14
	cvtsi2sdq	%r14, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L1483
	.p2align 4,,10
.L1564:
	movq	%r8, %r14
	andl	$1, %r8d
	shrq	%r14
	orq	%r8, %r14
	cvtsi2sdq	%r14, %xmm11
	addsd	%xmm11, %xmm11
	jmp	.L1481
	.p2align 4,,10
.L1563:
	movq	%rdx, %r14
	andl	$1, %edx
	shrq	%r14
	orq	%rdx, %r14
	cvtsi2sdq	%r14, %xmm10
	addsd	%xmm10, %xmm10
	jmp	.L1479
	.p2align 4,,10
.L1562:
	movq	%r8, %rdx
	andl	$1, %r8d
	shrq	%rdx
	orq	%r8, %rdx
	cvtsi2sdq	%rdx, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L1477
	.p2align 4,,10
.L1561:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm8
	addsd	%xmm8, %xmm8
	jmp	.L1475
	.p2align 4,,10
.L1567:
	movq	%rdx, %r14
	andl	$1, %edx
	shrq	%r14
	orq	%rdx, %r14
	cvtsi2sdq	%r14, %xmm14
	addsd	%xmm14, %xmm14
	jmp	.L1487
	.p2align 4,,10
.L1618:
	movq	%r9, %r14
	andl	$1, %r9d
	shrq	%r14
	orq	%r9, %r14
	cvtsi2sdq	%r14, %xmm6
	addsd	%xmm6, %xmm6
	jmp	.L1395
	.p2align 4,,10
.L1617:
	movq	%r10, %r14
	andl	$1, %r10d
	shrq	%r14
	orq	%r10, %r14
	cvtsi2sdq	%r14, %xmm5
	addsd	%xmm5, %xmm5
	jmp	.L1393
	.p2align 4,,10
.L1616:
	movq	%r9, %r14
	andl	$1, %r9d
	shrq	%r14
	orq	%r9, %r14
	cvtsi2sdq	%r14, %xmm4
	addsd	%xmm4, %xmm4
	jmp	.L1391
	.p2align 4,,10
.L1615:
	movq	%r10, %r14
	andl	$1, %r10d
	shrq	%r14
	orq	%r10, %r14
	cvtsi2sdq	%r14, %xmm3
	addsd	%xmm3, %xmm3
	jmp	.L1389
	.p2align 4,,10
.L1614:
	movq	%r9, %r14
	andl	$1, %r9d
	shrq	%r14
	orq	%r9, %r14
	cvtsi2sdq	%r14, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L1387
	.p2align 4,,10
.L1613:
	movq	%r10, %r9
	andl	$1, %r10d
	shrq	%r9
	orq	%r10, %r9
	cvtsi2sdq	%r9, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L1385
	.p2align 4,,10
.L1612:
	movq	%r14, %r10
	andl	$1, %r14d
	shrq	%r10
	orq	%r14, %r10
	cvtsi2sdq	%r10, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1383
	.p2align 4,,10
.L1640:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm14
	addsd	%xmm14, %xmm14
	jmp	.L1355
	.p2align 4,,10
.L1639:
	movq	%rcx, %r11
	andl	$1, %ecx
	shrq	%r11
	orq	%rcx, %r11
	cvtsi2sdq	%r11, %xmm13
	addsd	%xmm13, %xmm13
	jmp	.L1353
	.p2align 4,,10
.L1638:
	movq	%rsi, %rax
	andl	$1, %esi
	shrq	%rax
	orq	%rsi, %rax
	cvtsi2sdq	%rax, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L1351
	.p2align 4,,10
.L1637:
	movq	%rdi, %r9
	andl	$1, %edi
	shrq	%r9
	orq	%rdi, %r9
	cvtsi2sdq	%r9, %xmm11
	addsd	%xmm11, %xmm11
	jmp	.L1349
	.p2align 4,,10
.L1636:
	movq	%r14, %rcx
	andl	$1, %r14d
	shrq	%rcx
	orq	%r14, %rcx
	cvtsi2sdq	%rcx, %xmm10
	addsd	%xmm10, %xmm10
	jmp	.L1347
	.p2align 4,,10
.L1635:
	movq	%rsi, %r11
	andl	$1, %esi
	shrq	%r11
	orq	%rsi, %r11
	cvtsi2sdq	%r11, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L1345
	.p2align 4,,10
.L1634:
	movq	%r9, %rax
	andl	$1, %r9d
	shrq	%rax
	orq	%r9, %rax
	cvtsi2sdq	%rax, %xmm8
	addsd	%xmm8, %xmm8
	jmp	.L1343
	.p2align 4,,10
.L1553:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm15
	addsd	%xmm15, %xmm15
	jmp	.L1460
	.p2align 4,,10
.L1626:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm15
	addsd	%xmm15, %xmm15
	jmp	.L1328
	.p2align 4,,10
.L1568:
	movq	%r8, %rax
	andl	$1, %r8d
	shrq	%rax
	orq	%r8, %rax
	cvtsi2sdq	%rax, %xmm15
	addsd	%xmm15, %xmm15
	jmp	.L1448
	.p2align 4,,10
.L1546:
	movq	%r8, %rdi
	andl	$1, %r8d
	shrq	%rdi
	orq	%r8, %rdi
	cvtsi2sdq	%rdi, %xmm7
	addsd	%xmm7, %xmm7
	jmp	.L1488
	.p2align 4,,10
.L1619:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm7
	addsd	%xmm7, %xmm7
	jmp	.L1356
	.p2align 4,,10
.L1597:
	movq	%rcx, %r14
	andl	$1, %ecx
	shrq	%r14
	orq	%rcx, %r14
	cvtsi2sdq	%r14, %xmm15
	addsd	%xmm15, %xmm15
	jmp	.L1396
	.p2align 4,,10
.L1590:
	movq	%r15, %r14
	andl	$1, %r15d
	shrq	%r14
	orq	%r15, %r14
	cvtsi2sdq	%r14, %xmm7
	addsd	%xmm7, %xmm7
	jmp	.L1408
	.p2align 4,,10
.L1575:
	movq	%rdx, %r15
	andl	$1, %edx
	shrq	%r15
	orq	%rdx, %r15
	cvtsi2sdq	%r15, %xmm7
	addsd	%xmm7, %xmm7
	jmp	.L1420
	.p2align 4,,10
.L1604:
	movq	%rcx, %rax
	andl	$1, %ecx
	shrq	%rax
	orq	%rcx, %rax
	cvtsi2sdq	%rax, %xmm7
	addsd	%xmm7, %xmm7
	jmp	.L1368
	.p2align 4,,10
.L1605:
	movq	%rax, %r9
	andl	$1, %eax
	shrq	%r9
	orq	%rax, %r9
	cvtsi2sdq	%r9, %xmm8
	addsd	%xmm8, %xmm8
	jmp	.L1370
	.p2align 4,,10
.L1559:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm5
	addsd	%xmm5, %xmm5
	jmp	.L1472
	.p2align 4,,10
.L1560:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm6
	addsd	%xmm6, %xmm6
	jmp	.L1474
	.p2align 4,,10
.L1599:
	movq	%r15, %r9
	andl	$1, %r15d
	shrq	%r9
	orq	%r15, %r9
	cvtsi2sdq	%r9, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L1400
	.p2align 4,,10
.L1600:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L1402
	.p2align 4,,10
.L1569:
	movq	%r15, %r14
	andl	$1, %r15d
	shrq	%r14
	orq	%r15, %r14
	cvtsi2sdq	%r14, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1450
	.p2align 4,,10
.L1549:
	movq	%r8, %rdi
	andl	$1, %r8d
	shrq	%rdi
	orq	%r8, %rdi
	cvtsi2sdq	%rdi, %xmm10
	addsd	%xmm10, %xmm10
	jmp	.L1494
	.p2align 4,,10
.L1554:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1462
	.p2align 4,,10
.L1556:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L1466
	.p2align 4,,10
.L1555:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L1464
	.p2align 4,,10
.L1557:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm3
	addsd	%xmm3, %xmm3
	jmp	.L1468
	.p2align 4,,10
.L1558:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm4
	addsd	%xmm4, %xmm4
	jmp	.L1470
	.p2align 4,,10
.L1593:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm10
	addsd	%xmm10, %xmm10
	jmp	.L1414
	.p2align 4,,10
.L1598:
	movq	%r14, %r15
	andl	$1, %r14d
	shrq	%r15
	orq	%r14, %r15
	cvtsi2sdq	%r15, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1398
	.p2align 4,,10
.L1592:
	movq	%r15, %r9
	andl	$1, %r15d
	shrq	%r9
	orq	%r15, %r9
	cvtsi2sdq	%r9, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L1412
	.p2align 4,,10
.L1548:
	movq	%r8, %rdi
	andl	$1, %r8d
	shrq	%rdi
	orq	%r8, %rdi
	cvtsi2sdq	%rdi, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L1492
	.p2align 4,,10
.L1547:
	movq	%r8, %rdi
	andl	$1, %r8d
	shrq	%rdi
	orq	%r8, %rdi
	cvtsi2sdq	%rdi, %xmm8
	addsd	%xmm8, %xmm8
	jmp	.L1490
	.p2align 4,,10
.L1581:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm13
	addsd	%xmm13, %xmm13
	jmp	.L1432
	.p2align 4,,10
.L1582:
	movq	%r15, %rax
	andl	$1, %r15d
	shrq	%rax
	orq	%r15, %rax
	cvtsi2sdq	%rax, %xmm14
	addsd	%xmm14, %xmm14
	jmp	.L1434
	.p2align 4,,10
.L1591:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm8
	addsd	%xmm8, %xmm8
	jmp	.L1410
	.p2align 4,,10
.L1570:
	movq	%r15, %r8
	andl	$1, %r15d
	shrq	%r8
	orq	%r15, %r8
	cvtsi2sdq	%r8, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L1452
	.p2align 4,,10
.L1571:
	movq	%r15, %r14
	andl	$1, %r15d
	shrq	%r14
	orq	%r15, %r14
	cvtsi2sdq	%r14, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L1454
	.p2align 4,,10
.L1576:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm8
	addsd	%xmm8, %xmm8
	jmp	.L1422
	.p2align 4,,10
.L1580:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L1430
	.p2align 4,,10
.L1579:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm11
	addsd	%xmm11, %xmm11
	jmp	.L1428
	.p2align 4,,10
.L1577:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L1424
	.p2align 4,,10
.L1578:
	movq	%rax, %r15
	andl	$1, %eax
	shrq	%r15
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm10
	addsd	%xmm10, %xmm10
	jmp	.L1426
	.p2align 4,,10
.L1630:
	movq	%rcx, %r9
	andl	$1, %ecx
	shrq	%r9
	orq	%rcx, %r9
	cvtsi2sdq	%r9, %xmm3
	addsd	%xmm3, %xmm3
	jmp	.L1336
	.p2align 4,,10
.L1609:
	movq	%rax, %r14
	andl	$1, %eax
	shrq	%r14
	orq	%rax, %r14
	cvtsi2sdq	%r14, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L1378
	.p2align 4,,10
.L1633:
	movq	%r11, %r9
	andl	$1, %r11d
	shrq	%r9
	orq	%r11, %r9
	cvtsi2sdq	%r9, %xmm6
	addsd	%xmm6, %xmm6
	jmp	.L1342
	.p2align 4,,10
.L1627:
	movq	%rdi, %r14
	andl	$1, %edi
	shrq	%r14
	orq	%rdi, %r14
	cvtsi2sdq	%r14, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L1330
	.p2align 4,,10
.L1628:
	movq	%rax, %rdi
	andl	$1, %eax
	shrq	%rdi
	orq	%rax, %rdi
	cvtsi2sdq	%rdi, %xmm1
	addsd	%xmm1, %xmm1
	jmp	.L1332
	.p2align 4,,10
.L1629:
	movq	%rcx, %rax
	andl	$1, %ecx
	shrq	%rax
	orq	%rcx, %rax
	cvtsi2sdq	%rax, %xmm2
	addsd	%xmm2, %xmm2
	jmp	.L1334
	.p2align 4,,10
.L1631:
	movq	%r14, %rcx
	andl	$1, %r14d
	shrq	%rcx
	orq	%r14, %rcx
	cvtsi2sdq	%rcx, %xmm4
	addsd	%xmm4, %xmm4
	jmp	.L1338
	.p2align 4,,10
.L1632:
	movq	%rdi, %r14
	andl	$1, %edi
	shrq	%r14
	orq	%rdi, %r14
	cvtsi2sdq	%r14, %xmm5
	addsd	%xmm5, %xmm5
	jmp	.L1340
	.p2align 4,,10
.L1621:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L1360
	.p2align 4,,10
.L1622:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm10
	addsd	%xmm10, %xmm10
	jmp	.L1362
	.p2align 4,,10
.L1611:
	movq	%r14, %r10
	andl	$1, %r14d
	shrq	%r10
	orq	%r14, %r10
	cvtsi2sdq	%r10, %xmm14
	addsd	%xmm14, %xmm14
	jmp	.L1382
	.p2align 4,,10
.L1620:
	movq	%r14, %r9
	andl	$1, %r14d
	shrq	%r9
	orq	%r14, %r9
	cvtsi2sdq	%r9, %xmm8
	addsd	%xmm8, %xmm8
	jmp	.L1358
	.p2align 4,,10
.L1607:
	movq	%rax, %r14
	andl	$1, %eax
	shrq	%r14
	orq	%rax, %r14
	cvtsi2sdq	%r14, %xmm10
	addsd	%xmm10, %xmm10
	jmp	.L1374
	.p2align 4,,10
.L1608:
	movq	%r9, %r14
	andl	$1, %r9d
	shrq	%r14
	orq	%r9, %r14
	cvtsi2sdq	%r14, %xmm11
	addsd	%xmm11, %xmm11
	jmp	.L1376
	.p2align 4,,10
.L1606:
	movq	%r9, %r14
	andl	$1, %r9d
	shrq	%r14
	orq	%r9, %r14
	cvtsi2sdq	%r14, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L1372
	.p2align 4,,10
.L1610:
	movq	%r9, %r14
	andl	$1, %r9d
	shrq	%r14
	orq	%r9, %r14
	cvtsi2sdq	%r14, %xmm13
	addsd	%xmm13, %xmm13
	jmp	.L1380
.L1530:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r13d
	cmpl	%r13d, %ebp
	jle	.L895
	.p2align 4,,10
.L926:
	movslq	64(%rsp), %r9
	movslq	80(%rsp), %r11
	movslq	64(%rsp), %r8
	movq	(%rbx), %rax
	movslq	80(%rsp), %rdi
	movq	(%r12,%r9,8), %r10
	movq	(%rax,%r8,8), %rcx
	leaq	(%r10,%r11,8), %r14
	movq	(%rcx,%rdi,8), %rdi
	call	udfi1
	movl	80(%rsp), %esi
	movq	%rax, (%r14)
	addl	$1, %esi
	movl	%esi, 80(%rsp)
	movl	80(%rsp), %edx
	cmpl	%edx, %ebp
	jg	.L926
	jmp	.L895
.L1531:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %esi
	cmpl	%esi, %ebp
	jle	.L895
	.p2align 4,,10
.L928:
	movslq	64(%rsp), %rdi
	movslq	80(%rsp), %rcx
	movslq	64(%rsp), %r9
	movslq	80(%rsp), %r11
	movslq	64(%rsp), %r10
	movq	8(%rbx), %rax
	movslq	80(%rsp), %r14
	movq	(%r12,%rdi,8), %rdx
	movq	(%rbx), %rdi
	movq	(%rax,%r9,8), %r8
	leaq	(%rdx,%rcx,8), %r13
	movq	(%rdi,%r10,8), %rcx
	movq	(%r8,%r11,8), %rsi
	movq	(%rcx,%r14,8), %rdi
	call	udfi2
	movl	80(%rsp), %esi
	movq	%rax, 0(%r13)
	addl	$1, %esi
	movl	%esi, 80(%rsp)
	movl	80(%rsp), %edx
	cmpl	%edx, %ebp
	jg	.L928
	jmp	.L895
.L1534:
	movl	64(%rsp), %r8d
	testl	%r8d, %r8d
	jle	.L938
	movl	64(%rsp), %eax
	subl	$1, %eax
.L939:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %edx
	cmpl	%edx, %ebp
	jle	.L895
	cltq
	leaq	0(,%rax,8), %r13
	jmp	.L942
.L1641:
	movl	80(%rsp), %esi
	subl	$1, %esi
.L941:
	movslq	64(%rsp), %rdi
	movslq	80(%rsp), %rcx
	movslq	%esi, %rax
	movslq	64(%rsp), %r10
	movq	(%rbx), %r11
	movl	80(%rsp), %r8d
	movq	(%r12,%rdi,8), %r14
	movq	(%r11,%r13), %rdx
	movq	(%r11,%r10,8), %rdi
	movslq	%r8d, %r9
	movq	(%rdx,%rax,8), %rsi
	movq	48(%rsp), %rdx
	leaq	(%r14,%rcx,8), %r14
	movq	(%rdi,%r9,8), %rdi
	call	udfi3
	movl	80(%rsp), %esi
	movq	%rax, (%r14)
	addl	$1, %esi
	movl	%esi, 80(%rsp)
	movl	80(%rsp), %ecx
	cmpl	%ecx, %ebp
	jle	.L895
.L942:
	movl	80(%rsp), %r9d
	testl	%r9d, %r9d
	jg	.L1641
	leal	-1(%rbp), %esi
	jmp	.L941
.L1532:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r14d
	cmpl	%r14d, %ebp
	jle	.L895
	.p2align 4,,10
.L930:
	movslq	64(%rsp), %r9
	movslq	80(%rsp), %r13
	movslq	64(%rsp), %r11
	movq	16(%rbx), %rdx
	movslq	80(%rsp), %r10
	movslq	64(%rsp), %r8
	movslq	80(%rsp), %rsi
	movslq	64(%rsp), %rdi
	movq	(%r12,%r9,8), %rax
	movslq	80(%rsp), %rcx
	movq	(%rdx,%r11,8), %r9
	movq	(%rbx), %r11
	leaq	(%rax,%r13,8), %r14
	movq	8(%rbx), %r13
	movq	(%r9,%r10,8), %rdx
	movq	(%r11,%rdi,8), %r10
	movq	0(%r13,%r8,8), %rax
	movq	(%r10,%rcx,8), %rdi
	movq	(%rax,%rsi,8), %rsi
	call	udfi3
	movq	%rax, (%r14)
	movl	80(%rsp), %r14d
	addl	$1, %r14d
	movl	%r14d, 80(%rsp)
	movl	80(%rsp), %r8d
	cmpl	%r8d, %ebp
	jg	.L930
	jmp	.L895
.L1533:
	movl	64(%rsp), %r13d
	testl	%r13d, %r13d
	jle	.L932
	movl	64(%rsp), %eax
	subl	$1, %eax
.L933:
	movl	$0, 80(%rsp)
	movl	80(%rsp), %r14d
	cmpl	%r14d, %ebp
	jle	.L895
	cltq
	leaq	0(,%rax,8), %r14
	jmp	.L936
.L1642:
	movl	80(%rsp), %r10d
	subl	$1, %r10d
.L935:
	movslq	64(%rsp), %r8
	movslq	80(%rsp), %rdx
	movslq	%r10d, %rdi
	movslq	64(%rsp), %rcx
	movq	(%rbx), %rax
	movslq	80(%rsp), %r11
	movq	(%rax,%r14), %rsi
	movq	(%r12,%r8,8), %r9
	movq	(%rax,%rcx,8), %r10
	movq	(%rsi,%rdi,8), %rsi
	leaq	(%r9,%rdx,8), %r13
	movq	(%r10,%r11,8), %rdi
	call	udfi2
	movl	80(%rsp), %r8d
	movq	%rax, 0(%r13)
	addl	$1, %r8d
	movl	%r8d, 80(%rsp)
	movl	80(%rsp), %edx
	cmpl	%edx, %ebp
	jle	.L895
.L936:
	movl	80(%rsp), %r11d
	testl	%r11d, %r11d
	jg	.L1642
	leal	-1(%rbp), %r10d
	jmp	.L935
.L932:
	movl	24(%rsp), %eax
	subl	$1, %eax
	jmp	.L933
.L1537:
	subq	$1, %rsi
	addq	$1000000, %rdx
	jmp	.L889
.L938:
	movl	24(%rsp), %eax
	subl	$1, %eax
	jmp	.L939
	.cfi_endproc
.LFE68:
	.size	translate_file, .-translate_file
	.p2align 4,,15
	.globl	convert_thread
	.type	convert_thread, @function
convert_thread:
.LFB70:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movl	20(%rdi), %eax
	movl	4(%rdi), %ecx
	movq	32(%rdi), %rdx
	movq	24(%rdi), %rsi
	movl	12(%rbx), %r9d
	movl	8(%rbx), %r8d
	movl	%eax, 8(%rsp)
	movl	16(%rdi), %edi
	movl	%edi, (%rsp)
	movl	(%rbx), %edi
	call	translate_file
	movq	%rbx, %rdi
	call	free
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE70:
	.size	convert_thread, .-convert_thread
	.section	.rodata.str1.1
.LC37:
	.string	"Function not accepted."
.LC38:
	.string	"f(%f) = %f\n"
	.text
	.p2align 4,,15
	.globl	tests
	.type	tests, @function
tests:
.LFB69:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$41, %edx
	movabsq	$3564984146619542833, %rax
	movl	$12, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$1048, %rsp
	.cfi_def_cfa_offset 1072
	movw	%dx, 24(%rsp)
	movq	%rax, 16(%rsp)
	call	malloc
	movzbl	24(%rsp), %esi
	movq	16(%rsp), %rcx
	movl	$10, %edi
	movq	%rax, %rbx
	movw	%di, 9(%rax)
	movl	$1, %edi
	movq	%rcx, (%rax)
	movb	%sil, 8(%rax)
	call	fpar_init
	xorl	%edx, %edx
	xorl	%edi, %edi
	movq	%rbx, %rcx
	movl	$1, %esi
	call	fpar_function
	xorl	%edi, %edi
	call	fpar_ok
	testl	%eax, %eax
	je	.L1656
	movl	$8, %edi
	movl	$21, %ebp
	call	malloc
	movl	$8, %edi
	movq	%rax, %rbx
	call	malloc
	xorpd	%xmm0, %xmm0
	movq	%rax, (%rbx)
	movq	%rbx, %rsi
	movq	$0, (%rax)
	xorl	%edi, %edi
	call	fpar_f
	movapd	%xmm0, %xmm1
	movl	$.LC38, %edi
	xorpd	%xmm0, %xmm0
	movl	$2, %eax
	call	printf
	movsd	.LC39(%rip), %xmm9
	movq	(%rbx), %rax
	jmp	.L1648
	.p2align 4,,10
.L1657:
	movq	(%rbx), %r8
	xorpd	%xmm0, %xmm0
	movq	%rbx, %rsi
	xorl	%edi, %edi
	subl	$5, %ebp
	movsd	%xmm1, (%r8)
	movsd	%xmm1, (%rsp)
	call	fpar_f
	movapd	%xmm0, %xmm1
	movl	$.LC38, %edi
	movsd	(%rsp), %xmm0
	movl	$2, %eax
	call	printf
	movsd	(%rsp), %xmm3
	movq	(%rbx), %r9
	xorpd	%xmm0, %xmm0
	movq	%rbx, %rsi
	addsd	.LC39(%rip), %xmm3
	xorl	%edi, %edi
	movsd	%xmm3, (%r9)
	movsd	%xmm3, (%rsp)
	call	fpar_f
	movsd	(%rsp), %xmm4
	movl	$.LC38, %edi
	movapd	%xmm0, %xmm1
	movl	$2, %eax
	movapd	%xmm4, %xmm0
	call	printf
	movsd	(%rsp), %xmm5
	movq	(%rbx), %r10
	xorpd	%xmm0, %xmm0
	movq	%rbx, %rsi
	addsd	.LC39(%rip), %xmm5
	xorl	%edi, %edi
	movsd	%xmm5, (%r10)
	movsd	%xmm5, (%rsp)
	call	fpar_f
	movsd	(%rsp), %xmm6
	movl	$.LC38, %edi
	movapd	%xmm0, %xmm1
	movl	$2, %eax
	movapd	%xmm6, %xmm0
	call	printf
	movsd	(%rsp), %xmm7
	movq	(%rbx), %r11
	xorpd	%xmm0, %xmm0
	movq	%rbx, %rsi
	addsd	.LC39(%rip), %xmm7
	xorl	%edi, %edi
	movsd	%xmm7, (%r11)
	movsd	%xmm7, (%rsp)
	call	fpar_f
	movsd	(%rsp), %xmm8
	movl	$.LC38, %edi
	movapd	%xmm0, %xmm1
	movl	$2, %eax
	movapd	%xmm8, %xmm0
	call	printf
	movsd	(%rsp), %xmm9
	movq	(%rbx), %rax
	addsd	.LC39(%rip), %xmm9
.L1648:
	xorpd	%xmm0, %xmm0
	movsd	%xmm9, (%rax)
	xorl	%edi, %edi
	movq	%rbx, %rsi
	movsd	%xmm9, (%rsp)
	call	fpar_f
	movsd	(%rsp), %xmm2
	movl	$.LC38, %edi
	movapd	%xmm0, %xmm1
	movl	$2, %eax
	movapd	%xmm2, %xmm0
	call	printf
	movsd	(%rsp), %xmm1
	cmpl	$1, %ebp
	addsd	.LC39(%rip), %xmm1
	jne	.L1657
	movq	(%rbx), %rdi
	call	free
	movq	%rbx, %rdi
	call	free
	addq	$1048, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	fpar_deinit
	.p2align 4,,10
.L1656:
	.cfi_restore_state
	addq	$1048, %rsp
	.cfi_def_cfa_offset 24
	movl	$.LC37, %edi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	puts
	.cfi_endproc
.LFE69:
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
.LC75:
	.string	"%s "
	.text
	.p2align 4,,15
	.globl	fjpg
	.type	fjpg, @function
fjpg:
.LFB71:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$184, %rsp
	.cfi_def_cfa_offset 240
	testl	%r8d, %r8d
	movq	%rdi, 72(%rsp)
	movl	%edx, 60(%rsp)
	movl	%r8d, 56(%rsp)
	jle	.L1938
	movslq	60(%rsp), %rbp
	movq	%rsi, %r12
	movq	%rcx, %rbx
	movl	$0, ncache(%rip)
	leaq	0(%rbp,%rbp,2), %rdi
	salq	$3, %rdi
	call	malloc
	movq	%r12, %rdi
	movq	%rax, icache(%rip)
	call	strlen
	leaq	3(%rax), %rdi
	call	malloc
	leaq	0(,%rbp,8), %rdi
	salq	$2, %rbp
	movq	%rax, %r13
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 80(%rsp)
	call	malloc
	movq	%rbp, %rdi
	movq	%rax, 48(%rsp)
	call	malloc
	movslq	56(%rsp), %rdi
	movq	%rax, %rbp
	salq	$3, %rdi
	call	malloc
	movl	$5, %ecx
	movq	%rax, 88(%rsp)
	movl	$.LC41, %edi
	movq	%r12, %rsi
	repz; cmpsb
	jne	.L1939
	movb	$0, 103(%rsp)
	movl	$13, 96(%rsp)
.L1661:
	movl	ualoop(%rip), %r14d
	testl	%r14d, %r14d
	je	.L1679
	cmpl	$11, 96(%rsp)
	je	.L1738
	cmpl	$4, 96(%rsp)
	je	.L1738
	cmpl	$12, 96(%rsp)
	movl	$1, %r15d
	cmovne	56(%rsp), %r15d
	movl	%r15d, 56(%rsp)
.L1679:
	movq	72(%rsp), %rdi
	movl	$.LC61, %esi
	call	fopen
	testq	%rax, %rax
	movq	%rax, %r14
	je	.L1680
	movl	overwrite_mode(%rip), %eax
	testl	%eax, %eax
	je	.L1681
	movq	%r14, %rdi
	call	fclose
.L1680:
	movl	96(%rsp), %edx
	testl	%edx, %edx
	je	.L1682
.L1689:
	movl	60(%rsp), %r13d
	testl	%r13d, %r13d
	jle	.L1940
	movl	60(%rsp), %ecx
	movq	(%rbx), %r12
	movl	$.LC63, %esi
	movl	$65535, 44(%rsp)
	movq	%rbx, %r14
	movl	$65535, %r15d
	subl	$1, %ecx
	movq	%r12, %rdi
	leaq	8(%rbx,%rcx,8), %rdx
	andl	$1, %ecx
	movq	%rcx, 104(%rsp)
	movq	%rdx, 64(%rsp)
	call	strcasestr
	testq	%rax, %rax
	je	.L1941
.L1795:
	movq	48(%rsp), %rsi
	movq	80(%rsp), %rdi
	movq	%r12, %rcx
	movq	%rbp, %rdx
	call	load_jpeg_file
.L1907:
	testl	%eax, %eax
	jne	.L1695
	movq	48(%rsp), %r8
	movl	(%r8), %esi
	cmpl	$65535, %esi
	jl	.L1909
	jg	.L1942
.L1800:
	movl	0(%rbp), %esi
	cmpl	$65535, %esi
	jl	.L1910
	.p2align 4,,2
	jg	.L1943
.L1803:
	leaq	8(%rbx), %r14
	movq	80(%rsp), %r13
	movq	48(%rsp), %rbx
	addq	$4, %rbp
	addq	$4, %rbx
	addq	$8, %r13
	cmpq	64(%rsp), %r14
	je	.L1703
	cmpq	$0, 104(%rsp)
	je	.L1932
	movq	(%r14), %r12
	movl	$.LC63, %esi
	movq	%r12, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1911
.L1805:
	movq	%r12, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_jpeg_file
.L1912:
	testl	%eax, %eax
	jne	.L1695
	movl	(%rbx), %esi
	cmpl	44(%rsp), %esi
	jl	.L1914
	.p2align 4,,3
	jg	.L1944
.L1810:
	movl	0(%rbp), %esi
	cmpl	%r15d, %esi
	jl	.L1916
.L1957:
	.p2align 4,,5
	jg	.L1945
.L1813:
	addq	$8, %r14
	addq	$4, %rbp
	addq	$4, %rbx
	addq	$8, %r13
	cmpq	64(%rsp), %r14
	je	.L1703
.L1932:
	movl	%r15d, %r12d
	jmp	.L1704
	.p2align 4,,10
.L1953:
	cmpl	$65534, 44(%rsp)
	jle	.L1946
.L1698:
	movl	%esi, 44(%rsp)
.L1699:
	movl	0(%rbp), %esi
	cmpl	%r12d, %esi
	jge	.L1700
.L1954:
	cmpl	$65534, %r12d
	jle	.L1947
.L1701:
	movl	%esi, %r12d
.L1702:
	addq	$8, %r14
	movl	$.LC63, %esi
	addq	$4, %rbp
	movq	(%r14), %r15
	movq	%r14, 48(%rsp)
	addq	$4, %rbx
	addq	$8, %r13
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1948
.L1816:
	movq	%r15, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_jpeg_file
.L1918:
	testl	%eax, %eax
	jne	.L1695
	movl	(%rbx), %esi
	cmpl	44(%rsp), %esi
	jl	.L1920
	.p2align 4,,3
	jg	.L1949
.L1821:
	movl	0(%rbp), %esi
	cmpl	%r12d, %esi
	jl	.L1922
	.p2align 4,,5
	jg	.L1950
.L1824:
	movq	48(%rsp), %r14
	addq	$4, %rbp
	addq	$4, %rbx
	addq	$8, %r13
	addq	$8, %r14
	cmpq	64(%rsp), %r14
	je	.L1951
.L1704:
	movq	(%r14), %r15
	movl	$.LC63, %esi
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1952
.L1690:
	movq	%r15, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_jpeg_file
.L1692:
	testl	%eax, %eax
	jne	.L1695
	movl	(%rbx), %esi
	cmpl	44(%rsp), %esi
	jl	.L1953
	.p2align 4,,3
	jle	.L1699
	movl	44(%rsp), %edx
	xorl	%eax, %eax
	movl	$.LC70, %edi
	call	printf
	movl	0(%rbp), %esi
	cmpl	%r12d, %esi
	jl	.L1954
	.p2align 4,,10
.L1700:
	jle	.L1702
	movl	%r12d, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1702
	.p2align 4,,10
.L1939:
	movl	$6, %eax
	movl	$.LC42, %edi
	movq	%r12, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L1724
	movl	$.LC43, %edi
	movq	%r12, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	seta	%r14b
	setb	%dl
	subl	%edx, %r14d
	movsbl	%r14b, %r14d
	testl	%r14d, %r14d
	je	.L1955
.L1662:
	movl	$6, %ecx
	movl	$.LC44, %edi
	movq	%r12, %rsi
	repz; cmpsb
	seta	%r15b
	setb	%r8b
	subl	%r8d, %r15d
	movsbl	%r15b, %r15d
	testl	%r15d, %r15d
	jne	.L1663
	cmpl	$2, 60(%rsp)
	jle	.L1663
	movb	$1, 103(%rsp)
	movl	$12, 96(%rsp)
	jmp	.L1661
	.p2align 4,,10
.L1738:
	movl	$1, 56(%rsp)
	jmp	.L1679
	.p2align 4,,10
.L1922:
	cmpl	$65534, %r12d
	jle	.L1923
.L1825:
	movl	%esi, %r12d
	jmp	.L1824
	.p2align 4,,10
.L1920:
	cmpl	$65534, 44(%rsp)
	jle	.L1921
.L1822:
	movl	%esi, 44(%rsp)
	jmp	.L1821
	.p2align 4,,10
.L1952:
	movl	$.LC64, %esi
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1690
	movl	$.LC65, %esi
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1693
	movq	%r15, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_png_file
	jmp	.L1692
	.p2align 4,,10
.L1948:
	movl	$.LC64, %esi
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1816
	movl	$.LC65, %esi
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1956
	movq	%r15, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_png_file
	jmp	.L1918
	.p2align 4,,10
.L1947:
	movl	%esi, %edx
	movl	$.LC71, %edi
	movl	%r12d, %esi
	xorl	%eax, %eax
	call	printf
	movl	0(%rbp), %esi
	jmp	.L1701
	.p2align 4,,10
.L1946:
	movl	%esi, %edx
	movl	44(%rsp), %esi
	movl	$.LC69, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%rbx), %esi
	jmp	.L1698
	.p2align 4,,10
.L1950:
	movl	%r12d, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1824
	.p2align 4,,10
.L1949:
	movl	44(%rsp), %edx
	movl	$.LC70, %edi
	call	printf
	jmp	.L1821
	.p2align 4,,10
.L1923:
	movl	%esi, %edx
	movl	$.LC71, %edi
	movl	%r12d, %esi
	xorl	%eax, %eax
	call	printf
	movl	0(%rbp), %esi
	jmp	.L1825
	.p2align 4,,10
.L1921:
	movl	%esi, %edx
	movl	44(%rsp), %esi
	movl	$.LC69, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%rbx), %esi
	jmp	.L1822
	.p2align 4,,10
.L1681:
	movq	72(%rsp), %rsi
	movl	$.LC62, %edi
	xorl	%eax, %eax
	call	printf
	movq	%r14, %rdi
	call	fclose
	movl	$1, %eax
.L1897:
	addq	$184, %rsp
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
.L1933:
	.cfi_restore_state
	movq	%r15, %r12
.L1694:
	movq	%r12, %rsi
	movl	$.LC67, %edi
	xorl	%eax, %eax
	call	printf
	.p2align 4,,10
.L1695:
	movq	(%r14), %rsi
	movl	$.LC68, %edi
	xorl	%eax, %eax
	call	printf
	movl	$4, %eax
	jmp	.L1897
	.p2align 4,,10
.L1951:
	movl	%r12d, %r15d
.L1703:
	movslq	%r15d, %rdi
	movl	$8, %esi
	call	calloc
	testl	%r15d, %r15d
	movq	%rax, %rbp
	jle	.L1705
	movslq	44(%rsp), %r14
.L1706:
	leal	-1(%r15), %r13d
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	andl	$7, %r13d
	cmpl	$1, %r15d
	movq	%rax, 0(%rbp)
	movl	$1, %ebx
	jle	.L1705
	testl	%r13d, %r13d
	je	.L1707
	cmpl	$1, %r13d
	je	.L1854
	cmpl	$2, %r13d
	je	.L1855
	cmpl	$3, %r13d
	je	.L1856
	cmpl	$4, %r13d
	je	.L1857
	cmpl	$5, %r13d
	je	.L1858
	cmpl	$6, %r13d
	je	.L1859
	movl	$8, %esi
	movq	%r14, %rdi
	movl	$2, %ebx
	call	calloc
	movq	%rax, 8(%rbp)
.L1859:
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	movq	%rax, 0(%rbp,%rbx,8)
	addq	$1, %rbx
.L1858:
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	movq	%rax, 0(%rbp,%rbx,8)
	addq	$1, %rbx
.L1857:
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	movq	%rax, 0(%rbp,%rbx,8)
	addq	$1, %rbx
.L1856:
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	movq	%rax, 0(%rbp,%rbx,8)
	addq	$1, %rbx
.L1855:
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	movq	%rax, 0(%rbp,%rbx,8)
	addq	$1, %rbx
.L1854:
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	movq	%rax, 0(%rbp,%rbx,8)
	addq	$1, %rbx
	cmpl	%ebx, %r15d
	jle	.L1705
.L1707:
	leaq	1(%rbx), %r12
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	leaq	2(%rbx), %r13
	movq	%rax, 0(%rbp,%rbx,8)
	movl	$8, %esi
	movq	%r14, %rdi
	call	calloc
	movl	$8, %esi
	movq	%rax, 0(%rbp,%r12,8)
	movq	%r14, %rdi
	leaq	3(%rbx), %r12
	call	calloc
	movl	$8, %esi
	movq	%rax, 0(%rbp,%r13,8)
	movq	%r14, %rdi
	leaq	4(%rbx), %r13
	call	calloc
	movl	$8, %esi
	movq	%rax, 0(%rbp,%r12,8)
	movq	%r14, %rdi
	leaq	5(%rbx), %r12
	call	calloc
	movl	$8, %esi
	movq	%rax, 0(%rbp,%r13,8)
	movq	%r14, %rdi
	leaq	6(%rbx), %r13
	call	calloc
	movl	$8, %esi
	movq	%rax, 0(%rbp,%r12,8)
	movq	%r14, %rdi
	call	calloc
	leaq	7(%rbx), %r12
	movq	%rax, 0(%rbp,%r13,8)
	movl	$8, %esi
	movq	%r14, %rdi
	addq	$8, %rbx
	call	calloc
	cmpl	%ebx, %r15d
	movq	%rax, 0(%rbp,%r12,8)
	jg	.L1707
.L1705:
	cmpl	$1, 56(%rsp)
	jle	.L1708
	cmpb	$0, 103(%rsp)
	je	.L1708
	movl	44(%rsp), %edi
	movq	80(%rsp), %r9
	xorl	%r13d, %r13d
	movl	56(%rsp), %esi
	movl	96(%rsp), %r10d
	movl	60(%rsp), %eax
	movl	56(%rsp), %r14d
	movl	%edi, 132(%rsp)
	movl	$40, %edi
	movq	%r9, 152(%rsp)
	movl	%esi, 144(%rsp)
	movl	%r10d, 148(%rsp)
	movl	%eax, 128(%rsp)
	movq	%rbp, 160(%rsp)
	subl	$1, %r14d
	movl	%r15d, 136(%rsp)
	movl	$0, 140(%rsp)
	andl	$1, %r14d
	call	malloc
	movq	136(%rsp), %rcx
	movq	144(%rsp), %rdx
	xorl	%esi, %esi
	movq	128(%rsp), %r11
	movq	152(%rsp), %r8
	movq	160(%rsp), %rbx
	movq	88(%rsp), %rdi
	movq	%rcx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rax, %rcx
	movq	%r11, (%rax)
	movq	%r8, 24(%rax)
	movl	$convert_thread, %edx
	movq	%rbx, 32(%rax)
	call	pthread_create
	testl	%eax, %eax
	jne	.L1904
	movq	88(%rsp), %rbx
	movl	$1, %r13d
	addq	$8, %rbx
	cmpl	$1, 56(%rsp)
	jle	.L1710
	testl	%r14d, %r14d
	je	.L1928
	movl	$40, %edi
	movl	$1, 140(%rsp)
	call	malloc
	movq	144(%rsp), %rdi
	movq	152(%rsp), %rsi
	movq	%rax, %rcx
	movq	128(%rsp), %r12
	movq	136(%rsp), %r9
	movl	$convert_thread, %edx
	movq	160(%rsp), %r10
	movq	%rdi, 16(%rax)
	movq	%rsi, 24(%rax)
	movq	%rbx, %rdi
	xorl	%esi, %esi
	movq	%r12, (%rax)
	movq	%r9, 8(%rax)
	movq	%r10, 32(%rax)
	call	pthread_create
	testl	%eax, %eax
	jne	.L1904
	movq	88(%rsp), %rbx
	movb	$2, %r13b
	addq	$16, %rbx
	cmpl	$2, 56(%rsp)
	jle	.L1710
.L1928:
	movl	56(%rsp), %r12d
	jmp	.L1711
	.p2align 4,,10
.L1709:
	addl	$1, %r13d
	movl	$40, %edi
	leaq	8(%rbx), %r14
	movl	%r13d, 140(%rsp)
	call	malloc
	movq	136(%rsp), %rdi
	movq	144(%rsp), %rsi
	movq	%rax, %rcx
	movq	128(%rsp), %r9
	movq	152(%rsp), %r10
	movl	$convert_thread, %edx
	movq	160(%rsp), %r11
	movq	%rdi, 8(%rax)
	movq	%rsi, 16(%rax)
	movq	%r14, %rdi
	xorl	%esi, %esi
	movq	%r9, (%rax)
	movq	%r10, 24(%rax)
	movq	%r11, 32(%rax)
	call	pthread_create
	testl	%eax, %eax
	jne	.L1904
	addl	$1, %r13d
	addq	$16, %rbx
	cmpl	%r13d, %r12d
	jle	.L1710
.L1711:
	movl	$40, %edi
	movl	%r13d, 140(%rsp)
	call	malloc
	movq	144(%rsp), %rcx
	movq	152(%rsp), %rdx
	xorl	%esi, %esi
	movq	128(%rsp), %r14
	movq	136(%rsp), %r11
	movq	%rbx, %rdi
	movq	160(%rsp), %r8
	movq	%rcx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rax, %rcx
	movq	%r14, (%rax)
	movq	%r11, 8(%rax)
	movl	$convert_thread, %edx
	movq	%r8, 32(%rax)
	call	pthread_create
	testl	%eax, %eax
	je	.L1709
.L1904:
	movl	56(%rsp), %edx
	leal	1(%r13), %esi
	movl	$.LC73, %edi
	xorl	%eax, %eax
	call	printf
	movl	$8, %eax
	jmp	.L1897
	.p2align 4,,10
.L1910:
	movl	%esi, %r15d
	jmp	.L1803
	.p2align 4,,10
.L1909:
	movl	%esi, 44(%rsp)
	jmp	.L1800
	.p2align 4,,10
.L1724:
	movb	$1, 103(%rsp)
	movl	$4, 96(%rsp)
	jmp	.L1661
	.p2align 4,,10
.L1914:
	cmpl	$65534, 44(%rsp)
	jle	.L1915
.L1811:
	movl	%esi, 44(%rsp)
	movl	0(%rbp), %esi
	cmpl	%r15d, %esi
	jge	.L1957
.L1916:
	cmpl	$65534, %r15d
	jle	.L1917
.L1814:
	movl	%esi, %r15d
	jmp	.L1813
	.p2align 4,,10
.L1708:
	cmpl	$13, 96(%rsp)
	jne	.L1714
	movq	80(%rsp), %rbp
	movq	0(%rbp), %rbp
.L1715:
	movq	72(%rsp), %rdi
	movl	$.LC63, %esi
	call	strcasestr
	testq	%rax, %rax
	je	.L1958
.L1716:
	movq	72(%rsp), %rcx
	movl	44(%rsp), %esi
	movl	%r15d, %edx
	movq	%rbp, %rdi
	call	save_jpeg_file
.L1718:
	testl	%eax, %eax
	je	.L1722
.L1721:
	movq	72(%rsp), %rsi
	movl	$.LC74, %edi
	xorl	%eax, %eax
	call	printf
	movl	$16, %eax
	jmp	.L1897
	.p2align 4,,10
.L1955:
	cmpl	$1, 60(%rsp)
	movb	$1, 103(%rsp)
	movl	$11, 96(%rsp)
	jg	.L1661
	jmp	.L1662
	.p2align 4,,10
.L1943:
	movl	$65535, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1803
	.p2align 4,,10
.L1942:
	movl	$65535, %edx
	movl	$.LC70, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1800
	.p2align 4,,10
.L1663:
	movl	$5, %ecx
	movl	$.LC45, %edi
	movq	%r12, %rsi
	repz; cmpsb
	jne	.L1664
	movl	60(%rsp), %r10d
	movb	$1, 103(%rsp)
	movl	$1, 96(%rsp)
	testl	%r10d, %r10d
	jg	.L1661
.L1665:
	movl	$5, %ecx
	movl	$.LC47, %edi
	movq	%r12, %rsi
	repz; cmpsb
	seta	%r11b
	setb	%al
	subl	%eax, %r11d
	movsbl	%r11b, %edx
	testl	%edx, %edx
	jne	.L1666
	cmpl	$1, 60(%rsp)
	movb	$1, 103(%rsp)
	movl	$2, 96(%rsp)
	jg	.L1661
	movl	$.LC48, %esi
	movq	%r12, %rdi
	movl	%edx, 16(%rsp)
	call	strcmp
	movl	16(%rsp), %edx
	movl	%eax, %r8d
.L1668:
	movl	$5, %ecx
	movl	$.LC49, %edi
	movq	%r12, %rsi
	repz; cmpsb
	seta	%r10b
	setb	%r11b
	subl	%r11d, %r10d
	movsbl	%r10b, %r9d
	testl	%r9d, %r9d
	jne	.L1669
	cmpl	$2, 60(%rsp)
	jg	.L1731
	movl	$.LC50, %esi
	movq	%r12, %rdi
	movl	%edx, 16(%rsp)
	movl	%r8d, 32(%rsp)
	movl	%r9d, 24(%rsp)
	call	strcmp
	movl	16(%rsp), %edx
	movl	32(%rsp), %r8d
	movl	24(%rsp), %r9d
.L1671:
	movl	$7, %r11d
	movl	$.LC51, %edi
	movq	%r12, %rsi
	movq	%r11, %rcx
	repz; cmpsb
	je	.L1733
	movl	$.LC52, %edi
	movq	%r12, %rsi
	movq	%r11, %rcx
	repz; cmpsb
	je	.L1734
	movl	$6, %ecx
	movl	$.LC53, %edi
	movq	%r12, %rsi
	repz; cmpsb
	seta	%r10b
	setb	%r11b
	subl	%r11d, %r10d
	movsbl	%r10b, %ecx
	testl	%ecx, %ecx
	jne	.L1672
	cmpl	$5, 60(%rsp)
	jle	.L1672
	movb	$1, 103(%rsp)
	movl	$10, 96(%rsp)
	jmp	.L1661
	.p2align 4,,10
.L1664:
	movl	$6, %ecx
	movl	$.LC46, %edi
	movq	%r12, %rsi
	repz; cmpsb
	jne	.L1665
	movl	60(%rsp), %r9d
	movb	$1, 103(%rsp)
	movl	$5, 96(%rsp)
	testl	%r9d, %r9d
	jg	.L1661
	jmp	.L1665
	.p2align 4,,10
.L1938:
	movl	$.LC40, %edi
	call	puts
	xorl	%eax, %eax
	jmp	.L1897
	.p2align 4,,10
.L1682:
	movq	%r12, %rsi
	movq	%r13, %rdi
	movq	%r13, %r12
	call	strcpy
.L1685:
	movl	(%r12), %r8d
	addq	$4, %r12
	leal	-16843009(%r8), %eax
	notl	%r8d
	andl	%r8d, %eax
	movl	%eax, %r9d
	andl	$-2139062144, %r9d
	je	.L1685
	movl	%r9d, %edi
	movl	%r9d, %esi
	leaq	2(%r12), %r10
	shrl	$16, %edi
	testl	$32896, %eax
	movl	$10, %r11d
	cmove	%edi, %esi
	cmove	%r10, %r12
	movl	56(%rsp), %edi
	addb	%sil, %sil
	sbbq	$3, %r12
	movw	%r11w, (%r12)
	movl	$1, %r12d
	call	fpar_init
	movl	60(%rsp), %r14d
	movl	56(%rsp), %r15d
	xorl	%edi, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	addl	$1, %r14d
	subl	$1, %r15d
	movl	%r14d, %esi
	andl	$7, %r15d
	call	fpar_function
	cmpl	$1, 56(%rsp)
	jle	.L1687
	testl	%r15d, %r15d
	je	.L1688
	cmpl	$1, %r15d
	je	.L1860
	cmpl	$2, %r15d
	je	.L1861
	cmpl	$3, %r15d
	je	.L1862
	cmpl	$4, %r15d
	je	.L1863
	cmpl	$5, %r15d
	je	.L1864
	cmpl	$6, %r15d
	je	.L1865
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	movl	$1, %edi
	movl	$2, %r12d
	call	fpar_function
.L1865:
	movl	%r12d, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	addl	$1, %r12d
	call	fpar_function
.L1864:
	movl	%r12d, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	addl	$1, %r12d
	call	fpar_function
.L1863:
	movl	%r12d, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	addl	$1, %r12d
	call	fpar_function
.L1862:
	movl	%r12d, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	addl	$1, %r12d
	call	fpar_function
.L1861:
	movl	%r12d, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	addl	$1, %r12d
	call	fpar_function
.L1860:
	movl	%r12d, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	addl	$1, %r12d
	call	fpar_function
	cmpl	%r12d, 56(%rsp)
	jle	.L1687
.L1688:
	movl	%r12d, %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	call	fpar_function
	leal	1(%r12), %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	call	fpar_function
	leal	2(%r12), %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	call	fpar_function
	leal	3(%r12), %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	call	fpar_function
	leal	4(%r12), %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	call	fpar_function
	leal	5(%r12), %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	call	fpar_function
	leal	6(%r12), %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	call	fpar_function
	leal	7(%r12), %edi
	movq	%r13, %rcx
	movl	$8, %edx
	movl	%r14d, %esi
	addl	$8, %r12d
	call	fpar_function
	cmpl	%r12d, 56(%rsp)
	jg	.L1688
.L1687:
	xorl	%edi, %edi
	call	fpar_ok
	testl	%eax, %eax
	jne	.L1689
	movl	$.LC37, %edi
	call	puts
	movl	$2, %eax
	jmp	.L1897
	.p2align 4,,10
.L1941:
	movl	$.LC64, %esi
	movq	%r12, %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1795
	movl	$.LC65, %esi
	movq	%r12, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1959
	movq	48(%rsp), %rsi
	movq	80(%rsp), %rdi
	movq	%r12, %rcx
	movq	%rbp, %rdx
	call	load_png_file
	jmp	.L1907
	.p2align 4,,10
.L1911:
	movl	$.LC64, %esi
	movq	%r12, %rdi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1805
	movl	$.LC65, %esi
	movq	%r12, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1960
	movq	%r12, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_png_file
	jmp	.L1912
	.p2align 4,,10
.L1710:
	movq	88(%rsp), %rbx
	movl	56(%rsp), %eax
	xorl	%esi, %esi
	movl	$1, %r14d
	movq	(%rbx), %rdi
	leal	-1(%rax), %r13d
	andl	$7, %r13d
	call	pthread_join
	cmpl	$1, 56(%rsp)
	jle	.L1712
	testl	%r13d, %r13d
	je	.L1926
	cmpl	$1, %r13d
	je	.L1848
	cmpl	$2, %r13d
	je	.L1849
	cmpl	$3, %r13d
	je	.L1850
	cmpl	$4, %r13d
	je	.L1851
	cmpl	$5, %r13d
	je	.L1852
	cmpl	$6, %r13d
	je	.L1853
	movq	88(%rsp), %r12
	xorl	%esi, %esi
	movl	$2, %r14d
	movq	8(%r12), %rdi
	call	pthread_join
.L1853:
	movq	88(%rsp), %rcx
	xorl	%esi, %esi
	movq	(%rcx,%r14,8), %rdi
	addq	$1, %r14
	call	pthread_join
.L1852:
	movq	88(%rsp), %rdx
	xorl	%esi, %esi
	movq	(%rdx,%r14,8), %rdi
	addq	$1, %r14
	call	pthread_join
.L1851:
	movq	88(%rsp), %r8
	xorl	%esi, %esi
	movq	(%r8,%r14,8), %rdi
	addq	$1, %r14
	call	pthread_join
.L1850:
	movq	88(%rsp), %r9
	xorl	%esi, %esi
	movq	(%r9,%r14,8), %rdi
	addq	$1, %r14
	call	pthread_join
.L1849:
	movq	88(%rsp), %rdi
	xorl	%esi, %esi
	movq	(%rdi,%r14,8), %rdi
	addq	$1, %r14
	call	pthread_join
.L1848:
	movq	88(%rsp), %rsi
	movq	(%rsi,%r14,8), %rdi
	xorl	%esi, %esi
	addq	$1, %r14
	call	pthread_join
	cmpl	%r14d, 56(%rsp)
	jle	.L1712
.L1926:
	movq	88(%rsp), %r13
.L1713:
	movq	0(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	call	pthread_join
	movq	8(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	call	pthread_join
	movq	16(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	call	pthread_join
	movq	24(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	call	pthread_join
	movq	32(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	call	pthread_join
	movq	40(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	call	pthread_join
	movq	48(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	call	pthread_join
	movq	56(%r13,%r14,8), %rdi
	xorl	%esi, %esi
	addq	$8, %r14
	call	pthread_join
	cmpl	%r14d, 56(%rsp)
	jg	.L1713
	.p2align 4,,10
.L1712:
	movl	96(%rsp), %r11d
	testl	%r11d, %r11d
	jne	.L1715
	xorl	%eax, %eax
	call	fpar_deinit
	jmp	.L1715
	.p2align 4,,10
.L1714:
	movl	96(%rsp), %r10d
	movl	44(%rsp), %ecx
	xorl	%r9d, %r9d
	movq	80(%rsp), %rsi
	movl	60(%rsp), %edi
	movl	%r15d, %r8d
	movl	$1, (%rsp)
	movq	%rbp, %rdx
	movl	%r10d, 8(%rsp)
	call	translate_file
	jmp	.L1712
	.p2align 4,,10
.L1722:
	leaq	112(%rsp), %rdi
	xorl	%esi, %esi
	call	gettimeofday
	movq	72(%rsp), %rsi
	movl	$.LC75, %edi
	xorl	%eax, %eax
	call	printf
	xorl	%eax, %eax
	jmp	.L1897
	.p2align 4,,10
.L1944:
	movl	44(%rsp), %edx
	movl	$.LC70, %edi
	call	printf
	jmp	.L1810
	.p2align 4,,10
.L1945:
	movl	%r15d, %edx
	movl	$.LC72, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1813
	.p2align 4,,10
.L1915:
	movl	%esi, %edx
	movl	44(%rsp), %esi
	movl	$.LC69, %edi
	xorl	%eax, %eax
	call	printf
	movl	(%rbx), %esi
	jmp	.L1811
	.p2align 4,,10
.L1917:
	movl	%esi, %edx
	movl	$.LC71, %edi
	movl	%r15d, %esi
	xorl	%eax, %eax
	call	printf
	movl	0(%rbp), %esi
	jmp	.L1814
	.p2align 4,,10
.L1958:
	movq	72(%rsp), %rdi
	movl	$.LC64, %esi
	call	strcasestr
	testq	%rax, %rax
	jne	.L1716
	movq	72(%rsp), %rdi
	movl	$.LC65, %esi
	call	strcasestr
	testq	%rax, %rax
	je	.L1719
	movq	72(%rsp), %rcx
	movl	44(%rsp), %esi
	movl	%r15d, %edx
	movq	%rbp, %rdi
	call	save_png_file
	jmp	.L1718
	.p2align 4,,10
.L1693:
	movl	$.LC66, %esi
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1933
	movq	%r15, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_bmp_file
	jmp	.L1692
	.p2align 4,,10
.L1956:
	movl	$.LC66, %esi
	movq	%r15, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1933
	movq	%r15, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_bmp_file
	jmp	.L1918
.L1666:
	movl	$6, %ecx
	movl	$.LC48, %edi
	movq	%r12, %rsi
	repz; cmpsb
	seta	%r8b
	setb	%r9b
	subl	%r9d, %r8d
	movsbl	%r8b, %r8d
	testl	%r8d, %r8d
	jne	.L1668
	cmpl	$1, 60(%rsp)
	movb	$1, 103(%rsp)
	movl	$6, 96(%rsp)
	jg	.L1661
	jmp	.L1668
.L1940:
	movl	$8, %esi
	movl	$65535, %edi
	movl	$65535, %r14d
	call	calloc
	movl	$65535, %r15d
	movq	%rax, %rbp
	movl	$65535, 44(%rsp)
	jmp	.L1706
.L1669:
	movl	$6, %ecx
	movl	$.LC50, %edi
	movq	%r12, %rsi
	repz; cmpsb
	seta	%al
	setb	%r10b
	subl	%r10d, %eax
	movsbl	%al, %eax
	testl	%eax, %eax
	jne	.L1671
	cmpl	$2, 60(%rsp)
	jle	.L1671
	movb	$1, 103(%rsp)
	movl	$7, 96(%rsp)
	jmp	.L1661
.L1959:
	movl	$.LC66, %esi
	movq	%r12, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1694
	movq	48(%rsp), %rsi
	movq	80(%rsp), %rdi
	movq	%r12, %rcx
	movq	%rbp, %rdx
	call	load_bmp_file
	jmp	.L1907
.L1960:
	movl	$.LC66, %esi
	movq	%r12, %rdi
	call	strcasestr
	testq	%rax, %rax
	je	.L1694
	movq	%r12, %rcx
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movq	%r13, %rdi
	call	load_bmp_file
	jmp	.L1912
.L1733:
	movb	$1, 103(%rsp)
	movl	$8, 96(%rsp)
	jmp	.L1661
.L1731:
	movb	$1, 103(%rsp)
	movl	$3, 96(%rsp)
	jmp	.L1661
.L1719:
	movq	72(%rsp), %rdi
	movl	$.LC66, %esi
	call	strcasestr
	testq	%rax, %rax
	je	.L1720
	movq	72(%rsp), %rcx
	movl	44(%rsp), %esi
	movl	%r15d, %edx
	movq	%rbp, %rdi
	call	save_bmp_file
	jmp	.L1718
.L1734:
	movb	$1, 103(%rsp)
	movl	$9, 96(%rsp)
	jmp	.L1661
.L1672:
	testl	%r14d, %r14d
	jne	.L1673
	cmpl	$1, 60(%rsp)
	jle	.L1961
.L1673:
	testl	%r15d, %r15d
	jne	.L1674
	cmpl	$2, 60(%rsp)
	jle	.L1962
.L1674:
	testl	%edx, %edx
	jne	.L1675
	cmpl	$1, 60(%rsp)
	jle	.L1963
.L1675:
	testl	%r9d, %r9d
	jne	.L1676
	cmpl	$2, 60(%rsp)
	jle	.L1964
.L1676:
	testl	%r8d, %r8d
	jne	.L1677
	cmpl	$1, 60(%rsp)
	jle	.L1965
.L1677:
	testl	%eax, %eax
	jne	.L1678
	cmpl	$2, 60(%rsp)
	jle	.L1966
.L1678:
	testl	%ecx, %ecx
	.p2align 4,,2
	jne	.L1737
	cmpl	$5, 60(%rsp)
	jle	.L1967
.L1737:
	movb	$1, 103(%rsp)
	movl	$0, 96(%rsp)
	jmp	.L1661
.L1720:
	movq	72(%rsp), %rsi
	movl	$.LC67, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L1721
.L1962:
	movl	$.LC55, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1897
.L1961:
	movl	$.LC54, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1897
.L1963:
	movl	$.LC56, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1897
.L1964:
	movl	$.LC57, %edi
	call	puts
	movl	$64, %eax
	jmp	.L1897
.L1965:
	movl	$.LC58, %edi
	call	puts
	movl	$32, %eax
	jmp	.L1897
.L1966:
	movl	$.LC59, %edi
	call	puts
	movl	$64, %eax
	jmp	.L1897
.L1967:
	movl	$.LC60, %edi
	call	puts
	movl	$64, %eax
	jmp	.L1897
	.cfi_endproc
.LFE71:
	.size	fjpg, .-fjpg
	.p2align 4,,15
	.globl	separate_options
	.type	separate_options, @function
separate_options:
.LFB72:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%r9, %r14
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%rcx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%r8, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rsi, %rbx
	subq	$72, %rsp
	.cfi_def_cfa_offset 128
	movl	%edi, 40(%rsp)
	movslq	%edi, %rdi
	movq	%rdx, 56(%rsp)
	addq	%rdi, %rdi
	call	malloc
	cmpl	$1, 40(%rsp)
	movq	56(%rsp), %rdx
	movq	%rax, %r15
	movl	$1, 0(%rbp)
	movl	$1, (%rdx)
	jle	.L1982
	leaq	2(%rax), %rsi
	movl	40(%rsp), %eax
	leaq	8(%rbx), %r12
	movq	(%r12), %r9
	movq	%rsi, 8(%rsp)
	subl	$2, %eax
	leaq	16(%rbx,%rax,8), %rcx
	movq	%r9, %rdi
	movq	%r9, 16(%rsp)
	movq	%rcx, %r8
	movq	%rcx, 48(%rsp)
	subq	%r12, %r8
	subq	$8, %r8
	shrq	$3, %r8
	andl	$3, %r8d
	movq	%r8, (%rsp)
	call	strlen
	cmpl	$1, %eax
	movq	16(%rsp), %r10
	movq	(%rsp), %r11
	movq	8(%rsp), %rsi
	jle	.L2039
	cmpb	$45, (%r10)
	je	.L2074
.L2039:
	xorl	%r8d, %r8d
	addl	$1, 0(%rbp)
	movw	%r8w, (%rsi)
.L1992:
	leaq	8(%r12), %rdx
	cmpq	48(%rsp), %rdx
	leaq	2(%rsi), %r12
	je	.L1989
	testq	%r11, %r11
	je	.L1976
	cmpq	$1, %r11
	je	.L2033
	cmpq	$2, %r11
	je	.L2034
	movq	(%rdx), %r9
	movq	%rdx, 16(%rsp)
	movq	%r9, %rdi
	movq	%r9, (%rsp)
	call	strlen
	cmpl	$1, %eax
	movq	16(%rsp), %rdx
	movq	(%rsp), %r10
	jle	.L2050
	cmpb	$45, (%r10)
	je	.L2075
.L2050:
	xorl	%ecx, %ecx
	addl	$1, 0(%rbp)
	movw	%cx, (%r12)
.L1997:
	addq	$8, %rdx
	addq	$2, %r12
.L2034:
	movq	(%rdx), %r8
	movq	%rdx, 16(%rsp)
	movq	%r8, %rdi
	movq	%r8, (%rsp)
	call	strlen
	cmpl	$1, %eax
	movq	16(%rsp), %rdx
	movq	(%rsp), %r9
	jle	.L2054
	cmpb	$45, (%r9)
	je	.L2076
.L2054:
	xorl	%eax, %eax
	addl	$1, 0(%rbp)
	movw	%ax, (%r12)
.L2003:
	addq	$8, %rdx
	addq	$2, %r12
.L2033:
	movq	(%rdx), %rcx
	movq	%rdx, 16(%rsp)
	movq	%rcx, %rdi
	movq	%rcx, (%rsp)
	call	strlen
	cmpl	$1, %eax
	movq	16(%rsp), %rdx
	movq	(%rsp), %r8
	jle	.L2058
	cmpb	$45, (%r8)
	je	.L2077
.L2058:
	xorl	%edi, %edi
	addl	$1, 0(%rbp)
	movw	%di, (%r12)
.L2009:
	addq	$8, %rdx
	addq	$2, %r12
	cmpq	48(%rsp), %rdx
	jne	.L1976
.L1989:
	movq	56(%rsp), %rdi
	movslq	(%rdi), %r12
	salq	$3, %r12
.L1969:
	movslq	0(%rbp), %rdi
	salq	$3, %rdi
	call	malloc
	movq	%r12, %rdi
	movq	%rax, (%r14)
	call	malloc
	movq	%rax, 0(%r13)
	movq	(%rbx), %rdi
	movq	(%r14), %rbp
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	movq	(%rbx), %rdi
	movq	0(%r13), %r12
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	%rax, (%r12)
	movq	(%r14), %rax
	movq	(%rbx), %rsi
	movq	(%rax), %rdi
	call	strcpy
	movq	0(%r13), %r8
	movq	(%rbx), %rsi
	movq	(%r8), %rdi
	call	strcpy
	cmpl	$1, 40(%rsp)
	jle	.L1968
	movl	40(%rsp), %ecx
	movl	$1, %r12d
	movl	$1, 24(%rsp)
	subl	$2, %ecx
	leaq	2(%rcx,%rcx), %r9
	andl	$1, %ecx
	cmpw	$0, 2(%r15)
	movq	%rcx, 40(%rsp)
	movq	%r9, 32(%rsp)
	jne	.L2078
	movq	8(%rbx), %rdi
	movq	(%r14), %rbp
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	%rax, 8(%rbp)
	movq	(%r14), %r10
	movq	8(%rbx), %rsi
	movq	8(%r10), %rdi
	call	strcpy
	movl	$2, 24(%rsp)
.L2043:
	movl	$2, %ebp
	cmpq	32(%rsp), %rbp
	je	.L1968
	cmpq	$0, 40(%rsp)
	je	.L1981
	cmpw	$0, 2(%r15,%rbp)
	jne	.L2044
	movslq	24(%rsp), %rdx
	movq	(%r14), %rsi
	movq	8(%rbx,%rbp,4), %rdi
	salq	$3, %rdx
	addq	%rdx, %rsi
	movq	%rdx, (%rsp)
	movq	%rsi, 8(%rsp)
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	8(%rsp), %rdi
	movq	(%rsp), %r8
	movq	%rax, (%rdi)
	movq	(%r14), %rax
	movq	8(%rbx,%rbp,4), %rsi
	movq	(%rax,%r8), %rdi
	call	strcpy
	addl	$1, 24(%rsp)
	.p2align 4,,10
.L2046:
	addq	$2, %rbp
	cmpq	32(%rsp), %rbp
	je	.L1968
.L1981:
	cmpw	$0, 2(%r15,%rbp)
	je	.L1978
	movq	0(%r13), %r11
	movslq	%r12d, %r10
	movq	8(%rbx,%rbp,4), %rdi
	movq	%r10, (%rsp)
	addl	$1, %r12d
	leaq	(%r11,%r10,8), %rdx
	movq	%rdx, 8(%rsp)
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	8(%rsp), %r8
	movq	(%rsp), %rdi
	movq	%rax, (%r8)
	movq	0(%r13), %rax
	movq	8(%rbx,%rbp,4), %rsi
	movq	(%rax,%rdi,8), %rdi
	call	strcpy
.L1979:
	addq	$2, %rbp
	cmpw	$0, 2(%r15,%rbp)
	je	.L1987
	movq	0(%r13), %rax
	movslq	%r12d, %r8
	movq	8(%rbx,%rbp,4), %rdi
	movq	%r8, (%rsp)
	addl	$1, %r12d
	leaq	(%rax,%r8,8), %r9
	movq	%r9, 8(%rsp)
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	8(%rsp), %rdi
	movq	(%rsp), %rcx
	movq	%rax, (%rdi)
	movq	0(%r13), %r10
	movq	8(%rbx,%rbp,4), %rsi
	addq	$2, %rbp
	movq	(%r10,%rcx,8), %rdi
	call	strcpy
	cmpq	32(%rsp), %rbp
	jne	.L1981
.L1968:
	addq	$72, %rsp
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
	.p2align 4,,10
.L2013:
	.cfi_restore_state
	cmpb	$45, (%r8)
	je	.L2079
.L2062:
	movq	24(%rsp), %rsi
	xorl	%edi, %edi
	addl	$1, 0(%rbp)
	movw	%di, (%rsi)
.L2015:
	movq	32(%rsp), %rdx
	movq	8(%rdx), %r12
	movq	%r12, %rdi
	call	strlen
	cmpl	$1, %eax
	jle	.L2065
	cmpb	$45, (%r12)
	je	.L2080
.L2065:
	movq	24(%rsp), %rsi
	xorl	%edi, %edi
	addl	$1, 0(%rbp)
	movw	%di, 2(%rsi)
.L2020:
	movq	32(%rsp), %rax
	movq	16(%rax), %r12
	movq	%r12, %rdi
	call	strlen
	cmpl	$1, %eax
	jle	.L2069
	cmpb	$45, (%r12)
	je	.L2081
.L2069:
	movq	24(%rsp), %r11
	xorl	%esi, %esi
	addl	$1, 0(%rbp)
	movw	%si, 4(%r11)
.L2025:
	movq	32(%rsp), %rdx
	movq	24(%rsp), %r12
	addq	$24, %rdx
	addq	$6, %r12
	cmpq	48(%rsp), %rdx
	je	.L1989
.L1976:
	movq	(%rdx), %rax
	movq	%rdx, 16(%rsp)
	movq	%rax, %rdi
	movq	%rax, (%rsp)
	call	strlen
	cmpl	$1, %eax
	movq	16(%rsp), %rdx
	movq	(%rsp), %rcx
	jle	.L1974
	cmpb	$45, (%rcx)
	je	.L1972
.L1974:
	xorl	%esi, %esi
	addl	$1, 0(%rbp)
	movw	%si, (%r12)
.L1971:
	leaq	8(%rdx), %rdi
	movq	8(%rdx), %rdx
	leaq	2(%r12), %rax
	movq	%rdi, 32(%rsp)
	movq	%rax, 24(%rsp)
	movq	%rdx, %rdi
	movq	%rdx, 16(%rsp)
	call	strlen
	cmpl	$1, %eax
	movq	16(%rsp), %r8
	jg	.L2013
	xorl	%eax, %eax
	addl	$1, 0(%rbp)
	movw	%ax, 2(%r12)
	jmp	.L2015
	.p2align 4,,10
.L2078:
	movq	8(%rbx), %rdi
	movq	0(%r13), %r12
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	%rax, 8(%r12)
	movq	0(%r13), %r11
	movl	$2, %r12d
	movq	8(%rbx), %rsi
	movq	8(%r11), %rdi
	call	strcpy
	jmp	.L2043
	.p2align 4,,10
.L1987:
	movslq	24(%rsp), %r9
	movq	(%r14), %rsi
	movq	8(%rbx,%rbp,4), %rdi
	leaq	(%rsi,%r9,8), %rcx
	movq	%r9, (%rsp)
	movq	%rcx, 8(%rsp)
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	8(%rsp), %r10
	movq	(%rsp), %rdx
	movq	%rax, (%r10)
	movq	(%r14), %r11
	movq	8(%rbx,%rbp,4), %rsi
	movq	(%r11,%rdx,8), %rdi
	call	strcpy
	addl	$1, 24(%rsp)
	jmp	.L2046
	.p2align 4,,10
.L1978:
	movslq	24(%rsp), %r8
	movq	(%r14), %rsi
	movq	8(%rbx,%rbp,4), %rdi
	leaq	(%rsi,%r8,8), %rax
	movq	%r8, (%rsp)
	movq	%rax, 8(%rsp)
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	8(%rsp), %rdi
	movq	(%rsp), %rcx
	movq	%rax, (%rdi)
	movq	(%r14), %r9
	movq	8(%rbx,%rbp,4), %rsi
	movq	(%r9,%rcx,8), %rdi
	call	strcpy
	addl	$1, 24(%rsp)
	jmp	.L1979
	.p2align 4,,10
.L2079:
	movzbl	1(%r8), %r12d
	andl	$-33, %r12d
	subl	$65, %r12d
	cmpb	$22, %r12b
	ja	.L2015
	cmpl	$2, %eax
	je	.L2016
	movzbl	2(%r8), %ecx
	cmpb	$47, %cl
	jle	.L2062
	cmpb	$57, %cl
	jg	.L2062
	.p2align 4,,10
.L2016:
	movq	56(%rsp), %r9
	movq	24(%rsp), %r10
	movl	$1, %r11d
	addl	$1, (%r9)
	movw	%r11w, (%r10)
	jmp	.L2015
	.p2align 4,,10
.L1972:
	movzbl	1(%rcx), %r8d
	andl	$-33, %r8d
	subl	$65, %r8d
	cmpb	$22, %r8b
	ja	.L1971
	cmpl	$2, %eax
	je	.L1973
	movzbl	2(%rcx), %r9d
	cmpb	$47, %r9b
	jle	.L1974
	cmpb	$57, %r9b
	jg	.L1974
	.p2align 4,,10
.L1973:
	movq	56(%rsp), %r10
	movl	$1, %r11d
	movw	%r11w, (%r12)
	addl	$1, (%r10)
	jmp	.L1971
	.p2align 4,,10
.L2081:
	movzbl	1(%r12), %edx
	andl	$-33, %edx
	subl	$65, %edx
	cmpb	$22, %dl
	ja	.L2025
	cmpl	$2, %eax
	je	.L2026
	movzbl	2(%r12), %r8d
	cmpb	$47, %r8b
	jle	.L2069
	cmpb	$57, %r8b
	jg	.L2069
	.p2align 4,,10
.L2026:
	movq	56(%rsp), %rcx
	movq	24(%rsp), %r9
	movl	$1, %r10d
	addl	$1, (%rcx)
	movw	%r10w, 4(%r9)
	jmp	.L2025
	.p2align 4,,10
.L2080:
	movzbl	1(%r12), %r8d
	andl	$-33, %r8d
	subl	$65, %r8d
	cmpb	$22, %r8b
	ja	.L2020
	cmpl	$2, %eax
	je	.L2021
	movzbl	2(%r12), %ecx
	cmpb	$47, %cl
	jle	.L2065
	cmpb	$57, %cl
	jg	.L2065
	.p2align 4,,10
.L2021:
	movq	56(%rsp), %r9
	movq	24(%rsp), %r10
	movl	$1, %r11d
	addl	$1, (%r9)
	movw	%r11w, 2(%r10)
	jmp	.L2020
	.p2align 4,,10
.L2044:
	movq	0(%r13), %r9
	movq	8(%rbx,%rbp,4), %rdi
	movslq	%r12d, %rcx
	salq	$3, %rcx
	addl	$1, %r12d
	movq	%rcx, (%rsp)
	addq	%rcx, %r9
	movq	%r9, 8(%rsp)
	call	strlen
	leaq	1(%rax), %rdi
	call	malloc
	movq	8(%rsp), %r10
	movq	(%rsp), %rdx
	movq	%rax, (%r10)
	movq	0(%r13), %r11
	movq	8(%rbx,%rbp,4), %rsi
	movq	(%r11,%rdx), %rdi
	call	strcpy
	jmp	.L2046
	.p2align 4,,10
.L2074:
	movzbl	1(%r10), %edi
	andl	$-33, %edi
	subl	$65, %edi
	cmpb	$22, %dil
	ja	.L1992
	cmpl	$2, %eax
	je	.L1993
	movzbl	2(%r10), %edx
	cmpb	$47, %dl
	jle	.L2039
	cmpb	$57, %dl
	jg	.L2039
.L1993:
	movq	56(%rsp), %rax
	movl	$1, %ecx
	movw	%cx, (%rsi)
	addl	$1, (%rax)
	jmp	.L1992
	.p2align 4,,10
.L2075:
	movzbl	1(%r10), %r11d
	andl	$-33, %r11d
	subl	$65, %r11d
	cmpb	$22, %r11b
	ja	.L1997
	cmpl	$2, %eax
	je	.L1998
	movzbl	2(%r10), %esi
	cmpb	$47, %sil
	jle	.L2050
	cmpb	$57, %sil
	jg	.L2050
.L1998:
	movq	56(%rsp), %rdi
	movl	$1, %eax
	movw	%ax, (%r12)
	addl	$1, (%rdi)
	jmp	.L1997
	.p2align 4,,10
.L2076:
	movzbl	1(%r9), %r10d
	andl	$-33, %r10d
	subl	$65, %r10d
	cmpb	$22, %r10b
	ja	.L2003
	cmpl	$2, %eax
	je	.L2004
	movzbl	2(%r9), %r11d
	cmpb	$47, %r11b
	jle	.L2054
	cmpb	$57, %r11b
	jg	.L2054
.L2004:
	movq	56(%rsp), %rsi
	movl	$1, %edi
	movw	%di, (%r12)
	addl	$1, (%rsi)
	jmp	.L2003
	.p2align 4,,10
.L2077:
	movzbl	1(%r8), %r9d
	andl	$-33, %r9d
	subl	$65, %r9d
	cmpb	$22, %r9b
	ja	.L2009
	cmpl	$2, %eax
	je	.L2010
	movzbl	2(%r8), %r10d
	cmpb	$47, %r10b
	jle	.L2058
	cmpb	$57, %r10b
	jg	.L2058
.L2010:
	movq	56(%rsp), %r11
	movl	$1, %esi
	movw	%si, (%r12)
	addl	$1, (%r11)
	jmp	.L2009
	.p2align 4,,10
.L1982:
	movl	$8, %r12d
	jmp	.L1969
	.cfi_endproc
.LFE72:
	.size	separate_options, .-separate_options
	.section	.rodata.str1.8
	.align 8
.LC76:
	.string	"%s outfile.jpg 'equation' n_threads infile(s).jpg [...] [-a|-n|-q val]\n"
	.align 8
.LC77:
	.string	"\t-a\tdisable faster but singlethread assembler loop processing"
	.align 8
.LC78:
	.string	"\t-n\tdisable automatic output file overwrite"
	.section	.rodata.str1.1
.LC79:
	.string	"\t-q\tjpeg quality[100]"
	.text
	.p2align 4,,15
	.globl	usage
	.type	usage, @function
usage:
.LFB73:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	%rdi, %rsi
	xorl	%eax, %eax
	movl	$.LC76, %edi
	call	printf
	movl	$.LC77, %edi
	call	puts
	movl	$.LC78, %edi
	call	puts
	movl	$.LC79, %edi
	call	puts
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	fpar_info
	.cfi_endproc
.LFE73:
	.size	usage, .-usage
	.section	.rodata.str1.1
.LC80:
	.string	"Unrecognized option"
.LC81:
	.string	"hanq:"
.LC82:
	.string	"%d.%06ds\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB74:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rsi, %rbp
	xorl	%esi, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movl	%edi, %ebx
	subq	$104, %rsp
	.cfi_def_cfa_offset 160
	leaq	64(%rsp), %rdi
	call	gettimeofday
	leaq	32(%rsp), %r9
	leaq	48(%rsp), %rcx
	leaq	16(%rsp), %rdx
	movq	%rsp, %r8
	movq	%rbp, %rsi
	movl	%ebx, %edi
	call	separate_options
.L2099:
	movq	48(%rsp), %rsi
	movl	16(%rsp), %edi
	movl	$.LC81, %edx
	call	getopt
	cmpb	$-1, %al
	je	.L2101
	cmpb	$104, %al
	je	.L2089
	.p2align 4,,7
	jg	.L2091
	cmpb	$97, %al
	.p2align 4,,7
	je	.L2102
.L2086:
	movl	$.LC80, %edi
	movl	$1, %r12d
	call	puts
.L2093:
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	%r12d, %eax
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
	.p2align 4,,10
.L2091:
	.cfi_restore_state
	cmpb	$110, %al
	je	.L2088
	cmpb	$113, %al
	jne	.L2086
	movq	optarg(%rip), %rdi
	call	atoi
	movl	%eax, jqual(%rip)
	jmp	.L2099
.L2088:
	movl	$0, overwrite_mode(%rip)
	jmp	.L2099
.L2102:
	movl	$0, ualoop(%rip)
	jmp	.L2099
.L2089:
	movq	32(%rsp), %r8
	xorl	%r12d, %r12d
	movq	(%r8), %rdi
	call	usage
	jmp	.L2093
.L2101:
	movl	(%rsp), %ebp
	cmpl	$3, %ebp
	jg	.L2095
	movq	32(%rsp), %rsi
	movl	$32, %r12d
	movq	(%rsi), %rdi
	call	usage
	jmp	.L2093
.L2095:
	leal	-4(%rbp), %r14d
	xorl	%ebx, %ebx
	orq	$-1, %r13
	movslq	%r14d, %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, %r12
	jmp	.L2096
.L2097:
	leaq	32(%rdx,%rbx,8), %r15
	xorl	%eax, %eax
	movq	%r13, %rcx
	movq	(%r15), %rdi
	repnz; scasb
	notq	%rcx
	movq	%rcx, %rdi
	call	malloc
	movq	%rax, (%r12,%rbx,8)
	movq	(%r15), %rsi
	movq	%rax, %rdi
	addq	$1, %rbx
	call	strcpy
.L2096:
	leal	4(%rbx), %eax
	movq	32(%rsp), %rdx
	cmpl	%eax, %ebp
	jg	.L2097
	movq	24(%rdx), %rdi
	call	atoi
	movq	32(%rsp), %rcx
	movl	%r14d, %edx
	movl	%eax, %r8d
	movq	16(%rcx), %rsi
	movq	8(%rcx), %rdi
	movq	%r12, %rcx
	call	fjpg
	leaq	80(%rsp), %rdi
	xorl	%esi, %esi
	movl	%eax, %r12d
	call	gettimeofday
	movq	80(%rsp), %rsi
	movq	88(%rsp), %rdx
	subq	64(%rsp), %rsi
	subq	72(%rsp), %rdx
	js	.L2103
.L2098:
	movl	$.LC82, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L2093
.L2103:
	subq	$1, %rsi
	addq	$1000000, %rdx
	jmp	.L2098
	.cfi_endproc
.LFE74:
	.size	main, .-main
	.comm	ncache,4,4
	.comm	icache,8,8
	.local	errptr
	.comm	errptr,8,8
	.globl	overwrite_mode
	.data
	.align 16
	.type	overwrite_mode, @object
	.size	overwrite_mode, 4
overwrite_mode:
	.long	1
	.globl	ualoop
	.align 16
	.type	ualoop, @object
	.size	ualoop, 4
ualoop:
	.long	1
	.globl	jqual
	.align 4
	.type	jqual, @object
	.size	jqual, 4
jqual:
	.long	100
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
	.ident	"GCC: (FreeBSD Ports Collection) 4.8.0 20120805 (experimental)"
	.section	.note.GNU-stack,"",@progbits
