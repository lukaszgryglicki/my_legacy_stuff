.data
iv1:
	.long 0x0
iv1i:
	.long 0x0
iv1ij:
	.long 0x0
iv2:
	.long 0x0
iv2i:
	.long 0x0
iv2ij:
	.long 0x0

mem:
	.long 0x0
.text
.align 0x8
.globl asmf1
.globl asmf2
.globl asmf3
.globl loop_asm
.globl compute_asm
/*
(unsigned char* pix, unsigned long** tab, int x, int y)
extern void compute_asm(void* pix, void* tab, int x, int y);
*/

compute_asm:
	pushal

	movl 0x24(%esp), %edi		# EDI = char* pix
	movl 0x28(%esp), %esi		# ESI = long** tab

	movl 0x2C(%esp), %ecx		# ECX = x
	movl 0x30(%esp), %edx		# EDX = y

	movl %edx, %eax
	decl %eax			# EAX = i := y-1 --> 0
i_ca:
	movl %eax, %ebp			# compute i idx, 32 bit ptrs
	shll $0x2, %ebp
	addl %esi, %ebp	
	movl (%ebp), %ebp		# EBP = tab[i]

	xorl %ebx, %ebx			# EBX = j := 0 --> x
	pushl %eax			# not needed in j_loop
	pushl %esi			# -||-
j_ca:
	movl %ebx, %eax			# compute j idx, 32bit ptrs
	shll $0x2, %eax
	addl %ebp, %eax
	movl (%eax), %eax		# EAX = tab[i][j]

/*
	movl %eax, %edx
	andl $0xff0000, %edx
	shrl $0x10, %edx		# EDX: R --> B

	mov %eax, %esi
	andl $0xff, %esi
	shll $0x10, %esi		# ESI: B --> R

	andl $0xff00, %eax		# EAX: G

	addl %edx, %eax			# EAX: BG
	addl %esi, %eax			# EAX: BGR
*/
	bswapl %eax			# 0x??BBGGRR --> 0xRRGGBB??
	shrl $0x8, %eax			# 0xRRGGBB?? --> 0x00RRGGBB (RRGGBB) --> pix[k] next inst

	movl %eax, (%edi)		# pix[k] = tab[i][j]
	addl $0x3, %edi			# k += 3 (RGB tripple)

	incl %ebx			# EBX: j++
	cmpl %ecx, %ebx 		# j<x
	jl j_ca				# j_loop

	popl %esi			# needed after j_loop
	popl %eax			# needed after j_loop

	decl %eax			# EAX: i--
	cmpl $0x0, %eax			# i<y
	jge i_ca			# i_loop
done_ca:
					# all written to pix[k+=3] = tab[i]j[]: i=0..y, j=0..x
	popal
	ret


# MODE, N, X, Y, CT, NT, PTR*** iv, PTR** ov
#extern void loop_asm(int udfmode, int n, int x, int y, int ct, int nt, void* iv, void* ov);
#void translate_jpeg_file(int n, unsigned long*** iv, unsigned long** ov, int x, int y, int ct, int nt, int udfmode)
#for (i=ct;i<y;i+=nt)
#for (j=0;j<x;j++)
#ov[i][j] = asmf1(iv[0][i][j]);					# udfmode: 4
#ov[i][j] = asmf2(iv[0][i][j], iv[1][i][j]);			# udfmode: 11
#ov[i][j] = asmf3(iv[0][i][j], iv[1][i][j], iv[2][i][j]);	# udfmode: 12
# 0x04(%esp)  	udfmode
# 0x08		n
# 0x0C		x
# 0x10		y
# 0x14		ct		# currently not thread safe, start work only for 0th thread
# 0x18		nt		# currently not used, assume 1 threads, and work only for 0th
# 0x1C		iv ptr
# 0x20		ov ptr
loop_asm:
	movl 0x14(%esp), %esi		# ESI = i = ct

	# currently not thread safe, using just one thread 0th
	cmpl $0x0, %esi
	jz work_thread
	ret
