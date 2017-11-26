	.file	"udf.c"
	.text
.globl color_comix
	.type	color_comix, @function
color_comix:
.LFB16:
	.cfi_startproc
	movq	%rdi, %rax
	shrq	$16, %rax
	andl	$192, %eax
	movq	%rax, -24(%rsp)
	movq	%rdi, %rax
	shrq	$8, %rax
	andl	$192, %eax
	movq	%rax, -40(%rsp)
	andl	$192, %edi
	movq	%rdi, -56(%rsp)
	movq	-24(%rsp), %rcx
	movq	-40(%rsp), %rdx
	movq	-56(%rsp), %rax
	salq	$16, %rcx
	salq	$8, %rdx
	leaq	(%rcx,%rdx), %rdx
	leaq	(%rdx,%rax), %rax
	ret
	.cfi_endproc
.LFE16:
	.size	color_comix, .-color_comix
.globl edge_detect_comix
	.type	edge_detect_comix, @function
edge_detect_comix:
.LFB17:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	movq	%rdi, %rax
	movzbl	%ah, %ebx
	.cfi_offset 3, -16
	movzbl	%dil, %r8d
	leal	(%rdx,%rbx), %eax
	addl	%r8d, %eax
	movzbl	%sil, %ecx
	subl	%ecx, %eax
	movq	%rsi, %rcx
	andl	$16711680, %ecx
	shrq	$16, %rcx
	subl	%ecx, %eax
	movq	%rsi, %rcx
	movzbl	%ch, %esi
	subl	%esi, %eax
	movl	%eax, %ecx
	negl	%ecx
	testl	%eax, %eax
	cmovs	%ecx, %eax
	cltq
	cmpq	$15, %rax
	jbe	.L10
	cmpq	%rbx, %rdx
	jbe	.L6
	cmpq	%rdx, %r8
	sbbq	%rax, %rax
	andq	$-16711425, %rax
	addq	$16776960, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	cmpq	%r8, %rbx
	ja	.L8
	salq	$8, %rbx
	leaq	16711935(%rbx), %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L10:
	.cfi_restore_state
	andq	$-64, %rdx
	salq	$16, %rdx
	andq	$-64, %rbx
	salq	$8, %rbx
	leaq	(%rdx,%rbx), %rbx
	andl	$192, %edi
	leaq	(%rbx,%rdi), %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L8:
	.cfi_restore_state
	movl	$16711935, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE17:
	.size	edge_detect_comix, .-edge_detect_comix
.globl smooth_gs6
	.type	smooth_gs6, @function
smooth_gs6:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	movq	(%rdi), %rbx
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	movzbl	%bh, %ebp
	movq	8(%rdi), %r8
	movq	%r8, %rax
	movzbl	%ah, %eax
	movq	%rax, %r9
	movq	16(%rdi), %rax
	movzbl	%ah, %edx
	andl	$16711680, %eax
	shrq	$16, %rax
	leaq	(%rax,%rdx,2), %rsi
	movabsq	$-6148914691236517205, %rcx
	movq	%rsi, %rax
	mulq	%rcx
	movq	%rdx, %rsi
	shrq	%rsi
	andl	$16711680, %r8d
	shrq	$16, %r8
	leaq	(%r8,%r9,2), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	%rdx, %rsi
	andl	$16711680, %ebx
	shrq	$16, %rbx
	leaq	(%rbx,%rbp,2), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	leaq	(%rdx,%rsi), %rsi
	movq	24(%rdi), %rax
	movzbl	%ah, %edx
	andl	$16711680, %eax
	shrq	$16, %rax
	leaq	(%rax,%rdx,2), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	%rdx, %rsi
	movq	32(%rdi), %rax
	movzbl	%ah, %edx
	andl	$16711680, %eax
	shrq	$16, %rax
	leaq	(%rax,%rdx,2), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	leaq	(%rsi,%rdx), %rbx
	movq	40(%rdi), %rax
	movzbl	%ah, %edx
	andl	$16711680, %eax
	shrq	$16, %rax
	leaq	(%rax,%rdx,2), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	%rbx, %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	$2, %rdx
	movq	%rdx, %rcx
	salq	$16, %rcx
	movq	%rdx, %rax
	salq	$8, %rax
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE18:
	.size	smooth_gs6, .-smooth_gs6
.globl color_offset6
	.type	color_offset6, @function
color_offset6:
.LFB19:
	.cfi_startproc
	movzbl	10(%rdi), %edx
	movzbl	2(%rdi), %eax
	leaq	(%rdx,%rax), %rax
	shrq	%rax
	movq	%rax, -24(%rsp)
	movzbl	25(%rdi), %edx
	movzbl	17(%rdi), %eax
	leaq	(%rdx,%rax), %rax
	shrq	%rax
	movq	%rax, -40(%rsp)
	movzbl	40(%rdi), %edx
	movzbl	32(%rdi), %eax
	leaq	(%rdx,%rax), %rax
	shrq	%rax
	movq	%rax, -56(%rsp)
	movq	-24(%rsp), %rcx
	movq	-40(%rsp), %rdx
	movq	-56(%rsp), %rax
	salq	$16, %rcx
	salq	$8, %rdx
	leaq	(%rcx,%rdx), %rdx
	leaq	(%rdx,%rax), %rax
	ret
	.cfi_endproc
.LFE19:
	.size	color_offset6, .-color_offset6
.globl color_offset3
	.type	color_offset3, @function
color_offset3:
.LFB20:
	.cfi_startproc
	andl	$65280, %esi
	andl	$16711680, %edi
	leaq	(%rsi,%rdi), %rdi
	andl	$255, %edx
	leaq	(%rdi,%rdx), %rax
	ret
	.cfi_endproc
.LFE20:
	.size	color_offset3, .-color_offset3
.globl smooth_gs
	.type	smooth_gs, @function
smooth_gs:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	movq	%rdx, %rbx
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	movq	%rdi, %rax
	movzbl	%ah, %eax
	movq	%rax, %r8
	movq	%rsi, %rdx
	movzbl	%dh, %eax
	movzbl	%bh, %ebp
	andl	$16711680, %esi
	shrq	$16, %rsi
	leaq	(%rsi,%rax,2), %rsi
	movabsq	$-6148914691236517205, %rcx
	movq	%rsi, %rax
	mulq	%rcx
	movq	%rdx, %rsi
	shrq	%rsi
	andl	$16711680, %edi
	shrq	$16, %rdi
	leaq	(%rdi,%r8,2), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	%rdx, %rsi
	andl	$16711680, %ebx
	shrq	$16, %rbx
	leaq	(%rbx,%rbp,2), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	leaq	(%rsi,%rdx), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	movq	%rdx, %rcx
	salq	$16, %rcx
	movq	%rdx, %rax
	salq	$8, %rax
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE21:
	.size	smooth_gs, .-smooth_gs
.globl edge_detect
	.type	edge_detect, @function
edge_detect:
.LFB22:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdx, %rbx
	.cfi_offset 3, -16
	movl	$25000, %eax
	xorl	%edx, %edx
	divq	%rbx
	movq	%rsi, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	movq	%rsi, %rcx
	movzbl	%ch, %ebx
	andl	$255, %esi
	movq	%rdi, %r8
	andl	$16711680, %r8d
	shrq	$16, %r8
	subl	%edx, %r8d
	movl	%r8d, %r9d
	negl	%r9d
	testl	%r8d, %r8d
	cmovns	%r8d, %r9d
	movq	%rdi, %rcx
	movzbl	%ch, %ecx
	movq	%rcx, %r8
	subl	%ebx, %r8d
	movl	%r8d, %ecx
	negl	%ecx
	testl	%r8d, %r8d
	cmovs	%ecx, %r8d
	movslq	%r8d, %rcx
	andl	$255, %edi
	subl	%esi, %edi
	movl	%edi, %r8d
	negl	%r8d
	testl	%edi, %edi
	cmovs	%r8d, %edi
	movslq	%edi, %rdi
	movslq	%r9d, %r8
	cmpq	%r8, %rax
	jae	.L22
	salq	$16, %rdx
	cmpq	%rcx, %rax
	jae	.L23
.L25:
	salq	$8, %rbx
.L20:
	xorl	%ecx, %ecx
	cmpq	%rdi, %rax
	cmovb	%rsi, %rcx
	leaq	(%rbx,%rdx), %rax
	addq	%rcx, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L22:
	.cfi_restore_state
	xorl	%edx, %edx
	cmpq	%rcx, %rax
	jb	.L25
.L23:
	xorl	%ebx, %ebx
	jmp	.L20
	.cfi_endproc
.LFE22:
	.size	edge_detect, .-edge_detect
.globl alien_gs3
	.type	alien_gs3, @function
alien_gs3:
.LFB23:
	.cfi_startproc
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	movsd	.LC0(%rip), %xmm4
	ucomisd	%xmm4, %xmm0
	jae	.L27
	cvttsd2siq	%xmm0, %rax
