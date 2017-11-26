	.file	"msi4.c"
	.text
	.p2align 2,,3
.globl debug
	.type	debug, @function
debug:
.LFB4:
	movzbl	%al, %edx 
	subq	$64, %rsp
.LCFI0:
	leaq	0(,%rdx,4), %rax
	movl	$.L2, %edx
	subq	%rax, %rdx
	leaq	55(%rsp), %rax
	jmp	*%rdx
	movaps	%xmm7, -15(%rax)
	movaps	%xmm6, -31(%rax)
	movaps	%xmm5, -47(%rax)
	movaps	%xmm4, -63(%rax)
	movaps	%xmm3, -79(%rax)
	movaps	%xmm2, -95(%rax)
	movaps	%xmm1, -111(%rax)
	movaps	%xmm0, -127(%rax)
.L2:
	xorl	%eax, %eax
	addq	$64, %rsp
.LCFI1:
	ret
.LFE4:
	.size	debug, .-debug
	.p2align 2,,3
.globl mindebug
	.type	mindebug, @function
mindebug:
.LFB6:
	movzbl	%al, %edx 
	subq	$64, %rsp
.LCFI2:
	leaq	0(,%rdx,4), %rax
	movl	$.L4, %edx
	subq	%rax, %rdx
	leaq	55(%rsp), %rax
	jmp	*%rdx
	movaps	%xmm7, -15(%rax)
	movaps	%xmm6, -31(%rax)
	movaps	%xmm5, -47(%rax)
	movaps	%xmm4, -63(%rax)
	movaps	%xmm3, -79(%rax)
	movaps	%xmm2, -95(%rax)
	movaps	%xmm1, -111(%rax)
	movaps	%xmm0, -127(%rax)
.L4:
	xorl	%eax, %eax
	addq	$64, %rsp
.LCFI3:
	ret
.LFE6:
	.size	mindebug, .-mindebug
	.p2align 2,,3
.globl mgetch
	.type	mgetch, @function
mgetch:
.LFB8:
	pushq	%rbp
.LCFI4:
	pushq	%rbx
.LCFI5:
	subq	$104, %rsp
.LCFI6:
	movq	__stdinp(%rip), %rdi
	call	fileno
	leaq	48(%rsp), %rbp
	movl	%eax, %edi
	movq	%rbp, %rsi
	call	tcgetattr
	cld
	movq	%rsp, %rdi
	movl	$5, %ecx
	movq	%rbp, %rsi
	rep
	movsq
	movsl
	movq	__stdinp(%rip), %rdi
	andl	$-265, 12(%rsp)
	movb	$1, 32(%rsp)
	movb	$0, 33(%rsp)
	call	fileno
	movl	%eax, %edi
	movq	%rsp, %rdx
	movl	$2, %esi
	call	tcsetattr
	xorl	%edx, %edx
	testl	%eax, %eax
	je	.L7
.L5:
	addq	$104, %rsp
.LCFI7:
	popq	%rbx
.LCFI8:
	movl	%edx, %eax
	popq	%rbp
.LCFI9:
	ret
	.p2align 2,,3
.L7:
	call	getchar
	cmpl	$10, %eax
	movl	%eax, %ebx
	je	.L7
	movq	__stdinp(%rip), %rdi
	call	fileno
	movl	%eax, %edi
	movq	%rbp, %rdx
	movl	$2, %esi
	call	tcsetattr
	movl	%ebx, %edx
	jmp	.L5
.LFE8:
	.size	mgetch, .-mgetch
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"r"
.LC3:
	.string	"/dev/urandom"
	.section	.rodata.str1.32,"aMS",@progbits,1
	.p2align 5
.LC4:
	.string	"RANLIB Ooops:\nCannot open device: /dev/urandom"
	.text
	.p2align 2,,3
.globl Randomize
	.type	Randomize, @function
Randomize:
.LFB10:
	movl	$.LC2, %esi
	pushq	%rcx
.LCFI10:
	movl	$.LC3, %edi
	call	fopen
	testq	%rax, %rax
	movq	%rax, _ran_device(%rip)
	movl	$1, %edx
	je	.L12
.L9:
	movl	%edx, %eax
	popq	%rdx
.LCFI11:
	ret
	.p2align 2,,3
.L12:
	movl	$.LC4, %edi
	xorl	%eax, %eax
	call	printf
	xorl	%edx, %edx
	jmp	.L9
.LFE10:
	.size	Randomize, .-Randomize
	.p2align 2,,3
.globl Random
	.type	Random, @function
Random:
.LFB12:
	pushq	%r12
.LCFI12:
	pushq	%rbp
.LCFI13:
	pushq	%rbx
.LCFI14:
	subq	$32, %rsp
.LCFI15:
	movl	%edi, %r12d
	leaq	16(%rsp), %rbp
	movl	$3, %ebx
	.p2align 2,,3
.L18:
	movq	_ran_device(%rip), %rdi
	call	fgetc
	movl	%eax, (%rbp)
	addq	$4, %rbp
	decl	%ebx
	jns	.L18
	movl	28(%rsp), %esi
	sall	$8, %esi
	addl	24(%rsp), %esi
	sall	$8, %esi
	addl	20(%rsp), %esi
	sall	$8, %esi
	movl	$-1, %ecx
	addl	16(%rsp), %esi
	movl	%ecx, %eax
	addq	$32, %rsp
.LCFI16:
	xorl	%edx, %edx
	divl	%r12d
	popq	%rbx
.LCFI17:
	movl	%eax, %edi
	xorl	%edx, %edx
	movl	%esi, %eax
	popq	%rbp
.LCFI18:
	divl	%edi
	popq	%r12
.LCFI19:
	ret
.LFE12:
	.size	Random, .-Random
	.p2align 2,,3
.globl Kill_random
	.type	Kill_random, @function
Kill_random:
.LFB14:
	movq	_ran_device(%rip), %rdi
	testq	%rdi, %rdi
	jne	.L23
	ret
	.p2align 2,,3
.L23:
	jmp	fclose
.LFE14:
	.size	Kill_random, .-Kill_random
	.local	selected
	.comm	selected,4,4
	.local	done
	.comm	done,4,4
	.local	end_cond
	.comm	end_cond,4,4
	.local	cpu_wins
	.comm	cpu_wins,4,4
	.local	hum_wins
	.comm	hum_wins,4,4
	.section	.rodata.str1.1
.LC8:
	.string	"\n***********************"
.LC9:
	.string	"\n\nHUMAN/CPU: %d/%d\n\n"
.LC10:
	.string	"GRATULACJE WYGRALES(AS)"
	.section	.rodata.str1.32
	.p2align 5
.LC11:
	.string	"CPU WYGRAL 5 RAZY WIECEJ NIZ TY"
	.text
	.p2align 2,,3
.globl stats
	.type	stats, @function
stats:
.LFB16:
	pushq	%rdi
.LCFI20:
	movl	$.LC8, %edi
	call	puts
	movl	cpu_wins(%rip), %edx
	movl	hum_wins(%rip), %esi
	movl	$.LC9, %edi
	xorl	%eax, %eax
	call	printf
	movl	$.LC8, %edi
	call	puts
	movl	hum_wins(%rip), %eax
	subl	cpu_wins(%rip), %eax
	cmpl	$5, %eax
	movl	$.LC10, %edi
	jg	.L27
	cmpl	$-5, %eax
	jl	.L28
	popq	%rsi
.LCFI21:
	ret
.L28:
	movl	$.LC11, %edi
.L27:
	call	puts
	xorl	%edi, %edi
	call	exit
.LFE16:
	.size	stats, .-stats
	.p2align 2,,3
.globl init
	.type	init, @function
init:
.LFB18:
	movl	$board, %edx
	movl	$63, %eax
	.p2align 2,,3
.L34:
	movl	$0, (%rdx)
	addq	$4, %rdx
	decl	%eax
	jns	.L34
	ret
.LFE18:
	.size	init, .-init
	.p2align 2,,3
.globl check
	.type	check, @function
check:
.LFB20:
	movl	selected(%rip), %eax
	testl	%eax, %eax
	js	.L40
.L38:
	cmpl	$63, %eax
	jle	.L37
	subl	$64, %eax
	movl	%eax, selected(%rip)
.L37:
	ret
	.p2align 2,,3
.L40:
	addl	$64, %eax
	movl	%eax, selected(%rip)
	jmp	.L38
.LFE20:
	.size	check, .-check
	.p2align 2,,3
.globl move_x
	.type	move_x, @function
