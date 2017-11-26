.globl asmf1
.globl asmf2
.globl asmf3
.globl compute_asm
.globl loop_asm

#################################################################################
#args: user: RDI, RSI, RDX, RCX, R8, R9 and NO more in registers
# MODE, N, X, Y, CT, NT, PTR*** iv, PTR** ov
#extern void loop_asm(int udfmode, int n, int x, int y, int ct, int nt, void* iv, void* ov);
#void translate_jpeg_file(int n, unsigned long*** iv, unsigned long** ov, int x, int y, int ct, int nt, int udfmode)
#for (i=ct;i<y;i+=nt)
#for (j=0;j<x;j++)
#ov[i][j] = asmf1(iv[0][i][j]);					# udfmode: 4
#ov[i][j] = asmf2(iv[0][i][j], iv[1][i][j]);			# udfmode: 11
#ov[i][j] = asmf3(iv[0][i][j], iv[1][i][j], iv[2][i][j]);	# udfmode: 12
# RDI: mode
# RSI: N
# RDX: X
# RCX: Y
# R8:  CT (zostanie olane bo i tak wersja jednowatkowa)
# R9:  NT (jw.)
# 0x08(%rsp)  	***iv
# 0x10(%rsp)  	**ov
loop_asm:
	# currently not thread safe, using just one thread 0th
	cmpq $0x0, %r8
	jz work_thread
	ret				# thread other than 0, skipping, thread 0 is doing all the job in asm
work_thread:				# R8 = 0, tylko tutaj pracujemy
	# start, used regs: RDI, RSI, RDX, RCX
	
	movq 0x08(%rsp), %r15		# iv
	movq 0x10(%rsp), %r14		# ov

	pushq %rbp			# store amd64 cpu state
	pushq %rsi
	pushq %rdi
	pushq %rax
	pushq %rbx	
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	xorq %r13, %r13			# R13 = j = 0
	xorq %r12, %r12			# R12 = i = 0
					# RDX = x
					# RCX = y
					# RDI: mode

	cmpq $0x4, %rdi			# udfmode 4 asmf1
	jz lmode1
	cmpq $0xb, %rdi			# udfmode 11 asmf2
	jz lmode2
	cmpq $0xc, %rdi			# udfmode 12 asmf3
	jz lmode3
	jmp lmodedone
lmode1:					# mode asmf1
	cmpq $0x1, %rsi			# for asmf1 need n >= 1
	jge lmode1nok
	jmp lmodedone			# N of args too small for asmf1
lmode1nok:
	movq 0x00(%r15), %r11		# R11 = iv[0]

iloopmode1:				# for (i=ct ...
					# inside i_loop
	movq %r12, %r10			# i
	shlq $0x3, %r10			# 64 bit i offset
	addq %r11, %r10			
	movq 0x00(%r10), %r10		# R10 = iv[0][i]		

	xorq %r13, %r13			# R13 = j = 0
jloopmode1:				# for (j=0 ...
					# inside j_loop
					# iv[0][i][j]
	movq %r13, %rdi			# j
	shlq $0x3, %rdi			# 64 bit j
	addq %r10, %rdi			# &iv[0][i]
	movq 0x00(%rdi), %rdi		# RDI = iv[0][i][j]
					# R15: iv, R14: ov, R13: j, R12: i, R11: iv[0], R10: iv[0][i], RDI: iv[0][i][j], RDX: x, RCX: y

l1call:

	pushq %rcx			# store registers used
	pushq %rdx
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15			# all we need to store

					# in RDI we have iv[0][i][j] which is needed for asmf1 as arg

	call asmf1			# asmf1(iv[0][i][j])

					# rsult in rax
	popq %r15			# restore registers used
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %rdx
	popq %rcx			# all we need to restore

	movq %r12, %r9
	shlq $0x3, %r9			# R9 = 8*i 64 bit index
	addq %r14, %r9
	movq 0x00(%r9), %r9		# R9 = ov[i]

	movq %r13, %r8
	shlq $0x3, %r8
	addq %r8, %r9			# R9 = &ov[i][j]
