	.file	"hwc.c"
	.comm	gops,4,4
	.comm	ft,4,4
	.comm	av,4,4
	.section	.rodata
.LC2:
	.string	"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%12.6f GOPS"
	.text
	.globl	catch_int
	.type	catch_int, @function
catch_int:
.LFB1:
	.cfi_startproc
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	movl	ft, %eax
	testl	%eax, %eax
	je	.L2
	movl	__stdoutp, %eax
	movl	%eax, (%esp)
	call	fflush
	movl	$0, ft
	movl	$0, %edi
	movl	$0, av
	jmp	.L1
.L2:
	movl	av, %eax
	addl	$1, %eax
	movl	%eax, av
	flds	gops
	movl	%edi, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	movl	$0, %edx
	movl	%eax, 16(%esp)
	movl	%edx, 20(%esp)
	fildq	16(%esp)
	fldl	.LC0
	fdivrp	%st, %st(1)
	faddp	%st, %st(1)
	fstps	28(%esp)
	flds	28(%esp)
	fstps	gops
	movl	$0, %edi
	movl	av, %eax
	cmpl	$25, %eax
	jne	.L1
	flds	gops
	flds	.LC1
	fdivrp	%st, %st(1)
	fstps	gops
	flds	gops
	fstpl	4(%esp)
	movl	$.LC2, (%esp)
	call	printf
	movl	__stdoutp, %eax
	movl	%eax, (%esp)
	call	fflush
	movl	$0, av
	movl	$0x00000000, %eax
	movl	%eax, gops
.L1:
	addl	$44, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE1:
	.size	catch_int, .-catch_int
	.globl	signal_setup
	.type	signal_setup, @function
signal_setup:
.LFB2:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	$catch_int, act.1838
	movl	$act.1838+8, (%esp)
	call	sigfillset
	movl	$0, 8(%esp)
	movl	$act.1838, 4(%esp)
	movl	$14, (%esp)
	call	sigaction
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE2:
	.size	signal_setup, .-signal_setup
	.globl	hwc
	.type	hwc, @function
hwc:
.LFB3:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	call	signal_setup
	movl	$1, ft
	movl	$0, %edi
	movl	$0, av
	movl	$200000, 4(%esp)
	movl	$200000, (%esp)
	call	ualarm
.L6:
	incl	%edi
	jmp	.L6
	.cfi_endproc
.LFE3:
	.size	hwc, .-hwc
	.globl	main
	.type	main, @function
main:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	call	hwc
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	main, .-main
	.local	act.1838
	.comm	act.1838,24,4
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1104006501
	.align 4
.LC1:
	.long	1103626240
	.ident	"GCC: (FreeBSD Ports Collection) 4.6.3 20111209 (prerelease)"
