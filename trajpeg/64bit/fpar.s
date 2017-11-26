	.file	"fpar.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Parser Exception: %s\n\n"
.LC1:
	.string	"Buffer[%d] = '%s'\n"
.LC2:
	.string	"fpar.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"Occured at: File: %s, Line: %d\n"
	.text
	.type	exception.clone.0, @function
exception.clone.0:
.LFB32:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	movl	%edi, %r12d
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movl	%esi, %ebp
	movq	%rdx, %rsi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movslq	%ebp, %rbx
	movq	buffer(%rip), %rdx
	movq	(%rdx,%rbx,8), %rdx
	movl	%ebp, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	%r12d, %edx
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rax
	movl	$1, (%rax,%rbx,4)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE32:
	.size	exception.clone.0, .-exception.clone.0
.globl fpar_init
	.type	fpar_init, @function
fpar_init:
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
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	movl	%edi, %ebx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	testl	%edi, %edi
	js	.L12
	movslq	%edi, %r13
	leaq	0(,%r13,8), %rax
	movq	%rax, 8(%rsp)
	movq	%rax, %rdi
	call	malloc
	movq	%rax, %rbp
	movq	%rax, buffer(%rip)
	testq	%rax, %rax
	je	.L12
	leaq	0(,%r13,4), %r15
	movq	%r15, %rdi
	call	malloc
	movq	%rax, %r12
	movq	%rax, position(%rip)
	movq	%r13, %rdi
	call	malloc
	movq	%rax, ch(%rip)
	movq	%r15, %rdi
	call	malloc
	movq	%rax, maxpos(%rip)
	movq	8(%rsp), %rdi
	call	malloc
	movq	%rax, 16(%rsp)
	movq	%rax, y(%rip)
	movq	%r15, %rdi
	call	malloc
	movq	%rax, %r13
	movq	%rax, err(%rip)
	movq	%r15, %rdi
	call	malloc
	movq	%rax, %r14
	movq	%rax, nvar(%rip)
	movq	%r15, %rdi
	call	malloc
	movq	%rax, 24(%rsp)
	movq	%rax, ranks(%rip)
	movq	8(%rsp), %rdi
	call	malloc
	movq	%rax, t(%rip)
	movl	%ebx, N(%rip)
	testl	%ebx, %ebx
	je	.L4
	movq	%rbp, %rax
	andl	$15, %eax
	shrq	$3, %rax
	cmpl	%ebx, %eax
	cmova	%ebx, %eax
	mov	%eax, %edi
	testl	%eax, %eax
	je	.L13
	movq	$0, 0(%rbp)
	movl	$0, (%r12)
	movl	$0, 0(%r13)
	movl	$1, (%r14)
	movq	24(%rsp), %rcx
	movl	$1, (%rcx)
	movq	16(%rsp), %rdx
	movq	$0, (%rdx)
	cmpl	%ebx, %eax
	je	.L4
	movl	$1, 8(%rsp)
.L5:
	movl	%ebx, %r8d
	subl	%eax, %r8d
	movl	%r8d, 40(%rsp)
	movl	%r8d, %r11d
	shrl	$2, %r11d
	movl	%r11d, %esi
	sall	$2, %esi
	movl	%esi, 44(%rsp)
	je	.L7
	leaq	0(,%rdi,8), %r15
	leaq	0(%rbp,%r15), %rcx
	salq	$2, %rdi
	leaq	(%r12,%rdi), %r10
	leaq	0(%r13,%rdi), %r9
	leaq	(%r14,%rdi), %r8
	addq	24(%rsp), %rdi
	addq	16(%rsp), %r15
	xorl	%eax, %eax
	movdqa	.LC4(%rip), %xmm2
	pxor	%xmm1, %xmm1
	pxor	%xmm0, %xmm0
	leal	-1(%r11), %edx
	andl	$3, %edx
	movl	%edx, 36(%rsp)
	movdqa	%xmm1, (%rcx,%rax,2)
	movdqa	%xmm1, 16(%rcx,%rax,2)
	movdqu	%xmm0, (%r10)
	movdqu	%xmm0, (%r9)
	movdqu	%xmm2, (%r8)
	movdqu	%xmm2, (%rdi)
	movdqu	%xmm1, (%r15)
	movdqu	%xmm0, 16(%r15)
	movl	$1, %esi
	movb	$16, %al
	leaq	32(%r15), %rdx
	cmpl	%r11d, %esi
	jae	.L42
	cmpl	$0, 36(%rsp)
	je	.L8
	cmpl	$1, 36(%rsp)
	je	.L40
	cmpl	$2, 36(%rsp)
	je	.L41
	movdqa	%xmm1, (%rcx,%rax,2)
	movdqa	%xmm1, 48(%rcx)
	movdqu	%xmm0, 16(%r10)
	movdqu	%xmm0, 16(%r9)
	movdqu	%xmm2, 16(%r8)
	movdqu	%xmm2, 16(%rdi)
	movdqu	%xmm0, (%rdx)
	movdqu	%xmm0, 16(%rdx)
	movl	$2, %esi
	movl	$32, %eax
	leaq	64(%r15), %rdx
.L41:
	movdqa	%xmm1, (%rcx,%rax,2)
	movdqa	%xmm1, 16(%rcx,%rax,2)
	movdqu	%xmm0, (%r10,%rax)
	movdqu	%xmm0, (%r9,%rax)
	movdqu	%xmm2, (%r8,%rax)
	movdqu	%xmm2, (%rdi,%rax)
	movdqu	%xmm1, (%rdx)
	movdqu	%xmm0, 16(%rdx)
	addl	$1, %esi
	addq	$16, %rax
	addq	$32, %rdx
.L40:
	movdqa	%xmm1, (%rcx,%rax,2)
	movdqa	%xmm1, 16(%rcx,%rax,2)
	movdqu	%xmm0, (%r10,%rax)
	movdqu	%xmm0, (%r9,%rax)
	movdqu	%xmm2, (%r8,%rax)
	movdqu	%xmm2, (%rdi,%rax)
	movdqu	%xmm1, (%rdx)
	movdqu	%xmm0, 16(%rdx)
	addl	$1, %esi
	addq	$16, %rax
	addq	$32, %rdx
	cmpl	%r11d, %esi
	jae	.L42
.L8:
	movdqa	%xmm1, (%rcx,%rax,2)
	movdqa	%xmm1, 16(%rcx,%rax,2)
	movdqu	%xmm0, (%r10,%rax)
	movdqu	%xmm0, (%r9,%rax)
	movdqu	%xmm2, (%r8,%rax)
	movdqu	%xmm2, (%rdi,%rax)
	movdqu	%xmm1, (%rdx)
	movdqu	%xmm0, 16(%rdx)
	leaq	16(%rax), %r15
	movdqa	%xmm1, (%rcx,%r15,2)
	movdqa	%xmm1, 16(%rcx,%r15,2)
	movdqu	%xmm0, (%r10,%r15)
	movdqu	%xmm0, (%r9,%r15)
	movdqu	%xmm2, (%r8,%r15)
	movdqu	%xmm2, (%rdi,%r15)
	movdqu	%xmm0, 32(%rdx)
	movdqu	%xmm0, 48(%rdx)
	leaq	32(%rax), %r15
	movdqa	%xmm1, (%rcx,%r15,2)
	movdqa	%xmm1, 16(%rcx,%r15,2)
	movdqu	%xmm0, (%r10,%r15)
	movdqu	%xmm0, (%r9,%r15)
	movdqu	%xmm2, (%r8,%r15)
	movdqu	%xmm2, (%rdi,%r15)
	movdqu	%xmm0, 64(%rdx)
	movdqu	%xmm0, 80(%rdx)
	leaq	48(%rax), %r15
	movdqa	%xmm1, (%rcx,%r15,2)
	movdqa	%xmm1, 16(%rcx,%r15,2)
	movdqu	%xmm0, (%r10,%r15)
	movdqu	%xmm0, (%r9,%r15)
	movdqu	%xmm2, (%r8,%r15)
	movdqu	%xmm2, (%rdi,%r15)
	movdqu	%xmm0, 96(%rdx)
	movdqu	%xmm0, 112(%rdx)
	addl	$4, %esi
	addq	$64, %rax
	subq	$-128, %rdx
	cmpl	%r11d, %esi
	jb	.L8
.L42:
	movl	44(%rsp), %edi
	addl	%edi, 8(%rsp)
	cmpl	%edi, 40(%rsp)
	je	.L4
.L7:
	movslq	8(%rsp), %r11
	leaq	0(,%r11,8), %r15
	addq	%r15, %rbp
	salq	$2, %r11
	addq	%r11, %r12
	addq	%r11, %r13
	addq	%r11, %r14
	addq	24(%rsp), %r11
	addq	16(%rsp), %r15
	movl	8(%rsp), %r10d
	notl	%r10d
	addl	%ebx, %r10d
	andl	$3, %r10d
	movq	$0, 0(%rbp)
	movl	$0, (%r12)
	movl	$0, 0(%r13)
	movl	$1, (%r14)
	movl	$1, (%r11)
	movq	$0, (%r15)
	movl	8(%rsp), %r9d
	addl	$1, %r9d
	leaq	8(%rbp), %r8
	leaq	4(%r12), %rdi
	leaq	4(%r13), %rsi
	leaq	4(%r14), %rcx
	leaq	4(%r11), %rdx
	leaq	8(%r15), %rax
	cmpl	%r9d, %ebx
	jle	.L4
	testl	%r10d, %r10d
	je	.L10
	cmpl	$1, %r10d
	je	.L38
	cmpl	$2, %r10d
	je	.L39
	movq	$0, (%r8)
	movl	$0, (%rdi)
	movl	$0, (%rsi)
	movl	$1, (%rcx)
	movl	$1, (%rdx)
	movq	$0, (%rax)
	movl	8(%rsp), %r9d
	addl	$2, %r9d
	leaq	16(%rbp), %r8
	leaq	8(%r12), %rdi
	leaq	8(%r13), %rsi
	leaq	8(%r14), %rcx
	leaq	8(%r11), %rdx
	leaq	16(%r15), %rax
.L39:
	movq	$0, (%r8)
	movl	$0, (%rdi)
	movl	$0, (%rsi)
	movl	$1, (%rcx)
	movl	$1, (%rdx)
	movq	$0, (%rax)
	addl	$1, %r9d
	addq	$8, %r8
	addq	$4, %rdi
	addq	$4, %rsi
	addq	$4, %rcx
	addq	$4, %rdx
	addq	$8, %rax
.L38:
	movq	$0, (%r8)
	movl	$0, (%rdi)
	movl	$0, (%rsi)
	movl	$1, (%rcx)
	movl	$1, (%rdx)
	movq	$0, (%rax)
	addl	$1, %r9d
	addq	$8, %r8
	addq	$4, %rdi
	addq	$4, %rsi
	addq	$4, %rcx
	addq	$4, %rdx
	addq	$8, %rax
	cmpl	%r9d, %ebx
	jle	.L4
.L10:
	movq	$0, (%r8)
	movl	$0, (%rdi)
	movl	$0, (%rsi)
	movl	$1, (%rcx)
	movl	$1, (%rdx)
	movq	$0, (%rax)
	movq	$0, 8(%r8)
	movl	$0, 4(%rdi)
	movl	$0, 4(%rsi)
	movl	$1, 4(%rcx)
	movl	$1, 4(%rdx)
	movq	$0, 8(%rax)
	movq	$0, 16(%r8)
	movl	$0, 8(%rdi)
	movl	$0, 8(%rsi)
	movl	$1, 8(%rcx)
	movl	$1, 8(%rdx)
	movq	$0, 16(%rax)
	movq	$0, 24(%r8)
	movl	$0, 12(%rdi)
	movl	$0, 12(%rsi)
	movl	$1, 12(%rcx)
	movl	$1, 12(%rdx)
	movq	$0, 24(%rax)
	addl	$4, %r9d
	addq	$32, %r8
	addq	$16, %rdi
	addq	$16, %rsi
	addq	$16, %rcx
	addq	$16, %rdx
	addq	$32, %rax
	cmpl	%r9d, %ebx
	jg	.L10
.L4:
	movq	$0, gcnt(%rip)
	movl	$1, %eax
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
.L12:
	.cfi_restore_state
	xorl	%eax, %eax
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
.L13:
	.cfi_restore_state
	movl	$0, 8(%rsp)
	jmp	.L5
	.cfi_endproc
.LFE16:
	.size	fpar_init, .-fpar_init
.globl exception
	.type	exception, @function
exception:
.LFB17:
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
	movq	%rdi, %rbp
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movl	%esi, %r12d
	movl	%edx, %ebx
	movq	%rcx, %rsi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movslq	%ebx, %r13
	movq	buffer(%rip), %rdx
	movq	(%rdx,%r13,8), %rdx
	movl	%ebx, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	%r12d, %edx
	movq	%rbp, %rsi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rax
	movl	$1, (%rax,%r13,4)
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
	ret
	.cfi_endproc
.LFE17:
	.size	exception, .-exception
	.section	.rodata.str1.8
	.align 8
.LC5:
	.ascii	"supported functions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt"
	.ascii	",abs\nsupported functions: asin,acos,asinh"
	.string	",acosh,sinh,cosh,cbrt,ceil,sgn\nsupported functions: tanh,atan,atanh,tgh,atg,atgh,neg\nsupported bin-functions: max,min,mina,minb,maxa,maxb,and,or,xor,pow,mod\nsupported tri-functions: if(cond,cond>0,cond<=0)\nsupported rgb-functions: gr,gg,gb,sr,sg,sb,ggs,sgs"
	.align 8
.LC6:
	.string	"supported operators: +,-,*,/,^, unary -\nsupported user defined function: asmf[123](lx), udf1(x), udf2(x,y), udf3(x,y,z)\nand more...(see udf.c), special \"doing nothing\" none"
	.section	.rodata.str1.1
.LC7:
	.string	"blank characters are skipped"
	.text
.globl fpar_info
	.type	fpar_info, @function
fpar_info:
.LFB18:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$.LC5, %edi
	call	puts
	movl	$.LC6, %edi
	call	puts
	movl	$.LC7, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	puts
	.cfi_endproc
.LFE18:
	.size	fpar_info, .-fpar_info
.globl fpar_function
	.type	fpar_function, @function
fpar_function:
.LFB20:
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
	movl	%esi, %ebx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movl	%edx, 12(%rsp)
	movq	%rcx, %rsi
	testq	%rcx, %rcx
	je	.L55
	movslq	%edi, %r15
	leaq	0(,%r15,8), %r12
	movq	%r12, %rbp
	addq	buffer(%rip), %rbp
	movq	0(%rbp), %rdi
	testq	%rdi, %rdi
	je	.L52
	movq	%rcx, (%rsp)
	call	free
	movq	%r12, %rbp
	addq	buffer(%rip), %rbp
	movq	$0, 0(%rbp)
	movq	(%rsp), %rsi