move_x:
.LFB22:
	movl	selected(%rip), %ecx
	cmpl	$-1, %ecx
	leal	3(%rcx), %esi
	movl	%ecx, %eax
	cmovle	%esi, %eax
	andl	$-4, %eax
	movl	%ecx, %r8d
	subl	%eax, %r8d
	testl	%edi, %edi
	jle	.L43
	leal	1(%rcx), %edx
	cmpl	$-1, %edx
	leal	4(%rcx), %eax
	cmovg	%edx, %eax
	andl	$-4, %eax
	movl	%edx, selected(%rip)
	subl	%eax, %edx
	cmpl	%r8d, %edx
	jge	.L41
	leal	-3(%rcx), %eax
	movl	%eax, selected(%rip)
	ret
	.p2align 2,,3
.L41:
	ret
	.p2align 2,,3
.L43:
	leal	-1(%rcx), %edx
	testl	%edx, %edx
	movl	%edx, selected(%rip)
	js	.L50
	movl	%edx, %eax
	andl	$-4, %eax
	subl	%eax, %edx
	cmpl	%r8d, %edx
	movl	selected(%rip), %eax
	cmovg	%esi, %eax
	movl	%eax, selected(%rip)
	jmp	.L41
.L50:
	movl	%esi, selected(%rip)
	ret
.LFE22:
	.size	move_x, .-move_x
	.p2align 2,,3
.globl move_y
	.type	move_y, @function
move_y:
.LFB24:
	movl	selected(%rip), %esi
	cmpl	$-1, %esi
	leal	3(%rsi), %eax
	cmovg	%esi, %eax
	sarl	$2, %eax
	cmpl	$-1, %eax
	leal	3(%rax), %edx
	cmovg	%eax, %edx
	andl	$-4, %edx
	movl	%eax, %r8d
	subl	%edx, %r8d
	testl	%edi, %edi
	jle	.L54
	leal	4(%rsi), %eax
	cmpl	$-1, %eax
	leal	7(%rsi), %edx
	cmovg	%eax, %edx
	sarl	$2, %edx
	cmpl	$-1, %edx
	movl	%eax, selected(%rip)
	leal	3(%rdx), %eax
	cmovg	%edx, %eax
	andl	$-4, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	cmpl	%r8d, %ecx
	leal	-12(%rsi), %eax
	jl	.L63
.L51:
	ret
	.p2align 2,,3
.L63:
	movl	%eax, selected(%rip)
	ret
	.p2align 2,,3
.L54:
	leal	-4(%rsi), %eax
	testl	%eax, %eax
	movl	%eax, selected(%rip)
	js	.L64
	sarl	$2, %eax
	cmpl	$-1, %eax
	leal	3(%rax), %edx
	cmovg	%eax, %edx
	andl	$-4, %edx
	movl	%eax, %ecx
	subl	%edx, %ecx
	cmpl	%r8d, %ecx
	jle	.L51
	leal	12(%rsi), %eax
	movl	%eax, selected(%rip)
	jmp	.L51
.L64:
	leal	12(%rsi), %eax
	jmp	.L63
.LFE24:
	.size	move_y, .-move_y
	.p2align 2,,3
.globl move_z
	.type	move_z, @function
move_z:
.LFB26:
	testl	%edi, %edi
	movl	selected(%rip), %edx
	jle	.L67
	leal	16(%rdx), %eax
	cmpl	$63, %eax
	movl	%eax, selected(%rip)
	jle	.L65
	leal	-48(%rdx), %eax
	movl	%eax, selected(%rip)
	ret
	.p2align 2,,3
.L65:
	ret
	.p2align 2,,3
.L67:
	leal	-16(%rdx), %eax
	testl	%eax, %eax
	movl	%eax, selected(%rip)
	jns	.L65
	leal	48(%rdx), %eax
	movl	%eax, selected(%rip)
	jmp	.L65
.LFE26:
	.size	move_z, .-move_z
	.p2align 2,,3
.globl set_at
	.type	set_at, @function
set_at:
.LFB28:
	leal	(%rsi,%rdx,4), %edx
	leal	(%rdi,%rdx,4), %edx
	movslq	%edx,%rdx
	movl	%ecx, board(,%rdx,4)
	ret
.LFE28:
	.size	set_at, .-set_at
	.p2align 2,,3
.globl get_at
	.type	get_at, @function
get_at:
.LFB30:
	leal	(%rsi,%rdx,4), %edx
	leal	(%rdi,%rdx,4), %edx
	movslq	%edx,%rdx
	movl	board(,%rdx,4), %eax
	ret
.LFE30:
	.size	get_at, .-get_at
	.p2align 2,,3
.globl heuristic_count_moves_to_win
	.type	heuristic_count_moves_to_win, @function
heuristic_count_moves_to_win:
.LFB32:
	pushq	%r15
.LCFI22:
	pushq	%r14
.LCFI23:
	pushq	%r13
.LCFI24:
	pushq	%r12
.LCFI25:
	pushq	%rbp
.LCFI26:
	pushq	%rbx
.LCFI27:
	subq	$24, %rsp
.LCFI28:
	movl	%edi, %r14d
	movl	$5, %r15d
	xorl	%r13d, %r13d
.L96:
	xorl	%ebp, %ebp
	.p2align 2,,3
.L95:
	xorl	%r12d, %r12d
	movq	%rsp, %rbx
	.p2align 2,,3
.L86:
	movl	%r12d, %edx
	movl	%ebp, %esi
	movl	%r13d, %edi
	call	get_at
	incl	%r12d
	movl	%eax, (%rbx)
	addq	$4, %rbx
	cmpl	$3, %r12d
	jle	.L86
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L93:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L91
	cmpl	%r14d, %eax
	jne	.L275
.L91:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L93
.L88:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	incl	%ebp
	cmpl	$3, %ebp
	jle	.L95
	incl	%r13d
	cmpl	$3, %r13d
	jle	.L96
	xorl	%r13d, %r13d
.L119:
	xorl	%r12d, %r12d
	.p2align 2,,3
.L118:
	xorl	%ebp, %ebp
	movq	%rsp, %rbx
	.p2align 2,,3
.L109:
	movl	%ebp, %esi
	movl	%r12d, %edx
	movl	%r13d, %edi
	call	get_at
	incl	%ebp
	movl	%eax, (%rbx)
	addq	$4, %rbx
	cmpl	$3, %ebp
	jle	.L109
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L116:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L114
	cmpl	%r14d, %eax
	jne	.L276
.L114:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L116
.L111:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	incl	%r12d
	cmpl	$3, %r12d
	jle	.L118
	incl	%r13d
	cmpl	$3, %r13d
	jle	.L119
	xorl	%r12d, %r12d
.L142:
	xorl	%ebp, %ebp
	.p2align 2,,3
.L141:
	xorl	%r13d, %r13d
	movq	%rsp, %rbx
	.p2align 2,,3
.L132:
	movl	%r13d, %edi
	movl	%r12d, %edx
	movl	%ebp, %esi
	call	get_at
	incl	%r13d
	movl	%eax, (%rbx)
	addq	$4, %rbx
	cmpl	$3, %r13d
	jle	.L132
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L139:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L137
	cmpl	%r14d, %eax
	jne	.L277
.L137:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L139
.L134:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	incl	%ebp
	cmpl	$3, %ebp
	jle	.L141
	incl	%r12d
	cmpl	$3, %r12d
	jle	.L142
	xorl	%r12d, %r12d
.L163:
	movl	%r12d, %edx
	movl	$3, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	%r12d, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	%r12d, %edx
	movl	$1, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	%r12d, %edx
	xorl	%esi, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L153:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L151
	cmpl	%r14d, %eax
	jne	.L278
.L151:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L153
.L148:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	movl	%r12d, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	%r12d, %edx
	movl	$1, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	%r12d, %edx
	movl	$2, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	%r12d, %edx
	movl	$3, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L161:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L159
	cmpl	%r14d, %eax
	jne	.L279
.L159:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L161
.L156:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	incl	%r12d
	cmpl	$3, %r12d
	jle	.L163
	xorl	%ebp, %ebp
.L184:
	movl	$3, %edx
	movl	%ebp, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$2, %edx
	movl	%ebp, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$1, %edx
	movl	%ebp, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	xorl	%edx, %edx
	movl	%ebp, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L174:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L172
	cmpl	%r14d, %eax
	jne	.L280
.L172:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L174
.L169:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	movl	%ebp, %esi
	xorl	%edx, %edx
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	%ebp, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	%ebp, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	%ebp, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L182:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L180
	cmpl	%r14d, %eax
	jne	.L281