work_thread:

	pushal				# ESP -= 0x20
	xorl %edi, %edi			# EDI = j = 0
	movl 0x2c(%esp), %ecx		# ECX = x
	movl 0x30(%esp), %edx		# EDX = y

	movl 0x24(%esp), %eax		# udfmode in EAX

	cmpl $0x4, %eax			# udfmode 4 asmf1
	jz lmode1
	cmpl $0xb, %eax			# udfmode 11 asmf2
	jz lmode2
	cmpl $0xc, %eax			# udfmode 12 asmf3
	jz lmode3
	jmp lmodedone
lmode1:					# mode asmf1
#########
	movl 0x3C(%esp), %eax
	movl 0x00(%eax), %eax		# eax = iv[0]

iloopmode1:				# for (i=ct ...
					# inside i_loop
	movl %esi, %ebx
	shll $0x2, %ebx
	addl %eax, %ebx			
	movl 0x00(%ebx), %ebx		# EBX = iv[0][i]		

	xorl %edi, %edi			# EDI = j = 0
jloopmode1:				# for (j=0 ...
					# inside j_loop
					# USED: ESI, EDI, ECX, EDX, ESP, EAX, EBX
					# AVAIL: EBP
					# iv[0][i][j]
	movl %edi, %ebp
	shll $0x2, %ebp
	addl %ebx, %ebp
	movl 0x00(%ebp), %ebp		# EBP = iv[0][i][j]

l1call:

	pushl %eax			# store what needed iv[0] (1)

	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %esi
	pushl %edi

	pushl %ebp			#EBP = arg of asmf1
	call asmf1			# asmf1(iv[0][i][j])
	#movl $0xdeadbeaf, %eax		# test
	popl %ebp			# restore arg

	popl %edi
	popl %esi
	popl %edx
	popl %ecx
	popl %ebx
					# old eax left on stack, curr aex is result of  asmf1

	pushl %ebx			# need to be saved, is recomputed in external i_loop (2)

	movl 0x48(%esp), %ebx		# ebx = &ov (0x20 (8th arg) + 0x20 (pushal) + 0x8 = (push eax ebx))
	movl %esi, %ebp	
	shll $0x2, %ebp
	addl %ebp, %ebx			# ebx = 4*i (sizeof ptr(i))
	movl 0x00(%ebx), %ebx		# ebx = ov[i]

	movl %edi, %ebp
	shll $0x2, %ebp
	addl %ebp, %ebx			# ebp = &ov[i][j]
l1res:
	movl %eax, (%ebx)		# ov[i][j] = asmf1(iv[0][i][j])

	popl %ebx			# restored ebx = iv[0][i]  (2)

	popl %eax			# restored eax = iv[0]  (1)

	incl %edi			# j ++
	cmpl %ecx, %edi			# j<x
	jl jloopmode1

	#addl 0x38(%esp), %esi		# i += nt
	incl %esi			# i ++ (currently not thread safe, so only 0th thread is working, and doing all job)
	cmpl %edx, %esi			# j<y
	jl iloopmode1
	
	jmp lmodedone
#########				## finished mode1
lmode2:					# mode asmf2
#########
	movl 0x3C(%esp), %eax
	movl 0x00(%eax), %eax		# eax = iv[0]

	movl 0x3C(%esp), %ebx
	movl 0x04(%ebx), %ebx		# ebx = iv[1]
	movl %ebx, iv1			# iv1 = iv[1] 
iloopmode2:				# for (i=ct ...
					# inside i_loop
	movl %esi, %ebx
	shll $0x2, %ebx
	addl %eax, %ebx			
	movl 0x00(%ebx), %ebx		# EBX = iv[0][i]		

	pushl %ebx
	movl %esi, %ebx
	shll $0x2, %ebx
	addl (iv1), %ebx		# dodaj wartosc z iv1 czyli (iv1) a nie adres iv1 czyli iv1
	movl 0x00(%ebx), %ebx
	movl %ebx, iv1i			# iv1i = iv[1][i]
	popl %ebx

	xorl %edi, %edi			# EDI = j = 0
jloopmode2:				# for (j=0 ...
					# inside j_loop
					# USED: ESI, EDI, ECX, EDX, ESP, EAX, EBX
					# AVAIL: EBP
					# iv[0][i][j]
	movl %edi, %ebp
	shll $0x2, %ebp
	addl %ebx, %ebp
	movl 0x00(%ebp), %ebp		# EBP = iv[0][i][j]

	pushl %ebp
	movl %edi, %ebp
	shll $0x2, %ebp
	addl (iv1i), %ebp
	movl 0x00(%ebp), %ebp
	movl %ebp, iv1ij		# iv1ij = iv[1][i][j]
	popl %ebp