.L28:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movsd	.LC1(%rip), %xmm3
	divsd	%xmm3, %xmm0
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm5
	divsd	%xmm3, %xmm5
	addsd	%xmm5, %xmm0
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm5
	divsd	%xmm3, %xmm5
	addsd	%xmm5, %xmm0
	movsd	.LC2(%rip), %xmm5
	divsd	%xmm5, %xmm0
	mulsd	%xmm5, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm1, 64(%rsp)
	movsd	%xmm2, 48(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 16(%rsp)
	movsd	%xmm5, 32(%rsp)
	call	cos@PLT
	movsd	%xmm0, 80(%rsp)
	movsd	64(%rsp), %xmm1
	movsd	16(%rsp), %xmm4
	ucomisd	%xmm4, %xmm1
	movsd	48(%rsp), %xmm2
	movsd	(%rsp), %xmm3
	movsd	32(%rsp), %xmm5
	jae	.L35
	cvttsd2siq	%xmm1, %rax
.L36:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	divsd	%xmm3, %xmm0
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm1
	divsd	%xmm3, %xmm1
	addsd	%xmm1, %xmm0
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm3, %xmm1
	addsd	%xmm1, %xmm0
	divsd	%xmm5, %xmm0
	mulsd	%xmm5, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm2, 48(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 16(%rsp)
	movsd	%xmm5, 32(%rsp)
	call	cos@PLT
	movsd	%xmm0, 88(%rsp)
	movsd	48(%rsp), %xmm2
	movsd	16(%rsp), %xmm4
	ucomisd	%xmm4, %xmm2
	movsd	(%rsp), %xmm3
	movsd	32(%rsp), %xmm5
	jae	.L43
	cvttsd2siq	%xmm2, %rax
.L44:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	divsd	%xmm3, %xmm0
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm1
	divsd	%xmm3, %xmm1
	addsd	%xmm1, %xmm0
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm3, %xmm1
	addsd	%xmm1, %xmm0
	divsd	%xmm5, %xmm0
	mulsd	%xmm5, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm1
	movapd	%xmm1, %xmm2
	subsd	80(%rsp), %xmm2
	movsd	.LC5(%rip), %xmm5
	mulsd	%xmm5, %xmm2
	movsd	(%rsp), %xmm3
	mulsd	%xmm3, %xmm2
	movsd	16(%rsp), %xmm4
	ucomisd	%xmm4, %xmm2
	jae	.L51
	cvttsd2siq	%xmm2, %rdx
.L52:
	salq	$16, %rdx
	movapd	%xmm1, %xmm2
	subsd	88(%rsp), %xmm2
	mulsd	%xmm5, %xmm2
	mulsd	%xmm3, %xmm2
	ucomisd	%xmm4, %xmm2
	jae	.L53
	cvttsd2siq	%xmm2, %rcx
.L54:
	salq	$8, %rcx
	addq	%rcx, %rdx
	subsd	%xmm0, %xmm1
	mulsd	%xmm5, %xmm1
	mulsd	%xmm3, %xmm1
	ucomisd	%xmm4, %xmm1
	jae	.L55
	cvttsd2siq	%xmm1, %rax
	addq	%rdx, %rax
	js	.L57
.L59:
	cvtsi2sdq	%rax, %xmm0
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L27:
	.cfi_restore_state
	subsd	%xmm4, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L28
.L55:
	subsd	%xmm4, %xmm1
	cvttsd2siq	%xmm1, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L59
.L57:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L53:
	.cfi_restore_state
	subsd	%xmm4, %xmm2
	cvttsd2siq	%xmm2, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L54
.L51:
	subsd	%xmm4, %xmm2
	cvttsd2siq	%xmm2, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L52
.L43:
	subsd	%xmm4, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L44
.L35:
	subsd	%xmm4, %xmm1
	cvttsd2siq	%xmm1, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L36
	.cfi_endproc
.LFE23:
	.size	alien_gs3, .-alien_gs3
.globl alien_gs
	.type	alien_gs, @function
alien_gs:
.LFB24:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movsd	.LC0(%rip), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L61
	cvttsd2siq	%xmm0, %rax
.L62:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movsd	.LC1(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm3
	divsd	%xmm1, %xmm3
	addsd	%xmm3, %xmm0
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm3
	divsd	%xmm1, %xmm3
	addsd	%xmm3, %xmm0
	movsd	.LC2(%rip), %xmm3
	divsd	%xmm3, %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	subsd	%xmm0, %xmm3
	mulsd	.LC5(%rip), %xmm3
	movsd	16(%rsp), %xmm1
	mulsd	%xmm1, %xmm3
	movsd	(%rsp), %xmm2
	ucomisd	%xmm2, %xmm3
	jae	.L69
	cvttsd2siq	%xmm3, %rax
.L70:
	movq	%rax, %rcx
	salq	$16, %rcx
	movq	%rax, %rdx
	salq	$8, %rdx
	leaq	(%rcx,%rdx), %rdx
	addq	%rdx, %rax
	js	.L71
	cvtsi2sdq	%rax, %xmm0
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L61:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L62
.L69:
	subsd	%xmm2, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L70
.L71:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE24:
	.size	alien_gs, .-alien_gs
.globl alien_toon3
	.type	alien_toon3, @function
alien_toon3:
.LFB25:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdx, %rbx
	.cfi_offset 3, -16
	movl	%ecx, %r10d
	movl	%r8d, %ecx
	movq	%rdi, %r8
	andl	$16711680, %r8d
	shrq	$14, %r8
	movq	%rdi, %rax
	movzbl	%ah, %edx
	leaq	(%rdx,%rdx,4), %rdx
	leaq	(%r8,%rdx), %rax
	andl	$255, %edi
	leaq	(%rax,%rdi), %rdi
	movabsq	$-3689348814741910323, %r8
	movq	%rdi, %rax
	mulq	%r8
	movq	%rdx, %rdi
	shrq	$3, %rdi
	movslq	%r10d, %r10
	testq	%r10, %r10
	je	.L74
	xorl	%eax, %eax
	movl	$127, %r11d
.L77:
	movq	%r11, %rdx
	subq	%rdi, %rdx
	addq	%rdx, %rdx
	leaq	-256(%rdi,%rdi), %r8
	cmpq	$127, %rdi
	movq	%rdx, %rdi
	cmova	%r8, %rdi
	addq	$1, %rax
	cmpq	%r10, %rax
	jb	.L77
.L74:
	movslq	%r9d, %r9
	testq	%r9, %r9
	je	.L78
	xorl	%eax, %eax
.L79:
	imulq	%rdi, %rdi
	shrq	$8, %rdi
	addq	$1, %rax
	cmpq	%r9, %rax
	jb	.L79
.L78:
	shrq	%cl, %rdi
	salq	%cl, %rdi
	movq	%rsi, %r8
	andl	$16711680, %r8d
	shrq	$14, %r8
	movq	%rsi, %rax
	movzbl	%ah, %edx
	leaq	(%rdx,%rdx,4), %rdx
	leaq	(%r8,%rdx), %rax
	andl	$255, %esi
	leaq	(%rax,%rsi), %rsi
	movabsq	$-3689348814741910323, %r8
	movq	%rsi, %rax
	mulq	%r8
	movq	%rdx, %rsi
	shrq	$3, %rsi
	testq	%r10, %r10
	je	.L80
	xorl	%eax, %eax
	movl	$127, %r11d
.L83:
	movq	%r11, %rdx
	subq	%rsi, %rdx
	addq	%rdx, %rdx
	leaq	-256(%rsi,%rsi), %r8
	cmpq	$127, %rsi
	movq	%rdx, %rsi
	cmova	%r8, %rsi
	addq	$1, %rax
	cmpq	%r10, %rax
	jb	.L83
.L80:
	testq	%r9, %r9
	je	.L84
	xorl	%eax, %eax
.L85:
	imulq	%rsi, %rsi
	shrq	$8, %rsi
	addq	$1, %rax
	cmpq	%r9, %rax
	jb	.L85
.L84:
	shrq	%cl, %rsi
	salq	%cl, %rsi
	movq	%rbx, %rax
	andl	$16711680, %eax
	shrq	$14, %rax
	movzbl	%bh, %edx
	leaq	(%rdx,%rdx,4), %rdx
	leaq	(%rax,%rdx), %rdx
	andl	$255, %ebx
	addq	%rbx, %rdx
	movabsq	$-3689348814741910323, %r8
	movq	%rdx, %rax
	mulq	%r8
	shrq	$3, %rdx
	testq	%r10, %r10
	je	.L86
	xorl	%eax, %eax
	movl	$127, %r11d
.L89:
	movq	%r11, %rbx
	subq	%rdx, %rbx
	addq	%rbx, %rbx
	leaq	-256(%rdx,%rdx), %r8
	cmpq	$127, %rdx
	movq	%rbx, %rdx
	cmova	%r8, %rdx
	addq	$1, %rax
	cmpq	%r10, %rax
	jb	.L89
.L86:
	testq	%r9, %r9
	je	.L90
	xorl	%eax, %eax
.L91:
	imulq	%rdx, %rdx
	shrq	$8, %rdx
	addq	$1, %rax
	cmpq	%r9, %rax
	jb	.L91
.L90:
	shrq	%cl, %rdx
	salq	%cl, %rdx
	addq	%rdx, %rdx
	movl	$1, %eax
	sall	%cl, %eax
	cltq
	addq	%rax, %rdi
	salq	$8, %rsi
	movl	$255, %ecx
	cmpq	$255, %rdi
	cmova	%rcx, %rdi
	salq	$16, %rdi
	leaq	(%rsi,%rdi), %rdi
	cmpq	$255, %rdx
	cmova	%rcx, %rdx
	leaq	(%rdi,%rdx), %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE25:
	.size	alien_toon3, .-alien_toon3
.globl alien_toongs2
	.type	alien_toongs2, @function
alien_toongs2:
.LFB26:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movl	%edx, %r9d
	movq	%rdi, %rax
	andl	$16711680, %eax
	shrq	$14, %rax
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movzbl	%bh, %edx
	leaq	(%rdx,%rdx,4), %rdx
	leaq	(%rax,%rdx), %rdx
	andl	$255, %edi
	addq	%rdi, %rdx
	movabsq	$-3689348814741910323, %r10
	movq	%rdx, %rax
	mulq	%r10
	shrq	$3, %rdx
	movslq	%esi, %rsi
	testq	%rsi, %rsi
	je	.L99
	xorl	%eax, %eax
	movl	$127, %r11d
.L102:
	movq	%r11, %rdi
	subq	%rdx, %rdi
	addq	%rdi, %rdi
	leaq	-256(%rdx,%rdx), %r10
	cmpq	$127, %rdx
	movq	%rdi, %rdx
	cmova	%r10, %rdx
	addq	$1, %rax
	cmpq	%rsi, %rax
	jb	.L102
.L99:
	movslq	%ecx, %rcx
	testq	%rcx, %rcx
	je	.L103
	xorl	%eax, %eax
.L104:
	imulq	%rdx, %rdx
	shrq	$8, %rdx
	addq	$1, %rax
	cmpq	%rcx, %rax
	jb	.L104
.L103:
	movl	%r9d, %ecx
	shrq	%cl, %rdx
	salq	%cl, %rdx
	testb	$1, %r8b
	je	.L108
	leaq	(%rdx,%rdx), %rcx
	movl	$255, %eax
	cmpq	$255, %rcx
	cmovbe	%rcx, %rax
.L105:
	testb	$2, %r8b
	je	.L109
	leaq	(%rdx,%rdx), %rsi
	movl	$255, %ecx
	cmpq	$255, %rsi
	cmovbe	%rsi, %rcx
.L106:
	andb	$4, %r8b
	je	.L107
	leaq	(%rdx,%rdx), %rsi
	movl	$255, %edx
	cmpq	$255, %rsi
	cmovbe	%rsi, %rdx
.L107:
	salq	$8, %rcx
	salq	$16, %rax
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L109:
	.cfi_restore_state
	movq	%rdx, %rcx
	jmp	.L106
.L108:
	movq	%rdx, %rax
	jmp	.L105
	.cfi_endproc
.LFE26:
	.size	alien_toongs2, .-alien_toongs2
.globl alien_toongs
	.type	alien_toongs, @function
alien_toongs:
.LFB27:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movl	%edx, %r8d
	movq	%rdi, %rax
	andl	$16711680, %eax
	shrq	$14, %rax
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movzbl	%bh, %edx
	leaq	(%rdx,%rdx,4), %rdx
	leaq	(%rax,%rdx), %rdx
	andl	$255, %edi
	addq	%rdi, %rdx
	movabsq	$-3689348814741910323, %r9
	movq	%rdx, %rax
	mulq	%r9
	shrq	$3, %rdx
	movslq	%esi, %rsi
	testq	%rsi, %rsi
	je	.L114
	xorl	%eax, %eax
	movl	$127, %r10d
.L117:
	movq	%r10, %rdi
	subq	%rdx, %rdi
	addq	%rdi, %rdi
	leaq	-256(%rdx,%rdx), %r9
	cmpq	$127, %rdx
	movq	%rdi, %rdx
	cmova	%r9, %rdx
	addq	$1, %rax
	cmpq	%rsi, %rax
	jb	.L117
.L114:
	movslq	%ecx, %rcx
	testq	%rcx, %rcx
	je	.L118
	xorl	%eax, %eax
.L119:
	imulq	%rdx, %rdx
	shrq	$8, %rdx
	addq	$1, %rax
	cmpq	%rcx, %rax
	jb	.L119
.L118:
	movl	%r8d, %ecx
	shrq	%cl, %rdx
	salq	%cl, %rdx
	leaq	16(%rdx,%rdx), %rax
	movl	$255, %ecx
	cmpq	$255, %rax
	cmovbe	%rax, %rcx
	cmpq	$39, %rdx
	ja	.L124
	movl	$2621440, %eax
	salq	$8, %rdx
	addq	%rdx, %rcx
	leaq	(%rcx,%rax), %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L124:
	.cfi_restore_state
	movq	%rdx, %rax
	salq	$16, %rax
	salq	$8, %rdx
	addq	%rdx, %rcx
	leaq	(%rcx,%rax), %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE27:
	.size	alien_toongs, .-alien_toongs
.globl alien_toon
	.type	alien_toon, @function
alien_toon:
.LFB28:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %r9
	andl	$16711680, %r9d
	shrq	$16, %r9
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movzbl	%bh, %eax
	andl	$255, %edi
	movslq	%esi, %rsi
	testq	%rsi, %rsi
	je	.L126
	xorl	%r10d, %r10d
	movl	$127, %r8d
.L133:
	movq	%r8, %rbx
	subq	%r9, %rbx
	addq	%rbx, %rbx
	leaq	-256(%r9,%r9), %r11
	cmpq	$127, %r9
	movq	%rbx, %r9
	cmova	%r11, %r9
	movq	%r8, %rbx
	subq	%rax, %rbx
	addq	%rbx, %rbx
	leaq	-256(%rax,%rax), %r11
	cmpq	$127, %rax
	movq	%rbx, %rax
	cmova	%r11, %rax
	movq	%r8, %r11
	subq	%rdi, %r11
	addq	%r11, %r11
	leaq	-256(%rdi,%rdi), %rbx
	cmpq	$127, %rdi
	movq	%r11, %rdi
	cmova	%rbx, %rdi
	addq	$1, %r10
	cmpq	%rsi, %r10
	jb	.L133
.L126:
	leaq	(%r9,%r9,4), %rbx
	leaq	(%rbx,%rax,2), %r8
	addq	%rdi, %r8
	shrq	$3, %r8
	leaq	(%rax,%rax,4), %rbx
	leaq	(%rbx,%rdi,2), %rsi
	addq	%r9, %rsi
	shrq	$3, %rsi
	leaq	(%rdi,%rdi,4), %rbx
	leaq	(%rbx,%r9,2), %rbx
	leaq	(%rbx,%rax), %rax
	shrq	$3, %rax
	movslq	%ecx, %rcx
	testq	%rcx, %rcx
	je	.L134
	xorl	%edi, %edi
.L135:
	imulq	%r8, %r8
	shrq	$8, %r8
	imulq	%rsi, %rsi
	shrq	$8, %rsi
	imulq	%rax, %rax
	shrq	$8, %rax
	addq	$1, %rdi
	cmpq	%rcx, %rdi
	jb	.L135
.L134:
	movl	%edx, %ecx
	shrq	%cl, %r8
	salq	%cl, %r8
	shrq	%cl, %rsi
	salq	%cl, %rsi
	shrq	%cl, %rax
	salq	%cl, %rax
	addq	%rax, %rax
	movl	$255, %r9d
	cmpq	$255, %rax
	cmova	%r9, %rax
	cmpl	$1, %edx
	jle	.L136
	leal	-1(%rdx), %ecx
	movl	$1, %edi
	sall	%cl, %edi
	movslq	%edi, %rdi
	addq	%rdi, %r8
	cmpq	$255, %r8
	cmova	%r9, %r8
.L136:
	salq	$8, %rsi
	addq	%rsi, %rax
	salq	$16, %r8
	addq	%r8, %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE28:
	.size	alien_toon, .-alien_toon
.globl alien_gsi
	.type	alien_gsi, @function
alien_gsi:
.LFB29:
	.cfi_startproc
	movq	%rdi, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -24(%rsp)
	movq	%rdi, %rdx
	movzbl	%dh, %eax
	movq	%rax, -40(%rsp)
	andl	$255, %edi
	movq	%rdi, -56(%rsp)
	movq	-24(%rsp), %rax
	movq	-40(%rsp), %rcx
	movq	-56(%rsp), %rdx
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -72(%rsp)
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L143
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	ja	.L144
.L142:
	movq	-72(%rsp), %rax
	subq	$512, %rax
.L141:
	movq	%rax, %rcx
	salq	$16, %rcx
	movq	%rax, %rdx
	salq	$8, %rdx
	leaq	(%rcx,%rdx), %rdx
	leaq	(%rdx,%rax), %rax
	ret
.L144:
	movq	-72(%rsp), %rax
	cmpq	$511, %rax
	ja	.L142
	movq	-72(%rsp), %rdx
	movl	$511, %eax
	subq	%rdx, %rax
	jmp	.L141
.L143:
	movq	-72(%rsp), %rax
	jmp	.L141
	.cfi_endproc
.LFE29:
	.size	alien_gsi, .-alien_gsi
.globl alien_gsi6
	.type	alien_gsi6, @function
alien_gsi6:
.LFB30:
	.cfi_startproc
	movq	$0, -24(%rsp)
	movq	-24(%rsp), %rax
	cmpq	$5, %rax
	ja	.L146
	movl	$511, %esi
	jmp	.L151
.L153:
	movq	-24(%rsp), %rax
	movq	-88(%rsp), %rdx
	movq	%rdx, (%rdi,%rax,8)
.L148:
	movq	-24(%rsp), %rax
	addq	$1, %rax
	movq	%rax, -24(%rsp)
	movq	-24(%rsp), %rax
	cmpq	$5, %rax
	ja	.L146
.L151:
	movq	-24(%rsp), %rax
	movzbl	2(%rdi,%rax,8), %eax
	movq	%rax, -40(%rsp)
	movq	-24(%rsp), %rax
	movzbl	1(%rdi,%rax,8), %eax
	movq	%rax, -56(%rsp)
	movq	-24(%rsp), %rax
	movzbl	(%rdi,%rax,8), %eax
	movq	%rax, -72(%rsp)
	movq	-40(%rsp), %rax
	movq	-56(%rsp), %rcx
	movq	-72(%rsp), %rdx
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L153
	movq	-88(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L149
	movq	-88(%rsp), %rax
	cmpq	$511, %rax
	ja	.L149
	movq	-24(%rsp), %rax
	movq	-88(%rsp), %rdx
	movq	%rsi, %rcx
	subq	%rdx, %rcx
	movq	%rcx, (%rdi,%rax,8)
	jmp	.L148
.L149:
	movq	-24(%rsp), %rax
	movq	-88(%rsp), %rdx
	subq	$512, %rdx
	movq	%rdx, (%rdi,%rax,8)
	jmp	.L148
.L146:
	movq	8(%rdi), %rax
	addq	(%rdi), %rax
	shrq	%rax
	movq	%rax, -40(%rsp)
	movq	24(%rdi), %rax
	addq	16(%rdi), %rax
	shrq	%rax
	movq	%rax, -56(%rsp)
	movq	40(%rdi), %rax
	addq	32(%rdi), %rax
	shrq	%rax
	movq	%rax, -72(%rsp)
	movq	-40(%rsp), %rdx
	movq	-56(%rsp), %rax
	movq	-72(%rsp), %rcx
	salq	$16, %rdx
	salq	$8, %rax
	leaq	(%rdx,%rax), %rax
	addq	%rcx, %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rax
	ret
	.cfi_endproc
.LFE30:
	.size	alien_gsi6, .-alien_gsi6
.globl color_gsi3
	.type	color_gsi3, @function
color_gsi3:
.LFB31:
	.cfi_startproc
	andl	$65280, %esi
	andl	$16711680, %edi
	leaq	(%rsi,%rdi), %rdi
	andl	$255, %edx
	leaq	(%rdi,%rdx), %rax
	ret
	.cfi_endproc
.LFE31:
	.size	color_gsi3, .-color_gsi3
.globl alien_gsi3
	.type	alien_gsi3, @function
alien_gsi3:
.LFB32:
	.cfi_startproc
	movq	%rdi, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -24(%rsp)
	movq	%rdi, %rcx
	movzbl	%ch, %eax
	movq	%rax, -40(%rsp)
	andl	$255, %edi
	movq	%rdi, -56(%rsp)
	movq	-24(%rsp), %rax
	movq	-40(%rsp), %rdi
	movq	-56(%rsp), %rcx
	leaq	(%rdi,%rax), %rax
	addq	%rcx, %rax
	movq	%rax, -72(%rsp)
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	ja	.L156
	movq	-72(%rsp), %rdi
.L157:
	movq	%rsi, %rcx
	andl	$16711680, %ecx
	shrq	$16, %rcx
	movq	%rcx, -24(%rsp)
	movq	%rsi, %rcx
	movzbl	%ch, %eax
	movq	%rax, -40(%rsp)
	andl	$255, %esi
	movq	%rsi, -56(%rsp)
	movq	-24(%rsp), %rax
	movq	-40(%rsp), %rsi
	movq	-56(%rsp), %rcx
	leaq	(%rsi,%rax), %rax
	addq	%rcx, %rax
	movq	%rax, -72(%rsp)
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	ja	.L159
	movq	-72(%rsp), %rsi
.L160:
	movq	%rdx, %rcx
	andl	$16711680, %ecx
	shrq	$16, %rcx
	movq	%rcx, -24(%rsp)
	movzbl	%dh, %eax
	movq	%rax, -40(%rsp)
	andl	$255, %edx
	movq	%rdx, -56(%rsp)
	movq	-24(%rsp), %rax
	movq	-40(%rsp), %rcx
	movq	-56(%rsp), %rdx
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -72(%rsp)
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L165
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	ja	.L166
.L164:
	movq	-72(%rsp), %rax
	subq	$512, %rax
	salq	$8, %rsi
	salq	$16, %rdi
	addq	%rdi, %rsi
	leaq	(%rsi,%rax), %rax
	ret
.L156:
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L158
	movq	-72(%rsp), %rax
	cmpq	$511, %rax
	ja	.L158
	movq	-72(%rsp), %rax
	movl	$511, %edi
	subq	%rax, %rdi
	jmp	.L157
.L159:
	movq	-72(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L161
	movq	-72(%rsp), %rax
	cmpq	$511, %rax
	ja	.L161
	movq	-72(%rsp), %rax
	movl	$511, %esi
	subq	%rax, %rsi
	jmp	.L160
.L166:
	movq	-72(%rsp), %rax
	cmpq	$511, %rax
	ja	.L164
	movq	-72(%rsp), %rdx
	movl	$511, %eax
	subq	%rdx, %rax
	salq	$8, %rsi
	salq	$16, %rdi
	addq	%rdi, %rsi
	leaq	(%rsi,%rax), %rax
	ret
.L161:
	movq	-72(%rsp), %rsi
	subq	$512, %rsi
	jmp	.L160
.L158:
	movq	-72(%rsp), %rdi
	subq	$512, %rdi
	jmp	.L157
.L165:
	movq	-72(%rsp), %rax
	salq	$8, %rsi
	salq	$16, %rdi
	addq	%rdi, %rsi
	leaq	(%rsi,%rax), %rax
	ret
	.cfi_endproc
.LFE32:
	.size	alien_gsi3, .-alien_gsi3
.globl alien_sine
	.type	alien_sine, @function
alien_sine:
.LFB33:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$80, %rsp
	.cfi_def_cfa_offset 96
	movsd	.LC0(%rip), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L168
	.cfi_offset 3, -16
	cvttsd2siq	%xmm0, %rbx
.L169:
	movq	%rbx, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC1(%rip), %xmm3
	divsd	%xmm3, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	call	sin@PLT
	movsd	.LC4(%rip), %xmm6
	addsd	%xmm0, %xmm6
	mulsd	.LC5(%rip), %xmm6
	movzbl	%bh, %eax
	cvtsi2sdq	%rax, %xmm0
	movsd	(%rsp), %xmm3
	divsd	%xmm3, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm6, 32(%rsp)
	call	sin@PLT
	movsd	.LC4(%rip), %xmm5
	addsd	%xmm0, %xmm5
	mulsd	.LC5(%rip), %xmm5
	andl	$255, %ebx
	cvtsi2sdq	%rbx, %xmm0
	movsd	(%rsp), %xmm3
	divsd	%xmm3, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm5, 48(%rsp)
	call	sin@PLT
	movsd	.LC4(%rip), %xmm4
	addsd	%xmm0, %xmm4
	mulsd	.LC5(%rip), %xmm4
	movsd	32(%rsp), %xmm6
	movapd	%xmm6, %xmm1
	movapd	%xmm6, %xmm0
	movsd	%xmm4, 64(%rsp)
	call	pow@PLT
	movapd	%xmm0, %xmm6
	movsd	48(%rsp), %xmm5
	movapd	%xmm5, %xmm1
	movapd	%xmm5, %xmm0
	movsd	%xmm6, 32(%rsp)
	call	pow@PLT
	movapd	%xmm0, %xmm5
	movsd	64(%rsp), %xmm4
	movapd	%xmm4, %xmm1
	movapd	%xmm4, %xmm0
	movsd	%xmm5, 48(%rsp)
	call	pow@PLT
	movsd	.LC6(%rip), %xmm1
	movsd	32(%rsp), %xmm6
	subsd	%xmm1, %xmm6
	movsd	48(%rsp), %xmm5
	subsd	%xmm1, %xmm5
	subsd	%xmm1, %xmm0
	movsd	.LC7(%rip), %xmm1
	mulsd	%xmm1, %xmm6
	movsd	(%rsp), %xmm3
	mulsd	%xmm3, %xmm6
	movsd	16(%rsp), %xmm2
	ucomisd	%xmm2, %xmm6
	jae	.L176
	cvttsd2siq	%xmm6, %rbx
.L177:
	mulsd	%xmm1, %xmm5
	mulsd	%xmm3, %xmm5
	ucomisd	%xmm2, %xmm5
	jae	.L178
	cvttsd2siq	%xmm5, %rcx
.L179:
	mulsd	%xmm1, %xmm0
	mulsd	%xmm3, %xmm0
	ucomisd	%xmm2, %xmm0
	jae	.L180
	cvttsd2siq	%xmm0, %rdx
.L181:
	movl	$255, %eax
	cmpq	$255, %rbx
	cmova	%rax, %rbx
	salq	$16, %rbx
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	leaq	(%rbx,%rcx), %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	%rcx, %rax
	js	.L182
	cvtsi2sdq	%rax, %xmm0
	addq	$80, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L168:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rbx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rbx
	jmp	.L169
.L180:
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L181
.L178:
	subsd	%xmm2, %xmm5
	cvttsd2siq	%xmm5, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L179
.L176:
	subsd	%xmm2, %xmm6
	cvttsd2siq	%xmm6, %rbx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rbx
	jmp	.L177
.L182:
	movq	%rax, %rsi
	shrq	%rsi
	andl	$1, %eax
	orq	%rax, %rsi
	cvtsi2sdq	%rsi, %xmm0
	addsd	%xmm0, %xmm0
	addq	$80, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE33:
	.size	alien_sine, .-alien_sine
.globl alien_sine_ex
	.type	alien_sine_ex, @function
alien_sine_ex:
.LFB34:
	.cfi_startproc
	subq	$136, %rsp
	.cfi_def_cfa_offset 144
	movsd	.LC0(%rip), %xmm7
	ucomisd	%xmm7, %xmm0
	jae	.L185
	cvttsd2siq	%xmm0, %rax
.L186:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movsd	.LC1(%rip), %xmm8
	divsd	%xmm8, %xmm0
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm10
	divsd	%xmm8, %xmm10
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm9
	divsd	%xmm8, %xmm9
	mulsd	%xmm0, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	addsd	%xmm2, %xmm0
	movsd	%xmm3, 96(%rsp)
	movsd	%xmm4, 80(%rsp)
	movsd	%xmm5, 48(%rsp)
	movsd	%xmm6, 32(%rsp)
	movsd	%xmm7, 16(%rsp)
	movsd	%xmm8, (%rsp)
	movsd	%xmm9, 64(%rsp)
	movsd	%xmm10, 112(%rsp)
	call	sin@PLT
	movapd	%xmm0, %xmm2
	movsd	112(%rsp), %xmm10
	mulsd	%xmm10, %xmm10
	addsd	%xmm10, %xmm10
	mulsd	.LC3(%rip), %xmm10
	movsd	96(%rsp), %xmm3
	mulsd	%xmm3, %xmm10
	movsd	80(%rsp), %xmm4
	addsd	%xmm4, %xmm10
	movapd	%xmm10, %xmm0
	movsd	%xmm2, 96(%rsp)
	call	sin@PLT
	movapd	%xmm0, %xmm1
	movsd	64(%rsp), %xmm9
	mulsd	%xmm9, %xmm9
	addsd	%xmm9, %xmm9
	mulsd	.LC3(%rip), %xmm9
	movsd	48(%rsp), %xmm5
	mulsd	%xmm5, %xmm9
	movsd	32(%rsp), %xmm6
	addsd	%xmm6, %xmm9
	movapd	%xmm9, %xmm0
	movsd	%xmm1, 112(%rsp)
	call	sin@PLT
	movsd	.LC4(%rip), %xmm4
	movsd	96(%rsp), %xmm2
	addsd	%xmm4, %xmm2
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm2
	movsd	(%rsp), %xmm8
	mulsd	%xmm8, %xmm2
	movsd	16(%rsp), %xmm7
	ucomisd	%xmm7, %xmm2
	movsd	112(%rsp), %xmm1
	jae	.L193
	cvttsd2siq	%xmm2, %rsi
.L194:
	addsd	%xmm4, %xmm1
	mulsd	%xmm3, %xmm1
	mulsd	%xmm8, %xmm1
	ucomisd	%xmm7, %xmm1
	jae	.L195
	cvttsd2siq	%xmm1, %rcx
.L196:
	movapd	%xmm4, %xmm1
	addsd	%xmm0, %xmm1
	mulsd	%xmm3, %xmm1
	mulsd	%xmm8, %xmm1
	ucomisd	%xmm7, %xmm1
	jae	.L197
	cvttsd2siq	%xmm1, %rdx
.L198:
	movl	$255, %eax
	cmpq	$255, %rsi
	cmova	%rax, %rsi
	salq	$16, %rsi
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	leaq	(%rsi,%rcx), %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	%rcx, %rax
	js	.L199
	cvtsi2sdq	%rax, %xmm0
	addq	$136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L185:
	.cfi_restore_state
	subsd	%xmm7, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L186
.L197:
	subsd	%xmm7, %xmm1
	cvttsd2siq	%xmm1, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L198
.L195:
	subsd	%xmm7, %xmm1
	cvttsd2siq	%xmm1, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L196
.L193:
	subsd	%xmm7, %xmm2
	cvttsd2siq	%xmm2, %rsi
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rsi
	jmp	.L194
.L199:
	movq	%rax, %rdi
	shrq	%rdi
	andl	$1, %eax
	orq	%rax, %rdi
	cvtsi2sdq	%rdi, %xmm0
	addsd	%xmm0, %xmm0
	addq	$136, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE34:
	.size	alien_sine_ex, .-alien_sine_ex
.globl alien_col_mul
	.type	alien_col_mul, @function
alien_col_mul:
.LFB35:
	.cfi_startproc
	subq	$184, %rsp
	.cfi_def_cfa_offset 192
	movsd	%xmm5, 160(%rsp)
	movsd	%xmm6, 168(%rsp)
	movsd	.LC0(%rip), %xmm5
	ucomisd	%xmm5, %xmm0
	jae	.L202
	cvttsd2siq	%xmm0, %rax
.L203:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm10
	movsd	.LC1(%rip), %xmm7
	divsd	%xmm7, %xmm10
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm9
	divsd	%xmm7, %xmm9
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm8
	divsd	%xmm7, %xmm8
	sqrtsd	%xmm8, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L224
.L210:
	mulsd	%xmm10, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	addsd	%xmm2, %xmm0
	movsd	%xmm3, 80(%rsp)
	movsd	%xmm4, 64(%rsp)
	movsd	%xmm5, 16(%rsp)
	movsd	%xmm7, (%rsp)
	movsd	%xmm8, 32(%rsp)
	movsd	%xmm9, 48(%rsp)
	movsd	%xmm10, 96(%rsp)
	call	sin@PLT
	movsd	%xmm0, 144(%rsp)
	movsd	96(%rsp), %xmm10
	sqrtsd	%xmm10, %xmm0
	ucomisd	%xmm0, %xmm0
	movsd	80(%rsp), %xmm3
	movsd	64(%rsp), %xmm4
	movsd	16(%rsp), %xmm5
	movsd	(%rsp), %xmm7
	movsd	32(%rsp), %xmm8
	movsd	48(%rsp), %xmm9
	jp	.L225
.L212:
	mulsd	%xmm9, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	addsd	%xmm4, %xmm0
	movsd	%xmm5, 16(%rsp)
	movsd	%xmm7, (%rsp)
	movsd	%xmm8, 32(%rsp)
	movsd	%xmm9, 48(%rsp)
	call	sin@PLT
	movsd	%xmm0, 152(%rsp)
	movsd	48(%rsp), %xmm9
	sqrtsd	%xmm9, %xmm1
	ucomisd	%xmm1, %xmm1
	movsd	16(%rsp), %xmm5
	movsd	(%rsp), %xmm7
	movsd	32(%rsp), %xmm8
	jp	.L226
.L214:
	movapd	%xmm8, %xmm0
	mulsd	%xmm1, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	160(%rsp), %xmm0
	addsd	168(%rsp), %xmm0
	movsd	%xmm5, 16(%rsp)
	movsd	%xmm7, (%rsp)
	call	sin@PLT
	movsd	.LC4(%rip), %xmm1
	movsd	144(%rsp), %xmm2
	addsd	%xmm1, %xmm2
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm2
	movsd	(%rsp), %xmm7
	mulsd	%xmm7, %xmm2
	movsd	16(%rsp), %xmm5
	ucomisd	%xmm5, %xmm2
	jae	.L216
	cvttsd2siq	%xmm2, %rsi
.L217:
	movsd	152(%rsp), %xmm2
	addsd	%xmm1, %xmm2
	mulsd	%xmm3, %xmm2
	mulsd	%xmm7, %xmm2
	ucomisd	%xmm5, %xmm2
	jae	.L218
	cvttsd2siq	%xmm2, %rcx
.L219:
	addsd	%xmm0, %xmm1
	mulsd	%xmm3, %xmm1
	mulsd	%xmm7, %xmm1
	ucomisd	%xmm5, %xmm1
	jae	.L220
	cvttsd2siq	%xmm1, %rdx
.L221:
	movl	$255, %eax
	cmpq	$255, %rsi
	cmova	%rax, %rsi
	salq	$16, %rsi
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	leaq	(%rsi,%rcx), %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	%rcx, %rax
	js	.L222
	cvtsi2sdq	%rax, %xmm0
	addq	$184, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L202:
	.cfi_restore_state
	subsd	%xmm5, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L203
.L220:
	subsd	%xmm5, %xmm1
	cvttsd2siq	%xmm1, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L221
.L218:
	subsd	%xmm5, %xmm2
	cvttsd2siq	%xmm2, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L219
.L216:
	subsd	%xmm5, %xmm2
	cvttsd2siq	%xmm2, %rsi
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rsi
	jmp	.L217
.L222:
	movq	%rax, %rdi
	shrq	%rdi
	andl	$1, %eax
	orq	%rax, %rdi
	cvtsi2sdq	%rdi, %xmm0
	addsd	%xmm0, %xmm0
	addq	$184, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L226:
	.cfi_restore_state
	movapd	%xmm9, %xmm0
	call	sqrt@PLT
	movapd	%xmm0, %xmm1
	movsd	32(%rsp), %xmm8
	movsd	(%rsp), %xmm7
	movsd	16(%rsp), %xmm5
	jmp	.L214
.L225:
	movapd	%xmm10, %xmm0
	call	sqrt@PLT
	movsd	48(%rsp), %xmm9
	movsd	32(%rsp), %xmm8
	movsd	(%rsp), %xmm7
	movsd	16(%rsp), %xmm5
	movsd	64(%rsp), %xmm4
	movsd	80(%rsp), %xmm3
	jmp	.L212
.L224:
	movapd	%xmm8, %xmm0
	movsd	%xmm1, 128(%rsp)
	movsd	%xmm2, 112(%rsp)
	movsd	%xmm3, 80(%rsp)
	movsd	%xmm4, 64(%rsp)
	movsd	%xmm5, 16(%rsp)
	movsd	%xmm7, (%rsp)
	movsd	%xmm8, 32(%rsp)
	movsd	%xmm9, 48(%rsp)
	movsd	%xmm10, 96(%rsp)
	call	sqrt@PLT
	movsd	96(%rsp), %xmm10
	movsd	48(%rsp), %xmm9
	movsd	32(%rsp), %xmm8
	movsd	(%rsp), %xmm7
	movsd	16(%rsp), %xmm5
	movsd	64(%rsp), %xmm4
	movsd	80(%rsp), %xmm3
	movsd	112(%rsp), %xmm2
	movsd	128(%rsp), %xmm1
	jmp	.L210
	.cfi_endproc
.LFE35:
	.size	alien_col_mul, .-alien_col_mul
.globl color_mul
	.type	color_mul, @function
color_mul:
.LFB36:
	.cfi_startproc
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L228
	cvttsd2siq	%xmm0, %rax
.L229:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm4
	movsd	.LC1(%rip), %xmm2
	divsd	%xmm2, %xmm4
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm3
	divsd	%xmm2, %xmm3
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm0
	divsd	%xmm2, %xmm0
	movapd	%xmm4, %xmm5
	mulsd	%xmm3, %xmm5
	mulsd	%xmm2, %xmm5
	ucomisd	%xmm1, %xmm5
	jae	.L236
	cvttsd2siq	%xmm5, %rsi
.L237:
	mulsd	%xmm0, %xmm3
	mulsd	%xmm2, %xmm3
	ucomisd	%xmm1, %xmm3
	jae	.L238
	cvttsd2siq	%xmm3, %rcx
.L239:
	mulsd	%xmm4, %xmm0
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L240
	cvttsd2siq	%xmm0, %rdx
.L241:
	movl	$255, %eax
	cmpq	$255, %rsi
	cmova	%rax, %rsi
	salq	$16, %rsi
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	leaq	(%rsi,%rcx), %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	%rcx, %rax
	js	.L242
	cvtsi2sdq	%rax, %xmm0
	ret
.L228:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L229
.L240:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L241
.L238:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L239
.L236:
	subsd	%xmm1, %xmm5
	cvttsd2siq	%xmm5, %rsi
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rsi
	jmp	.L237
.L242:
	movq	%rax, %rdi
	shrq	%rdi
	andl	$1, %eax
	orq	%rax, %rdi
	cvtsi2sdq	%rdi, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.cfi_endproc
.LFE36:
	.size	color_mul, .-color_mul
.globl color_invi3
	.type	color_invi3, @function
color_invi3:
.LFB37:
	.cfi_startproc
	movq	%rdi, %rax
	notq	%rax
	andl	$255, %eax
	addq	$255, %rax
	movq	%rdi, %rcx
	movzbl	%ch, %edi
	subq	%rdi, %rax
	shrq	%rax
	cmpq	$127, %rax
	ja	.L245
	movl	$127, %ecx
	subq	%rax, %rcx
	leaq	(%rcx,%rcx), %rax
.L246:
	movq	%rsi, %rcx
	notq	%rcx
	andl	$255, %ecx
	addq	$255, %rcx
	andl	$16711680, %esi
	shrq	$16, %rsi
	subq	%rsi, %rcx
	shrq	%rcx
	cmpq	$127, %rcx
	ja	.L247
	movl	$127, %esi
	subq	%rcx, %rsi
	addq	%rsi, %rsi
.L248:
	movzbl	%dh, %ecx
	negq	%rcx
	andl	$16711680, %edx
	shrq	$16, %rdx
	subq	%rdx, %rcx
	addq	$510, %rcx
	shrq	%rcx
	cmpq	$127, %rcx
	jbe	.L251
	leaq	-256(%rcx,%rcx), %rdx
.L250:
	movq	%rsi, %rcx
	salq	$8, %rcx
	salq	$16, %rax
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	ret
.L245:
	leaq	-256(%rax,%rax), %rax
	jmp	.L246
.L247:
	leaq	-256(%rcx,%rcx), %rsi
	jmp	.L248
.L251:
	movl	$127, %edx
	subq	%rcx, %rdx
	addq	%rdx, %rdx
	jmp	.L250
	.cfi_endproc
.LFE37:
	.size	color_invi3, .-color_invi3
.globl color_invi
	.type	color_invi, @function
color_invi:
.LFB38:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	movq	%rdi, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	movl	$255, %edx
	movq	%rdx, %rcx
	subq	%rax, %rcx
	movq	%rdi, %rbx
	.cfi_offset 3, -16
	movzbl	%bh, %eax
	subq	%rax, %rdx
	notq	%rdi
	andl	$255, %edi
	leaq	(%rdx,%rdi), %rax
	shrq	%rax
	leaq	(%rcx,%rdi), %rdi
	shrq	%rdi
	addq	%rcx, %rdx
	shrq	%rdx
	cmpq	$127, %rax
	ja	.L253
	movl	$127, %ecx
	subq	%rax, %rcx
	leaq	(%rcx,%rcx), %rax
	cmpq	$127, %rdi
	ja	.L255
.L260:
	movl	$127, %ecx
	subq	%rdi, %rcx
	addq	%rcx, %rcx
	cmpq	$127, %rdx
	jbe	.L259
.L257:
	leaq	-256(%rdx,%rdx), %rdx
.L258:
	salq	$8, %rcx
	salq	$16, %rax
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L253:
	.cfi_restore_state
	leaq	-256(%rax,%rax), %rax
	cmpq	$127, %rdi
	jbe	.L260
.L255:
	leaq	-256(%rdi,%rdi), %rcx
	cmpq	$127, %rdx
	ja	.L257
.L259:
	movl	$127, %esi
	subq	%rdx, %rsi
	leaq	(%rsi,%rsi), %rdx
	jmp	.L258
	.cfi_endproc
.LFE38:
	.size	color_invi, .-color_invi
.globl color_inv2
	.type	color_inv2, @function
color_inv2:
.LFB39:
	.cfi_startproc
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L262
	cvttsd2siq	%xmm0, %rax
.L263:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movsd	.LC1(%rip), %xmm2
	divsd	%xmm2, %xmm0
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm5
	subsd	%xmm0, %xmm5
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm0
	divsd	%xmm2, %xmm0
	movapd	%xmm3, %xmm4
	subsd	%xmm0, %xmm4
	movapd	%xmm4, %xmm0
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm4
	divsd	%xmm2, %xmm4
	subsd	%xmm4, %xmm3
	movapd	%xmm3, %xmm4
	mulsd	%xmm0, %xmm4
	mulsd	%xmm2, %xmm4
	ucomisd	%xmm1, %xmm4
	jae	.L270
	cvttsd2siq	%xmm4, %rdx
.L271:
	salq	$16, %rdx
	mulsd	%xmm5, %xmm3
	mulsd	%xmm2, %xmm3
	ucomisd	%xmm1, %xmm3
	jae	.L272
	cvttsd2siq	%xmm3, %rcx
.L273:
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	%xmm5, %xmm0
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L274
	cvttsd2siq	%xmm0, %rax
	addq	%rdx, %rax
	js	.L276
.L278:
	cvtsi2sdq	%rax, %xmm0
	ret
.L262:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L263
.L274:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L278
.L276:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
.L272:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L273
.L270:
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L271
	.cfi_endproc
.LFE39:
	.size	color_inv2, .-color_inv2
.globl alien_inv
	.type	alien_inv, @function
alien_inv:
.LFB40:
	.cfi_startproc
	subq	$120, %rsp
	.cfi_def_cfa_offset 128
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L280
	cvttsd2siq	%xmm0, %rax
.L281:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movsd	.LC1(%rip), %xmm2
	divsd	%xmm2, %xmm0
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm4
	subsd	%xmm0, %xmm4
	movsd	%xmm4, 88(%rsp)
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm0
	divsd	%xmm2, %xmm0
	movapd	%xmm3, %xmm4
	subsd	%xmm0, %xmm4
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm0
	divsd	%xmm2, %xmm0
	movapd	%xmm3, %xmm5
	subsd	%xmm0, %xmm5
	movapd	%xmm5, %xmm0
	mulsd	%xmm4, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm1, 32(%rsp)
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 48(%rsp)
	movsd	%xmm5, 64(%rsp)
	call	cos@PLT
	movsd	%xmm0, 96(%rsp)
	movsd	64(%rsp), %xmm5
	movsd	88(%rsp), %xmm0
	mulsd	%xmm5, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	%xmm0, 104(%rsp)
	movsd	48(%rsp), %xmm4
	movsd	88(%rsp), %xmm0
	mulsd	%xmm4, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	(%rsp), %xmm3
	movapd	%xmm3, %xmm4
	subsd	96(%rsp), %xmm4
	movsd	.LC5(%rip), %xmm5
	mulsd	%xmm5, %xmm4
	movsd	16(%rsp), %xmm2
	mulsd	%xmm2, %xmm4
	movsd	32(%rsp), %xmm1
	ucomisd	%xmm1, %xmm4
	jae	.L288
	cvttsd2siq	%xmm4, %rdx
.L289:
	salq	$16, %rdx
	movapd	%xmm3, %xmm4
	subsd	104(%rsp), %xmm4
	mulsd	%xmm5, %xmm4
	mulsd	%xmm2, %xmm4
	ucomisd	%xmm1, %xmm4
	jae	.L290
	cvttsd2siq	%xmm4, %rcx
.L291:
	salq	$8, %rcx
	addq	%rcx, %rdx
	subsd	%xmm0, %xmm3
	mulsd	%xmm5, %xmm3
	mulsd	%xmm2, %xmm3
	ucomisd	%xmm1, %xmm3
	jae	.L292
	cvttsd2siq	%xmm3, %rax
	addq	%rdx, %rax
	js	.L294
.L296:
	cvtsi2sdq	%rax, %xmm0
	addq	$120, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L280:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L281
.L292:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L296
.L294:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$120, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L290:
	.cfi_restore_state
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L291
.L288:
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L289
	.cfi_endproc
.LFE40:
	.size	alien_inv, .-alien_inv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC9:
	.string	"x=%f, l=%f --> error\n"
	.text
.globl get_heat
	.type	get_heat, @function
get_heat:
.LFB41:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbx
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movq	%rsi, %rbp
	movq	%rdx, %r12
	pxor	%xmm2, %xmm2
	ucomisd	%xmm0, %xmm2
	ja	.L298
	movsd	.LC4(%rip), %xmm3
	ucomisd	%xmm3, %xmm0
	ja	.L298
	ucomisd	%xmm1, %xmm2
	ja	.L298
	ucomisd	%xmm3, %xmm1
	ja	.L298
	movsd	.LC10(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L322
	mulsd	.LC11(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	movapd	%xmm1, %xmm3
	movapd	%xmm2, %xmm4
.L304:
	movsd	%xmm3, (%rbx)
	movsd	%xmm0, 0(%rbp)
	movsd	%xmm4, (%r12)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L298:
	.cfi_restore_state
	leaq	.LC9(%rip), %rdi
	movl	$2, %eax
	call	printf@PLT
	movabsq	$4607182418800017408, %rcx
	movq	%rcx, (%r12)
	movabsq	$4607182418800017408, %rdx
	movq	%rdx, 0(%rbp)
	movabsq	$4607182418800017408, %rax
	movq	%rax, (%rbx)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L322:
	.cfi_restore_state
	movsd	.LC12(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jae	.L328
	movsd	.LC5(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L324
	movsd	.LC11(%rip), %xmm4
	mulsd	%xmm0, %xmm4
	subsd	.LC13(%rip), %xmm4
	mulsd	%xmm1, %xmm4
	movapd	%xmm1, %xmm0
	movapd	%xmm2, %xmm3
	jmp	.L304
.L328:
	mulsd	.LC11(%rip), %xmm0
	subsd	%xmm3, %xmm0
	subsd	%xmm0, %xmm3
	mulsd	%xmm1, %xmm3
	movapd	%xmm1, %xmm0
	movapd	%xmm2, %xmm4
	jmp	.L304
.L324:
	movsd	.LC14(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L325
	movsd	.LC11(%rip), %xmm4
	mulsd	%xmm0, %xmm4
	subsd	.LC2(%rip), %xmm4
	movapd	%xmm3, %xmm0
	subsd	%xmm4, %xmm0
	mulsd	%xmm1, %xmm0
	movapd	%xmm1, %xmm4
	movapd	%xmm2, %xmm3
	jmp	.L304
.L325:
	movsd	.LC15(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L326
	movsd	.LC11(%rip), %xmm3
	mulsd	%xmm0, %xmm3
	subsd	.LC16(%rip), %xmm3
	mulsd	%xmm1, %xmm3
	movapd	%xmm1, %xmm4
	movapd	%xmm2, %xmm0
	jmp	.L304
.L326:
	ucomisd	%xmm0, %xmm3
	jb	.L327
	mulsd	.LC11(%rip), %xmm0
	subsd	.LC17(%rip), %xmm0
	movapd	%xmm3, %xmm4
	subsd	%xmm0, %xmm4
	mulsd	%xmm1, %xmm4
	movapd	%xmm1, %xmm3
	movapd	%xmm2, %xmm0
	jmp	.L304
.L327:
	movapd	%xmm3, %xmm4
	movapd	%xmm3, %xmm0
	jmp	.L304
	.cfi_endproc
.LFE41:
	.size	get_heat, .-get_heat
.globl color_hue2
	.type	color_hue2, @function
color_hue2:
.LFB42:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movsd	.LC0(%rip), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L330
	cvttsd2siq	%xmm0, %rax
.L331:
	leaq	40(%rsp), %rdx
	leaq	48(%rsp), %rsi
	leaq	56(%rsp), %rdi
	movzbl	%ah, %ecx
	cvtsi2sdq	%rcx, %xmm1
	movsd	.LC1(%rip), %xmm3
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm0
	divsd	%xmm3, %xmm0
	divsd	%xmm3, %xmm1
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	call	get_heat@PLT
	movsd	(%rsp), %xmm3
	movsd	56(%rsp), %xmm0
	mulsd	%xmm3, %xmm0
	movsd	16(%rsp), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L336
	cvttsd2siq	%xmm0, %rdx
.L337:
	salq	$16, %rdx
	movsd	48(%rsp), %xmm0
	mulsd	%xmm3, %xmm0
	ucomisd	%xmm2, %xmm0
	jae	.L338
	cvttsd2siq	%xmm0, %rcx
.L339:
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	40(%rsp), %xmm3
	ucomisd	%xmm2, %xmm3
	jae	.L340
	cvttsd2siq	%xmm3, %rax
	addq	%rdx, %rax
	js	.L342
.L344:
	cvtsi2sdq	%rax, %xmm0
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L330:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L331
.L340:
	subsd	%xmm2, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L344
.L342:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L338:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L339
.L336:
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L337
	.cfi_endproc
.LFE42:
	.size	color_hue2, .-color_hue2
.globl color_hue
	.type	color_hue, @function
color_hue:
.LFB43:
	.cfi_startproc
	subq	$216, %rsp
	.cfi_def_cfa_offset 224
	movsd	.LC0(%rip), %xmm3
	ucomisd	%xmm3, %xmm0
	jae	.L346
	cvttsd2siq	%xmm0, %rax
.L347:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movsd	.LC1(%rip), %xmm4
	divsd	%xmm4, %xmm0
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm6
	divsd	%xmm4, %xmm6
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm5
	divsd	%xmm4, %xmm5
	movapd	%xmm0, %xmm2
	addsd	%xmm6, %xmm2
	addsd	%xmm5, %xmm2
	divsd	.LC2(%rip), %xmm2
	leaq	128(%rsp), %rdx
	leaq	168(%rsp), %rsi
	leaq	200(%rsp), %rdi
	movapd	%xmm2, %xmm1
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 16(%rsp)
	movsd	%xmm4, (%rsp)
	movsd	%xmm5, 48(%rsp)
	movsd	%xmm6, 64(%rsp)
	call	get_heat@PLT
	leaq	112(%rsp), %rdx
	leaq	160(%rsp), %rsi
	leaq	192(%rsp), %rdi
	movsd	32(%rsp), %xmm2
	movapd	%xmm2, %xmm1
	movsd	64(%rsp), %xmm6
	movapd	%xmm6, %xmm0
	call	get_heat@PLT
	leaq	96(%rsp), %rdx
	leaq	144(%rsp), %rsi
	leaq	176(%rsp), %rdi
	movsd	32(%rsp), %xmm2
	movapd	%xmm2, %xmm1
	movsd	48(%rsp), %xmm5
	movapd	%xmm5, %xmm0
	call	get_heat@PLT
	movsd	200(%rsp), %xmm1
	movsd	168(%rsp), %xmm0
	ucomisd	%xmm1, %xmm0
	movsd	16(%rsp), %xmm3
	movsd	(%rsp), %xmm4
	jbe	.L386
	movapd	%xmm1, %xmm0
	maxsd	128(%rsp), %xmm0
.L356:
	movsd	192(%rsp), %xmm2
	movsd	160(%rsp), %xmm1
	ucomisd	%xmm2, %xmm1
	jbe	.L387
	movapd	%xmm2, %xmm1
	maxsd	112(%rsp), %xmm1
.L361:
	movsd	176(%rsp), %xmm5
	movsd	144(%rsp), %xmm2
	ucomisd	%xmm5, %xmm2
	jbe	.L388
	maxsd	96(%rsp), %xmm5
	movsd	%xmm5, 88(%rsp)
.L366:
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm1, 64(%rsp)
	movsd	%xmm3, 16(%rsp)
	movsd	%xmm4, (%rsp)
	call	cos@PLT
	movapd	%xmm0, %xmm2
	movsd	64(%rsp), %xmm1
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	movsd	%xmm2, 32(%rsp)
	call	cos@PLT
	movapd	%xmm0, %xmm1
	movsd	.LC3(%rip), %xmm0
	mulsd	88(%rsp), %xmm0
	movsd	%xmm1, 64(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	mulsd	%xmm2, %xmm2
	movsd	.LC4(%rip), %xmm5
	movapd	%xmm5, %xmm6
	subsd	%xmm2, %xmm6
	movapd	%xmm6, %xmm2
	movsd	(%rsp), %xmm4
	mulsd	%xmm4, %xmm2
	movsd	16(%rsp), %xmm3
	ucomisd	%xmm3, %xmm2
	movsd	64(%rsp), %xmm1
	jae	.L369
	cvttsd2siq	%xmm2, %rdx
.L370:
	salq	$16, %rdx
	mulsd	%xmm1, %xmm1
	movapd	%xmm5, %xmm2
	subsd	%xmm1, %xmm2
	mulsd	%xmm4, %xmm2
	ucomisd	%xmm3, %xmm2
	jae	.L371
	cvttsd2siq	%xmm2, %rcx
.L372:
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	%xmm0, %xmm0
	movapd	%xmm5, %xmm1
	subsd	%xmm0, %xmm1
	mulsd	%xmm4, %xmm1
	ucomisd	%xmm3, %xmm1
	jae	.L373
	cvttsd2siq	%xmm1, %rax
	addq	%rdx, %rax
	js	.L375
.L389:
	cvtsi2sdq	%rax, %xmm0
	addq	$216, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L346:
	.cfi_restore_state
	subsd	%xmm3, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L347
.L373:
	subsd	%xmm3, %xmm1
	cvttsd2siq	%xmm1, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L389
.L375:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$216, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L371:
	.cfi_restore_state
	subsd	%xmm3, %xmm2
	cvttsd2siq	%xmm2, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L372
.L369:
	subsd	%xmm3, %xmm2
	cvttsd2siq	%xmm2, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L370
.L388:
	maxsd	96(%rsp), %xmm2
	movsd	%xmm2, 88(%rsp)
	jmp	.L366
.L387:
	maxsd	112(%rsp), %xmm1
	jmp	.L361
.L386:
	maxsd	128(%rsp), %xmm0
	jmp	.L356
	.cfi_endproc
.LFE43:
	.size	color_hue, .-color_hue
.globl color_complex
	.type	color_complex, @function
color_complex:
.LFB44:
	.cfi_startproc
	subq	$184, %rsp
	.cfi_def_cfa_offset 192
	movsd	.LC0(%rip), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L391
	cvttsd2siq	%xmm0, %rax
.L392:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm5
	movsd	.LC1(%rip), %xmm3
	divsd	%xmm3, %xmm5
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm1
	divsd	%xmm3, %xmm1
	movsd	.LC18(%rip), %xmm7
	divsd	%xmm7, %xmm1
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm4
	divsd	%xmm3, %xmm4
	divsd	%xmm7, %xmm4
	movapd	%xmm1, %xmm9
	mulsd	%xmm1, %xmm9
	movapd	%xmm4, %xmm8
	mulsd	%xmm4, %xmm8
	movapd	%xmm9, %xmm0
	addsd	%xmm8, %xmm0
	sqrtsd	%xmm0, %xmm10
	ucomisd	%xmm10, %xmm10
	jp	.L428
.L399:
	pxor	%xmm6, %xmm6
	ucomisd	%xmm6, %xmm10
	ja	.L429
.L401:
	divsd	%xmm7, %xmm5
	movapd	%xmm5, %xmm7
	mulsd	%xmm5, %xmm7
	addsd	%xmm7, %xmm9
	sqrtsd	%xmm9, %xmm1
	ucomisd	%xmm1, %xmm1
	jp	.L430
.L405:
	ucomisd	%xmm6, %xmm1
	ja	.L431
	movapd	%xmm6, %xmm5
.L407:
	addsd	%xmm8, %xmm7
	sqrtsd	%xmm7, %xmm9
	ucomisd	%xmm9, %xmm9
	jp	.L432
.L409:
	ucomisd	%xmm6, %xmm9
	ja	.L433
.L411:
	leaq	152(%rsp), %rdx
	leaq	160(%rsp), %rsi
	leaq	168(%rsp), %rdi
	movapd	%xmm5, %xmm0
	divsd	.LC3(%rip), %xmm0
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	call	get_heat@PLT
	movsd	(%rsp), %xmm3
	movsd	168(%rsp), %xmm0
	mulsd	%xmm3, %xmm0
	movsd	16(%rsp), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L415
	cvttsd2siq	%xmm0, %rdx
.L416:
	salq	$16, %rdx
	movsd	160(%rsp), %xmm0
	mulsd	%xmm3, %xmm0
	ucomisd	%xmm2, %xmm0
	jae	.L417
	cvttsd2siq	%xmm0, %rcx
.L418:
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	152(%rsp), %xmm3
	ucomisd	%xmm2, %xmm3
	jae	.L419
	cvttsd2siq	%xmm3, %rax
	addq	%rdx, %rax
	js	.L421
.L434:
	cvtsi2sdq	%rax, %xmm0
	addq	$184, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L391:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L392
.L419:
	subsd	%xmm2, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L434
.L421:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$184, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L417:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L418
.L415:
	subsd	%xmm2, %xmm0
	cvttsd2siq	%xmm0, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L416
.L433:
	movapd	%xmm4, %xmm0
	divsd	%xmm9, %xmm0
	ucomisd	.LC4(%rip), %xmm0
	ja	.L413
	movsd	.LC19(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jbe	.L411
.L413:
	movsd	%xmm1, 48(%rsp)
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm5, 32(%rsp)
	call	acos@PLT
	movsd	32(%rsp), %xmm5
	movsd	(%rsp), %xmm3
	movsd	16(%rsp), %xmm2
	movsd	48(%rsp), %xmm1
	jmp	.L411
.L431:
	movapd	%xmm5, %xmm0
	divsd	%xmm1, %xmm0
	movsd	%xmm1, 48(%rsp)
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 64(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 96(%rsp)
	movsd	%xmm8, 112(%rsp)
	call	acos@PLT
	movapd	%xmm0, %xmm5
	movsd	48(%rsp), %xmm1
	movsd	16(%rsp), %xmm2
	movsd	(%rsp), %xmm3
	movsd	64(%rsp), %xmm4
	movsd	80(%rsp), %xmm6
	movsd	96(%rsp), %xmm7
	movsd	112(%rsp), %xmm8
	jmp	.L407
.L429:
	movapd	%xmm1, %xmm0
	divsd	%xmm10, %xmm0
	ucomisd	.LC4(%rip), %xmm0
	ja	.L403
	movsd	.LC19(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	jbe	.L401
.L403:
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 64(%rsp)
	movsd	%xmm5, 32(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 96(%rsp)
	movsd	%xmm8, 112(%rsp)
	movsd	%xmm9, 128(%rsp)
	call	acos@PLT
	movsd	128(%rsp), %xmm9
	movsd	112(%rsp), %xmm8
	movsd	96(%rsp), %xmm7
	movsd	80(%rsp), %xmm6
	movsd	32(%rsp), %xmm5
	movsd	64(%rsp), %xmm4
	movsd	(%rsp), %xmm3
	movsd	16(%rsp), %xmm2
	jmp	.L401
.L428:
	movsd	%xmm1, 48(%rsp)
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 64(%rsp)
	movsd	%xmm5, 32(%rsp)
	movsd	%xmm7, 96(%rsp)
	movsd	%xmm8, 112(%rsp)
	movsd	%xmm9, 128(%rsp)
	call	sqrt@PLT
	movapd	%xmm0, %xmm10
	movsd	128(%rsp), %xmm9
	movsd	112(%rsp), %xmm8
	movsd	96(%rsp), %xmm7
	movsd	32(%rsp), %xmm5
	movsd	64(%rsp), %xmm4
	movsd	(%rsp), %xmm3
	movsd	16(%rsp), %xmm2
	movsd	48(%rsp), %xmm1
	jmp	.L399
.L432:
	movapd	%xmm7, %xmm0
	movsd	%xmm1, 48(%rsp)
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 64(%rsp)
	movsd	%xmm5, 32(%rsp)
	movsd	%xmm6, 80(%rsp)
	call	sqrt@PLT
	movapd	%xmm0, %xmm9
	movsd	80(%rsp), %xmm6
	movsd	32(%rsp), %xmm5
	movsd	64(%rsp), %xmm4
	movsd	(%rsp), %xmm3
	movsd	16(%rsp), %xmm2
	movsd	48(%rsp), %xmm1
	jmp	.L409
.L430:
	movapd	%xmm9, %xmm0
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	movsd	%xmm4, 64(%rsp)
	movsd	%xmm5, 32(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 96(%rsp)
	movsd	%xmm8, 112(%rsp)
	call	sqrt@PLT
	movapd	%xmm0, %xmm1
	movsd	112(%rsp), %xmm8
	movsd	96(%rsp), %xmm7
	movsd	80(%rsp), %xmm6
	movsd	32(%rsp), %xmm5
	movsd	64(%rsp), %xmm4
	movsd	(%rsp), %xmm3
	movsd	16(%rsp), %xmm2
	jmp	.L405
	.cfi_endproc
.LFE44:
	.size	color_complex, .-color_complex
.globl color_angle
	.type	color_angle, @function
color_angle:
.LFB45:
	.cfi_startproc
	subq	$136, %rsp
	.cfi_def_cfa_offset 144
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L436
	cvttsd2siq	%xmm0, %rax
.L437:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm3
	movsd	.LC1(%rip), %xmm2
	divsd	%xmm2, %xmm3
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm5
	divsd	%xmm2, %xmm5
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm4
	divsd	%xmm2, %xmm4
	movapd	%xmm5, %xmm8
	mulsd	%xmm5, %xmm8
	movapd	%xmm4, %xmm9
	mulsd	%xmm4, %xmm9
	movapd	%xmm8, %xmm6
	addsd	%xmm9, %xmm6
	sqrtsd	%xmm6, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L473
.L444:
	pxor	%xmm6, %xmm6
	ucomisd	%xmm6, %xmm0
	jbe	.L470
	divsd	%xmm0, %xmm5
.L446:
	movapd	%xmm3, %xmm7
	mulsd	%xmm3, %xmm7
	addsd	%xmm7, %xmm9
	sqrtsd	%xmm9, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L474
.L448:
	ucomisd	%xmm6, %xmm0
	jbe	.L471
	divsd	%xmm0, %xmm4
.L450:
	addsd	%xmm8, %xmm7
	sqrtsd	%xmm7, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L475
.L452:
	ucomisd	%xmm6, %xmm0
	jbe	.L472
	divsd	%xmm0, %xmm3
.L454:
	movapd	%xmm5, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 64(%rsp)
	movsd	%xmm4, 32(%rsp)
	call	asin@PLT
	movapd	%xmm0, %xmm5
	movsd	32(%rsp), %xmm4
	movapd	%xmm4, %xmm0
	movsd	%xmm5, 48(%rsp)
	call	asin@PLT
	movapd	%xmm0, %xmm4
	movsd	64(%rsp), %xmm3
	movapd	%xmm3, %xmm0
	movsd	%xmm4, 32(%rsp)
	call	asin@PLT
	movsd	.LC3(%rip), %xmm3
	movsd	48(%rsp), %xmm5
	divsd	%xmm3, %xmm5
	movsd	(%rsp), %xmm2
	mulsd	%xmm2, %xmm5
	movsd	16(%rsp), %xmm1
	ucomisd	%xmm1, %xmm5
	movsd	32(%rsp), %xmm4
	jae	.L456
	cvttsd2siq	%xmm5, %rdx
.L457:
	salq	$16, %rdx
	divsd	%xmm3, %xmm4
	mulsd	%xmm2, %xmm4
	ucomisd	%xmm1, %xmm4
	jae	.L458
	cvttsd2siq	%xmm4, %rcx
.L459:
	salq	$8, %rcx
	addq	%rcx, %rdx
	divsd	%xmm3, %xmm0
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L460
	cvttsd2siq	%xmm0, %rax
	addq	%rdx, %rax
	js	.L462
.L476:
	cvtsi2sdq	%rax, %xmm0
	addq	$136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L436:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L437
.L460:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L476
.L462:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L458:
	.cfi_restore_state
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L459
.L456:
	subsd	%xmm1, %xmm5
	cvttsd2siq	%xmm5, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L457
.L471:
	movapd	%xmm6, %xmm4
	jmp	.L450
.L470:
	movapd	%xmm6, %xmm5
	jmp	.L446
.L472:
	movapd	%xmm6, %xmm3
	jmp	.L454
.L473:
	movapd	%xmm6, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 64(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm5, 48(%rsp)
	movsd	%xmm8, 96(%rsp)
	movsd	%xmm9, 112(%rsp)
	call	sqrt@PLT
	movsd	112(%rsp), %xmm9
	movsd	96(%rsp), %xmm8
	movsd	48(%rsp), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	64(%rsp), %xmm3
	movsd	(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L444
.L475:
	movapd	%xmm7, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 64(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm5, 48(%rsp)
	movsd	%xmm6, 80(%rsp)
	call	sqrt@PLT
	movsd	80(%rsp), %xmm6
	movsd	48(%rsp), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	64(%rsp), %xmm3
	movsd	(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L452
.L474:
	movapd	%xmm9, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 64(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm5, 48(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 112(%rsp)
	movsd	%xmm8, 96(%rsp)
	call	sqrt@PLT
	movsd	96(%rsp), %xmm8
	movsd	112(%rsp), %xmm7
	movsd	80(%rsp), %xmm6
	movsd	48(%rsp), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	64(%rsp), %xmm3
	movsd	(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L448
	.cfi_endproc
.LFE45:
	.size	color_angle, .-color_angle
.globl color_heati2
	.type	color_heati2, @function
color_heati2:
.LFB46:
	.cfi_startproc
	movq	%rdi, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -24(%rsp)
	movq	%rdi, %rdx
	movzbl	%dh, %eax
	movq	%rax, -40(%rsp)
	andl	$255, %edi
	movq	%rdi, -56(%rsp)
	movq	-24(%rsp), %rax
	movq	-40(%rsp), %rcx
	movq	-56(%rsp), %rdx
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -72(%rsp)
	movq	-72(%rsp), %rax
	movq	%rax, -88(%rsp)
	movq	-72(%rsp), %rax
	cmpq	$110, %rax
	jbe	.L485
	movq	-72(%rsp), %rax
	cmpq	$220, %rax
	jbe	.L486
	movq	-72(%rsp), %rax
	cmpq	$330, %rax
	jbe	.L487
	movq	-72(%rsp), %rax
	cmpq	$440, %rax
	jbe	.L488
	movq	-72(%rsp), %rax
	cmpq	$550, %rax
	jbe	.L489
	movq	-72(%rsp), %rax
	cmpq	$660, %rax
	jbe	.L490
	movq	-72(%rsp), %rax
	leaq	-2640(,%rax,4), %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rdx
	movabsq	$-6148914691236517205, %rcx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -24(%rsp)
	movq	-88(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	$2, %rdx
	movq	%rdx, -40(%rsp)
	movq	-88(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	$2, %rdx
	movq	%rdx, -56(%rsp)
	jmp	.L479
.L486:
	movq	-72(%rsp), %rax
	leaq	-220(%rax,%rax), %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rdx
	movabsq	$-6148914691236517205, %rcx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	movl	$73, %eax
	subq	%rdx, %rax
	movq	%rax, -24(%rsp)
	movq	$0, -40(%rsp)
	movq	-88(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	$146, %rdx
	movq	%rdx, -56(%rsp)
.L479:
	movq	-24(%rsp), %rax
	addq	$16, %rax
	movq	%rax, -24(%rsp)
	movq	-40(%rsp), %rax
	addq	$16, %rax
	movq	%rax, -40(%rsp)
	movq	-56(%rsp), %rax
	addq	$16, %rax
	movq	%rax, -56(%rsp)
	movq	-24(%rsp), %rdx
	movq	-40(%rsp), %rax
	movq	-56(%rsp), %rcx
	salq	$16, %rdx
	salq	$8, %rax
	leaq	(%rdx,%rax), %rax
	addq	%rcx, %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rax
	ret
.L485:
	movq	-72(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -88(%rsp)
	movq	$73, -24(%rsp)
	movq	-88(%rsp), %rdx
	movabsq	$-6148914691236517205, %rcx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	movl	$73, %eax
	subq	%rdx, %rax
	movq	%rax, -40(%rsp)
	movq	-88(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	$73, %rdx
	movq	%rdx, -56(%rsp)
	jmp	.L479
.L488:
	movq	-72(%rsp), %rax
	subq	$330, %rax
	movq	%rax, -88(%rsp)
	movq	$0, -24(%rsp)
	movq	-88(%rsp), %rax
	addq	$110, %rax
	movq	%rax, -40(%rsp)
	movq	-88(%rsp), %rdx
	movl	$110, %eax
	subq	%rdx, %rax
	movq	%rax, -56(%rsp)
	jmp	.L479
.L487:
	movq	-72(%rsp), %rax
	subq	$220, %rax
	movq	%rax, -88(%rsp)
	movq	$0, -24(%rsp)
	movq	-88(%rsp), %rax
	movq	%rax, -40(%rsp)
	movq	-88(%rsp), %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -56(%rsp)
	jmp	.L479
.L489:
	movq	-72(%rsp), %rax
	subq	$440, %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rax
	movq	%rax, -24(%rsp)
	movq	-88(%rsp), %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -40(%rsp)
	movq	$0, -56(%rsp)
	jmp	.L479
.L490:
	movq	-72(%rsp), %rax
	subq	$550, %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rax
	addq	$110, %rax
	movq	%rax, -24(%rsp)
	movq	-88(%rsp), %rdx
	movl	$110, %eax
	subq	%rdx, %rax
	movq	%rax, -40(%rsp)
	movq	$0, -56(%rsp)
	jmp	.L479
	.cfi_endproc
.LFE46:
	.size	color_heati2, .-color_heati2
.globl color_heati
	.type	color_heati, @function
color_heati:
.LFB47:
	.cfi_startproc
	movq	%rdi, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -24(%rsp)
	movq	%rdi, %rdx
	movzbl	%dh, %eax
	movq	%rax, -40(%rsp)
	andl	$255, %edi
	movq	%rdi, -56(%rsp)
	movq	-24(%rsp), %rax
	movq	-40(%rsp), %rcx
	movq	-56(%rsp), %rdx
	leaq	(%rcx,%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -72(%rsp)
	movq	-72(%rsp), %rax
	movq	%rax, -88(%rsp)
	movq	-72(%rsp), %rax
	cmpq	$110, %rax
	jbe	.L499
	movq	-72(%rsp), %rax
	cmpq	$220, %rax
	jbe	.L500
	movq	-72(%rsp), %rax
	cmpq	$330, %rax
	jbe	.L501
	movq	-72(%rsp), %rax
	cmpq	$440, %rax
	jbe	.L502
	movq	-72(%rsp), %rax
	cmpq	$550, %rax
	jbe	.L503
	movq	-72(%rsp), %rax
	cmpq	$660, %rax
	jbe	.L504
	movq	-72(%rsp), %rax
	subq	$660, %rax
	movq	%rax, -88(%rsp)
	movq	$220, -24(%rsp)
	movq	-88(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -40(%rsp)
	movq	-88(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -56(%rsp)
	jmp	.L493
.L500:
	movq	-88(%rsp), %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -24(%rsp)
	movq	$0, -40(%rsp)
	movq	$220, -56(%rsp)
.L493:
	movq	-24(%rsp), %rdx
	movq	-40(%rsp), %rax
	movq	-56(%rsp), %rcx
	salq	$16, %rdx
	salq	$8, %rax
	leaq	(%rdx,%rax), %rax
	addq	%rcx, %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rax
	ret
.L499:
	movq	-72(%rsp), %rax
	movq	%rax, -24(%rsp)
	movq	$0, -40(%rsp)
	movq	-72(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -56(%rsp)
	jmp	.L493
.L502:
	movq	-72(%rsp), %rax
	subq	$330, %rax
	movq	%rax, -88(%rsp)
	movq	$0, -24(%rsp)
	movq	$220, -40(%rsp)
	movq	-88(%rsp), %rdx
	addq	%rdx, %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -56(%rsp)
	jmp	.L493
.L501:
	movq	-72(%rsp), %rax
	subq	$220, %rax
	movq	%rax, -88(%rsp)
	movq	$0, -24(%rsp)
	movq	-88(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -40(%rsp)
	movq	$220, -56(%rsp)
	jmp	.L493
.L503:
	movq	-72(%rsp), %rax
	subq	$440, %rax
	movq	%rax, -88(%rsp)
	movq	-88(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -24(%rsp)
	movq	$220, -40(%rsp)
	movq	$0, -56(%rsp)
	jmp	.L493
.L504:
	movq	-72(%rsp), %rax
	subq	$550, %rax
	movq	%rax, -88(%rsp)
	movq	$220, -24(%rsp)
	movq	-88(%rsp), %rdx
	addq	%rdx, %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -40(%rsp)
	movq	$0, -56(%rsp)
	jmp	.L493
	.cfi_endproc
.LFE47:
	.size	color_heati, .-color_heati
.globl color_heat2
	.type	color_heat2, @function
color_heat2:
.LFB48:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L506
	cvttsd2siq	%xmm0, %rax
.L507:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	movzbl	%ah, %ecx
	addq	%rcx, %rdx
	andl	$255, %eax
	leaq	(%rdx,%rax), %rax
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC21(%rip), %xmm0
	movsd	.LC22(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	ja	.L579
	ucomisd	%xmm2, %xmm0
	jae	.L561
	movsd	.LC25(%rip), %xmm2
.L519:
	ucomisd	%xmm2, %xmm0
	jae	.L563
	movsd	.LC26(%rip), %xmm2
.L526:
	ucomisd	%xmm2, %xmm0
	jae	.L565
	movsd	.LC27(%rip), %xmm2
.L533:
	ucomisd	%xmm2, %xmm0
	jae	.L567
	movsd	.LC28(%rip), %xmm2
.L540:
	ucomisd	%xmm2, %xmm0
	jae	.L569
	movsd	.LC29(%rip), %xmm2
.L547:
	ucomisd	%xmm2, %xmm0
	jb	.L573
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC11(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	48(%rsp), %xmm3
	subsd	%xmm0, %xmm3
	movsd	32(%rsp), %xmm2
	mulsd	%xmm2, %xmm3
	mulsd	.LC1(%rip), %xmm3
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm3
	jae	.L555
	cvttsd2siq	%xmm3, %rax
.L556:
	movq	%rax, %rdx
	salq	$8, %rdx
	leaq	16711680(%rax,%rdx), %rax
	testq	%rax, %rax
	js	.L557
.L578:
	cvtsi2sdq	%rax, %xmm0
.L518:
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L506:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L507
.L561:
	movsd	.LC25(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L519
	mulsd	.LC23(%rip), %xmm0
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm2, %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movapd	%xmm3, %xmm0
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm5
	subsd	%xmm0, %xmm5
	movsd	.LC24(%rip), %xmm0
	mulsd	%xmm5, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	subsd	%xmm0, %xmm2
	movsd	48(%rsp), %xmm3
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm2
	jae	.L522
	cvttsd2siq	%xmm2, %rax
.L523:
	salq	$16, %rax
	addq	$255, %rax
	jns	.L578
.L557:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L518
.L579:
	mulsd	.LC23(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm4
	movapd	%xmm4, %xmm2
	subsd	%xmm0, %xmm2
	mulsd	.LC5(%rip), %xmm2
	movsd	.LC24(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	16(%rsp), %xmm4
	movapd	%xmm4, %xmm3
	subsd	%xmm0, %xmm3
	mulsd	.LC5(%rip), %xmm3
	movsd	32(%rsp), %xmm2
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	16(%rsp), %xmm4
	subsd	%xmm0, %xmm4
	mulsd	.LC5(%rip), %xmm4
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm0, %xmm4
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm4
	movsd	48(%rsp), %xmm3
	jae	.L512
	cvttsd2siq	%xmm4, %rax
	mulsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jae	.L514
.L580:
	cvttsd2siq	%xmm3, %rdx
.L515:
	salq	$16, %rdx
	addq	%rdx, %rax
	jns	.L578
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L518
.L563:
	movsd	.LC26(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L526
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC13(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	48(%rsp), %xmm3
	subsd	%xmm0, %xmm3
	movsd	32(%rsp), %xmm2
	mulsd	%xmm2, %xmm3
	mulsd	.LC1(%rip), %xmm3
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm3
	jae	.L529
	cvttsd2siq	%xmm3, %rax
.L530:
	salq	$8, %rax
	addq	$255, %rax
	jns	.L578
	jmp	.L557
.L565:
	movsd	.LC27(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L533
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC2(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movapd	%xmm3, %xmm0
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm5
	subsd	%xmm0, %xmm5
	movapd	%xmm5, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	subsd	%xmm0, %xmm2
	movsd	48(%rsp), %xmm3
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm2
	jae	.L536
	cvttsd2siq	%xmm2, %rax
.L537:
	addq	$65280, %rax
	jns	.L578
	jmp	.L557
.L512:
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	mulsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jb	.L580
.L514:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rdx
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rdx
	jmp	.L515
.L567:
	movsd	.LC28(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L540
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC16(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	48(%rsp), %xmm3
	subsd	%xmm0, %xmm3
	movsd	32(%rsp), %xmm2
	mulsd	%xmm2, %xmm3
	mulsd	.LC1(%rip), %xmm3
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm3
	jae	.L543
	cvttsd2siq	%xmm3, %rax
.L544:
	salq	$16, %rax
	addq	$65280, %rax
	jns	.L578
.L552:
	shrq	%rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L518
.L569:
	movsd	.LC29(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L547
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC17(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movapd	%xmm3, %xmm0
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm5
	subsd	%xmm0, %xmm5
	movapd	%xmm5, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	subsd	%xmm0, %xmm2
	movsd	48(%rsp), %xmm3
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm2
	jae	.L550
	cvttsd2siq	%xmm2, %rax
.L551:
	salq	$8, %rax
	addq	$16711680, %rax
	jns	.L578
	jmp	.L552
.L573:
	movsd	.LC20(%rip), %xmm0
	jmp	.L518
.L555:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L556
.L522:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L523
.L529:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L530
.L536:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L537
.L543:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L544
.L550:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L551
	.cfi_endproc
.LFE48:
	.size	color_heat2, .-color_heat2
.globl color_heat
	.type	color_heat, @function
color_heat:
.LFB49:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L582
	cvttsd2siq	%xmm0, %rax
.L583:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	movzbl	%ah, %ecx
	addq	%rcx, %rdx
	andl	$255, %eax
	leaq	(%rdx,%rax), %rax
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC21(%rip), %xmm0
	movsd	.LC22(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	ja	.L655
	ucomisd	%xmm2, %xmm0
	jae	.L637
	movsd	.LC25(%rip), %xmm2
.L595:
	ucomisd	%xmm2, %xmm0
	jae	.L639
	movsd	.LC26(%rip), %xmm2
.L602:
	ucomisd	%xmm2, %xmm0
	jae	.L641
	movsd	.LC27(%rip), %xmm2
.L609:
	ucomisd	%xmm2, %xmm0
	jae	.L643
	movsd	.LC28(%rip), %xmm2
.L616:
	ucomisd	%xmm2, %xmm0
	jae	.L645
	movsd	.LC29(%rip), %xmm2
.L623:
	ucomisd	%xmm2, %xmm0
	jb	.L649
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC11(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	48(%rsp), %xmm3
	subsd	%xmm0, %xmm3
	movsd	32(%rsp), %xmm2
	mulsd	%xmm2, %xmm3
	mulsd	.LC1(%rip), %xmm3
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm3
	jae	.L631
	cvttsd2siq	%xmm3, %rax
.L632:
	movq	%rax, %rdx
	salq	$8, %rdx
	leaq	16711680(%rax,%rdx), %rax
	testq	%rax, %rax
	js	.L633
.L654:
	cvtsi2sdq	%rax, %xmm0
.L594:
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L582:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L583
.L637:
	movsd	.LC25(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L595
	mulsd	.LC23(%rip), %xmm0
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm2, %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movapd	%xmm3, %xmm0
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm5
	subsd	%xmm0, %xmm5
	movsd	.LC24(%rip), %xmm0
	mulsd	%xmm5, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	subsd	%xmm0, %xmm2
	movsd	48(%rsp), %xmm3
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm2
	jae	.L598
	cvttsd2siq	%xmm2, %rax
.L599:
	salq	$16, %rax
	addq	$255, %rax
	jns	.L654
.L633:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L594
.L655:
	mulsd	.LC23(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm4
	movapd	%xmm4, %xmm2
	subsd	%xmm0, %xmm2
	mulsd	.LC5(%rip), %xmm2
	movsd	.LC24(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	16(%rsp), %xmm4
	movapd	%xmm4, %xmm3
	subsd	%xmm0, %xmm3
	mulsd	.LC5(%rip), %xmm3
	movsd	32(%rsp), %xmm2
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	16(%rsp), %xmm4
	subsd	%xmm0, %xmm4
	mulsd	.LC5(%rip), %xmm4
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm0, %xmm4
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm4
	movsd	48(%rsp), %xmm3
	jae	.L588
	cvttsd2siq	%xmm4, %rax
	mulsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jae	.L590
.L656:
	cvttsd2siq	%xmm3, %rdx
.L591:
	salq	$16, %rdx
	addq	%rdx, %rax
	jns	.L654
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L594
.L639:
	movsd	.LC26(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L602
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC13(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	48(%rsp), %xmm3
	subsd	%xmm0, %xmm3
	movsd	32(%rsp), %xmm2
	mulsd	%xmm2, %xmm3
	mulsd	.LC1(%rip), %xmm3
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm3
	jae	.L605
	cvttsd2siq	%xmm3, %rax
.L606:
	salq	$8, %rax
	addq	$255, %rax
	jns	.L654
	jmp	.L633
.L641:
	movsd	.LC27(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L609
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC2(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movapd	%xmm3, %xmm0
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm5
	subsd	%xmm0, %xmm5
	movapd	%xmm5, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	subsd	%xmm0, %xmm2
	movsd	48(%rsp), %xmm3
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm2
	jae	.L612
	cvttsd2siq	%xmm2, %rax
.L613:
	addq	$65280, %rax
	jns	.L654
	jmp	.L633
.L588:
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	mulsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jb	.L656
.L590:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rdx
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rdx
	jmp	.L591
.L643:
	movsd	.LC28(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L616
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC16(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	48(%rsp), %xmm3
	subsd	%xmm0, %xmm3
	movsd	32(%rsp), %xmm2
	mulsd	%xmm2, %xmm3
	mulsd	.LC1(%rip), %xmm3
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm3
	jae	.L619
	cvttsd2siq	%xmm3, %rax
.L620:
	salq	$16, %rax
	addq	$65280, %rax
	jns	.L654
.L628:
	shrq	%rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L594
.L645:
	movsd	.LC29(%rip), %xmm2
	ucomisd	%xmm0, %xmm2
	jbe	.L623
	mulsd	.LC23(%rip), %xmm0
	subsd	.LC17(%rip), %xmm0
	movsd	.LC3(%rip), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm1, (%rsp)
	movsd	%xmm4, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movapd	%xmm3, %xmm0
	movsd	.LC5(%rip), %xmm3
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm5
	subsd	%xmm0, %xmm5
	movapd	%xmm5, %xmm0
	movsd	16(%rsp), %xmm4
	mulsd	%xmm4, %xmm0
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 48(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm2
	subsd	%xmm0, %xmm2
	movsd	48(%rsp), %xmm3
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	movsd	(%rsp), %xmm1
	ucomisd	%xmm1, %xmm2
	jae	.L626
	cvttsd2siq	%xmm2, %rax
.L627:
	salq	$8, %rax
	addq	$16711680, %rax
	jns	.L654
	jmp	.L628
.L649:
	movsd	.LC20(%rip), %xmm0
	jmp	.L594
.L631:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L632
.L598:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L599
.L605:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L606
.L612:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L613
.L619:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L620
.L626:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L627
	.cfi_endproc
.LFE49:
	.size	color_heat, .-color_heat
.globl color_ghost
	.type	color_ghost, @function
color_ghost:
.LFB50:
	.cfi_startproc
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L658
	cvttsd2siq	%xmm0, %rax
.L659:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movsd	.LC1(%rip), %xmm2
	divsd	%xmm2, %xmm0
	movsd	.LC4(%rip), %xmm3
	movapd	%xmm3, %xmm4
	subsd	%xmm0, %xmm4
	movsd	%xmm4, 80(%rsp)
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm0
	divsd	%xmm2, %xmm0
	movapd	%xmm3, %xmm4
	subsd	%xmm0, %xmm4
	movsd	%xmm4, 88(%rsp)
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm0
	divsd	%xmm2, %xmm0
	subsd	%xmm0, %xmm3
	movapd	%xmm3, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 32(%rsp)
	call	cbrt@PLT
	movapd	%xmm0, %xmm5
	movsd	80(%rsp), %xmm0
	movsd	%xmm5, 48(%rsp)
	call	cbrt@PLT
	movapd	%xmm0, %xmm4
	movsd	88(%rsp), %xmm0
	movsd	%xmm4, 64(%rsp)
	call	cbrt@PLT
	movsd	48(%rsp), %xmm5
	mulsd	88(%rsp), %xmm5
	mulsd	88(%rsp), %xmm5
	movsd	(%rsp), %xmm2
	mulsd	%xmm2, %xmm5
	movsd	16(%rsp), %xmm1
	ucomisd	%xmm1, %xmm5
	movsd	32(%rsp), %xmm3
	movsd	64(%rsp), %xmm4
	jae	.L666
	cvttsd2siq	%xmm5, %rdx
.L667:
	salq	$16, %rdx
	mulsd	%xmm3, %xmm4
	mulsd	%xmm3, %xmm4
	mulsd	%xmm2, %xmm4
	ucomisd	%xmm1, %xmm4
	jae	.L668
	cvttsd2siq	%xmm4, %rcx
.L669:
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	80(%rsp), %xmm0
	mulsd	80(%rsp), %xmm0
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L670
	cvttsd2siq	%xmm0, %rax
	addq	%rdx, %rax
	js	.L672
.L674:
	cvtsi2sdq	%rax, %xmm0
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L658:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L659
.L670:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L674
.L672:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L668:
	.cfi_restore_state
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L669
.L666:
	subsd	%xmm1, %xmm5
	cvttsd2siq	%xmm5, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L667
	.cfi_endproc
.LFE50:
	.size	color_ghost, .-color_ghost
.globl color_inv
	.type	color_inv, @function
color_inv:
.LFB51:
	.cfi_startproc
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L676
	cvttsd2siq	%xmm0, %rax
.L677:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm3
	movsd	.LC1(%rip), %xmm0
	divsd	%xmm0, %xmm3
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm4
	subsd	%xmm3, %xmm4
	mulsd	%xmm0, %xmm4
	movzbl	%ah, %edx
	cvtsi2sdq	%rdx, %xmm3
	divsd	%xmm0, %xmm3
	movapd	%xmm2, %xmm5
	subsd	%xmm3, %xmm5
	movapd	%xmm5, %xmm3
	mulsd	%xmm0, %xmm3
	andl	$255, %eax
	cvtsi2sdq	%rax, %xmm5
	divsd	%xmm0, %xmm5
	subsd	%xmm5, %xmm2
	mulsd	%xmm0, %xmm2
	ucomisd	%xmm1, %xmm4
	jae	.L684
	cvttsd2siq	%xmm4, %rdx
.L685:
	salq	$16, %rdx
	ucomisd	%xmm1, %xmm3
	jae	.L686
	cvttsd2siq	%xmm3, %rcx
.L687:
	salq	$8, %rcx
	addq	%rcx, %rdx
	ucomisd	%xmm1, %xmm2
	jae	.L688
	cvttsd2siq	%xmm2, %rax
	addq	%rdx, %rax
	js	.L690
.L692:
	cvtsi2sdq	%rax, %xmm0
	ret
.L676:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L677
.L688:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L692
.L690:
	movq	%rax, %rcx
	shrq	%rcx
	andl	$1, %eax
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
.L686:
	subsd	%xmm1, %xmm3
	cvttsd2siq	%xmm3, %rcx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rcx
	jmp	.L687
.L684:
	subsd	%xmm1, %xmm4
	cvttsd2siq	%xmm4, %rdx
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, %rdx
	jmp	.L685
	.cfi_endproc
.LFE51:
	.size	color_inv, .-color_inv
.globl color_box
	.type	color_box, @function
color_box:
.LFB52:
	.cfi_startproc
	movsd	.LC30(%rip), %xmm2
	subsd	%xmm1, %xmm2
	movsd	.LC31(%rip), %xmm1
	movapd	%xmm0, %xmm3
	mulsd	%xmm1, %xmm3
	cvttsd2si	%xmm3, %eax
	mulsd	%xmm2, %xmm1
	cvttsd2si	%xmm1, %esi
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L694
	cvttsd2siq	%xmm0, %rcx
	ucomisd	%xmm1, %xmm2
	jae	.L696
.L772:
	cvttsd2siq	%xmm2, %rdx
	andl	$255, %ecx
	andl	$255, %edx
	testl	%esi, %esi
	jne	.L698
.L773:
	cmpl	$1, %eax
	je	.L769
	testl	%eax, %eax
	je	.L770
	cmpl	$2, %eax
	.p2align 4,,3
	je	.L771
	cmpl	$3, %eax
	.p2align 4,,3
	jne	.L760
	leaq	(%rdx,%rcx), %rax
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L719
	cvttsd2siq	%xmm0, %rax
.L720:
	salq	$16, %rax
	salq	$8, %rcx
	addq	%rcx, %rdx
	addq	%rax, %rdx
	js	.L725
.L764:
	cvtsi2sdq	%rdx, %xmm0
	ret
.L694:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rcx
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rcx
	ucomisd	%xmm1, %xmm2
	jb	.L772
.L696:
	subsd	%xmm1, %xmm2
	cvttsd2siq	%xmm2, %rdx
	movabsq	$-9223372036854775808, %rdi
	xorq	%rdi, %rdx
	andl	$255, %ecx
	andl	$255, %edx
	testl	%esi, %esi
	je	.L773
.L698:
	cmpl	$1, %esi
	je	.L774
	cmpl	$2, %esi
	je	.L775
.L760:
	pxor	%xmm0, %xmm0
	.p2align 4,,1
	ret
.L774:
	testl	%eax, %eax
	.p2align 4,,3
	je	.L776
	cmpl	$1, %eax
	.p2align 4,,3
	je	.L777
	cmpl	$2, %eax
	.p2align 4,,3
	je	.L778
	cmpl	$3, %eax
	.p2align 4,,2
	jne	.L760
	movl	$255, %eax
	subq	%rcx, %rax
	salq	$16, %rax
	salq	$8, %rdx
	leaq	255(%rax,%rdx), %rax
	testq	%rax, %rax
	jns	.L765
.L743:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	ret
.L769:
	salq	$16, %rcx
	addq	$255, %rcx
	subq	%rdx, %rcx
	js	.L756
.L763:
	cvtsi2sdq	%rcx, %xmm0
	ret
.L770:
	movq	%rcx, %rax
	salq	$16, %rax
	movq	%rdx, %rsi
	salq	$8, %rsi
.L768:
	addq	%rax, %rsi
	leaq	(%rdx,%rcx), %rax
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L741
	cvttsd2siq	%xmm0, %rax
.L742:
	addq	%rsi, %rax
	js	.L743
.L765:
	cvtsi2sdq	%rax, %xmm0
	ret
.L775:
	cmpl	$1, %eax
	je	.L779
	testl	%eax, %eax
	.p2align 4,,2
	je	.L780
	cmpl	$2, %eax
	.p2align 4,,3
	je	.L781
	cmpl	$3, %eax
	.p2align 4,,3
	je	.L782
	pxor	%xmm0, %xmm0
	.p2align 4,,1
	ret
.L741:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L742
.L776:
	salq	$8, %rdx
	subq	%rcx, %rdx
	addq	$255, %rdx
	jns	.L764
	jmp	.L725
.L771:
	movq	%rcx, %rsi
	salq	$16, %rsi
	leaq	(%rdx,%rcx), %rax
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L713
	cvttsd2siq	%xmm0, %rax
.L714:
	salq	$8, %rax
	addq	%rsi, %rdx
	addq	%rax, %rdx
	jns	.L764
.L725:
	movq	%rdx, %rax
	shrq	%rax
	andl	$1, %edx
	orq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	ret
.L719:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rsi
	xorq	%rsi, %rax
	jmp	.L720
.L782:
	leaq	(%rdx,%rcx), %rax
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L754
	cvttsd2siq	%xmm0, %rax
.L755:
	salq	$16, %rax
	salq	$8, %rdx
	leaq	(%rdx,%rcx), %rcx
	addq	%rax, %rcx
	jns	.L763
.L756:
	movq	%rcx, %rax
	shrq	%rax
	andl	$1, %ecx
	orq	%rcx, %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	ret
.L780:
	movq	%rdx, %rsi
	salq	$16, %rsi
	movq	%rcx, %rax
	salq	$8, %rax
	jmp	.L768
.L779:
	salq	$16, %rcx
	leaq	65280(%rdx,%rcx), %rax
	jmp	.L765
.L713:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rcx
	xorq	%rcx, %rax
	jmp	.L714
.L777:
	salq	$16, %rcx
	salq	$8, %rdx
	leaq	(%rdx,%rcx), %rax
	cvtsi2sdq	%rax, %xmm0
	ret
.L778:
	salq	$8, %rdx
	leaq	16711680(%rcx,%rdx), %rax
	jmp	.L765
.L781:
	movq	%rdx, %rsi
	salq	$16, %rsi
	leaq	(%rdx,%rcx), %rax
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L748
	cvttsd2siq	%xmm0, %rax
.L749:
	salq	$8, %rax
	leaq	(%rsi,%rcx), %rcx
	addq	%rax, %rcx
	jns	.L763
	jmp	.L756
.L754:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rsi
	xorq	%rsi, %rax
	jmp	.L755
.L748:
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movabsq	$-9223372036854775808, %rdx
	xorq	%rdx, %rax
	jmp	.L749
	.cfi_endproc
.LFE52:
	.size	color_box, .-color_box
.globl udf3
	.type	udf3, @function
udf3:
.LFB54:
	.cfi_startproc
	jmp	alien_gs3@PLT
	.cfi_endproc
.LFE54:
	.size	udf3, .-udf3
.globl udf2
	.type	udf2, @function
udf2:
.LFB55:
	.cfi_startproc
	jmp	color_box@PLT
	.cfi_endproc
.LFE55:
	.size	udf2, .-udf2
.globl min_udf2
	.type	min_udf2, @function
min_udf2:
.LFB53:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movapd	%xmm1, %xmm3
	movd	%xmm2, %rbx
	.cfi_offset 3, -16
	movsd	%xmm3, (%rsp)
	call	udf2@PLT
	movapd	%xmm0, %xmm2
	movd	%rbx, %xmm1
	movsd	(%rsp), %xmm3
	movapd	%xmm3, %xmm0
	movsd	%xmm2, (%rsp)
	call	udf2@PLT
	movsd	(%rsp), %xmm2
	minsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE53:
	.size	min_udf2, .-min_udf2
.globl udf1
	.type	udf1, @function
udf1:
.LFB56:
	.cfi_startproc
	jmp	alien_gs@PLT
	.cfi_endproc
.LFE56:
	.size	udf1, .-udf1
.globl udfi1
	.type	udfi1, @function
udfi1:
.LFB57:
	.cfi_startproc
	jmp	alien_gsi@PLT
	.cfi_endproc
.LFE57:
	.size	udfi1, .-udfi1
.globl udfi2
	.type	udfi2, @function
udfi2:
.LFB58:
	.cfi_startproc
	jmp	edge_detect_comix@PLT
	.cfi_endproc
.LFE58:
	.size	udfi2, .-udfi2
.globl udfi3
	.type	udfi3, @function
udfi3:
.LFB59:
	.cfi_startproc
	jmp	color_invi3@PLT
	.cfi_endproc
.LFE59:
	.size	udfi3, .-udfi3
.globl udfi6
	.type	udfi6, @function
udfi6:
.LFB60:
	.cfi_startproc
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, (%rsp)
	movq	%rsi, 8(%rsp)
	movq	%rdx, 16(%rsp)
	movq	%rcx, 24(%rsp)
	movq	%r8, 32(%rsp)
	movq	%r9, 40(%rsp)
	movq	%rsp, %rdi
	call	alien_gsi6@PLT
	addq	$56, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE60:
	.size	udfi6, .-udfi6
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1138753536
	.align 8
.LC1:
	.long	0
	.long	1081073664
	.align 8
.LC2:
	.long	0
	.long	1074266112
	.align 8
.LC3:
	.long	1413754136
	.long	1074340347
	.align 8
.LC4:
	.long	0
	.long	1072693248
	.align 8
.LC5:
	.long	0
	.long	1071644672
	.align 8
.LC6:
	.long	1546188227
	.long	1072022159
	.align 8
.LC7:
	.long	0
	.long	1074331648
	.align 8
.LC10:
	.long	1431655765
	.long	1069897045
	.align 8
.LC11:
	.long	0
	.long	1075314688
	.align 8
.LC12:
	.long	1431655765
	.long	1070945621
	.align 8
.LC13:
	.long	0
	.long	1073741824
	.align 8
.LC14:
	.long	1431655765
	.long	1071994197
	.align 8
.LC15:
	.long	2863311531
	.long	1072343722
	.align 8
.LC16:
	.long	0
	.long	1074790400
	.align 8
.LC17:
	.long	0
	.long	1075052544
	.align 8
.LC18:
	.long	1719614413
	.long	1073127582
	.align 8
.LC19:
	.long	0
	.long	-1074790400
	.align 8
.LC20:
	.long	3221225472
	.long	1096802271
	.align 8
.LC21:
	.long	0
	.long	1082648576
	.align 8
.LC22:
	.long	2454267026
	.long	1069697316
	.align 8
.LC23:
	.long	0
	.long	1075576832
	.align 8
.LC24:
	.long	858993459
	.long	1071854387
	.align 8
.LC25:
	.long	2454267026
	.long	1070745892
	.align 8
.LC26:
	.long	3681400539
	.long	1071345078
	.align 8
.LC27:
	.long	2454267026
	.long	1071794468
	.align 8
.LC28:
	.long	3067833783
	.long	1072094061
	.align 8
.LC29:
	.long	3681400539
	.long	1072393654
	.align 8
.LC30:
	.long	0
	.long	1082654720
	.align 8
.LC31:
	.long	0
	.long	1064304640
	.ident	"GCC: (NetBSD nb2 20110806) 4.5.3"