l1res:
	movq %rax, (%r9)		# ov[i][j] = asmf1(iv[0][i][j])
	#movl %eax, (%r9)		# ov[i][j] = asmf1(iv[0][i][j])

	incq %r13			# j ++
	cmpq %rdx, %r13			# j<x
	jl jloopmode1

	incq %r12			# i ++ (currently not thread safe, so only 0th thread is working, and doing all job)
	cmpq %rcx, %r12			# j<y
	jl iloopmode1
	
	jmp lmodedone
#########				## finished mode1
lmode2:					# mode asmf2
	cmpq $0x2, %rsi			# for asmf2 need n >= 2
	jge lmode2nok
	jmp lmodedone			# N of args too small for asmf2, need N >= 2
lmode2nok:
	movq 0x00(%r15), %r11		# R11 = iv[0]
	movq 0x08(%r15), %rbx		# RBX = iv[1]

iloopmode2:				# for (i=ct ...
	movq %r12, %r10			# i
	shlq $0x3, %r10			# 64 bit i offset
	addq %r11, %r10			
	movq 0x00(%r10), %r10		# R10 = iv[0][i]		

	movq %r12, %rax			# i
	shlq $0x3, %rax			# 64 bit i offset
	addq %rbx, %rax			
	movq 0x00(%rax), %rax		# RAX = iv[1][i]		

	xorq %r13, %r13			# R13 = j = 0
					# inside i_loop
jloopmode2:				# for (j=0 ...
	movq %r13, %rdi			# j
	shlq $0x3, %rdi			# 64 bit j
	addq %r10, %rdi			# &iv[0][i]
	movq 0x00(%rdi), %rdi		# RDI = iv[0][i][j]

	movq %r13, %rsi			# j
	shlq $0x3, %rsi			# 64 bit j
	addq %rax, %rsi			# &iv1[][i]
	movq 0x00(%rsi), %rsi		# RSI = iv[1][i][j]
					# R15: iv, R14: ov, R13: j, R12: i, R11: iv[0], R10: iv[0][i], RDI: iv[0][i][j], RDX: x, RCX: y
					# RBX: iv[1], RAX: iv[1][i], RSI = iv[1][i][j]

l2call:
	pushq %rax			# store registers used
	pushq %rbx	
	pushq %rcx
	pushq %rdx
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15			# all we need to store

					# in RDI, RSI we have iv[0][i][j], iv[1][i][j] which is needed for asmf2 as args

	call asmf2			# asmf2(iv[0][i][j], iv[1][i][j])
					# rsult in rax
	movq %rax, %rbp			# RAX is restored from stack, so result in RBP

	popq %r15			# restore registers used
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax			# all we need to restore

	movq %r12, %r9
	shlq $0x3, %r9			# R9 = 8*i 64 bit index
	addq %r14, %r9
	movq 0x00(%r9), %r9		# R9 = ov[i]

	movq %r13, %r8
	shlq $0x3, %r8
	addq %r8, %r9			# R9 = &ov[i][j]
l2res:
	movq %rbp, (%r9)		# ov[i][j] = asmf2(iv[0][i][j], iv[1][i][j])

	incq %r13			# j ++
	cmpq %rdx, %r13			# j<x
	jl jloopmode2

	incq %r12			# i ++ (currently not thread safe, so only 0th thread is working, and doing all job)
	cmpq %rcx, %r12			# j<y
	jl iloopmode2
	
	jmp lmodedone
########				## finished mode2
lmode3:					# mode asmf3
########
	cmpq $0x3, %rsi			# for asmf3 need n >= 3
	jge lmode3nok
	jmp lmodedone			# N of args too small for asmf2, >=3 neede
lmode3nok:
	movq 0x00(%r15), %r11		# R11 = iv[0]
	movq 0x08(%r15), %rbx		# RBX = iv[1]
	movq 0x10(%r15), %r8		# R8  = iv[2]