l2call:

	pushl %eax			# store what needed iv[0] (1)

	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %esi
	pushl %edi

	pushl (iv1ij)
	pushl %ebp			#EBP = arg of asmf1

	call asmf2			# asmf1(iv[0][i][j])
	#movl $0xdeadbeaf, %eax		# test
	#movl %ebp, %eax		# test
	#movl %ebp, (iv1ij)
	#movl (iv1ij), %eax		# test
	#movl $iv1ij, %eax
	#movl (%eax), %eax

	popl %ebp			# restore arg
	popl %ebp

	popl %edi
	popl %esi
	popl %edx
	popl %ecx
	popl %ebx
					# old eax left on stack, curr aex is result of  asmf1

	pushl %ebx			# need to be saved, is recomputed in external i_loop (2)

	movl 0x48(%esp), %ebx		# ebx = &ov (0x20 (8th arg) + 0x20 (pushal) + 0x8 = (push eax ebx))
	movl %esi, %ebp	
	shll $0x2, %ebp
	addl %ebp, %ebx			# ebx = 4*i (sizeof ptr(i))
	movl 0x00(%ebx), %ebx		# ebx = ov[i]

	movl %edi, %ebp
	shll $0x2, %ebp
	addl %ebp, %ebx			# ebp = &ov[i][j]
l2res:
	movl %eax, (%ebx)		# ov[i][j] = asmf2(iv[0][i][j], iv[1][i][j])

	popl %ebx			# restored ebx = iv[0][i]  (2)

	popl %eax			# restored eax = iv[0]  (1)

	incl %edi			# j ++
	cmpl %ecx, %edi			# j<x
	jl jloopmode2

	#addl 0x38(%esp), %esi		# i += nt
	incl %esi			# i ++ (currently not thread safe, so only 0th thread is working, and doing all job)
	cmpl %edx, %esi			# j<y
	jl iloopmode2
	
	jmp lmodedone
########				## finished mode2
lmode3:					# mode asmf3
#########
	movl 0x3C(%esp), %eax
	movl 0x00(%eax), %eax		# eax = iv[0]

	movl 0x3C(%esp), %ebx
	movl 0x04(%ebx), %ebx		# ebx = iv[1]
	movl %ebx, iv1			# iv1 = iv[1] 

	movl 0x3C(%esp), %ebx
	movl 0x08(%ebx), %ebx		# ebx = iv[2]
	movl %ebx, iv2			# iv2 = iv[2] 
iloopmode3:				# for (i=ct ...
					# inside i_loop
	movl %esi, %ebx
	shll $0x2, %ebx
	addl %eax, %ebx			
	movl 0x00(%ebx), %ebx		# EBX = iv[0][i]		

	pushl %ebx			# backup ebx

	movl %esi, %ebx
	shll $0x2, %ebx
	addl (iv1), %ebx		# dodaj wartosc z iv1 czyli (iv1) a nie adres iv1 czyli iv1
	movl 0x00(%ebx), %ebx
	movl %ebx, iv1i			# iv1i = iv[1][i]

	movl %esi, %ebx
	shll $0x2, %ebx
	addl (iv2), %ebx		# dodaj wartosc z iv2 czyli (iv2) a nie adres iv2 czyli iv2
	movl 0x00(%ebx), %ebx
	movl %ebx, iv2i			# iv2i = iv[2][i]

	popl %ebx			# restore ebx

	xorl %edi, %edi			# EDI = j = 0
jloopmode3:				# for (j=0 ...
					# inside j_loop
					# USED: ESI, EDI, ECX, EDX, ESP, EAX, EBX
					# AVAIL: EBP
					# iv[0][i][j]
	movl %edi, %ebp
	shll $0x2, %ebp
	addl %ebx, %ebp
	movl 0x00(%ebp), %ebp		# EBP = iv[0][i][j]

	pushl %ebp			# backup ebp

	movl %edi, %ebp
	shll $0x2, %ebp
	addl (iv1i), %ebp
	movl 0x00(%ebp), %ebp
	movl %ebp, iv1ij		# iv1ij = iv[1][i][j]

	movl %edi, %ebp
	shll $0x2, %ebp
	addl (iv2i), %ebp
	movl 0x00(%ebp), %ebp
	movl %ebp, iv2ij		# iv2ij = iv[2][i][j]

	popl %ebp			# restore ebp

