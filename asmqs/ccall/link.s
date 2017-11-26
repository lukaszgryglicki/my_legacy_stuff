	.file	"link.c"
	.version	"01.01"
gcc2_compiled.:
.data
	.p2align 2
	.type	 _rval,@object
	.size	 _rval,4
_rval:
	.long 1
	.p2align 2
	.type	 r_type,@object
	.size	 r_type,4
r_type:
	.long 0
	.p2align 2
	.type	 __SLOW_RND,@object
	.size	 __SLOW_RND,4
__SLOW_RND:
	.long 0
	.p2align 2
	.type	 __URAN,@object
	.size	 __URAN,4
__URAN:
	.long 1
	.section	.rodata
.LC0:
	.byte	 0x72,0x0
	.p2align 5
.LC1:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x43,0x61,0x6e,0x6e,0x6f,0x74
	.byte	 0x20,0x6f,0x70,0x65,0x6e,0x20,0x64,0x65,0x76,0x69
	.byte	 0x63,0x65,0x3a,0x20,0x25,0x73,0xa,0x0
.text
	.p2align 2,0x90
.globl reopen__Fv
		.type		 reopen__Fv,@function
reopen__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $0,_ran_device
	jne .L8
	jmp .L7
	.p2align 2,0x90
.L8:
	addl $-12,%esp
	movl _ran_device,%eax
	pushl %eax
	call fclose
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC0
	pushl $_ran_dev_name
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,_ran_device
	cmpl $0,_ran_device
	jne .L9
	addl $-8,%esp
	pushl $_ran_dev_name
	pushl $.LC1
	call printf
	addl $16,%esp
.L9:
	jmp .L10
	jmp .L7
	.p2align 2,0x90
.L10:
.L7:
	leave
	ret
.Lfe1:
		.size		 reopen__Fv,.Lfe1-reopen__Fv
	.section	.rodata
	.p2align 5
.LC2:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x49,0x6e,0x76,0x61,0x6c,0x69
	.byte	 0x64,0x20,0x66,0x6c,0x61,0x67,0x20,0x6f,0x70,0x65
	.byte	 0x72,0x61,0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f,0x72
	.byte	 0x20,0x2f,0x64,0x65,0x76,0x2f,0x75,0x72,0x61,0x6e
	.byte	 0x64,0x6f,0x6d,0x2e,0xa,0x54,0x72,0x69,0x65,0x64
	.byte	 0x20,0x74,0x6f,0x20,0x75,0x73,0x65,0x20,0x6f,0x70
	.byte	 0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f,0x72,0x20,0x43
	.byte	 0x5f,0x54,0x4d,0x50,0x20,0x77,0x69,0x74,0x68,0x20
	.byte	 0x2f,0x64,0x65,0x76,0x2f,0x72,0x61,0x6e,0x64,0x6f
	.byte	 0x6d,0xa,0x0
.text
	.p2align 2,0x90
.globl flag_ctmp__Fv
		.type		 flag_ctmp__Fv,@function
flag_ctmp__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $1,r_type
	jne .L12
	cmpl $0,__SLOW_RND
	je .L13
	movl $0,__SLOW_RND
	jmp .L14
	.p2align 2,0x90
.L13:
	movl $1,__SLOW_RND
.L14:
	jmp .L11
	.p2align 2,0x90
.L12:
	addl $-12,%esp
	pushl $.LC2
	call printf
	addl $16,%esp
	jmp .L15
	jmp .L11
	.p2align 2,0x90
.L15:
.L11:
	leave
	ret
.Lfe2:
		.size		 flag_ctmp__Fv,.Lfe2-flag_ctmp__Fv
	.section	.rodata
.LC3:
	.byte	 0x2f,0x64,0x65,0x76,0x2f,0x72,0x61,0x6e,0x64,0x6f
	.byte	 0x6d,0x0