iloopmode3:				# for (i=ct ...
	movq %r12, %r10			# i
	shlq $0x3, %r10			# 64 bit i offset
	addq %r11, %r10			
	movq 0x00(%r10), %r10		# R10 = iv[0][i]		

	movq %r12, %rax			# i
	shlq $0x3, %rax			# 64 bit i offset
	addq %rbx, %rax			
	movq 0x00(%rax), %rax		# RAX = iv[1][i]		

	movq %r12, %r9			# i
	shlq $0x3, %r9			# 64 bit i offset
	addq %r8, %r9			
	movq 0x00(%r9), %r9		# R9 = iv[2][i]		

	xorq %r13, %r13			# R13 = j = 0
					# inside i_loop
jloopmode3:				# for (j=0 ...

	movq %r13, %rdi			# j
	shlq $0x3, %rdi			# 64 bit j
	addq %r10, %rdi			# &iv[0][i]
	movq 0x00(%rdi), %rdi		# RDI = iv[0][i][j]

	movq %r13, %rsi			# j
	shlq $0x3, %rsi			# 64 bit j
	addq %rax, %rsi			# &iv[1][i]
	movq 0x00(%rsi), %rsi		# RSI = iv[1][i][j]

	movq %r13, %rbp			# j
	shlq $0x3, %rbp			# 64 bit j
	addq %r9, %rbp			# &iv[2][i]
	movq 0x00(%rbp), %rbp		# RBP = iv[2][i][j]

					# R15: iv, R14: ov, R13: j, R12: i, R11: iv[0], R10: iv[0][i], RDI: iv[0][i][j], RDX: x, RCX: y
					# RBX: iv[1], RAX: iv[1][i], RSI: iv[1][i][j]
					# R8:  iv[2], R9:  iv[2][i], RBP: iv[2][i][j]
					# only dangerous registers unused: RIP and RSP (15 of 16 AMD64 registers used and NO memory)
					# R(ax, bx, cx, dx, si, di, bp, 8, 9, 10, 11, 12, 13, 14, 15) not used r(sp, ip)

l3call:

					# si, di - args, bp - temporary (3 regs)
	pushq %rax			# store registers used (ax, bx, cx, dx, r8-15): 12 regs, 2 totally usused, 3 + 12 +2 = 17 regs
	pushq %rbx	
	pushq %rcx
	pushq %rdx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15			# all we need to store

					# in RDI, RSI we have iv[0][i][j], iv[1][i][j] which is needed for asmf2 as args
	movq %rbp, %rdx			# need to copy value of iv[2][i][j] from RBP to RDX (3 parameters in registers: EDI, ESI, RDX), next are RCX, R8, R9, next on stack

	call asmf3			# asmf3(iv[0][i][j], iv[1][i][j], iv[2][i][j])
	movq %rax, %rbp			# result in RAX, but copy it to RBP, rax is for iv[1][i]
					# RBP: iv[2][i][j] -> RDX as 3rd arg, RAX as result --> RBP (fiannly rbp iv[2][i][j] -> result)

	popq %r15			# restore registers used
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax			# all we need to restore

					# we dont need values of 3 args now: so RDI, RSI are free to use
					# RDI, RSi output addressing, RBP result to write

	movq %r12, %rdi
	shlq $0x3, %rdi			# RDI = 8*i 64 bit index
	addq %r14, %rdi
	movq 0x00(%rdi), %rdi		# RDI = ov[i]

	movq %r13, %rsi
	shlq $0x3, %rsi
	addq %rdi, %rsi			# RSI = &ov[i][j]
l3res:
	movq %rbp, (%rsi)		# ov[i][j] = asmf2(iv[0][i][j], iv[1][i][j])

	incq %r13			# j ++
	cmpq %rdx, %r13			# j<x
	jl jloopmode3

	incq %r12			# i ++ (currently not thread safe, so only 0th thread is working, and doing all job)
	cmpq %rcx, %r12			# j<y
	jl iloopmode3
########				## finished mode2
lmodedone:				# all computed

	popq %r15			# restore amd64 cpu state
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	popq %rdi
	popq %rsi
	popq %rbp

	ret