l3call:

	pushl %eax			# store what needed iv[0] (1)

	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %esi
	pushl %edi

	pushl (iv2ij)
	pushl (iv1ij)
	pushl %ebp			#EBP = arg of asmf1

	call asmf3			# asmf1(iv[0][i][j])
	#movl $0xdeadbeaf, %eax		# test
	#movl %ebp, %eax		# test
	#movl %ebp, (iv1ij)
	#movl (iv1ij), %eax		# test
	#movl $iv1ij, %eax
	#movl (%eax), %eax

	popl %ebp			# restore arg
	popl %ebp
	popl %ebp

	popl %edi
	popl %esi
	popl %edx
	popl %ecx
	popl %ebx

	#comparer for I_function
/*
	cmpl %ebp, %eax
	jz compare3_ok
compare3_error:
	movl $0x0, %eax
compare3_ok:
*/
					# old eax left on stack, curr aex is result of  asmf1

	pushl %ebx			# need to be saved, is recomputed in external i_loop (2)

	movl 0x48(%esp), %ebx		# ebx = &ov (0x20 (8th arg) + 0x20 (pushal) + 0x8 = (push eax ebx))
	movl %esi, %ebp	
	shll $0x2, %ebp
	addl %ebp, %ebx			# ebx = 4*i (sizeof ptr(i))
	movl 0x00(%ebx), %ebx		# ebx = ov[i]

	movl %edi, %ebp
	shll $0x2, %ebp
	addl %ebp, %ebx			# ebp = &ov[i][j]
l3res:
	movl %eax, (%ebx)		# ov[i][j] = asmf3(iv[0][i][j], iv[1][i][j], iv[2][i][j])

	popl %ebx			# restored ebx = iv[0][i]  (2)

	popl %eax			# restored eax = iv[0]  (1)

	incl %edi			# j ++
	cmpl %ecx, %edi			# j<x
	jl jloopmode3

	#addl 0x38(%esp), %esi		# i += nt
	incl %esi			# i ++ (currently not thread safe, so only 0th thread is working, and doing all job)
	cmpl %edx, %esi			# j<y
	jl iloopmode3
########
lmodedone:				# all computed
	popal				# ESP += 0x20
	ret

asmf2:
stereo:
	movl 0x04(%esp), %eax
	andl $0xff0000, %eax		# eax = red1 on its own position

	movl 0x08(%esp), %ecx
	andl $0xff00, %ecx		# ecx = green2 on its own position

	addl %ecx, %eax			# EAX = RGB(R1, G2, 0)	stereo from 1 and 2 using red and green

	ret
#end of loop_asm

#asmf2:
cdiff:
	movl 0x04(%esp), %eax
	andl $0xff0000, %eax
	shrl $0x10, %eax		# EAX = red1

	movl 0x08(%esp), %ecx
	andl $0xff0000, %ecx
	shrl $0x10, %ecx		# ECX = red2

	subl %ecx, %eax
	cmpl $0x0, %eax
	jge rge0
	imull $-1, %eax
rge0:
	movl %eax, %esi

	movl 0x04(%esp), %eax
	andl $0xff00, %eax
	shrl $0x08, %eax		# EAX = green1

	movl 0x08(%esp), %ecx
	andl $0xff00, %ecx
	shrl $0x08, %ecx		# ECX = green2

	subl %ecx, %eax
	cmpl $0x0, %eax
	jge gge0
	imull $-1, %eax
gge0:
	movl %eax, %edi
	
	movl 0x04(%esp), %eax
	andl $0xff, %eax		# EAX = blue1

	movl 0x08(%esp), %ecx
	andl $0xff, %ecx		# ECX = blue2	

	subl %ecx, %eax
	cmpl $0x0, %eax
	jge bge0
	imull $-1, %eax
bge0:

#	shll $0x1, %esi			# more power
#	shll $0x1, %edi
#	shll $0x1, %eax

