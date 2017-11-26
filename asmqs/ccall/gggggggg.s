.data				#compiler data
dane:				#sekcja danych
	.long 0x0,0x0,0x0,0x0,0x0,0x0	#adresy zmiennych
        /*dptr,i,j,v,tmp,stack,func*/	#tylko 20 bajtow RAM!
.globl qsortasm__FiiPvPFPvPv_i	#global entry for c++
.text				#sekcja textu
	.align 0x4
qsortasm__FiiPvPFPvPv_i:	#go!
qsorta:
	popl dane+0x14		#adres powrotu asemblera
	popl %eax		#l=eax
	popl %ebx		#r=ebx
	popl dane		#adres danych
	popl dane+0x18		#funkcja
	movl %eax, dane+0x4	#i=l   |po prawej stronie nawiasy
	movl %ebx, dane+0x8	#j=r   |obojetne czy nawiasy czy nie
	incl dane+0x8		#j=r+1 |tu tez bo nie da sie zwiekszac
	movl (dane+0x4), %ecx	#ecx=i |adresu, ale tu istotne
	shll $2, %ecx		#ecx=4 | czy przeniesc adres czy dana
	addl (dane), %ecx	#ecx=A[i]=A[l]
	movl %ecx, dane+0xc	#v=A[i,l] | obojetne (dane) czy dane
	movl %ecx, dane+0x10	#tmp=A[i] (zachowa to nawet po while)
while_loop:			#petla while
	movl (dane+0x10), %ecx	#ecx=tmp (tmp po wyjsciu z petli ma A[i])
				#oczywiscie chodzi o adres :)
	movl (dane+0xc), %edx	#edx=v=A[l]
loop_i:				#petla i
	incl dane+0x4		#i++;
	addl $4, %ecx		#A[i]=A[i+1] ecx zawiera adres!
	#cmpl (dane+0x4), %ebx	#porownaj prawy z aktualnym i
	#jl end_loop_i		#jesli poza tabela zakoncz
	movl (%ecx), %edi	#w edi przetrzymaj WARTOSC z ecx
	cmpl (%edx), %edi	#porownaj edi z WARTSCIA z edx
	jl loop_i 		#petla dopuki A[i]<v (edi<edx)
end_loop_i:			#koniec petli i
	movl %ecx, dane+0x10	#tmp=address A[i]
	movl (dane+0x8), %ecx	#ecx=j
	shll $2, %ecx		#ecx=4j 32bity!
	addl (dane), %ecx	#ecx=A[j]
loop_j:				#petla j
	decl dane+0x8		#j--
	subl $4, %ecx		#A[j]=A[j-1] ecx zawiera adres!
	#cmpl (dane+0x8), %eax	#porownaj lewy z aktualnym j
	#jg end_loop_j		#jesli przed tablela zakoncz petle
	movl (%ecx), %edi	#w edi WARTOSC z ecx
	cmpl (%edx), %edi	#porownaj edi z wartoscia z edx
	jg loop_j		#petla dopuki A[j]<v (edi<edx)
end_loop_j:			#koniec petli j
 	movl (dane+0x4), %esi	#esi=wartosc i
	cmpl (dane+0x8), %esi	#porownaj (po wartsciach) i z j
	jl swap			#i<j (esi<dane+8) swap
	jmp continue		#inaczej omin zamiane
swap:				#zamiana danych
	movl (%ecx), %edx	#edx = wartosc z ecx czyli A[j]
				#dawniej edx byl A[l]=v
	movl (dane+0x10), %edi	#edi = adres danej A[i]
	movl (%edi), %edi	#edi = wartosc A[i]
	movl %edi, (%ecx)	#zapisz pod adres ecx(A[j]) wart A[i]
				#czyli (wart) A[j] = A[i]
	movl (dane+0x10), %edi	#edi = adres A[i]
	movl %edx, (%edi)	#pod adres A[i] zapisz edx czyli A[j]
				#czyli (wart) A[i] = A[j] A[i] <-> A[j]
continue:			#po zamianie danych
	movl (dane+0x4), %edi	#edi = i
	cmpl (dane+0x8), %edi	#porownaj (po wartosciach) i z j
	jl while_loop		#while (i<j) {....}
	movl (%ecx), %edx	#edx=co wslazuje ecx=A[j] (adres)
	movl (dane+0xc), %edi	#edi=v=A[l] (adres)
	movl (%edi), %edi	#edi=A[l] wartosc
	movl %edi, (%ecx)	#A[j]=v (po wartosciach)
				#wstaw wartosc pod adres
	movl (dane+0xc), %edi	#edi=A[l] (adres)
	movl %edx, (%edi)	#A[l]=edx=A[j] (po wartosciach)
				#czyli swap A[l] <-> A[j]
recurse:			#zaczynamy rekursje :)
	decl dane+0x8		#j--
	cmpl (dane+0x8), %eax	#porownaj j-1 z lewym
	jl qsort_left		#jesli j-1>l sortuj lewa
ret_from_left:			#po qsort_left
	addl $2, dane+0x8	#j+=2
	cmpl (dane+0x8), %ebx	#porownaj j+1 z prawym
	jg qsort_right		#jesli j+1<r sortuj prawa
	jmp endcode		#zakoncz algorytm
qsort_left:			#sortuj lewa strone
	pushl %eax		#zapisz lewy
	pushl %ebx		#prawy
	pushl (dane+0x8)	#oraz j
	pushl (dane+0x14)	#adres powrotu
	pushl (dane+0x18)	#funkcja powrotu
	pushl (dane)		#wyslij stos dla wywolania rekurencyjnego
	pushl (dane+0x8)	#adres danych r=j+1 i l=l
	pushl %eax		#w odwrotnej kolejnosci
	call qsorta		#wywolanie rekurencyjne
	addl $16, %esp		#wyczysc stos
	popl dane+0x14		#wczytaj adres powrotu
	popl dane+0x8		#wczytaj j
	popl %ebx		#odzyskaj ebx=prawy
	popl %eax		#eax=lewy
	jmp ret_from_left	#powroc na swoje miejsce
qsort_right:			#sortuj prawa strone
	pushl (dane+0x14)	#zapisz adres powrotu
	pushl (dane+0x18)
	pushl (dane)		#wyslij adres danych
	pushl %ebx		#r=ebx r=r
	pushl (dane+0x8)	#l=j+1
	call qsorta		#wywolanie rekurencyjne
	addl $16, %esp		#wyczysc stos
	popl dane+0x14		#przypomnij sobie adres powrotu
endcode:			#zakoncz zabawe
	subl $16, %esp		#clean stack
	pushl (dane+0x14)	#gdzie powrocic!
	ret			#return eax
