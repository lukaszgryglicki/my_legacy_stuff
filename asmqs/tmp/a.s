	.file	"a.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.p2align 2,0x90
.globl c__FPPc
		.type		 c__FPPc,@function
c__FPPc:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%eax
	movl $0,(%eax)
	jmp .L3
	jmp .L2
	.p2align 2,0x90
.L3:
.L2:
	leave
	ret
.Lfe1:
		.size		 c__FPPc,.Lfe1-c__FPPc
	.section	.rodata
.LC0:
	.byte	 0x61,0x62,0x62,0x61,0x0
.LC1:
	.byte	 0x63,0x61,0x6c,0x6c,0x0
.LC2:
	.byte	 0x62,0x6f,0x69,0x6c,0x0
.text
	.p2align 2,0x90
.globl main
		.type		 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $.LC0,-12(%ebp)
	movl $.LC1,-8(%ebp)
	movl $.LC2,-4(%ebp)
	addl $-12,%esp
	leal -12(%ebp),%eax
	pushl %eax
	call c__FPPc
	addl $16,%esp
	xorl %eax,%eax
	jmp .L4
	.p2align 2,0x90
.L4:
	leave
	ret
.Lfe2:
		.size		 main,.Lfe2-main
	.ident	"GCC: (GNU) cplusplus 2.95.3 20010315 (release) [FreeBSD]"