#	andl $0xff, %edi
#	andl $0xff, %esi
#	andl $0xff, %eax

	shll $0x10, %esi		# shift rdiff to red
	shll $0x08, %edi		# shift gdiff to green

	addl %esi, %eax
	addl %edi, %eax			# eax = RGB(rdiff, gdiff, bdiff)

	ret

#asmf3:
bitops:
	movl 0x04(%esp), %eax		# EAX = X1
	movl 0x08(%esp), %ecx		# ECX = X2
	movl 0x0C(%esp), %edx		# EDX = X3

	xorl %ecx, %eax
	xorl %edx, %eax

	ret

#asmf3:
sharpen:
	movl 0x08(%esp), %eax		# EAX = X2

	movl %eax, %ecx
	andl $0xff0000, %ecx
	shrl $0x10, %ecx		# ECX = red2

	movl %eax, %edx
	andl $0xff00, %edx
	shrl $0x8, %edx			# EDX = green2

	movl %eax, %esi
	andl $0xff, %esi		# ESI = blue2

	movl %ecx, %edi
	shll $0x1, %ecx
	addl %edi, %ecx			# ECX = 3r2

	movl %edx, %edi
	shll $0x1, %edx
	addl %edi, %edx			# EDX = 3g2

	movl %esi, %edi
	shll $0x1, %esi
	addl %edi, %esi			# ESI = 3b1

	movl 0x04(%esp), %eax		# EAX = X1

	movl %eax, %edi
	andl $0xff0000, %edi
	shrl $0x10, %edi		# EDI = red1
	subl %edi, %ecx			# ECX = 3r2-r1

	movl %eax, %edi
	andl $0xff00, %edi
	shrl $0x8, %edi			# EDI = green1
	subl %edi, %edx			# EDX = 3g2-g1

	movl %eax, %edi
	andl $0xff, %edi		# EDI = blue1
	subl %edi, %esi			# ESI = 3b2-b1


	movl 0x0C(%esp), %eax		# EAX = X3

	movl %eax, %edi
	andl $0xff0000, %edi
	shrl $0x10, %edi		# EDI = red3
	subl %edi, %ecx			# ECX = 3r2-r1-r3

	movl %eax, %edi
	andl $0xff00, %edi
	shrl $0x8, %edi			# EDI = green3
	subl %edi, %edx			# EDX = 3g2-g1-g3

	movl %eax, %edi
	andl $0xff, %edi		# EDI = blue3
	subl %edi, %esi			# ESI = 3b2-b1-b3

	cmpl $0x0, %ecx			# force R to [0,ff]
	jge rmore0
	xorl %ecx, %ecx
	jmp rok
rmore0:
	cmpl $0xff, %ecx
	jle rok
	movl $0xff, %ecx
rok:

	cmpl $0x0, %edx			# force G to [0,ff]
	jge gmore0
	xorl %edx, %edx
	jmp gok
gmore0:
	cmpl $0xff, %edx
	jle gok
	movl $0xff, %edx
gok:

	cmpl $0x0, %esi			# force B to [0,ff]
	jge bmore0
	xorl %esi, %esi
	jmp bok
bmore0:
	cmpl $0xff, %esi
	jle bok
	movl $0xff, %esi
bok:

	shll $0x10, %ecx		# shift to red
	shll $0x08, %edx		# shift to green

	movl %ecx, %eax
	addl %edx, %eax
	addl %esi, %eax			# eax = RGB(srp_r, shr_b, srp_g)

	ret

asmf3:
gsalien_rgbtail:
	pushl %ebx

	movl 0x08(%esp), %eax		# EAX = X1

	movl %eax, %ebx
	andl $0xff0000, %ebx
	shrl $0x10, %ebx		# EBX = red1

	movl %eax, %ecx
	andl $0xff00, %ecx
	shrl $0x8, %ecx			# ECX = green1

	movl %eax, %edx
	andl $0xff, %edx		# EDX = blue1

	movl %ebx, %eax
	addl %ecx, %eax
	addl %edx, %eax			# EAX = GS1 [0, 765)

	cmpl $0x0FF, %eax
	jle x1done
	cmpl $0x1FE, %eax
	jle x1middle
x1high:
	subl $0x1FE, %eax
	jmp x1done