.L52:
	movq	$-1, %r14
	xorl	%r13d, %r13d
	movq	%r14, %rcx
	movq	%rsi, %rdi
	movl	%r13d, %eax
	repnz scasb
	negq	%rcx
	movq	%rcx, %rdi
	movq	%rsi, (%rsp)
	call	malloc
	movq	%rax, 0(%rbp)
	testq	%rax, %rax
	movq	(%rsp), %rsi
	je	.L55
	movq	%rax, %rdi
	call	strcpy
	movq	0(%rbp), %r11
	movq	%r14, %rcx
	movq	%r11, %rdi
	movl	%r13d, %eax
	repnz scasb
	notq	%rcx
	movb	$10, -1(%r11,%rcx)
	movq	buffer(%rip), %r10
	movq	(%r10,%r12), %r9
	movq	%r14, %rcx
	movq	%r9, %rdi
	movl	%r13d, %eax
	repnz scasb
	notq	%rcx
	movb	$0, (%r9,%rcx)
	salq	$2, %r15
	movq	buffer(%rip), %r8
	movq	(%r8,%r12), %rdi
	movq	%r14, %rcx
	movl	%r13d, %eax
	repnz scasb
	movq	%rcx, %r14
	notq	%r14
	subq	$1, %r14
	movq	maxpos(%rip), %rbp
	movl	%r14d, 0(%rbp,%r15)
	movq	nvar(%rip), %rsi
	movl	%ebx, (%rsi,%r15)
	movl	12(%rsp), %r13d
	addl	$1, %r13d
	movq	ranks(%rip), %rdx
	movl	%r13d, (%rdx,%r15)
	addq	y(%rip), %r12
	movslq	%r13d, %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, %rbp
	movq	%rax, (%r12)
	testl	%r13d, %r13d
	jle	.L56
	movslq	%ebx, %rbx
	salq	$3, %rbx
	movl	12(%rsp), %r14d
	andl	$7, %r14d
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	movl	$1, %r12d
	addq	$8, %rbp
	cmpl	$1, %r13d
	jle	.L56
	testl	%r14d, %r14d
	je	.L53
	cmpl	$1, %r14d
	je	.L81
	cmpl	$2, %r14d
	je	.L82
	cmpl	$3, %r14d
	je	.L83
	cmpl	$4, %r14d
	je	.L84
	cmpl	$5, %r14d
	je	.L85
	cmpl	$6, %r14d
	je	.L86
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	movl	$2, %r12d
	addq	$8, %rbp
.L86:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	addl	$1, %r12d
	addq	$8, %rbp
.L85:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	addl	$1, %r12d
	addq	$8, %rbp
.L84:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	addl	$1, %r12d
	addq	$8, %rbp
.L83:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	addl	$1, %r12d
	addq	$8, %rbp
.L82:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	addl	$1, %r12d
	addq	$8, %rbp
.L81:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	addl	$1, %r12d
	addq	$8, %rbp
	cmpl	%r12d, %r13d
	jle	.L56
.L53:
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 0(%rbp)
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 8(%rbp)
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 16(%rbp)
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 24(%rbp)
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 32(%rbp)
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 40(%rbp)
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 48(%rbp)
	movq	%rbx, %rdi
	call	malloc
	movq	%rax, 56(%rbp)
	addl	$8, %r12d
	addq	$64, %rbp
	cmpl	%r12d, %r13d
	jg	.L53
.L56:
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
.L55:
	.cfi_restore_state
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
.LFE20:
	.size	fpar_function, .-fpar_function
.globl fpar_free
	.type	fpar_free, @function
fpar_free:
.LFB21:
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
	movslq	%edi, %r13
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	leaq	0(,%r13,8), %r12
	movq	buffer(%rip), %rax
	movq	(%rax,%r12), %rdi
	testq	%rdi, %rdi
	je	.L89
	call	free
	movq	buffer(%rip), %rdx
	movq	$0, (%rdx,%r12)
.L89:
	salq	$2, %r13
	movq	ranks(%rip), %rcx
	movl	(%rcx,%r13), %edx
	testl	%edx, %edx
	jle	.L96
	movq	y(%rip), %rsi
	movq	(%rsi,%r12), %rax
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
.L93:
	movq	(%rax,%rbx), %rdi
	testq	%rdi, %rdi
	je	.L92
	call	free
	movq	y(%rip), %r8
	movq	(%r8,%r12), %rax
	movq	$0, (%rax,%rbx)
	movq	ranks(%rip), %rdi
	movl	(%rdi,%r13), %edx
.L92:
	addl	$1, %ebp
	addq	$8, %rbx
	cmpl	%edx, %ebp
	jl	.L93
.L91:
	testq	%rax, %rax
	je	.L88
	movq	%rax, %rdi
	call	free
	movq	y(%rip), %rbp
	movq	$0, 0(%rbp,%r12)
.L88:
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
	ret
.L96:
	.cfi_restore_state
	movq	y(%rip), %rbx
	movq	(%rbx,%r12), %rax
	jmp	.L91
	.cfi_endproc
.LFE21:
	.size	fpar_free, .-fpar_free
	.section	.rodata.str1.1
.LC8:
	.string	"Internal parser calls: %lld\n"
	.text
.globl fpar_deinit
	.type	fpar_deinit, @function
fpar_deinit:
.LFB22:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movl	N(%rip), %eax
	testl	%eax, %eax
	jle	.L98
	.cfi_offset 3, -16
	xorl	%ebx, %ebx
.L99:
	movl	%ebx, %edi
	call	fpar_free
	leal	1(%rbx), %edi
	cmpl	%edi, N(%rip)
	jle	.L98
	call	fpar_free
	leal	2(%rbx), %edi
	cmpl	%edi, N(%rip)
	jle	.L98
	call	fpar_free
	leal	3(%rbx), %edi
	cmpl	%edi, N(%rip)
	jle	.L98
	call	fpar_free
	leal	4(%rbx), %edi
	cmpl	%edi, N(%rip)
	jle	.L98
	call	fpar_free
	leal	5(%rbx), %edi
	cmpl	%edi, N(%rip)
	jle	.L98
	call	fpar_free
	leal	6(%rbx), %edi
	cmpl	%edi, N(%rip)
	jle	.L98
	call	fpar_free
	leal	7(%rbx), %edi
	cmpl	%edi, N(%rip)
	jle	.L98
	call	fpar_free
	addl	$8, %ebx
	cmpl	%ebx, N(%rip)
	jg	.L99
.L98:
	movq	buffer(%rip), %rdi
	call	free
	movq	$0, buffer(%rip)
	movq	position(%rip), %rdi
	call	free
	movq	$0, position(%rip)
	movq	ch(%rip), %rdi
	call	free
	movq	$0, ch(%rip)
	movq	maxpos(%rip), %rdi
	call	free
	movq	$0, maxpos(%rip)
	movq	t(%rip), %rdi
	call	free
	movq	$0, t(%rip)
	movq	y(%rip), %rdi
	call	free
	movq	$0, y(%rip)
	movq	err(%rip), %rdi
	call	free
	movq	$0, err(%rip)
	movq	nvar(%rip), %rdi
	call	free
	movq	$0, nvar(%rip)
	movq	ranks(%rip), %rdi
	call	free
	movq	$0, ranks(%rip)
	movq	gcnt(%rip), %rsi
	movl	$.LC8, %edi
	xorl	%eax, %eax
	call	printf
	movq	$0, gcnt(%rip)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
	.size	fpar_deinit, .-fpar_deinit
.globl read_next_char
	.type	read_next_char, @function
read_next_char:
.LFB23:
	.cfi_startproc
	movslq	%edi, %rdi
	leaq	0(,%rdi,4), %rax
	movq	%rax, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %edx
	movq	maxpos(%rip), %rsi
	cmpl	(%rsi,%rax), %edx
	jge	.L115
	movq	%rdi, %rax
	addq	ch(%rip), %rax
	cmpb	$10, (%rax)
	je	.L115
	movslq	%edx, %r9
	movq	buffer(%rip), %r10
	movq	(%r10,%rdi,8), %r8
	movzbl	(%r8,%r9), %edi
	movb	%dil, (%rax)
	addl	$1, %edx
	movl	%edx, (%rcx)
.L115:
	ret
	.cfi_endproc
.LFE23:
	.size	read_next_char, .-read_next_char
.globl skipblanks
	.type	skipblanks, @function
skipblanks:
.LFB24:
	.cfi_startproc
	movq	ch(%rip), %rax
	movslq	%edi, %rdi
	movzbl	(%rax,%rdi), %edx
	cmpb	$10, %dl
	je	.L117
	leaq	0(,%rdi,4), %rsi
	leaq	0(,%rdi,8), %r10
	jmp	.L119
.L123:
	movslq	%eax, %r11
	movq	buffer(%rip), %rdx
	movq	(%rdx,%r10), %r8
	movzbl	(%r8,%r11), %edx
	movb	%dl, (%r9)
	addl	$1, %eax
	movl	%eax, (%rcx)
	movq	ch(%rip), %rax
	movzbl	(%rax,%rdi), %edx
.L119:
	leaq	(%rax,%rdi), %r9
	movsbq	%dl, %rax
	movq	%rsi, %rcx
	addq	position(%rip), %rcx
	movq	%rsi, %r8
	addq	maxpos(%rip), %r8
	movq	_ctype_(%rip), %r11
	testb	$8, 1(%r11,%rax)
	je	.L117
	cmpb	$10, %dl
	je	.L117
	movl	(%rcx), %eax
.L121:
	cmpl	(%r8), %eax
	jl	.L123
	.p2align 4,,3
	jl	.L123
	.p2align 4,,6
	jl	.L123
	.p2align 4,,11
	jl	.L123
	.p2align 4,,11
	jl	.L123
	.p2align 4,,11
	jl	.L123
	.p2align 4,,11
	jl	.L123
	.p2align 4,,11
	jl	.L123
	.p2align 4,,11
	jmp	.L121
.L117:
	.p2align 4,,11
	ret
	.cfi_endproc
.LFE24:
	.size	skipblanks, .-skipblanks
	.section	.rodata.str1.8
	.align 8
.LC9:
	.string	"Expected: function name or variable.\n"
	.text
.globl read_id
	.type	read_id, @function
read_id:
.LFB25:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	movl	%edi, %ebp
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movq	%rsi, %r12
	call	skipblanks
	movq	ch(%rip), %rdx
	movslq	%ebp, %rbx
	movzbl	(%rdx,%rbx), %eax
	movsbq	%al, %rcx
	movq	_ctype_(%rip), %rsi
	movzbl	1(%rsi,%rcx), %esi
	testb	$3, %sil
	je	.L126
	leaq	0(,%rbx,4), %rdi
	leaq	0(,%rbx,8), %r9
	xorl	%ecx, %ecx
	jmp	.L127
.L134:
	leaq	(%rdx,%rbx), %r8
.L129:
	movq	%rdi, %rsi
	addq	position(%rip), %rsi
	movl	(%rsi), %edx
	movq	maxpos(%rip), %rbp
	cmpl	0(%rbp,%rdi), %edx
	jge	.L130
	cmpb	$10, %al
	je	.L130
	movslq	%edx, %r10
	movq	buffer(%rip), %rbp
	movq	0(%rbp,%r9), %r11
	movzbl	(%r11,%r10), %eax
	movb	%al, (%r8)
	addl	$1, %edx
	movl	%edx, (%rsi)
.L130:
	movq	ch(%rip), %rdx
	movzbl	(%rdx,%rbx), %eax
	movsbq	%al, %r8
	movq	_ctype_(%rip), %rsi
	movzbl	1(%rsi,%r8), %esi
.L127:
	andb	$7, %sil
	jne	.L131
	cmpb	$39, %al
	je	.L131
	cmpb	$95, %al
	jne	.L133
.L131:
	cmpl	$30, %ecx
	.p2align 4,,4
	jg	.L134
	movslq	%ecx, %rdx
	movb	%al, (%r12,%rdx)
	addl	$1, %ecx
	movq	%rbx, %r8
	addq	ch(%rip), %r8
	movzbl	(%r8), %eax
	jmp	.L129
.L133:
	movslq	%ecx, %rbx
	movb	$0, (%r12,%rbx)
	addq	$1, gcnt(%rip)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L126:
	.cfi_restore_state
	movl	$.LC9, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %r9
	movq	(%r9,%rbx,8), %rdx
	movl	%ebp, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	$174, %edx
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rdi
	movl	$1, (%rdi,%rbx,4)
	addq	$1, gcnt(%rip)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE25:
	.size	read_id, .-read_id
.globl exponential
	.type	exponential, @function
exponential:
.LFB29:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	subq	$80, %rsp
	.cfi_def_cfa_offset 112
	movl	%edi, %ebx
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	addq	$1, gcnt(%rip)
	call	factor
	movsd	%xmm0, 24(%rsp)
	movq	ch(%rip), %r12
	movslq	%ebx, %rbp
.L136:
	cmpb	$94, (%r12,%rbp)
	jne	.L167
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movsd	%xmm0, 32(%rsp)
	movq	ch(%rip), %r12
.L137:
	cmpb	$94, (%r12,%rbp)
	jne	.L168
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movsd	%xmm0, 40(%rsp)
	movq	ch(%rip), %r12
.L138:
	cmpb	$94, (%r12,%rbp)
	jne	.L169
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movsd	%xmm0, 56(%rsp)
	movq	ch(%rip), %r12
.L139:
	cmpb	$94, (%r12,%rbp)
	jne	.L170
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movsd	%xmm0, 72(%rsp)
	movq	ch(%rip), %r12
.L140:
	cmpb	$94, (%r12,%rbp)
	jne	.L171
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movsd	%xmm0, 64(%rsp)
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L141
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movsd	%xmm0, 48(%rsp)
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L142
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movsd	%xmm0, 16(%rsp)
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L143
	addq	$1, gcnt(%rip)
	movl	%ebx, %edi
	call	factor
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
.L151:
	movl	%ebx, %edi
	movsd	%xmm2, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm2
	movapd	%xmm2, %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm3
	movapd	%xmm3, %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm4
	movapd	%xmm4, %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm5
	movapd	%xmm5, %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm6
	movapd	%xmm6, %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm7
	movapd	%xmm7, %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	jne	.L144
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movapd	%xmm0, %xmm1
	movsd	(%rsp), %xmm8
	movapd	%xmm8, %xmm0
	call	pow
	movapd	%xmm0, %xmm2
	movq	ch(%rip), %r12
	cmpb	$94, (%r12,%rbp)
	je	.L151