#end of loop_asm
#################################################################################
/*
(unsigned char* pix, unsigned long** tab, int x, int y)
extern void compute_asm(void* pix, void* tab, int x, int y);
*/
#args: user: RDI, RSI, RDX, RCX, R8, R9 and NO more in registers
#return values: RAX, RDX and no more
compute_asm:
	#pushal
	# RDI = char* pix
	# RSI = long** tab
	# RDX = X
	# RCX = Y

	movq %rcx, %r15
	decq %r15			# R15 = i := y-1 --> 0
i_ca:
	movq %r15, %r14			# compute i idx, 64 bit ptrs
	shlq $0x3, %r14
	addq %rsi, %r14	
	movq (%r14), %r14		# R14 = tab[i]

	xorq %r13, %r13			# R13 = j := 0 --> x
j_ca:
	movq %r13, %r12			# compute j idx, 32bit ptrs
	shlq $0x3, %r12			# compute 64 bit index
	addq %r14, %r12
	movq (%r12), %r12		# R12 = tab[i][j]

/*
	movq %r12, %r11
	andq $0xff0000, %r11
	shrq $0x10, %r11		# EDX: R --> B

	movq %r12, %r10
	andq $0xff, %r10
	shlq $0x10, %r10		# ESI: B --> R

	andq $0xff00, %r12		# EAX: G

	addq %r11, %r12			# EAX: BG
	addq %r11, %r12			# EAX: BGR
*/
	bswapq %r12			# 0x0000000000BBGGRR --> 0xRRGGBB0000000000
	shrq $0x28, %r12		# 0xRRGGBB0000000000 --> 0x0000000000RRGGBB (RRGGBB) --> pix[k] next inst

	movq %r12, (%rdi)		# pix[k] = tab[i][j] but writes out 8 BYTES, so 5 more bytes need to be allocated!
	addq $0x3, %rdi			# k += 3 (RGB tripple)

	incq %r13			# R13: j++
	cmpq %rdx, %r13 		# j<x
	jl j_ca				# j_loop

	decq %r15			# EAX: i--
	cmpq $0x0, %r15			# i<y
	jge i_ca			# i_loop
done_ca:
					# all written to pix[k+=3] = tab[i]j[]: i=0..y, j=0..x
	ret



#ARGS: RDI, RSI
asmf2:
stereo:
	movq %rsi, %r8
	andq $0xff, %r8

	movq %rdi, %r9
	andq $0xff, %r9

	addq %r9, %r8
	shrq $0x2, %r8			# R8 = ( blue1 + blue2 ) / 4

	movq %rdi, %r9
	movq %rsi, %r10

	andq $0xff0000, %r10		# R10 = red1 on its own position
	andq $0xff00, %r9		# R9 = green2 on its own position

	movq %r8, %rax
	addq %r9, %rax		
	addq %r10, %rax			# RAX = RGB(R1, G2, R8)	stereo from 1 and 2 using red and green


	ret

#ARGS: RDI, RSI
#asmf2:
cdiff:
	movq %rdi, %rax
	movq %rsi, %r9

	andq $0xff0000, %rax
	shrq $0x10, %rax	# RAX = red1

	andq $0xff0000, %r9
	shrq $0x10, %r9		# R9 = red2

	subq %r9, %rax
	cmpq $0x0, %rax
	jge rge0
	imulq $-1, %rax
rge0:				# RAX = rdiff
	movq %rdi, %r9
	movq %rsi, %r10

	andq $0xff00, %r9
	shrq $0x08, %r9		# R9 = green1

	andq $0xff00, %r10
	shrq $0x08, %r10	# R10 = green2

	subq %r10, %r9
	cmpq $0x0, %r9
	jge gge0
	imulq $-1, %r9
gge0:				# R9 = gdiff

	movq %rdi, %r10
	movq %rsi, %r11
	andq $0xff, %r10	# R10 = blue1
	andq $0xff, %r11	# R11 = blue2

	subq %r10, %r11
	cmpq $0x0, %r11
	jge bge0
	imulq $-1, %r11
