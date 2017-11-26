	.file	"fpar.c"
	.text
	.p2align 4,,15
	.globl	fpar_init
	.type	fpar_init, @function
fpar_init:
.LFB35:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	testl	%edi, %edi
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movl	%edi, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	js	.L4
	movslq	%edi, %r14
	leaq	0(,%r14,8), %r13
	movq	%r13, %rdi
	call	malloc
	testq	%rax, %rax
	movq	%rax, %rbx
	movq	%rax, buffer(%rip)
	je	.L4
	leaq	0(,%r14,4), %r12
	movq	%r12, %rdi
	call	malloc
	movq	%r14, %rdi
	movq	%rax, position(%rip)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, ch(%rip)
	call	malloc
	movq	%r13, %rdi
	movq	%rax, maxpos(%rip)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, y(%rip)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, err(%rip)
	call	malloc
	movq	%r12, %rdi
	movq	%rax, nvar(%rip)
	call	malloc
	movq	%r13, %rdi
	movq	%rax, ranks(%rip)
	call	malloc
	xorl	%edx, %edx
	xorl	%ecx, %ecx
	testl	%ebp, %ebp
	movq	%rax, t(%rip)
	movl	%ebp, N(%rip)
	jne	.L10
	jmp	.L6
	.p2align 4,,10
.L14:
	movq	buffer(%rip), %rbx
.L10:
	movq	$0, (%rbx,%rdx,2)
	movq	position(%rip), %rax
	addl	$1, %ecx
	movq	err(%rip), %rbx
	movq	nvar(%rip), %rsi
	movq	ranks(%rip), %rdi
	movq	y(%rip), %r8
	movl	$0, (%rax,%rdx)
	movl	$0, (%rbx,%rdx)
	movl	$1, (%rsi,%rdx)
	movl	$1, (%rdi,%rdx)
	movq	$0, (%r8,%rdx,2)
	addq	$4, %rdx
	cmpl	%ecx, N(%rip)
	jg	.L14
.L6:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	movq	$0, gcnt(%rip)
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L4:
	.cfi_restore_state
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE35:
	.size	fpar_init, .-fpar_init
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Parser Exception: %s\n\n"
.LC1:
	.string	"Buffer[%d] = '%s'\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"Occured at: File: %s, Line: %d\n"
	.text
	.p2align 4,,15
	.globl	exception
	.type	exception, @function
exception:
.LFB36:
	.cfi_startproc
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	xorl	%eax, %eax
	movq	%r12, -16(%rsp)
	movq	%r13, -8(%rsp)
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	movq	%rdi, %r12
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	%esi, %r13d
	movl	$.LC0, %edi
	movq	%rcx, %rsi
	movl	%edx, %ebx
	call	printf
	movq	buffer(%rip), %rax
	movslq	%ebx, %rbp
	movl	%ebx, %esi
	movl	$.LC1, %edi
	movq	(%rax,%rbp,8), %rdx
	xorl	%eax, %eax
	call	printf
	movl	%r13d, %edx
	movq	%r12, %rsi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rdx
	movq	8(%rsp), %rbx
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	movl	$1, (%rdx,%rbp,4)
	movq	16(%rsp), %rbp
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE36:
	.size	exception, .-exception
	.section	.rodata.str1.8
	.align 8
.LC3:
	.ascii	"supported functions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt"
	.ascii	",abs\nsupported functions: asin,acos,asinh"
	.string	",acosh,sinh,cosh,cbrt,ceil,sgn\nsupported functions: tanh,atan,atanh,tgh,atg,atgh,neg\nsupported bin-functions: max,min,mina,minb,maxa,maxb,and,or,xor,pow,mod\nsupported tri-functions: if(cond,cond>0,cond<=0)\nsupported rgb-functions: gr,gg,gb,sr,sg,sb,ggs,sgs"
	.align 8
.LC4:
	.string	"supported operators: +,-,*,/,^, unary -\nsupported user defined function: asmf[123](lx), udf1(x), udf2(x,y), udf3(x,y,z)\nand more...(see udf.c), special \"doing nothing\" none"
	.section	.rodata.str1.1
.LC5:
	.string	"blank characters are skipped"
	.text
	.p2align 4,,15
	.globl	fpar_info
	.type	fpar_info, @function
fpar_info:
.LFB37:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$.LC3, %edi
	call	puts
	movl	$.LC4, %edi
	call	puts
	movl	$.LC5, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	puts
	.cfi_endproc
.LFE37:
	.size	fpar_info, .-fpar_info
	.p2align 4,,15
	.globl	fpar_function
	.type	fpar_function, @function
fpar_function:
.LFB39:
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
	movq	%rcx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	testq	%rcx, %rcx
	je	.L23
	movslq	%edi, %rbx
	movq	buffer(%rip), %r13
	movl	%esi, %r15d
	leaq	0(,%rbx,8), %rbp
	movl	%edx, %r14d
	addq	%rbp, %r13
	movq	0(%r13), %rdi
	testq	%rdi, %rdi
	je	.L22
	call	free
	movq	buffer(%rip), %rax
	movq	$0, (%rax,%rbx,8)
	movq	buffer(%rip), %r13
	addq	%rbp, %r13
.L22:
	movq	%r12, %rdi
	call	strlen
	leaq	2(%rax), %rdi
	call	malloc
	movq	%rax, 0(%r13)
	movq	buffer(%rip), %rdx
	movq	(%rdx,%rbx,8), %rdi
	testq	%rdi, %rdi
	je	.L23
	movq	%r12, %rsi
	call	strcpy
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbx,8), %r12
	movq	%r12, %rdi
	call	strlen
	movb	$10, (%r12,%rax)
	movq	buffer(%rip), %rsi
	leal	1(%r14), %r12d
	movq	(%rsi,%rbx,8), %r13
	movq	%r13, %rdi
	call	strlen
	movb	$0, 1(%r13,%rax)
	movq	buffer(%rip), %rdi
	movq	(%rdi,%rbx,8), %rdi
	call	strlen
	movq	maxpos(%rip), %r8
	movq	nvar(%rip), %r9
	movslq	%r12d, %rdi
	movq	ranks(%rip), %r10
	salq	$3, %rdi
	movl	%eax, (%r8,%rbx,4)
	movl	%r15d, (%r9,%rbx,4)
	movl	%r12d, (%r10,%rbx,4)
	movq	y(%rip), %rbx
	call	malloc
	addq	%rbp, %rbx
	testl	%r12d, %r12d
	movq	%rax, (%rbx)
	jle	.L25
	movq	y(%rip), %r11
	movslq	%r15d, %r12
	movl	%r14d, %r14d
	salq	$3, %r12
	leaq	8(,%r14,8), %r15
	movl	$8, %ebx
	movq	%r12, %rdi
	andl	$7, %r14d
	movq	(%r11,%rbp), %r13
	call	malloc
	cmpq	%r15, %rbx
	movq	%rax, 0(%r13)
	je	.L25
	testq	%r14, %r14
	je	.L26
	cmpq	$1, %r14
	je	.L52
	cmpq	$2, %r14
	je	.L53
	cmpq	$3, %r14
	je	.L54
	cmpq	$4, %r14
	je	.L55
	cmpq	$5, %r14
	je	.L56
	cmpq	$6, %r14
	je	.L57
	movq	y(%rip), %rax
	movq	%r12, %rdi
	movq	(%rax,%rbp), %rbx
	call	malloc
	movq	%rax, 8(%rbx)
	movl	$16, %ebx
.L57:
	movq	y(%rip), %rdx
	movq	%r12, %rdi
	movq	(%rdx,%rbp), %r14
	call	malloc
	addq	%rbx, %r14
	addq	$8, %rbx
	movq	%rax, (%r14)
.L56:
	movq	y(%rip), %rcx
	movq	%r12, %rdi
	movq	(%rcx,%rbp), %r13
	call	malloc
	addq	%rbx, %r13
	addq	$8, %rbx
	movq	%rax, 0(%r13)
.L55:
	movq	y(%rip), %rsi
	movq	%r12, %rdi
	movq	(%rsi,%rbp), %r14
	call	malloc
	addq	%rbx, %r14
	addq	$8, %rbx
	movq	%rax, (%r14)
.L54:
	movq	y(%rip), %rdi
	movq	(%rdi,%rbp), %r13
	movq	%r12, %rdi
	call	malloc
	addq	%rbx, %r13
	addq	$8, %rbx
	movq	%rax, 0(%r13)
.L53:
	movq	y(%rip), %r8
	movq	%r12, %rdi
	movq	(%r8,%rbp), %r14
	call	malloc
	addq	%rbx, %r14
	addq	$8, %rbx
	movq	%rax, (%r14)
.L52:
	movq	y(%rip), %r9
	movq	%r12, %rdi
	movq	(%r9,%rbp), %r13
	call	malloc
	addq	%rbx, %r13
	addq	$8, %rbx
	cmpq	%r15, %rbx
	movq	%rax, 0(%r13)
	je	.L25
.L26:
	movq	y(%rip), %r10
	movq	%r12, %rdi
	movq	(%r10,%rbp), %r14
	call	malloc
	movq	%r12, %rdi
	addq	%rbx, %r14
	movq	%rax, (%r14)
	movq	y(%rip), %r11
	movq	(%r11,%rbp), %rax
	leaq	8(%rax,%rbx), %r13
	call	malloc
	movq	%rax, 0(%r13)
	movq	y(%rip), %rdx
	movq	%r12, %rdi
	movq	(%rdx,%rbp), %rcx
	leaq	16(%rcx,%rbx), %r14
	call	malloc
	movq	%rax, (%r14)
	movq	y(%rip), %rsi
	movq	(%rsi,%rbp), %rdi
	leaq	24(%rdi,%rbx), %r13
	movq	%r12, %rdi
	call	malloc
	movq	%rax, 0(%r13)
	movq	y(%rip), %r8
	movq	%r12, %rdi
	movq	(%r8,%rbp), %r9
	leaq	32(%r9,%rbx), %r14
	call	malloc
	movq	%rax, (%r14)
	movq	y(%rip), %r10
	movq	%r12, %rdi
	movq	(%r10,%rbp), %r11
	leaq	40(%r11,%rbx), %r13
	call	malloc
	movq	%rax, 0(%r13)
	movq	y(%rip), %rax
	movq	%r12, %rdi
	movq	(%rax,%rbp), %rdx
	leaq	48(%rdx,%rbx), %r14
	call	malloc
	movq	%rax, (%r14)
	movq	y(%rip), %rcx
	movq	%r12, %rdi
	movq	(%rcx,%rbp), %rsi
	leaq	56(%rsi,%rbx), %r13
	addq	$64, %rbx
	call	malloc
	cmpq	%r15, %rbx
	movq	%rax, 0(%r13)
	jne	.L26
.L25:
	addq	$8, %rsp
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
.L23:
	.cfi_restore_state
	addq	$8, %rsp
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
.LFE39:
	.size	fpar_function, .-fpar_function
	.p2align 4,,15
	.globl	fpar_free
	.type	fpar_free, @function
fpar_free:
.LFB40:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movq	buffer(%rip), %rax
	movslq	%edi, %r14
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	leaq	0(,%r14,8), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	(%rax,%r14,8), %rdi
	testq	%rdi, %rdi
	je	.L70
	call	free
	movq	buffer(%rip), %rdx
	movq	$0, (%rdx,%r14,8)
.L70:
	movq	ranks(%rip), %r11
	movq	y(%rip), %rsi
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	leaq	0(,%r14,4), %r13
	movl	(%r11,%r14,4), %ecx
	movq	(%rsi,%r14,8), %r10
	testl	%ecx, %ecx
	jle	.L72
	.p2align 4,,10
.L74:
	movq	(%r10,%rbx), %rdi
	testq	%rdi, %rdi
	je	.L73
	call	free
	movq	y(%rip), %rdi
	movq	(%rdi,%r12), %r8
	movq	$0, (%r8,%rbx)
	movq	y(%rip), %r9
	movq	ranks(%rip), %r11
	movq	(%r9,%r12), %r10
.L73:
	addl	$1, %ebp
	addq	$8, %rbx
	cmpl	%ebp, (%r11,%r13)
	jg	.L74
.L72:
	testq	%r10, %r10
	je	.L69
	movq	%r10, %rdi
	call	free
	movq	y(%rip), %r12
	movq	$0, (%r12,%r14,8)
.L69:
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	fpar_free, .-fpar_free
	.section	.rodata.str1.1
.LC6:
	.string	"Internal parser calls: %lld\n"
	.text
	.p2align 4,,15
	.globl	fpar_deinit
	.type	fpar_deinit, @function
fpar_deinit:
.LFB41:
	.cfi_startproc
	movl	N(%rip), %esi
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	xorl	%ebx, %ebx
	testl	%esi, %esi
	jle	.L88
	.p2align 4,,10
.L90:
	movl	%ebx, %edi
	addl	$1, %ebx
	call	fpar_free
	cmpl	%ebx, N(%rip)
	jg	.L90