.L144:
	movapd	%xmm2, %xmm1
	movsd	16(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 16(%rsp)
.L143:
	movsd	16(%rsp), %xmm1
	movsd	48(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 48(%rsp)
.L142:
	movsd	48(%rsp), %xmm1
	movsd	64(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 64(%rsp)
.L141:
	movsd	64(%rsp), %xmm1
	movsd	72(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 72(%rsp)
	jmp	.L140
.L171:
	movsd	72(%rsp), %xmm1
	movsd	56(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 56(%rsp)
	jmp	.L139
.L170:
	movsd	56(%rsp), %xmm1
	movsd	40(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 40(%rsp)
	jmp	.L138
.L169:
	movsd	40(%rsp), %xmm1
	movsd	32(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 32(%rsp)
	jmp	.L137
.L168:
	movsd	32(%rsp), %xmm1
	movsd	24(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 24(%rsp)
	jmp	.L136
.L167:
	movsd	24(%rsp), %xmm0
	addq	$80, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE29:
	.size	exponential, .-exponential
.globl term
	.type	term, @function
term:
.LFB27:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	movl	%edi, %ebx
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	addq	$1, gcnt(%rip)
	call	exponential
	movapd	%xmm0, %xmm1
	movslq	%ebx, %rbp
.L179:
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbp), %eax
	cmpb	$42, %al
	je	.L175
	cmpb	$47, %al
	je	.L180
	movapd	%xmm1, %xmm0
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L175:
	.cfi_restore_state
	movl	%ebx, %edi
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	mulsd	%xmm0, %xmm1
	jmp	.L179
.L180:
	movl	%ebx, %edi
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	divsd	%xmm0, %xmm1
	jmp	.L179
	.cfi_endproc
.LFE27:
	.size	term, .-term
.globl expression
	.type	expression, @function
expression:
.LFB28:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	movl	%edi, %ebp
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	addq	$2, gcnt(%rip)
	call	exponential
	movsd	%xmm0, 24(%rsp)
	movslq	%ebp, %rbx
.L203:
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbx), %eax
	cmpb	$42, %al
	je	.L184
	cmpb	$47, %al
	je	.L209
	cmpb	$43, %al
	je	.L189
.L212:
	cmpb	$45, %al
	.p2align 4,,5
	je	.L210
	movsd	24(%rsp), %xmm0
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L184:
	.cfi_restore_state
	movl	%ebp, %edi
	call	exponential
	mulsd	24(%rsp), %xmm0
	movsd	%xmm0, 24(%rsp)
	jmp	.L203
.L209:
	movl	%ebp, %edi
	call	exponential
	movsd	24(%rsp), %xmm1
	divsd	%xmm0, %xmm1
	movsd	%xmm1, 24(%rsp)
	jmp	.L203
.L210:
	addq	$1, gcnt(%rip)
	movl	%ebp, %edi
	call	exponential
	movapd	%xmm0, %xmm1
.L208:
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbx), %edx
	cmpb	$42, %dl
	je	.L199
.L214:
	cmpb	$47, %dl
	je	.L211
	movsd	24(%rsp), %xmm0
	subsd	%xmm1, %xmm0
	movsd	%xmm0, 24(%rsp)
.L196:
	movzbl	(%rax,%rbx), %eax
	cmpb	$43, %al
	jne	.L212
.L189:
	addq	$1, gcnt(%rip)
	movl	%ebp, %edi
	call	exponential
	movapd	%xmm0, %xmm1
.L206:
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbx), %edx
	cmpb	$42, %dl
	je	.L193
.L215:
	cmpb	$47, %dl
	je	.L213
	addsd	24(%rsp), %xmm1
	movsd	%xmm1, 24(%rsp)
	jmp	.L196
.L211:
	movl	%ebp, %edi
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	divsd	%xmm0, %xmm1
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbx), %edx
	cmpb	$42, %dl
	jne	.L214
.L199:
	movl	%ebp, %edi
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	mulsd	%xmm0, %xmm1
	jmp	.L208
.L213:
	movl	%ebp, %edi
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	divsd	%xmm0, %xmm1
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbx), %edx
	cmpb	$42, %dl
	jne	.L215
.L193:
	movl	%ebp, %edi
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	mulsd	%xmm0, %xmm1
	jmp	.L206
	.cfi_endproc
.LFE28:
	.size	expression, .-expression
	.section	.rodata.str1.1
.LC12:
	.string	"%lf%c"
.LC13:
	.string	"Expected: ')'.\n"
.LC14:
	.string	"x"
.LC15:
	.string	"t"
.LC16:
	.string	"a"
.LC17:
	.string	"y%d_%d"
	.section	.rodata.str1.8
	.align 8
.LC18:
	.string	"found y in definition, but i > numer_of_args\n"
	.section	.rodata.str1.1
.LC19:
	.string	"found y, but i <= 0\n"
	.section	.rodata.str1.8
	.align 8
.LC20:
	.string	"found y in definition, but yranks > ranks of this\n"
	.section	.rodata.str1.1
.LC21:
	.string	"found y, but ranks < 0\n"
.LC22:
	.string	"y%d"
.LC23:
	.string	"pi"
.LC25:
	.string	"sin"
.LC26:
	.string	"Expected: '(' after sin.\n"
.LC27:
	.string	"cos"
.LC28:
	.string	"Expected: '(' after cos.\n"
.LC29:
	.string	"sinh"
.LC30:
	.string	"Expected: '(' after sinh.\n"
.LC31:
	.string	"cosh"
.LC32:
	.string	"Expected: '(' after cosh.\n"
.LC33:
	.string	"acos"
.LC34:
	.string	"Expected: '(' after acos.\n"
.LC35:
	.string	"asin"
.LC36:
	.string	"Expected: '(' after asin.\n"
.LC37:
	.string	"asinh"
.LC38:
	.string	"Expected: '(' after asinh.\n"
.LC39:
	.string	"acosh"
.LC40:
	.string	"Expected: '(' after acosh.\n"
.LC41:
	.string	"tg"
.LC42:
	.string	"tan"
	.section	.rodata.str1.8
	.align 8
.LC43:
	.string	"Expected: '(' after tg or tan.\n"
	.section	.rodata.str1.1
.LC44:
	.string	"atg"
.LC45:
	.string	"atan"
	.section	.rodata.str1.8
	.align 8
.LC46:
	.string	"Expected: '(' after atg or atan.\n"
	.section	.rodata.str1.1
.LC47:
	.string	"tgh"
.LC48:
	.string	"tanh"
	.section	.rodata.str1.8
	.align 8
.LC49:
	.string	"Expected: '(' after tgh or tanh.\n"
	.section	.rodata.str1.1
.LC50:
	.string	"atgh"
.LC51:
	.string	"atanh"
	.section	.rodata.str1.8
	.align 8
.LC52:
	.string	"Expected: '(' after atgh or atanh.\n"
	.section	.rodata.str1.1
.LC53:
	.string	"ctg"
.LC54:
	.string	"ctan"
	.section	.rodata.str1.8
	.align 8
.LC55:
	.string	"Expected: '(' after ctg or ctan.\n"
	.section	.rodata.str1.1
.LC56:
	.string	"exp"
.LC57:
	.string	"e"
	.section	.rodata.str1.8
	.align 8
.LC58:
	.string	"Expected: '(' after exp or e.\n"
	.section	.rodata.str1.1
.LC59:
	.string	"ln"
.LC60:
	.string	"Expected: '(' after ln.\n"
.LC61:
	.string	"log"
.LC62:
	.string	"Expected: '(' after log.\n"
.LC63:
	.string	"sqrt"
.LC64:
	.string	"Expected: '(' after sqrt.\n"
.LC65:
	.string	"cbrt"
.LC66:
	.string	"Expected: '(' after cbrt.\n"
.LC67:
	.string	"ceil"
.LC68:
	.string	"Expected: '(' after ceil.\n"
.LC69:
	.string	"sgn"
.LC72:
	.string	"Expected: '(' after sgn.\n"
.LC73:
	.string	"abs"
.LC75:
	.string	"Expected: '(' after abs.\n"
.LC76:
	.string	"udf1"
.LC77:
	.string	"Expected: '(' after udf1.\n"
.LC78:
	.string	"gb"
.LC80:
	.string	"Expected: '(' after gb.\n"
.LC81:
	.string	"gg"
.LC82:
	.string	"Expected: '(' after gg.\n"
.LC83:
	.string	"gr"
.LC84:
	.string	"Expected: '(' after gr.\n"
.LC85:
	.string	"sb"
.LC86:
	.string	"Expected: '(' after sb.\n"
.LC87:
	.string	"sg"
.LC88:
	.string	"Expected: '(' after sg.\n"
.LC89:
	.string	"sr"
.LC90:
	.string	"Expected: '(' after sr.\n"
.LC91:
	.string	"sgs"
.LC92:
	.string	"Expected: '(' after sgs.\n"
.LC93:
	.string	"ggs"
.LC94:
	.string	"Expected: '(' after ggs.\n"
.LC95:
	.string	"neg"
.LC96:
	.string	"Expected: '(' after neg.\n"
.LC97:
	.string	"pow"
	.section	.rodata.str1.8
	.align 8
.LC98:
	.string	"Expected: ',' in pow(..., ...).\n"
	.section	.rodata.str1.1
.LC99:
	.string	"Expected: '(' after pow.\n"
.LC100:
	.string	"udf2"
	.section	.rodata.str1.8
	.align 8
.LC101:
	.string	"Expected: ',' in udf2(..., ...).\n"
	.section	.rodata.str1.1
.LC102:
	.string	"Expected: '(' after max.\n"
.LC103:
	.string	"max"
	.section	.rodata.str1.8
	.align 8
.LC104:
	.string	"Expected: ',' in max(..., ...).\n"
	.section	.rodata.str1.1
.LC105:
	.string	"min"
	.section	.rodata.str1.8
	.align 8
.LC106:
	.string	"Expected: ',' in min(..., ...).\n"
	.section	.rodata.str1.1
.LC107:
	.string	"Expected: '(' after min.\n"
.LC108:
	.string	"mina"
	.section	.rodata.str1.8
	.align 8
.LC109:
	.string	"Expected: ',' in mina(..., ...).\n"
	.section	.rodata.str1.1
.LC110:
	.string	"Expected: '(' after mina.\n"
.LC111:
	.string	"minb"
	.section	.rodata.str1.8
	.align 8
.LC112:
	.string	"Expected: ',' in minb(..., ...).\n"
	.section	.rodata.str1.1
.LC113:
	.string	"Expected: '(' after minb.\n"
.LC114:
	.string	"maxa"
	.section	.rodata.str1.8
	.align 8
.LC115:
	.string	"Expected: ',' in maxa(..., ...).\n"
	.section	.rodata.str1.1
.LC116:
	.string	"Expected: '(' after maxa.\n"
.LC117:
	.string	"maxb"
	.section	.rodata.str1.8
	.align 8
.LC118:
	.string	"Expected: ',' in maxb(..., ...).\n"
	.section	.rodata.str1.1
.LC119:
	.string	"Expected: '(' after maxb.\n"
.LC120:
	.string	"mod"
	.section	.rodata.str1.8
	.align 8
.LC121:
	.string	"Expected: ',' in mod(..., ...).\n"
	.section	.rodata.str1.1
.LC122:
	.string	"Expected: '(' after and.\n"
.LC123:
	.string	"and"
	.section	.rodata.str1.8
	.align 8
.LC124:
	.string	"Expected: ',' in and(..., ...).\n"
	.section	.rodata.str1.1
.LC125:
	.string	"or"
	.section	.rodata.str1.8
	.align 8
.LC126:
	.string	"Expected: ',' in or(..., ...).\n"
	.section	.rodata.str1.1
.LC127:
	.string	"Expected: '(' after or.\n"
.LC128:
	.string	"xor"
	.section	.rodata.str1.8
	.align 8
.LC129:
	.string	"Expected: ',' in xor(..., ...).\n"
	.section	.rodata.str1.1
.LC130:
	.string	"Expected: '(' after xor.\n"
.LC131:
	.string	"if"
	.section	.rodata.str1.8
	.align 8
.LC132:
	.string	"Expected: 1st ',' in if(..., ..., ...).\n"
	.align 8
.LC133:
	.string	"Expected: 2nd ',' in if(..., ..., ...).\n"
	.section	.rodata.str1.1
.LC134:
	.string	"udf3"
	.section	.rodata.str1.8
	.align 8
.LC135:
	.string	"Expected: 1st ',' in udf3(..., ..., ...).\n"
	.align 8
.LC136:
	.string	"Expected: 2nd ',' in udf3(..., ..., ...).\n"
	.section	.rodata.str1.1
.LC137:
	.string	"Unknown identifier.\n"
	.text
.globl factor
	.type	factor, @function
factor:
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
	subq	$120, %rsp
	.cfi_def_cfa_offset 176
	movl	%edi, %r13d
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	addq	$1, gcnt(%rip)
	movslq	%edi, %rbp
	leaq	0(,%rbp,4), %rbx
	movq	%rbx, %rdx
	addq	position(%rip), %rdx
	movl	(%rdx), %eax
	movq	maxpos(%rip), %rcx
	cmpl	(%rcx,%rbx), %eax
	jge	.L217
	movq	%rbp, %rcx
	addq	ch(%rip), %rcx
	cmpb	$10, (%rcx)
	je	.L217
	movslq	%eax, %r8
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %rdi
	movzbl	(%rdi,%r8), %esi
	movb	%sil, (%rcx)
	addl	$1, %eax
	movl	%eax, (%rdx)
.L217:
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	movzbl	(%rdx,%rbp), %eax
	leaq	0(,%rbp,8), %r12
	movabsq	$4607182418800017408, %r14
	movq	%r14, 8(%rsp)
	leaq	(%rdx,%rbp), %rdi
	movsd	.LC11(%rip), %xmm0
.L444:
	cmpb	$43, %al
	je	.L220
	cmpb	$45, %al
	jne	.L447
.L221:
	movsd	8(%rsp), %xmm1
	xorpd	%xmm0, %xmm1
	movsd	%xmm1, 8(%rsp)
.L220:
	movq	%rbx, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %edx
	movq	maxpos(%rip), %r10
	cmpl	(%r10,%rbx), %edx
	jge	.L444
	cmpb	$10, %al
	je	.L444
	movslq	%edx, %r15
	movq	buffer(%rip), %r8
	movq	(%r8,%r12), %rsi
	movzbl	(%rsi,%r15), %eax
	movb	%al, (%rdi)
	addl	$1, %edx
	movl	%edx, (%rcx)
	movq	ch(%rip), %r11
	movzbl	(%r11,%rbp), %eax
	leaq	(%r11,%rbp), %rdi
	cmpb	$43, %al
	je	.L220
	cmpb	$45, %al
	je	.L221
.L447:
	movsbq	%al, %r9
	movq	_ctype_(%rip), %rcx
	testb	$4, 1(%rcx,%r9)
	jne	.L222
	cmpb	$46, %al
	je	.L222
	cmpb	$40, %al
	je	.L227
	cmpb	$44, %al
	.p2align 4,,5
	je	.L227
	leaq	32(%rsp), %r15
	movq	%r15, %rsi
	movl	%r13d, %edi
	call	read_id
	xorl	%eax, %eax
	movq	$-1, %rcx
	movq	%r15, %rdi
	repnz scasb
	notq	%rcx
	movzbl	30(%rsp,%rcx), %r8d
	movl	$2, %eax
	movl	$.LC14, %edi
	movq	%r15, %rsi
	movq	%rax, %rcx
	repz cmpsb
	jne	.L448
.L232:
	movq	t(%rip), %r15
	movq	(%r15,%r12), %rdx
	movq	%rdx, 64(%rsp)
.L226:
	movl	%r13d, %edi
	call	skipblanks
.L446:
	movsd	8(%rsp), %xmm0
	mulsd	64(%rsp), %xmm0
	addq	$120, %rsp
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
.L227:
	.cfi_restore_state
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 64(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	addq	ch(%rip), %rbp
	movzbl	0(%rbp), %eax
	cmpb	$41, %al
	je	.L449
	cmpb	$44, %al
	je	.L446
	movl	$.LC13, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %rax
	movq	(%rax,%r12), %rdx
	movl	%r13d, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	$213, %edx
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rbp
	movl	$1, 0(%rbp,%rbx)
	jmp	.L226
.L222:
	movq	buffer(%rip), %r15
	movq	(%r15,%r12), %r10
	movq	%rbx, %r11
	addq	position(%rip), %r11
	movl	(%r11), %eax
	subl	$1, %eax
	movl	%eax, (%r11)
	cltq
	movzbl	(%rdi), %edx
	movb	%dl, (%r10,%rax)
	movq	%rbp, %rcx
	addq	ch(%rip), %rcx
	leaq	64(%rsp), %rdx
	movq	position(%rip), %rdi
	movslq	(%rdi,%rbx), %rdi
	movq	buffer(%rip), %r14
	addq	(%r14,%r12), %rdi
	movl	$.LC12, %esi
	xorl	%eax, %eax
	call	sscanf
	movq	ch(%rip), %rdi
	movzbl	(%rdi,%rbp), %r8d
	movl	%r8d, %ecx
.L225:
	movq	%rbx, %rdx
	addq	position(%rip), %rdx
	movl	(%rdx), %eax
	movq	maxpos(%rip), %rsi
	cmpl	(%rsi,%rbx), %eax
	jge	.L224
	leaq	(%rdi,%rbp), %rsi
	cmpb	$10, %cl
	je	.L224
	movslq	%eax, %rcx
	movq	buffer(%rip), %rdi
	movq	(%rdi,%r12), %r14
	movzbl	(%r14,%rcx), %r9d
	movb	%r9b, (%rsi)
	addl	$1, %eax
	movl	%eax, (%rdx)
	movq	ch(%rip), %rdi
	movzbl	(%rdi,%rbp), %ecx
.L224:
	cmpb	%r8b, %cl
	jne	.L225
	jmp	.L226
.L448:
	movl	$.LC15, %edi
	movq	%r15, %rsi
	movq	%rax, %rcx
	repz cmpsb
	je	.L232
	movl	$.LC16, %edi
	movq	%r15, %rsi
	movq	%rax, %rcx
	repz cmpsb
	je	.L232
	leaq	80(%rsp), %rcx
	leaq	96(%rsp), %r12
	movq	%r12, %rdx
	movl	$.LC17, %esi
	movq	%r15, %rdi
	xorl	%eax, %eax
	movb	%r8b, (%rsp)
	call	sscanf
	cmpl	$2, %eax
	movzbl	(%rsp), %r8d
	jne	.L234
	cmpb	$39, %r8b
	je	.L450
.L234:
	movq	%r12, %rdx
	movl	$.LC22, %esi
	movq	%r15, %rdi
	xorl	%eax, %eax
	movb	%r8b, (%rsp)
	call	sscanf
	subl	$1, %eax
	movzbl	(%rsp), %r8d
	je	.L451
.L239:
	movl	$.LC23, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L452
	movl	$.LC25, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L453
	movl	$.LC27, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L454
	movl	$.LC29, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L455
	movl	$.LC31, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L456
	movl	$.LC33, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L457
	movl	$.LC35, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L458
	movl	$.LC37, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L459
	movl	$.LC39, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L460
	movl	$.LC41, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L260
	movl	$.LC42, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L261
.L260:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L461
	movl	$.LC43, %edx
	movl	%r13d, %esi
	movl	$329, %edi
	call	exception.clone.0
	jmp	.L226
.L449:
	movq	%rbx, %rdx
	addq	position(%rip), %rdx
	movl	(%rdx), %eax
	movq	maxpos(%rip), %r8
	cmpl	(%r8,%rbx), %eax
	jge	.L226
	movslq	%eax, %r10
	movq	buffer(%rip), %r11
	movq	(%r11,%r12), %r12
	movzbl	(%r12,%r10), %ebx
	movb	%bl, 0(%rbp)
	addl	$1, %eax
	movl	%eax, (%rdx)
	jmp	.L226
.L451:
	cmpb	$39, %r8b
	je	.L239
	movl	$0, 80(%rsp)
	movl	96(%rsp), %eax
	movq	nvar(%rip), %rcx
	cmpl	%eax, (%rcx,%rbp,4)
	jl	.L462
	testl	%eax, %eax
	jle	.L463
	movq	ranks(%rip), %rsi
	cmpl	$0, (%rsi,%rbp,4)
	jle	.L464
	movq	y(%rip), %r15
	movq	(%r15,%rbp,8), %rdx
	cltq
	movq	(%rdx), %rdi
	movq	-8(%rdi,%rax,8), %rbp
	movq	%rbp, 64(%rsp)
	jmp	.L226
.L453:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L465
	movl	$.LC26, %edx
	movl	%r13d, %esi
	movl	$249, %edi
	call	exception.clone.0
	jmp	.L226
.L452:
	movl	%r13d, %edi
	call	skipblanks
	movabsq	$4614256656552045848, %rax
	movq	%rax, 64(%rsp)
	jmp	.L226
.L455:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L466
	movl	$.LC30, %edx
	movl	%r13d, %esi
	movl	$269, %edi
	call	exception.clone.0
	jmp	.L226
.L454:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L467
	movl	$.LC28, %edx
	movl	%r13d, %esi
	movl	$259, %edi
	call	exception.clone.0
	jmp	.L226
.L456:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L468
	movl	$.LC32, %edx
	movl	%r13d, %esi
	movl	$279, %edi
	call	exception.clone.0
	jmp	.L226
.L450:
	movl	96(%rsp), %eax
	movq	nvar(%rip), %r9
	cmpl	%eax, (%r9,%rbp,4)
	jl	.L469
	testl	%eax, %eax
	jle	.L470
	movl	80(%rsp), %edx
	movq	ranks(%rip), %r14
	cmpl	%edx, (%r14,%rbp,4)
	jle	.L471
	testl	%edx, %edx
	js	.L472
	movslq	%edx, %r10
	movq	y(%rip), %r11
	movq	(%r11,%rbp,8), %r12
	cltq
	movq	(%r12,%r10,8), %rbx
	movq	-8(%rbx,%rax,8), %r8
	movq	%r8, 64(%rsp)
	jmp	.L226
.L462:
	movl	$.LC18, %edx
	movl	%r13d, %esi
	movl	$231, %edi
	call	exception.clone.0
	jmp	.L226
.L459:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L473
	movl	$.LC38, %edx
	movl	%r13d, %esi
	movl	$309, %edi
	call	exception.clone.0
	jmp	.L226
.L458:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L474
	movl	$.LC36, %edx
	movl	%r13d, %esi
	movl	$299, %edi
	call	exception.clone.0
	jmp	.L226
.L473:
	movq	buffer(%rip), %rsi
	movq	(%rsi,%rbp,8), %r12
	addq	position(%rip), %rbx
	movl	(%rbx), %r11d
	subl	$1, %r11d
	movl	%r11d, (%rbx)
	movslq	%r11d, %r10
	movzbl	(%rax), %eax
	movb	%al, (%r12,%r10)
	movl	%r13d, %edi
	call	exponential
	call	asinh
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L474:
	movq	buffer(%rip), %r8
	movq	(%r8,%rbp,8), %r9
	addq	position(%rip), %rbx
	movl	(%rbx), %r14d
	subl	$1, %r14d
	movl	%r14d, (%rbx)
	movslq	%r14d, %r15
	movzbl	(%rax), %ebp
	movb	%bpl, (%r9,%r15)
	movl	%r13d, %edi
	call	exponential
	call	asin
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L457:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L475
	movl	$.LC34, %edx
	movl	%r13d, %esi
	movl	$289, %edi
	call	exception.clone.0
	jmp	.L226
.L467:
	movq	buffer(%rip), %rdi
	movq	(%rdi,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %esi
	subl	$1, %esi
	movl	%esi, (%rbx)
	movslq	%esi, %r11
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%r11)
	movl	%r13d, %edi
	call	exponential
	call	cos
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L475:
	movq	buffer(%rip), %rdx
	movq	(%rdx,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %edi
	subl	$1, %edi
	movl	%edi, (%rbx)
	movslq	%edi, %rsi
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%rsi)
	movl	%r13d, %edi
	call	exponential
	call	acos
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L466:
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %r15
	addq	position(%rip), %rbx
	movl	(%rbx), %r9d
	subl	$1, %r9d
	movl	%r9d, (%rbx)
	movslq	%r9d, %rdx
	movzbl	(%rax), %ebp
	movb	%bpl, (%r15,%rdx)
	movl	%r13d, %edi
	call	exponential
	call	sinh
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L463:
	movl	$.LC19, %edx
	movl	%r13d, %esi
	movl	$232, %edi
	call	exception.clone.0
	jmp	.L226
.L464:
	movl	$.LC20, %edx
	movl	%r13d, %esi
	movl	$233, %edi
	call	exception.clone.0
	jmp	.L226
.L465:
	movq	buffer(%rip), %r12
	movq	(%r12,%rbp,8), %r8
	addq	position(%rip), %rbx
	movl	(%rbx), %r10d
	subl	$1, %r10d
	movl	%r10d, (%rbx)
	movslq	%r10d, %r14
	movzbl	(%rax), %r9d
	movb	%r9b, (%r8,%r14)
	movl	%r13d, %edi
	call	exponential
	call	sin
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L460:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L476
	movl	$.LC40, %edx
	movl	%r13d, %esi
	movl	$319, %edi
	call	exception.clone.0
	jmp	.L226
.L261:
	movl	$.LC44, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L263
	movl	$.LC45, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L264
.L263:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L477
	movl	$.LC46, %edx
	movl	%r13d, %esi
	movl	$339, %edi
	call	exception.clone.0
	jmp	.L226
.L472:
	movl	$.LC21, %edx
	movl	%r13d, %esi
	movl	$225, %edi
	call	exception.clone.0
	jmp	.L226
.L461:
	movq	buffer(%rip), %r10
	movq	(%r10,%rbp,8), %r14
	addq	position(%rip), %rbx
	movl	(%rbx), %r8d
	subl	$1, %r8d
	movl	%r8d, (%rbx)
	movslq	%r8d, %r9
	movzbl	(%rax), %ebp
	movb	%bpl, (%r14,%r9)
	movl	%r13d, %edi
	call	exponential
	call	tan
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L468:
	movq	buffer(%rip), %r11
	movq	(%r11,%rbp,8), %r10
	addq	position(%rip), %rbx
	movl	(%rbx), %r12d
	subl	$1, %r12d
	movl	%r12d, (%rbx)
	movslq	%r12d, %r8
	movzbl	(%rax), %eax
	movb	%al, (%r10,%r8)
	movl	%r13d, %edi
	call	exponential
	call	cosh
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L476:
	movq	buffer(%rip), %r15
	movq	(%r15,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %edx
	subl	$1, %edx
	movl	%edx, (%rbx)
	movslq	%edx, %rdi
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%rdi)
	movl	%r13d, %edi
	call	exponential
	call	acosh
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L264:
	movl	$.LC47, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L266
	movl	$.LC48, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L267
.L266:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L478
	movl	$.LC49, %edx
	movl	%r13d, %esi
	movl	$349, %edi
	call	exception.clone.0
	jmp	.L226
.L267:
	movl	$.LC50, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L269
	movl	$.LC51, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L270
.L269:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L479
	movl	$.LC52, %edx
	movl	%r13d, %esi
	movl	$359, %edi
	call	exception.clone.0
	jmp	.L226
.L478:
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %r15d
	subl	$1, %r15d
	movl	%r15d, (%rbx)
	movslq	%r15d, %rdx
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%rdx)
	movl	%r13d, %edi
	call	exponential
	call	tanh
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L270:
	movl	$.LC53, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L272
	movl	$.LC54, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L273
.L272:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L480
	movl	$.LC55, %edx
	movl	%r13d, %esi
	movl	$369, %edi
	call	exception.clone.0
	jmp	.L226
.L479:
	movq	buffer(%rip), %r12
	movq	(%r12,%rbp,8), %r8
	addq	position(%rip), %rbx
	movl	(%rbx), %r10d
	subl	$1, %r10d
	movl	%r10d, (%rbx)
	movslq	%r10d, %r14
	movzbl	(%rax), %ebp
	movb	%bpl, (%r8,%r14)
	movl	%r13d, %edi
	call	exponential
	call	atanh
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L273:
	movl	$.LC56, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L275
	movl	$.LC57, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L276
.L275:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L481
	movl	$.LC58, %edx
	movl	%r13d, %esi
	movl	$379, %edi
	call	exception.clone.0
	jmp	.L226
.L480:
	movq	buffer(%rip), %rdx
	movq	(%rdx,%rbp,8), %rsi
	addq	position(%rip), %rbx
	movl	(%rbx), %edi
	subl	$1, %edi
	movl	%edi, (%rbx)
	movslq	%edi, %r11
	movzbl	(%rax), %eax
	movb	%al, (%rsi,%r11)
	movl	%r13d, %edi
	call	exponential
	call	tan
	movd	%r14, %xmm2
	divsd	%xmm0, %xmm2
	movsd	%xmm2, 64(%rsp)
	jmp	.L226
.L276:
	movl	$.LC59, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L482
	movl	$.LC61, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L483
	movl	$.LC63, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L484
	movl	$.LC65, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L485
	movl	$.LC67, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L486
	movl	$.LC69, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L487
	movl	$.LC73, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L488
	movl	$.LC76, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L489
	movl	$.LC78, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L490
	movl	$.LC81, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L491
	movl	$.LC83, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L492
	movl	$.LC85, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L493
	movl	$.LC87, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L494
	movl	$.LC89, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L495
	movl	$.LC91, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L496
	movl	$.LC93, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L497
	movl	$.LC95, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L498
	movl	$.LC97, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L499
	movl	$.LC100, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L500
	movl	$.LC103, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L501
	movl	$.LC105, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L502
	movl	$.LC108, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L503
	movl	$.LC111, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L504
	movl	$.LC114, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L505
	movl	$.LC117, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L506
	movl	$.LC120, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L507
	movl	$.LC123, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L508
	movl	$.LC125, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L509
	movl	$.LC128, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L510
	movl	$.LC131, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L511
	movl	$.LC134, %esi
	movq	%r15, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L432
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L512
	movl	$.LC102, %edx
	movl	%r13d, %esi
	movl	$925, %edi
	call	exception.clone.0
	jmp	.L226
.L481:
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %r9d
	subl	$1, %r9d
	movl	%r9d, (%rbx)
	movslq	%r9d, %r15
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%r15)
	movl	%r13d, %edi
	call	exponential
	call	exp
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L512:
	movq	buffer(%rip), %r11
	movq	(%r11,%rbp,8), %r10
	movq	%rbx, %rdx
	addq	position(%rip), %rdx
	movl	(%rdx), %r12d
	subl	$1, %r12d
	movl	%r12d, (%rdx)
	movslq	%r12d, %r8
	movzbl	(%rax), %r14d
	movb	%r14b, (%r10,%r8)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rcx
	cmpb	$44, (%rcx,%rbp)
	je	.L513
	movl	$.LC135, %edx
	movl	%r13d, %esi
	movl	$912, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
	movsd	%xmm0, 24(%rsp)
.L435:
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r14
	cmpb	$44, (%r14,%rbp)
	je	.L514
	movl	$.LC136, %edx
	movl	%r13d, %esi
	movl	$921, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm2
.L437:
	movsd	24(%rsp), %xmm1
	movsd	16(%rsp), %xmm0
	call	udf3
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L432:
	movl	$.LC137, %edx
	movl	%r13d, %esi
	movl	$927, %edi
	call	exception.clone.0
	jmp	.L226
.L514:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r10
	movq	(%r10,%rbp,8), %r8
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %rbx
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%r8,%rax)
	movl	%r13d, %edi
	call	expression
	movapd	%xmm0, %xmm2
	jmp	.L437
