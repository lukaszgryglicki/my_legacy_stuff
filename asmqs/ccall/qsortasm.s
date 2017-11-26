section .data				
	dptr   dd 0x00000000		
	i      dd 0x00000000		
	tmp    dd 0x00000000		
	tmpv   dd 0x00000000		
	v      dd 0x00000000	
	j      dd 0x00000000		
	global qsortasm__FiiPi		
section .text				
qsortasm__FiiPi:			;globalna funkcja
	pop eax
startcode:				;start kodu funkcji
	pop eax				;EAX=lewy
	pop ebx				;EBX=prawy
	pop dword [dptr]		;dptr=(int*)table
	mov dword [i], eax		;i=l
	mov dword [j], ebx		;j=r
	inc dword [j]			;j++
	mov ecx, [dptr]			;ECX=A
	add ecx, dword [i]		;ECX=A[i]=A[l]
	add ecx, dword [i]		;*4 poniewaz pointer to 32bity
	add ecx, dword [i]		;tzn 4 bajty
	add ecx, dword [i]		;adresowanie
	mov dword [v], ecx		;v=ECX=A[l]
while_loop:				;dopuki i<j
	mov ecx, dword [v]		;potrzebne w petli :-(
	mov edx, ecx			;EDX=A[l]
loop_i:					;powtarzaj po i
	inc dword [i]			;i++
	add ecx,4			;A[i]=A[i+1] (porownuj z nast.)
	cmp ebx, dword [i]		;porownaj prawy z aktualnym i
	jl end_loop_i 			;jesli przekroczony zakres to koniec
	mov edi, [ecx]			;edi przechowa dane z adresu ecx A[i]
	cmp edi, dword [edx]		;porownaj A[i] z v
	jl loop_i			;jesli A[i]<v skocz do petli i
end_loop_i:				;koniec petli po i
	mov dword [tmp], ecx		;tmp=address A[i]
	mov ecx, [dptr]			;ECX=A
	add ecx, dword [j]		;ECX=A[j]
	add ecx, dword [j]		;*4 poniewaz wskazniki sa 32 bitowe
	add ecx, dword [j]		;a bajt to 8 bitow
	add ecx, dword [j]		;wiec mamy adresowanie
loop_j:					;petla po j
	dec dword [j]			;j--
 	sub ecx, 4			;A[j]=A[j-1]
	cmp eax, dword [j]		;porownaj lewy z aktualnym j
	jg end_loop_j			;jesli overflow to skocz do end
	mov edi, [ecx]			;edi przechowa aktualna wart tablicy
	cmp edi, dword [edx]		;porownaj A[j] z v
	jg loop_j			;jesli A[j]>v skocz do petli j
end_loop_j:				;koniec petli po j
	mov esi, dword [i]		;zachowaj i w esi
	cmp esi, dword [j]		;porownaj i z j
	jl swap				;jesli i<j zamien
	jmp continue			;inaczej kontynuuj
swap:					;zamien A[i] <-> A[j]
	mov edx, [ecx]			;zapamietaj w edx dana w A[j]
	mov edi, dword [tmp]		;zapamietaj w edi adres A[i]
	mov edi, [edi]			;edi staje sie dana w A[i]
	mov [ecx], edi			;zapisz pod adres A[j] dana z A[i]
	mov edi, dword [tmp]		;edi = adres A[i]
	mov [edi], edx			;zapisz w A[i] dana z A[j]
continue:				;kontynuuj po zamianie
	mov edi, dword [i]		;przenies do edi i
	cmp edi, dword [j]		;porownaj i z j
	jl while_loop			;jesli i<j skocz do while_loop
	mov edx, [ecx]			;zapamietaj w edx dana z ecx (A[j])
	mov edi, dword [v]		;edi = adres A[l]
	mov edi, [edi]			;edi staje sie dana z A[l]
	mov [ecx], edi			;A[l] = v
	mov edi, dword [v]		;adi = adres A[l]
	mov [edi], edx			;zapisz w A[l] dana z v
recurse:				;rekursja
	dec dword [j]			;j--
	cmp eax, dword [j]		;prownaj l z j
	jl  qsort_left			;jezli j-1>l qsort left
ret_from_left:				;powroc z lewej
	add dword [j], 2		;j+=2
	cmp ebx, dword [j]		;jezeli j+1<r qsort right
	jg qsort_right			;sortuj prawa czesc
ret_from_right:				;powroc z prawej
	jmp endcode			;zakoncz rekursje
qsort_left:				;sortuj lewa
	push eax			;zapisz eax (lewy)
	push ebx			;zapisz ebx (prawy)
	push dword [j]			;zapisz j
	push dword [dptr]		;wyslij adres danych
	push dword [j]			;wyslij prawy jako (j-1)
	push eax			;wyslij lewy (oryginalna wartosc)
	jmp startcode			;zawolaj funkcje call-back (powinna
					;uporzadkowac po sobie stos)
	pop dword [j]			;odczytaj j
	pop ebx				;odczytaj ebx (prawy)
	pop eax				;odczytaj eax (lewy)
	jmp ret_from_left		;powroc z rekursji lewej strony
qsort_right:
	push eax
	mov eax, ' qsr'
	;call put_pixel
	pop eax
	push dword [dptr]		;wyslij adres danych
	push ebx			;wyslij prawy
	push dword [j]			;wyslij lewy (jako j+1)
	jmp startcode			;zawolaj funkcje call-back powinna
					;uporzadkowac po sobie stos
	jmp ret_from_right
endcode:				;koniec kodu
	sub esp, 20			;uporzadkuj stos
	ret				;powroc do c++
