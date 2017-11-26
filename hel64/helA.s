.code64
.data
msg:
    .string "AS64\n"
    len = .-msg-1
.text           
    .global start
start:
    movq    $len, %rdx
    movq    $msg, %rsi
    movq    $2,   %rdi
    movq    $4, %rax
    syscall
    movq    $0, %rdi
    movq    $1, %rax
    syscall