.L513:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r15
	movq	%rbx, %r9
	addq	position(%rip), %r9
	movl	(%r9), %eax
	subl	$1, %eax
	movl	%eax, (%r9)
	cltq
	movq	ch(%rip), %rsi
	movzbl	(%rsi,%rbp), %edi
	movb	%dil, (%r15,%rax)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 24(%rsp)
	jmp	.L435
.L511:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L515
	movl	$.LC102, %edx
	movl	%r13d, %esi
	movl	$893, %edi
	call	exception.clone.0
	jmp	.L226
.L510:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L516
	movl	$.LC130, %edx
	movl	%r13d, %esi
	movl	$859, %edi
	call	exception.clone.0
	jmp	.L226
.L515:
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %rsi
	movq	%rbx, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %r9d
	subl	$1, %r9d
	movl	%r9d, (%rcx)
	movslq	%r9d, %r15
	movzbl	(%rax), %edi
	movb	%dil, (%rsi,%r15)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r11
	cmpb	$44, (%r11,%rbp)
	je	.L517
	movl	$.LC132, %edx
	movl	%r13d, %esi
	movl	$878, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm1
	movsd	%xmm1, 24(%rsp)
.L427:
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rdi
	cmpb	$44, (%rdi,%rbp)
	je	.L518
	movl	$.LC133, %edx
	movl	%r13d, %esi
	movl	$887, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
