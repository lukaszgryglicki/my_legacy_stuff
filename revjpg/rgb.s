.globl _Z6ARGB24iii
.globl _Z6ARGB16iii
.globl ARGB24__Fiii
.globl ARGB16__Fiii
.text
_Z6ARGB24iii:
ARGB24__Fiii:
	pushl %edx			#zachowaj bez tego niebezpiecznie
	pushl %ecx			#ale ryzykujemy //FIXME
	movl  0x0C(%esp), %eax		#wez R
	movl  0x10(%esp), %edx		#wez G
	movl  0x14(%esp), %ecx		#wez B
	#andl $0xFF, %eax		#r%0xff wszystko dla optymalizacji
	#andl $0xFF, %edx		#g%0xff darujemy sobie, ale to powoduje ze wywolania
	#andl $0xFF, %ecx		#b%0xff RGB(>255,..) sa niepoprawne, nie uzywam takich
	shll $0x10, %eax		#r*0x10000
	shll $0x08, %edx		#g*0x100
	addl %edx, %eax			#result=r+g
	addl %ecx, %eax			#result=r+g+b RETURM in EAX
	popl %ecx			#odzyskaj zachowne rejestry
	popl %edx			#nie uzywane... dla szybkosci
	ret				#wroc do C++
_Z6ARGB16iii:				#UWAGA //FIXME maksymalnie zoptymalizowany patrz wyzej!!
ARGB16__Fiii:
	pushl %edx			#zachowaj bez tego niebezpiecznie
	pushl %ecx			#ale ryzykujemy //FIXME
	movl  0x0C(%esp), %eax		#wez R
	movl  0x10(%esp), %edx		#wez G
	movl  0x14(%esp), %ecx		#wez B
	shrl $0x03, %eax		#r/=8
	shrl $0x02, %edx		#g/=4
	shrl $0x03, %ecx		#b/=8
	shll $0x0b, %eax		#r*0x800
	shll $0x05, %edx		#g*0x20
	addl %edx, %eax			#result=r+g nie biore (r%0x20, g%0x40,b%0x20) patrz ARGB24
	addl %ecx, %eax			#result=r+g+b RETURM in EAX
	popl %ecx			#odzyskaj zachowne rejestry
	popl %edx			#nie uzywane... dla szybkosci
	ret