bge0:				# R11 = bdiff


	# RAX, R9, R11
	shlq $0x1, %rax
	shlq $0x1, %r9
	shlq $0x1, %r11

	andq $0xff, %rax
	andq $0xff, %r9
	andq $0xff, %r11

	shlq $0x10, %rax	# shift rdiff to red
	shlq $0x08, %r9		# shift gdiff to green

	addq %r9, %rax
	addq %r11, %rax		# RAX = RGB(rdiff, gdiff, bdiff)

	ret

# args: rdi rsi rdx
#asmf3:
bitops:
	movq %rdi, %rax		# EAX = X1
	movq %rsi, %r8		# ECX = X2
	movq %rdx, %r9		# EDX = X3

	xorq %r8, %rax
	xorq %r9, %rax

	ret

# args: rdi rsi rdx
#asmf3:
sharpen:
	movq %rsi, %r8
	andq $0xff0000, %r8
	shrq $0x10, %r8		# R8 = red2

	movq %rsi, %r9
	andq $0xff00, %r9
	shrq $0x8, %r9		# R9 = green2

	movq %rsi, %r10
	andq $0xff, %r10	# R10 = blue2

	movq %r8, %r11
	shlq $0x1, %r11
	addq %r11, %r8		# R8= 3r2

	movq %r9, %r11
	shlq $0x1, %r11
	addq %r11, %r9		# R9 = 3g2

	movq %r10, %r11
	shlq $0x1, %r11
	addq %r11, %r10		# R10 = 3b1

	movq %rdi, %r11		# R11 = X1
	andq $0xff0000, %r11
	shrq $0x10, %r11	# R11 = red1
	subq %r11, %r8		# R8  = 3r2-r1

	movq %rdi, %r11
	andq $0xff00, %r11
	shrq $0x8, %r11		# R11 = green1
	subq %r11, %r9		# R9  = 3g2-g1

	movq %rdi, %r11
	andq $0xff, %r11	# R11 = blue1
	subq %r11, %r10		# R10 = 3b2-b1

	movq %rdx, %r11		# R11 = X3
	andq $0xff0000, %r11
	shrq $0x10, %r11	# R11 = red3
	subq %r11, %r8		# R8  = 3r2-r1-r3

	movq %rdx, %r11
	andq $0xff00, %r11
	shrq $0x8, %r11		# R11 = green3
	subq %r11, %r9		# R9  = 3g2-g1-g3

	movq %rdx, %r11
	andq $0xff, %r11	# R11 = blue3
	subq %r11, %r10		# R9  = 3b2-b1-b3

	cmpq $0x0, %r8		# force R to [0,ff]
	jge rmore0
	xorq %r8, %r8
	jmp rok
rmore0:
	cmpq $0xff, %r8
	jle rok
	movq $0xff, %r8
rok:

	cmpq $0x0, %r9		# force G to [0,ff]
	jge gmore0
	xorq %r9, %r9
	jmp gok
gmore0:
	cmpq $0xff, %r9
	jle gok
	movq $0xff, %r9
gok:

	cmpq $0x0, %r10		# force B to [0,ff]
	jge bmore0
	xorq %r10, %r10
	jmp bok
bmore0:
	cmpq $0xff, %r10
	jle bok
	movq $0xff, %r10
bok:

	shlq $0x10, %r8		# shift to red
	shlq $0x08, %r9		# shift to green

	movq %r8, %rax
	addq %r9, %rax
	addq %r10, %rax		# rax = RGB(srp_r, shr_b, srp_g)

	ret

#ARGS: RDI, RSI, RDX
asmf3:
gsalien_rgbtail:
	movq %rdi, %r8
	andq $0xff0000, %r8
	shrq $0x10, %r8		# R8 = R1

	movq %rdi, %r9
	andq $0xff00, %r9
	shrq $0x08, %r9		# R9 = G1

	movq %rdi, %rax		# RAX = B1
	andq $0xff, %rax

	addq %r8, %rax
	addq %r9, %rax		# RAX = GS1 [0, 765)

	cmpq $0x0ff, %rax
	jle x1done
	cmpq $0x1fe, %rax
	jle x1middle
