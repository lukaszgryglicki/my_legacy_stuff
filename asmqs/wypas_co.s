.data				#compiler data
dane:				#sekcja danych
	.long 0x0,0x0,0x0,0x0,0x0	#adresy zmiennych 
        /*dptr,i,j,v,tmp,stack*/	#tylko 20 bajtow RAM!
tabl:
	.long 0x0,4,3,2,5,1,3,6,5,7,8,4,0,7	#dane do sortowania
					#pierwsze 0x0 to wskaznik na tabele
					#zeby nie adresowac etykiety
	#wyniki zobacz w gdb breakpoint po call qsortasm
	#w _start sprawdz adres print &dane = addr
	#print *(*addr+k*0x4), dla k e {0,...,12}
	#np print *(*0x80491e4+0x2c) beda posortowane
	#skompiluj z debugiem as --gstabs -o wypas.o wypas.s
	#ld -e _start -o wypas wypas.o (by MorgothDBMA) 
.globl qsortasm__FiiPi		#global entry for c++
.globl _start			#wejscie dla linkera ld
.text				#sekcja textu
_start:				#wejscie linkera
	.align 0x4		#wyrownaj do 32bitow
	leal (tabl+0x4), %eax	#jako dane podaj adres tabeli int
	pushl %eax		#odloz ten adres dla f-cji
	pushl $0xc		#tablica ma 13 elementow 0xc+1 
	pushl $0x0		#gdyz od 0 do 12
	call qsortasm__FiiPi	#wywolaj qsort
	addl $12, %esp		#wyczysc stos
 	movl $1, %eax		#funkcja systemowa 1(sys_exit)
	pushl $0		#zwroc 0 bledow
	pushl %eax		#fikcyjny adres dla kernela
	int $0x80		#zawolaj jadro
	ret			#say goodbye!
qsortasm__FiiPi:		#go!
	.align 0x4		#do 32bitow
	popl dane+0x14		#adres powrotu asemblera
	popl %eax		#l=eax
	popl %ebx		#r=ebx
	popl dane		#adres danych
	movl %eax, dane+0x4	#i=l   |po prawej stronie nawiasy
	movl %ebx, dane+0x8	#j=r   |obojetne czy nawiasy czy nie
	incl dane+0x8		#j=r+1 |tu tez bo nie da sie zwiekszac
	movl (dane+0x4), %ecx	#ecx=i |adresu, ale tu istotne
	shll $2, %ecx		#ecx=4 | czy przeniesc adres czy dana
	addl (dane), %ecx	#ecx=A[i]=A[l]
	movl %ecx, dane+0xc	#v=A[i,l] | obojetne (dane) czy dane
	movl %ecx, dane+0x10	#tmp=A[i] (zachowa to nawet po while)
while_loop:			#petla while
	.align 0x4
	movl (dane+0x10), %ecx	#ecx=tmp (tmp po wyjsciu z petli ma A[i])
				#oczywiscie chodzi o adres :)
	movl (dane+0xc), %edx	#edx=v=A[l]
loop_i:				#petla i
	.align 0x4
	incl dane+0x4		#i++;
	addl $4, %ecx		#A[i]=A[i+1] ecx zawiera adres!
	#cmpl (dane+0x4), %ebx	#porownaj prawy z aktualnym i
	#jl end_loop_i		#jesli poza tabela zakoncz
	movl (%ecx), %edi	#w edi przetrzymaj WARTOSC z ecx
	cmpl (%edx), %edi	#porownaj edi z WARTSCIA z edx
	jl loop_i 		#petla dopuki A[i]<v (edi<edx)
end_loop_i:			#koniec petli i
	.align 0x4
	movl %ecx, dane+0x10	#tmp=address A[i]
	movl (dane+0x8), %ecx	#ecx=j
	shll $2, %ecx		#ecx=4j 32bity!
	addl (dane), %ecx	#ecx=A[j]
loop_j:				#petla j
	.align 0x4
	decl dane+0x8		#j--
	subl $4, %ecx		#A[j]=A[j-1] ecx zawiera adres!
	#cmpl (dane+0x8), %eax	#porownaj lewy z aktualnym j
	#jg end_loop_j		#jesli przed tablela zakoncz petle
	movl (%ecx), %edi	#w edi WARTOSC z ecx
	cmpl (%edx), %edi	#porownaj edi z wartoscia z edx
	jg loop_j		#petla dopuki A[j]<v (edi<edx)
end_loop_j:			#koniec petli j
	.align 0x4
 	movl (dane+0x4), %esi	#esi=wartosc i
	cmpl (dane+0x8), %esi	#porownaj (po wartsciach) i z j
	jl swap			#i<j (esi<dane+8) swap
	jmp continue		#inaczej omin zamiane
swap:				#zamiana danych
	.align 0x4
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
	.align 0x4
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
	.align 0x4
	decl dane+0x8		#j--
	cmpl (dane+0x8), %eax	#porownaj j-1 z lewym
	jl qsort_left		#jesli j-1>l sortuj lewa
ret_from_left:			#po qsort_left
	.align 0x4
	addl $2, dane+0x8	#j+=2
	cmpl (dane+0x8), %ebx	#porownaj j+1 z prawym
	jg qsort_right		#jesli j+1<r sortuj prawa
	jmp endcode		#zakoncz algorytm
qsort_left:			#sortuj lewa strone
	.align 0x4
	pushl %eax		#zapisz lewy
	pushl %ebx		#prawy
	pushl (dane+0x8)	#oraz j
	pushl (dane+0x14)	#adres powrotu
	pushl (dane)		#wyslij stos dla wywolania rekurencyjnego
	pushl (dane+0x8)	#adres danych r=j+1 i l=l
	pushl %eax		#w odwrotnej kolejnosci
	call qsortasm__FiiPi	#wywolanie rekurencyjne 
	addl $12, %esp		#wyczysc stos
	popl dane+0x14		#wczytaj adres powrotu
	popl dane+0x8		#wczytaj j
	popl %ebx		#odzyskaj ebx=prawy
	popl %eax		#eax=lewy
	jmp ret_from_left	#powroc na swoje miejsce
qsort_right:			#sortuj prawa strone
	.align 0x4
	pushl (dane+0x14)	#zapisz adres powrotu
	pushl (dane)		#wyslij adres danych
	pushl %ebx		#r=ebx r=r
	pushl (dane+0x8)	#l=j+1
	call qsortasm__FiiPi	#wywolanie rekurencyjne
	addl $12, %esp		#wyczysc stos
	popl dane+0x14		#przypomnij sobie adres powrotu
endcode:			#zakoncz zabawe
	.align 0x4
	subl $12, %esp		#clean stack
	pushl (dane+0x14)	#gdzie powrocic!
	ret			#return eax