.LC4:
	.byte	 0x2f,0x64,0x65,0x76,0x2f,0x75,0x72,0x61,0x6e,0x64
	.byte	 0x6f,0x6d,0x0
	.p2align 5
.LC5:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x49,0x6e,0x76,0x61,0x6c,0x69
	.byte	 0x64,0x20,0x66,0x6c,0x61,0x67,0x20,0x6f,0x70,0x65
	.byte	 0x72,0x61,0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f,0x72
	.byte	 0x20,0x43,0x5f,0x54,0x4d,0x50,0x2e,0xa,0x54,0x72
	.byte	 0x69,0x65,0x64,0x20,0x74,0x6f,0x20,0x75,0x73,0x65
	.byte	 0x20,0x6f,0x70,0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f
	.byte	 0x72,0x20,0x2f,0x64,0x65,0x76,0x2f,0x72,0x61,0x6e
	.byte	 0x64,0x6f,0x6d,0x20,0x77,0x69,0x74,0x68,0x20,0x43
	.byte	 0x5f,0x54,0x4d,0x50,0xa,0x0
.text
	.p2align 2,0x90
.globl flag_uran__Fv
		.type		 flag_uran__Fv,@function
flag_uran__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $0,r_type
	jne .L17
	cmpl $0,__URAN
	je .L18
	movl $0,__URAN
	addl $-8,%esp
	pushl $.LC3
	pushl $_ran_dev_name
	call strcpy
	addl $16,%esp
	call reopen__Fv
	jmp .L16
	.p2align 2,0x90
.L18:
	cmpl $0,__URAN
	jne .L17
	movl $1,__URAN
	addl $-8,%esp
	pushl $.LC4
	pushl $_ran_dev_name
	call strcpy
	addl $16,%esp
	call reopen__Fv
	jmp .L16
	.p2align 2,0x90
.L19:
.L17:
	addl $-12,%esp
	pushl $.LC5
	call printf
	addl $16,%esp
	jmp .L20
	jmp .L16
	.p2align 2,0x90
.L20:
.L16:
	leave
	ret
.Lfe3:
		.size		 flag_uran__Fv,.Lfe3-flag_uran__Fv
	.section	.rodata
	.p2align 5
.LC6:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x55,0x73,0x69,0x6e,0x67,0x20
	.byte	 0x75,0x6e,0x6b,0x6e,0x6f,0x77,0x6e,0x20,0x72,0x61
	.byte	 0x6e,0x64,0x6f,0x6d,0x20,0x74,0x79,0x70,0x65,0x2e
	.byte	 0xa,0x0
.text
	.p2align 2,0x90
.globl type_ran__Fv
		.type		 type_ran__Fv,@function
type_ran__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $1,r_type
	jne .L22
	cmpl $0,__SLOW_RND
	je .L22
	movl $273,%eax
	jmp .L21
	jmp .L23
	.p2align 2,0x90
.L22:
	cmpl $1,r_type
	jne .L24
	cmpl $0,__SLOW_RND
	jne .L24
	movl $274,%eax
	jmp .L21
	jmp .L23
	.p2align 2,0x90
.L24:
	cmpl $0,r_type
	jne .L26
	cmpl $0,__URAN
	je .L26
	movl $275,%eax
	jmp .L21
	jmp .L23
	.p2align 2,0x90
.L26:
	cmpl $0,r_type
	jne .L28
	cmpl $0,__URAN
	jne .L28
	movl $276,%eax
	jmp .L21
	jmp .L23
	.p2align 2,0x90
.L28:
	addl $-12,%esp
	pushl $.LC6
	call printf
	addl $16,%esp
	xorl %eax,%eax
	jmp .L21
	.p2align 2,0x90
.L29:
.L27:
.L25:
.L23:
	jmp .L30
	jmp .L21
	.p2align 2,0x90
.L30:
.L21:
	leave
	ret
.Lfe4:
		.size		 type_ran__Fv,.Lfe4-type_ran__Fv
	.section	.rodata
	.p2align 5
