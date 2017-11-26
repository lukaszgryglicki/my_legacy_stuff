.data
no_disk_msg: .string	"no disk to search on!\n"
no_open_msg: .string	"cannot open disk given!\n"
open_perr:   .string	"open"
lseek_perr:  .string	"lseek"
read_perr:   .string	"read"
write_perr:  .string	"write"
usage_msg:   .string	"usage:\t%s file_disk from nsects\n"
usage2_msg:  .string	"writes to stdout.\n"
.text
.globl main
rd:
	pushl	%ebp
	movl	%esp, %ebp	# C frame  stack
	subl	$568, %esp	# WANT (512+1) align 4 = 516 bytes (buff)
				# disk,i (8 BYTES)   516+8 = 524 ALIGN P2
				# Aligned to the 8 BYTE granularity
	cmpl	$0, 8(%ebp)	# 0 -pushed EBP, 4 STACK frames, 8 1st arg
				# is NULL?
	
	jne	no_null_disk	# null disk name given
	subl	$12, %esp	# clear stack
	pushl	$no_disk_msg	# msg
	call	printf
	addl	$16, %esp
	movl	$2, -556(%ebp)	# return code is 2
	jmp	return
no_null_disk:
	subl	$8, %esp	# get fargs
	pushl	$0		# mode O_RDONLY
	pushl	8(%ebp)		# disk_name
	call	open		# syscall
	addl	$16, %esp	# clr stack
	movl	%eax, -12(%ebp)
	cmpl	$-1, -12(%ebp)	# is result -1?
	jne	open_ok
	subl	$12, %esp
	pushl	$no_open_msg	# error
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	$open_perr	# print errno
	call	perror
	addl	$16, %esp
	movl	$4, -556(%ebp)	# ret code is 4
	jmp	return
open_ok:
	pushl	$0
	movl	12(%ebp), %eax	# get number: from which sector
	sall	$9, %eax	# multiply by 512 (sectsize)
	cltd			# now EDX = 0
	pushl	%edx		# SEEK_SET
	pushl	%eax		# offset
	pushl	-12(%ebp)	# fd
	call	lseek
	addl	$16, %esp
	movl	%edx, %ecx
	xorl	$-1, %ecx
	xorl	$-1, %eax
	orl	%ecx, %eax	# testing on 64bit values
	testl	%eax, %eax	# in AMD64 will be just cmpq $-1, %rax
	jne	lseek_ok
	subl	$12, %esp
	pushl	$lseek_perr
	call	perror
	addl	$16, %esp
	movl	$8, -556(%ebp)	# errcode is 8 
	jmp	return
lseek_ok:
	movl	$0, -16(%ebp)	# i=0
.L12:
	movl	-16(%ebp), %eax
	cmpl	16(%ebp), %eax	# is i<nsect?
	jl	iterate		# YES
	jmp	loop_done	# EQUAL
iterate:
	subl	$4, %esp	# fd
	pushl	$512		# sectsize
	leal	-552(%ebp), %eax	
				#  buffaddr
	pushl	%eax
	pushl	-12(%ebp)
	call	read
	addl	$16, %esp
	cmpl	$512, %eax	# did we read all sectsize?
	je	read_all	# YES
	subl	$12, %esp
	pushl	$read_perr	# stderr
	call	perror
	addl	$16, %esp
	movl	$16, -556(%ebp)	# errcode is 16
	jmp	return
read_all:
	subl	$4, %esp
	pushl	$512		# how much write
	leal	-552(%ebp), %eax
				# buffer addres
	pushl	%eax
	pushl	$1		# fd, stdout=1
	call	write
	addl	$16, %esp
	cmpl	$512, %eax	# written 512bytes?
	je	write_all	# YES
	subl	$12, %esp
	pushl	$write_perr
	call	perror
	addl	$16, %esp
	movl	$32, -556(%ebp)	# NO, errcode = 32
	jmp	return
write_all:
	leal	-16(%ebp), %eax
	incl	(%eax)
	jmp	.L12
loop_done:
	subl	$12, %esp
	pushl	-12(%ebp)
	call	close
	addl	$16, %esp
	movl	$0, -556(%ebp)
return:
	movl	-556(%ebp), %eax
	leave
	ret
main:
	pushl	%ebp		# LIBC stuff...
	movl	%esp, %ebp
	subl	$8, %esp
	andl	$-16, %esp
	movl	$0, %eax
	subl	%eax, %esp
	cmpl	$3, 8(%ebp)	# is 1st (argc) cmdline param >=3
	jg	params_enough
	subl	$8, %esp	# NO
	movl	12(%ebp), %eax
	pushl	(%eax)
	pushl	$usage_msg	# write usage info then.
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	$usage2_msg	# and another
	call	printf
	addl	$16, %esp
	movl	$1, -4(%ebp)	# errcode = 1
	jmp	leave_main
params_enough:			#lets start.
	subl	$4, %esp
	subl	$8, %esp
	movl	12(%ebp), %eax
	addl	$12, %eax
	pushl	(%eax)
	call	atoi		# translate strings to ints
	addl	$12, %esp	# from where char* -> int
	pushl	%eax
	subl	$4, %esp
	movl	12(%ebp), %eax
	addl	$8, %eax
	pushl	(%eax)
	call	atoi		# how much char* -> int
	addl	$8, %esp
	pushl	%eax
	movl	12(%ebp), %eax
	addl	$4, %eax
	pushl	(%eax)
	call	rd		# args ready.
	addl	$16, %esp
	movl	%eax, -4(%ebp)
leave_main:
	movl	-4(%ebp), %eax
	leave
	ret