.L429:
	pxor	%xmm10, %xmm10
	cmpltsd	16(%rsp), %xmm10
	movsd	24(%rsp), %xmm11
	andpd	%xmm10, %xmm11
	andnpd	%xmm0, %xmm10
	orpd	%xmm11, %xmm10
	movsd	%xmm10, 64(%rsp)
	jmp	.L226
.L516:
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %r15
	movq	%rbx, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %esi
	subl	$1, %esi
	movl	%esi, (%rcx)
	movslq	%esi, %rdi
	movzbl	(%rax), %r11d
	movb	%r11b, (%r15,%rdi)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r12
	cmpb	$44, (%r12,%rbp)
	je	.L519
	movl	$.LC129, %edx
	movl	%r13d, %esi
	movl	$851, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
.L417:
	movsd	.LC79(%rip), %xmm1
	movsd	16(%rsp), %xmm8
	cvttsd2siq	%xmm8, %rax
	comisd	%xmm1, %xmm8
	jb	.L419
	movsd	16(%rsp), %xmm9
	subsd	%xmm1, %xmm9
	cvttsd2siq	%xmm9, %rax
	movabsq	$-9223372036854775808, %rbp
	xorq	%rbp, %rax
.L419:
	cvttsd2siq	%xmm0, %rdx
	comisd	%xmm1, %xmm0
	jb	.L421
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movabsq	$-9223372036854775808, %r12
	xorq	%r12, %rdx
.L421:
	xorq	%rdx, %rax
	js	.L422
	cvtsi2sdq	%rax, %xmm0
.L423:
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L519:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r10
	movq	(%r10,%rbp,8), %r14
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r8
	movzbl	(%r8,%rbp), %ebx
	movb	%bl, (%r14,%rax)
	movl	%r13d, %edi
	call	expression
	jmp	.L417
.L422:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L423
.L518:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %r15
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %rsi
	movzbl	(%rsi,%rbp), %ebx
	movb	%bl, (%r15,%rax)
	movl	%r13d, %edi
	call	expression
	jmp	.L429
.L517:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r11
	movq	(%r11,%rbp,8), %r10
	movq	%rbx, %rdx
	addq	position(%rip), %rdx
	movl	(%rdx), %eax
	subl	$1, %eax
	movl	%eax, (%rdx)
	cltq
	movq	ch(%rip), %r12
	movzbl	(%r12,%rbp), %r8d
	movb	%r8b, (%r10,%rax)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 24(%rsp)
	jmp	.L427
.L509:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L520
	movl	$.LC127, %edx
	movl	%r13d, %esi
	movl	$833, %edi
	call	exception.clone.0
	jmp	.L226
.L508:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L521
	movl	$.LC122, %edx
	movl	%r13d, %esi
	movl	$807, %edi
	call	exception.clone.0
	jmp	.L226
.L520:
	movq	buffer(%rip), %rsi
	movq	(%rsi,%rbp,8), %rdi
	movq	%rbx, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %r15d
	subl	$1, %r15d
	movl	%r15d, (%rcx)
	movslq	%r15d, %r11
	movzbl	(%rax), %r12d
	movb	%r12b, (%rdi,%r11)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r10
	cmpb	$44, (%r10,%rbp)
	je	.L522
	movl	$.LC126, %edx
	movl	%r13d, %esi
	movl	$825, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
.L407:
	movsd	.LC79(%rip), %xmm1
	movsd	16(%rsp), %xmm6
	cvttsd2siq	%xmm6, %rax
	comisd	%xmm1, %xmm6
	jb	.L409
	movsd	16(%rsp), %xmm7
	subsd	%xmm1, %xmm7
	cvttsd2siq	%xmm7, %rax
	movabsq	$-9223372036854775808, %rbp
	xorq	%rbp, %rax
.L409:
	cvttsd2siq	%xmm0, %rdx
	comisd	%xmm1, %xmm0
	jb	.L411
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movabsq	$-9223372036854775808, %r10
	xorq	%r10, %rdx
.L411:
	orq	%rdx, %rax
	js	.L412
	cvtsi2sdq	%rax, %xmm0
.L413:
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L522:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r8
	movq	(%r8,%rbp,8), %r9
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r14
	movzbl	(%r14,%rbp), %ebx
	movb	%bl, (%r9,%rax)
	movl	%r13d, %edi
	call	expression
	jmp	.L407
.L493:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L523
	movl	$.LC86, %edx
	movl	%r13d, %esi
	movl	$510, %edi
	call	exception.clone.0
	jmp	.L226
.L492:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L524
	movl	$.LC84, %edx
	movl	%r13d, %esi
	movl	$498, %edi
	call	exception.clone.0
	jmp	.L226
.L523:
	movq	buffer(%rip), %rdx
	movq	(%rdx,%rbp,8), %rdi
	addq	position(%rip), %rbx
	movl	(%rbx), %r15d
	subl	$1, %r15d
	movl	%r15d, (%rbx)
	movslq	%r15d, %rsi
	movzbl	(%rax), %r11d
	movb	%r11b, (%rdi,%rsi)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L319
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %r9
	xorq	%r9, %rax
.L319:
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm8
	movsd	%xmm8, 64(%rsp)
	jmp	.L226
.L524:
	movq	buffer(%rip), %r12
	movq	(%r12,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %r10d
	subl	$1, %r10d
	movl	%r10d, (%rbx)
	movslq	%r10d, %r8
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%r8)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L313
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rbp
	xorq	%rbp, %rax
.L313:
	andl	$16711680, %eax
	shrq	$16, %rax
	cvtsi2sdq	%rax, %xmm7
	movsd	%xmm7, 64(%rsp)
	jmp	.L226
.L491:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L525
	movl	$.LC82, %edx
	movl	%r13d, %esi
	movl	$486, %edi
	call	exception.clone.0
	jmp	.L226
.L490:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L526
	movl	$.LC80, %edx
	movl	%r13d, %esi
	movl	$474, %edi
	call	exception.clone.0
	jmp	.L226
.L525:
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %r15
	addq	position(%rip), %rbx
	movl	(%rbx), %edx
	subl	$1, %edx
	movl	%edx, (%rbx)
	movslq	%edx, %rdi
	movzbl	(%rax), %esi
	movb	%sil, (%r15,%rdi)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L307
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %r14
	xorq	%r14, %rax
.L307:
	movzbl	%ah, %eax
	cvtsi2sdq	%rax, %xmm6
	movsd	%xmm6, 64(%rsp)
	jmp	.L226
.L526:
	movq	buffer(%rip), %r12
	movq	(%r12,%rbp,8), %r10
	addq	position(%rip), %rbx
	movl	(%rbx), %ebp
	subl	$1, %ebp
	movl	%ebp, (%rbx)
	movslq	%ebp, %r8
	movzbl	(%rax), %ebx
	movb	%bl, (%r10,%r8)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L301
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %r11
	xorq	%r11, %rax
.L301:
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm5
	movsd	%xmm5, 64(%rsp)
	jmp	.L226
.L489:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L527
	movl	$.LC77, %edx
	movl	%r13d, %esi
	movl	$462, %edi
	call	exception.clone.0
	jmp	.L226
.L488:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L528
	movl	$.LC75, %edx
	movl	%r13d, %esi
	movl	$452, %edi
	call	exception.clone.0
	jmp	.L226
.L527:
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r9
	addq	position(%rip), %rbx
	movl	(%rbx), %r14d
	subl	$1, %r14d
	movl	%r14d, (%rbx)
	movslq	%r14d, %rdx
	movzbl	(%rax), %eax
	movb	%al, (%r9,%rdx)
	movl	%r13d, %edi
	call	exponential
	call	udf1
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L528:
	movq	buffer(%rip), %r15
	movq	(%r15,%rbp,8), %rsi
	addq	position(%rip), %rbx
	movl	(%rbx), %edi
	subl	$1, %edi
	movl	%edi, (%rbx)
	movslq	%edi, %r11
	movzbl	(%rax), %r12d
	movb	%r12b, (%rsi,%r11)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC74(%rip), %xmm4
	andpd	%xmm4, %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L487:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L529
	movl	$.LC72, %edx
	movl	%r13d, %esi
	movl	$442, %edi
	call	exception.clone.0
	jmp	.L226
.L486:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L530
	movl	$.LC68, %edx
	movl	%r13d, %esi
	movl	$429, %edi
	call	exception.clone.0
	jmp	.L226
.L529:
	movq	buffer(%rip), %r10
	movq	(%r10,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %r8d
	subl	$1, %r8d
	movl	%r8d, (%rbx)
	movslq	%r8d, %r14
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%r14)
	movl	%r13d, %edi
	call	exponential
	movapd	%xmm0, %xmm2
	pxor	%xmm0, %xmm0
	comisd	%xmm0, %xmm2
	jbe	.L445
	movabsq	$4607182418800017408, %rbp
	movq	%rbp, 64(%rsp)
	jmp	.L226
.L530:
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %r15
	addq	position(%rip), %rbx
	movl	(%rbx), %edx
	subl	$1, %edx
	movl	%edx, (%rbx)
	movslq	%edx, %rdi
	movzbl	(%rax), %eax
	movb	%al, (%r15,%rdi)
	movl	%r13d, %edi
	call	exponential
	call	ceil
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L445:
	movsd	.LC71(%rip), %xmm3
	cmpltsd	%xmm0, %xmm2
	andpd	%xmm2, %xmm3
	orpd	%xmm3, %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L485:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L531
	movl	$.LC66, %edx
	movl	%r13d, %esi
	movl	$419, %edi
	call	exception.clone.0
	jmp	.L226
.L484:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L532
	movl	$.LC64, %edx
	movl	%r13d, %esi
	movl	$409, %edi
	call	exception.clone.0
	jmp	.L226
.L531:
	movq	buffer(%rip), %rsi
	movq	(%rsi,%rbp,8), %r12
	addq	position(%rip), %rbx
	movl	(%rbx), %r11d
	subl	$1, %r11d
	movl	%r11d, (%rbx)
	movslq	%r11d, %r10
	movzbl	(%rax), %ebp
	movb	%bpl, (%r12,%r10)
	movl	%r13d, %edi
	call	exponential
	call	cbrt
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L532:
	movq	buffer(%rip), %r8
	movq	(%r8,%rbp,8), %rcx
	addq	position(%rip), %rbx
	movl	(%rbx), %r14d
	subl	$1, %r14d
	movl	%r14d, (%rbx)
	movslq	%r14d, %r9
	movzbl	(%rax), %ebx
	movb	%bl, (%rcx,%r9)
	movl	%r13d, %edi
	call	exponential
	sqrtsd	%xmm0, %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L483:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L533
	movl	$.LC62, %edx
	movl	%r13d, %esi
	movl	$399, %edi
	call	exception.clone.0
	jmp	.L226
.L482:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L534
	movl	$.LC60, %edx
	movl	%r13d, %esi
	movl	$389, %edi
	call	exception.clone.0
	jmp	.L226
.L533:
	movq	buffer(%rip), %r15
	movq	(%r15,%rbp,8), %rdi
	addq	position(%rip), %rbx
	movl	(%rbx), %edx
	subl	$1, %edx
	movl	%edx, (%rbx)
	movslq	%edx, %rsi
	movzbl	(%rax), %eax
	movb	%al, (%rdi,%rsi)
	movl	%r13d, %edi
	call	exponential
	call	log10
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L534:
	movq	buffer(%rip), %r11
	movq	(%r11,%rbp,8), %r10
	addq	position(%rip), %rbx
	movl	(%rbx), %r12d
	subl	$1, %r12d
	movl	%r12d, (%rbx)
	movslq	%r12d, %r8
	movzbl	(%rax), %ebp
	movb	%bpl, (%r10,%r8)
	movl	%r13d, %edi
	call	exponential
	call	log
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L470:
	movl	$.LC19, %edx
	movl	%r13d, %esi
	movl	$223, %edi
	call	exception.clone.0
	jmp	.L226
.L469:
	movl	$.LC18, %edx
	movl	%r13d, %esi
	movl	$222, %edi
	call	exception.clone.0
	jmp	.L226
.L471:
	movl	$.LC20, %edx
	movl	%r13d, %esi
	movl	$224, %edi
	call	exception.clone.0
	jmp	.L226
.L477:
	movq	buffer(%rip), %rdi
	movq	(%rdi,%rbp,8), %r11
	addq	position(%rip), %rbx
	movl	(%rbx), %esi
	subl	$1, %esi
	movl	%esi, (%rbx)
	movslq	%esi, %r12
	movzbl	(%rax), %eax
	movb	%al, (%r11,%r12)
	movl	%r13d, %edi
	call	exponential
	call	atan
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L501:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L535
	movl	$.LC102, %edx
	movl	%r13d, %esi
	movl	$640, %edi
	call	exception.clone.0
	jmp	.L226
.L500:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L536
	movl	$.LC102, %edx
	movl	%r13d, %esi
	movl	$618, %edi
	call	exception.clone.0
	jmp	.L226
.L535:
	movq	buffer(%rip), %r8
	movq	(%r8,%rbp,8), %r9
	movq	%rbx, %rdx
	addq	position(%rip), %rdx
	movl	(%rdx), %r14d
	subl	$1, %r14d
	movl	%r14d, (%rdx)
	movslq	%r14d, %r15
	movzbl	(%rax), %edi
	movb	%dil, (%r9,%r15)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rsi
	cmpb	$44, (%rsi,%rbp)
	je	.L537
	movl	$.LC104, %edx
	movl	%r13d, %esi
	movl	$636, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
.L363:
	maxsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L536:
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movq	%rbx, %r8
	addq	position(%rip), %r8
	movl	(%r8), %edx
	subl	$1, %edx
	movl	%edx, (%r8)
	movslq	%edx, %r9
	movzbl	(%rax), %r15d
	movb	%r15b, (%r14,%r9)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rdi
	cmpb	$44, (%rdi,%rbp)
	je	.L538
	movl	$.LC101, %edx
	movl	%r13d, %esi
	movl	$614, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm1
.L359:
	movsd	16(%rsp), %xmm0
	call	udf2
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L537:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r12
	movq	(%r12,%rbp,8), %r10
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %rcx
	movzbl	(%rcx,%rbp), %ebx
	movb	%bl, (%r10,%rax)
	movl	%r13d, %edi
	call	expression
	jmp	.L363
.L538:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r11
	movq	(%r11,%rbp,8), %r10
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r12
	movzbl	(%r12,%rbp), %ebp
	movb	%bpl, (%r10,%rax)
	movl	%r13d, %edi
	call	expression
	movapd	%xmm0, %xmm1
	jmp	.L359
.L499:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L539
	movl	$.LC99, %edx
	movl	%r13d, %esi
	movl	$596, %edi
	call	exception.clone.0
	jmp	.L226
.L498:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L540
	movl	$.LC96, %edx
	movl	%r13d, %esi
	movl	$574, %edi
	call	exception.clone.0
	jmp	.L226
.L539:
	movq	buffer(%rip), %r10
	movq	(%r10,%rbp,8), %r14
	movq	%rbx, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %r8d
	subl	$1, %r8d
	movl	%r8d, (%rcx)
	movslq	%r8d, %rdx
	movzbl	(%rax), %eax
	movb	%al, (%r14,%rdx)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r9
	cmpb	$44, (%r9,%rbp)
	je	.L541
	movl	$.LC98, %edx
	movl	%r13d, %esi
	movl	$592, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm1