.L88:
	movq	buffer(%rip), %rdi
	call	free
	movq	position(%rip), %rdi
	movq	$0, buffer(%rip)
	call	free
	movq	ch(%rip), %rdi
	movq	$0, position(%rip)
	call	free
	movq	maxpos(%rip), %rdi
	movq	$0, ch(%rip)
	call	free
	movq	t(%rip), %rdi
	movq	$0, maxpos(%rip)
	call	free
	movq	y(%rip), %rdi
	movq	$0, t(%rip)
	call	free
	movq	err(%rip), %rdi
	movq	$0, y(%rip)
	call	free
	movq	nvar(%rip), %rdi
	movq	$0, err(%rip)
	call	free
	movq	ranks(%rip), %rdi
	movq	$0, nvar(%rip)
	call	free
	movq	gcnt(%rip), %rsi
	movl	$.LC6, %edi
	xorl	%eax, %eax
	movq	$0, ranks(%rip)
	call	printf
	movq	$0, gcnt(%rip)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE41:
	.size	fpar_deinit, .-fpar_deinit
	.p2align 4,,15
	.globl	read_next_char
	.type	read_next_char, @function
read_next_char:
.LFB42:
	.cfi_startproc
	movq	position(%rip), %rax
	movslq	%edi, %rdi
	movq	maxpos(%rip), %rcx
	leaq	(%rax,%rdi,4), %rdx
	movslq	(%rdx), %r8
	cmpl	(%rcx,%rdi,4), %r8d
	jge	.L93
	movq	ch(%rip), %r9
	addq	%rdi, %r9
	cmpb	$10, (%r9)
	je	.L93
	movq	buffer(%rip), %rsi
	leal	1(%r8), %r11d
	movq	(%rsi,%rdi,8), %r10
	movl	%r11d, (%rdx)
	movzbl	(%r10,%r8), %edi
	movb	%dil, (%r9)
.L93:
	rep; ret
	.cfi_endproc
.LFE42:
	.size	read_next_char, .-read_next_char
	.p2align 4,,15
	.globl	skipblanks
	.type	skipblanks, @function
skipblanks:
.LFB43:
	.cfi_startproc
	movq	ch(%rip), %rax
	movslq	%edi, %rdi
	movzbl	(%rax,%rdi), %edx
	cmpb	$10, %dl
	je	.L113
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	_ThreadRuneLocale@gottpoff(%rip), %r11
	leaq	0(,%rdi,4), %r8
	leaq	0(,%rdi,8), %r10
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%fs:0, %rbx
	.p2align 4,,10
.L97:
	movq	position(%rip), %rsi
	movq	maxpos(%rip), %r12
	movsbq	%dl, %rcx
	addq	%rdi, %rax
	movl	__mb_sb_limit(%rip), %ebp
	movq	(%rbx,%r11), %r9
	movq	_CurrentRuneLocale(%rip), %r13
	addq	%r8, %rsi
	addq	%r8, %r12
	testl	%ecx, %ecx
	js	.L95
	cmpb	$10, %dl
	je	.L95
	testq	%r9, %r9
	je	.L114
	cmpl	%ecx, %ebp
	.p2align 4,,3
	jg	.L115
.L95:
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_restore 13
	.cfi_def_cfa_offset 8
.L113:
	rep; ret
	.p2align 4,,10
.L115:
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	.cfi_offset 6, -32
	.cfi_offset 12, -24
	.cfi_offset 13, -16
	testb	$64, 65(%r9,%rcx,8)
	je	.L95
	movl	(%rsi), %ecx
.L105:
	cmpl	(%r12), %ecx
	movslq	%ecx, %rdx
	jge	.L105
.L102:
	movq	buffer(%rip), %r9
	leal	1(%rdx), %r12d
	movq	(%r9,%r10), %r13
	movl	%r12d, (%rsi)
	movzbl	0(%r13,%rdx), %esi
	movb	%sil, (%rax)
	movq	ch(%rip), %rax
	movzbl	(%rax,%rdi), %edx
	jmp	.L97
	.p2align 4,,10
.L114:
	testq	%r13, %r13
	je	.L116
	cmpl	%ecx, %ebp
	jle	.L95
.L107:
	testb	$64, 65(%r13,%rcx,8)
	je	.L95
	movslq	(%rsi), %rdx
	cmpl	(%r12), %edx
	jl	.L102
	jmp	.L107
	.p2align 4,,10
.L116:
	cmpl	%ecx, %ebp
	jle	.L95
	movq	_DefaultRuneLocale+64(,%rcx,8), %rbp
	andl	$16384, %ebp
	.p2align 4,,10
.L106:
	testl	%ebp, %ebp
	je	.L95
	movslq	(%rsi), %rdx
	cmpl	(%r12), %edx
	jl	.L102
	jmp	.L106
	.cfi_endproc
.LFE43:
	.size	skipblanks, .-skipblanks
	.section	.rodata.str1.8
	.align 8
.LC7:
	.string	"Expected: function name or variable.\n"
	.section	.rodata.str1.1
.LC8:
	.string	"fpar.c"
	.text
	.p2align 4,,15
	.globl	read_id
	.type	read_id, @function
read_id:
.LFB44:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movl	%edi, %r14d
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movslq	%r14d, %rbx
	call	skipblanks
	movq	ch(%rip), %r8
	movzbl	(%r8,%rbx), %edx
	movsbq	%dl, %rax
	testl	%eax, %eax
	js	.L118
	cmpl	__mb_sb_limit(%rip), %eax
	jge	.L118
	movq	%fs:0, %r13
	movq	_ThreadRuneLocale@gottpoff(%rip), %r12
	movq	0(%r13,%r12), %rcx
	testq	%rcx, %rcx
	je	.L142
.L119:
	movslq	%eax, %rdi
	testb	$1, 65(%rcx,%rdi,8)
	je	.L118
	leaq	0(,%rbx,4), %r9
	leaq	0(,%rbx,8), %rsi
	xorl	%ecx, %ecx
	movl	$_DefaultRuneLocale, %r14d
	jmp	.L120
	.p2align 4,,10
.L144:
	movq	0(%r13,%r12), %r11
	testq	%r11, %r11
	je	.L143
.L125:
	movslq	%eax, %r10
	testb	$1, 65(%r11,%r10,8)
	je	.L124
.L126:
	cmpl	$30, %ecx
	leaq	(%r8,%rbx), %r11
	jg	.L122
	movslq	%ecx, %rax
	addl	$1, %ecx
	movb	%dl, 0(%rbp,%rax)
	movq	ch(%rip), %r8
	leaq	(%r8,%rbx), %r11
	movzbl	(%r11), %edx
.L122:
	movq	position(%rip), %rdi
	movq	maxpos(%rip), %r10
	addq	%r9, %rdi
	movslq	(%rdi), %rax
	cmpl	(%r10,%r9), %eax
	jge	.L123
	cmpb	$10, %dl
	je	.L123
	movq	buffer(%rip), %rdx
	leal	1(%rax), %r8d
	movq	(%rdx,%rsi), %r10
	movl	%r8d, (%rdi)
	movzbl	(%r10,%rax), %edi
	movb	%dil, (%r11)
	movq	ch(%rip), %r8
	movzbl	(%r8,%rbx), %edx
.L123:
	movsbq	%dl, %rax
.L120:
	testl	%eax, %eax
	js	.L124
	cmpl	__mb_sb_limit(%rip), %eax
	jl	.L144
.L124:
	cmpl	$127, %eax
	ja	.L127
	testb	$4, _DefaultRuneLocale+65(,%rax,8)
	jne	.L126
.L127:
	cmpb	$39, %dl
	je	.L126
	cmpb	$95, %dl
	je	.L126
	movslq	%ecx, %rdi
	movb	$0, 0(%rbp,%rdi)
.L128:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	addq	$1, gcnt(%rip)
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L118:
	.cfi_restore_state
	movl	$.LC7, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %rax
	movl	%r14d, %esi
	movl	$.LC1, %edi
	movq	(%rax,%rbx,8), %rdx
	xorl	%eax, %eax
	call	printf
	movl	$174, %edx
	movl	$.LC8, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rdx
	movl	$1, (%rdx,%rbx,4)
	jmp	.L128
	.p2align 4,,10
.L143:
	movq	_CurrentRuneLocale(%rip), %r11
	testq	%r11, %r11
	cmove	%r14, %r11
	jmp	.L125
.L142:
	movq	_CurrentRuneLocale(%rip), %rsi
	movl	$_DefaultRuneLocale, %ecx
	testq	%rsi, %rsi
	cmovne	%rsi, %rcx
	jmp	.L119
	.cfi_endproc
.LFE44:
	.size	read_id, .-read_id
	.p2align 4,,15
	.globl	exponential
	.type	exponential, @function
exponential:
.LFB48:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movl	%edi, %ebx
	movslq	%ebx, %rbp
	subq	$88, %rsp
	.cfi_def_cfa_offset 112
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %rax
	movsd	%xmm0, 72(%rsp)
	cmpb	$94, (%rax,%rbp)
	jne	.L156
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %rdx
	movsd	%xmm0, 64(%rsp)
	cmpb	$94, (%rdx,%rbp)
	jne	.L147
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %rcx
	movsd	%xmm0, 56(%rsp)
	cmpb	$94, (%rcx,%rbp)
	jne	.L148
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %rsi
	movsd	%xmm0, 24(%rsp)
	cmpb	$94, (%rsi,%rbp)
	jne	.L149
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %rdi
	movsd	%xmm0, 48(%rsp)
	cmpb	$94, (%rdi,%rbp)
	jne	.L150
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %r8
	movsd	%xmm0, 32(%rsp)
	cmpb	$94, (%r8,%rbp)
	jne	.L151
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %r9
	movsd	%xmm0, 40(%rsp)
	cmpb	$94, (%r9,%rbp)
	jne	.L152
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %r10
	movsd	%xmm0, 16(%rsp)
	cmpb	$94, (%r10,%rbp)
	jne	.L153
	movl	%ebx, %edi
	addq	$1, gcnt(%rip)
	call	factor
	movq	ch(%rip), %r11
	cmpb	$94, (%r11,%rbp)
	jne	.L154
	.p2align 4,,10
.L155:
	movl	%ebx, %edi
	movsd	%xmm0, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm2
	movapd	%xmm0, %xmm1
	movapd	%xmm2, %xmm0
	call	pow
	movq	ch(%rip), %rax
	cmpb	$94, (%rax,%rbp)
	je	.L155
.L154:
	movapd	%xmm0, %xmm1
	movsd	16(%rsp), %xmm0
	call	pow
	movsd	%xmm0, 16(%rsp)
.L153:
	movsd	40(%rsp), %xmm0
	movsd	16(%rsp), %xmm1
	call	pow
	movsd	%xmm0, 40(%rsp)
.L152:
	movsd	32(%rsp), %xmm0
	movsd	40(%rsp), %xmm1
	call	pow
	movsd	%xmm0, 32(%rsp)
.L151:
	movsd	48(%rsp), %xmm0
	movsd	32(%rsp), %xmm1
	call	pow
	movsd	%xmm0, 48(%rsp)
.L150:
	movsd	24(%rsp), %xmm0
	movsd	48(%rsp), %xmm1
	call	pow
	movsd	%xmm0, 24(%rsp)
.L149:
	movsd	56(%rsp), %xmm0
	movsd	24(%rsp), %xmm1
	call	pow
	movsd	%xmm0, 56(%rsp)
.L148:
	movsd	64(%rsp), %xmm0
	movsd	56(%rsp), %xmm1
	call	pow
	movsd	%xmm0, 64(%rsp)
.L147:
	movsd	64(%rsp), %xmm1
	movsd	72(%rsp), %xmm0
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	pow
.L156:
	.cfi_restore_state
	addq	$88, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE48:
	.size	exponential, .-exponential
	.p2align 4,,15
	.globl	term
	.type	term, @function
term:
.LFB46:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	xorl	%eax, %eax
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movl	%edi, %ebx
	movslq	%ebx, %rbp
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	addq	$1, gcnt(%rip)
	call	exponential
	movapd	%xmm0, %xmm1
.L165:
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbp), %edx
	cmpb	$42, %dl
	je	.L162
	cmpb	$47, %dl
	je	.L167
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movapd	%xmm1, %xmm0
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L162:
	.cfi_restore_state
	movl	%ebx, %edi
	xorl	%eax, %eax
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	mulsd	%xmm0, %xmm1
	jmp	.L165
	.p2align 4,,10
.L167:
	movl	%ebx, %edi
	xorl	%eax, %eax
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	divsd	%xmm0, %xmm1
	jmp	.L165
	.cfi_endproc
.LFE46:
	.size	term, .-term
	.p2align 4,,15
	.globl	expression
	.type	expression, @function
expression:
.LFB47:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	xorl	%eax, %eax
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movl	%edi, %ebx
	movslq	%ebx, %rbp
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	addq	$2, gcnt(%rip)
	call	exponential
	movsd	%xmm0, 24(%rsp)
.L190:
	movq	ch(%rip), %rax
	movzbl	(%rax,%rbp), %ecx
	cmpb	$42, %cl
	je	.L172
	cmpb	$47, %cl
	je	.L197
.L189:
	cmpb	$43, %cl
	je	.L177