.L180:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L182
.L177:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	incl	%ebp
	cmpl	$3, %ebp
	jle	.L184
	xorl	%r13d, %r13d
.L205:
	xorl	%edx, %edx
	movl	$3, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$2, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$1, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	xorl	%esi, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L195:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L193
	cmpl	%r14d, %eax
	jne	.L282
.L193:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L195
.L190:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	movl	%r13d, %edi
	xorl	%edx, %edx
	xorl	%esi, %esi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$1, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$2, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	$3, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
	.p2align 2,,3
.L203:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L201
	cmpl	%r14d, %eax
	jne	.L283
.L201:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L203
.L198:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	incl	%r13d
	cmpl	$3, %r13d
	jle	.L205
	xorl	%edx, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$1, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$2, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	$3, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
.L212:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L210
	cmpl	%r14d, %eax
	jne	.L284
.L210:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L212
.L207:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	movl	$3, %edi
	xorl	%edx, %edx
	xorl	%esi, %esi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$1, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	$3, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
.L220:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L218
	cmpl	%r14d, %eax
	jne	.L285
.L218:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L220
.L215:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	movl	$3, %esi
	xorl	%edx, %edx
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$1, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	xorl	%esi, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
.L228:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L226
	cmpl	%r14d, %eax
	jne	.L286
.L226:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L228
.L223:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	movl	$3, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$2, %edx
	movl	$1, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$1, %edx
	movl	$2, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	xorl	%edx, %edx
	movl	$3, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	movl	$4, %ecx
	xorl	%esi, %esi
	xorl	%edx, %edx
.L236:
	movl	(%rsp,%rdx,4), %eax
	testl	%eax, %eax
	je	.L234
	cmpl	%r14d, %eax
	jne	.L287
.L234:
	cmpl	%r14d, (%rsp,%rdx,4)
	leal	-1(%rcx), %eax
	cmove	%eax, %ecx
	incl	%esi
	incq	%rdx
	cmpl	$3, %esi
	jle	.L236
.L231:
	cmpl	%r15d, %ecx
	cmovl	%ecx, %r15d
	addq	$24, %rsp
.LCFI29:
	popq	%rbx
.LCFI30:
	popq	%rbp
.LCFI31:
	popq	%r12
.LCFI32:
	popq	%r13
.LCFI33:
	popq	%r14
.LCFI34:
	movl	%r15d, %eax
	popq	%r15
.LCFI35:
	ret
.L287:
	movl	$6, %ecx
	jmp	.L231
.L286:
	movl	$6, %ecx
	jmp	.L223
.L285:
	movl	$6, %ecx
	jmp	.L215
.L284:
	movl	$6, %ecx
	jmp	.L207
.L283:
	movl	$6, %ecx
	jmp	.L198
.L282:
	movl	$6, %ecx
	jmp	.L190
.L281:
	movl	$6, %ecx
	jmp	.L177
.L280:
	movl	$6, %ecx
	jmp	.L169
.L279:
	movl	$6, %ecx
	jmp	.L156
.L278:
	movl	$6, %ecx
	jmp	.L148
.L277:
	movl	$6, %ecx
	jmp	.L134
.L276:
	movl	$6, %ecx
	jmp	.L111
.L275:
	movl	$6, %ecx
	jmp	.L88
.LFE32:
	.size	heuristic_count_moves_to_win, .-heuristic_count_moves_to_win
	.p2align 2,,3
.globl heuristic_count_ways_to_win
	.type	heuristic_count_ways_to_win, @function
heuristic_count_ways_to_win:
.LFB34:
	pushq	%r15
.LCFI36:
	pushq	%r14
.LCFI37:
	pushq	%r13
.LCFI38:
	pushq	%r12
.LCFI39:
	pushq	%rbp
.LCFI40:
	pushq	%rbx
.LCFI41:
	subq	$24, %rsp
.LCFI42:
	movl	%edi, %r14d
	xorl	%r15d, %r15d
	xorl	%r13d, %r13d
.L309:
	xorl	%ebp, %ebp
	.p2align 2,,3
.L308:
	xorl	%r12d, %r12d
	movq	%rsp, %rbx
	.p2align 2,,3
.L301:
	movl	%r12d, %edx
	movl	%ebp, %esi
	movl	%r13d, %edi
	call	get_at
	incl	%r12d
	movl	%eax, (%rbx)
	addq	$4, %rbx
	cmpl	$3, %r12d
	jle	.L301
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L307:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L304
	cmpl	%r14d, %eax
	jne	.L464
.L304:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L307
.L303:
	incl	%ebp
	incl	%r15d
	cmpl	$3, %ebp
	jle	.L308
	incl	%r13d
	cmpl	$3, %r13d
	jle	.L309
	xorl	%r13d, %r13d
.L330:
	xorl	%r12d, %r12d
	.p2align 2,,3
.L329:
	xorl	%ebp, %ebp
	movq	%rsp, %rbx
	.p2align 2,,3
.L322:
	movl	%ebp, %esi
	movl	%r12d, %edx
	movl	%r13d, %edi
	call	get_at
	incl	%ebp
	movl	%eax, (%rbx)
	addq	$4, %rbx
	cmpl	$3, %ebp
	jle	.L322
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L328:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L325
	cmpl	%r14d, %eax
	jne	.L465
.L325:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L328
.L324:
	incl	%r12d
	incl	%r15d
	cmpl	$3, %r12d
	jle	.L329
	incl	%r13d
	cmpl	$3, %r13d
	jle	.L330
	xorl	%r12d, %r12d
.L351:
	xorl	%ebp, %ebp
	.p2align 2,,3
.L350:
	xorl	%r13d, %r13d
	movq	%rsp, %rbx
	.p2align 2,,3
.L343:
	movl	%r13d, %edi
	movl	%r12d, %edx
	movl	%ebp, %esi
	call	get_at
	incl	%r13d
	movl	%eax, (%rbx)
	addq	$4, %rbx
	cmpl	$3, %r13d
	jle	.L343
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L349:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L346
	cmpl	%r14d, %eax
	jne	.L466
.L346:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L349
.L345:
	incl	%ebp
	incl	%r15d
	cmpl	$3, %ebp
	jle	.L350
	incl	%r12d
	cmpl	$3, %r12d
	jle	.L351
	xorl	%r12d, %r12d
.L368:
	movl	%r12d, %edx
	movl	$3, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	%r12d, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	%r12d, %edx
	movl	$1, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	%r12d, %edx
	xorl	%esi, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L361:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L358
	cmpl	%r14d, %eax
	jne	.L467
.L358:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L361
.L357:
	movl	%r12d, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	%r12d, %edx
	movl	$1, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	%r12d, %edx
	movl	$2, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	%r12d, %edx
	movl	$3, %esi
	movl	$3, %edi
	call	get_at
	incl	%r15d
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L367:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L364
	cmpl	%r14d, %eax
	jne	.L468
.L364:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L367
.L363:
	incl	%r12d
	incl	%r15d
	cmpl	$3, %r12d
	jle	.L368
	xorl	%ebp, %ebp
.L385:
	movl	$3, %edx
	movl	%ebp, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$2, %edx
	movl	%ebp, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$1, %edx
	movl	%ebp, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	xorl	%edx, %edx
	movl	%ebp, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L378:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L375
	cmpl	%r14d, %eax
	jne	.L469
.L375:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L378
.L374:
	xorl	%edx, %edx
	movl	%ebp, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	%ebp, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	%ebp, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	%ebp, %esi
	movl	$3, %edi
	call	get_at
	incl	%r15d
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L384:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L381
	cmpl	%r14d, %eax
	jne	.L470
.L381:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L384
.L380:
	incl	%ebp
	incl	%r15d
	cmpl	$3, %ebp
	jle	.L385
	xorl	%r13d, %r13d
.L402:
	xorl	%edx, %edx
	movl	$3, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$2, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$1, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	xorl	%esi, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L395:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L392
	cmpl	%r14d, %eax
	jne	.L471
.L392:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L395
.L391:
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$1, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$2, %esi
	movl	%r13d, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	$3, %esi
	movl	%r13d, %edi
	call	get_at
	incl	%r15d
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
	.p2align 2,,3
.L401:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L398
	cmpl	%r14d, %eax
	jne	.L472
.L398:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L401
.L397:
	incl	%r13d
	incl	%r15d
	cmpl	$3, %r13d
	jle	.L402
	xorl	%edx, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$1, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$2, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	$3, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
.L408:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L405
	cmpl	%r14d, %eax
	jne	.L473