.LC7:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x43,0x61,0x6e,0x6e,0x6f,0x74
	.byte	 0x20,0x6f,0x70,0x65,0x6e,0x20,0x64,0x65,0x76,0x69
	.byte	 0x63,0x65,0x3a,0x20,0x25,0x73,0x0
.LC8:
	.byte	 0x2f,0x63,0x74,0x6d,0x70,0x2f,0x5f,0x5f,0x72,0x61
	.byte	 0x6e,0x64,0x6f,0x6d,0x0
.LC9:
	.byte	 0x77,0x0
	.p2align 5
.LC10:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x46,0x61,0x74,0x61,0x6c,0x20
	.byte	 0x45,0x72,0x72,0x6f,0x72,0xa,0x43,0x61,0x6e,0x6e
	.byte	 0x6f,0x74,0x20,0x63,0x72,0x65,0x61,0x74,0x65,0x20
	.byte	 0x25,0x73,0xa,0x0
.LC11:
	.byte	 0x25,0x64,0x0
.text
	.p2align 2,0x90
.globl init_random__Fi
		.type		 init_random__Fi,@function
init_random__Fi:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl 8(%ebp),%eax
	movl %eax,r_type
	cmpl $0,r_type
	jne .L32
	addl $-8,%esp
	pushl $.LC4
	pushl $_ran_dev_name
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC0
	pushl $_ran_dev_name
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,_ran_device
	cmpl $0,_ran_device
	jne .L33
	addl $-8,%esp
	pushl $_ran_dev_name
	pushl $.LC7
	call printf
	addl $16,%esp
	movb $0,%al
	jmp .L31
	.p2align 2,0x90
.L33:
	movb $1,%al
	jmp .L31
	jmp .L34
	.p2align 2,0x90
.L32:
	cmpl $1,r_type
	jne .L34
	movl $.LC8,-8(%ebp)
	addl $-8,%esp
	pushl $.LC0
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L36
	addl $-8,%esp
	pushl $.LC9
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L37
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC10
	call printf
	addl $16,%esp
	jmp .L38
	.p2align 2,0x90
.L37:
	addl $-4,%esp
	pushl $1
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L38:
	jmp .L39
	.p2align 2,0x90
.L36:
	addl $-4,%esp
	pushl $_rval
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fscanf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L39:
	movzwl _rval,%eax
	movw %ax,-10(%ebp)
	movl $0,-16(%ebp)
	.p2align 2,0x90
.L40:
	movswl -10(%ebp),%eax
	cmpl %eax,-16(%ebp)
	jl .L43
	jmp .L41
	.p2align 2,0x90
.L43:
	addl $-12,%esp
	pushl $5
	call random__Fi
	addl $16,%esp
.L42:
	incl -16(%ebp)
	jmp .L40
	.p2align 2,0x90
.L41:
	movb $1,%al
	jmp .L31
	.p2align 2,0x90
.L35:
.L34:
	movb $1,%al
	jmp .L31
	jmp .L44
	jmp .L31
	.p2align 2,0x90
.L44:
.L31:
	leave
	ret
.Lfe5:
		.size		 init_random__Fi,.Lfe5-init_random__Fi
	.p2align 2,0x90
.globl randomize__Fi
		.type		 randomize__Fi,@function
randomize__Fi:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call init_random__Fi
	addl $16,%esp
	jmp .L46
	jmp .L45
	.p2align 2,0x90
.L46:
.L45:
	leave
	ret
.Lfe6:
		.size		 randomize__Fi,.Lfe6-randomize__Fi
	.p2align 2,0x90
.globl do_mess__Fv
		.type		 do_mess__Fv,@function
do_mess__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	addl $2,_rval
	cmpl $4095,_rval
	jle .L48
	movl $0,_rval
.L48:
	movl $.LC8,-8(%ebp)
	addl $-8,%esp
	pushl $.LC9
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L49
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC10
	call printf
	addl $16,%esp
	jmp .L50
	.p2align 2,0x90
