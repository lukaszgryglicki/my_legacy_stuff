	.file	"udf.c"
	.text
	.p2align 4,,15
	.globl	color_comix
	.type	color_comix, @function
color_comix:
.LFB1:
	.cfi_startproc
	movq	%rdi, %rax
	shrq	$16, %rax
	andl	$192, %eax
	movq	%rax, -24(%rsp)
	movq	%rdi, %rax
	andl	$192, %edi
	shrq	$8, %rax
	andl	$192, %eax
	movq	%rax, -16(%rsp)
	movq	%rdi, -8(%rsp)
	movq	-24(%rsp), %rcx
	movq	-16(%rsp), %rdx
	movq	-8(%rsp), %rax
	salq	$16, %rcx
	salq	$8, %rdx
	addq	%rcx, %rdx
	addq	%rdx, %rax
	ret
	.cfi_endproc
.LFE1:
	.size	color_comix, .-color_comix
	.p2align 4,,15
	.globl	edge_detect_comix
	.type	edge_detect_comix, @function
edge_detect_comix:
.LFB2:
	.cfi_startproc
	movq	%rdi, %rax
	movq	%rdi, %rcx
	movzbl	%dil, %r8d
	andl	$16711680, %ecx
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%r8d, %edx
	movzbl	%ah, %ebx
	movzbl	%sil, %eax
	shrq	$16, %rcx
	subl	%eax, %edx
	movq	%rsi, %rax
	addl	%ecx, %edx
	andl	$16711680, %eax
	shrq	$16, %rax
	addl	%ebx, %edx
	subl	%eax, %edx
	movq	%rsi, %rax
	movzbl	%ah, %esi
	subl	%esi, %edx
	movl	%edx, %eax
	sarl	$31, %eax
	xorl	%eax, %edx
	subl	%eax, %edx
	movslq	%edx, %rdx
	cmpq	$15, %rdx
	jbe	.L10
	cmpq	%rbx, %rcx
	jbe	.L5
	cmpq	%rcx, %r8
	sbbq	%rax, %rax
	andq	$-16711425, %rax
	addq	$16776960, %rax
.L4:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L5:
	.cfi_restore_state
	cmpq	%r8, %rbx
	movl	$16711935, %eax
	ja	.L4
	salq	$8, %rbx
	leaq	16711935(%rbx), %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L10:
	.cfi_restore_state
	andq	$-64, %rcx
	andq	$-64, %rbx
	andl	$192, %edi
	salq	$16, %rcx
	salq	$8, %rbx
	addq	%rcx, %rbx
	leaq	(%rbx,%rdi), %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2:
	.size	edge_detect_comix, .-edge_detect_comix
	.p2align 4,,15
	.globl	smooth_gs6
	.type	smooth_gs6, @function
smooth_gs6:
.LFB3:
	.cfi_startproc
	movq	16(%rdi), %rdx
	movabsq	$-6148914691236517205, %rcx
	movq	8(%rdi), %r8
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	(%rdi), %r9
	movq	%rdx, %rax
	movzbl	%dh, %esi
	movzbl	%dl, %edx
	andl	$16711680, %eax
	movq	%r8, %rbx
	shrq	$16, %rax
	addq	%rax, %rsi
	addq	%rdx, %rsi
	movq	%rsi, %rax
	mulq	%rcx
	movq	%r8, %rax
	movzbl	%r8b, %r8d
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rdx, %rsi
	movzbl	%bh, %edx
	movq	%r9, %rbx
	addq	%rax, %rdx
	shrq	%rsi
	addq	%r8, %rdx
	movq	%rdx, %rax
	mulq	%rcx
	movq	%r9, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	shrq	%rdx
	leaq	(%rsi,%rdx), %r8
	movzbl	%bh, %edx
	movzbl	%r9b, %ebx
	addq	%rax, %rdx
	addq	%rbx, %rdx
	movq	24(%rdi), %rbx
	movq	%rdx, %rax
	mulq	%rcx
	movq	%rbx, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	shrq	%rdx
	addq	%rdx, %r8
	movzbl	%bh, %edx
	movzbl	%bl, %ebx
	addq	%rax, %rdx
	addq	%rbx, %rdx
	movq	32(%rdi), %rbx
	movq	%rdx, %rax
	mulq	%rcx
	movq	%rbx, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	shrq	%rdx
	addq	%rdx, %r8
	movzbl	%bh, %edx
	movzbl	%bl, %ebx
	addq	%rax, %rdx
	addq	%rbx, %rdx
	movq	40(%rdi), %rbx
	movq	%rdx, %rax
	mulq	%rcx
	movq	%rbx, %rsi
	shrq	%rdx
	addq	%rdx, %r8
	andl	$16711680, %esi
	movzbl	%bh, %edx
	shrq	$16, %rsi
	movzbl	%bl, %ebx
	addq	%rsi, %rdx
	addq	%rbx, %rdx
	movq	%rdx, %rax
	mulq	%rcx
	popq	%rbx
	.cfi_def_cfa_offset 8
	shrq	%rdx
	addq	%r8, %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	$2, %rdx
	movq	%rdx, %rcx
	movq	%rdx, %rax
	salq	$16, %rcx
	salq	$8, %rax
	addq	%rcx, %rax
	addq	%rdx, %rax
	ret
	.cfi_endproc
.LFE3:
	.size	smooth_gs6, .-smooth_gs6
	.p2align 4,,15
	.globl	color_offset6
	.type	color_offset6, @function
color_offset6:
.LFB4:
	.cfi_startproc
	movzbl	10(%rdi), %edx
	movzbl	2(%rdi), %eax
	addq	%rdx, %rax
	movzbl	25(%rdi), %edx
	shrq	%rax
	movq	%rax, -24(%rsp)
	movzbl	17(%rdi), %eax
	addq	%rdx, %rax
	movzbl	40(%rdi), %edx
	shrq	%rax
	movq	%rax, -16(%rsp)
	movzbl	32(%rdi), %eax
	addq	%rdx, %rax
	shrq	%rax
	movq	%rax, -8(%rsp)
	movq	-24(%rsp), %rcx
	movq	-16(%rsp), %rdx
	movq	-8(%rsp), %rax
	salq	$16, %rcx
	salq	$8, %rdx
	addq	%rcx, %rdx
	addq	%rdx, %rax
	ret
	.cfi_endproc
.LFE4:
	.size	color_offset6, .-color_offset6
	.p2align 4,,15
	.globl	color_offset3
	.type	color_offset3, @function
color_offset3:
.LFB5:
	.cfi_startproc
	andl	$65280, %esi
	movzbl	%dl, %edx
	andl	$16711680, %edi
	addq	%rdx, %rsi
	leaq	(%rsi,%rdi), %rax
	ret
	.cfi_endproc
.LFE5:
	.size	color_offset3, .-color_offset3
	.p2align 4,,15
	.globl	smooth_gs
	.type	smooth_gs, @function
smooth_gs:
.LFB6:
	.cfi_startproc
	movq	%rsi, %rax
	movq	%rsi, %rcx
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	andl	$16711680, %eax
	movq	%rdx, %rbx
	movzbl	%ch, %edx
	shrq	$16, %rax
	movzbl	%sil, %esi
	movabsq	$-6148914691236517205, %rcx
	addq	%rdx, %rax
	movq	%rdi, %r8
	addq	%rax, %rsi
	andl	$16711680, %r8d
	movq	%rsi, %rax
	shrq	$16, %r8
	mulq	%rcx
	movq	%rbx, %rax
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rdx, %rsi
	movzbl	%bh, %edx
	movzbl	%bl, %ebx
	addq	%rax, %rdx
	shrq	%rsi
	addq	%rbx, %rdx
	movq	%rdx, %rax
	mulq	%rcx
	movq	%rdi, %rax
	movzbl	%dil, %edi
	popq	%rbx
	.cfi_def_cfa_offset 8
	shrq	%rdx
	addq	%rdx, %rsi
	movzbl	%ah, %edx
	addq	%r8, %rdx
	addq	%rdi, %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	%rsi, %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	movq	%rdx, %rcx
	movq	%rdx, %rax
	salq	$16, %rcx
	salq	$8, %rax
	addq	%rcx, %rax
	addq	%rdx, %rax
	ret
	.cfi_endproc
.LFE6:
	.size	smooth_gs, .-smooth_gs
	.p2align 4,,15
	.globl	edge_detect
	.type	edge_detect, @function
edge_detect:
.LFB7:
	.cfi_startproc
	movq	%rdx, %rcx
	movl	$25000, %eax
	xorl	%edx, %edx
	divq	%rcx
	movq	%rsi, %rdx
	movq	%rsi, %rcx
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movzbl	%dh, %edx
	andl	$16711680, %ecx
	movzbl	%sil, %esi
	movq	%rdx, %r10
	movq	%rdi, %rdx
	shrq	$16, %rcx
	andl	$16711680, %edx
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	shrq	$16, %rdx
	movzbl	%bh, %ebp
	movzbl	%dil, %edi
	subl	%ecx, %edx
	subl	%esi, %edi
	salq	$16, %rcx
	movl	%edx, %r8d
	movq	%r10, %rbx
	sarl	$31, %r8d
	xorl	%r8d, %edx
	subl	%r8d, %edx
	movl	%ebp, %r8d
	subl	%r10d, %r8d
	movl	%r8d, %r9d
	sarl	$31, %r9d
	xorl	%r9d, %r8d
	subl	%r9d, %r8d
	movl	%edi, %r9d
	sarl	$31, %r9d
	movslq	%r8d, %r8
	xorl	%r9d, %edi
	subl	%r9d, %edi
	movslq	%edx, %r9
	xorl	%edx, %edx
	cmpq	%r9, %rax
	movslq	%edi, %rdi
	cmovae	%rdx, %rcx
	salq	$8, %rbx
	cmpq	%r8, %rax
	cmovae	%rdx, %rbx
	cmpq	%rdi, %rax
	leaq	(%rbx,%rcx), %rax
	cmovae	%rdx, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 16
	addq	%rsi, %rax
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE7:
	.size	edge_detect, .-edge_detect
	.p2align 4,,15
	.globl	alien_gs3
	.type	alien_gs3, @function
alien_gs3:
.LFB8:
	.cfi_startproc
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	movsd	.LC0(%rip), %xmm3
	movsd	%xmm2, 72(%rsp)
	ucomisd	%xmm3, %xmm0
	jae	.L25
	cvttsd2siq	%xmm0, %rax
.L26:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm2
	andl	$16711680, %edx
	movsd	%xmm1, 48(%rsp)
	shrq	$16, %rdx
	movsd	%xmm2, 16(%rsp)
	cvtsi2sdq	%rdx, %xmm0
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm4
	movsd	%xmm3, (%rsp)
	divsd	%xmm2, %xmm0
	divsd	%xmm2, %xmm4
	addsd	%xmm4, %xmm0
	cvtsi2sdq	%rax, %xmm4
	divsd	%xmm2, %xmm4
	addsd	%xmm4, %xmm0
	movsd	.LC2(%rip), %xmm4
	movsd	%xmm4, 32(%rsp)
	divsd	%xmm4, %xmm0
	mulsd	%xmm4, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	48(%rsp), %xmm1
	movsd	(%rsp), %xmm3
	movsd	%xmm0, 80(%rsp)
	ucomisd	%xmm3, %xmm1
	movsd	16(%rsp), %xmm2
	movsd	32(%rsp), %xmm4
	jae	.L27
	cvttsd2siq	%xmm1, %rax
.L28:
	movq	%rax, %rdx
	movsd	%xmm2, 16(%rsp)
	andl	$16711680, %edx
	movsd	%xmm3, (%rsp)
	shrq	$16, %rdx
	movsd	%xmm4, 32(%rsp)
	cvtsi2sdq	%rdx, %xmm0
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm1
	divsd	%xmm2, %xmm0
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	divsd	%xmm4, %xmm0
	mulsd	%xmm4, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	(%rsp), %xmm3
	movsd	%xmm0, 88(%rsp)
	movsd	72(%rsp), %xmm0
	movsd	16(%rsp), %xmm2
	ucomisd	%xmm3, %xmm0
	movsd	32(%rsp), %xmm4
	jae	.L29
	cvttsd2siq	%xmm0, %rax
.L30:
	movq	%rax, %rdx
	movsd	%xmm2, 16(%rsp)
	andl	$16711680, %edx
	movsd	%xmm3, (%rsp)
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm0
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm1
	divsd	%xmm2, %xmm0
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	divsd	%xmm4, %xmm0
	mulsd	%xmm4, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm1
	movsd	.LC5(%rip), %xmm5
	movapd	%xmm1, %xmm4
	movsd	16(%rsp), %xmm2
	movsd	(%rsp), %xmm3
	subsd	80(%rsp), %xmm4
	mulsd	%xmm5, %xmm4
	mulsd	%xmm2, %xmm4
	ucomisd	%xmm3, %xmm4
	jae	.L31
	cvttsd2siq	%xmm4, %rdx
.L32:
	movapd	%xmm1, %xmm4
	salq	$16, %rdx
	subsd	88(%rsp), %xmm4
	mulsd	%xmm5, %xmm4
	mulsd	%xmm2, %xmm4
	ucomisd	%xmm3, %xmm4
	jae	.L33
	cvttsd2siq	%xmm4, %rcx
.L34:
	subsd	%xmm0, %xmm1
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	%xmm5, %xmm1
	mulsd	%xmm2, %xmm1
	ucomisd	%xmm3, %xmm1
	jae	.L35
	cvttsd2siq	%xmm1, %rax
	addq	%rdx, %rax
	js	.L37
.L40:
	cvtsi2sdq	%rax, %xmm0
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L25:
	.cfi_restore_state
	subsd	%xmm3, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L26
	.p2align 4,,10
.L35:
	subsd	%xmm3, %xmm1
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm1, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L40
.L37:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L33:
	.cfi_restore_state
	subsd	%xmm3, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rcx
	xorq	%rax, %rcx
	jmp	.L34
	.p2align 4,,10
.L31:
	subsd	%xmm3, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rdx
	xorq	%rax, %rdx
	jmp	.L32
	.p2align 4,,10
.L29:
	subsd	%xmm3, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L30
	.p2align 4,,10
.L27:
	subsd	%xmm3, %xmm1
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm1, %rax
	xorq	%rdx, %rax
	jmp	.L28
	.cfi_endproc
.LFE8:
	.size	alien_gs3, .-alien_gs3
	.p2align 4,,15
	.globl	alien_gs
	.type	alien_gs, @function
alien_gs:
.LFB9:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movsd	.LC0(%rip), %xmm3
	ucomisd	%xmm3, %xmm0
	jae	.L42
	cvttsd2siq	%xmm0, %rax