.L405:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L408
.L404:
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	$3, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$1, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	movl	$3, %esi
	xorl	%edi, %edi
	call	get_at
	incl	%r15d
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
.L414:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L411
	cmpl	%r14d, %eax
	jne	.L474
.L411:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L414
.L410:
	xorl	%edx, %edx
	movl	$3, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$2, %edx
	movl	$1, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	movl	$3, %edx
	xorl	%esi, %esi
	movl	$3, %edi
	call	get_at
	incl	%r15d
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
.L420:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L417
	cmpl	%r14d, %eax
	jne	.L475
.L417:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L420
.L416:
	movl	$3, %edx
	xorl	%esi, %esi
	xorl	%edi, %edi
	call	get_at
	movl	%eax, (%rsp)
	movl	$2, %edx
	movl	$1, %esi
	movl	$1, %edi
	call	get_at
	movl	%eax, 4(%rsp)
	movl	$1, %edx
	movl	$2, %esi
	movl	$2, %edi
	call	get_at
	movl	%eax, 8(%rsp)
	xorl	%edx, %edx
	movl	$3, %esi
	movl	$3, %edi
	call	get_at
	incl	%r15d
	movl	%eax, 12(%rsp)
	xorl	%ecx, %ecx
	movq	%rsp, %rdx
.L426:
	movl	(%rdx), %eax
	testl	%eax, %eax
	je	.L423
	cmpl	%r14d, %eax
	jne	.L476
.L423:
	incl	%ecx
	addq	$4, %rdx
	cmpl	$3, %ecx
	jle	.L426
.L422:
	addq	$24, %rsp
.LCFI43:
	popq	%rbx
.LCFI44:
	popq	%rbp
.LCFI45:
	popq	%r12
.LCFI46:
	popq	%r13
.LCFI47:
	incl	%r15d
	popq	%r14
.LCFI48:
	movl	%r15d, %eax
	popq	%r15
.LCFI49:
	ret
.L476:
	decl	%r15d
	jmp	.L422
.L475:
	decl	%r15d
	jmp	.L416
.L474:
	decl	%r15d
	jmp	.L410
.L473:
	decl	%r15d
	jmp	.L404
.L472:
	decl	%r15d
	jmp	.L397
.L471:
	decl	%r15d
	jmp	.L391
.L470:
	decl	%r15d
	jmp	.L380
.L469:
	decl	%r15d
	jmp	.L374
.L468:
	decl	%r15d
	jmp	.L363
.L467:
	decl	%r15d
	jmp	.L357
.L466:
	decl	%r15d
	jmp	.L345
.L465:
	decl	%r15d
	jmp	.L324
.L464:
	decl	%r15d
	jmp	.L303
.LFE34:
	.size	heuristic_count_ways_to_win, .-heuristic_count_ways_to_win
	.p2align 2,,3
.globl clear_board
	.type	clear_board, @function
clear_board:
.LFB36:
	movl	$board, %edx
	movl	$63, %eax
	.p2align 2,,3
.L482:
	movl	$0, (%rdx)
	addq	$4, %rdx
	decl	%eax
	jns	.L482
	ret
.LFE36:
	.size	clear_board, .-clear_board
	.p2align 2,,3
.globl check_draw
	.type	check_draw, @function
check_draw:
.LFB38:
	xorl	%r8d, %r8d
	xorl	%ecx, %ecx
	cmpl	%edx, %r8d
	jge	.L493
	.p2align 2,,3
.L492:
	movslq	%ecx,%rax
	movl	(%rdi,%rax,4), %r10d
	testl	%r10d, %r10d
	jne	.L491
	movl	(%rsi,%rax,4), %r9d
	testl	%r9d, %r9d
	je	.L488
.L491:
	incl	%r8d
.L488:
	incl	%ecx
	cmpl	%edx, %ecx
	jl	.L492
	xorl	%eax, %eax
	testl	%r8d, %r8d
	jne	.L485
.L493:
	movl	$1, %eax
.L485:
	ret
.LFE38:
	.size	check_draw, .-check_draw
	.section	.rodata.str1.32
	.p2align 5
.LC29:
	.string	"HEURISTICS BEFORE: h1=%d,h2=%d,h3=%d,h4=%d\n"
	.p2align 5
.LC30:
	.string	"WIN HEURISTICS: hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n"
	.section	.rodata.str1.1
.LC31:
	.string	"ONBOARD:%d>> (%d,%d,%d)\n"
.LC32:
	.string	"MSI COMPUTED RESULT"
	.section	.rodata.str1.32
	.p2align 5
.LC25:
	.string	"CPU: Uzytkownik zaczal od srodkowego pola, bedzie trudno wygrac!"
	.text
	.p2align 2,,3
.globl forseen_best_move
	.type	forseen_best_move, @function
forseen_best_move:
.LFB40:
	pushq	%r15
.LCFI50:
	pushq	%r14
.LCFI51:
	pushq	%r13
.LCFI52:
	pushq	%r12
.LCFI53:
	pushq	%rbp
.LCFI54:
	pushq	%rbx
.LCFI55:
	subq	$72, %rsp
.LCFI56:
	movl	%edx, 60(%rsp)
	movl	%edi, 68(%rsp)
	movl	%esi, 64(%rsp)
	movl	%ecx, 56(%rsp)
	movl	%r8d, 52(%rsp)
	movl	%r9d, 48(%rsp)
	xorl	%r14d, %r14d
	movl	$board, %edx
	movl	$63, %r12d
	.p2align 2,,3
.L504:
	movl	(%rdx), %ecx
	testl	%ecx, %ecx
	leal	1(%r14), %eax
	cmove	%eax, %r14d
	addq	$4, %rdx
	decl	%r12d
	jns	.L504
	movslq	%r14d,%rax
	leaq	0(,%rax,4), %rbx
	movq	%rbx, %rdi
	call	malloc
	movq	%rbx, %rdi
	movq	%rax, 40(%rsp)
	call	malloc
	movq	%rbx, %rdi
	movq	%rax, 32(%rsp)
	call	malloc
	movq	%rbx, %rdi
	movq	%rax, 24(%rsp)
	call	malloc
	movq	%rbx, %rdi
	movq	%rax, 16(%rsp)
	call	malloc
	movq	%rbx, %rdi
	movq	%rax, 8(%rsp)
	call	malloc
	movq	%rbx, %rdi
	movq	%rax, %r15
	call	malloc
	movq	%rax, (%rsp)
	xorl	%r13d, %r13d
	xorl	%r12d, %r12d
	xorl	%ebp, %ebp
	.p2align 2,,3
.L510:
	movl	board(,%rbp,4), %eax
	testl	%eax, %eax
	je	.L786
.L507:
	incl	%r12d
	incq	%rbp
	cmpl	$63, %r12d
	jle	.L510
	movl	%r14d, %edx
	movq	16(%rsp), %rsi
	movq	24(%rsp), %rdi
	call	check_draw
	testl	%eax, %eax
	jne	.L787
	xorl	%ebx, %ebx
	movl	$board, %edx
	movl	$63, %r12d
	.p2align 2,,3
.L517:
	cmpl	$1, (%rdx)
	leal	1(%rbx), %eax
	cmove	%eax, %ebx
	addq	$4, %rdx
	decl	%r12d
	jns	.L517
	decl	%ebx
	je	.L788
.L518:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L728
.L524:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L524
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L728
.L538:
	movslq	%r12d,%rbx
	movq	40(%rsp), %rcx
	movl	(%rcx,%rbx,4), %eax
	testl	%eax, %eax
	jle	.L789
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L538
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L728
.L543:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L543
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L728
.L548:
	movslq	%r12d,%rdx
	incl	%r12d
	movl	(%r15,%rdx,4), %eax
	movq	(%rsp), %rcx
	cmpl	%r14d, %r12d
	movl	%eax, (%rcx,%rdx,4)
	jl	.L548
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L728
.L554:
	movslq	%r12d,%rax
	movq	32(%rsp), %rbx
	cmpl	$2, (%rbx,%rax,4)
	jg	.L551
	movl	$0, (%r15,%rax,4)
.L551:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L554
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L728
.L560:
	movslq	%r12d,%rax
	movq	32(%rsp), %rdx
	cmpl	$1, (%rdx,%rax,4)
	jle	.L790
.L557:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L560
.L728:
	xorl	%ebx, %ebx
	xorl	%r12d, %r12d
	cmpl	%r14d, %ebx
	jge	.L736