.L200:
	cmpb	$45, %cl
	.p2align 4,,2
	je	.L198
	movsd	24(%rsp), %xmm0
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L172:
	.cfi_restore_state
	movl	%ebx, %edi
	xorl	%eax, %eax
	call	exponential
	mulsd	24(%rsp), %xmm0
	movsd	%xmm0, 24(%rsp)
	jmp	.L190
	.p2align 4,,10
.L197:
	movl	%ebx, %edi
	xorl	%eax, %eax
	call	exponential
	movsd	24(%rsp), %xmm3
	divsd	%xmm0, %xmm3
	movsd	%xmm3, 24(%rsp)
	jmp	.L190
	.p2align 4,,10
.L198:
	movl	%ebx, %edi
	xorl	%eax, %eax
	addq	$1, gcnt(%rip)
	call	exponential
	movapd	%xmm0, %xmm1
.L195:
	movq	ch(%rip), %rdx
	movzbl	(%rdx,%rbp), %ecx
	cmpb	$42, %cl
	je	.L187
.L202:
	cmpb	$47, %cl
	je	.L199
	movsd	24(%rsp), %xmm0
	cmpb	$43, %cl
	subsd	%xmm1, %xmm0
	movsd	%xmm0, 24(%rsp)
	jne	.L200
.L177:
	movl	%ebx, %edi
	xorl	%eax, %eax
	addq	$1, gcnt(%rip)
	call	exponential
	movapd	%xmm0, %xmm2
.L193:
	movq	ch(%rip), %rsi
	movzbl	(%rsi,%rbp), %ecx
	cmpb	$42, %cl
	je	.L181
.L203:
	cmpb	$47, %cl
	je	.L201
	addsd	24(%rsp), %xmm2
	movsd	%xmm2, 24(%rsp)
	jmp	.L189
	.p2align 4,,10
.L199:
	xorl	%eax, %eax
	movl	%ebx, %edi
	movsd	%xmm1, (%rsp)
	call	exponential
	movq	ch(%rip), %rdx
	movsd	(%rsp), %xmm1
	movzbl	(%rdx,%rbp), %ecx
	divsd	%xmm0, %xmm1
	cmpb	$42, %cl
	jne	.L202
.L187:
	movl	%ebx, %edi
	xorl	%eax, %eax
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	mulsd	%xmm0, %xmm1
	jmp	.L195
	.p2align 4,,10
.L201:
	xorl	%eax, %eax
	movl	%ebx, %edi
	movsd	%xmm2, (%rsp)
	call	exponential
	movq	ch(%rip), %rsi
	movsd	(%rsp), %xmm2
	movzbl	(%rsi,%rbp), %ecx
	divsd	%xmm0, %xmm2
	cmpb	$42, %cl
	jne	.L203
.L181:
	movl	%ebx, %edi
	xorl	%eax, %eax
	movsd	%xmm2, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm2
	mulsd	%xmm0, %xmm2
	jmp	.L193
	.cfi_endproc
.LFE47:
	.size	expression, .-expression
	.section	.rodata.str1.1
.LC11:
	.string	"%lf%c"
.LC12:
	.string	"Expected: ')'.\n"
.LC13:
	.string	"x"
.LC14:
	.string	"t"
.LC15:
	.string	"a"
.LC16:
	.string	"y%d_%d"
	.section	.rodata.str1.8
	.align 8
.LC17:
	.string	"found y in definition, but i > numer_of_args\n"
	.section	.rodata.str1.1
.LC18:
	.string	"found y, but i <= 0\n"
	.section	.rodata.str1.8
	.align 8
.LC19:
	.string	"found y in definition, but yranks > ranks of this\n"
	.section	.rodata.str1.1
.LC20:
	.string	"found y, but ranks < 0\n"
.LC21:
	.string	"y%d"
.LC22:
	.string	"pi"
.LC24:
	.string	"sin"
.LC25:
	.string	"Expected: '(' after sin.\n"
.LC26:
	.string	"cos"
.LC27:
	.string	"Expected: '(' after cos.\n"
.LC28:
	.string	"sinh"
.LC29:
	.string	"Expected: '(' after sinh.\n"
.LC30:
	.string	"cosh"
.LC31:
	.string	"Expected: '(' after cosh.\n"
.LC32:
	.string	"acos"
.LC33:
	.string	"Expected: '(' after acos.\n"
.LC34:
	.string	"asin"
.LC35:
	.string	"Expected: '(' after asin.\n"
.LC36:
	.string	"asinh"
.LC37:
	.string	"Expected: '(' after asinh.\n"
.LC38:
	.string	"acosh"
.LC39:
	.string	"Expected: '(' after acosh.\n"
.LC40:
	.string	"tg"
.LC41:
	.string	"tan"
	.section	.rodata.str1.8
	.align 8
.LC42:
	.string	"Expected: '(' after tg or tan.\n"
	.section	.rodata.str1.1
.LC43:
	.string	"atg"
.LC44:
	.string	"atan"
	.section	.rodata.str1.8
	.align 8
.LC45:
	.string	"Expected: '(' after atg or atan.\n"
	.section	.rodata.str1.1
.LC46:
	.string	"tgh"
.LC47:
	.string	"tanh"
	.section	.rodata.str1.8
	.align 8
.LC48:
	.string	"Expected: '(' after tgh or tanh.\n"
	.section	.rodata.str1.1
.LC49:
	.string	"atgh"
.LC50:
	.string	"atanh"
	.section	.rodata.str1.8
	.align 8
.LC51:
	.string	"Expected: '(' after atgh or atanh.\n"
	.section	.rodata.str1.1
.LC52:
	.string	"ctg"
.LC53:
	.string	"ctan"
	.section	.rodata.str1.8
	.align 8
.LC54:
	.string	"Expected: '(' after ctg or ctan.\n"
	.section	.rodata.str1.1
.LC55:
	.string	"exp"
.LC56:
	.string	"e"
	.section	.rodata.str1.8
	.align 8
.LC57:
	.string	"Expected: '(' after exp or e.\n"
	.section	.rodata.str1.1
.LC58:
	.string	"ln"
.LC59:
	.string	"Expected: '(' after ln.\n"
.LC60:
	.string	"log"
.LC61:
	.string	"Expected: '(' after log.\n"
.LC62:
	.string	"sqrt"
.LC63:
	.string	"Expected: '(' after sqrt.\n"
.LC64:
	.string	"cbrt"
.LC65:
	.string	"Expected: '(' after cbrt.\n"
.LC66:
	.string	"ceil"
.LC69:
	.string	"Expected: '(' after ceil.\n"
.LC70:
	.string	"sgn"
.LC73:
	.string	"Expected: '(' after sgn.\n"
.LC74:
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
	.p2align 4,,15
	.globl	factor
	.type	factor, @function
factor:
.LFB45:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movl	%edi, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movslq	%edi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	leaq	0(,%rbp,4), %rbx
	subq	$112, %rsp
	.cfi_def_cfa_offset 160
	movq	position(%rip), %rdx
	movq	maxpos(%rip), %rcx
	addq	$1, gcnt(%rip)
	addq	%rbx, %rdx
	movslq	(%rdx), %rax
	cmpl	(%rcx,%rbp,4), %eax
	jge	.L205
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$10, (%r8)
	je	.L205
	movq	buffer(%rip), %rsi
	leal	1(%rax), %edi
	movq	(%rsi,%rbp,8), %r9
	movl	%edi, (%rdx)
	movzbl	(%r9,%rax), %r10d
	movb	%r10b, (%r8)
.L205:
	movl	%r12d, %edi
	movabsq	$4607182418800017408, %r14
	leaq	0(,%rbp,8), %r13
	call	skipblanks
	movq	ch(%rip), %r11
	movsd	.LC10(%rip), %xmm3
	movq	%r14, 8(%rsp)
	movzbl	(%r11,%rbp), %eax
	leaq	(%r11,%rbp), %r11
	.p2align 4,,10
.L466:
	cmpb	$45, %al
	jne	.L472
.L420:
	movsd	8(%rsp), %xmm2
	xorpd	%xmm3, %xmm2
	movsd	%xmm2, 8(%rsp)
.L207:
	movq	position(%rip), %r8
	movq	maxpos(%rip), %r10
	addq	%rbx, %r8
	movslq	(%r8), %r9
	cmpl	(%r10,%rbx), %r9d
	jge	.L466
	cmpb	$10, %al
	je	.L466
	movq	buffer(%rip), %r14
	leal	1(%r9), %ecx
	movq	(%r14,%r13), %rax
	movl	%ecx, (%r8)
	movzbl	(%rax,%r9), %edi
	movb	%dil, (%r11)
	movq	ch(%rip), %rsi
	movzbl	(%rsi,%rbp), %eax
	leaq	(%rsi,%rbp), %r11
	cmpb	$45, %al
	je	.L420
.L472:
	cmpb	$43, %al
	je	.L207
	movsbq	%al, %rdx
	cmpl	$127, %edx
	ja	.L210
	testb	$4, _DefaultRuneLocale+65(,%rdx,8)
	je	.L210
.L414:
	movq	position(%rip), %rdx
	movq	buffer(%rip), %r10
	movl	$.LC11, %esi
	addq	%rbx, %rdx
	movq	(%r10,%rbp,8), %r14
	movl	(%rdx), %eax
	subl	$1, %eax
	movl	%eax, (%rdx)
	movzbl	(%r11), %ecx
	cltq
	leaq	64(%rsp), %rdx
	movb	%cl, (%r14,%rax)
	movq	position(%rip), %rdi
	xorl	%eax, %eax
	movq	buffer(%rip), %r11
	movq	ch(%rip), %rcx
	movslq	(%rdi,%rbp,4), %rdi
	addq	(%r11,%rbp,8), %rdi
	addq	%rbp, %rcx
	call	sscanf
	movq	ch(%rip), %r11
	movzbl	(%r11,%rbp), %r8d
	movl	%r8d, %edx
	.p2align 4,,10
.L214:
	movq	position(%rip), %r9
	movq	maxpos(%rip), %rsi
	addq	%rbx, %r9
	movslq	(%r9), %r10
	cmpl	(%rsi,%rbx), %r10d
	jge	.L211
	cmpb	$10, %dl
	leaq	(%r11,%rbp), %r14
	je	.L211
	movq	buffer(%rip), %rdx
	leal	1(%r10), %ecx
	movq	(%rdx,%r13), %rax
	movl	%ecx, (%r9)
	movzbl	(%rax,%r10), %edi
	movb	%dil, (%r14)
	movq	ch(%rip), %r11
	movzbl	(%r11,%rbp), %edx
.L211:
	cmpb	%dl, %r8b
	jne	.L214
.L213:
	movl	%r12d, %edi
	call	skipblanks
.L471:
	movsd	8(%rsp), %xmm0
	mulsd	64(%rsp), %xmm0
	addq	$112, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L210:
	.cfi_restore_state
	cmpb	$46, %al
	je	.L414
	andl	$-5, %eax
	cmpb	$40, %al
	jne	.L215
	xorl	%eax, %eax
	movl	%r12d, %edi
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 64(%rsp)
	call	skipblanks
	movq	ch(%rip), %r14
	addq	%rbp, %r14
	movzbl	(%r14), %ecx
	cmpb	$41, %cl
	je	.L473
	cmpb	$44, %cl
	je	.L471
	movl	$.LC12, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %rdi
	movl	%r12d, %esi
	xorl	%eax, %eax
	movq	(%rdi,%rbp,8), %rdx
	movl	$.LC1, %edi
	call	printf
	movl	$213, %edx
	movl	$.LC8, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %r13
	movl	$1, 0(%r13,%rbp,4)
	jmp	.L213
	.p2align 4,,10
.L215:
	leaq	80(%rsp), %r13
	movl	%r12d, %edi
	movq	%r13, %rsi
	call	read_id
	movq	%r13, %r8
.L220:
	movl	(%r8), %ecx
	addq	$4, %r8
	leal	-16843009(%rcx), %eax
	notl	%ecx
	andl	%ecx, %eax
	movl	%eax, %r9d
	andl	$-2139062144, %r9d
	je	.L220
	movl	%r9d, %esi
	movl	%r9d, %edi
	leaq	2(%r8), %r10
	shrl	$16, %esi
	testl	$32896, %eax
	movl	$2, %r11d
	cmove	%esi, %edi
	cmove	%r10, %r8
	movq	%r13, %rsi
	addb	%dil, %dil
	movq	%r11, %rcx
	movl	$.LC13, %edi
	sbbq	$3, %r8
	subq	%r13, %r8
	repz; cmpsb
	movzbl	79(%rsp,%r8), %r14d
	je	.L222
	movl	$.LC14, %edi
	movq	%r13, %rsi
	movq	%r11, %rcx
	repz; cmpsb
	je	.L222
	movl	$.LC15, %edi
	movq	%r13, %rsi
	movq	%r11, %rcx
	repz; cmpsb
	jne	.L223
.L222:
	movq	t(%rip), %rdx
	movsd	(%rdx,%rbp,8), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L223:
	leaq	48(%rsp), %rcx
	leaq	32(%rsp), %rdx
	xorl	%eax, %eax
	movl	$.LC16, %esi
	movq	%r13, %rdi
	call	sscanf
	cmpl	$2, %eax
	jne	.L224
	cmpb	$39, %r14b
	je	.L474
	leaq	32(%rsp), %rdx
	xorl	%eax, %eax
	movl	$.LC21, %esi
	movq	%r13, %rdi
	call	sscanf
	cmpl	$1, %eax
	je	.L416