.L43:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm2
	andl	$16711680, %edx
	movsd	%xmm3, (%rsp)
	shrq	$16, %rdx
	movsd	%xmm2, 16(%rsp)
	cvtsi2sdq	%rdx, %xmm0
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm1
	divsd	%xmm2, %xmm0
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	movsd	.LC2(%rip), %xmm1
	divsd	%xmm1, %xmm0
	mulsd	%xmm1, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm1
	movsd	16(%rsp), %xmm2
	subsd	%xmm0, %xmm1
	movsd	(%rsp), %xmm3
	mulsd	.LC5(%rip), %xmm1
	mulsd	%xmm2, %xmm1
	ucomisd	%xmm3, %xmm1
	jae	.L44
	cvttsd2siq	%xmm1, %rax
.L45:
	movq	%rax, %rcx
	movq	%rax, %rdx
	salq	$16, %rcx
	salq	$8, %rdx
	addq	%rcx, %rdx
	addq	%rdx, %rax
	js	.L46
	cvtsi2sdq	%rax, %xmm0
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L42:
	.cfi_restore_state
	subsd	%xmm3, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L43
	.p2align 4,,10
.L44:
	subsd	%xmm3, %xmm1
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm1, %rax
	xorq	%rdx, %rax
	jmp	.L45
	.p2align 4,,10
.L46:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.cfi_endproc
.LFE9:
	.size	alien_gs, .-alien_gs
	.p2align 4,,15
	.globl	alien_toon3
	.type	alien_toon3, @function
alien_toon3:
.LFB10:
	.cfi_startproc
	movq	%rdi, %rax
	movslq	%ecx, %r10
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%r8d, %ecx
	movq	%rdx, %rbx
	movq	%rdi, %r8
	movzbl	%ah, %edx
	andl	$16711680, %r8d
	movzbl	%dil, %edi
	leaq	(%rdx,%rdx,4), %rdx
	shrq	$14, %r8
	leaq	(%r8,%rdx), %rax
	movabsq	$-3689348814741910323, %r8
	addq	%rax, %rdi
	movq	%rdi, %rax
	mulq	%r8
	movq	%rdx, %rdi
	shrq	$3, %rdi
	testq	%r10, %r10
	je	.L50
	xorl	%eax, %eax
	movl	$127, %r11d
	.p2align 4,,10
.L51:
	movq	%r11, %r8
	leaq	-256(%rdi,%rdi), %rdx
	subq	%rdi, %r8
	addq	%r8, %r8
	cmpq	$127, %rdi
	movq	%rdx, %rdi
	cmovbe	%r8, %rdi
	addq	$1, %rax
	cmpq	%r10, %rax
	jb	.L51
.L50:
	movslq	%r9d, %r9
	testq	%r9, %r9
	je	.L52
	xorl	%eax, %eax
	.p2align 4,,10
.L53:
	imulq	%rdi, %rdi
	addq	$1, %rax
	shrq	$8, %rdi
	cmpq	%r9, %rax
	jb	.L53
.L52:
	movq	%rsi, %rax
	movq	%rsi, %r8
	movzbl	%sil, %esi
	movzbl	%ah, %edx
	andl	$16711680, %r8d
	shrq	%cl, %rdi
	leaq	(%rdx,%rdx,4), %rdx
	shrq	$14, %r8
	salq	%cl, %rdi
	leaq	(%r8,%rdx), %rax
	movabsq	$-3689348814741910323, %r8
	addq	%rax, %rsi
	movq	%rsi, %rax
	mulq	%r8
	movq	%rdx, %rsi
	shrq	$3, %rsi
	testq	%r10, %r10
	je	.L54
	xorl	%eax, %eax
	movl	$127, %r11d
	.p2align 4,,10
.L55:
	movq	%r11, %r8
	leaq	-256(%rsi,%rsi), %rdx
	subq	%rsi, %r8
	addq	%r8, %r8
	cmpq	$127, %rsi
	movq	%rdx, %rsi
	cmovbe	%r8, %rsi
	addq	$1, %rax
	cmpq	%r10, %rax
	jb	.L55
.L54:
	testq	%r9, %r9
	je	.L56
	xorl	%eax, %eax
	.p2align 4,,10
.L57:
	imulq	%rsi, %rsi
	addq	$1, %rax
	shrq	$8, %rsi
	cmpq	%r9, %rax
	jb	.L57
.L56:
	movq	%rbx, %rax
	movzbl	%bh, %edx
	movzbl	%bl, %ebx
	andl	$16711680, %eax
	leaq	(%rdx,%rdx,4), %rdx
	movabsq	$-3689348814741910323, %r8
	shrq	$14, %rax
	shrq	%cl, %rsi
	addq	%rax, %rdx
	salq	%cl, %rsi
	addq	%rbx, %rdx
	movq	%rdx, %rax
	mulq	%r8
	shrq	$3, %rdx
	testq	%r10, %r10
	je	.L58
	xorl	%eax, %eax
	movl	$127, %ebx
	.p2align 4,,10
.L59:
	movq	%rbx, %r11
	leaq	-256(%rdx,%rdx), %r8
	subq	%rdx, %r11
	addq	%r11, %r11
	cmpq	$127, %rdx
	movq	%r8, %rdx
	cmovbe	%r11, %rdx
	addq	$1, %rax
	cmpq	%r10, %rax
	jb	.L59
.L58:
	testq	%r9, %r9
	je	.L60
	xorl	%eax, %eax
	.p2align 4,,10
.L61:
	imulq	%rdx, %rdx
	addq	$1, %rax
	shrq	$8, %rdx
	cmpq	%r9, %rax
	jb	.L61
.L60:
	movl	$1, %eax
	shrq	%cl, %rdx
	salq	$8, %rsi
	sall	%cl, %eax
	salq	%cl, %rdx
	movl	$255, %ecx
	cltq
	addq	%rdx, %rdx
	addq	%rax, %rdi
	cmpq	$255, %rdi
	cmova	%rcx, %rdi
	salq	$16, %rdi
	addq	%rsi, %rdi
	cmpq	$255, %rdx
	cmova	%rcx, %rdx
	leaq	(%rdi,%rdx), %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE10:
	.size	alien_toon3, .-alien_toon3
	.p2align 4,,15
	.globl	alien_toongs2
	.type	alien_toongs2, @function
alien_toongs2:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	%ecx, %ebp
	movq	%rdi, %rcx
	movq	%rdi, %rax
	movzbl	%dil, %edi
	movabsq	$-3689348814741910323, %r9
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movl	%edx, %ebx
	movzbl	%ch, %edx
	andl	$16711680, %eax
	leaq	(%rdx,%rdx,4), %rdx
	movslq	%esi, %r10
	shrq	$14, %rax
	addq	%rax, %rdx
	addq	%rdi, %rdx
	movq	%rdx, %rax
	mulq	%r9
	shrq	$3, %rdx
	testq	%r10, %r10
	je	.L82
	xorl	%eax, %eax
	movl	$127, %r11d
	.p2align 4,,10
.L83:
	movq	%r11, %r9
	leaq	-256(%rdx,%rdx), %rdi
	subq	%rdx, %r9
	addq	%r9, %r9
	cmpq	$127, %rdx
	movq	%rdi, %rdx
	cmovbe	%r9, %rdx
	addq	$1, %rax
	cmpq	%r10, %rax
	jb	.L83
.L82:
	movslq	%ebp, %rsi
	testq	%rsi, %rsi
	je	.L84
	xorl	%eax, %eax
	.p2align 4,,10
.L85:
	imulq	%rdx, %rdx
	addq	$1, %rax
	shrq	$8, %rdx
	cmpq	%rsi, %rax
	jb	.L85
.L84:
	movq	%rdx, %rax
	movl	%ebx, %ecx
	shrq	%cl, %rax
	salq	%cl, %rax
	testb	$1, %r8b
	movq	%rax, %rdx
	je	.L86
	leaq	(%rax,%rax), %rcx
	movl	$255, %edx
	cmpq	$255, %rcx
	cmovbe	%rcx, %rdx
.L86:
	testb	$2, %r8b
	movq	%rax, %rcx
	je	.L87
	leaq	(%rax,%rax), %rsi
	movl	$255, %ecx
	cmpq	$255, %rsi
	cmovbe	%rsi, %rcx
.L87:
	andl	$4, %r8d
	je	.L88
	leaq	(%rax,%rax), %rsi
	movl	$255, %eax
	cmpq	$255, %rsi
	cmovbe	%rsi, %rax
.L88:
	salq	$8, %rcx
	salq	$16, %rdx
	addq	%rcx, %rdx
	popq	%rbx
	.cfi_def_cfa_offset 16
	addq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	alien_toongs2, .-alien_toongs2
	.p2align 4,,15
	.globl	alien_toongs
	.type	alien_toongs, @function
alien_toongs:
.LFB12:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	%rdi, %rax
	movl	%edx, %r11d
	movzbl	%bh, %edx
	andl	$16711680, %eax
	leaq	(%rdx,%rdx,4), %rdx
	shrq	$14, %rax
	movzbl	%dil, %edi
	movabsq	$-3689348814741910323, %r8
	movslq	%esi, %r9
	addq	%rax, %rdx
	addq	%rdi, %rdx
	movq	%rdx, %rax
	mulq	%r8
	shrq	$3, %rdx
	testq	%r9, %r9
	je	.L104
	xorl	%eax, %eax
	movl	$127, %r10d
	.p2align 4,,10
.L105:
	movq	%r10, %r8
	leaq	-256(%rdx,%rdx), %rdi
	subq	%rdx, %r8
	addq	%r8, %r8
	cmpq	$127, %rdx
	movq	%rdi, %rdx
	cmovbe	%r8, %rdx
	addq	$1, %rax
	cmpq	%r9, %rax
	jb	.L105
.L104:
	movslq	%ecx, %rsi
	testq	%rsi, %rsi
	je	.L106
	xorl	%eax, %eax
	.p2align 4,,10
.L107:
	imulq	%rdx, %rdx
	addq	$1, %rax
	shrq	$8, %rdx
	cmpq	%rsi, %rax
	jb	.L107
.L106:
	movl	%r11d, %ecx
	shrq	%cl, %rdx
	salq	%cl, %rdx
	movl	$255, %ecx
	leaq	16(%rdx,%rdx), %rax
	movq	%rdx, %rsi
	popq	%rbx
	.cfi_def_cfa_offset 8
	cmpq	$255, %rax
	cmovbe	%rax, %rcx
	salq	$16, %rsi
	movl	$2621440, %eax
	cmpq	$40, %rdx
	cmovae	%rsi, %rax
	salq	$8, %rdx
	addq	%rdx, %rcx
	addq	%rcx, %rax
	ret
	.cfi_endproc
.LFE12:
	.size	alien_toongs, .-alien_toongs
	.p2align 4,,15
	.globl	alien_toon
	.type	alien_toon, @function
alien_toon:
.LFB13:
	.cfi_startproc
	movq	%rdi, %r9
	movslq	%esi, %rsi
	movq	%rdi, %rax
	andl	$16711680, %r9d
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movzbl	%dil, %edi
	shrq	$16, %r9
	testq	%rsi, %rsi
	movzbl	%ah, %ebx
	je	.L118
	xorl	%r8d, %r8d
	movl	$127, %eax
	.p2align 4,,10
.L119:
	leaq	-256(%r9,%r9), %r10
	movq	%rax, %r11
	subq	%r9, %r11
	addq	%r11, %r11
	cmpq	$127, %r9
	movq	%r10, %r9
	cmovbe	%r11, %r9
	leaq	-256(%rbx,%rbx), %r10
	movq	%rax, %r11
	subq	%rbx, %r11
	addq	%r11, %r11
	cmpq	$127, %rbx
	movq	%r10, %rbx
	cmovbe	%r11, %rbx
	leaq	-256(%rdi,%rdi), %r10
	movq	%rax, %r11
	subq	%rdi, %r11
	addq	%r11, %r11
	cmpq	$127, %rdi
	movq	%r10, %rdi
	cmovbe	%r11, %rdi
	addq	$1, %r8
	cmpq	%rsi, %r8
	jb	.L119
.L118:
	leaq	(%rbx,%rbx,4), %rsi
	leaq	(%r9,%r9,4), %rax
	movslq	%ecx, %rcx
	leaq	(%rsi,%rdi,2), %r8
	leaq	(%rdi,%rdi,4), %rsi
	leaq	(%rax,%rbx,2), %rax
	leaq	(%rsi,%r9,2), %rsi
	addq	%rdi, %rax
	addq	%r9, %r8
	shrq	$3, %rax
	shrq	$3, %r8
	addq	%rbx, %rsi
	shrq	$3, %rsi
	testq	%rcx, %rcx
	je	.L120
	xorl	%edi, %edi
	.p2align 4,,10
.L121:
	imulq	%rax, %rax
	addq	$1, %rdi
	imulq	%r8, %r8
	imulq	%rsi, %rsi
	shrq	$8, %rax
	shrq	$8, %r8
	shrq	$8, %rsi
	cmpq	%rcx, %rdi
	jb	.L121
.L120:
	movl	%edx, %ecx
	movl	$255, %r9d
	shrq	%cl, %rsi
	shrq	%cl, %rax
	shrq	%cl, %r8
	salq	%cl, %rsi
	salq	%cl, %rax
	salq	%cl, %r8
	addq	%rsi, %rsi
	cmpq	$255, %rsi
	cmova	%r9, %rsi
	cmpl	$1, %edx
	jle	.L122
	leal	-1(%rdx), %ecx
	movl	$1, %edi
	sall	%cl, %edi
	movslq	%edi, %rdi
	addq	%rdi, %rax
	cmpq	$255, %rax
	cmova	%r9, %rax
.L122:
	salq	$8, %r8
	salq	$16, %rax
	addq	%r8, %rsi
	addq	%rsi, %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	alien_toon, .-alien_toon
	.p2align 4,,15
	.globl	alien_gsi
	.type	alien_gsi, @function
alien_gsi:
.LFB14:
	.cfi_startproc
	movq	%rdi, %rax
	movq	%rdi, %rdx
	movzbl	%dil, %edi
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -32(%rsp)
	movzbl	%dh, %eax
	movq	%rax, -24(%rsp)
	movq	%rdi, -16(%rsp)
	movq	-32(%rsp), %rax
	movq	-24(%rsp), %rcx
	movq	-16(%rsp), %rdx
	addq	%rcx, %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	movq	-8(%rsp), %rax
	jbe	.L132
	cmpq	$255, %rax
	jbe	.L133
	movq	-8(%rsp), %rax
	cmpq	$511, %rax
	jbe	.L134
.L133:
	movq	-8(%rsp), %rax
	subq	$512, %rax