.L49:
	addl $-4,%esp
	movl _rval,%eax
	pushl %eax
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L50:
	jmp .L51
	jmp .L47
	.p2align 2,0x90
.L51:
.L47:
	leave
	ret
.Lfe7:
		.size		 do_mess__Fv,.Lfe7-do_mess__Fv
	.section	.rodata
	.p2align 5
.LC12:
	.byte	 0x46,0x61,0x74,0x61,0x6c,0x2c,0x20,0x52,0x61,0x6e
	.byte	 0x64,0x6f,0x6d,0x20,0x64,0x65,0x76,0x69,0x63,0x65
	.byte	 0x20,0x6e,0x6f,0x74,0x20,0x6f,0x70,0x65,0x6e,0x21
	.byte	 0xa,0x0
.text
	.p2align 2,0x90
.globl random__Fi
		.type		 random__Fi,@function
random__Fi:
	pushl %ebp
	movl %esp,%ebp
	subl $48,%esp
	pushl %esi
	pushl %ebx
	cmpl $0,r_type
	jne .L53
	cmpl $0,_ran_device
	jne .L54
	addl $-12,%esp
	pushl $.LC12
	call printf
	addl $16,%esp
	xorl %eax,%eax
	jmp .L52
	.p2align 2,0x90
.L54:
	movl $0,-20(%ebp)
	.p2align 2,0x90
.L55:
	cmpl $3,-20(%ebp)
	jle .L58
	jmp .L56
	.p2align 2,0x90
.L58:
	addl $-12,%esp
	movl _ran_device,%eax
	pushl %eax
	call fgetc
	addl $16,%esp
	movl %eax,%eax
	movl -20(%ebp),%edx
	movl %edx,%ecx
	leal 0(,%ecx,4),%edx
	leal -16(%ebp),%ecx
	movl %eax,(%edx,%ecx)
.L57:
	incl -20(%ebp)
	jmp .L55
	.p2align 2,0x90
.L56:
	movl -12(%ebp),%eax
	movl %eax,%edx
	movl %edx,%ecx
	sall $8,%ecx
	movl %ecx,%eax
	addl -16(%ebp),%eax
	movl -8(%ebp),%edx
	movl %edx,%ecx
	movl %ecx,%edx
	sall $16,%edx
	addl %edx,%eax
	movl -4(%ebp),%edx
	movl %edx,%ecx
	movl %ecx,%edx
	sall $24,%edx
	leal (%edx,%eax),%ecx
	movl %ecx,-24(%ebp)
	movl $-1,%ecx
	movl %ecx,%eax
	xorl %edx,%edx
	divl 8(%ebp)
	movl %eax,-28(%ebp)
	movl -24(%ebp),%ecx
	movl %ecx,%eax
	xorl %edx,%edx
	divl -28(%ebp)
	movl %eax,-28(%ebp)
	movl -28(%ebp),%edx
	movl %edx,-24(%ebp)
	movl -24(%ebp),%edx
	movl %edx,%eax
	jmp .L52
	jmp .L59
	.p2align 2,0x90
.L53:
	cmpl $1,r_type
	jne .L59
	cmpl $0,__SLOW_RND
	je .L61
	call do_mess__Fv
.L61:
	cmpl $0,8(%ebp)
	jg .L62
	movl $-1,%eax
	jmp .L52
	jmp .L59
	.p2align 2,0x90
.L62:
	call random
	movl %eax,-28(%ebp)
	movl $-2004318072,%ecx
	movl %ecx,%eax
	xorl %edx,%edx
	divl 8(%ebp)
	movl %eax,%esi
	movl -28(%ebp),%eax
	xorl %edx,%edx
	divl %esi
	movl %eax,%ebx
	movl %ebx,%eax
	jmp .L52
	.p2align 2,0x90
