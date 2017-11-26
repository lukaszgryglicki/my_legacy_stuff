section .text
global sCPU__Fi
global sCPU

sCPU:			;sumowanie
sCPU__Fi:		;2 instr
	pop eax
	pop eax		;4instr
a:
	dec eax		;4+3*stack
	cmp eax,0
	jnz a
	sub esp,8	;5+3*stack 
	ret		;6+3*stack
			;+call-ret 8+3*stack instructions