.L566:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %ebp
	testl	%ebp, %ebp
	leal	1(%rbx), %edx
	cmovne	%edx, %ebx
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L566
	testl	%ebx, %ebx
	jne	.L567
	xorl	%r12d, %r12d
	cmpl	%r14d, %ebx
	jge	.L567
.L572:
	movslq	%r12d,%rdx
	movq	(%rsp), %rcx
	incl	%r12d
	movl	(%rcx,%rdx,4), %eax
	cmpl	%r14d, %r12d
	movl	%eax, (%r15,%rdx,4)
	jl	.L572
.L567:
	xorl	%ebx, %ebx
	xorl	%r12d, %r12d
	cmpl	%r14d, %ebx
	jge	.L736
.L578:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %r11d
	testl	%r11d, %r11d
	leal	1(%rbx), %edx
	cmovne	%edx, %ebx
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L578
	decl	%ebx
	je	.L791
.L579:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L736
.L590:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L590
.L736:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	movl	$6, %ecx
	jge	.L742
.L596:
	movslq	%r12d,%rax
	movq	40(%rsp), %rbx
	movl	(%rbx,%rax,4), %edx
	cmpl	%ecx, %edx
	jge	.L593
	movl	(%r15,%rax,4), %r9d
	testl	%r9d, %r9d
	cmovne	%edx, %ecx
.L593:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L596
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L742
.L602:
	movslq	%r12d,%rax
	movq	40(%rsp), %rdx
	cmpl	%ecx, (%rdx,%rax,4)
	jle	.L599
	movl	$0, (%r15,%rax,4)
.L599:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L602
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L742
.L607:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L607
.L742:
	xorl	%ebx, %ebx
	xorl	%r12d, %r12d
	cmpl	%r14d, %ebx
	jge	.L747
.L613:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %r8d
	testl	%r8d, %r8d
	leal	1(%rbx), %edx
	cmovne	%edx, %ebx
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L613
	decl	%ebx
	je	.L792
.L614:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L747
.L625:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L625
.L747:
	xorl	%r12d, %r12d
	xorl	%ecx, %ecx
	cmpl	%r14d, %r12d
	jge	.L753
.L631:
	movslq	%r12d,%rax
	movq	24(%rsp), %rbx
	movl	(%rbx,%rax,4), %edx
	cmpl	%ecx, %edx
	jle	.L628
	movl	(%r15,%rax,4), %esi
	testl	%esi, %esi
	cmovne	%edx, %ecx
.L628:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L631
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L753
.L637:
	movslq	%r12d,%rax
	movq	24(%rsp), %rdx
	cmpl	%ecx, (%rdx,%rax,4)
	jge	.L634
	movl	$0, (%r15,%rax,4)
.L634:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L637
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L753
.L642:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L642
.L753:
	xorl	%ebx, %ebx
	xorl	%r12d, %r12d
	cmpl	%r14d, %ebx
	jge	.L758
.L648:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %ecx
	testl	%ecx, %ecx
	leal	1(%rbx), %edx
	cmovne	%edx, %ebx
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L648
	decl	%ebx
	je	.L793
.L649:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L758
.L660:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L660
.L758:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	movl	$100, %ecx
	jge	.L764
.L666:
	movslq	%r12d,%rax
	movq	16(%rsp), %rbx
	movl	(%rbx,%rax,4), %edx
	cmpl	%ecx, %edx
	jge	.L663
	movl	(%r15,%rax,4), %eax
	testl	%eax, %eax
	cmovne	%edx, %ecx
.L663:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L666
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L764
.L672:
	movslq	%r12d,%rax
	movq	16(%rsp), %rdx
	cmpl	(%rdx,%rax,4), %ecx
	je	.L669
	movl	$0, (%r15,%rax,4)
.L669:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L672
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L764
.L677:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L677
.L764:
	xorl	%ebx, %ebx
	xorl	%r12d, %r12d
	cmpl	%r14d, %ebx
	jge	.L684
.L683:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %r13d
	testl	%r13d, %r13d
	leal	1(%rbx), %edx
	cmovne	%edx, %ebx
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L683
	cmpl	$1, %ebx
	je	.L794
.L684:
	testl	%ebx, %ebx
	jne	.L691
	xorl	%r12d, %r12d
	cmpl	%r14d, %ebx
	movl	$5, %edx
	jge	.L769
.L697:
	movslq	%r12d,%rax
	movq	40(%rsp), %rcx
	movl	(%rcx,%rax,4), %eax
	cmpl	%edx, %eax
	cmovl	%eax, %edx
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L697
.L769:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L699
.L703:
	movslq	%r12d,%rax
	movq	40(%rsp), %rbx
	cmpl	%edx, (%rbx,%rax,4)
	je	.L783
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L703
.L699:
	movl	%r14d, %edi
	call	Random
	cltq
	movq	8(%rsp), %rdx
	movslq	(%rdx,%rax,4),%rax
	movl	52(%rsp), %ecx
	movl	%ecx, board(,%rax,4)
.L704:
	movq	8(%rsp), %rdi
.L784:
	call	free
	movq	%r15, %rdi
	call	free
	movq	(%rsp), %rdi
	call	free
	movq	40(%rsp), %rdi
	call	free
	movq	32(%rsp), %rdi
	call	free
	movq	24(%rsp), %rdi
	call	free
	movq	16(%rsp), %rdi
	call	free
.L785:
	xorl	%eax, %eax
.L498:
	addq	$72, %rsp
.LCFI57:
	popq	%rbx
.LCFI58:
	popq	%rbp
.LCFI59:
	popq	%r12
.LCFI60:
	popq	%r13
.LCFI61:
	popq	%r14
.LCFI62:
	popq	%r15
.LCFI63:
	ret
.L783:
	movq	8(%rsp), %rbx
	movslq	(%rbx,%rax,4),%rax
	movl	52(%rsp), %edx
	movl	%edx, board(,%rax,4)
	jmp	.L699
.L691:
	movslq	%ebx,%rdi
	salq	$2, %rdi
	xorl	%r13d, %r13d
	call	malloc
	xorl	%r12d, %r12d
	cmpl	%r14d, %r13d
	movq	%rax, %rbp
	jge	.L772
.L710:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %r11d
	testl	%r11d, %r11d
	je	.L707
	movq	8(%rsp), %rcx
	movslq	%r13d,%rdx
	movl	(%rcx,%rax,4), %eax
	movl	%eax, (%rbp,%rdx,4)
	incl	%r13d
.L707:
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L710
.L772:
	movl	%ebx, %edi
	call	Random
	cltq
	movslq	(%rbp,%rax,4),%rax
	movl	52(%rsp), %ebx
	movq	%rbp, %rdi
	movl	%ebx, board(,%rax,4)
	call	free
	jmp	.L704
.L794:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L684
.L690:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %ebp
	testl	%ebp, %ebp
	jne	.L782
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L690
	jmp	.L684
.L782:
	movq	8(%rsp), %rbx
	movslq	(%rbx,%rax,4),%rax
	movl	52(%rsp), %edx
	movq	%rbx, %rdi
	movl	%edx, board(,%rax,4)
	jmp	.L784
.L793:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L758
.L655:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %edx
	testl	%edx, %edx
	jne	.L781
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L655
	jmp	.L649
.L781:
	movq	8(%rsp), %rdx
	movslq	(%rdx,%rax,4),%rax
	movl	52(%rsp), %ecx
	movq	%rdx, %rdi
	movl	%ecx, board(,%rax,4)
	jmp	.L784
.L792:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L747
.L620:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %edi
	testl	%edi, %edi
	jne	.L780
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L620
	jmp	.L614
.L780:
	movq	8(%rsp), %rcx
	movslq	(%rcx,%rax,4),%rax
	movl	52(%rsp), %ebx
	movq	%rcx, %rdi
	movl	%ebx, board(,%rax,4)
	jmp	.L784
.L791:
	xorl	%r12d, %r12d
	cmpl	%r14d, %r12d
	jge	.L736
.L585:
	movslq	%r12d,%rax
	movl	(%r15,%rax,4), %r10d
	testl	%r10d, %r10d
	jne	.L782
	incl	%r12d
	cmpl	%r14d, %r12d
	jl	.L585
	jmp	.L579
.L790:
	movq	(%rsp), %rcx
	movl	$0, (%rcx,%rax,4)
	jmp	.L557