.L355:
	movsd	16(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L540:
	movq	buffer(%rip), %rsi
	movq	(%rsi,%rbp,8), %rbp
	addq	position(%rip), %rbx
	movl	(%rbx), %r11d
	subl	$1, %r11d
	movl	%r11d, (%rbx)
	movslq	%r11d, %r12
	movzbl	(%rax), %r10d
	movb	%r10b, 0(%rbp,%r12)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L349
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdi
	xorq	%rdi, %rax
.L349:
	xorq	$-1, %rax
	js	.L350
	cvtsi2sdq	%rax, %xmm0
.L351:
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L541:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %rsi
	movq	(%rsi,%rbp,8), %r12
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r11
	movzbl	(%r11,%rbp), %ebx
	movb	%bl, (%r12,%rax)
	movl	%r13d, %edi
	call	expression
	movapd	%xmm0, %xmm1
	jmp	.L355
.L350:
	movq	%rax, %r15
	shrq	%r15
	andl	$1, %eax
	orq	%rax, %r15
	cvtsi2sdq	%r15, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L351
.L497:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L542
	movl	$.LC94, %edx
	movl	%r13d, %esi
	movl	$562, %edi
	call	exception.clone.0
	jmp	.L226
.L496:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L543
	movl	$.LC92, %edx
	movl	%r13d, %esi
	movl	$547, %edi
	call	exception.clone.0
	jmp	.L226
.L542:
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %rdi
	addq	position(%rip), %rbx
	movl	(%rbx), %r15d
	subl	$1, %r15d
	movl	%r15d, (%rbx)
	movslq	%r15d, %rsi
	movzbl	(%rax), %eax
	movb	%al, (%rdi,%rsi)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L343
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
.L343:
	movzbl	%ah, %ecx
	movq	%rax, %rbx
	andl	$16711680, %ebx
	shrq	$16, %rbx
	leaq	(%rcx,%rbx), %r8
	andl	$255, %eax
	leaq	(%r8,%rax), %rax
	movl	$3, %r14d
	xorl	%edx, %edx
	divq	%r14
	cvtsi2sdq	%rax, %xmm11
	movsd	%xmm11, 64(%rsp)
	jmp	.L226
.L543:
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	addq	position(%rip), %rbx
	movl	(%rbx), %r8d
	subl	$1, %r8d
	movl	%r8d, (%rbx)
	movslq	%r8d, %rdx
	movzbl	(%rax), %r9d
	movb	%r9b, (%r14,%rdx)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L337
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rbx
	xorq	%rbx, %rax
.L337:
	andl	$255, %eax
	movq	%rax, %r11
	salq	$8, %r11
	movq	%rax, %rbp
	salq	$16, %rbp
	leaq	(%r11,%rbp), %r12
	leaq	(%r12,%rax), %r10
	cvtsi2sdq	%r10, %xmm10
	movsd	%xmm10, 64(%rsp)
	jmp	.L226
.L495:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L544
	movl	$.LC90, %edx
	movl	%r13d, %esi
	movl	$534, %edi
	call	exception.clone.0
	jmp	.L226
.L494:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L545
	movl	$.LC88, %edx
	movl	%r13d, %esi
	movl	$522, %edi
	call	exception.clone.0
	jmp	.L226
.L544:
	movq	buffer(%rip), %rdi
	movq	(%rdi,%rbp,8), %r11
	addq	position(%rip), %rbx
	movl	(%rbx), %esi
	subl	$1, %esi
	movl	%esi, (%rbx)
	movslq	%esi, %rbp
	movzbl	(%rax), %eax
	movb	%al, (%r11,%rbp)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L331
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %r15
	xorq	%r15, %rax
.L331:
	andl	$255, %eax
	salq	$16, %rax
	js	.L332
	cvtsi2sdq	%rax, %xmm0
.L333:
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L545:
	movq	buffer(%rip), %r10
	movq	(%r10,%rbp,8), %r8
	addq	position(%rip), %rbx
	movl	(%rbx), %ecx
	subl	$1, %ecx
	movl	%ecx, (%rbx)
	movslq	%ecx, %rbx
	movzbl	(%rax), %r14d
	movb	%r14b, (%r8,%rbx)
	movl	%r13d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %rax
	comisd	%xmm1, %xmm0
	jb	.L325
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %r12
	xorq	%r12, %rax
.L325:
	salq	$8, %rax
	andl	$65535, %eax
	cvtsi2sdq	%rax, %xmm9
	movsd	%xmm9, 64(%rsp)
	jmp	.L226
.L332:
	shrq	%rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L333
.L505:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L546
	movl	$.LC116, %edx
	movl	%r13d, %esi
	movl	$732, %edi
	call	exception.clone.0
	jmp	.L226
.L504:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L547
	movl	$.LC113, %edx
	movl	%r13d, %esi
	movl	$708, %edi
	call	exception.clone.0
	jmp	.L226
.L546:
	movq	buffer(%rip), %rsi
	movq	(%rsi,%rbp,8), %r11
	movq	%rbx, %r15
	addq	position(%rip), %r15
	movl	(%r15), %edi
	subl	$1, %edi
	movl	%edi, (%r15)
	movslq	%edi, %r12
	movzbl	(%rax), %ecx
	movb	%cl, (%r11,%r12)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r10
	cmpb	$44, (%r10,%rbp)
	je	.L548
	movl	$.LC115, %edx
	movl	%r13d, %esi
	movl	$726, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm14
	movsd	%xmm14, 64(%rsp)
	jmp	.L226
.L547:
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %rdi
	movq	%rbx, %rsi
	addq	position(%rip), %rsi
	movl	(%rsi), %r15d
	subl	$1, %r15d
	movl	%r15d, (%rsi)
	movslq	%r15d, %r11
	movzbl	(%rax), %r12d
	movb	%r12b, (%rdi,%r11)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rcx
	cmpb	$44, (%rcx,%rbp)
	je	.L549
	movl	$.LC112, %edx
	movl	%r13d, %esi
	movl	$703, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm13
	movsd	%xmm13, 64(%rsp)
	jmp	.L226
.L548:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %rdx
	movq	(%rdx,%rbp,8), %r9
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r14
	movzbl	(%r14,%rbp), %ebx
	movb	%bl, (%r9,%rax)
	movl	%r13d, %edi
	call	expression
	minsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L549:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r8
	movq	(%r8,%rbp,8), %r14
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %rdx
	movzbl	(%rdx,%rbp), %ebp
	movb	%bpl, (%r14,%rax)
	movl	%r13d, %edi
	call	expression
	maxsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L503:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L550
	movl	$.LC110, %edx
	movl	%r13d, %esi
	movl	$685, %edi
	call	exception.clone.0
	jmp	.L226
.L502:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L551
	movl	$.LC107, %edx
	movl	%r13d, %esi
	movl	$662, %edi
	call	exception.clone.0
	jmp	.L226
.L550:
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %r15
	movq	%rbx, %r9
	addq	position(%rip), %r9
	movl	(%r9), %esi
	subl	$1, %esi
	movl	%esi, (%r9)
	movslq	%esi, %rdi
	movzbl	(%rax), %r11d
	movb	%r11b, (%r15,%rdi)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r12
	cmpb	$44, (%r12,%rbp)
	je	.L552
	movl	$.LC109, %edx
	movl	%r13d, %esi
	movl	$680, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm12
	movsd	%xmm12, 64(%rsp)
	jmp	.L226
.L551:
	movq	buffer(%rip), %rdx
	movq	(%rdx,%rbp,8), %rsi
	movq	%rbx, %r14
	addq	position(%rip), %r14
	movl	(%r14), %r9d
	subl	$1, %r9d
	movl	%r9d, (%r14)
	movslq	%r9d, %r15
	movzbl	(%rax), %edi
	movb	%dil, (%rsi,%r15)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r11
	cmpb	$44, (%r11,%rbp)
	je	.L553
	movl	$.LC106, %edx
	movl	%r13d, %esi
	movl	$658, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
.L367:
	minsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L552:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r10
	movq	(%r10,%rbp,8), %rdx
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r8
	movzbl	(%r8,%rbp), %ebx
	movb	%bl, (%rdx,%rax)
	movl	%r13d, %edi
	call	expression
	maxsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L553:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r8
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r10
	movzbl	(%r10,%rbp), %ebp
	movb	%bpl, (%r8,%rax)
	movl	%r13d, %edi
	call	expression
	jmp	.L367
.L507:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L554
	movl	$.LC122, %edx
	movl	%r13d, %esi
	movl	$781, %edi
	call	exception.clone.0
	jmp	.L226
.L506:
	movl	%r13d, %edi
	call	skipblanks
	movq	%rbp, %rax
	addq	ch(%rip), %rax
	cmpb	$40, (%rax)
	je	.L555
	movl	$.LC119, %edx
	movl	%r13d, %esi
	movl	$755, %edi
	call	exception.clone.0
	jmp	.L226
.L554:
	movq	buffer(%rip), %rdi
	movq	(%rdi,%rbp,8), %r12
	movq	%rbx, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %r11d
	subl	$1, %r11d
	movl	%r11d, (%rcx)
	movslq	%r11d, %r10
	movzbl	(%rax), %r8d
	movb	%r8b, (%r12,%r10)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	cmpb	$44, (%rdx,%rbp)
	je	.L556
	movl	$.LC121, %edx
	movl	%r13d, %esi
	movl	$773, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
.L387:
	movsd	.LC79(%rip), %xmm1
	movsd	16(%rsp), %xmm2
	cvttsd2siq	%xmm2, %rax
	comisd	%xmm1, %xmm2
	jb	.L389
	movsd	16(%rsp), %xmm3
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rbp
	xorq	%rbp, %rax
.L389:
	cvttsd2siq	%xmm0, %rcx
	comisd	%xmm1, %xmm0
	jb	.L391
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rcx
	movabsq	$-9223372036854775808, %r14
	xorq	%r14, %rcx
.L391:
	xorl	%edx, %edx
	divq	%rcx
	cvtsi2sdq	%rdx, %xmm0
	testq	%rdx, %rdx
	jns	.L393
	movq	%rdx, %r8
	shrq	%r8
	andl	$1, %edx
	orq	%rdx, %r8
	cvtsi2sdq	%r8, %xmm0
	addsd	%xmm0, %xmm0
.L393:
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L556:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r9
	movq	(%r9,%rbp,8), %r15
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %rsi
	movzbl	(%rsi,%rbp), %ebx
	movb	%bl, (%r15,%rax)
	movl	%r13d, %edi
	call	expression
	jmp	.L387
.L555:
	movq	buffer(%rip), %r15
	movq	(%r15,%rbp,8), %r12
	movq	%rbx, %rdi
	addq	position(%rip), %rdi
	movl	(%rdi), %r11d
	subl	$1, %r11d
	movl	%r11d, (%rdi)
	movslq	%r11d, %rcx
	movzbl	(%rax), %r10d
	movb	%r10b, (%r12,%rcx)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	cmpb	$44, (%r8,%rbp)
	je	.L557
	movl	$.LC118, %edx
	movl	%r13d, %esi
	movl	$750, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm15
	movsd	%xmm15, 64(%rsp)
	jmp	.L226
.L521:
	movq	buffer(%rip), %r15
	movq	(%r15,%rbp,8), %r11
	movq	%rbx, %rcx
	addq	position(%rip), %rcx
	movl	(%rcx), %edi
	subl	$1, %edi
	movl	%edi, (%rcx)
	movslq	%edi, %r12
	movzbl	(%rax), %r10d
	movb	%r10b, (%r11,%r12)
	movl	%r13d, %edi
	call	expression
	movsd	%xmm0, 16(%rsp)
	movl	%r13d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	cmpb	$44, (%rdx,%rbp)
	je	.L558
	movl	$.LC124, %edx
	movl	%r13d, %esi
	movl	$799, %edi
	call	exception.clone.0
	movsd	16(%rsp), %xmm0
.L397:
	movsd	.LC79(%rip), %xmm1
	movsd	16(%rsp), %xmm4
	cvttsd2siq	%xmm4, %rax
	comisd	%xmm1, %xmm4
	jb	.L399
	movsd	16(%rsp), %xmm5
	subsd	%xmm1, %xmm5
	cvttsd2siq	%xmm5, %rax
	movabsq	$-9223372036854775808, %rbp
	xorq	%rbp, %rax
.L399:
	cvttsd2siq	%xmm0, %rdx
	comisd	%xmm1, %xmm0
	jb	.L401
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movabsq	$-9223372036854775808, %r8
	xorq	%r8, %rdx
.L401:
	andq	%rdx, %rax
	js	.L402
	cvtsi2sdq	%rax, %xmm0
.L403:
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
.L558:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %rsi
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r9
	movzbl	(%r9,%rbp), %ebx
	movb	%bl, (%rsi,%rax)
	movl	%r13d, %edi
	call	expression
	jmp	.L397
.L402:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L403
.L412:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L413
.L557:
	movl	%r13d, %edi
	call	skipblanks
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %rsi
	addq	position(%rip), %rbx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	cltq
	movq	ch(%rip), %r9
	movzbl	(%r9,%rbp), %ebp
	movb	%bpl, (%rsi,%rax)
	movl	%r13d, %edi
	call	expression
	minsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L226
	.cfi_endproc
.LFE26:
	.size	factor, .-factor
	.section	.rodata.str1.8
	.align 8
.LC139:
	.string	"Garbage in function expression.\n"
	.align 8
.LC140:
	.string	"Value returned MAY BE invalid, exceptions occured.\n"
	.text
.globl fpar_ok
	.type	fpar_ok, @function
fpar_ok:
.LFB19:
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
	subq	$72, %rsp
	.cfi_def_cfa_offset 128
	movl	%edi, 52(%rsp)
	movslq	%edi, %rbx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	%rbx, 40(%rsp)
	salq	$2, %rbx
	movq	%rbx, 32(%rsp)
	movq	nvar(%rip), %rcx
	movq	32(%rsp), %rdx
	movl	(%rcx,%rdx), %ebp
	movq	ranks(%rip), %rax
	movl	(%rax,%rdx), %r14d
	movslq	%r14d, %rdi
	salq	$3, %rdi
	call	malloc
	movq	%rax, %rbx
	testl	%r14d, %r14d
	jle	.L560
	movslq	%ebp, %r15
	salq	$3, %r15
	movq	%rax, %r13
	xorl	%r12d, %r12d
	movdqa	.LC138(%rip), %xmm2
.L568:
	movq	%r15, %rdi
	movdqa	%xmm2, (%rsp)
	call	malloc
	movq	%rax, 0(%r13)
	testl	%ebp, %ebp
	movdqa	(%rsp), %xmm2
	jle	.L561
	movq	%rax, %rcx
	andl	$15, %ecx
	shrq	$3, %rcx
	cmpl	%ebp, %ecx
	cmova	%ebp, %ecx
	mov	%ecx, %edx
	testl	%ecx, %ecx
	je	.L584
	movq	$0, (%rax)
	cmpl	%ecx, %ebp
	je	.L561
	movl	$1, %esi
.L562:
	movl	%ebp, %r10d
	subl	%ecx, %r10d
	movl	%r10d, %edi
	shrl	$2, %edi
	movl	%edi, %r11d
	sall	$2, %r11d
	je	.L564
	leal	1(%rsi), %ecx
	leal	2(%rsi), %r9d
	leal	3(%rsi), %r8d
	movd	%r9d, %xmm0
	movd	%r8d, %xmm1
	punpckldq	%xmm1, %xmm0
	movd	%esi, %xmm1
	movd	%ecx, %xmm4
	punpckldq	%xmm4, %xmm1
	punpcklqdq	%xmm0, %xmm1
	movd	%r12d, %xmm3
	pshufd	$0, %xmm3, %xmm0
	leaq	(%rax,%rdx,8), %rdx
	xorl	%ecx, %ecx
	leal	-1(%rdi), %r8d
	andl	$3, %r8d
	je	.L565
	movdqa	%xmm1, %xmm14
	pmuludq	%xmm0, %xmm14
	movdqa	%xmm1, %xmm15
	psrldq	$4, %xmm15
	movdqa	%xmm0, %xmm13
	psrldq	$4, %xmm13
	pmuludq	%xmm15, %xmm13
	pshufd	$8, %xmm14, %xmm10
	pshufd	$8, %xmm13, %xmm12
	punpckldq	%xmm12, %xmm10
	cvtdq2pd	%xmm10, %xmm11
	movapd	%xmm11, (%rdx)
	pshufd	$238, %xmm10, %xmm9
	cvtdq2pd	%xmm9, %xmm3
	movapd	%xmm3, 16(%rdx)
	movl	$1, %ecx
	addq	$32, %rdx
	paddd	%xmm2, %xmm1
	cmpl	$1, %r8d
	je	.L565
	cmpl	$2, %r8d
	je	.L688
	movdqa	%xmm1, %xmm3
	pmuludq	%xmm0, %xmm3
	movdqa	%xmm1, %xmm10
	psrldq	$4, %xmm10
	movdqa	%xmm0, %xmm9
	psrldq	$4, %xmm9
	pmuludq	%xmm10, %xmm9
	pshufd	$8, %xmm3, %xmm7
	pshufd	$8, %xmm9, %xmm5
	punpckldq	%xmm5, %xmm7
	cvtdq2pd	%xmm7, %xmm8
	movapd	%xmm8, (%rdx)
	pshufd	$238, %xmm7, %xmm4
	cvtdq2pd	%xmm4, %xmm6
	movapd	%xmm6, 16(%rdx)
	movb	$2, %cl
	addq	$32, %rdx
	paddd	%xmm2, %xmm1
.L688:
	movdqa	%xmm1, %xmm4
	pmuludq	%xmm0, %xmm4
	movdqa	%xmm1, %xmm7
	psrldq	$4, %xmm7
	movdqa	%xmm0, %xmm6
	psrldq	$4, %xmm6
	pmuludq	%xmm7, %xmm6
	pshufd	$8, %xmm4, %xmm13
	pshufd	$8, %xmm6, %xmm15
	punpckldq	%xmm15, %xmm13
	cvtdq2pd	%xmm13, %xmm14
	movapd	%xmm14, (%rdx)
	pshufd	$238, %xmm13, %xmm12
	cvtdq2pd	%xmm12, %xmm11
	movapd	%xmm11, 16(%rdx)
	addl	$1, %ecx
	addq	$32, %rdx
	paddd	%xmm2, %xmm1
	jmp	.L565
.L585:
	movdqa	%xmm3, %xmm6
	paddd	%xmm2, %xmm6
	movdqa	%xmm3, %xmm12
	pmuludq	%xmm0, %xmm12
	psrldq	$4, %xmm3
	movdqa	%xmm0, %xmm13
	psrldq	$4, %xmm13
	pmuludq	%xmm13, %xmm3
	pshufd	$8, %xmm12, %xmm9
	pshufd	$8, %xmm3, %xmm11
	punpckldq	%xmm11, %xmm9
	cvtdq2pd	%xmm9, %xmm10
	movapd	%xmm10, 32(%rdx)
	pshufd	$238, %xmm9, %xmm8
	cvtdq2pd	%xmm8, %xmm1
	movapd	%xmm1, 16(%r9)
	movdqa	%xmm6, %xmm1
	paddd	%xmm2, %xmm1
	movdqa	%xmm6, %xmm7
	pmuludq	%xmm0, %xmm7
	psrldq	$4, %xmm6
	movdqa	%xmm0, %xmm5
	psrldq	$4, %xmm5
	pmuludq	%xmm5, %xmm6
	pshufd	$8, %xmm7, %xmm15
	pshufd	$8, %xmm6, %xmm4
	punpckldq	%xmm4, %xmm15
	cvtdq2pd	%xmm15, %xmm3
	movapd	%xmm3, 64(%rdx)
	pshufd	$238, %xmm15, %xmm14
	cvtdq2pd	%xmm14, %xmm13
	movapd	%xmm13, 80(%rdx)
	movdqa	%xmm1, %xmm11
	pmuludq	%xmm0, %xmm11
	movdqa	%xmm1, %xmm12
	psrldq	$4, %xmm12
	movdqa	%xmm0, %xmm10
	psrldq	$4, %xmm10
	pmuludq	%xmm12, %xmm10
	pshufd	$8, %xmm11, %xmm7
	pshufd	$8, %xmm10, %xmm9
	punpckldq	%xmm9, %xmm7
	cvtdq2pd	%xmm7, %xmm8
	movapd	%xmm8, 96(%rdx)
	pshufd	$238, %xmm7, %xmm6
	cvtdq2pd	%xmm6, %xmm5
	movapd	%xmm5, 112(%rdx)
	addl	$4, %ecx
	subq	$-128, %rdx
	paddd	%xmm2, %xmm1
.L565:
	movdqa	%xmm1, %xmm3
	paddd	%xmm2, %xmm3
	movdqa	%xmm1, %xmm5
	pmuludq	%xmm0, %xmm5
	psrldq	$4, %xmm1
	movdqa	%xmm0, %xmm8
	psrldq	$4, %xmm8
	pmuludq	%xmm1, %xmm8
	pshufd	$8, %xmm5, %xmm6
	pshufd	$8, %xmm8, %xmm7
	punpckldq	%xmm7, %xmm6
	cvtdq2pd	%xmm6, %xmm4
	movapd	%xmm4, (%rdx)
	pshufd	$238, %xmm6, %xmm15
	cvtdq2pd	%xmm15, %xmm14
	movapd	%xmm14, 16(%rdx)
	leal	1(%rcx), %r8d
	leaq	32(%rdx), %r9
	cmpl	%edi, %r8d
	jb	.L585
	addl	%r11d, %esi
	cmpl	%r11d, %r10d
	je	.L561
.L564:
	movl	%esi, %edx
	imull	%r12d, %edx
	movslq	%esi, %rdi
	leaq	(%rax,%rdi,8), %r8
	movl	%esi, %edi
	notl	%edi
	addl	%ebp, %edi
	andl	$7, %edi
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, (%r8)
	leal	1(%rsi), %ecx
	addl	%r12d, %edx
	leaq	8(%r8), %rax
	cmpl	%ecx, %ebp
	jle	.L561
	testl	%edi, %edi
	je	.L567
	cmpl	$1, %edi
	je	.L682
	cmpl	$2, %edi
	.p2align 4,,3
	je	.L683
	cmpl	$3, %edi
	.p2align 4,,2
	je	.L684
	cmpl	$4, %edi
	.p2align 4,,2
	je	.L685
	cmpl	$5, %edi
	.p2align 4,,2
	je	.L686
	cmpl	$6, %edi
	.p2align 4,,2
	je	.L687
	cvtsi2sd	%edx, %xmm15
	movsd	%xmm15, 8(%r8)
	leal	2(%rsi), %ecx
	addl	%r12d, %edx
	leaq	16(%r8), %rax
.L687:
	cvtsi2sd	%edx, %xmm6
	movsd	%xmm6, (%rax)
	addl	$1, %ecx
	addl	%r12d, %edx
	addq	$8, %rax
.L686:
	cvtsi2sd	%edx, %xmm4
	movsd	%xmm4, (%rax)
	addl	$1, %ecx
	addl	%r12d, %edx
	addq	$8, %rax
.L685:
	cvtsi2sd	%edx, %xmm7
	movsd	%xmm7, (%rax)
	addl	$1, %ecx
	addl	%r12d, %edx
	addq	$8, %rax
.L684:
	cvtsi2sd	%edx, %xmm8
	movsd	%xmm8, (%rax)
	addl	$1, %ecx
	addl	%r12d, %edx
	addq	$8, %rax
.L683:
	cvtsi2sd	%edx, %xmm5
	movsd	%xmm5, (%rax)
	addl	$1, %ecx
	addl	%r12d, %edx
	addq	$8, %rax
.L682:
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, (%rax)
	addl	$1, %ecx
	addl	%r12d, %edx
	addq	$8, %rax
	cmpl	%ecx, %ebp
	jle	.L561
.L567:
	cvtsi2sd	%edx, %xmm14
	movsd	%xmm14, (%rax)
	addl	%r12d, %edx
	cvtsi2sd	%edx, %xmm13
	movsd	%xmm13, 8(%rax)
	addl	%r12d, %edx
	cvtsi2sd	%edx, %xmm12
	movsd	%xmm12, 16(%rax)
	addl	%r12d, %edx
	cvtsi2sd	%edx, %xmm11
	movsd	%xmm11, 24(%rax)
	addl	%r12d, %edx
	cvtsi2sd	%edx, %xmm10
	movsd	%xmm10, 32(%rax)
	addl	%r12d, %edx
	cvtsi2sd	%edx, %xmm9
	movsd	%xmm9, 40(%rax)
	addl	%r12d, %edx
	cvtsi2sd	%edx, %xmm1
	movsd	%xmm1, 48(%rax)
	addl	%r12d, %edx
	cvtsi2sd	%edx, %xmm3
	movsd	%xmm3, 56(%rax)
	addl	$8, %ecx
	addl	%r12d, %edx
	addq	$64, %rax
	cmpl	%ecx, %ebp
	jg	.L567
.L561:
	addl	$1, %r12d
	addq	$8, %r13
	cmpl	%r12d, %r14d
	jg	.L568
.L560:
	addq	$1, gcnt(%rip)
	movq	40(%rsp), %r11
	salq	$3, %r11
	movq	%r11, 56(%rsp)
	movq	t(%rip), %r10
	movq	56(%rsp), %rdx
	movabsq	$4607182418800017408, %rsi
	movq	%rsi, (%r10,%rdx)
	testq	%rbx, %rbx
	je	.L569
	testl	%r14d, %r14d
	jle	.L570
	movq	%rdx, %r15
	addq	y(%rip), %r15
	subl	$1, %r14d
	leaq	8(,%r14,8), %r14
	movq	%r14, 24(%rsp)
	xorl	%r10d, %r10d
.L571:
	testl	%ebp, %ebp
	jle	.L575
	movq	(%r15), %rcx
	movq	(%rcx,%r10), %r11
	movq	(%rbx,%r10), %r12
	movq	%r11, %rax
	andl	$15, %eax
	shrq	$3, %rax
	cmpl	%ebp, %eax
	cmova	%ebp, %eax
	mov	%eax, %esi
	testl	%eax, %eax
	je	.L586
	movq	(%r12), %rdx
	movq	%rdx, (%r11)
	cmpl	%ebp, %eax
	je	.L575
	movl	$1, %r9d
.L576:
	movl	%ebp, %r13d
	subl	%eax, %r13d
	movl	%r13d, %r8d
	shrl	%r8d
	movl	%r8d, %r14d
	addl	%r14d, %r14d
	je	.L578
	salq	$3, %rsi
	leaq	(%r12,%rsi), %rdx
	leaq	(%r11,%rsi), %rsi
	leal	-1(%r8), %edi
	andl	$7, %edi
	movsd	(%rdx), %xmm15
	movhpd	8(%rdx), %xmm15
	movapd	%xmm15, (%rsi)
	movl	$1, %ecx
	movl	$16, %eax
	cmpl	%r8d, %ecx
	jae	.L690
	testl	%edi, %edi
	je	.L572
	cmpl	$1, %edi
	je	.L676
	cmpl	$2, %edi
	.p2align 4,,3
	je	.L677
	cmpl	$3, %edi
	.p2align 4,,2
	je	.L678
	cmpl	$4, %edi
	.p2align 4,,2
	je	.L679
	cmpl	$5, %edi
	.p2align 4,,2
	je	.L680
	cmpl	$6, %edi
	.p2align 4,,2
	je	.L681
	movsd	16(%rdx), %xmm4
	movhpd	24(%rdx), %xmm4
	movapd	%xmm4, 16(%rsi)
	movl	$2, %ecx
	movl	$32, %eax
.L681:
	movsd	(%rdx,%rax), %xmm8
	movhpd	8(%rdx,%rax), %xmm8
	movapd	%xmm8, (%rsi,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L680:
	movsd	(%rdx,%rax), %xmm0
	movhpd	8(%rdx,%rax), %xmm0
	movapd	%xmm0, (%rsi,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L679:
	movsd	(%rdx,%rax), %xmm1
	movhpd	8(%rdx,%rax), %xmm1
	movapd	%xmm1, (%rsi,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L678:
	movsd	(%rdx,%rax), %xmm10
	movhpd	8(%rdx,%rax), %xmm10
	movapd	%xmm10, (%rsi,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L677:
	movsd	(%rdx,%rax), %xmm12
	movhpd	8(%rdx,%rax), %xmm12
	movapd	%xmm12, (%rsi,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L676:
	movsd	(%rdx,%rax), %xmm14
	movhpd	8(%rdx,%rax), %xmm14
	movapd	%xmm14, (%rsi,%rax)
	addl	$1, %ecx
	addq	$16, %rax
	cmpl	%r8d, %ecx
	jae	.L690
.L572:
	movsd	(%rdx,%rax), %xmm14
	movhpd	8(%rdx,%rax), %xmm14
	movapd	%xmm14, (%rsi,%rax)
	leaq	16(%rax), %rdi
	movsd	(%rdx,%rdi), %xmm12
	movhpd	8(%rdx,%rdi), %xmm12
	movapd	%xmm12, (%rsi,%rdi)
	leaq	32(%rax), %rdi
	movsd	(%rdx,%rdi), %xmm10
	movhpd	8(%rdx,%rdi), %xmm10
	movapd	%xmm10, (%rsi,%rdi)
	leaq	48(%rax), %rdi
	movsd	(%rdx,%rdi), %xmm1
	movhpd	8(%rdx,%rdi), %xmm1
	movapd	%xmm1, (%rsi,%rdi)
	leaq	64(%rax), %rdi
	movsd	(%rdx,%rdi), %xmm0
	movhpd	8(%rdx,%rdi), %xmm0
	movapd	%xmm0, (%rsi,%rdi)
	leaq	80(%rax), %rdi
	movsd	(%rdx,%rdi), %xmm8
	movhpd	8(%rdx,%rdi), %xmm8
	movapd	%xmm8, (%rsi,%rdi)
	leaq	96(%rax), %rdi
	movsd	(%rdx,%rdi), %xmm4
	movhpd	8(%rdx,%rdi), %xmm4
	movapd	%xmm4, (%rsi,%rdi)
	leaq	112(%rax), %rdi
	movsd	(%rdx,%rdi), %xmm15
	movhpd	8(%rdx,%rdi), %xmm15
	movapd	%xmm15, (%rsi,%rdi)
	addl	$8, %ecx
	subq	$-128, %rax
	cmpl	%r8d, %ecx
	jb	.L572
.L690:
	addl	%r14d, %r9d
	cmpl	%r14d, %r13d
	je	.L575
.L578:
	movslq	%r9d, %rdx
	salq	$3, %rdx
	addq	%rdx, %r12
	addq	%rdx, %r11
	movl	%r9d, %esi
	notl	%esi
	addl	%ebp, %esi
	andl	$7, %esi
	movq	(%r12), %rax
	movq	%rax, (%r11)
	leal	1(%r9), %ecx
	leaq	8(%r12), %rdx
	leaq	8(%r11), %rax
	cmpl	%ecx, %ebp
	jle	.L575
	testl	%esi, %esi
	je	.L574
	cmpl	$1, %esi
	je	.L670
	cmpl	$2, %esi
	.p2align 4,,3
	je	.L671
	cmpl	$3, %esi
	.p2align 4,,2
	je	.L672
	cmpl	$4, %esi
	.p2align 4,,2
	je	.L673
	cmpl	$5, %esi
	.p2align 4,,2
	je	.L674
	cmpl	$6, %esi
	.p2align 4,,2
	je	.L675
	movq	8(%r12), %r8
	movq	%r8, 8(%r11)
	leal	2(%r9), %ecx
	leaq	16(%r12), %rdx
	leaq	16(%r11), %rax
.L675:
	movq	(%rdx), %r9
	movq	%r9, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L674:
	movq	(%rdx), %rsi
	movq	%rsi, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L673:
	movq	(%rdx), %r11
	movq	%r11, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L672:
	movq	(%rdx), %r14
	movq	%r14, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L671:
	movq	(%rdx), %r12
	movq	%r12, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L670:
	movq	(%rdx), %r13
	movq	%r13, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
	cmpl	%ecx, %ebp
	jle	.L575
.L574:
	movq	(%rdx), %rdi
	movq	%rdi, (%rax)
	movq	8(%rdx), %r13
	movq	%r13, 8(%rax)
	movq	16(%rdx), %r12
	movq	%r12, 16(%rax)
	movq	24(%rdx), %r14
	movq	%r14, 24(%rax)
	movq	32(%rdx), %r11
	movq	%r11, 32(%rax)
	movq	40(%rdx), %rsi
	movq	%rsi, 40(%rax)
	movq	48(%rdx), %r8
	movq	%r8, 48(%rax)
	movq	56(%rdx), %r9
	movq	%r9, 56(%rax)
	addl	$8, %ecx
	addq	$64, %rdx
	addq	$64, %rax
	cmpl	%ecx, %ebp
	jg	.L574
.L575:
	addq	$8, %r10
	cmpq	24(%rsp), %r10
	jne	.L571
.L570:
	movq	position(%rip), %r9
	movq	32(%rsp), %r8
	movl	$0, (%r9,%r8)
	movq	ch(%rip), %r15
	movq	40(%rsp), %rax
	movb	$0, (%r15,%rax)
	movl	52(%rsp), %edi
	call	expression
	movq	ch(%rip), %r10
	movq	40(%rsp), %rbp
	movzbl	(%r10,%rbp), %eax
	cmpb	$10, %al
	je	.L699
	cmpb	$59, %al
	je	.L700
	movl	$.LC139, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %rdx
	movq	56(%rsp), %rcx
	movq	(%rdx,%rcx), %rdx
	movl	52(%rsp), %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	$993, %edx
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %r13
	movq	32(%rsp), %rdi
	movl	$1, 0(%r13,%rdi)
.L582:
	movl	$.LC140, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %r15
	movq	56(%rsp), %rax
	movq	(%r15,%rax), %rdx
	movl	52(%rsp), %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	$996, %edx
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %r10
	movq	32(%rsp), %rbp
	movl	$1, (%r10,%rbp)
	xorl	%ebp, %ebp
.L583:
	movq	%rbx, %rdi
	call	free
	movl	%ebp, %eax
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
.L700:
	.cfi_restore_state
	movq	err(%rip), %r14
	movq	32(%rsp), %r12
	movl	(%r14,%r12), %eax
.L580:
	testl	%eax, %eax
	jne	.L582
	movl	$1, %ebp
	jmp	.L583
.L699:
	movq	err(%rip), %rsi
	movq	32(%rsp), %r11
	movl	(%rsi,%r11), %eax
	jmp	.L580
.L569:
	movq	y(%rip), %r13
	movq	0(%r13,%rdx), %r12
	movq	(%r12), %rbp
	movq	$0, 0(%rbp)
	jmp	.L570
.L586:
	xorl	%r9d, %r9d
	jmp	.L576
.L584:
	xorl	%esi, %esi
	jmp	.L562
	.cfi_endproc
.LFE19:
	.size	fpar_ok, .-fpar_ok
.globl fpar_f
	.type	fpar_f, @function
fpar_f:
.LFB30:
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
	movl	%edi, 36(%rsp)
	addq	$1, gcnt(%rip)
	movslq	%edi, %rbx
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	leaq	0(,%rbx,8), %rdx
	movq	%rdx, 40(%rsp)
	movq	t(%rip), %rax
	movq	40(%rsp), %rdx
	movsd	%xmm0, (%rax,%rdx)
	testq	%rsi, %rsi
	je	.L783
	leaq	0(,%rbx,4), %r11
	movq	%r11, 24(%rsp)
	movq	nvar(%rip), %r8
	movq	24(%rsp), %r10
	movl	(%r8,%r10), %r8d
	movq	ranks(%rip), %r9
	movl	(%r9,%r10), %eax
	testl	%eax, %eax
	jle	.L703
	movq	40(%rsp), %r15
	addq	y(%rip), %r15
	subl	$1, %eax
	leaq	8(,%rax,8), %r14
	xorl	%r9d, %r9d
	movl	%r8d, %r12d
	shrl	%r12d
	leal	(%r12,%r12), %r13d
.L704:
	testl	%r8d, %r8d
	jle	.L708
	movq	(%r15), %rdx
	movq	(%rdx,%r9), %rdx
	movq	(%rsi,%r9), %r11
	testl	%r13d, %r13d
	je	.L715
	cmpl	$19, %r8d
	seta	%cl
	leaq	16(%r11), %rdi
	cmpq	%rdi, %rdx
	seta	%al
	leaq	16(%rdx), %r10
	cmpq	%r10, %r11
	seta	%bpl
	orb	%bpl, %al
	testb	%al, %cl
	je	.L715
	leal	-1(%r12), %edi
	andl	$7, %edi
	movsd	(%r11), %xmm1
	movhpd	8(%r11), %xmm1
	movlpd	%xmm1, (%rdx)
	movhpd	%xmm1, 8(%rdx)
	movl	$1, %ecx
	movl	$16, %eax
	cmpl	$1, %r12d
	jbe	.L776
	testl	%edi, %edi
	je	.L705
	cmpl	$1, %edi
	je	.L770
	cmpl	$2, %edi
	.p2align 4,,3
	je	.L771
	cmpl	$3, %edi
	.p2align 4,,2
	je	.L772
	cmpl	$4, %edi
	.p2align 4,,2
	je	.L773
	cmpl	$5, %edi
	.p2align 4,,2
	je	.L774
	cmpl	$6, %edi
	.p2align 4,,2
	je	.L775
	movsd	16(%r11), %xmm3
	movhpd	24(%r11), %xmm3
	movlpd	%xmm3, 16(%rdx)
	movhpd	%xmm3, 24(%rdx)
	movl	$2, %ecx
	movl	$32, %eax
.L775:
	movsd	(%r11,%rax), %xmm5
	movhpd	8(%r11,%rax), %xmm5
	movlpd	%xmm5, (%rdx,%rax)
	movhpd	%xmm5, 8(%rdx,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L774:
	movsd	(%r11,%rax), %xmm7
	movhpd	8(%r11,%rax), %xmm7
	movlpd	%xmm7, (%rdx,%rax)
	movhpd	%xmm7, 8(%rdx,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L773:
	movsd	(%r11,%rax), %xmm9
	movhpd	8(%r11,%rax), %xmm9
	movlpd	%xmm9, (%rdx,%rax)
	movhpd	%xmm9, 8(%rdx,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L772:
	movsd	(%r11,%rax), %xmm11
	movhpd	8(%r11,%rax), %xmm11
	movlpd	%xmm11, (%rdx,%rax)
	movhpd	%xmm11, 8(%rdx,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L771:
	movsd	(%r11,%rax), %xmm13
	movhpd	8(%r11,%rax), %xmm13
	movlpd	%xmm13, (%rdx,%rax)
	movhpd	%xmm13, 8(%rdx,%rax)
	addl	$1, %ecx
	addq	$16, %rax
.L770:
	movsd	(%r11,%rax), %xmm15
	movhpd	8(%r11,%rax), %xmm15
	movlpd	%xmm15, (%rdx,%rax)
	movhpd	%xmm15, 8(%rdx,%rax)
	addl	$1, %ecx
	addq	$16, %rax
	cmpl	%ecx, %r12d
	jbe	.L776
.L705:
	movsd	(%r11,%rax), %xmm15
	movhpd	8(%r11,%rax), %xmm15
	movlpd	%xmm15, (%rdx,%rax)
	movhpd	%xmm15, 8(%rdx,%rax)
	leaq	16(%rax), %rdi
	movsd	(%r11,%rdi), %xmm13
	movhpd	8(%r11,%rdi), %xmm13
	movlpd	%xmm13, (%rdx,%rdi)
	movhpd	%xmm13, 8(%rdx,%rdi)
	leaq	32(%rax), %r10
	movsd	(%r11,%r10), %xmm11
	movhpd	8(%r11,%r10), %xmm11
	movlpd	%xmm11, (%rdx,%r10)
	movhpd	%xmm11, 8(%rdx,%r10)
	leaq	48(%rax), %rbp
	movsd	(%r11,%rbp), %xmm9
	movhpd	8(%r11,%rbp), %xmm9
	movlpd	%xmm9, (%rdx,%rbp)
	movhpd	%xmm9, 8(%rdx,%rbp)
	leaq	64(%rax), %rdi
	movsd	(%r11,%rdi), %xmm7
	movhpd	8(%r11,%rdi), %xmm7
	movlpd	%xmm7, (%rdx,%rdi)
	movhpd	%xmm7, 8(%rdx,%rdi)
	leaq	80(%rax), %r10
	movsd	(%r11,%r10), %xmm5
	movhpd	8(%r11,%r10), %xmm5
	movlpd	%xmm5, (%rdx,%r10)
	movhpd	%xmm5, 8(%rdx,%r10)
	leaq	96(%rax), %rbp
	movsd	(%r11,%rbp), %xmm3
	movhpd	8(%r11,%rbp), %xmm3
	movlpd	%xmm3, (%rdx,%rbp)
	movhpd	%xmm3, 8(%rdx,%rbp)
	leaq	112(%rax), %rdi
	movsd	(%r11,%rdi), %xmm1
	movhpd	8(%r11,%rdi), %xmm1
	movlpd	%xmm1, (%rdx,%rdi)
	movhpd	%xmm1, 8(%rdx,%rdi)
	addl	$8, %ecx
	subq	$-128, %rax
	cmpl	%ecx, %r12d
	ja	.L705
.L776:
	movl	%r13d, %ebp
	cmpl	%r13d, %r8d
	je	.L708
.L709:
	movslq	%ebp, %rcx
	salq	$3, %rcx
	addq	%rcx, %r11
	leaq	(%rdx,%rcx), %r10
	movl	%ebp, %edi
	notl	%edi
	addl	%r8d, %edi
	andl	$7, %edi
	movq	(%r11), %rax
	movq	%rax, (%r10)
	leal	1(%rbp), %ecx
	leaq	8(%r11), %rdx
	leaq	8(%r10), %rax
	cmpl	%ecx, %r8d
	jle	.L708
	testl	%edi, %edi
	je	.L707
	cmpl	$1, %edi
	je	.L764
	cmpl	$2, %edi
	.p2align 4,,3
	je	.L765
	cmpl	$3, %edi
	.p2align 4,,2
	je	.L766
	cmpl	$4, %edi
	.p2align 4,,2
	je	.L767
	cmpl	$5, %edi
	.p2align 4,,2
	je	.L768
	cmpl	$6, %edi
	.p2align 4,,2
	je	.L769
	movq	8(%r11), %rdi
	movq	%rdi, 8(%r10)
	leal	2(%rbp), %ecx
	leaq	16(%r11), %rdx
	leaq	16(%r10), %rax
.L769:
	movq	(%rdx), %r11
	movq	%r11, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L768:
	movq	(%rdx), %rbp
	movq	%rbp, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L767:
	movq	(%rdx), %r10
	movq	%r10, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L766:
	movq	(%rdx), %rdi
	movq	%rdi, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L765:
	movq	(%rdx), %r11
	movq	%r11, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
.L764:
	movq	(%rdx), %rbp
	movq	%rbp, (%rax)
	addl	$1, %ecx
	addq	$8, %rdx
	addq	$8, %rax
	cmpl	%ecx, %r8d
	jle	.L708
.L707:
	movq	(%rdx), %rdi
	movq	%rdi, (%rax)
	movq	8(%rdx), %r10
	movq	%r10, 8(%rax)
	movq	16(%rdx), %rbp
	movq	%rbp, 16(%rax)
	movq	24(%rdx), %r11
	movq	%r11, 24(%rax)
	movq	32(%rdx), %rdi
	movq	%rdi, 32(%rax)
	movq	40(%rdx), %r10
	movq	%r10, 40(%rax)
	movq	48(%rdx), %rbp
	movq	%rbp, 48(%rax)
	movq	56(%rdx), %r11
	movq	%r11, 56(%rax)
	addl	$8, %ecx
	addq	$64, %rdx
	addq	$64, %rax
	cmpl	%ecx, %r8d
	jg	.L707
.L708:
	addq	$8, %r9
	cmpq	%r14, %r9
	jne	.L704
.L703:
	movq	position(%rip), %r14
	movq	24(%rsp), %r15
	movl	$0, (%r14,%r15)
	movq	ch(%rip), %r13
	movb	$0, 0(%r13,%rbx)
	movl	36(%rsp), %edi
	call	expression
	movq	ch(%rip), %r12
	movzbl	(%r12,%rbx), %eax
	cmpb	$10, %al
	je	.L782
	cmpb	$59, %al
	je	.L782
	movl	$.LC139, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	movsd	%xmm0, (%rsp)
	call	printf
	movq	buffer(%rip), %r9
	movq	40(%rsp), %r8
	movq	(%r9,%r8), %rdx
	movl	36(%rsp), %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	$993, %edx
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rbx
	movq	24(%rsp), %rsi
	movl	$1, (%rbx,%rsi)
	movsd	(%rsp), %xmm0
.L713:
	movl	$.LC140, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	movsd	%xmm0, (%rsp)
	call	printf
	movq	buffer(%rip), %rbp
	movq	40(%rsp), %r10
	movq	0(%rbp,%r10), %rdx
	movl	36(%rsp), %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	call	printf
	movl	$996, %edx
	movl	$.LC2, %esi
	movl	$.LC3, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rax
	movq	24(%rsp), %rcx
	movl	$1, (%rax,%rcx)
	movsd	(%rsp), %xmm0
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
.L782:
	.cfi_restore_state
	movq	err(%rip), %rax
	movq	24(%rsp), %rdx
	movl	(%rax,%rdx), %r11d
	testl	%r11d, %r11d
	jne	.L713
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
.L783:
	.cfi_restore_state
	movq	y(%rip), %rbp
	movq	0(%rbp,%rdx), %rdi
	movq	(%rdi), %rsi
	movq	$0, (%rsi)
	leaq	0(,%rbx,4), %rcx
	movq	%rcx, 24(%rsp)
	jmp	.L703
.L715:
	xorl	%ebp, %ebp
	jmp	.L709
	.cfi_endproc
.LFE30:
	.size	fpar_f, .-fpar_f
	.comm	buffer,8,16
	.comm	position,8,16
	.comm	ch,8,16
	.comm	maxpos,8,16
	.comm	y,8,16
	.comm	t,8,16
	.comm	err,8,16
	.comm	nvar,8,16
	.comm	ranks,8,16
	.comm	N,4,16
	.local	gcnt
	.comm	gcnt,8,16
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC4:
	.long	1
	.long	1
	.long	1
	.long	1
	.align 16
.LC11:
	.long	0
	.long	-2147483648
	.long	0
	.long	0
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC71:
	.long	0
	.long	-1074790400
	.section	.rodata.cst16
	.align 16
.LC74:
	.long	4294967295
	.long	2147483647
	.long	0
	.long	0
	.section	.rodata.cst8
	.align 8
.LC79:
	.long	0
	.long	1138753536
	.section	.rodata.cst16
	.align 16
.LC138:
	.long	4
	.long	4
	.long	4
	.long	4
	.ident	"GCC: (NetBSD nb2 20110806) 4.5.3"