x1middle:
	movl %eax, %ebx
	movl $0x1FF, %eax
	subl %ebx, %eax
	
x1done:					# EAX = GSalien1

	movl %eax, %esi			# ESI = GSalien1

	movl 0x0C(%esp), %eax		# EAX = X2

	movl %eax, %ebx
	andl $0xff0000, %ebx
	shrl $0x10, %ebx		# EBX = red2

	movl %eax, %ecx
	andl $0xff00, %ecx
	shrl $0x8, %ecx			# ECX = green2

	movl %eax, %edx
	andl $0xff, %edx		# EDX = blue2

	movl %ebx, %eax
	addl %ecx, %eax
	addl %edx, %eax			# EAX = GS2 [0, 765)

	cmpl $0x0FF, %eax
	jle x2done
	cmpl $0x1FE, %eax
	jle x2middle
x2high:
	subl $0x1FE, %eax
	jmp x2done

x2middle:
	movl %eax, %ebx
	movl $0x1FF, %eax
	subl %ebx, %eax
	
x2done:					# EAX = GSalien2

	movl %eax, %edi			# EDI = GSalien2

	movl 0x10(%esp), %eax		# EAX = X3

	movl %eax, %ebx
	andl $0xff0000, %ebx
	shrl $0x10, %ebx		# EBX = red3

	movl %eax, %ecx
	andl $0xff00, %ecx
	shrl $0x8, %ecx			# ECX = green3

	movl %eax, %edx
	andl $0xff, %edx		# EDX = blue3

	movl %ebx, %eax
	addl %ecx, %eax
	addl %edx, %eax			# EAX = GS3 [0, 765)

	cmpl $0x0FF, %eax
	jle x3done
	cmpl $0x1FE, %eax
	jle x3middle
x3high:
	subl $0x1FE, %eax
	jmp x3done

x3middle:
	movl %eax, %ebx
	movl $0x1FF, %eax
	subl %ebx, %eax
	
x3done:					# EAX = GSalien3

#	shrl $0x1, %esi			# red 75%
#	movl %esi, %ebx
#	shrl $0x1, %ebx
#	addl %ebx, %esi

	movl %esi, %ecx	
	shrl $0x2, %ecx
	subl %ecx, %esi

	movl %edi, %ecx
	shrl $0x1, %ecx
	subl %ecx, %edi

#	movl %eax, %ecx
#	shrl $0x2, %ecx
#	subl %ecx, %eax

	shll $0x10, %esi		# shift GSs1 to red
	shll $0x08, %edi		# shift GSs2 to green

	addl %esi, %eax
	addl %edi, %eax			# eax = RGB(GSa1, GSa2, GSa3)

	popl %ebx

	ret
#asmf1:
heat:
	movl  0x04(%esp), %eax		# eax = X

	pushl %ebx

	movl %eax, %ebx
	andl $0xff0000, %ebx
	shrl $0x10, %ebx		# ebx = red

	movl %eax, %ecx
	andl $0xff00, %ecx
	shrl $0x8, %ecx			# ecx = green

	movl %eax, %edx
	andl $0xff, %edx		# edx = blue

	movl %ebx, %eax
	addl %ecx, %eax
	addl %edx, %eax			# eax = GS [0, 765)

	cmpl $110, %eax
	jg l2
l0:
	#<110
	movl %eax, %ebx			# r: 0 --> 110
	xorl %ecx, %ecx			# g: 0
	movl %eax, %edx
	shll $1, %edx			# b: 0 --> 220
	jmp ldone
l2:
	cmpl $220, %eax
	jg l3
	#<220
	subl $110, %eax			# eax: 0 --> 110
	movl $110, %ebx		
	subl %eax, %ebx			# r: 110 --> 0
	xorl %ecx, %ecx			# g: 0
	movl $220, %edx			# b: 220

	jmp ldone
l3:
	cmpl $330, %eax
	jg l4
	#<330
	subl $220, %eax			# eax: 0 --> 110
	xorl %ebx, %ebx			# r: 0
	movl %eax, %ecx
	shll $1, %ecx			# g: 0 --> 220
	movl $220, %edx			# b: 220
	jmp ldone