.L789:
	movq	8(%rsp), %rdx
	movl	52(%rsp), %ecx
	movslq	(%rdx,%rbx,4),%rax
	movl	56(%rsp), %r8d
	movl	64(%rsp), %edx
	movl	68(%rsp), %esi
	movl	%ecx, board(,%rax,4)
	movl	$.LC29, %edi
	movl	60(%rsp), %ecx
	xorl	%eax, %eax
	call	printf
	movq	8(%rsp), %rax
	movl	(%rax,%rbx,4), %r9d
	movq	24(%rsp), %rax
	movq	16(%rsp), %rdx
	movl	(%rax,%rbx,4), %ecx
	movq	32(%rsp), %rax
	movl	(%rdx,%rbx,4), %r8d
	movl	(%rax,%rbx,4), %edx
	movq	40(%rsp), %rax
	movl	(%rax,%rbx,4), %esi
	pushq	%r12
.LCFI64:
	movl	(%r15,%rbx,4), %eax
	pushq	%rax
.LCFI65:
	movl	$.LC30, %edi
	xorl	%eax, %eax
	call	printf
	cmpl	$-1, %r12d
	leal	15(%r12), %r8d
	cmovg	%r12d, %r8d
	sarl	$4, %r8d
	leal	3(%r12), %eax
	cmpl	$-1, %r12d
	cmovg	%r12d, %eax
	sarl	$2, %eax
	cmpl	$-1, %eax
	leal	3(%rax), %edx
	cmovg	%eax, %edx
	andl	$-4, %edx
	movl	%eax, %ecx
	subl	%edx, %ecx
	sall	$2, %eax
	movl	%r12d, %edx
	subl	%eax, %edx
	movl	%r12d, %esi
	movl	$.LC31, %edi
	xorl	%eax, %eax
	call	printf
	movl	$.LC32, %edi
	call	puts
	movq	24(%rsp), %rdi
	call	free
	movq	%r15, %rdi
	call	free
	movq	16(%rsp), %rdi
	call	free
	movq	56(%rsp), %rdi
	call	free
	movq	48(%rsp), %rdi
	call	free
	movq	40(%rsp), %rdi
	call	free
	movq	32(%rsp), %rdi
	call	free
	popq	%r12
.LCFI66:
	movl	$1, %eax
	popq	%r13
.LCFI67:
	jmp	.L498
.L788:
	cmpl	$1, board+84(%rip)
	je	.L519
	cmpl	$1, board+88(%rip)
	je	.L519
	cmpl	$1, board+100(%rip)
	je	.L519
	cmpl	$1, board+104(%rip)
	je	.L519
	cmpl	$1, board+148(%rip)
	je	.L519
	cmpl	$1, board+152(%rip)
	je	.L519
	cmpl	$1, board+164(%rip)
	je	.L519
	cmpl	$1, board+168(%rip)
	jne	.L518
.L519:
	movl	$.LC25, %edi
	call	puts
	jmp	.L518
.L787:
	xorl	%eax, %eax
	call	draw
	jmp	.L785
	.p2align 2,,3
.L786:
	movslq	%r13d,%rbx
	leaq	0(,%rbx,4), %rdi
	movl	52(%rsp), %eax
	movq	16(%rsp), %rcx
	movq	24(%rsp), %rdx
	movq	32(%rsp), %rsi
	addq	%rdi, %rdx
	addq	%rdi, %rcx
	addq	%rdi, %rsi
	movl	48(%rsp), %r9d
	addq	40(%rsp), %rdi
	movl	%eax, %r8d
	movl	%eax, board(,%rbp,4)
	call	get_current_heuristics
	movq	8(%rsp), %rdx
	movl	%r12d, (%rdx,%rbx,4)
	incl	%r13d
	movl	$1, (%r15,%rbx,4)
	movl	$0, board(,%rbp,4)
	jmp	.L507
.LFE40:
	.size	forseen_best_move, .-forseen_best_move
	.p2align 2,,3
.globl get_current_heuristics
	.type	get_current_heuristics, @function
get_current_heuristics:
.LFB42:
	pushq	%r15
.LCFI68:
	pushq	%r14
.LCFI69:
	pushq	%r13
.LCFI70:
	pushq	%r12
.LCFI71:
	pushq	%rbp
.LCFI72:
	pushq	%rbx
.LCFI73:
	movq	%rdi, %r13
	movl	%r8d, %edi
	pushq	%r10
.LCFI74:
	movq	%rsi, %r14
	movl	%r9d, %r12d
	movq	%rdx, %r15
	movq	%rcx, %rbp
	movl	%r8d, %ebx
	call	heuristic_count_moves_to_win
	movl	%eax, (%r13)
	movl	%r12d, %edi
	call	heuristic_count_moves_to_win
	movl	%eax, (%r14)
	movl	%ebx, %edi
	call	heuristic_count_ways_to_win
	movl	%eax, (%r15)
	movl	%r12d, %edi
	call	heuristic_count_ways_to_win
	movl	%eax, (%rbp)
	xorl	%eax, %eax
	cmpl	$0, (%r14)
	popq	%rsi
.LCFI75:
	popq	%rbx
.LCFI76:
	popq	%rbp
.LCFI77:
	popq	%r12
.LCFI78:
	popq	%r13
.LCFI79:
	setg	%al
	popq	%r14
.LCFI80:
	decl	%eax
	popq	%r15
.LCFI81:
	ret
.LFE42:
	.size	get_current_heuristics, .-get_current_heuristics
	.section	.rodata.str1.32
	.p2align 5
.LC52:
	.string	"\n%c%c%c\nCPU: I HAVE WIN!\n%c%c%c\n"
	.p2align 5
.LC49:
	.string	"HEURISTICS WHEN DEFETED: h1=%d,h2=%d,h3=%d,h4=%d\n"
	.p2align 5
.LC50:
	.string	"PANIC: MSI COMPUTED RESULT, CPU LOST"
	.p2align 5
.LC51:
	.string	"\n%c%c%c\nCPU: I HAVE LOST!\n%c%c%c\n"
	.text
	.p2align 2,,3
.globl cpu_msi_move
	.type	cpu_msi_move, @function
cpu_msi_move:
.LFB44:
	subq	$24, %rsp
.LCFI82:
	movl	$1, %r9d
	movl	$2, %r8d
	leaq	20(%rsp), %rcx
	leaq	16(%rsp), %rdx
	leaq	12(%rsp), %rsi
	leaq	8(%rsp), %rdi
	movl	$0, end_cond(%rip)
	call	get_current_heuristics
	incl	%eax
	je	.L802
	movl	20(%rsp), %ecx
	movl	16(%rsp), %edx
	movl	12(%rsp), %esi
	movl	8(%rsp), %edi
	movl	$1, %r9d
	movl	$2, %r8d
	call	forseen_best_move
	testl	%eax, %eax
	jne	.L803
.L798:
	addq	$24, %rsp
.LCFI83:
	ret
	.p2align 2,,3
.L803:
	pushq	%rax
.LCFI84:
	pushq	$7
.LCFI85:
	movl	$7, %r9d
	movl	$7, %r8d
	movl	$7, %ecx
	movl	$7, %edx
	movl	$7, %esi
	movl	$.LC52, %edi
	xorl	%eax, %eax
	call	printf
	movl	$1, end_cond(%rip)
	incl	cpu_wins(%rip)
.L801:
	xorl	%eax, %eax
	call	stats
	popq	%r11
.LCFI86:
	popq	%rax
.LCFI87:
	jmp	.L798
	.p2align 2,,3
.L802:
	movl	16(%rsp), %ecx
	movl	12(%rsp), %edx
	movl	8(%rsp), %esi
	movl	20(%rsp), %r8d
	movl	$.LC49, %edi
	xorl	%eax, %eax
	call	printf
	movl	$.LC50, %edi
	call	puts
	pushq	%rax
.LCFI88:
	pushq	$7
.LCFI89:
	movl	$7, %r9d
	movl	$7, %r8d
	movl	$7, %ecx
	movl	$7, %edx
	movl	$7, %esi
	movl	$.LC51, %edi
	xorl	%eax, %eax
	call	printf
	incl	hum_wins(%rip)
	movl	$1, end_cond(%rip)
	jmp	.L801
.LCFI90:
.LFE44:
	.size	cpu_msi_move, .-cpu_msi_move
	.section	.rodata.str1.32
	.p2align 5
.LC53:
	.string	"\n%c%c%c\nCPU: DRAW DETECTED!\n%c%c%c\n"
	.text
	.p2align 2,,3
.globl draw
	.type	draw, @function
draw:
.LFB46:
	subq	$16, %rsp
.LCFI91:
	pushq	$7
.LCFI92:
	movl	$7, %r9d
	movl	$7, %r8d
	movl	$7, %ecx
	movl	$7, %edx
	movl	$7, %esi
	movl	$.LC53, %edi
	xorl	%eax, %eax
	call	printf
	movl	$1, end_cond(%rip)
	xorl	%eax, %eax
	addq	$24, %rsp