.L230:
	movl	$3, %edx
	movl	$.LC22, %edi
	movq	%r13, %rsi
	movq	%rdx, %rcx
	repz; cmpsb
	je	.L475
	movl	$4, %r8d
	movl	$.LC24, %edi
	movq	%r13, %rsi
	movq	%r8, %rcx
	repz; cmpsb
	je	.L476
	movl	$.LC26, %edi
	movq	%r13, %rsi
	movq	%r8, %rcx
	repz; cmpsb
	je	.L477
	movl	$5, %eax
	movl	$.LC28, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L478
	movl	$.LC30, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L479
	movl	$.LC32, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L480
	movl	$.LC34, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L481
	movl	$6, %r9d
	movl	$.LC36, %edi
	movq	%r13, %rsi
	movq	%r9, %rcx
	repz; cmpsb
	je	.L482
	movl	$.LC38, %edi
	movq	%r13, %rsi
	movq	%r9, %rcx
	repz; cmpsb
	je	.L483
	movl	$.LC40, %edi
	movq	%r13, %rsi
	movq	%rdx, %rcx
	repz; cmpsb
	je	.L251
	movl	$4, %r10d
	movl	$.LC41, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	jne	.L252
.L251:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L484
	movl	$.LC42, %ecx
	movl	%r12d, %edx
	movl	$329, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
	.p2align 4,,10
.L473:
	addq	position(%rip), %rbx
	movq	maxpos(%rip), %rsi
	movslq	(%rbx), %r11
	cmpl	(%rsi,%rbp,4), %r11d
	jge	.L213
	movq	buffer(%rip), %r8
	leal	1(%r11), %eax
	movq	(%r8,%rbp,8), %r9
	movl	%eax, (%rbx)
	movzbl	(%r9,%r11), %ebx
	movb	%bl, (%r14)
	jmp	.L213
.L224:
	leaq	32(%rsp), %rdx
	xorl	%eax, %eax
	movl	$.LC21, %esi
	movq	%r13, %rdi
	call	sscanf
	cmpl	$1, %eax
	jne	.L230
	cmpb	$39, %r14b
	je	.L230
.L416:
	movq	nvar(%rip), %rdx
	movslq	32(%rsp), %r10
	movl	$0, 48(%rsp)
	cmpl	%r10d, (%rdx,%rbp,4)
	jl	.L485
	testl	%r10d, %r10d
	jle	.L486
	movq	ranks(%rip), %rcx
	movl	(%rcx,%rbp,4), %r14d
	testl	%r14d, %r14d
	jle	.L487
	movq	y(%rip), %rdi
	movq	(%rdi,%rbp,8), %r13
	movq	0(%r13), %rsi
	movsd	-8(%rsi,%r10,8), %xmm15
	movsd	%xmm15, 64(%rsp)
	jmp	.L213
.L475:
	movl	%r12d, %edi
	movabsq	$4614256656552045848, %rbp
	call	skipblanks
	movq	%rbp, 64(%rsp)
	jmp	.L213
.L474:
	movq	nvar(%rip), %r8
	movslq	32(%rsp), %r11
	cmpl	%r11d, (%r8,%rbp,4)
	jl	.L488
	testl	%r11d, %r11d
	jle	.L489
	movq	ranks(%rip), %rax
	movslq	48(%rsp), %r9
	cmpl	%r9d, (%rax,%rbp,4)
	jle	.L490
	testl	%r9d, %r9d
	js	.L491
	movq	y(%rip), %rbx
	movq	(%rbx,%rbp,8), %rbp
	movq	0(%rbp,%r9,8), %r10
	movsd	-8(%r10,%r11,8), %xmm1
	movsd	%xmm1, 64(%rsp)
	jmp	.L213
.L477:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L492
	movl	$.LC27, %ecx
	movl	%r12d, %edx
	movl	$259, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L476:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L493
	movl	$.LC25, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %rsi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	movq	(%rsi,%rbp,8), %rdx
	movl	%r12d, %esi
	call	printf
	movl	$249, %edx
	movl	$.LC8, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %r11
	movl	$1, (%r11,%rbp,4)
	jmp	.L213
.L488:
	movl	$.LC17, %ecx
	movl	%r12d, %edx
	movl	$222, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L480:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L494
	movl	$.LC33, %ecx
	movl	%r12d, %edx
	movl	$289, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
	.p2align 4,,10