.L132:
	movq	%rax, %rcx
	movq	%rax, %rdx
	salq	$16, %rcx
	salq	$8, %rdx
	addq	%rcx, %rdx
	addq	%rdx, %rax
	ret
	.p2align 4,,10
.L134:
	movq	-8(%rsp), %rdx
	movl	$511, %eax
	subq	%rdx, %rax
	jmp	.L132
	.cfi_endproc
.LFE14:
	.size	alien_gsi, .-alien_gsi
	.p2align 4,,15
	.globl	alien_gsi6
	.type	alien_gsi6, @function
alien_gsi6:
.LFB15:
	.cfi_startproc
	movq	$0, -40(%rsp)
	movq	-40(%rsp), %rax
	movl	$511, %esi
	cmpq	$5, %rax
	jbe	.L142
	jmp	.L140
	.p2align 4,,10
.L144:
	movq	-40(%rsp), %rax
	movq	-8(%rsp), %rdx
	movq	%rdx, (%rdi,%rax,8)
.L138:
	movq	-40(%rsp), %rax
	addq	$1, %rax
	movq	%rax, -40(%rsp)
	movq	-40(%rsp), %rax
	cmpq	$5, %rax
	ja	.L140
.L142:
	movq	-40(%rsp), %rax
	movzbl	2(%rdi,%rax,8), %eax
	movq	%rax, -32(%rsp)
	movq	-40(%rsp), %rax
	movzbl	1(%rdi,%rax,8), %eax
	movq	%rax, -24(%rsp)
	movq	-40(%rsp), %rax
	movzbl	(%rdi,%rax,8), %eax
	movq	%rax, -16(%rsp)
	movq	-32(%rsp), %rax
	movq	-24(%rsp), %rcx
	movq	-16(%rsp), %rdx
	addq	%rcx, %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L144
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L139
	movq	-8(%rsp), %rax
	cmpq	$511, %rax
	ja	.L139
	movq	-40(%rsp), %rax
	movq	-8(%rsp), %rdx
	movq	%rsi, %rcx
	subq	%rdx, %rcx
	movq	%rcx, (%rdi,%rax,8)
	jmp	.L138
	.p2align 4,,10
.L139:
	movq	-40(%rsp), %rax
	movq	-8(%rsp), %rdx
	subq	$512, %rdx
	movq	%rdx, (%rdi,%rax,8)
	jmp	.L138
	.p2align 4,,10
.L140:
	movq	8(%rdi), %rax
	addq	(%rdi), %rax
	shrq	%rax
	movq	%rax, -32(%rsp)
	movq	24(%rdi), %rax
	addq	16(%rdi), %rax
	shrq	%rax
	movq	%rax, -24(%rsp)
	movq	40(%rdi), %rax
	addq	32(%rdi), %rax
	shrq	%rax
	movq	%rax, -16(%rsp)
	movq	-32(%rsp), %rdx
	movq	-24(%rsp), %rax
	movq	-16(%rsp), %rcx
	salq	$16, %rdx
	salq	$8, %rax
	addq	%rdx, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	ret
	.cfi_endproc
.LFE15:
	.size	alien_gsi6, .-alien_gsi6
	.p2align 4,,15
	.globl	color_gsi3
	.type	color_gsi3, @function
color_gsi3:
.LFB16:
	.cfi_startproc
	andl	$65280, %esi
	movzbl	%dl, %edx
	andl	$16711680, %edi
	addq	%rdx, %rsi
	leaq	(%rsi,%rdi), %rax
	ret
	.cfi_endproc
.LFE16:
	.size	color_gsi3, .-color_gsi3
	.p2align 4,,15
	.globl	alien_gsi3
	.type	alien_gsi3, @function
alien_gsi3:
.LFB17:
	.cfi_startproc
	movq	%rdi, %rax
	movq	%rdi, %rcx
	movzbl	%dil, %edi
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -32(%rsp)
	movzbl	%ch, %eax
	movq	%rax, -24(%rsp)
	movq	%rdi, -16(%rsp)
	movq	-32(%rsp), %rax
	movq	-24(%rsp), %rdi
	movq	-16(%rsp), %rcx
	addq	%rdi, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	ja	.L147
	movq	-8(%rsp), %rdi
.L148:
	movq	%rsi, %rcx
	andl	$16711680, %ecx
	shrq	$16, %rcx
	movq	%rcx, -32(%rsp)
	movq	%rsi, %rcx
	movzbl	%sil, %esi
	movzbl	%ch, %eax
	movq	%rax, -24(%rsp)
	movq	%rsi, -16(%rsp)
	movq	-32(%rsp), %rax
	movq	-24(%rsp), %rsi
	movq	-16(%rsp), %rcx
	addq	%rsi, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	ja	.L150
	movq	-8(%rsp), %rsi
.L151:
	movq	%rdx, %rcx
	movzbl	%dh, %eax
	movzbl	%dl, %edx
	andl	$16711680, %ecx
	shrq	$16, %rcx
	movq	%rcx, -32(%rsp)
	movq	%rax, -24(%rsp)
	movq	%rdx, -16(%rsp)
	movq	-32(%rsp), %rax
	movq	-24(%rsp), %rcx
	movq	-16(%rsp), %rdx
	addq	%rcx, %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	movq	-8(%rsp), %rax
	jbe	.L154
	cmpq	$255, %rax
	jbe	.L155
	movq	-8(%rsp), %rax
	cmpq	$511, %rax
	jbe	.L156
.L155:
	movq	-8(%rsp), %rax
	subq	$512, %rax
.L154:
	salq	$8, %rsi
	salq	$16, %rdi
	addq	%rdi, %rsi
	addq	%rsi, %rax
	ret
	.p2align 4,,10
.L150:
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L152
	movq	-8(%rsp), %rax
	cmpq	$511, %rax
	ja	.L152
	movq	-8(%rsp), %rax
	movl	$511, %esi
	subq	%rax, %rsi
	jmp	.L151
	.p2align 4,,10
.L147:
	movq	-8(%rsp), %rax
	cmpq	$255, %rax
	jbe	.L149
	movq	-8(%rsp), %rax
	cmpq	$511, %rax
	ja	.L149
	movq	-8(%rsp), %rax
	movl	$511, %edi
	subq	%rax, %rdi
	jmp	.L148
	.p2align 4,,10
.L149:
	movq	-8(%rsp), %rdi
	subq	$512, %rdi
	jmp	.L148
	.p2align 4,,10
.L152:
	movq	-8(%rsp), %rsi
	subq	$512, %rsi
	jmp	.L151
	.p2align 4,,10
.L156:
	movq	-8(%rsp), %rdx
	salq	$8, %rsi
	salq	$16, %rdi
	movl	$511, %eax
	addq	%rdi, %rsi
	subq	%rdx, %rax
	addq	%rsi, %rax
	ret
	.cfi_endproc
.LFE17:
	.size	alien_gsi3, .-alien_gsi3
	.p2align 4,,15
	.globl	alien_sine
	.type	alien_sine, @function
alien_sine:
.LFB18:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	movsd	.LC0(%rip), %xmm3
	ucomisd	%xmm3, %xmm0
	jae	.L158
	cvttsd2siq	%xmm0, %rbx
.L159:
	movq	%rbx, %rax
	movsd	%xmm3, (%rsp)
	andl	$16711680, %eax
	shrq	$16, %rax
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC1(%rip), %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	sin@PLT
	movsd	.LC4(%rip), %xmm5
	movzbl	%bh, %eax
	movzbl	%bl, %ebx
	addsd	%xmm0, %xmm5
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm5
	movsd	%xmm5, 16(%rsp)
	divsd	.LC1(%rip), %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	sin@PLT
	movsd	.LC4(%rip), %xmm4
	addsd	%xmm0, %xmm4
	cvtsi2sdq	%rbx, %xmm0
	mulsd	.LC5(%rip), %xmm4
	movsd	%xmm4, 32(%rsp)
	divsd	.LC1(%rip), %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	sin@PLT
	movsd	.LC4(%rip), %xmm2
	movsd	16(%rsp), %xmm5
	addsd	%xmm0, %xmm2
	movapd	%xmm5, %xmm1
	movapd	%xmm5, %xmm0
	mulsd	.LC5(%rip), %xmm2
	movsd	%xmm2, 16(%rsp)
	call	pow@PLT
	movsd	32(%rsp), %xmm4
	movapd	%xmm0, %xmm6
	movapd	%xmm4, %xmm1
	movapd	%xmm4, %xmm0
	movsd	%xmm6, 32(%rsp)
	call	pow@PLT
	movsd	16(%rsp), %xmm2
	movapd	%xmm0, %xmm5
	movapd	%xmm2, %xmm1
	movapd	%xmm2, %xmm0
	movsd	%xmm5, 16(%rsp)
	call	pow@PLT
	movsd	32(%rsp), %xmm6
	movsd	.LC6(%rip), %xmm1
	movapd	%xmm6, %xmm2
	movsd	16(%rsp), %xmm5
	subsd	%xmm1, %xmm0
	movsd	(%rsp), %xmm3
	subsd	%xmm1, %xmm2
	subsd	%xmm1, %xmm5
	movsd	.LC7(%rip), %xmm1
	mulsd	%xmm1, %xmm2
	mulsd	.LC1(%rip), %xmm2
	ucomisd	%xmm3, %xmm2
	jae	.L160
	cvttsd2siq	%xmm2, %rsi
.L161:
	movapd	%xmm5, %xmm2
	mulsd	%xmm1, %xmm2
	mulsd	.LC1(%rip), %xmm2
	ucomisd	%xmm3, %xmm2
	jae	.L162
	cvttsd2siq	%xmm2, %rcx
.L163:
	mulsd	%xmm0, %xmm1
	mulsd	.LC1(%rip), %xmm1
	ucomisd	%xmm3, %xmm1
	jae	.L164
	cvttsd2siq	%xmm1, %rdx
.L165:
	movl	$255, %eax
	cmpq	$255, %rsi
	cmova	%rax, %rsi
	salq	$16, %rsi
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	addq	%rsi, %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	$48, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	addq	%rcx, %rax
	cvtsi2sdq	%rax, %xmm0
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L158:
	.cfi_restore_state
	subsd	%xmm3, %xmm0
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm0, %rbx
	xorq	%rax, %rbx
	jmp	.L159
	.p2align 4,,10
.L164:
	subsd	%xmm3, %xmm1
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm1, %rdx
	xorq	%rax, %rdx
	jmp	.L165
	.p2align 4,,10
.L162:
	subsd	%xmm3, %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rcx
	xorq	%rax, %rcx
	jmp	.L163
	.p2align 4,,10
.L160:
	subsd	%xmm3, %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rsi
	xorq	%rax, %rsi
	jmp	.L161
	.cfi_endproc
.LFE18:
	.size	alien_sine, .-alien_sine
	.p2align 4,,15
	.globl	alien_sine_ex
	.type	alien_sine_ex, @function
alien_sine_ex:
.LFB19:
	.cfi_startproc
	subq	$136, %rsp
	.cfi_def_cfa_offset 144
	movapd	%xmm1, %xmm10
	movsd	.LC0(%rip), %xmm7
	ucomisd	%xmm7, %xmm0
	jae	.L168
	cvttsd2siq	%xmm0, %rax
.L169:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm8
	andl	$16711680, %edx
	movsd	%xmm7, 16(%rsp)
	shrq	$16, %rdx
	movsd	%xmm8, (%rsp)
	cvtsi2sdq	%rdx, %xmm0
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm9
	cvtsi2sdq	%rax, %xmm1
	movsd	%xmm5, 48(%rsp)
	movsd	%xmm6, 32(%rsp)
	movsd	%xmm3, 96(%rsp)
	movsd	%xmm4, 80(%rsp)
	divsd	%xmm8, %xmm0
	divsd	%xmm8, %xmm9
	mulsd	%xmm0, %xmm0
	divsd	%xmm8, %xmm1
	movsd	%xmm9, 112(%rsp)
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm1, 64(%rsp)
	mulsd	%xmm10, %xmm0
	addsd	%xmm2, %xmm0
	call	sin@PLT
	movsd	112(%rsp), %xmm9
	movapd	%xmm0, %xmm10
	movsd	96(%rsp), %xmm3
	movapd	%xmm9, %xmm0
	movsd	80(%rsp), %xmm4
	mulsd	%xmm9, %xmm0
	movsd	%xmm10, 96(%rsp)
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	addsd	%xmm4, %xmm0
	call	sin@PLT
	movsd	64(%rsp), %xmm1
	movapd	%xmm0, %xmm2
	movsd	48(%rsp), %xmm5
	movapd	%xmm1, %xmm0
	movsd	32(%rsp), %xmm6
	mulsd	%xmm1, %xmm0
	movsd	%xmm2, 112(%rsp)
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	%xmm5, %xmm0
	addsd	%xmm6, %xmm0
	call	sin@PLT
	movsd	96(%rsp), %xmm10
	movsd	.LC4(%rip), %xmm1
	movapd	%xmm10, %xmm3
	movsd	.LC5(%rip), %xmm4
	movsd	(%rsp), %xmm8
	addsd	%xmm1, %xmm3
	movsd	16(%rsp), %xmm7
	movsd	112(%rsp), %xmm2
	mulsd	%xmm4, %xmm3
	mulsd	%xmm8, %xmm3
	ucomisd	%xmm7, %xmm3
	jae	.L170
	cvttsd2siq	%xmm3, %rsi
.L171:
	addsd	%xmm1, %xmm2
	mulsd	%xmm4, %xmm2
	mulsd	%xmm8, %xmm2
	ucomisd	%xmm7, %xmm2
	jae	.L172
	cvttsd2siq	%xmm2, %rcx
.L173:
	addsd	%xmm0, %xmm1
	mulsd	%xmm4, %xmm1
	mulsd	%xmm8, %xmm1
	ucomisd	%xmm7, %xmm1
	jae	.L174
	cvttsd2siq	%xmm1, %rdx
.L175:
	movl	$255, %eax
	cmpq	$255, %rsi
	cmova	%rax, %rsi
	salq	$16, %rsi
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	addq	%rsi, %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	$136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	addq	%rcx, %rax
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L168:
	.cfi_restore_state
	subsd	%xmm7, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L169
	.p2align 4,,10
.L174:
	subsd	%xmm7, %xmm1
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm1, %rdx
	xorq	%rax, %rdx
	jmp	.L175
	.p2align 4,,10
.L172:
	subsd	%xmm7, %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rcx
	xorq	%rax, %rcx
	jmp	.L173
	.p2align 4,,10
.L170:
	subsd	%xmm7, %xmm3
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm3, %rsi
	xorq	%rax, %rsi
	jmp	.L171
	.cfi_endproc
.LFE19:
	.size	alien_sine_ex, .-alien_sine_ex
	.p2align 4,,15
	.globl	alien_col_mul
	.type	alien_col_mul, @function