.LCFI93:
	jmp	stats
.LFE46:
	.size	draw, .-draw
	.section	.rodata.str1.1
.LC54:
	.string	"/----+----+----+----\\\n|"
.LC56:
	.string	"----+----+----+----/"
.LC55:
	.string	"|\n|"
	.text
	.p2align 2,,3
.globl DrawScene
	.type	DrawScene, @function
DrawScene:
.LFB48:
	pushq	%rbp
.LCFI94:
	pushq	%rbx
.LCFI95:
	movl	$10, %edi
	pushq	%rcx
.LCFI96:
	call	putchar
	movl	$.LC54, %edi
	xorl	%eax, %eax
	call	printf
	xorl	%ebx, %ebx
	movl	$board, %ebp
	.p2align 2,,3
.L819:
	testb	$15, %bl
	je	.L810
	testb	$3, %bl
	je	.L827
.L810:
	movl	selected(%rip), %eax
	cmpl	%eax, %ebx
	je	.L828
.L822:
	cmpl	$1, (%rbp)
	je	.L829
.L812:
	cmpl	%eax, %ebx
	je	.L823
.L824:
	cmpl	$2, (%rbp)
	je	.L830
.L814:
	cmpl	%eax, %ebx
	je	.L825
.L826:
	movl	(%rbp), %eax
	testl	%eax, %eax
	je	.L831
.L816:
	cmpl	$-1, %ebx
	leal	15(%rbx), %eax
	cmovg	%ebx, %eax
	andl	$-16, %eax
	movl	%ebx, %edx
	subl	%eax, %edx
	cmpl	$15, %edx
	je	.L832
.L808:
	incl	%ebx
	addq	$4, %rbp
	cmpl	$63, %ebx
	jle	.L819
	movl	$.LC56, %edi
	call	puts
	popq	%rax
.LCFI97:
	popq	%rbx
.LCFI98:
	movl	$10, %edi
	popq	%rbp
.LCFI99:
	jmp	putchar
	.p2align 2,,3
.L832:
	movl	$.LC55, %edi
	xorl	%eax, %eax
	call	printf
	jmp	.L808
	.p2align 2,,3
.L831:
	movl	$32, %edi
	call	putchar
	jmp	.L816
	.p2align 2,,3
.L825:
	movl	(%rbp), %edx
	testl	%edx, %edx
	je	.L833
.L815:
	cmpl	%eax, %ebx
	jne	.L826
	jmp	.L816
	.p2align 2,,3
.L833:
	movl	$43, %edi
	call	putchar
	movl	selected(%rip), %eax
	jmp	.L815
	.p2align 2,,3
.L830:
	movl	$111, %edi
	call	putchar
	movl	selected(%rip), %eax
	jmp	.L814
	.p2align 2,,3
.L823:
	cmpl	$2, (%rbp)
	je	.L834
.L813:
	cmpl	%eax, %ebx
	jne	.L824
	jmp	.L825
.L834:
	movl	$79, %edi
	call	putchar
	movl	selected(%rip), %eax
	jmp	.L813
	.p2align 2,,3
.L829:
	movl	$120, %edi
	call	putchar
	movl	selected(%rip), %eax
	jmp	.L812
	.p2align 2,,3
.L828:
	cmpl	$1, (%rbp)
	je	.L835
.L811:
	cmpl	%eax, %ebx
	jne	.L822
	jmp	.L823
.L835:
	movl	$88, %edi
	call	putchar
	movl	selected(%rip), %eax
	jmp	.L811
	.p2align 2,,3
.L827:
	movl	$124, %edi
	call	putchar
	jmp	.L810
.LFE48:
	.size	DrawScene, .-DrawScene
	.section	.rodata.str1.32
	.p2align 5
.LC57:
	.string	"Program by Morgoth DBMA - Lukasz Gryglicki MiNI M1: morgothdbma@o2.pl; 693582014"
	.p2align 5
.LC58:
	.string	"Z             REFRESH SCENE\nH             HELP\nSPACE         MARK HERE\nQ             EXIT PROGRAM\nARROWS/PGUP/PGDOWN  MOVING"
	.text
	.p2align 2,,3
.globl help
	.type	help, @function
help:
.LFB50:
	pushq	%rdi
.LCFI100:
	movl	$.LC57, %edi
	call	puts
	movl	$.LC58, %edi
	popq	%rsi
.LCFI101:
	jmp	puts
.LFE50:
	.size	help, .-help
	.section	.rodata.str1.32
	.p2align 5
.LC62:
	.string	"\n%c%c%cMSI CPU ShutDown\n\n%c%c%c\n"
	.section	.rodata.str1.1
.LC59:
	.string	"\n>> "
.LC61:
	.string	"\n%c%c%c\n"
	.text
	.p2align 2,,3
.globl main
	.type	main, @function
main:
.LFB52:
	pushq	%rbx
.LCFI102:
	xorl	%eax, %eax
	call	help
	xorl	%eax, %eax
	call	init
	xorl	%eax, %eax
	call	Randomize
	movl	$20, %edi
	call	Random
	cmpl	$10, %eax
	jg	.L860
.L838:
	movl	done(%rip), %ecx
	testl	%ecx, %ecx
	je	.L857
.L859:
	pushq	%r10
.LCFI103:
	pushq	$7
.LCFI104:
	movl	$7, %r9d
	movl	$7, %r8d
	movl	$7, %ecx
	movl	$7, %edx
	movl	$7, %esi
	movl	$.LC62, %edi
	xorl	%eax, %eax
	call	printf
	popq	%r8
.LCFI105:
	popq	%r9
.LCFI106:
	xorl	%eax, %eax
	popq	%rbx
.LCFI107:
	ret
	.p2align 2,,3
.L857:
	xorl	%eax, %eax
	call	DrawScene
	movl	$.LC59, %edi
	xorl	%eax, %eax
	call	printf
	xorl	%eax, %eax
	call	mgetch
	movl	%eax, %ebx
	movl	$10, %edi
	call	putchar
	cmpl	$27, %ebx
	je	.L861
.L842:
	cmpl	$91, %ebx
	je	.L862
.L843:
	cmpl	$113, %ebx
	movl	$1, %eax
	cmovne	done(%rip), %eax
	cmpl	$66, %ebx
	movl	%eax, done(%rip)
	je	.L863
.L845:
	cmpl	$65, %ebx
	je	.L864
.L846:
	cmpl	$67, %ebx
	je	.L865
.L847:
	cmpl	$68, %ebx
	je	.L866
.L848:
	cmpl	$73, %ebx
	je	.L867
.L849:
	cmpl	$71, %ebx
	je	.L868
.L850:
	cmpl	$104, %ebx
	je	.L869
.L851:
	cmpl	$122, %ebx
	je	.L870
.L852:
	cmpl	$32, %ebx
	je	.L871
.L839:
	movl	done(%rip), %r11d
	testl	%r11d, %r11d
	je	.L857
	jmp	.L859
	.p2align 2,,3
.L871:
	movslq	selected(%rip),%rax
	movl	board(,%rax,4), %edx
	testl	%edx, %edx
	je	.L872
.L854:
	movl	end_cond(%rip), %ebx
	testl	%ebx, %ebx
	je	.L839
	xorl	%eax, %eax
	call	clear_board
	movl	$20, %edi
	call	Random
	cmpl	$10, %eax
	jg	.L873
.L856:
	movl	$7, %ecx
	movl	$7, %edx
	movl	$7, %esi
	movl	$.LC61, %edi
	xorl	%eax, %eax
	call	printf
	movl	$0, end_cond(%rip)
	jmp	.L839
.L873:
	xorl	%eax, %eax
	call	cpu_msi_move
	jmp	.L856
.L872:
	movl	$1, board(,%rax,4)
	xorl	%eax, %eax
	call	cpu_msi_move
	xorl	%eax, %eax
	call	DrawScene
	jmp	.L854
	.p2align 2,,3
.L870:
	xorl	%eax, %eax
	call	DrawScene
	jmp	.L852
	.p2align 2,,3
.L869:
	xorl	%eax, %eax
	call	help
	jmp	.L851
	.p2align 2,,3
.L868:
	movl	$-1, %edi
	call	move_y
	jmp	.L850
	.p2align 2,,3
.L867:
	movl	$1, %edi
	call	move_y
	jmp	.L849
	.p2align 2,,3
.L866:
	movl	$-1, %edi
	call	move_x
	jmp	.L848
	.p2align 2,,3