x1high:

	subq $0x1fe, %rax
	jmp x1done
x1middle:

	movq %rax, %r8
	movq $0x1fe, %rax
	subq %r8, %rax
x1done:				# RAX = GSa1

	movq %rsi, %r8
	andq $0xff0000, %r8
	shrq $0x10, %r8		# R8 = R2

	movq %rsi, %r9
	andq $0xff00, %r9
	shrq $0x08, %r9		# R9 = G2

	movq %rsi, %r10		# R10 = B2
	andq $0xff, %r10

	addq %r8, %r10
	addq %r9, %r10		# R10 = GS2 [0, 765)

	cmpq $0x0ff, %r10
	jle x2done
	cmpq $0x1fe, %r10
	jle x2middle
x2high:

	subq $0x1fe, %r10
	jmp x2done
x2middle:

	movq %r10, %r8
	movq $0x1fe, %r10
	subq %r8, %r10
x2done:				# R10 = GSa2

	movq %rdx, %r8
	andq $0xff0000, %r8
	shrq $0x10, %r8		# R8 = R3

	movq %rdx, %r9
	andq $0xff00, %r9
	shrq $0x08, %r9		# R9 = G3

	movq %rdx, %r11		# R11 = B3
	andq $0xff, %r11

	addq %r8, %r11
	addq %r9, %r11		# R11 = GS3 [0, 765)

	cmpq $0x0ff, %r11
	jle x3done
	cmpq $0x1fe, %r11
	jle x3middle
x3high:

	subq $0x1fe, %r11
	jmp x3done
x3middle:

	movq %r11, %r8
	movq $0x1fe, %r11
	subq %r8, %r11
x3done:				# R11 = GSa3
				# gsa1, gsa2, gsa3 --> RAX, R10, R11

	movq %rax, %r8		# 75% gsa1
	shrq $0x2, %r8
	subq %r8, %rax

	shrq $0x1, %r10		# 50% gsa2

	shlq $0x10, %rax	# shift 75% gsa1 to red
	shlq $0x08, %r10	# shift 50% gsa2 to green

	addq %r10, %rax
	addq %r11, %rax		# eax = RGB(GSa1, GSa2, GSa3)

	ret

#ARGS: RDI
#asmf1:
heat:

	movq %rdi, %r8
	andq $0xff0000, %r8
	shrq $0x10, %r8		# r8 = red

	movq %rdi, %r9
	andq $0xff00, %r9
	shrq $0x8, %r9		# r9 = green

	movq %rdi, %r10
	andq $0xff, %r10	# r10 = blue

	movq %r8, %rax
	addq %r9, %rax
	addq %r10, %rax		# rax = GS [0, 765)

	cmpq $110, %rax
	jg hl2
hl0:
	#<110				# R8 -> red, R9 -> green, R10 -> blue, RAX -> val shifted
	movq %rax, %r8			# r8:  r: 0 --> 110
	xorq %r9, %r9			# r9:  g: 0
	movq %rax, %r10
	shlq $1, %r10			# r10: b: 0 --> 220
	jmp hldone
hl2:
	cmpq $220, %rax
	jg hl3
	#<220
	subq $110, %rax			# eax: 0 --> 110
	movq $110, %r8		
	subq %rax, %r8			# r: 110 --> 0
	xorq %r9,  %r9			# g: 0
	movq $220, %r10			# b: 220

	jmp hldone
hl3:
	cmpq $330, %rax
	jg hl4
	#<330
	subq $220, %rax			# eax: 0 --> 110
	xorq %r8, %r8			# r: 0
	movq %rax, %r9
	shlq $1, %r9			# g: 0 --> 220
	movq $220, %r10			# b: 220
	jmp hldone
hl4:
	cmpq $440, %rax
	jg hl5
	#<440
	subq $330, %rax			# eax: 0 -->110
	xorq %r8, %r8			# r: 0
	movq $220, %r9			# g: 220
	movq $220, %r10
	subq %rax, %r10
	subq %rax, %r10			# b: 220 --> 0
	jmp hldone
