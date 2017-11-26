.data           
res:
.long 0x0
bin:
.long 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
dig:
.byte 0x0
.global _start
_start:              

    pushl   $0x4
    pushl   $res
    pushl   $0x0
    movl    $0x3, %eax
    call kernel
    addl    $0xc, %esp

    movl    $res, %ebx		# EBX = &res
    movl    (%ebx), %eax	# EAX = res, from STDIN
    movl    %eax, %ebx		# EBX = EAX
    subl    $0x1, %ebx		# EBX = res - 1
    andl    %ebx, %eax		# EAX = res & (res - 1)
    movl    $res, %ebx		# EBX = &res
    movl    %eax, (%ebx)	# res = res & (res - 1)
    movl    $0xdeadbeaf, %eax	# test

    call    binout

    pushl   $0x0             
    movl    $0x1, %eax        
    call    kernel
kernel:
 int     $0x80     
 ret

binout:
	movl %eax, %edx
	movl $0x1, %ecx
	shll $0x1f, %ecx
putloop:
	movl $dig, %eax
	andl %ecx, %edx
	cmpl $0x0, %edx
	jz putzero
putone:
	movb $0x31, (%eax)
	jmp digitput
putzero:
	movb $0x30, (%eax)
digitput:
	pushl 	%edx
	pushl   %ecx
	pushl   %eax
    	pushl   $0x1
    	pushl   $dig
    	pushl   $0x1          
    	movl    $0x4, %eax     
    	call    kernel
    	addl    $0xc, %esp     
	popl	%eax
	popl	%ecx
	popl	%edx

	shrl $0x1, %ecx
	cmpl $0x0, %ecx
	jnz putloop
	ret