.L865:
	movl	$1, %edi
	call	move_x
	jmp	.L847
	.p2align 2,,3
.L864:
	movl	$-1, %edi
	call	move_z
	jmp	.L846
	.p2align 2,,3
.L863:
	movl	$1, %edi
	call	move_z
	jmp	.L845
	.p2align 2,,3
.L862:
	xorl	%eax, %eax
	call	mgetch
	movl	%eax, %ebx
	jmp	.L843
	.p2align 2,,3
.L861:
	xorl	%eax, %eax
	call	mgetch
	movl	%eax, %ebx
	jmp	.L842
.L860:
	xorl	%eax, %eax
	call	cpu_msi_move
	jmp	.L838
.LFE52:
	.size	main, .-main
	.local	_ran_device
	.comm	_ran_device,8,8
	.local	board
	.comm	board,256,32
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.p2align 3
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.quad	.LFB4
	.quad	.LFE4-.LFB4
	.byte	0x4
	.long	.LCFI0-.LFB4
	.byte	0xe
	.uleb128 0x48
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.quad	.LFB6
	.quad	.LFE6-.LFB6
	.byte	0x4
	.long	.LCFI2-.LFB6
	.byte	0xe
	.uleb128 0x48
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.byte	0x4
	.long	.LCFI4-.LFB8
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x80
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.quad	.LFB10
	.quad	.LFE10-.LFB10
	.byte	0x4
	.long	.LCFI10-.LFB10
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x8
	.byte	0x2e
	.uleb128 0x0
	.p2align 3
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.quad	.LFB12
	.quad	.LFE12-.LFB12
	.byte	0x4
	.long	.LCFI12-.LFB12
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI17-.LCFI16
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.quad	.LFB14
	.quad	.LFE14-.LFB14
	.p2align 3
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.quad	.LFB16
	.quad	.LFE16-.LFB16
	.byte	0x4
	.long	.LCFI20-.LFB16
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0x8
	.byte	0x2e
	.uleb128 0x0
	.p2align 3
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.quad	.LFB18
	.quad	.LFE18-.LFB18
	.p2align 3
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.quad	.LFB20
	.quad	.LFE20-.LFB20
	.p2align 3
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.p2align 3
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.p2align 3
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.p2align 3
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.quad	.LFB28
	.quad	.LFE28-.LFB28
	.p2align 3
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.quad	.LFB30
	.quad	.LFE30-.LFB30
	.p2align 3
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.quad	.LFB32
	.quad	.LFE32-.LFB32
	.byte	0x4
	.long	.LCFI22-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI25-.LCFI24
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI27-.LCFI26
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI28-.LCFI27
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI29-.LCFI28
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI30-.LCFI29
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI31-.LCFI30
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI32-.LCFI31
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI33-.LCFI32
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI34-.LCFI33
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI35-.LCFI34
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.quad	.LFB34
	.quad	.LFE34-.LFB34
	.byte	0x4
	.long	.LCFI36-.LFB34
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI37-.LCFI36
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI38-.LCFI37
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI39-.LCFI38
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI40-.LCFI39
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI41-.LCFI40
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI42-.LCFI41
	.byte	0xe
	.uleb128 0x50
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI43-.LCFI42
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI44-.LCFI43
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI45-.LCFI44
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI46-.LCFI45
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI47-.LCFI46
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI48-.LCFI47
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI49-.LCFI48
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.quad	.LFB36
	.quad	.LFE36-.LFB36
	.p2align 3
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.quad	.LFB38
	.quad	.LFE38-.LFB38
	.p2align 3
.LEFDE35:
.LSFDE37:
	.long	.LEFDE37-.LASFDE37
.LASFDE37:
	.long	.LASFDE37-.Lframe1
	.quad	.LFB40
	.quad	.LFE40-.LFB40
	.byte	0x4
	.long	.LCFI50-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI51-.LCFI50
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI52-.LCFI51
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI53-.LCFI52
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI54-.LCFI53
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI55-.LCFI54
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI56-.LCFI55
	.byte	0xe
	.uleb128 0x80
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI57-.LCFI56
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI58-.LCFI57
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI59-.LCFI58
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI60-.LCFI59
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI61-.LCFI60
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI62-.LCFI61
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI63-.LCFI62
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI64-.LCFI63
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI65-.LCFI64
	.byte	0xe
	.uleb128 0x18
	.byte	0x2e
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI66-.LCFI65
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI67-.LCFI66
	.byte	0xe
	.uleb128 0x8
	.byte	0x2e
	.uleb128 0x0
	.p2align 3
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.quad	.LFB42
	.quad	.LFE42-.LFB42
	.byte	0x4
	.long	.LCFI68-.LFB42
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI69-.LCFI68
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI70-.LCFI69
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI71-.LCFI70
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI72-.LCFI71
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI73-.LCFI72
	.byte	0xe
	.uleb128 0x38
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI74-.LCFI73
	.byte	0xe
	.uleb128 0x40
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI75-.LCFI74
	.byte	0xe
	.uleb128 0x38
	.byte	0x2e
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI76-.LCFI75
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI77-.LCFI76
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI78-.LCFI77
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI79-.LCFI78
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI80-.LCFI79
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI81-.LCFI80
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE39:
.LSFDE41:
	.long	.LEFDE41-.LASFDE41
.LASFDE41:
	.long	.LASFDE41-.Lframe1
	.quad	.LFB44
	.quad	.LFE44-.LFB44
	.byte	0x4
	.long	.LCFI82-.LFB44
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI83-.LCFI82
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI84-.LCFI83
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI85-.LCFI84
	.byte	0xe
	.uleb128 0x18
	.byte	0x2e
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI86-.LCFI85
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI87-.LCFI86
	.byte	0xe
	.uleb128 0x8
	.byte	0x2e
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI88-.LCFI87
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI89-.LCFI88
	.byte	0xe
	.uleb128 0x18
	.byte	0x2e
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI90-.LCFI89
	.byte	0xe
	.uleb128 0x8
	.byte	0x2e
	.uleb128 0x0
	.p2align 3
.LEFDE41:
.LSFDE43:
	.long	.LEFDE43-.LASFDE43
.LASFDE43:
	.long	.LASFDE43-.Lframe1
	.quad	.LFB46
	.quad	.LFE46-.LFB46
	.byte	0x4
	.long	.LCFI91-.LFB46
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI92-.LCFI91
	.byte	0xe
	.uleb128 0x20
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI93-.LCFI92
	.byte	0xe
	.uleb128 0x8
	.byte	0x2e
	.uleb128 0x0
	.p2align 3
.LEFDE43:
.LSFDE45:
	.long	.LEFDE45-.LASFDE45
.LASFDE45:
	.long	.LASFDE45-.Lframe1
	.quad	.LFB48
	.quad	.LFE48-.LFB48
	.byte	0x4
	.long	.LCFI94-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI95-.LCFI94
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI96-.LCFI95
	.byte	0xe
	.uleb128 0x20
	.byte	0x2e
	.uleb128 0x8
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI97-.LCFI96
	.byte	0xe
	.uleb128 0x18
	.byte	0x2e
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI98-.LCFI97
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI99-.LCFI98
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE45:
.LSFDE47:
	.long	.LEFDE47-.LASFDE47
.LASFDE47:
	.long	.LASFDE47-.Lframe1
	.quad	.LFB50
	.quad	.LFE50-.LFB50
	.byte	0x4
	.long	.LCFI100-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI101-.LCFI100
	.byte	0xe
	.uleb128 0x8
	.byte	0x2e
	.uleb128 0x0
	.p2align 3
.LEFDE47:
.LSFDE49:
	.long	.LEFDE49-.LASFDE49
.LASFDE49:
	.long	.LASFDE49-.Lframe1
	.quad	.LFB52
	.quad	.LFE52-.LFB52
	.byte	0x4
	.long	.LCFI102-.LFB52
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI103-.LCFI102
	.byte	0xe
	.uleb128 0x18
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI104-.LCFI103
	.byte	0xe
	.uleb128 0x20
	.byte	0x2e
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI105-.LCFI104
	.byte	0xe
	.uleb128 0x18
	.byte	0x2e
	.uleb128 0x8
	.byte	0x4
	.long	.LCFI106-.LCFI105
	.byte	0xe
	.uleb128 0x10
	.byte	0x2e
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI107-.LCFI106
	.byte	0xe
	.uleb128 0x8
	.p2align 3
.LEFDE49:
	.ident	"GCC: (GNU) 3.3.3 [FreeBSD] 20031106"