alien_col_mul:
.LFB20:
	.cfi_startproc
	subq	$152, %rsp
	.cfi_def_cfa_offset 160
	ucomisd	.LC0(%rip), %xmm0
	movsd	%xmm5, 128(%rsp)
	movsd	%xmm6, 136(%rsp)
	jae	.L178
	cvttsd2siq	%xmm0, %rax
.L179:
	movq	%rax, %rdx
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm7
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rax, %xmm5
	cvtsi2sdq	%rdx, %xmm6
	divsd	.LC1(%rip), %xmm7
	divsd	.LC1(%rip), %xmm5
	divsd	.LC1(%rip), %xmm6
	sqrtsd	%xmm5, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L193
.L180:
	mulsd	%xmm7, %xmm0
	movsd	%xmm3, 48(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm5, (%rsp)
	movsd	%xmm6, 16(%rsp)
	addsd	%xmm0, %xmm0
	movsd	%xmm7, 64(%rsp)
	mulsd	.LC3(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	addsd	%xmm2, %xmm0
	call	sin@PLT
	movsd	64(%rsp), %xmm7
	movsd	%xmm0, 112(%rsp)
	sqrtsd	%xmm7, %xmm0
	ucomisd	%xmm0, %xmm0
	movsd	48(%rsp), %xmm3
	movsd	32(%rsp), %xmm4
	movsd	(%rsp), %xmm5
	movsd	16(%rsp), %xmm6
	jp	.L194
.L182:
	mulsd	%xmm6, %xmm0
	movsd	%xmm5, (%rsp)
	movsd	%xmm6, 16(%rsp)
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	addsd	%xmm4, %xmm0
	call	sin@PLT
	movsd	16(%rsp), %xmm6
	movsd	%xmm0, 120(%rsp)
	sqrtsd	%xmm6, %xmm0
	ucomisd	%xmm0, %xmm0
	movsd	(%rsp), %xmm5
	jp	.L195
.L184:
	mulsd	%xmm5, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	mulsd	128(%rsp), %xmm0
	addsd	136(%rsp), %xmm0
	call	sin@PLT
	movsd	.LC4(%rip), %xmm1
	movsd	112(%rsp), %xmm2
	movsd	.LC5(%rip), %xmm3
	addsd	%xmm1, %xmm2
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	ucomisd	.LC0(%rip), %xmm2
	jae	.L186
	cvttsd2siq	%xmm2, %rsi
.L187:
	movsd	120(%rsp), %xmm2
	addsd	%xmm1, %xmm2
	mulsd	%xmm3, %xmm2
	mulsd	.LC1(%rip), %xmm2
	ucomisd	.LC0(%rip), %xmm2
	jae	.L188
	cvttsd2siq	%xmm2, %rcx
.L189:
	addsd	%xmm0, %xmm1
	mulsd	%xmm3, %xmm1
	mulsd	.LC1(%rip), %xmm1
	ucomisd	.LC0(%rip), %xmm1
	jae	.L190
	cvttsd2siq	%xmm1, %rdx
.L191:
	movl	$255, %eax
	cmpq	$255, %rsi
	cmova	%rax, %rsi
	salq	$16, %rsi
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	addq	%rsi, %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	$152, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	addq	%rcx, %rax
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L178:
	.cfi_restore_state
	subsd	.LC0(%rip), %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L179
	.p2align 4,,10
.L190:
	subsd	.LC0(%rip), %xmm1
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm1, %rdx
	xorq	%rax, %rdx
	jmp	.L191
	.p2align 4,,10
.L188:
	subsd	.LC0(%rip), %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rcx
	xorq	%rax, %rcx
	jmp	.L189
	.p2align 4,,10
.L186:
	subsd	.LC0(%rip), %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rsi
	xorq	%rax, %rsi
	jmp	.L187
.L193:
	movapd	%xmm5, %xmm0
	movsd	%xmm1, 96(%rsp)
	movsd	%xmm2, 80(%rsp)
	movsd	%xmm3, 48(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm5, (%rsp)
	movsd	%xmm6, 16(%rsp)
	movsd	%xmm7, 64(%rsp)
	call	sqrt@PLT
	movsd	64(%rsp), %xmm7
	movsd	16(%rsp), %xmm6
	movsd	(%rsp), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	48(%rsp), %xmm3
	movsd	80(%rsp), %xmm2
	movsd	96(%rsp), %xmm1
	jmp	.L180
.L195:
	movapd	%xmm6, %xmm0
	call	sqrt@PLT
	movsd	(%rsp), %xmm5
	jmp	.L184
.L194:
	movapd	%xmm7, %xmm0
	call	sqrt@PLT
	movsd	16(%rsp), %xmm6
	movsd	(%rsp), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	48(%rsp), %xmm3
	jmp	.L182
	.cfi_endproc
.LFE20:
	.size	alien_col_mul, .-alien_col_mul
	.p2align 4,,15
	.globl	color_mul
	.type	color_mul, @function
color_mul:
.LFB21:
	.cfi_startproc
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L197
	cvttsd2siq	%xmm0, %rax
.L198:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm0
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm2
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm4
	cvtsi2sdq	%rax, %xmm3
	divsd	%xmm0, %xmm2
	divsd	%xmm0, %xmm4
	movapd	%xmm2, %xmm5
	divsd	%xmm0, %xmm3
	mulsd	%xmm4, %xmm5
	mulsd	%xmm0, %xmm5
	ucomisd	%xmm1, %xmm5
	jae	.L199
	cvttsd2siq	%xmm5, %rsi
.L200:
	mulsd	%xmm3, %xmm4
	mulsd	%xmm0, %xmm4
	ucomisd	%xmm1, %xmm4
	jae	.L201
	cvttsd2siq	%xmm4, %rcx
.L202:
	mulsd	%xmm3, %xmm2
	mulsd	%xmm0, %xmm2
	ucomisd	%xmm1, %xmm2
	jae	.L203
	cvttsd2siq	%xmm2, %rdx
.L204:
	movl	$255, %eax
	cmpq	$255, %rsi
	cmova	%rax, %rsi
	salq	$16, %rsi
	cmpq	$255, %rcx
	cmova	%rax, %rcx
	salq	$8, %rcx
	addq	%rsi, %rcx
	cmpq	$255, %rdx
	cmovbe	%rdx, %rax
	addq	%rcx, %rax
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L197:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L198
	.p2align 4,,10
.L203:
	subsd	%xmm1, %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rdx
	xorq	%rax, %rdx
	jmp	.L204
	.p2align 4,,10
.L201:
	subsd	%xmm1, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rcx
	xorq	%rax, %rcx
	jmp	.L202
	.p2align 4,,10
.L199:
	subsd	%xmm1, %xmm5
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm5, %rsi
	xorq	%rax, %rsi
	jmp	.L200
	.cfi_endproc
.LFE21:
	.size	color_mul, .-color_mul
	.p2align 4,,15
	.globl	color_invi3
	.type	color_invi3, @function
color_invi3:
.LFB22:
	.cfi_startproc
	movq	%rdi, %rcx
	movq	%rdi, %rax
	notq	%rcx
	movzbl	%ah, %edi
	movzbl	%cl, %ecx
	addq	$255, %rcx
	subq	%rdi, %rcx
	shrq	%rcx
	cmpq	$127, %rcx
	leaq	-256(%rcx,%rcx), %rdi
	ja	.L207
	movl	$127, %edi
	subq	%rcx, %rdi
	addq	%rdi, %rdi
.L207:
	movq	%rsi, %rcx
	andl	$16711680, %esi
	notq	%rcx
	shrq	$16, %rsi
	movzbl	%cl, %ecx
	addq	$255, %rcx
	subq	%rsi, %rcx
	shrq	%rcx
	cmpq	$127, %rcx
	leaq	-256(%rcx,%rcx), %rsi
	ja	.L209
	movl	$127, %esi
	subq	%rcx, %rsi
	addq	%rsi, %rsi
.L209:
	movzbl	%dh, %eax
	andl	$16711680, %edx
	movl	$510, %ecx
	shrq	$16, %rdx
	addq	%rax, %rdx
	subq	%rdx, %rcx
	shrq	%rcx
	cmpq	$127, %rcx
	leaq	-256(%rcx,%rcx), %rax
	ja	.L211
	movl	$127, %eax
	subq	%rcx, %rax
	addq	%rax, %rax
.L211:
	salq	$8, %rsi
	salq	$16, %rdi
	addq	%rdi, %rsi
	addq	%rsi, %rax
	ret
	.cfi_endproc
.LFE22:
	.size	color_invi3, .-color_invi3
	.p2align 4,,15
	.globl	color_invi
	.type	color_invi, @function
color_invi:
.LFB23:
	.cfi_startproc
	movq	%rdi, %rax
	movl	$255, %ecx
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	andl	$16711680, %eax
	movq	%rcx, %rdx
	movq	%rdi, %rbx
	shrq	$16, %rax
	notq	%rdi
	subq	%rax, %rdx
	movzbl	%dil, %edi
	movq	%rdx, %rax
	movzbl	%bh, %edx
	subq	%rdx, %rcx
	leaq	(%rcx,%rdi), %rdx
	addq	%rax, %rdi
	addq	%rax, %rcx
	shrq	%rdi
	shrq	%rcx
	shrq	%rdx
	cmpq	$127, %rdx
	leaq	-256(%rdx,%rdx), %rsi
	ja	.L214
	movl	$127, %esi
	subq	%rdx, %rsi
	addq	%rsi, %rsi
.L214:
	cmpq	$127, %rdi
	leaq	-256(%rdi,%rdi), %rdx
	ja	.L216
	movl	$127, %edx
	subq	%rdi, %rdx
	addq	%rdx, %rdx
.L216:
	cmpq	$127, %rcx
	leaq	-256(%rcx,%rcx), %rax
	ja	.L218
	movl	$127, %eax
	subq	%rcx, %rax
	addq	%rax, %rax
.L218:
	salq	$8, %rdx
	salq	$16, %rsi
	addq	%rsi, %rdx
	addq	%rdx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE23:
	.size	color_invi, .-color_invi
	.p2align 4,,15
	.globl	color_inv2
	.type	color_inv2, @function
color_inv2:
.LFB24:
	.cfi_startproc
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L221
	cvttsd2siq	%xmm0, %rax
.L222:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm0
	andl	$16711680, %edx
	movsd	.LC4(%rip), %xmm3
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm2
	movapd	%xmm3, %xmm4
	movzbl	%ah, %edx
	movapd	%xmm3, %xmm5
	movzbl	%al, %eax
	divsd	%xmm0, %xmm2
	subsd	%xmm2, %xmm4
	movapd	%xmm4, %xmm2
	cvtsi2sdq	%rdx, %xmm4
	divsd	%xmm0, %xmm4
	subsd	%xmm4, %xmm5
	cvtsi2sdq	%rax, %xmm4
	divsd	%xmm0, %xmm4
	subsd	%xmm4, %xmm3
	movapd	%xmm3, %xmm4
	mulsd	%xmm5, %xmm4
	mulsd	%xmm0, %xmm4
	ucomisd	%xmm1, %xmm4
	jae	.L223
	cvttsd2siq	%xmm4, %rdx
.L224:
	mulsd	%xmm2, %xmm3
	salq	$16, %rdx
	mulsd	%xmm0, %xmm3
	ucomisd	%xmm1, %xmm3
	jae	.L225
	cvttsd2siq	%xmm3, %rcx
.L226:
	mulsd	%xmm5, %xmm2
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	%xmm0, %xmm2
	ucomisd	%xmm1, %xmm2
	jae	.L227
	cvttsd2siq	%xmm2, %rax
	addq	%rdx, %rax
	js	.L229
.L231:
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L221:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L222
	.p2align 4,,10
.L227:
	subsd	%xmm1, %xmm2
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm2, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L231
.L229:
	movq	%rax, %rcx
	andl	$1, %eax
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L225:
	subsd	%xmm1, %xmm3
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm3, %rcx
	xorq	%rax, %rcx
	jmp	.L226
	.p2align 4,,10
.L223:
	subsd	%xmm1, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rdx
	xorq	%rax, %rdx
	jmp	.L224
	.cfi_endproc
.LFE24:
	.size	color_inv2, .-color_inv2
	.p2align 4,,15
	.globl	alien_inv
	.type	alien_inv, @function
alien_inv:
.LFB25:
	.cfi_startproc
	subq	$120, %rsp
	.cfi_def_cfa_offset 128
	movsd	.LC0(%rip), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L233
	cvttsd2siq	%xmm0, %rax
.L234:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm3
	andl	$16711680, %edx
	movsd	.LC4(%rip), %xmm1
	shrq	$16, %rdx
	movsd	%xmm2, 32(%rsp)
	cvtsi2sdq	%rdx, %xmm0
	movapd	%xmm1, %xmm4
	movzbl	%ah, %edx
	movzbl	%al, %eax
	movsd	%xmm1, (%rsp)
	movapd	%xmm1, %xmm6
	movsd	%xmm3, 16(%rsp)
	movapd	%xmm1, %xmm5
	divsd	%xmm3, %xmm0
	subsd	%xmm0, %xmm4
	cvtsi2sdq	%rdx, %xmm0
	movsd	%xmm4, 88(%rsp)
	cvtsi2sdq	%rax, %xmm4
	divsd	%xmm3, %xmm0
	divsd	%xmm3, %xmm4
	subsd	%xmm0, %xmm5
	movsd	%xmm5, 48(%rsp)
	subsd	%xmm4, %xmm6
	movapd	%xmm6, %xmm0
	movsd	%xmm6, 64(%rsp)
	mulsd	%xmm5, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	64(%rsp), %xmm6
	movsd	%xmm0, 96(%rsp)
	movsd	88(%rsp), %xmm0
	mulsd	%xmm6, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	48(%rsp), %xmm5
	movsd	%xmm0, 104(%rsp)
	movsd	88(%rsp), %xmm0
	mulsd	%xmm5, %xmm0
	addsd	%xmm0, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	(%rsp), %xmm1
	movsd	.LC5(%rip), %xmm5
	movapd	%xmm1, %xmm4
	movsd	16(%rsp), %xmm3
	movsd	32(%rsp), %xmm2
	subsd	96(%rsp), %xmm4
	mulsd	%xmm5, %xmm4
	mulsd	%xmm3, %xmm4
	ucomisd	%xmm2, %xmm4
	jae	.L235
	cvttsd2siq	%xmm4, %rdx
.L236:
	movapd	%xmm1, %xmm4
	salq	$16, %rdx
	subsd	104(%rsp), %xmm4
	mulsd	%xmm5, %xmm4
	mulsd	%xmm3, %xmm4
	ucomisd	%xmm2, %xmm4
	jae	.L237
	cvttsd2siq	%xmm4, %rcx
.L238:
	subsd	%xmm0, %xmm1
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	%xmm5, %xmm1
	mulsd	%xmm3, %xmm1
	ucomisd	%xmm2, %xmm1
	jae	.L239
	cvttsd2siq	%xmm1, %rax
	addq	%rdx, %rax
	js	.L241
.L244:
	cvtsi2sdq	%rax, %xmm0
	addq	$120, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L233:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L234
	.p2align 4,,10
.L239:
	subsd	%xmm2, %xmm1
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm1, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L244
.L241:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$120, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L237:
	.cfi_restore_state
	subsd	%xmm2, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rcx
	xorq	%rax, %rcx
	jmp	.L238
	.p2align 4,,10
.L235:
	subsd	%xmm2, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rdx
	xorq	%rax, %rdx
	jmp	.L236
	.cfi_endproc
.LFE25:
	.size	alien_inv, .-alien_inv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC9:
	.string	"x=%f, l=%f --> error\n"
	.text
	.p2align 4,,15
	.globl	get_heat
	.type	get_heat, @function
get_heat:
.LFB26:
	.cfi_startproc
	xorpd	%xmm2, %xmm2
	movq	%rbx, -24(%rsp)
	movq	%rbp, -16(%rsp)
	movq	%r12, -8(%rsp)
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	movq	%rdx, %r12
	ucomisd	%xmm0, %xmm2
	ja	.L246
	movsd	.LC4(%rip), %xmm3
	ucomisd	%xmm3, %xmm0
	jbe	.L277
.L246:
	leaq	.LC9(%rip), %rdi
	movl	$2, %eax
	call	printf@PLT
	movabsq	$4607182418800017408, %rax
	movabsq	$4607182418800017408, %rdx
	movabsq	$4607182418800017408, %rcx
	movq	%rax, (%r12)
	movq	%rdx, 0(%rbp)
	movq	%rcx, (%rbx)
.L245:
	movq	(%rsp), %rbx
	movq	8(%rsp), %rbp
	movq	16(%rsp), %r12
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L277:
	.cfi_restore_state
	ucomisd	%xmm1, %xmm2
	ja	.L246
	ucomisd	%xmm3, %xmm1
	ja	.L246
	movsd	.LC10(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jae	.L278
	movsd	.LC12(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L271
	mulsd	.LC11(%rip), %xmm0
	subsd	%xmm3, %xmm0
	subsd	%xmm0, %xmm3
	movapd	%xmm1, %xmm0
	mulsd	%xmm3, %xmm1
	movapd	%xmm2, %xmm3
.L252:
	movsd	%xmm1, (%rbx)
	movsd	%xmm0, 0(%rbp)
	movsd	%xmm3, (%r12)
	jmp	.L245
	.p2align 4,,10
.L278:
	mulsd	.LC11(%rip), %xmm0
	movapd	%xmm2, %xmm3
	mulsd	%xmm1, %xmm0
	jmp	.L252
	.p2align 4,,10
.L271:
	movsd	.LC5(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L272
	movsd	.LC11(%rip), %xmm3
	mulsd	%xmm0, %xmm3
	movapd	%xmm1, %xmm0
	subsd	.LC13(%rip), %xmm3
	mulsd	%xmm1, %xmm3
	movapd	%xmm2, %xmm1
	jmp	.L252
	.p2align 4,,10
.L272:
	movsd	.LC14(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L273
	movsd	.LC11(%rip), %xmm4
	mulsd	%xmm0, %xmm4
	movapd	%xmm3, %xmm0
	movapd	%xmm1, %xmm3
	subsd	.LC2(%rip), %xmm4
	subsd	%xmm4, %xmm0
	mulsd	%xmm1, %xmm0
	movapd	%xmm2, %xmm1
	jmp	.L252
.L273:
	movsd	.LC15(%rip), %xmm4
	ucomisd	%xmm0, %xmm4
	jb	.L274
	mulsd	.LC11(%rip), %xmm0
	movapd	%xmm1, %xmm3
	subsd	.LC16(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movapd	%xmm2, %xmm0
	jmp	.L252
.L274:
	ucomisd	%xmm0, %xmm3
	jae	.L279
	movapd	%xmm3, %xmm0
	movapd	%xmm3, %xmm1
	jmp	.L252
.L279:
	mulsd	.LC11(%rip), %xmm0
	subsd	.LC17(%rip), %xmm0
	subsd	%xmm0, %xmm3
	movapd	%xmm2, %xmm0
	mulsd	%xmm1, %xmm3
	jmp	.L252
	.cfi_endproc
.LFE26:
	.size	get_heat, .-get_heat
	.p2align 4,,15
	.globl	color_hue2
	.type	color_hue2, @function
color_hue2:
.LFB27:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movsd	.LC0(%rip), %xmm2
	ucomisd	%xmm2, %xmm0
	jae	.L281
	cvttsd2siq	%xmm0, %rax
.L282:
	movzbl	%ah, %edx
	movzbl	%al, %eax
	leaq	48(%rsp), %rsi
	cvtsi2sdq	%rdx, %xmm1
	leaq	40(%rsp), %rdi
	cvtsi2sdq	%rax, %xmm0
	leaq	56(%rsp), %rdx
	movsd	.LC1(%rip), %xmm3
	movsd	%xmm2, 16(%rsp)
	movsd	%xmm3, (%rsp)
	divsd	%xmm3, %xmm1
	divsd	%xmm3, %xmm0
	call	get_heat@PLT
	movsd	(%rsp), %xmm3
	movsd	40(%rsp), %xmm0
	movsd	16(%rsp), %xmm2
	mulsd	%xmm3, %xmm0
	ucomisd	%xmm2, %xmm0
	jae	.L283
	cvttsd2siq	%xmm0, %rdx
.L284:
	movsd	48(%rsp), %xmm0
	salq	$16, %rdx
	mulsd	%xmm3, %xmm0
	ucomisd	%xmm2, %xmm0
	jae	.L285
	cvttsd2siq	%xmm0, %rcx
.L286:
	mulsd	56(%rsp), %xmm3
	salq	$8, %rcx
	addq	%rcx, %rdx
	ucomisd	%xmm2, %xmm3
	jae	.L287
	cvttsd2siq	%xmm3, %rax
	addq	%rdx, %rax
	js	.L289
.L292:
	cvtsi2sdq	%rax, %xmm0
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L281:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L282
	.p2align 4,,10
.L287:
	subsd	%xmm2, %xmm3
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm3, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L292
.L289:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L285:
	.cfi_restore_state
	subsd	%xmm2, %xmm0
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm0, %rcx
	xorq	%rax, %rcx
	jmp	.L286
	.p2align 4,,10
.L283:
	subsd	%xmm2, %xmm0
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm0, %rdx
	xorq	%rax, %rdx
	jmp	.L284
	.cfi_endproc
.LFE27:
	.size	color_hue2, .-color_hue2
	.p2align 4,,15
	.globl	color_hue
	.type	color_hue, @function
color_hue:
.LFB28:
	.cfi_startproc
	subq	$248, %rsp
	.cfi_def_cfa_offset 256
	movsd	.LC0(%rip), %xmm3
	ucomisd	%xmm3, %xmm0
	jae	.L294
	cvttsd2siq	%xmm0, %rax
.L295:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm4
	andl	$16711680, %edx
	leaq	144(%rsp), %rsi
	leaq	96(%rsp), %rdi
	shrq	$16, %rdx
	movsd	%xmm3, 16(%rsp)
	cvtsi2sdq	%rdx, %xmm0
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm6
	leaq	192(%rsp), %rdx
	cvtsi2sdq	%rax, %xmm5
	movsd	%xmm4, (%rsp)
	divsd	%xmm4, %xmm0
	divsd	%xmm4, %xmm6
	movapd	%xmm0, %xmm2
	divsd	%xmm4, %xmm5
	addsd	%xmm6, %xmm2
	movsd	%xmm6, 64(%rsp)
	addsd	%xmm5, %xmm2
	movsd	%xmm5, 32(%rsp)
	divsd	.LC2(%rip), %xmm2
	movapd	%xmm2, %xmm1
	movsd	%xmm2, 48(%rsp)
	call	get_heat@PLT
	movsd	48(%rsp), %xmm2
	leaq	208(%rsp), %rdx
	movsd	64(%rsp), %xmm6
	leaq	160(%rsp), %rsi
	movapd	%xmm2, %xmm1
	leaq	112(%rsp), %rdi
	movapd	%xmm6, %xmm0
	call	get_heat@PLT
	movsd	48(%rsp), %xmm2
	leaq	224(%rsp), %rdx
	movsd	32(%rsp), %xmm5
	leaq	176(%rsp), %rsi
	movapd	%xmm2, %xmm1
	leaq	128(%rsp), %rdi
	movapd	%xmm5, %xmm0
	call	get_heat@PLT
	movsd	96(%rsp), %xmm1
	movsd	144(%rsp), %xmm0
	movsd	16(%rsp), %xmm3
	ucomisd	%xmm1, %xmm0
	movsd	(%rsp), %xmm4
	jbe	.L328
	movapd	%xmm1, %xmm0
	maxsd	192(%rsp), %xmm0
.L298:
	movsd	112(%rsp), %xmm2
	movsd	160(%rsp), %xmm1
	ucomisd	%xmm2, %xmm1
	jbe	.L329
	movapd	%xmm2, %xmm5
	maxsd	208(%rsp), %xmm5
.L303:
	movsd	128(%rsp), %xmm2
	movsd	176(%rsp), %xmm1
	ucomisd	%xmm2, %xmm1
	jbe	.L330
	maxsd	224(%rsp), %xmm2
	movsd	%xmm2, 88(%rsp)
.L308:
	mulsd	.LC3(%rip), %xmm0
	movsd	%xmm3, 16(%rsp)
	movsd	%xmm4, (%rsp)
	movsd	%xmm5, 32(%rsp)
	call	cos@PLT
	movsd	32(%rsp), %xmm5
	movapd	%xmm0, %xmm1
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm5, %xmm0
	movsd	%xmm1, 64(%rsp)
	call	cos@PLT
	movapd	%xmm0, %xmm5
	movsd	.LC3(%rip), %xmm0
	movsd	%xmm5, 32(%rsp)
	mulsd	88(%rsp), %xmm0
	call	cos@PLT
	movsd	64(%rsp), %xmm1
	movsd	(%rsp), %xmm4
	movapd	%xmm1, %xmm2
	movsd	16(%rsp), %xmm3
	movsd	32(%rsp), %xmm5
	mulsd	%xmm1, %xmm2
	movsd	.LC4(%rip), %xmm1
	movapd	%xmm1, %xmm6
	subsd	%xmm2, %xmm6
	movapd	%xmm6, %xmm2
	mulsd	%xmm4, %xmm2
	ucomisd	%xmm3, %xmm2
	jae	.L311
	cvttsd2siq	%xmm2, %rdx
.L312:
	mulsd	%xmm5, %xmm5
	movapd	%xmm1, %xmm2
	salq	$16, %rdx
	subsd	%xmm5, %xmm2
	mulsd	%xmm4, %xmm2
	ucomisd	%xmm3, %xmm2
	jae	.L313
	cvttsd2siq	%xmm2, %rcx
.L314:
	mulsd	%xmm0, %xmm0
	salq	$8, %rcx
	addq	%rcx, %rdx
	subsd	%xmm0, %xmm1
	mulsd	%xmm4, %xmm1
	ucomisd	%xmm3, %xmm1
	jae	.L315
	cvttsd2siq	%xmm1, %rax
	addq	%rdx, %rax
	js	.L317
.L332:
	cvtsi2sdq	%rax, %xmm0
	addq	$248, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L294:
	.cfi_restore_state
	subsd	%xmm3, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L295
	.p2align 4,,10
.L315:
	subsd	%xmm3, %xmm1
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm1, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L332
.L317:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$248, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L313:
	.cfi_restore_state
	subsd	%xmm3, %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rcx
	xorq	%rax, %rcx
	jmp	.L314
	.p2align 4,,10
.L311:
	subsd	%xmm3, %xmm2
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm2, %rdx
	xorq	%rax, %rdx
	jmp	.L312
	.p2align 4,,10
.L330:
	maxsd	224(%rsp), %xmm1
	movsd	%xmm1, 88(%rsp)
	jmp	.L308
	.p2align 4,,10
.L329:
	movapd	%xmm1, %xmm5
	maxsd	208(%rsp), %xmm5
	jmp	.L303
	.p2align 4,,10
.L328:
	maxsd	192(%rsp), %xmm0
	jmp	.L298
	.cfi_endproc
.LFE28:
	.size	color_hue, .-color_hue
	.p2align 4,,15
	.globl	color_complex
	.type	color_complex, @function
color_complex:
.LFB29:
	.cfi_startproc
	subq	$152, %rsp
	.cfi_def_cfa_offset 160
	ucomisd	.LC0(%rip), %xmm0
	jae	.L334
	cvttsd2siq	%xmm0, %rax
.L335:
	movq	%rax, %rdx
	movsd	.LC18(%rip), %xmm5
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm6
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm1
	cvtsi2sdq	%rax, %xmm2
	divsd	.LC1(%rip), %xmm6
	divsd	.LC1(%rip), %xmm1
	divsd	.LC1(%rip), %xmm2
	divsd	%xmm5, %xmm1
	divsd	%xmm5, %xmm2
	movapd	%xmm1, %xmm8
	mulsd	%xmm1, %xmm8
	movapd	%xmm2, %xmm7
	movapd	%xmm8, %xmm0
	mulsd	%xmm2, %xmm7
	addsd	%xmm7, %xmm0
	sqrtsd	%xmm0, %xmm3
	ucomisd	%xmm3, %xmm3
	jp	.L366
.L336:
	xorpd	%xmm4, %xmm4
	ucomisd	%xmm4, %xmm3
	jbe	.L338
	movapd	%xmm1, %xmm0
	divsd	%xmm3, %xmm0
	ucomisd	.LC4(%rip), %xmm0
	ja	.L340
	movsd	.LC19(%rip), %xmm1
	ucomisd	%xmm0, %xmm1
	ja	.L340
.L338:
	divsd	%xmm5, %xmm6
	movapd	%xmm8, %xmm0
	movapd	%xmm6, %xmm3
	mulsd	%xmm6, %xmm3
	addsd	%xmm3, %xmm0
	sqrtsd	%xmm0, %xmm1
	ucomisd	%xmm1, %xmm1
	jp	.L367
.L342:
	ucomisd	%xmm4, %xmm1
	movapd	%xmm4, %xmm5
	jbe	.L344
	movapd	%xmm6, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, 32(%rsp)
	divsd	%xmm1, %xmm0
	movsd	%xmm3, 96(%rsp)
	movsd	%xmm4, 48(%rsp)
	movsd	%xmm7, 64(%rsp)
	call	acos@PLT
	movsd	16(%rsp), %xmm1
	movapd	%xmm0, %xmm5
	movsd	32(%rsp), %xmm2
	movsd	96(%rsp), %xmm3
	movsd	48(%rsp), %xmm4
	movsd	64(%rsp), %xmm7
.L344:
	addsd	%xmm3, %xmm7
	sqrtsd	%xmm7, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L368
.L346:
	ucomisd	%xmm4, %xmm0
	jbe	.L348
	divsd	%xmm0, %xmm2
	ucomisd	.LC4(%rip), %xmm2
	ja	.L350
	movsd	.LC19(%rip), %xmm0
	ucomisd	%xmm2, %xmm0
	ja	.L350
.L348:
	movapd	%xmm5, %xmm0
	leaq	136(%rsp), %rdx
	leaq	128(%rsp), %rsi
	leaq	120(%rsp), %rdi
	divsd	.LC3(%rip), %xmm0
	call	get_heat@PLT
	movsd	.LC1(%rip), %xmm0
	mulsd	120(%rsp), %xmm0
	ucomisd	.LC0(%rip), %xmm0
	jae	.L352
	cvttsd2siq	%xmm0, %rdx
.L353:
	movsd	.LC1(%rip), %xmm0
	salq	$16, %rdx
	mulsd	128(%rsp), %xmm0
	ucomisd	.LC0(%rip), %xmm0
	jae	.L354
	cvttsd2siq	%xmm0, %rcx
.L355:
	movsd	.LC1(%rip), %xmm0
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	136(%rsp), %xmm0
	ucomisd	.LC0(%rip), %xmm0
	jae	.L356
	cvttsd2siq	%xmm0, %rax
	addq	%rdx, %rax
	js	.L358
.L369:
	cvtsi2sdq	%rax, %xmm0
	addq	$152, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L350:
	.cfi_restore_state
	movapd	%xmm2, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm5, (%rsp)
	call	acos@PLT
	movsd	(%rsp), %xmm5
	movsd	16(%rsp), %xmm1
	jmp	.L348
	.p2align 4,,10
.L340:
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm4, 48(%rsp)
	movsd	%xmm5, (%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 64(%rsp)
	movsd	%xmm8, 96(%rsp)
	call	acos@PLT
	movsd	96(%rsp), %xmm8
	movsd	64(%rsp), %xmm7
	movsd	80(%rsp), %xmm6
	movsd	(%rsp), %xmm5
	movsd	48(%rsp), %xmm4
	movsd	32(%rsp), %xmm2
	jmp	.L338
	.p2align 4,,10
.L356:
	subsd	.LC0(%rip), %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L369
.L358:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$152, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L334:
	.cfi_restore_state
	subsd	.LC0(%rip), %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L335
	.p2align 4,,10
.L354:
	subsd	.LC0(%rip), %xmm0
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm0, %rcx
	xorq	%rax, %rcx
	jmp	.L355
	.p2align 4,,10
.L352:
	subsd	.LC0(%rip), %xmm0
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm0, %rdx
	xorq	%rax, %rdx
	jmp	.L353
.L368:
	movapd	%xmm7, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm4, 48(%rsp)
	movsd	%xmm5, (%rsp)
	call	sqrt@PLT
	movsd	(%rsp), %xmm5
	movsd	48(%rsp), %xmm4
	movsd	32(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L346
.L367:
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm3, 96(%rsp)
	movsd	%xmm4, 48(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 64(%rsp)
	call	sqrt@PLT
	movsd	64(%rsp), %xmm7
	movapd	%xmm0, %xmm1
	movsd	80(%rsp), %xmm6
	movsd	48(%rsp), %xmm4
	movsd	96(%rsp), %xmm3
	movsd	32(%rsp), %xmm2
	jmp	.L342
.L366:
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, 32(%rsp)
	movsd	%xmm5, (%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 64(%rsp)
	movsd	%xmm8, 96(%rsp)
	call	sqrt@PLT
	movsd	96(%rsp), %xmm8
	movapd	%xmm0, %xmm3
	movsd	64(%rsp), %xmm7
	movsd	80(%rsp), %xmm6
	movsd	(%rsp), %xmm5
	movsd	32(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L336
	.cfi_endproc
.LFE29:
	.size	color_complex, .-color_complex
	.p2align 4,,15
	.globl	color_angle
	.type	color_angle, @function
color_angle:
.LFB30:
	.cfi_startproc
	subq	$152, %rsp
	.cfi_def_cfa_offset 160
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L371
	cvttsd2siq	%xmm0, %rax
.L372:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm2
	andl	$16711680, %edx
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm3
	movzbl	%ah, %edx
	movzbl	%al, %eax
	cvtsi2sdq	%rdx, %xmm5
	cvtsi2sdq	%rax, %xmm4
	divsd	%xmm2, %xmm3
	divsd	%xmm2, %xmm5
	divsd	%xmm2, %xmm4
	movapd	%xmm5, %xmm9
	mulsd	%xmm5, %xmm9
	movapd	%xmm4, %xmm10
	movapd	%xmm9, %xmm0
	mulsd	%xmm4, %xmm10
	addsd	%xmm10, %xmm0
	sqrtsd	%xmm0, %xmm7
	ucomisd	%xmm7, %xmm7
	jp	.L403
.L373:
	xorpd	%xmm6, %xmm6
	ucomisd	%xmm6, %xmm7
	jbe	.L399
	divsd	%xmm7, %xmm5
.L375:
	movapd	%xmm3, %xmm8
	mulsd	%xmm3, %xmm8
	addsd	%xmm8, %xmm10
	sqrtsd	%xmm10, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L404
.L377:
	ucomisd	%xmm6, %xmm0
	jbe	.L400
	divsd	%xmm0, %xmm4
	movsd	%xmm4, 128(%rsp)
.L379:
	addsd	%xmm9, %xmm8
	sqrtsd	%xmm8, %xmm0
	ucomisd	%xmm0, %xmm0
	jp	.L405
.L381:
	ucomisd	%xmm6, %xmm0
	jbe	.L401
	divsd	%xmm0, %xmm3
	movsd	%xmm3, 136(%rsp)
.L383:
	movapd	%xmm5, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	call	asin@PLT
	movapd	%xmm0, %xmm4
	movsd	128(%rsp), %xmm0
	movsd	%xmm4, 32(%rsp)
	call	asin@PLT
	movapd	%xmm0, %xmm3
	movsd	136(%rsp), %xmm0
	movsd	%xmm3, 48(%rsp)
	call	asin@PLT
	movsd	.LC3(%rip), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	(%rsp), %xmm2
	divsd	%xmm5, %xmm4
	movsd	16(%rsp), %xmm1
	movsd	48(%rsp), %xmm3
	mulsd	%xmm2, %xmm4
	ucomisd	%xmm1, %xmm4
	jae	.L385
	cvttsd2siq	%xmm4, %rdx
.L386:
	divsd	%xmm5, %xmm3
	salq	$16, %rdx
	mulsd	%xmm2, %xmm3
	ucomisd	%xmm1, %xmm3
	jae	.L387
	cvttsd2siq	%xmm3, %rcx
.L388:
	divsd	%xmm5, %xmm0
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L389
	cvttsd2siq	%xmm0, %rax
	addq	%rdx, %rax
	js	.L391
.L406:
	cvtsi2sdq	%rax, %xmm0
	addq	$152, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L371:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L372
	.p2align 4,,10
.L389:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L406
.L391:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$152, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L387:
	.cfi_restore_state
	subsd	%xmm1, %xmm3
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm3, %rcx
	xorq	%rax, %rcx
	jmp	.L388
	.p2align 4,,10
.L385:
	subsd	%xmm1, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rdx
	xorq	%rax, %rdx
	jmp	.L386
	.p2align 4,,10
.L399:
	movapd	%xmm6, %xmm5
	jmp	.L375
	.p2align 4,,10
.L400:
	movsd	%xmm6, 128(%rsp)
	jmp	.L379
	.p2align 4,,10
.L401:
	movsd	%xmm6, 136(%rsp)
	jmp	.L383
.L403:
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 48(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm5, 64(%rsp)
	movsd	%xmm9, 96(%rsp)
	movsd	%xmm10, 112(%rsp)
	call	sqrt@PLT
	movsd	112(%rsp), %xmm10
	movapd	%xmm0, %xmm7
	movsd	96(%rsp), %xmm9
	movsd	64(%rsp), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	48(%rsp), %xmm3
	movsd	(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L373
.L405:
	movapd	%xmm8, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 48(%rsp)
	movsd	%xmm5, 64(%rsp)
	movsd	%xmm6, 80(%rsp)
	call	sqrt@PLT
	movsd	80(%rsp), %xmm6
	movsd	64(%rsp), %xmm5
	movsd	48(%rsp), %xmm3
	movsd	(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L381
.L404:
	movapd	%xmm10, %xmm0
	movsd	%xmm1, 16(%rsp)
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 48(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm5, 64(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm8, 112(%rsp)
	movsd	%xmm9, 96(%rsp)
	call	sqrt@PLT
	movsd	96(%rsp), %xmm9
	movsd	112(%rsp), %xmm8
	movsd	80(%rsp), %xmm6
	movsd	64(%rsp), %xmm5
	movsd	32(%rsp), %xmm4
	movsd	48(%rsp), %xmm3
	movsd	(%rsp), %xmm2
	movsd	16(%rsp), %xmm1
	jmp	.L377
	.cfi_endproc
.LFE30:
	.size	color_angle, .-color_angle
	.p2align 4,,15
	.globl	color_heati2
	.type	color_heati2, @function
color_heati2:
.LFB31:
	.cfi_startproc
	movq	%rdi, %rax
	movq	%rdi, %rdx
	movzbl	%dil, %edi
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -40(%rsp)
	movzbl	%dh, %eax
	movq	%rax, -32(%rsp)
	movq	%rdi, -24(%rsp)
	movq	-40(%rsp), %rax
	movq	-32(%rsp), %rcx
	movq	-24(%rsp), %rdx
	addq	%rcx, %rax
	addq	%rdx, %rax
	movq	%rax, -16(%rsp)
	movq	-16(%rsp), %rax
	movq	%rax, -8(%rsp)
	movq	-16(%rsp), %rax
	cmpq	$110, %rax
	movq	-16(%rsp), %rax
	jbe	.L415
	cmpq	$220, %rax
	movq	-16(%rsp), %rax
	jbe	.L416
	cmpq	$330, %rax
	movq	-16(%rsp), %rax
	jbe	.L417
	cmpq	$440, %rax
	movq	-16(%rsp), %rax
	jbe	.L418
	cmpq	$550, %rax
	movq	-16(%rsp), %rax
	jbe	.L419
	cmpq	$660, %rax
	movq	-16(%rsp), %rax
	jbe	.L420
	leaq	-2640(,%rax,4), %rax
	movabsq	$-6148914691236517205, %rcx
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	movl	$220, %eax
	shrq	%rdx
	subq	%rdx, %rax
	movq	%rax, -40(%rsp)
	movq	-8(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	$2, %rdx
	movq	%rdx, -32(%rsp)
	movq	-8(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	$2, %rdx
	movq	%rdx, -24(%rsp)
	jmp	.L409
	.p2align 4,,10
.L416:
	leaq	-220(%rax,%rax), %rax
	movabsq	$-6148914691236517205, %rcx
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	movl	$73, %eax
	shrq	%rdx
	subq	%rdx, %rax
	movq	%rax, -40(%rsp)
	movq	$0, -32(%rsp)
	movq	-8(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	$146, %rdx
	movq	%rdx, -24(%rsp)
.L409:
	movq	-40(%rsp), %rax
	addq	$16, %rax
	movq	%rax, -40(%rsp)
	movq	-32(%rsp), %rax
	addq	$16, %rax
	movq	%rax, -32(%rsp)
	movq	-24(%rsp), %rax
	addq	$16, %rax
	movq	%rax, -24(%rsp)
	movq	-40(%rsp), %rdx
	movq	-32(%rsp), %rax
	movq	-24(%rsp), %rcx
	salq	$16, %rdx
	salq	$8, %rax
	addq	%rdx, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	ret
	.p2align 4,,10
.L415:
	addq	%rax, %rax
	movabsq	$-6148914691236517205, %rcx
	movq	%rax, -8(%rsp)
	movq	$73, -40(%rsp)
	movq	-8(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	movl	$73, %eax
	shrq	%rdx
	subq	%rdx, %rax
	movq	%rax, -32(%rsp)
	movq	-8(%rsp), %rdx
	movq	%rdx, %rax
	mulq	%rcx
	shrq	%rdx
	addq	$73, %rdx
	movq	%rdx, -24(%rsp)
	jmp	.L409
	.p2align 4,,10
.L418:
	subq	$330, %rax
	movq	%rax, -8(%rsp)
	movq	$0, -40(%rsp)
	movq	-8(%rsp), %rax
	addq	$110, %rax
	movq	%rax, -32(%rsp)
	movq	-8(%rsp), %rdx
	movl	$110, %eax
	subq	%rdx, %rax
	movq	%rax, -24(%rsp)
	jmp	.L409
	.p2align 4,,10
.L417:
	subq	$220, %rax
	movq	%rax, -8(%rsp)
	movq	$0, -40(%rsp)
	movq	-8(%rsp), %rax
	movq	%rax, -32(%rsp)
	movq	-8(%rsp), %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -24(%rsp)
	jmp	.L409
	.p2align 4,,10
.L419:
	subq	$440, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	movq	%rax, -40(%rsp)
	movq	-8(%rsp), %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -32(%rsp)
	movq	$0, -24(%rsp)
	jmp	.L409
	.p2align 4,,10
.L420:
	subq	$550, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	addq	$110, %rax
	movq	%rax, -40(%rsp)
	movq	-8(%rsp), %rdx
	movl	$110, %eax
	subq	%rdx, %rax
	movq	%rax, -32(%rsp)
	movq	$0, -24(%rsp)
	jmp	.L409
	.cfi_endproc
.LFE31:
	.size	color_heati2, .-color_heati2
	.p2align 4,,15
	.globl	color_heati
	.type	color_heati, @function
color_heati:
.LFB32:
	.cfi_startproc
	movq	%rdi, %rax
	movq	%rdi, %rdx
	movzbl	%dil, %edi
	andl	$16711680, %eax
	shrq	$16, %rax
	movq	%rax, -40(%rsp)
	movzbl	%dh, %eax
	movq	%rax, -32(%rsp)
	movq	%rdi, -24(%rsp)
	movq	-40(%rsp), %rax
	movq	-32(%rsp), %rcx
	movq	-24(%rsp), %rdx
	addq	%rcx, %rax
	addq	%rdx, %rax
	movq	%rax, -16(%rsp)
	movq	-16(%rsp), %rax
	movq	%rax, -8(%rsp)
	movq	-16(%rsp), %rax
	cmpq	$110, %rax
	movq	-16(%rsp), %rax
	jbe	.L429
	cmpq	$220, %rax
	jbe	.L430
	movq	-16(%rsp), %rax
	cmpq	$330, %rax
	movq	-16(%rsp), %rax
	jbe	.L431
	cmpq	$440, %rax
	movq	-16(%rsp), %rax
	jbe	.L432
	cmpq	$550, %rax
	movq	-16(%rsp), %rax
	jbe	.L433
	cmpq	$660, %rax
	movq	-16(%rsp), %rax
	jbe	.L434
	subq	$660, %rax
	movq	%rax, -8(%rsp)
	movq	$220, -40(%rsp)
	movq	-8(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -32(%rsp)
	movq	-8(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -24(%rsp)
	jmp	.L423
	.p2align 4,,10
.L430:
	movq	-8(%rsp), %rdx
	movl	$220, %eax
	subq	%rdx, %rax
	movq	%rax, -40(%rsp)
	movq	$0, -32(%rsp)
	movq	$220, -24(%rsp)
.L423:
	movq	-40(%rsp), %rdx
	movq	-32(%rsp), %rax
	movq	-24(%rsp), %rcx
	salq	$16, %rdx
	salq	$8, %rax
	addq	%rdx, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	ret
	.p2align 4,,10
.L429:
	movq	%rax, -40(%rsp)
	movq	$0, -32(%rsp)
	movq	-16(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -24(%rsp)
	jmp	.L423
	.p2align 4,,10
.L432:
	subq	$330, %rax
	movq	%rax, -8(%rsp)
	movq	$0, -40(%rsp)
	movl	$220, %eax
	movq	$220, -32(%rsp)
	movq	-8(%rsp), %rdx
	addq	%rdx, %rdx
	subq	%rdx, %rax
	movq	%rax, -24(%rsp)
	jmp	.L423
	.p2align 4,,10
.L431:
	subq	$220, %rax
	movq	%rax, -8(%rsp)
	movq	$0, -40(%rsp)
	movq	-8(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -32(%rsp)
	movq	$220, -24(%rsp)
	jmp	.L423
	.p2align 4,,10
.L433:
	subq	$440, %rax
	movq	%rax, -8(%rsp)
	movq	-8(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -40(%rsp)
	movq	$220, -32(%rsp)
	movq	$0, -24(%rsp)
	jmp	.L423
	.p2align 4,,10
.L434:
	subq	$550, %rax
	movq	%rax, -8(%rsp)
	movq	$220, -40(%rsp)
	movl	$220, %eax
	movq	-8(%rsp), %rdx
	addq	%rdx, %rdx
	subq	%rdx, %rax
	movq	%rax, -32(%rsp)
	movq	$0, -24(%rsp)
	jmp	.L423
	.cfi_endproc
.LFE32:
	.size	color_heati, .-color_heati
	.p2align 4,,15
	.globl	color_heat2
	.type	color_heat2, @function
color_heat2:
.LFB33:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L436
	cvttsd2siq	%xmm0, %rax
.L437:
	movq	%rax, %rdx
	movzbl	%ah, %ecx
	movzbl	%al, %eax
	andl	$16711680, %edx
	movsd	.LC22(%rip), %xmm2
	shrq	$16, %rdx
	addq	%rcx, %rdx
	addq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC21(%rip), %xmm0
	ucomisd	%xmm0, %xmm2
	ja	.L508
	ucomisd	%xmm2, %xmm0
	movsd	.LC25(%rip), %xmm2
	jae	.L489
.L447:
	ucomisd	%xmm2, %xmm0
	movsd	.LC26(%rip), %xmm2
	jb	.L454
	ucomisd	%xmm0, %xmm2
	ja	.L509
.L454:
	ucomisd	%xmm2, %xmm0
	movsd	.LC27(%rip), %xmm2
	jb	.L461
	ucomisd	%xmm0, %xmm2
	jbe	.L461
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC2(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	movsd	%xmm2, 16(%rsp)
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	call	cos@PLT
	movsd	16(%rsp), %xmm2
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L464
	cvttsd2siq	%xmm0, %rax
.L465:
	addq	$65280, %rax
	jns	.L507
	.p2align 4,,10
.L485:
	movq	%rax, %rdx
	andl	$1, %eax
	shrq	%rdx
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L446
	.p2align 4,,10
.L436:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L437
	.p2align 4,,10
.L489:
	ucomisd	%xmm0, %xmm2
	jbe	.L447
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC4(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	subsd	%xmm0, %xmm2
	movsd	.LC24(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L450
	cvttsd2siq	%xmm0, %rax
.L451:
	salq	$16, %rax
	addq	$255, %rax
	jns	.L507
	jmp	.L485
	.p2align 4,,10
.L461:
	ucomisd	%xmm2, %xmm0
	movsd	.LC28(%rip), %xmm2
	jb	.L468
	ucomisd	%xmm0, %xmm2
	jbe	.L468
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC16(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L471
	cvttsd2siq	%xmm0, %rax
.L472:
	salq	$16, %rax
	addq	$65280, %rax
	jns	.L507
.L480:
	shrq	%rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L446
	.p2align 4,,10
.L468:
	ucomisd	%xmm2, %xmm0
	movsd	.LC29(%rip), %xmm2
	jb	.L475
	ucomisd	%xmm0, %xmm2
	jbe	.L475
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC17(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	movsd	%xmm2, 16(%rsp)
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	call	cos@PLT
	movsd	16(%rsp), %xmm2
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L478
	cvttsd2siq	%xmm0, %rax
.L479:
	salq	$8, %rax
	addq	$16711680, %rax
	jns	.L507
	jmp	.L480
	.p2align 4,,10
.L475:
	ucomisd	%xmm2, %xmm0
	jb	.L501
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC11(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L483
	cvttsd2siq	%xmm0, %rax
.L484:
	movq	%rax, %rdx
	salq	$8, %rdx
	leaq	16711680(%rax,%rdx), %rax
	testq	%rax, %rax
	jns	.L507
	jmp	.L485
	.p2align 4,,10
.L508:
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC24(%rip), %xmm0
	mulsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	%xmm2, 16(%rsp)
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	16(%rsp), %xmm2
	subsd	%xmm0, %xmm3
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC5(%rip), %xmm3
	movsd	%xmm3, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	movsd	16(%rsp), %xmm3
	mulsd	%xmm2, %xmm0
	movsd	.LC1(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L440
	cvttsd2siq	%xmm0, %rax
	movapd	%xmm2, %xmm0
	mulsd	%xmm3, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L442
.L510:
	cvttsd2siq	%xmm0, %rdx
.L443:
	salq	$16, %rdx
	addq	%rdx, %rax
	js	.L444
.L507:
	cvtsi2sdq	%rax, %xmm0
.L446:
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L440:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	movapd	%xmm2, %xmm0
	mulsd	%xmm3, %xmm0
	xorq	%rdx, %rax
	ucomisd	%xmm1, %xmm0
	jb	.L510
.L442:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rdx
	xorq	%rcx, %rdx
	jmp	.L443
	.p2align 4,,10
.L509:
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC13(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L457
	cvttsd2siq	%xmm0, %rax
.L458:
	salq	$8, %rax
	addq	$255, %rax
	jns	.L507
	jmp	.L485
	.p2align 4,,10
.L501:
	movsd	.LC20(%rip), %xmm0
	jmp	.L446
	.p2align 4,,10
.L444:
	movq	%rax, %rcx
	andl	$1, %eax
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L446
	.p2align 4,,10
.L483:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L484
	.p2align 4,,10
.L450:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L451
	.p2align 4,,10
.L457:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L458
	.p2align 4,,10
.L464:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L465
	.p2align 4,,10
.L471:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L472
	.p2align 4,,10
.L478:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L479
	.cfi_endproc
.LFE33:
	.size	color_heat2, .-color_heat2
	.p2align 4,,15
	.globl	color_heat
	.type	color_heat, @function
color_heat:
.LFB34:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L512
	cvttsd2siq	%xmm0, %rax
.L513:
	movq	%rax, %rdx
	movzbl	%ah, %ecx
	movzbl	%al, %eax
	andl	$16711680, %edx
	movsd	.LC22(%rip), %xmm2
	shrq	$16, %rdx
	addq	%rcx, %rdx
	addq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC21(%rip), %xmm0
	ucomisd	%xmm0, %xmm2
	ja	.L584
	ucomisd	%xmm2, %xmm0
	movsd	.LC25(%rip), %xmm2
	jae	.L565
.L523:
	ucomisd	%xmm2, %xmm0
	movsd	.LC26(%rip), %xmm2
	jb	.L530
	ucomisd	%xmm0, %xmm2
	ja	.L585
.L530:
	ucomisd	%xmm2, %xmm0
	movsd	.LC27(%rip), %xmm2
	jb	.L537
	ucomisd	%xmm0, %xmm2
	jbe	.L537
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC2(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	movsd	%xmm2, 16(%rsp)
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	call	cos@PLT
	movsd	16(%rsp), %xmm2
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L540
	cvttsd2siq	%xmm0, %rax
.L541:
	addq	$65280, %rax
	jns	.L583
	.p2align 4,,10
.L561:
	movq	%rax, %rdx
	andl	$1, %eax
	shrq	%rdx
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L522
	.p2align 4,,10
.L512:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L513
	.p2align 4,,10
.L565:
	ucomisd	%xmm0, %xmm2
	jbe	.L523
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC4(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	subsd	%xmm0, %xmm2
	movsd	.LC24(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L526
	cvttsd2siq	%xmm0, %rax
.L527:
	salq	$16, %rax
	addq	$255, %rax
	jns	.L583
	jmp	.L561
	.p2align 4,,10
.L537:
	ucomisd	%xmm2, %xmm0
	movsd	.LC28(%rip), %xmm2
	jb	.L544
	ucomisd	%xmm0, %xmm2
	jbe	.L544
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC16(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L547
	cvttsd2siq	%xmm0, %rax
.L548:
	salq	$16, %rax
	addq	$65280, %rax
	jns	.L583
.L556:
	shrq	%rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L522
	.p2align 4,,10
.L544:
	ucomisd	%xmm2, %xmm0
	movsd	.LC29(%rip), %xmm2
	jb	.L551
	ucomisd	%xmm0, %xmm2
	jbe	.L551
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC17(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movapd	%xmm2, %xmm3
	movsd	%xmm2, 16(%rsp)
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	movapd	%xmm2, %xmm3
	subsd	%xmm0, %xmm3
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	call	cos@PLT
	movsd	16(%rsp), %xmm2
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L554
	cvttsd2siq	%xmm0, %rax
.L555:
	salq	$8, %rax
	addq	$16711680, %rax
	jns	.L583
	jmp	.L556
	.p2align 4,,10
.L551:
	ucomisd	%xmm2, %xmm0
	jb	.L577
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC11(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L559
	cvttsd2siq	%xmm0, %rax
.L560:
	movq	%rax, %rdx
	salq	$8, %rdx
	leaq	16711680(%rax,%rdx), %rax
	testq	%rax, %rax
	jns	.L583
	jmp	.L561
	.p2align 4,,10
.L584:
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC24(%rip), %xmm0
	mulsd	.LC5(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	movsd	%xmm2, 16(%rsp)
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	16(%rsp), %xmm2
	subsd	%xmm0, %xmm3
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC5(%rip), %xmm3
	movsd	%xmm3, 16(%rsp)
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	movsd	16(%rsp), %xmm3
	mulsd	%xmm2, %xmm0
	movsd	.LC1(%rip), %xmm2
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L516
	cvttsd2siq	%xmm0, %rax
	movapd	%xmm2, %xmm0
	mulsd	%xmm3, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L518
.L586:
	cvttsd2siq	%xmm0, %rdx
.L519:
	salq	$16, %rdx
	addq	%rdx, %rax
	js	.L520
.L583:
	cvtsi2sdq	%rax, %xmm0
.L522:
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L516:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	movapd	%xmm2, %xmm0
	mulsd	%xmm3, %xmm0
	xorq	%rdx, %rax
	ucomisd	%xmm1, %xmm0
	jb	.L586
.L518:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rdx
	xorq	%rcx, %rdx
	jmp	.L519
	.p2align 4,,10
.L585:
	mulsd	.LC23(%rip), %xmm0
	movsd	%xmm1, (%rsp)
	subsd	.LC13(%rip), %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm2
	subsd	%xmm0, %xmm2
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	cos@PLT
	movsd	.LC4(%rip), %xmm3
	movsd	(%rsp), %xmm1
	subsd	%xmm0, %xmm3
	movsd	.LC5(%rip), %xmm0
	mulsd	%xmm3, %xmm0
	mulsd	.LC1(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L533
	cvttsd2siq	%xmm0, %rax
.L534:
	salq	$8, %rax
	addq	$255, %rax
	jns	.L583
	jmp	.L561
	.p2align 4,,10
.L577:
	movsd	.LC20(%rip), %xmm0
	jmp	.L522
	.p2align 4,,10
.L520:
	movq	%rax, %rcx
	andl	$1, %eax
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L522
	.p2align 4,,10
.L559:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L560
	.p2align 4,,10
.L526:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L527
	.p2align 4,,10
.L533:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L534
	.p2align 4,,10
.L540:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L541
	.p2align 4,,10
.L547:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L548
	.p2align 4,,10
.L554:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L555
	.cfi_endproc
.LFE34:
	.size	color_heat, .-color_heat
	.p2align 4,,15
	.globl	color_ghost
	.type	color_ghost, @function
color_ghost:
.LFB35:
	.cfi_startproc
	subq	$88, %rsp
	.cfi_def_cfa_offset 96
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L588
	cvttsd2siq	%xmm0, %rax
.L589:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm2
	andl	$16711680, %edx
	movsd	.LC4(%rip), %xmm5
	shrq	$16, %rdx
	movsd	%xmm1, 16(%rsp)
	cvtsi2sdq	%rdx, %xmm0
	movapd	%xmm5, %xmm3
	movzbl	%ah, %edx
	movzbl	%al, %eax
	movsd	%xmm2, (%rsp)
	divsd	%xmm2, %xmm0
	subsd	%xmm0, %xmm3
	cvtsi2sdq	%rdx, %xmm0
	movsd	%xmm3, 64(%rsp)
	movapd	%xmm5, %xmm3
	divsd	%xmm2, %xmm0
	subsd	%xmm0, %xmm3
	cvtsi2sdq	%rax, %xmm0
	movsd	%xmm3, 72(%rsp)
	divsd	%xmm2, %xmm0
	subsd	%xmm0, %xmm5
	movapd	%xmm5, %xmm0
	movsd	%xmm5, 48(%rsp)
	call	cbrt@PLT
	movsd	72(%rsp), %xmm4
	mulsd	%xmm0, %xmm4
	movsd	64(%rsp), %xmm0
	mulsd	72(%rsp), %xmm4
	movsd	%xmm4, 32(%rsp)
	call	cbrt@PLT
	movsd	48(%rsp), %xmm5
	movapd	%xmm0, %xmm3
	movsd	72(%rsp), %xmm0
	mulsd	%xmm5, %xmm3
	mulsd	%xmm5, %xmm3
	movsd	%xmm3, 48(%rsp)
	call	cbrt@PLT
	movsd	(%rsp), %xmm2
	movsd	32(%rsp), %xmm4
	movsd	16(%rsp), %xmm1
	mulsd	%xmm2, %xmm4
	movsd	48(%rsp), %xmm3
	ucomisd	%xmm1, %xmm4
	jae	.L590
	cvttsd2siq	%xmm4, %rdx
.L591:
	mulsd	%xmm2, %xmm3
	salq	$16, %rdx
	ucomisd	%xmm1, %xmm3
	jae	.L592
	cvttsd2siq	%xmm3, %rcx
.L593:
	mulsd	64(%rsp), %xmm0
	salq	$8, %rcx
	addq	%rcx, %rdx
	mulsd	64(%rsp), %xmm0
	mulsd	%xmm2, %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L594
	cvttsd2siq	%xmm0, %rax
	addq	%rdx, %rax
	js	.L596
.L599:
	cvtsi2sdq	%rax, %xmm0
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
.L588:
	.cfi_restore_state
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L589
	.p2align 4,,10
.L594:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L599
.L596:
	movq	%rax, %rcx
	andl	$1, %eax
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L592:
	.cfi_restore_state
	subsd	%xmm1, %xmm3
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm3, %rcx
	xorq	%rax, %rcx
	jmp	.L593
	.p2align 4,,10
.L590:
	subsd	%xmm1, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rdx
	xorq	%rax, %rdx
	jmp	.L591
	.cfi_endproc
.LFE35:
	.size	color_ghost, .-color_ghost
	.p2align 4,,15
	.globl	color_inv
	.type	color_inv, @function
color_inv:
.LFB36:
	.cfi_startproc
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L601
	cvttsd2siq	%xmm0, %rax
.L602:
	movq	%rax, %rdx
	movsd	.LC1(%rip), %xmm0
	andl	$16711680, %edx
	movsd	.LC4(%rip), %xmm2
	shrq	$16, %rdx
	cvtsi2sdq	%rdx, %xmm3
	movapd	%xmm2, %xmm4
	movzbl	%ah, %edx
	movapd	%xmm2, %xmm5
	movzbl	%al, %eax
	divsd	%xmm0, %xmm3
	subsd	%xmm3, %xmm4
	cvtsi2sdq	%rdx, %xmm3
	mulsd	%xmm0, %xmm4
	ucomisd	%xmm1, %xmm4
	divsd	%xmm0, %xmm3
	subsd	%xmm3, %xmm5
	movapd	%xmm5, %xmm3
	cvtsi2sdq	%rax, %xmm5
	mulsd	%xmm0, %xmm3
	divsd	%xmm0, %xmm5
	subsd	%xmm5, %xmm2
	mulsd	%xmm0, %xmm2
	jae	.L603
	cvttsd2siq	%xmm4, %rdx
.L604:
	salq	$16, %rdx
	ucomisd	%xmm1, %xmm3
	jae	.L605
	cvttsd2siq	%xmm3, %rcx
.L606:
	salq	$8, %rcx
	addq	%rcx, %rdx
	ucomisd	%xmm1, %xmm2
	jae	.L607
	cvttsd2siq	%xmm2, %rax
	addq	%rdx, %rax
	js	.L609
.L611:
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L601:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rdx
	cvttsd2siq	%xmm0, %rax
	xorq	%rdx, %rax
	jmp	.L602
	.p2align 4,,10
.L607:
	subsd	%xmm1, %xmm2
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm2, %rax
	xorq	%rcx, %rax
	addq	%rdx, %rax
	jns	.L611
.L609:
	movq	%rax, %rcx
	andl	$1, %eax
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L605:
	subsd	%xmm1, %xmm3
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm3, %rcx
	xorq	%rax, %rcx
	jmp	.L606
	.p2align 4,,10
.L603:
	subsd	%xmm1, %xmm4
	movabsq	$-9223372036854775808, %rax
	cvttsd2siq	%xmm4, %rdx
	xorq	%rax, %rdx
	jmp	.L604
	.cfi_endproc
.LFE36:
	.size	color_inv, .-color_inv
	.p2align 4,,15
	.globl	color_box
	.type	color_box, @function
color_box:
.LFB37:
	.cfi_startproc
	movsd	.LC30(%rip), %xmm2
	movapd	%xmm0, %xmm3
	subsd	%xmm1, %xmm2
	movsd	.LC31(%rip), %xmm1
	mulsd	%xmm1, %xmm3
	mulsd	%xmm2, %xmm1
	cvttsd2si	%xmm3, %eax
	cvttsd2si	%xmm1, %ecx
	movsd	.LC0(%rip), %xmm1
	ucomisd	%xmm1, %xmm0
	jae	.L613
	cvttsd2siq	%xmm0, %rdx
	ucomisd	%xmm1, %xmm2
	movzbl	%dl, %esi
	jae	.L615
.L662:
	cvttsd2siq	%xmm2, %rdx
	testl	%ecx, %ecx
	movzbl	%dl, %edx
	jne	.L617
.L663:
	cmpl	$1, %eax
	je	.L659
	testl	%eax, %eax
	je	.L660
	cmpl	$2, %eax
	.p2align 4,,3
	je	.L661
	cmpl	$3, %eax
	xorpd	%xmm0, %xmm0
	jne	.L619
	leaq	(%rdx,%rsi), %rax
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L630
	cvttsd2siq	%xmm0, %rax
.L631:
	salq	$8, %rsi
	salq	$16, %rax
	addq	%rsi, %rdx
	addq	%rax, %rdx
	jns	.L658
.L632:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L613:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rsi
	cvttsd2siq	%xmm0, %rdx
	xorq	%rsi, %rdx
	ucomisd	%xmm1, %xmm2
	movzbl	%dl, %esi
	jb	.L662
.L615:
	subsd	%xmm1, %xmm2
	movabsq	$-9223372036854775808, %rdi
	cvttsd2siq	%xmm2, %rdx
	xorq	%rdi, %rdx
	testl	%ecx, %ecx
	movzbl	%dl, %edx
	je	.L663
.L617:
	cmpl	$1, %ecx
	je	.L664
	cmpl	$2, %ecx
	xorpd	%xmm0, %xmm0
	je	.L665
.L619:
	rep; ret
	.p2align 4,,10
.L664:
	testl	%eax, %eax
	je	.L666
	cmpl	$1, %eax
	.p2align 4,,6
	je	.L667
	cmpl	$2, %eax
	.p2align 4,,3
	je	.L668
	cmpl	$3, %eax
	xorpd	%xmm0, %xmm0
	jne	.L619
	movl	$255, %eax
	salq	$8, %rdx
	subq	%rsi, %rax
	salq	$16, %rax
	leaq	255(%rax,%rdx), %rax
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L665:
	cmpl	$1, %eax
	je	.L669
	testl	%eax, %eax
	je	.L670
	cmpl	$2, %eax
	.p2align 4,,3
	je	.L671
	cmpl	$3, %eax
	.p2align 4,,3
	jne	.L619
	leaq	(%rdx,%rsi), %rax
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L649
	cvttsd2siq	%xmm0, %rax
.L650:
	salq	$8, %rdx
	salq	$16, %rax
	addq	%rdx, %rsi
	addq	%rax, %rsi
	js	.L651
	cvtsi2sdq	%rsi, %xmm0
	ret
	.p2align 4,,10
.L659:
	salq	$16, %rsi
	addq	$255, %rsi
	subq	%rdx, %rsi
	cvtsi2sdq	%rsi, %xmm0
	ret
	.p2align 4,,10
.L660:
	leaq	(%rdx,%rsi), %rax
	salq	$16, %rsi
	salq	$8, %rdx
	addq	%rsi, %rdx
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L621
	cvttsd2siq	%xmm0, %rax
.L622:
	addq	%rdx, %rax
	js	.L623
.L657:
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L621:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	jmp	.L622
	.p2align 4,,10
.L666:
	salq	$8, %rdx
	subq	%rsi, %rdx
	leaq	255(%rdx), %rax
	jmp	.L657
	.p2align 4,,10
.L661:
	leaq	(%rdx,%rsi), %rax
	salq	$16, %rsi
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L626
	cvttsd2siq	%xmm0, %rax
.L627:
	salq	$8, %rax
	addq	%rsi, %rdx
	addq	%rax, %rdx
	js	.L632
.L658:
	cvtsi2sdq	%rdx, %xmm0
	ret
	.p2align 4,,10
.L630:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	jmp	.L631
	.p2align 4,,10
.L670:
	leaq	(%rdx,%rsi), %rax
	salq	$16, %rdx
	salq	$8, %rsi
	addq	%rsi, %rdx
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L640
	cvttsd2siq	%xmm0, %rax
.L641:
	addq	%rdx, %rax
	jns	.L657
.L647:
	movq	%rax, %rdx
	andl	$1, %eax
	shrq	%rdx
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L623:
	movq	%rax, %rcx
	andl	$1, %eax
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.p2align 4,,10
.L669:
	salq	$16, %rsi
	leaq	65280(%rdx,%rsi), %rax
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L626:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	jmp	.L627
	.p2align 4,,10
.L640:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	jmp	.L641
	.p2align 4,,10
.L667:
	salq	$16, %rsi
	salq	$8, %rdx
	addq	%rsi, %rdx
	jmp	.L658
	.p2align 4,,10
.L668:
	salq	$8, %rdx
	leaq	16711680(%rsi,%rdx), %rax
	cvtsi2sdq	%rax, %xmm0
	ret
	.p2align 4,,10
.L671:
	leaq	(%rdx,%rsi), %rax
	salq	$16, %rdx
	cvtsi2sdq	%rax, %xmm0
	mulsd	.LC5(%rip), %xmm0
	ucomisd	%xmm1, %xmm0
	jae	.L645
	cvttsd2siq	%xmm0, %rax
.L646:
	salq	$8, %rax
	addq	%rdx, %rsi
	addq	%rsi, %rax
	jns	.L657
	jmp	.L647
	.p2align 4,,10
.L649:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	jmp	.L650
.L645:
	subsd	%xmm1, %xmm0
	movabsq	$-9223372036854775808, %rcx
	cvttsd2siq	%xmm0, %rax
	xorq	%rcx, %rax
	jmp	.L646
.L651:
	movq	%rsi, %rax
	andl	$1, %esi
	shrq	%rax
	orq	%rsi, %rax
	cvtsi2sdq	%rax, %xmm0
	addsd	%xmm0, %xmm0
	ret
	.cfi_endproc
.LFE37:
	.size	color_box, .-color_box
	.p2align 4,,15
	.globl	udf3
	.type	udf3, @function
udf3:
.LFB39:
	.cfi_startproc
	jmp	alien_gs3@PLT
	.cfi_endproc
.LFE39:
	.size	udf3, .-udf3
	.p2align 4,,15
	.globl	udf2
	.type	udf2, @function
udf2:
.LFB40:
	.cfi_startproc
	jmp	color_box@PLT
	.cfi_endproc
.LFE40:
	.size	udf2, .-udf2
	.p2align 4,,15
	.globl	min_udf2
	.type	min_udf2, @function
min_udf2:
.LFB38:
	.cfi_startproc
	movapd	%xmm1, %xmm3
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movsd	%xmm2, (%rsp)
	movsd	%xmm3, 16(%rsp)
	call	udf2@PLT
	movsd	16(%rsp), %xmm3
	movsd	(%rsp), %xmm2
	movapd	%xmm0, %xmm4
	movapd	%xmm3, %xmm0
	movapd	%xmm2, %xmm1
	movsd	%xmm4, 16(%rsp)
	call	udf2@PLT
	movsd	16(%rsp), %xmm4
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	minsd	%xmm0, %xmm4
	movapd	%xmm4, %xmm0
	ret
	.cfi_endproc
.LFE38:
	.size	min_udf2, .-min_udf2
	.p2align 4,,15
	.globl	udf1
	.type	udf1, @function
udf1:
.LFB41:
	.cfi_startproc
	jmp	alien_gs@PLT
	.cfi_endproc
.LFE41:
	.size	udf1, .-udf1
	.p2align 4,,15
	.globl	udfi1
	.type	udfi1, @function
udfi1:
.LFB42:
	.cfi_startproc
	jmp	alien_gsi@PLT
	.cfi_endproc
.LFE42:
	.size	udfi1, .-udfi1
	.p2align 4,,15
	.globl	udfi2
	.type	udfi2, @function
udfi2:
.LFB43:
	.cfi_startproc
	jmp	edge_detect_comix@PLT
	.cfi_endproc
.LFE43:
	.size	udfi2, .-udfi2
	.p2align 4,,15
	.globl	udfi3
	.type	udfi3, @function
udfi3:
.LFB44:
	.cfi_startproc
	jmp	color_invi3@PLT
	.cfi_endproc
.LFE44:
	.size	udfi3, .-udfi3
	.p2align 4,,15
	.globl	udfi6
	.type	udfi6, @function
udfi6:
.LFB45:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, 8(%rsp)
	leaq	16(%rsp), %rdi
	movq	8(%rsp), %xmm1
	movq	%rsi, 8(%rsp)
	movq	8(%rsp), %xmm2
	movq	%rdx, 8(%rsp)
	punpcklqdq	%xmm2, %xmm1
	movdqa	%xmm1, 16(%rsp)
	movq	8(%rsp), %xmm1
	movq	%rcx, 8(%rsp)
	movq	8(%rsp), %xmm2
	movq	%r8, 8(%rsp)
	punpcklqdq	%xmm2, %xmm1
	movdqa	%xmm1, 32(%rsp)
	movq	8(%rsp), %xmm1
	movq	%r9, 8(%rsp)
	movq	8(%rsp), %xmm2
	punpcklqdq	%xmm2, %xmm1
	movdqa	%xmm1, 48(%rsp)
	call	alien_gsi6@PLT
	addq	$72, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE45:
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
	.ident	"GCC: (FreeBSD Ports Collection) 4.8.0 20120805 (experimental)"
	.section	.note.GNU-stack,"",@progbits