hl5:
	cmpq $550, %rax
	jg hl6
	#<550
	subq $440, %rax			# eax: 0 --> 110
	movq %rax, %r8
	shlq $1, %r8			# r: 0 --> 220
	movq $220, %r9			# g: 220
	xorq %r10, %r10			# b: 0
	jmp hldone
hl6:
	cmpq $660, %rax
	jg hl7
	#<660
	subq $550, %rax			# eax: 0 --> 110
	movq $220, %r8			# r: 220
	movq $220, %r9
	subq %rax, %r9
	subq %rax, %r9			# g: 220 --> 0
	xorq %r10, %r10			# b: 0
	jmp hldone
hl7:
	#>=660
	subq $660, %rax			# eax: 0 --> 110
	movq $220, %r8			# r: 220
	movq %rax, %r9
	shlq $1, %r9			# g: 0 --> 220
	movq %rax, %r10
	shlq $1, %r10			# b: 0 --> 220
hldone:
	shlq $0x8, %r9			# red shift
	shlq $0x10, %r8			# green shift

	movq %r8, %rax
	addq %r9, %rax
	addq %r10, %rax			# rax = RGB(r, g, b)

	ret

#ARGS: RDI
asmf1:
maxred:
	movq %rdi, %r8
	andq $0xff0000, %r8
	shrq $0x10, %r8		# R8 = red

	movq %rdi, %r9
	andq $0xff00, %r9
	shrq $0x7, %r9		# R9 = 2*green

	movq %rdi, %r10
	andq $0xff, %r10	# R10 = blue

	addq %r9, %r8
	addq %r10, %r8		# R8 = r+2g+b

	shrq $0x1, %r8		# R8 = 1/2r+g+1/2b = [0,511)

	xorq %r11, %r11		# R11 = nie ma niedomiaru

	cmpq $0xff, %r8
	jle mrdone

	movq %r8, %r11
	subq $0xff, %r11	# R11 = nadmiar
	movq $0xff, %r8		# R8 = nasycone
mrdone:
				# R8 = poziom red
	
	movq %r11, %rax		# RAX = nadmiar blue
	shlq $0x8, %r11
	addq %r11, %rax		# RAX = nadmiar blue i green

	shlq $0x10, %r8
	addq %r8, %rax		# RAX = RGB(poziom R, nadmiar G i B)

	ret

#ARGS: RDI
#asmf1:
invmul:
	movq %rdi, %rax
	andq $0xff0000, %rax
	shrq $0x10, %rax		# RAX = red

	movq %rdi, %r8
	andq $0xff00, %r8
	shrq $0x08, %r8			# R8 = green

	movq %rdi, %r9
	andq $0xff, %r9			# R9 = blue

	movq %rax, %r10
	movq $0xFF, %rax
	subq %r10, %rax			# RAX = 255 - red 

	movq %r8, %r10
	movq $0xFF, %r8
	subq %r10, %r8			# R8 = 255 - green

	movq %r9, %r10
	movq $0xFF, %r9
	subq %r10, %r9			# R9 = 255 - blue
	
	movq %rax, %r10			# RAX = R10 = newR
	movq %r8, %r11			# R8  = R11 = newG
	movq %r9, %r12			# R9  = R12 = newB

	imulq %r11, %rax		# RAX = nR * nG -> antiB
	imulq %r12, %r8			# R8  = nG * nB -> antiR
	imulq %r10, %r9			# R9  = nB * nR -> antiG

	shrq $0x8, %rax			# shift after mul to 1B len		
	shrq $0x8, %r8		
	shrq $0x8, %r9		

	andq $0xff, %rax		# wrap to [0,255]
	andq $0xff, %r8
	andq $0xff, %r9

	shlq $0x8, %r9			# antiG --> shift to green
	shlq $0x10, %r8			# antiR --> shift to red

	addq %r8, %rax			# recolect to RGB(antiR, antiG, antiB)
	addq %r9, %rax

	ret

fence:
	nop
	nop
	nop
	xorq %rax, %rax
	ret