.L479:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L495
	movl	$.LC31, %ecx
	movl	%r12d, %edx
	movl	$279, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L478:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L496
	movl	$.LC29, %ecx
	movl	%r12d, %edx
	movl	$269, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L485:
	movl	$.LC17, %ecx
	movl	%r12d, %edx
	movl	$231, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L493:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r8
	movl	%r12d, %edi
	movq	(%r8,%rbp,8), %r9
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	0(%r13), %ebx
	cltq
	movb	%bl, (%r9,%rax)
	xorl	%eax, %eax
	call	exponential
	call	sin
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L494:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdx), %edi
	cltq
	movb	%dil, (%r14,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	exponential
	call	acos
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L495:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rsi
	movl	%r12d, %edi
	movq	(%rsi,%rbp,8), %r11
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	0(%r13), %ebx
	cltq
	movb	%bl, (%r11,%rax)
	xorl	%eax, %eax
	call	exponential
	call	cosh
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L487:
	movl	$.LC19, %ecx
	movl	%r12d, %edx
	movl	$233, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L492:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdx), %edi
	cltq
	movb	%dil, (%r14,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	exponential
	call	cos
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L486:
	movl	$.LC18, %ecx
	movl	%r12d, %edx
	movl	$232, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L496:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %r10d
	cltq
	movb	%r10b, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	call	sinh
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L481:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L497
	movl	$.LC35, %ecx
	movl	%r12d, %edx
	movl	$299, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L489:
	movl	$.LC18, %ecx
	movl	%r12d, %edx
	movl	$223, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L497:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %r10d
	cltq
	movb	%r10b, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	call	asin
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L491:
	movl	$.LC20, %ecx
	movl	%r12d, %edx
	movl	$225, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L490:
	movl	$.LC19, %ecx
	movl	%r12d, %edx
	movl	$224, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L483:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L498
	movl	$.LC39, %ecx
	movl	%r12d, %edx
	movl	$319, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L482:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L499
	movl	$.LC37, %ecx
	movl	%r12d, %edx
	movl	$309, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L498:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdx), %edi
	cltq
	movb	%dil, (%r14,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	exponential
	call	acosh
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L499:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rsi
	movl	%r12d, %edi
	movq	(%rsi,%rbp,8), %r11
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	0(%r13), %ebx
	cltq
	movb	%bl, (%r11,%rax)
	xorl	%eax, %eax
	call	exponential
	call	asinh
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L252:
	movl	$.LC43, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L254
	movl	$5, %r14d
	movl	$.LC44, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	jne	.L255
.L254:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L500
	movl	$.LC45, %ecx
	movl	%r12d, %edx
	movl	$339, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L484:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %r10d
	cltq
	movb	%r10b, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	call	tan
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L255:
	movl	$.LC46, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L257
	movl	$.LC47, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	jne	.L258
.L257:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L501
	movl	$.LC48, %ecx
	movl	%r12d, %edx
	movl	$349, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L500:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rsi
	movl	%r12d, %edi
	movq	(%rsi,%rbp,8), %r11
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	0(%r13), %ebx
	cltq
	movb	%bl, (%r11,%rax)
	xorl	%eax, %eax
	call	exponential
	call	atan
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L258:
	movl	$.LC49, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	je	.L260
	movl	$6, %ecx
	movl	$.LC50, %edi
	movq	%r13, %rsi
	repz; cmpsb
	jne	.L261
.L260:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L502
	movl	$.LC51, %ecx
	movl	%r12d, %edx
	movl	$359, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L501:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdx), %edi
	cltq
	movb	%dil, (%r14,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	exponential
	call	tanh
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L261:
	movl	$.LC52, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L263
	movl	$.LC53, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	jne	.L264
.L263:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L503
	movl	$.LC54, %ecx
	movl	%r12d, %edx
	movl	$369, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L502:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %r10d
	cltq
	movb	%r10b, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	call	atanh
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L264:
	movl	$.LC55, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L266
	movl	$2, %ecx
	movl	$.LC56, %edi
	movq	%r13, %rsi
	repz; cmpsb
	jne	.L267
.L266:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L504
	movl	$.LC57, %ecx
	movl	%r12d, %edx
	movl	$379, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L503:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rsi
	movl	%r12d, %edi
	movq	(%rsi,%rbp,8), %r11
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	0(%r13), %ebx
	cltq
	movb	%bl, (%r11,%rax)
	xorl	%eax, %eax
	call	exponential
	call	tan
	movsd	.LC9(%rip), %xmm14
	divsd	%xmm0, %xmm14
	movsd	%xmm14, 64(%rsp)
	jmp	.L213
.L267:
	movl	$3, %r11d
	movl	$.LC58, %edi
	movq	%r13, %rsi
	movq	%r11, %rcx
	repz; cmpsb
	je	.L505
	movl	$4, %edx
	movl	$.LC60, %edi
	movq	%r13, %rsi
	movq	%rdx, %rcx
	repz; cmpsb
	je	.L506
	movl	$5, %r8d
	movl	$.LC62, %edi
	movq	%r13, %rsi
	movq	%r8, %rcx
	repz; cmpsb
	je	.L507
	movl	$.LC64, %edi
	movq	%r13, %rsi
	movq	%r8, %rcx
	repz; cmpsb
	je	.L508
	movl	$.LC66, %edi
	movq	%r13, %rsi
	movq	%r8, %rcx
	repz; cmpsb
	je	.L509
	movl	$.LC70, %edi
	movq	%r13, %rsi
	movq	%rdx, %rcx
	repz; cmpsb
	je	.L510
	movl	$.LC74, %edi
	movq	%r13, %rsi
	movq	%rdx, %rcx
	repz; cmpsb
	je	.L511
	movl	$.LC76, %edi
	movq	%r13, %rsi
	movq	%r8, %rcx
	repz; cmpsb
	je	.L512
	movl	$.LC78, %edi
	movq	%r13, %rsi
	movq	%r11, %rcx
	repz; cmpsb
	je	.L513
	movl	$3, %eax
	movl	$.LC81, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L514
	movl	$.LC83, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L515
	movl	$.LC85, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L516
	movl	$.LC87, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L517
	movl	$.LC89, %edi
	movq	%r13, %rsi
	movq	%rax, %rcx
	repz; cmpsb
	je	.L518
	movl	$4, %r9d
	movl	$.LC91, %edi
	movq	%r13, %rsi
	movq	%r9, %rcx
	repz; cmpsb
	je	.L519
	movl	$.LC93, %edi
	movq	%r13, %rsi
	movq	%r9, %rcx
	repz; cmpsb
	je	.L520
	movl	$.LC95, %edi
	movq	%r13, %rsi
	movq	%r9, %rcx
	repz; cmpsb
	je	.L521
	movl	$.LC97, %edi
	movq	%r13, %rsi
	movq	%r9, %rcx
	repz; cmpsb
	je	.L522
	movl	$5, %ecx
	movl	$.LC100, %edi
	movq	%r13, %rsi
	repz; cmpsb
	je	.L523
	movl	$4, %r10d
	movl	$.LC103, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L524
	movl	$.LC105, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L525
	movl	$5, %r14d
	movl	$.LC108, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	je	.L526
	movl	$.LC111, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	je	.L527
	movl	$.LC114, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	je	.L528
	movl	$.LC117, %edi
	movq	%r13, %rsi
	movq	%r14, %rcx
	repz; cmpsb
	je	.L529
	movl	$.LC120, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L530
	movl	$.LC123, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L531
	movl	$3, %ecx
	movl	$.LC125, %edi
	movq	%r13, %rsi
	repz; cmpsb
	je	.L532
	movl	$.LC128, %edi
	movq	%r13, %rsi
	movq	%r10, %rcx
	repz; cmpsb
	je	.L533
	movl	$3, %ecx
	movl	$.LC131, %edi
	movq	%r13, %rsi
	repz; cmpsb
	je	.L534
	movl	$5, %ecx
	movl	$.LC134, %edi
	movq	%r13, %rsi
	repz; cmpsb
	jne	.L408
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L535
	movl	$.LC102, %ecx
	movl	%r12d, %edx
	movl	$925, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L504:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdx), %edi
	cltq
	movb	%dil, (%r14,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	exponential
	call	exp
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L535:
	movq	position(%rip), %rdx
	movq	buffer(%rip), %r11
	movl	%r12d, %edi
	addq	%rbx, %rdx
	movq	(%r11,%rbp,8), %r8
	movl	(%rdx), %eax
	subl	$1, %eax
	movl	%eax, (%rdx)
	movzbl	0(%r13), %r9d
	cltq
	movb	%r9b, (%r8,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %r10
	cmpb	$44, (%r10,%rbp)
	je	.L536
	movl	$.LC135, %ecx
	movl	%r12d, %edx
	movl	$912, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
	movsd	%xmm0, 24(%rsp)
.L411:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r11
	cmpb	$44, (%r11,%rbp)
	je	.L537
	movl	$.LC136, %ecx
	movl	%r12d, %edx
	movl	$921, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm2
	movapd	%xmm2, %xmm0
.L413:
	movsd	24(%rsp), %xmm1
	call	udf3
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L408:
	movl	$.LC137, %ecx
	movl	%r12d, %edx
	movl	$927, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L537:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r8
	movl	%r12d, %edi
	movq	(%r8,%rbp,8), %rdx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%rdx,%rax)
	xorl	%eax, %eax
	call	expression
	movapd	%xmm0, %xmm2
	movsd	16(%rsp), %xmm0
	jmp	.L413
.L536:
	movl	%r12d, %edi
	call	skipblanks
	movq	position(%rip), %rdi
	movq	buffer(%rip), %r14
	movq	ch(%rip), %rsi
	addq	%rbx, %rdi
	movq	(%r14,%rbp,8), %rcx
	movl	(%rdi), %eax
	subl	$1, %eax
	movl	%eax, (%rdi)
	movzbl	(%rsi,%rbp), %r13d
	cltq
	movl	%r12d, %edi
	movb	%r13b, (%rcx,%rax)
	xorl	%eax, %eax
	call	expression
	movsd	%xmm0, 24(%rsp)
	jmp	.L411
.L534:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r9
	addq	%rbp, %r9
	cmpb	$40, (%r9)
	je	.L538
	movl	$.LC102, %ecx
	movl	%r12d, %edx
	movl	$893, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L533:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L539
	movl	$.LC130, %ecx
	movl	%r12d, %edx
	movl	$859, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L538:
	movq	position(%rip), %rcx
	movq	buffer(%rip), %r10
	addq	%rbx, %rcx
	movq	(%r10,%rbp,8), %r14
	movl	(%rcx), %eax
	subl	$1, %eax
	movl	%eax, (%rcx)
	movzbl	(%r9), %edi
	cltq
	movb	%dil, (%r14,%rax)
	xorl	%eax, %eax
	movl	%r12d, %edi
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rsi
	cmpb	$44, (%rsi,%rbp)
	je	.L540
	movl	$.LC132, %ecx
	movl	%r12d, %edx
	movl	$878, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm1
	movsd	%xmm1, 24(%rsp)
.L403:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r10
	cmpb	$44, (%r10,%rbp)
	je	.L541
	movl	$.LC133, %ecx
	movl	%r12d, %edx
	movl	$887, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
.L405:
	xorpd	%xmm3, %xmm3
	movsd	24(%rsp), %xmm2
	cmpltsd	16(%rsp), %xmm3
	andpd	%xmm3, %xmm2
	andnpd	%xmm0, %xmm3
	orpd	%xmm2, %xmm3
	movsd	%xmm3, 64(%rsp)
	jmp	.L213
.L539:
	movq	position(%rip), %r11
	movq	buffer(%rip), %rcx
	movl	%r12d, %edi
	addq	%rbx, %r11
	movq	(%rcx,%rbp,8), %rsi
	movl	(%r11), %eax
	subl	$1, %eax
	movl	%eax, (%r11)
	movzbl	0(%r13), %r8d
	cltq
	movb	%r8b, (%rsi,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rdx
	cmpb	$44, (%rdx,%rbp)
	je	.L542
	movl	$.LC129, %ecx
	movl	%r12d, %edx
	movl	$851, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
	movapd	%xmm0, %xmm4
.L393:
	movsd	.LC79(%rip), %xmm5
	cvttsd2siq	%xmm4, %rbx
	comisd	%xmm5, %xmm4
	jb	.L395
	subsd	%xmm5, %xmm4
	movabsq	$-9223372036854775808, %rbp
	cvttsd2siq	%xmm4, %rbx
	xorq	%rbp, %rbx
.L395:
	comisd	%xmm5, %xmm0
	cvttsd2siq	%xmm0, %r13
	jb	.L397
	subsd	%xmm5, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %r13
	xorq	%rcx, %r13
.L397:
	xorq	%r13, %rbx
	js	.L398
	cvtsi2sdq	%rbx, %xmm6
.L399:
	movsd	%xmm6, 64(%rsp)
	jmp	.L213
.L542:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movq	ch(%rip), %r14
	movq	(%r9,%rbp,8), %r10
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r14,%rbp), %edi
	cltq
	movb	%dil, (%r10,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	expression
	movsd	16(%rsp), %xmm4
	jmp	.L393
.L398:
	movq	%rbx, %rsi
	andl	$1, %ebx
	shrq	%rsi
	orq	%rbx, %rsi
	cvtsi2sdq	%rsi, %xmm6
	addsd	%xmm6, %xmm6
	jmp	.L399
.L541:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %rdi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%rdi,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	expression
	jmp	.L405
.L540:
	movl	%r12d, %edi
	call	skipblanks
	movq	position(%rip), %r8
	movq	buffer(%rip), %r13
	movl	%r12d, %edi
	movq	ch(%rip), %rdx
	addq	%rbx, %r8
	movq	0(%r13,%rbp,8), %r11
	movl	(%r8), %eax
	subl	$1, %eax
	movl	%eax, (%r8)
	movzbl	(%rdx,%rbp), %r9d
	cltq
	movb	%r9b, (%r11,%rax)
	xorl	%eax, %eax
	call	expression
	movsd	%xmm0, 24(%rsp)
	jmp	.L403
.L532:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r11
	addq	%rbp, %r11
	cmpb	$40, (%r11)
	je	.L543
	movl	$.LC127, %ecx
	movl	%r12d, %edx
	movl	$833, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L531:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L544
	movl	$.LC122, %ecx
	movl	%r12d, %edx
	movl	$807, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L543:
	movq	position(%rip), %rdx
	movq	buffer(%rip), %r8
	movl	%r12d, %edi
	addq	%rbx, %rdx
	movq	(%r8,%rbp,8), %r9
	movl	(%rdx), %eax
	subl	$1, %eax
	movl	%eax, (%rdx)
	movzbl	(%r11), %r10d
	cltq
	movb	%r10b, (%r9,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %r14
	cmpb	$44, (%r14,%rbp)
	je	.L545
	movl	$.LC126, %ecx
	movl	%r12d, %edx
	movl	$825, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
	movapd	%xmm0, %xmm7
.L383:
	movsd	.LC79(%rip), %xmm8
	cvttsd2siq	%xmm7, %rsi
	comisd	%xmm8, %xmm7
	jb	.L385
	subsd	%xmm8, %xmm7
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm7, %rsi
	xorq	%rcx, %rsi
.L385:
	comisd	%xmm8, %xmm0
	cvttsd2siq	%xmm0, %r11
	jb	.L387
	subsd	%xmm8, %xmm0
	movabsq	$-9223372036854775808, %r8
	cvttsd2siq	%xmm0, %r11
	xorq	%r8, %r11
.L387:
	orq	%r11, %rsi
	js	.L388
	cvtsi2sdq	%rsi, %xmm9
.L389:
	movsd	%xmm9, 64(%rsp)
	jmp	.L213
.L545:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rdi
	movq	(%rdi,%rbp,8), %r13
	movl	%r12d, %edi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, 0(%r13,%rax)
	xorl	%eax, %eax
	call	expression
	movsd	16(%rsp), %xmm7
	jmp	.L383
.L530:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdi
	addq	%rbp, %rdi
	cmpb	$40, (%rdi)
	je	.L546
	movl	$.LC122, %ecx
	movl	%r12d, %edx
	movl	$781, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L529:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r11
	addq	%rbp, %r11
	cmpb	$40, (%r11)
	je	.L547
	movl	$.LC119, %ecx
	movl	%r12d, %edx
	movl	$755, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L546:
	movq	position(%rip), %rcx
	movq	buffer(%rip), %r13
	addq	%rbx, %rcx
	movq	0(%r13,%rbp,8), %rsi
	movl	(%rcx), %eax
	subl	$1, %eax
	movl	%eax, (%rcx)
	movzbl	(%rdi), %r11d
	cltq
	movl	%r12d, %edi
	movb	%r11b, (%rsi,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %r8
	cmpb	$44, (%r8,%rbp)
	je	.L548
	movl	$.LC121, %ecx
	movl	%r12d, %edx
	movl	$773, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
	movapd	%xmm0, %xmm13
.L363:
	movsd	.LC79(%rip), %xmm14
	cvttsd2siq	%xmm13, %rax
	comisd	%xmm14, %xmm13
	jb	.L365
	subsd	%xmm14, %xmm13
	movabsq	$-9223372036854775808, %r10
	cvttsd2siq	%xmm13, %rax
	xorq	%r10, %rax
.L365:
	comisd	%xmm14, %xmm0
	cvttsd2siq	%xmm0, %r14
	jb	.L367
	subsd	%xmm14, %xmm0
	movabsq	$-9223372036854775808, %rdi
	cvttsd2siq	%xmm0, %r14
	xorq	%rdi, %r14
.L367:
	xorl	%edx, %edx
	divq	%r14
	testq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm15
	jns	.L369
	movq	%rdx, %r13
	andl	$1, %edx
	shrq	%r13
	orq	%rdx, %r13
	cvtsi2sdq	%r13, %xmm15
	addsd	%xmm15, %xmm15
.L369:
	movsd	%xmm15, 64(%rsp)
	jmp	.L213
.L548:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rdx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%rdx,%rax)
	xorl	%eax, %eax
	call	expression
	movsd	16(%rsp), %xmm13
	jmp	.L363
.L547:
	movq	position(%rip), %rcx
	movq	buffer(%rip), %rsi
	movl	%r12d, %edi
	addq	%rbx, %rcx
	movq	(%rsi,%rbp,8), %r8
	movl	(%rcx), %eax
	subl	$1, %eax
	movl	%eax, (%rcx)
	movzbl	(%r11), %r9d
	cltq
	movb	%r9b, (%r8,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %r10
	cmpb	$44, (%r10,%rbp)
	je	.L549
	movl	$.LC118, %ecx
	movl	%r12d, %edx
	movl	$750, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L544:
	movq	position(%rip), %rdi
	movq	buffer(%rip), %r10
	addq	%rbx, %rdi
	movq	(%r10,%rbp,8), %r14
	movl	(%rdi), %eax
	subl	$1, %eax
	movl	%eax, (%rdi)
	movzbl	(%rdx), %r13d
	cltq
	movl	%r12d, %edi
	movb	%r13b, (%r14,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rsi
	cmpb	$44, (%rsi,%rbp)
	je	.L550
	movl	$.LC124, %ecx
	movl	%r12d, %edx
	movl	$799, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
	movapd	%xmm0, %xmm10
.L373:
	movsd	.LC79(%rip), %xmm11
	cvttsd2siq	%xmm10, %r8
	comisd	%xmm11, %xmm10
	jb	.L375
	subsd	%xmm11, %xmm10
	movabsq	$-9223372036854775808, %r9
	cvttsd2siq	%xmm10, %r8
	xorq	%r9, %r8
.L375:
	comisd	%xmm11, %xmm0
	cvttsd2siq	%xmm0, %rdx
	jb	.L377
	subsd	%xmm11, %xmm0
	movabsq	$-9223372036854775808, %r10
	cvttsd2siq	%xmm0, %rdx
	xorq	%r10, %rdx
.L377:
	andq	%rdx, %r8
	js	.L378
	cvtsi2sdq	%r8, %xmm12
.L379:
	movsd	%xmm12, 64(%rsp)
	jmp	.L213
.L550:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movl	%r12d, %edi
	movq	(%rcx,%rbp,8), %r11
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%r11,%rax)
	xorl	%eax, %eax
	call	expression
	movsd	16(%rsp), %xmm10
	jmp	.L373
.L378:
	movq	%r8, %r14
	andl	$1, %r8d
	shrq	%r14
	orq	%r8, %r14
	cvtsi2sdq	%r14, %xmm12
	addsd	%xmm12, %xmm12
	jmp	.L379
.L388:
	movq	%rsi, %r9
	andl	$1, %esi
	shrq	%r9
	orq	%rsi, %r9
	cvtsi2sdq	%r9, %xmm9
	addsd	%xmm9, %xmm9
	jmp	.L389
.L549:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r14
	movq	(%r14,%rbp,8), %rdi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%rdi,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	expression
	minsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L528:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L551
	movl	$.LC116, %ecx
	movl	%r12d, %edx
	movl	$732, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L527:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L552
	movl	$.LC113, %ecx
	movl	%r12d, %edx
	movl	$708, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L551:
	movq	position(%rip), %rsi
	movq	buffer(%rip), %r13
	movl	%r12d, %edi
	addq	%rbx, %rsi
	movq	0(%r13,%rbp,8), %r11
	movl	(%rsi), %eax
	subl	$1, %eax
	movl	%eax, (%rsi)
	movzbl	(%rdx), %r8d
	cltq
	movb	%r8b, (%r11,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rcx
	cmpb	$44, (%rcx,%rbp)
	je	.L553
	movl	$.LC115, %ecx
	movl	%r12d, %edx
	movl	$726, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm1
	movsd	%xmm1, 64(%rsp)
	jmp	.L213
.L552:
	movq	position(%rip), %rsi
	movq	buffer(%rip), %r13
	movl	%r12d, %edi
	addq	%rbx, %rsi
	movq	0(%r13,%rbp,8), %r11
	movl	(%rsi), %eax
	subl	$1, %eax
	movl	%eax, (%rsi)
	movzbl	(%rdx), %r8d
	cltq
	movb	%r8b, (%r11,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rcx
	cmpb	$44, (%rcx,%rbp)
	je	.L554
	movl	$.LC112, %ecx
	movl	%r12d, %edx
	movl	$703, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm3
	movsd	%xmm3, 64(%rsp)
	jmp	.L213
.L553:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movq	ch(%rip), %r14
	movq	(%r9,%rbp,8), %r10
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r14,%rbp), %edi
	cltq
	movb	%dil, (%r10,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	expression
	minsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L554:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %r10
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%r10,%rax)
	xorl	%eax, %eax
	call	expression
	maxsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L526:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r14
	addq	%rbp, %r14
	cmpb	$40, (%r14)
	je	.L555
	movl	$.LC110, %ecx
	movl	%r12d, %edx
	movl	$685, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L525:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r14
	addq	%rbp, %r14
	cmpb	$40, (%r14)
	je	.L556
	movl	$.LC107, %ecx
	movl	%r12d, %edx
	movl	$662, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L555:
	movq	position(%rip), %rdx
	movq	buffer(%rip), %rdi
	addq	%rbx, %rdx
	movq	(%rdi,%rbp,8), %r13
	movl	%r12d, %edi
	movl	(%rdx), %eax
	subl	$1, %eax
	movl	%eax, (%rdx)
	movzbl	(%r14), %r11d
	cltq
	movb	%r11b, 0(%r13,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rsi
	cmpb	$44, (%rsi,%rbp)
	je	.L557
	movl	$.LC109, %ecx
	movl	%r12d, %edx
	movl	$680, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm2
	movsd	%xmm2, 64(%rsp)
	jmp	.L213
.L556:
	movq	position(%rip), %rdx
	movq	buffer(%rip), %rdi
	addq	%rbx, %rdx
	movq	(%rdi,%rbp,8), %r13
	movl	%r12d, %edi
	movl	(%rdx), %eax
	subl	$1, %eax
	movl	%eax, (%rdx)
	movzbl	(%r14), %r11d
	cltq
	movb	%r11b, 0(%r13,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rsi
	cmpb	$44, (%rsi,%rbp)
	je	.L558
	movl	$.LC106, %ecx
	movl	%r12d, %edx
	movl	$658, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
.L343:
	minsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L557:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r8
	movl	%r12d, %edi
	movq	ch(%rip), %rcx
	movq	(%r8,%rbp,8), %r9
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rcx,%rbp), %r10d
	cltq
	movb	%r10b, (%r9,%rax)
	xorl	%eax, %eax
	call	expression
	maxsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L558:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r8
	movl	%r12d, %edi
	movq	(%r8,%rbp,8), %r9
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%r9,%rax)
	xorl	%eax, %eax
	call	expression
	jmp	.L343
.L524:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r10
	addq	%rbp, %r10
	cmpb	$40, (%r10)
	je	.L559
	movl	$.LC102, %ecx
	movl	%r12d, %edx
	movl	$640, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L523:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r10
	addq	%rbp, %r10
	cmpb	$40, (%r10)
	je	.L560
	movl	$.LC102, %ecx
	movl	%r12d, %edx
	movl	$618, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L559:
	movq	position(%rip), %rdi
	movq	buffer(%rip), %rcx
	addq	%rbx, %rdi
	movq	(%rcx,%rbp,8), %r14
	movl	(%rdi), %eax
	subl	$1, %eax
	movl	%eax, (%rdi)
	movzbl	(%r10), %r13d
	cltq
	movl	%r12d, %edi
	movb	%r13b, (%r14,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rdx
	cmpb	$44, (%rdx,%rbp)
	je	.L561
	movl	$.LC104, %ecx
	movl	%r12d, %edx
	movl	$636, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm0
.L339:
	maxsd	16(%rsp), %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L560:
	movq	position(%rip), %rdi
	movq	buffer(%rip), %rcx
	addq	%rbx, %rdi
	movq	(%rcx,%rbp,8), %r14
	movl	(%rdi), %eax
	subl	$1, %eax
	movl	%eax, (%rdi)
	movzbl	(%r10), %r13d
	cltq
	movl	%r12d, %edi
	movb	%r13b, (%r14,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rdx
	cmpb	$44, (%rdx,%rbp)
	je	.L562
	movl	$.LC101, %ecx
	movl	%r12d, %edx
	movl	$614, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm1
	movapd	%xmm1, %xmm0
.L335:
	call	udf2
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L561:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r11
	movl	%r12d, %edi
	movq	ch(%rip), %r8
	movq	(%r11,%rbp,8), %rsi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8,%rbp), %r9d
	cltq
	movb	%r9b, (%rsi,%rax)
	xorl	%eax, %eax
	call	expression
	jmp	.L339
.L562:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r11
	movl	%r12d, %edi
	movq	(%r11,%rbp,8), %rsi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movq	ch(%rip), %rbx
	cltq
	movzbl	(%rbx,%rbp), %ebp
	movb	%bpl, (%rsi,%rax)
	xorl	%eax, %eax
	call	expression
	movapd	%xmm0, %xmm1
	movsd	16(%rsp), %xmm0
	jmp	.L335
.L522:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L563
	movl	$.LC99, %ecx
	movl	%r12d, %edx
	movl	$596, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L521:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L564
	movl	$.LC96, %ecx
	movl	%r12d, %edx
	movl	$574, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L563:
	movq	position(%rip), %rcx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	addq	%rbx, %rcx
	movq	(%r9,%rbp,8), %r10
	movl	(%rcx), %eax
	subl	$1, %eax
	movl	%eax, (%rcx)
	movzbl	(%r8), %r14d
	cltq
	movb	%r14b, (%r10,%rax)
	xorl	%eax, %eax
	call	expression
	movl	%r12d, %edi
	movsd	%xmm0, 16(%rsp)
	call	skipblanks
	movq	ch(%rip), %rdi
	cmpb	$44, (%rdi,%rbp)
	je	.L565
	movl	$.LC98, %ecx
	movl	%r12d, %edx
	movl	$592, %esi
	movl	$.LC8, %edi
	call	exception
	movsd	16(%rsp), %xmm1
	movapd	%xmm1, %xmm0
.L331:
	call	pow
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L564:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %ebx
	cltq
	movb	%bl, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC79(%rip), %xmm4
	cvttsd2siq	%xmm0, %r10
	comisd	%xmm4, %xmm0
	jb	.L325
	subsd	%xmm4, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %r10
	xorq	%rcx, %r10
.L325:
	xorq	$-1, %r10
	js	.L326
	cvtsi2sdq	%r10, %xmm5
.L327:
	movsd	%xmm5, 64(%rsp)
	jmp	.L213
.L565:
	movl	%r12d, %edi
	call	skipblanks
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r13
	movl	%r12d, %edi
	movq	ch(%rip), %r11
	movq	0(%r13,%rbp,8), %rdx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r11,%rbp), %esi
	cltq
	movb	%sil, (%rdx,%rax)
	xorl	%eax, %eax
	call	expression
	movapd	%xmm0, %xmm1
	movsd	16(%rsp), %xmm0
	jmp	.L331
.L326:
	movq	%r10, %r14
	andl	$1, %r10d
	shrq	%r14
	orq	%r10, %r14
	cvtsi2sdq	%r14, %xmm5
	addsd	%xmm5, %xmm5
	jmp	.L327
.L520:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdi
	addq	%rbp, %rdi
	cmpb	$40, (%rdi)
	je	.L566
	movl	$.LC94, %ecx
	movl	%r12d, %edx
	movl	$562, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L519:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r10
	addq	%rbp, %r10
	cmpb	$40, (%r10)
	je	.L567
	movl	$.LC92, %ecx
	movl	%r12d, %edx
	movl	$547, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L566:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r13
	movq	0(%r13,%rbp,8), %r11
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdi), %edx
	cltq
	movl	%r12d, %edi
	movb	%dl, (%r11,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC79(%rip), %xmm6
	cvttsd2siq	%xmm0, %rbx
	comisd	%xmm6, %xmm0
	jb	.L321
	subsd	%xmm6, %xmm0
	movabsq	$-9223372036854775808, %rsi
	cvttsd2siq	%xmm0, %rbx
	xorq	%rsi, %rbx
.L321:
	movq	%rbx, %r8
	movzbl	%bh, %ebp
	movzbl	%bl, %eax
	andl	$16711680, %r8d
	movabsq	$-6148914691236517205, %r9
	shrq	$16, %r8
	addq	%rbp, %r8
	addq	%r8, %rax
	mulq	%r9
	shrq	%rdx
	cvtsi2sdq	%rdx, %xmm7
	movsd	%xmm7, 64(%rsp)
	jmp	.L213
.L567:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r10), %edi
	cltq
	movb	%dil, (%r14,%rax)
	xorl	%eax, %eax
	movl	%r12d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm8
	cvttsd2siq	%xmm0, %r13
	comisd	%xmm8, %xmm0
	jb	.L317
	subsd	%xmm8, %xmm0
	movabsq	$-9223372036854775808, %r11
	cvttsd2siq	%xmm0, %r13
	xorq	%r11, %r13
.L317:
	movzbl	%r13b, %ebx
	movq	%rbx, %rsi
	movq	%rbx, %rbp
	salq	$8, %rsi
	salq	$16, %rbp
	addq	%rsi, %rbp
	addq	%rbp, %rbx
	cvtsi2sdq	%rbx, %xmm9
	movsd	%xmm9, 64(%rsp)
	jmp	.L213
.L518:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L568
	movl	$.LC90, %ecx
	movl	%r12d, %edx
	movl	$534, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L517:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L569
	movl	$.LC88, %ecx
	movl	%r12d, %edx
	movl	$522, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L568:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %r10
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %edx
	cltq
	movb	%dl, (%r10,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC79(%rip), %xmm10
	cvttsd2siq	%xmm0, %rcx
	comisd	%xmm10, %xmm0
	jb	.L313
	subsd	%xmm10, %xmm0
	movabsq	$-9223372036854775808, %r14
	cvttsd2siq	%xmm0, %rcx
	xorq	%r14, %rcx
.L313:
	movzbl	%cl, %edi
	salq	$16, %rdi
	cvtsi2sdq	%rdi, %xmm11
	movsd	%xmm11, 64(%rsp)
	jmp	.L213
.L569:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r11
	movl	%r12d, %edi
	movq	(%r11,%rbp,8), %rsi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	0(%r13), %ebx
	cltq
	movb	%bl, (%rsi,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC79(%rip), %xmm12
	cvttsd2siq	%xmm0, %rbp
	comisd	%xmm12, %xmm0
	jb	.L309
	subsd	%xmm12, %xmm0
	movabsq	$-9223372036854775808, %r8
	cvttsd2siq	%xmm0, %rbp
	xorq	%r8, %rbp
.L309:
	salq	$8, %rbp
	movzwl	%bp, %r9d
	cvtsi2sdq	%r9, %xmm13
	movsd	%xmm13, 64(%rsp)
	jmp	.L213
.L516:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r10
	addq	%rbp, %r10
	cmpb	$40, (%r10)
	je	.L570
	movl	$.LC86, %ecx
	movl	%r12d, %edx
	movl	$510, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L515:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rsi
	addq	%rbp, %rsi
	cmpb	$40, (%rsi)
	je	.L571
	movl	$.LC84, %ecx
	movl	%r12d, %edx
	movl	$498, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L570:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rdx
	movl	%r12d, %edi
	movq	(%rdx,%rbp,8), %rcx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r10), %r14d
	cltq
	movb	%r14b, (%rcx,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC79(%rip), %xmm14
	cvttsd2siq	%xmm0, %rdi
	comisd	%xmm14, %xmm0
	jb	.L305
	subsd	%xmm14, %xmm0
	movabsq	$-9223372036854775808, %r13
	cvttsd2siq	%xmm0, %rdi
	xorq	%r13, %rdi
.L305:
	movzbl	%dil, %r11d
	cvtsi2sdq	%r11, %xmm15
	movsd	%xmm15, 64(%rsp)
	jmp	.L213
.L571:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r8
	movl	%r12d, %edi
	movq	(%r8,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rsi), %ebx
	cltq
	movb	%bl, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC79(%rip), %xmm1
	cvttsd2siq	%xmm0, %r9
	comisd	%xmm1, %xmm0
	jb	.L301
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %r10
	cvttsd2siq	%xmm0, %r9
	xorq	%r10, %r9
.L301:
	andl	$16711680, %r9d
	shrq	$16, %r9
	cvtsi2sdq	%r9, %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L514:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L572
	movl	$.LC82, %ecx
	movl	%r12d, %edx
	movl	$486, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L513:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r11
	addq	%rbp, %r11
	cmpb	$40, (%r11)
	je	.L573
	movl	$.LC80, %ecx
	movl	%r12d, %edx
	movl	$474, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L572:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdx), %edi
	cltq
	movb	%dil, (%r14,%rax)
	xorl	%eax, %eax
	movl	%r12d, %edi
	call	exponential
	movsd	.LC79(%rip), %xmm3
	cvttsd2siq	%xmm0, %rbx
	comisd	%xmm3, %xmm0
	jb	.L297
	subsd	%xmm3, %xmm0
	movabsq	$-9223372036854775808, %r13
	cvttsd2siq	%xmm0, %rbx
	xorq	%r13, %rbx
.L297:
	movzbl	%bh, %esi
	cvtsi2sdq	%rsi, %xmm2
	movsd	%xmm2, 64(%rsp)
	jmp	.L213
.L573:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r8
	movl	%r12d, %edi
	movq	(%r8,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r11), %r9d
	cltq
	movb	%r9b, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC79(%rip), %xmm4
	cvttsd2siq	%xmm0, %r10
	comisd	%xmm4, %xmm0
	jb	.L293
	subsd	%xmm4, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %r10
	xorq	%rdx, %r10
.L293:
	movzbl	%r10b, %ecx
	cvtsi2sdq	%rcx, %xmm5
	movsd	%xmm5, 64(%rsp)
	jmp	.L213
.L512:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r14
	addq	%rbp, %r14
	cmpb	$40, (%r14)
	je	.L574
	movl	$.LC77, %ecx
	movl	%r12d, %edx
	movl	$462, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L511:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rsi
	addq	%rbp, %rsi
	cmpb	$40, (%rsi)
	je	.L575
	movl	$.LC75, %ecx
	movl	%r12d, %edx
	movl	$452, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L574:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rdi
	movq	(%rdi,%rbp,8), %r13
	movl	%r12d, %edi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r14), %ebx
	cltq
	movb	%bl, 0(%r13,%rax)
	xorl	%eax, %eax
	call	exponential
	call	udf1
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L575:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r11
	movl	%r12d, %edi
	movq	(%r11,%rbp,8), %r8
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rsi), %ebp
	cltq
	movb	%bpl, (%r8,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC68(%rip), %xmm6
	andpd	%xmm6, %xmm0
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L510:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r9
	addq	%rbp, %r9
	cmpb	$40, (%r9)
	je	.L576
	movl	$.LC73, %ecx
	movl	%r12d, %edx
	movl	$442, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L509:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdi
	addq	%rbp, %rdi
	cmpb	$40, (%rdi)
	je	.L577
	movl	$.LC69, %ecx
	movl	%r12d, %edx
	movl	$429, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L576:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r10
	movl	%r12d, %edi
	movq	(%r10,%rbp,8), %rcx
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r9), %edx
	cltq
	movb	%dl, (%rcx,%rax)
	xorl	%eax, %eax
	call	exponential
	xorpd	%xmm7, %xmm7
	comisd	%xmm7, %xmm0
	jbe	.L468
	movabsq	$4607182418800017408, %r14
	movq	%r14, 64(%rsp)
	jmp	.L213
.L577:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r13
	movq	0(%r13,%rbp,8), %rsi
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdi), %ebx
	cltq
	movl	%r12d, %edi
	movb	%bl, (%rsi,%rax)
	xorl	%eax, %eax
	call	exponential
	movsd	.LC68(%rip), %xmm9
	movsd	.LC67(%rip), %xmm10
	andpd	%xmm0, %xmm9
	ucomisd	%xmm9, %xmm10
	jbe	.L279
	cvttsd2siq	%xmm0, %r11
	movsd	.LC9(%rip), %xmm12
	cvtsi2sdq	%r11, %xmm11
	cmpnlesd	%xmm11, %xmm0
	andpd	%xmm12, %xmm0
	addsd	%xmm11, %xmm0
.L279:
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L468:
	movsd	.LC72(%rip), %xmm8
	cmpltsd	%xmm7, %xmm0
	andpd	%xmm0, %xmm8
	movsd	%xmm8, 64(%rsp)
	jmp	.L213
.L508:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L578
	movl	$.LC65, %ecx
	movl	%r12d, %edx
	movl	$419, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L507:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %rdx
	addq	%rbp, %rdx
	cmpb	$40, (%rdx)
	je	.L579
	movl	$.LC63, %ecx
	movl	%r12d, %edx
	movl	$409, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L578:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %r10d
	cltq
	movb	%r10b, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	call	cbrt
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L579:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rcx
	movq	(%rcx,%rbp,8), %r14
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%rdx), %edi
	cltq
	movb	%dil, (%r14,%rax)
	movl	%r12d, %edi
	xorl	%eax, %eax
	call	exponential
	sqrtsd	%xmm0, %xmm13
	movsd	%xmm13, 64(%rsp)
	jmp	.L213
.L506:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r13
	addq	%rbp, %r13
	cmpb	$40, 0(%r13)
	je	.L580
	movl	$.LC61, %ecx
	movl	%r12d, %edx
	movl	$399, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L505:
	movl	%r12d, %edi
	call	skipblanks
	movq	ch(%rip), %r8
	addq	%rbp, %r8
	cmpb	$40, (%r8)
	je	.L581
	movl	$.LC59, %ecx
	movl	%r12d, %edx
	movl	$389, %esi
	movl	$.LC8, %edi
	call	exception
	jmp	.L213
.L580:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %rsi
	movl	%r12d, %edi
	movq	(%rsi,%rbp,8), %r11
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	0(%r13), %ebx
	cltq
	movb	%bl, (%r11,%rax)
	xorl	%eax, %eax
	call	exponential
	call	log10
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
.L581:
	addq	position(%rip), %rbx
	movq	buffer(%rip), %r9
	movl	%r12d, %edi
	movq	(%r9,%rbp,8), %rbp
	movl	(%rbx), %eax
	subl	$1, %eax
	movl	%eax, (%rbx)
	movzbl	(%r8), %r10d
	cltq
	movb	%r10b, 0(%rbp,%rax)
	xorl	%eax, %eax
	call	exponential
	call	log
	movsd	%xmm0, 64(%rsp)
	jmp	.L213
	.cfi_endproc
.LFE45:
	.size	factor, .-factor
	.section	.rodata.str1.8
	.align 8
.LC138:
	.string	"Garbage in function expression.\n"
	.align 8
.LC139:
	.string	"Value returned MAY BE invalid, exceptions occured.\n"
	.text
	.p2align 4,,15
	.globl	fpar_f
	.type	fpar_f, @function
fpar_f:
.LFB49:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movl	%edi, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movslq	%edi, %rbx
	leaq	0(,%rbx,8), %r11
	subq	$40, %rsp
	.cfi_def_cfa_offset 80
	movq	gcnt(%rip), %r13
	movq	t(%rip), %rdx
	testq	%rsi, %rsi
	leaq	1(%r13), %rax
	movsd	%xmm0, (%rdx,%rbx,8)
	movq	%rax, gcnt(%rip)
	je	.L661
	movq	ranks(%rip), %r10
	movq	nvar(%rip), %r8
	leaq	0(,%rbx,4), %r12
	movl	(%r10,%rbx,4), %eax
	movl	(%r8,%rbx,4), %r9d
	testl	%eax, %eax
	jle	.L584
	addq	y(%rip), %r11
	subl	$1, %eax
	xorl	%r8d, %r8d
	leaq	8(,%rax,8), %r10
	.p2align 4,,10
.L585:
	testl	%r9d, %r9d
	jle	.L587
	movq	(%r11), %rdx
	movq	(%rsi,%r8), %rcx
	leal	-1(%r9), %edi
	movl	$1, %eax
	andl	$7, %edi
	cmpl	$1, %r9d
	movq	(%rdx,%r8), %rdx
	movsd	(%rcx), %xmm0
	movsd	%xmm0, (%rdx)
	jle	.L587
	testl	%edi, %edi
	je	.L588
	cmpl	$1, %edi
	je	.L639
	cmpl	$2, %edi
	.p2align 4,,3
	je	.L640
	cmpl	$3, %edi
	.p2align 4,,2
	je	.L641
	cmpl	$4, %edi
	.p2align 4,,2
	je	.L642
	cmpl	$5, %edi
	.p2align 4,,2
	je	.L643
	cmpl	$6, %edi
	.p2align 4,,2
	je	.L644
	movsd	8(%rcx), %xmm1
	movl	$2, %eax
	movsd	%xmm1, 8(%rdx)
.L644:
	movsd	(%rcx,%rax,8), %xmm2
	movsd	%xmm2, (%rdx,%rax,8)
	addq	$1, %rax
.L643:
	movsd	(%rcx,%rax,8), %xmm3
	movsd	%xmm3, (%rdx,%rax,8)
	addq	$1, %rax
.L642:
	movsd	(%rcx,%rax,8), %xmm4
	movsd	%xmm4, (%rdx,%rax,8)
	addq	$1, %rax
.L641:
	movsd	(%rcx,%rax,8), %xmm5
	movsd	%xmm5, (%rdx,%rax,8)
	addq	$1, %rax
.L640:
	movsd	(%rcx,%rax,8), %xmm6
	movsd	%xmm6, (%rdx,%rax,8)
	addq	$1, %rax
.L639:
	movsd	(%rcx,%rax,8), %xmm7
	movsd	%xmm7, (%rdx,%rax,8)
	addq	$1, %rax
	cmpl	%eax, %r9d
	jle	.L587
.L588:
	movsd	(%rcx,%rax,8), %xmm8
	leaq	1(%rax), %rdi
	movsd	%xmm8, (%rdx,%rax,8)
	movsd	(%rcx,%rdi,8), %xmm9
	movsd	%xmm9, (%rdx,%rdi,8)
	leaq	2(%rax), %rdi
	movsd	(%rcx,%rdi,8), %xmm10
	movsd	%xmm10, (%rdx,%rdi,8)
	leaq	3(%rax), %rdi
	movsd	(%rcx,%rdi,8), %xmm11
	movsd	%xmm11, (%rdx,%rdi,8)
	leaq	4(%rax), %rdi
	movsd	(%rcx,%rdi,8), %xmm12
	movsd	%xmm12, (%rdx,%rdi,8)
	leaq	5(%rax), %rdi
	movsd	(%rcx,%rdi,8), %xmm13
	movsd	%xmm13, (%rdx,%rdi,8)
	leaq	6(%rax), %rdi
	movsd	(%rcx,%rdi,8), %xmm14
	movsd	%xmm14, (%rdx,%rdi,8)
	leaq	7(%rax), %rdi
	addq	$8, %rax
	cmpl	%eax, %r9d
	movsd	(%rcx,%rdi,8), %xmm15
	movsd	%xmm15, (%rdx,%rdi,8)
	jg	.L588
.L587:
	addq	$8, %r8
	cmpq	%r10, %r8
	jne	.L585
.L584:
	movq	position(%rip), %rsi
	movq	ch(%rip), %rdx
	addq	$3, %r13
	movl	%ebp, %edi
	xorl	%eax, %eax
	movq	%r13, gcnt(%rip)
	movl	$0, (%rsi,%r12)
	movb	$0, (%rdx,%rbx)
	call	exponential
	movsd	%xmm0, 24(%rsp)
.L654:
	movq	ch(%rip), %rcx
	movzbl	(%rcx,%rbx), %edi
	cmpb	$42, %dil
	je	.L592
	cmpb	$47, %dil
	je	.L662
.L653:
	cmpb	$43, %dil
	je	.L597
.L666:
	cmpb	$45, %dil
	je	.L663
	cmpb	$59, %dil
	jne	.L664
.L613:
	movq	err(%rip), %r9
	movl	(%r9,%r12), %r10d
	testl	%r10d, %r10d
	jne	.L611
.L612:
	movsd	24(%rsp), %xmm0
	addq	$40, %rsp
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
	.p2align 4,,10
.L592:
	.cfi_restore_state
	movl	%ebp, %edi
	xorl	%eax, %eax
	call	exponential
	mulsd	24(%rsp), %xmm0
	movsd	%xmm0, 24(%rsp)
	jmp	.L654
	.p2align 4,,10
.L663:
	movl	%ebp, %edi
	xorl	%eax, %eax
	addq	$1, gcnt(%rip)
	call	exponential
	movapd	%xmm0, %xmm1
.L659:
	movq	ch(%rip), %r13
	movzbl	0(%r13,%rbx), %edi
	cmpb	$42, %dil
	je	.L607
	cmpb	$47, %dil
	je	.L665
	movsd	24(%rsp), %xmm0
	cmpb	$43, %dil
	subsd	%xmm1, %xmm0
	movsd	%xmm0, 24(%rsp)
	jne	.L666
.L597:
	movl	%ebp, %edi
	xorl	%eax, %eax
	addq	$1, gcnt(%rip)
	call	exponential
	movapd	%xmm0, %xmm2
.L657:
	movq	ch(%rip), %rsi
	movzbl	(%rsi,%rbx), %edi
	cmpb	$42, %dil
	je	.L601
	cmpb	$47, %dil
	je	.L667
	addsd	24(%rsp), %xmm2
	movsd	%xmm2, 24(%rsp)
	jmp	.L653
	.p2align 4,,10
.L662:
	movl	%ebp, %edi
	xorl	%eax, %eax
	call	exponential
	movsd	24(%rsp), %xmm3
	divsd	%xmm0, %xmm3
	movsd	%xmm3, 24(%rsp)
	jmp	.L654
	.p2align 4,,10
.L667:
	movl	%ebp, %edi
	xorl	%eax, %eax
	movsd	%xmm2, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm2
	divsd	%xmm0, %xmm2
	jmp	.L657
	.p2align 4,,10
.L601:
	movl	%ebp, %edi
	xorl	%eax, %eax
	movsd	%xmm2, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm2
	mulsd	%xmm0, %xmm2
	jmp	.L657
	.p2align 4,,10
.L665:
	movl	%ebp, %edi
	xorl	%eax, %eax
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	divsd	%xmm0, %xmm1
	jmp	.L659
	.p2align 4,,10
.L607:
	movl	%ebp, %edi
	xorl	%eax, %eax
	movsd	%xmm1, (%rsp)
	call	exponential
	movsd	(%rsp), %xmm1
	mulsd	%xmm0, %xmm1
	jmp	.L659
	.p2align 4,,10
.L664:
	cmpb	$10, %dil
	je	.L613
	movl	$.LC138, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %rax
	movl	%ebp, %esi
	movl	$.LC1, %edi
	movq	(%rax,%rbx,8), %rdx
	xorl	%eax, %eax
	call	printf
	movl	$993, %edx
	movl	$.LC8, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %r8
	movl	$1, (%r8,%r12)
.L611:
	movl	$.LC139, %esi
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	movq	buffer(%rip), %r11
	movl	%ebp, %esi
	movl	$.LC1, %edi
	xorl	%eax, %eax
	movq	(%r11,%rbx,8), %rdx
	call	printf
	movl	$996, %edx
	movl	$.LC8, %esi
	movl	$.LC2, %edi
	xorl	%eax, %eax
	call	printf
	movq	err(%rip), %rbx
	movl	$1, (%rbx,%r12)
	jmp	.L612
	.p2align 4,,10
.L661:
	movq	y(%rip), %rcx
	leaq	0(,%rbx,4), %r12
	movq	(%rcx,%rbx,8), %rsi
	movq	(%rsi), %rdi
	movq	$0, (%rdi)
	jmp	.L584
	.cfi_endproc
.LFE49:
	.size	fpar_f, .-fpar_f
	.p2align 4,,15
	.globl	fpar_ok
	.type	fpar_ok, @function
fpar_ok:
.LFB38:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movslq	%edi, %rdx
	xorl	%r15d, %r15d
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
	subq	$72, %rsp
	.cfi_def_cfa_offset 128
	movq	ranks(%rip), %rcx
	movq	nvar(%rip), %rax
	movl	%edi, 60(%rsp)
	movq	%rdx, 48(%rsp)
	movl	(%rcx,%rdx,4), %r12d
	movl	(%rax,%rdx,4), %r14d
	movslq	%r12d, %rdi
	movslq	%r14d, %r13
	salq	$3, %rdi
	salq	$3, %r13
	call	malloc
	testl	%r12d, %r12d
	movq	%rax, %rbp
	movdqa	.LC140(%rip), %xmm4
	jle	.L672
	.p2align 4,,10
.L698:
	movq	%r13, %rdi
	movl	%r15d, %ebx
	movdqa	%xmm4, 16(%rsp)
	call	malloc
	testl	%r14d, %r14d
	movq	%rax, 0(%rbp,%r15,8)
	movdqa	16(%rsp), %xmm4
	jle	.L682
	movq	%rax, %rdi
	movq	%rax, %rsi
	salq	$60, %rdi
	shrq	$63, %rdi
	cmpl	%edi, %r14d
	cmovb	%r14d, %edi
	cmpl	$6, %r14d
	ja	.L709
	movl	%r14d, %edi
.L673:
	movl	%r15d, %r8d
	xorl	%r9d, %r9d
	xorl	%edx, %edx
	.p2align 4,,10
.L676:
	cvtsi2sd	%r9d, %xmm0
	addl	$1, %edx
	addl	%r8d, %r9d
	movsd	%xmm0, (%rsi)
	addq	$8, %rsi
	cmpl	%edi, %edx
	jne	.L676
	cmpl	%edx, %r14d
	je	.L682
.L674:
	movl	%r14d, %r11d
	movl	%edi, %ecx
	subl	%edi, %r11d
	movl	%r11d, %edi
	shrl	$2, %edi
	leal	0(,%rdi,4), %r10d
	testl	%r10d, %r10d
	je	.L678
	movl	%edx, 12(%rsp)
	leal	1(%rdx), %esi
	leal	2(%rdx), %r8d
	movd	12(%rsp), %xmm0
	leal	3(%rdx), %r9d
	movl	%ebx, 12(%rsp)
	movd	12(%rsp), %xmm5
	movl	%esi, 40(%rsp)
	xorl	%esi, %esi
	movl	%r8d, 44(%rsp)
	movl	%r9d, 56(%rsp)
	leal	-1(%rdi), %r8d
	movd	44(%rsp), %xmm1
	pshufd	$0, %xmm5, %xmm7
	movd	56(%rsp), %xmm2
	andl	$3, %r8d
	leaq	(%rax,%rcx,8), %rcx
	movd	40(%rsp), %xmm3
	punpckldq	%xmm2, %xmm1
	movdqa	%xmm7, %xmm8
	punpckldq	%xmm3, %xmm0
	psrlq	$32, %xmm8
	punpcklqdq	%xmm1, %xmm0
	je	.L683
	movdqa	%xmm0, %xmm9
	movl	$1, %esi
	movdqa	%xmm7, %xmm6
	psrlq	$32, %xmm9
	pmuludq	%xmm8, %xmm9
	pshufd	$8, %xmm9, %xmm11
	pmuludq	%xmm0, %xmm6
	pshufd	$8, %xmm6, %xmm10
	paddd	%xmm4, %xmm0
	punpckldq	%xmm11, %xmm10
	pshufd	$238, %xmm10, %xmm13
	cvtdq2pd	%xmm10, %xmm12
	movapd	%xmm12, (%rcx)
	cvtdq2pd	%xmm13, %xmm14
	movapd	%xmm14, 16(%rcx)
	addq	$32, %rcx
	cmpl	$1, %r8d
	je	.L683
	cmpl	$2, %r8d
	je	.L699
	movdqa	%xmm0, %xmm1
	movb	$2, %sil
	movdqa	%xmm7, %xmm15
	psrlq	$32, %xmm1
	pmuludq	%xmm8, %xmm1
	pshufd	$8, %xmm1, %xmm3
	pmuludq	%xmm0, %xmm15
	pshufd	$8, %xmm15, %xmm2
	paddd	%xmm4, %xmm0
	punpckldq	%xmm3, %xmm2
	pshufd	$238, %xmm2, %xmm6
	cvtdq2pd	%xmm2, %xmm5
	movapd	%xmm5, (%rcx)
	cvtdq2pd	%xmm6, %xmm9
	movapd	%xmm9, 16(%rcx)
	addq	$32, %rcx
.L699:
	movdqa	%xmm0, %xmm11
	addl	$1, %esi
	movdqa	%xmm7, %xmm10
	psrlq	$32, %xmm11
	pmuludq	%xmm8, %xmm11
	pshufd	$8, %xmm11, %xmm13
	pmuludq	%xmm0, %xmm10
	pshufd	$8, %xmm10, %xmm12
	paddd	%xmm4, %xmm0
	punpckldq	%xmm13, %xmm12
	pshufd	$238, %xmm12, %xmm15
	cvtdq2pd	%xmm12, %xmm14
	movapd	%xmm14, (%rcx)
	cvtdq2pd	%xmm15, %xmm1
	movapd	%xmm1, 16(%rcx)
	addq	$32, %rcx
	jmp	.L683
	.p2align 4,,10
.L679:
	movdqa	%xmm7, %xmm12
	addl	$4, %esi
	movdqa	%xmm2, %xmm11
	pmuludq	%xmm2, %xmm12
	psrlq	$32, %xmm2
	pshufd	$8, %xmm12, %xmm13
	pmuludq	%xmm8, %xmm2
	pshufd	$8, %xmm2, %xmm14
	paddd	%xmm4, %xmm11
	movdqa	%xmm11, %xmm0
	movdqa	%xmm7, %xmm3
	movdqa	%xmm7, %xmm12
	punpckldq	%xmm14, %xmm13
	paddd	%xmm4, %xmm0
	pmuludq	%xmm11, %xmm3
	pmuludq	%xmm0, %xmm12
	pshufd	$8, %xmm3, %xmm9
	psrlq	$32, %xmm11
	pshufd	$8, %xmm12, %xmm14
	pmuludq	%xmm8, %xmm11
	cvtdq2pd	%xmm13, %xmm15
	pshufd	$238, %xmm13, %xmm1
	movdqa	%xmm0, %xmm13
	movapd	%xmm15, 32(%rcx)
	paddd	%xmm4, %xmm0
	pshufd	$8, %xmm11, %xmm5
	psrlq	$32, %xmm13
	pmuludq	%xmm8, %xmm13
	pshufd	$8, %xmm13, %xmm15
	cvtdq2pd	%xmm1, %xmm2
	movapd	%xmm2, 16(%r9)
	punpckldq	%xmm5, %xmm9
	punpckldq	%xmm15, %xmm14
	pshufd	$238, %xmm9, %xmm10
	cvtdq2pd	%xmm9, %xmm6
	pshufd	$238, %xmm14, %xmm2
	cvtdq2pd	%xmm14, %xmm1
	movapd	%xmm6, 64(%rcx)
	cvtdq2pd	%xmm10, %xmm11
	movapd	%xmm1, 96(%rcx)
	cvtdq2pd	%xmm2, %xmm3
	movapd	%xmm11, 80(%rcx)
	movapd	%xmm3, 112(%rcx)
	subq	$-128, %rcx
.L683:
	movdqa	%xmm7, %xmm3
	leal	1(%rsi), %r8d
	movdqa	%xmm0, %xmm2
	leaq	32(%rcx), %r9
	pmuludq	%xmm0, %xmm3
	psrlq	$32, %xmm0
	pshufd	$8, %xmm3, %xmm5
	pmuludq	%xmm8, %xmm0
	pshufd	$8, %xmm0, %xmm0
	cmpl	%edi, %r8d
	paddd	%xmm4, %xmm2
	punpckldq	%xmm0, %xmm5
	pshufd	$238, %xmm5, %xmm9
	cvtdq2pd	%xmm5, %xmm6
	movapd	%xmm6, (%rcx)
	cvtdq2pd	%xmm9, %xmm10
	movapd	%xmm10, 16(%rcx)
	jb	.L679
	addl	%r10d, %edx
	cmpl	%r10d, %r11d
	je	.L682
.L678:
	movslq	%edx, %rdi
	movl	%r15d, %r11d
	imull	%edx, %ebx
	leaq	(%rax,%rdi,8), %rax
	.p2align 4,,10
.L681:
	cvtsi2sd	%ebx, %xmm7
	addl	$1, %edx
	addl	%r11d, %ebx
	movsd	%xmm7, (%rax)
	addq	$8, %rax
	cmpl	%edx, %r14d
	jg	.L681
.L682:
	addq	$1, %r15
	cmpl	%r15d, %r12d
	jg	.L698
.L672:
	movl	60(%rsp), %edi
	movsd	.LC9(%rip), %xmm0
	movq	%rbp, %rsi
	call	fpar_f
	movq	%rbp, %rdi
	call	free
	movq	err(%rip), %r14
	movq	48(%rsp), %r12
	xorl	%eax, %eax
	movl	(%r14,%r12,4), %ebp
	testl	%ebp, %ebp
	sete	%al
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
.L709:
	.cfi_restore_state
	testl	%edi, %edi
	jne	.L673
	xorl	%edx, %edx
	jmp	.L674
	.cfi_endproc
.LFE38:
	.size	fpar_ok, .-fpar_ok
	.comm	N,4,4
	.comm	ranks,8,8
	.comm	nvar,8,8
	.comm	err,8,8
	.comm	t,8,8
	.comm	y,8,8
	.comm	maxpos,8,8
	.comm	ch,8,8
	.comm	position,8,8
	.comm	buffer,8,8
	.local	gcnt
	.comm	gcnt,8,16
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC9:
	.long	0
	.long	1072693248
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC10:
	.long	0
	.long	-2147483648
	.long	0
	.long	0
	.section	.rodata.cst8
	.align 8
.LC67:
	.long	0
	.long	1127219200
	.section	.rodata.cst16
	.align 16
.LC68:
	.long	4294967295
	.long	2147483647
	.long	0
	.long	0
	.section	.rodata.cst8
	.align 8
.LC72:
	.long	0
	.long	-1074790400
	.align 8
.LC79:
	.long	0
	.long	1138753536
	.section	.rodata.cst16
	.align 16
.LC140:
	.long	4
	.long	4
	.long	4
	.long	4
	.ident	"GCC: (FreeBSD Ports Collection) 4.8.0 20120805 (experimental)"
	.section	.note.GNU-stack,"",@progbits
