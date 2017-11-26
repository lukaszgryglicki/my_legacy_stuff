.data           
res:
.long 0x0
.global _start
_start:              

    pushl   $4
    pushl   $res
    pushl   $0
    movl    $3, %eax
    call kernel
    addl $12, %esp

    movl    $res, %ebx		# EBX = &res
    movl    (%ebx), %eax	# EAX = res, from STDIN
    movl    %eax, %ebx		# EBX = EAX
    subl    $1, %ebx		# EBX = res - 1
    andl    %ebx, %eax		# EAX = res & (res - 1)
    movl    $res, %ebx		# EBX = &res
    movl    %eax, (%ebx)	# res = res & (res - 1)

    pushl   $4      
    pushl   $res
    pushl   $1          
    movl    $4, %eax     
    call    kernel
    addl    $12, %esp     

    pushl   $0             
    movl    $1, %eax        
    call    kernel
kernel:
 int     $0x80     
 ret