.L63:
.L60:
.L59:
	xorl %eax,%eax
	jmp .L52
	jmp .L64
	jmp .L52
	.p2align 2,0x90
.L64:
.L52:
	leal -56(%ebp),%esp
	popl %ebx
	popl %esi
	leave
	ret
.Lfe8:
		.size		 random__Fi,.Lfe8-random__Fi
	.p2align 2,0x90
.globl kill_random__Fv
		.type		 kill_random__Fv,@function
kill_random__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	cmpl $0,r_type
	jne .L66
	cmpl $0,_ran_device
	je .L67
	addl $-12,%esp
	movl _ran_device,%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L67:
	jmp .L68
	.p2align 2,0x90
.L66:
	cmpl $1,r_type
	jne .L68
	addl $-12,%esp
	pushl $20
	call random__Fi
	addl $16,%esp
	movl %eax,%eax
	movl _rval,%edx
	addl $10,%edx
	leal (%eax,%edx),%ecx
	movl %ecx,_rval
	cmpl $4095,_rval
	jle .L70
	movl $0,_rval
.L70:
	movl $.LC8,-8(%ebp)
	addl $-8,%esp
	pushl $.LC9
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L71
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC10
	call printf
	addl $16,%esp
	jmp .L68
	.p2align 2,0x90
.L71:
	addl $-4,%esp
	movl _rval,%eax
	pushl %eax
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L72:
.L69:
.L68:
	jmp .L73
	jmp .L65
	.p2align 2,0x90
.L73:
.L65:
	leave
	ret
.Lfe9:
		.size		 kill_random__Fv,.Lfe9-kill_random__Fv
.data
	.p2align 2
	.type	 n,@object
	.size	 n,4
n:
	.long 20
	.section	.rodata
.LC13:
	.byte	 0x25,0x64,0x2c,0x20,0x0
.text
	.p2align 2,0x90
.globl show__FPi
		.type		 show__FPi,@function
show__FPi:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L77:
	movl -4(%ebp),%eax
	cmpl n,%eax
	jl .L80
	jmp .L78
	.p2align 2,0x90
.L80:
	addl $-8,%esp
	movl -4(%ebp),%eax
	leal 0(,%eax,4),%edx
	movl 8(%ebp),%eax
	movl (%eax,%edx),%edx
	pushl %edx
	pushl $.LC13
	call printf
	addl $16,%esp
.L79:
	incl -4(%ebp)
	jmp .L77
	.p2align 2,0x90
.L78:
	addl $-8,%esp
	pushl $__sF+88
	pushl $10
	call __sputc__FiP7__sFILE
	addl $16,%esp
	jmp .L81
	jmp .L76
	.p2align 2,0x90
.L81:
.L76:
	leave
	ret
.Lfe10:
		.size		 show__FPi,.Lfe10-show__FPi
	.section	.rodata
.LC14:
	.byte	 0x45,0x41,0x58,0x3d,0x25,0x64,0xa,0x0
.text
	.p2align 2,0x90