l4:
	cmpl $440, %eax
	jg l5
	#<440
	subl $330, %eax			# eax: 0 -->110
	xorl %ebx, %ebx			# r: 0
	movl $220, %ecx			# g: 220
	movl $220, %edx
	subl %eax, %edx
	subl %eax, %edx			# b: 220 --> 0
	jmp ldone
l5:
	cmpl $550, %eax
	jg l6
	#<550
	subl $440, %eax			# eax: 0 --> 110
	movl %eax, %ebx
	shll $1, %ebx			# r: 0 --> 220
	movl $220, %ecx			# g: 220
	xorl %edx, %edx			# b: 0
	jmp ldone
l6:
	cmpl $660, %eax
	jg l7
	#<660
	subl $550, %eax			# eax: 0 --> 110
	movl $220, %ebx			# r: 220
	movl $220, %ecx
	subl %eax, %ecx
	subl %eax, %ecx			# g: 220 --> 0
	xorl %edx, %edx			# b: 0
	jmp ldone
l7:
	#>=660
	subl $660, %eax			# eax: 0 --> 110
	movl $220, %ebx			# r: 220
	movl %eax, %ecx
	shll $1, %ecx			# g: 0 --> 220
	movl %eax, %edx
	shll $1, %edx			# b: 0 --> 220
ldone:
	shll $0x8, %ecx
	shll $0x10, %ebx

	movl %ebx, %eax
	addl %ecx, %eax
	addl %edx, %eax			# eax = RGB(r, g, b)

	popl %ebx

	ret

asmf1:
maxred:
	movl  0x04(%esp), %eax		# eax = X

	pushl %edi

	movl %eax, %edx
	andl $0xff0000, %edx
	shrl $0x10, %edx		# edx = red

	movl %eax, %ecx
	andl $0xff00, %ecx
	shrl $0x7, %ecx			# ecx = 2*green

	movl %eax, %edi
	andl $0xff, %edi		# edi = blue

	addl %ecx, %edx
	addl %edi, %edx			# edx = r+2g+b

	shrl $0x1, %edx			# edx = 1/2r+g+1/2b = [0,511)

	xorl %edi, %edi			# edi = nie ma niedomiaru

	cmpl $0xff, %edx
	jl mrdone

	movl %edx, %edi
	subl $0xff, %edi		# edi = nadmiar
	movl $0xff, %edx
mrdone:
					# ecx = poziom red
	
	movl %edi, %eax			# eax = nadmiar blue
	shll $0x8, %edi
	addl %edi, %eax			# eax = nadmiar blue i green

	shll $0x10, %edx
	addl %edx, %eax			# eax = RGB(poziom R, nadmiar G i B)

	popl %edi

	ret

#asmf1:
invmul:
	movl  0x04(%esp), %eax		# eax = X

	pushl %ebx

	movl %eax, %ebx
	andl $0xff0000, %ebx
	shrl $0x10, %ebx		# ebx = red

	movl %eax, %ecx
	andl $0xff00, %ecx
	shrl $0x8, %ecx			# ecx = green

	movl %eax, %edx
	andl $0xff, %edx		# edx = blue

	movl $0xff, %eax
	subl %ebx, %eax
	mov %eax, %ebx			# red = 255 - red

	movl $0xff, %eax
	subl %ecx, %eax
	mov %eax, %ecx			# green = 255 - green

	movl $0xff, %eax
	subl %edx, %eax
	mov %eax, %edx			# blue = 255 - blue

	pushl %esi			# store registers
	pushl %edi

	movl %ecx, %esi
	movl %edx, %eax
	imull %esi, %eax
	shrl $0x8, %eax			# eax = g * b

	movl %ebx, %esi
	movl %edx, %edi
	imull %esi, %edi
	shrl $0x8, %edi			# edi = r * b

	movl %ebx, %esi
	imull %ecx, %esi
	shrl $0x8, %esi			# esi = r * g

	movl %eax, %ebx			# recollect r, g, b
	movl %edi, %ecx
	movl %esi, %edx

	popl %edi			# restore registers
	popl %esi

	shll $0x8, %ecx
	shll $0x10, %ebx

	movl %ebx, %eax
	addl %ecx, %eax
	addl %edx, %eax			# eax = RGB(r, g, b)

	popl %ebx

	ret