.globl main
		.type		 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $28,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	cmpl $2,8(%ebp)
	jne .L83
	addl $-4,%esp
	pushl $n
	pushl $.LC11
	movl 12(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	pushl %edx
	call sscanf
	addl $16,%esp
.L83:
	addl $-12,%esp
	pushl $0
	call randomize__Fi
	addl $16,%esp
	addl $-12,%esp
	movl n,%ebx
	leal 0(,%ebx,4),%eax
	pushl %eax
	call __builtin_vec_new
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L84
	movl $-1,%eax
	jmp .L82
	.p2align 2,0x90
.L84:
	movl $0,-8(%ebp)
	.p2align 2,0x90
.L85:
	movl -8(%ebp),%eax
	cmpl n,%eax
	jl .L88
	jmp .L86
	.p2align 2,0x90
.L88:
	addl $-12,%esp
	pushl $1000
	call random__Fi
	addl $16,%esp
	movl %eax,%eax
	movl -8(%ebp),%edx
	leal 0(,%edx,4),%ecx
	movl -4(%ebp),%edx
	movl %eax,(%edx,%ecx)
.L87:
	incl -8(%ebp)
	jmp .L85
	.p2align 2,0x90
.L86:
	addl $-12,%esp
	movl n,%esi
	leal 0(,%esi,4),%eax
	pushl %eax
	call __builtin_vec_new
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-12(%ebp)
	movl $0,-16(%ebp)
	.p2align 2,0x90
.L89:
	movl -16(%ebp),%eax
	cmpl n,%eax
	jl .L92
	jmp .L90
	.p2align 2,0x90
.L92:
	movl -16(%ebp),%edx
	leal 0(,%edx,4),%eax
	movl -12(%ebp),%edx
	movl -16(%ebp),%ecx
	leal 0(,%ecx,4),%edi
	movl -4(%ebp),%ecx
	movl (%ecx,%edi),%edi
	movl %edi,(%edx,%eax)
.L91:
	incl -16(%ebp)
	jmp .L89
	.p2align 2,0x90
.L90:
	addl $-12,%esp
	movl -12(%ebp),%eax
	pushl %eax
	call show__FPi
	addl $16,%esp
	pushl $cmpint__FPvT0
	movl -12(%ebp),%eax
	pushl %eax
	movl n,%eax
	pushl %eax
	pushl $0
	call qsortasm__FiiPPvPFPvPv_i
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-20(%ebp)
	addl $-8,%esp
	movl -20(%ebp),%eax
	pushl %eax
	pushl $.LC14
	call printf
	addl $16,%esp
	addl $-12,%esp
	movl -12(%ebp),%eax
	pushl %eax
	call show__FPi
	addl $16,%esp
	xorl %eax,%eax
	jmp .L82
.L82:
	leal -40(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe11:
		.size		 main,.Lfe11-main
	.p2align 2,0x90
		.type		 __sputc__FiP7__sFILE,@function
__sputc__FiP7__sFILE:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%esi
	decl 8(%esi)
	cmpl $0,8(%esi)
	jge .L4
	movl 8(%esi),%eax
	cmpl 24(%esi),%eax
	jl .L3
	cmpb $10,%bl
	jne .L4
	jmp .L3
	.p2align 2,0x90
.L4:
	movl (%esi),%edi
	movb %bl,%al
	movb %al,(%edi)
	andl $255,%eax
	incl (%esi)
	movl %eax,%eax
	jmp .L2
	jmp .L5
	.p2align 2,0x90
.L3:
	addl $-8,%esp
	pushl %esi
	pushl %ebx
	call __swbuf
	addl $16,%esp
	movl %eax,%edx
	movl %edx,%eax
	jmp .L2
	.p2align 2,0x90
.L5:
	jmp .L6
	jmp .L2
	.p2align 2,0x90
.L6:
.L2:
	leal -24(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe12:
		.size		 __sputc__FiP7__sFILE,.Lfe12-__sputc__FiP7__sFILE
.section	.gnu.linkonce.t.cmpint__FPvT0,"ax",@progbits
	.p2align 2
	.globl	cmpint__FPvT0
	.weak	cmpint__FPvT0
		.type		 cmpint__FPvT0,@function
cmpint__FPvT0:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%edx
	movl 12(%ebp),%ecx
	cmpl %ecx,%edx
	setl %al
	movzbl %al,%ebx
	movl %ebx,%eax
	jmp .L74
	jmp .L75
	jmp .L74
	.p2align 2
.L75:
.L74:
	movl -4(%ebp),%ebx
	leave
	ret
.Lfe13:
		.size		 cmpint__FPvT0,.Lfe13-cmpint__FPvT0
	.local	_ran_device
	.comm	_ran_device,4,4
	.local	_ran_dev_name
	.comm	_ran_dev_name,30,1
	.ident	"GCC: (GNU) cplusplus 2.95.3 20010315 (release) [FreeBSD]"
