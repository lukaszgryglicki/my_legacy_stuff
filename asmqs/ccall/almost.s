.data				#compiler data
dane:				#sekcja danych
	.long 0x0,0x0,0x0,0x0,0x0,0x0,0x0 #adresy zmiennych
        #dptr,i,j,v,tmp,stack,func	tylko 20 bajtow RAM!
.globl qsortasm__FPvPFPCvPCv_iUiUi	#global entry for c++
.text				#sekcja textu
			#poza rekurencje dane i funkcja
	.align 0x4		#wyrownaj do 32 bitow
cmpstr:				#orownaj dane w pamieci
	popl %esi		#ignoruj adres powrotu
	popl %esi		#adres str1
	popl %edi		#adres str2
petla:				#czy trzeba clear ax w petli ???
	#xor %ax, %ax
	lodsb
	movb %al, %ah
	movl %esi, %edx
	movl %edi, %esi
	lodsb
	movl %edx, %edi
	cmpb $0x0, %ah
	je done_cmp
	cmpb $0x0, %al
	je done_cmp
	cmpb %ah, %al
	je petla
done_cmp:
	cmpb %ah, %al
	jle set_register
	xorl %eax, %eax
	jmp ret_from_cmp
set_register:
	movl $0x1, %eax
ret_from_cmp:
	subl $0xc, %esp		#oczysc stos
	ret			#powroc w miejsce wywolania

qsortasm__FPvPFPCvPCv_iUiUi:	#go!
	popl dane+0x14		#glupawy adres powrotu zapamietaj
	popl dane		#adres danych globalny!
	popl dane+0x18		#funkcja globalna!
	leal cmpstr, %eax	#zaladuj adres funkcji asemblera
	movl %eax, dane+0x18	#wyslij ten adres qsort'owi
	popl %eax		#lewy dla qsorta
	popl %ebx		#prawy dla qsorta trzeba sie dokopac
	pushl (dane+0x14)	#adres powrotu bo qsorta niezle
	pushl %ebx		#manipuluje stosem
	pushl %eax		#poloz lewy i prawy duzo istrukcji
	call qsorta		#wywolaj szybsza wersje (ale tylko raz)
	addl $0x8, %esp		#wykonane, wyczysc stos po qsorta
	popl dane+0x14		#wrzuc na stos adres powrotu
	subl $0x4, %esp		#pierwszy pop sie nie liczy =0
	ret		#nastepnie 2 popy globalne =8 zdeejmnij l,p =16
qsorta:			#poloz addr_ret =12 l,r dla funkcji =4 funkcja
	.align 0x4	#posprzata to =12 pop addr_ret =8 subl =4 zostal
			#tylko adres powrotu do C++ wiec po ret =0 [OK]
	popl dane+0x14		#adres powrotu asemblera
	popl %eax		#l=eax
	popl %ebx		#r=ebx
	movl %eax, dane+0x4	#i=l   |po prawej stronie nawiasy
	movl %ebx, dane+0x8	#j=r   |obojetne czy nawiasy czy nie
	incl dane+0x8		#j=r+1 |tu tez bo nie da sie zwiekszac
	movl (dane+0x4), %ecx	#ecx=i |adresu, ale tu istotne
	shll $0x2, %ecx		#ecx=4 | czy przeniesc adres czy dana
	addl (dane), %ecx	#ecx=A[i]=A[l]
	movl %ecx, dane+0xc	#v=A[i,l] | obojetne (dane) czy dane
	movl %ecx, dane+0x10	#tmp=A[i] (zachowa to nawet po while)
while_loop:			#petla while
	.align 0x4
	movl (dane+0x10), %ecx	#ecx=tmp (tmp po wyjsciu z petli ma A[i])
				#oczywiscie chodzi o adres :)
	movl (dane+0xc), %edx	#edx=v=A[l]
	pushl %eax		#zapisz 4 odstawowe rejestry
loop_i:				#petla i
	.align 0x4
	incl dane+0x4		#i++;
	addl $0x4, %ecx		#A[i]=A[i+1] ecx zawiera adres!
	cmpl (dane+0x4), %ebx	#porownaj prawy z aktualnym i
	jl end_loop_i		#jesli poza tabela zakoncz
	pushl %ecx		#rozne f-cje C/C++ moga
	pushl %ebx		#je modyfikowac
	pushl %edx		#jest to konieczne gdyz
	pushl (%edx)		#porownaj V z...
	pushl (%ecx)		#A[i]
	call *(dane+0x18)	#funkcja zewnetrzna z C/C++ lub
			#trzeba jeszcze napisac funkcje porownujaca
			#stringi, ultraszybka w assemblerze :-)
			#oraz przykladowy program w calosci w ASM
			#robiacy qsortasm na tekstach i wypisujacy je
			#przerobic f-cje z link.c cmp_pchar_func
			#i ewentualnie wysylac jako adres :-)
	addl $0x8, %esp		#z assemblera i wyczysc stos
	movl %eax, %edi		#niech edi przechowa wynik porownania
	popl %edx		#ecx-adres, edx-adres
	popl %ebx		#tymczasem odzyskajmy rejestry
	popl %ecx		#aby program dzilal poprawnie dalej
	cmpl $0x0, %edi		#jesli A[i]<v to kontynuuj petle
	jz loop_i		#dane sa abstrakcyjne
end_loop_i:			#koniec petli i
	.align 0x4		#wyrownaj do 32 bitow
	popl %eax		#eax,ebx lewy-prawy
	movl %ecx, dane+0x10	#tmp=address A[i]
	movl (dane+0x8), %ecx	#ecx=j
	shll $0x2, %ecx		#ecx=4*j  ==> 32bity!
	addl (dane), %ecx	#ecx=A[j]
	pushl %ebx		#procka
loop_j:				#petla j
	.align 0x4		#WOW!
	decl dane+0x8		#j--
	subl $0x4, %ecx		#A[j]=A[j-1] ecx zawiera adres!
	cmpl (dane+0x8), %eax	#porownaj lewy z aktualnym j
	jg end_loop_j		#jesli przed tablela zakoncz petle
	pushl %eax		#zapisz rejestry
	pushl %ecx		#aby je potem
	pushl %edx		#odtworzyc
	pushl (%ecx)		#wyslij dane do porownywania
	pushl (%edx)		#to moga byc np wskazniki lub klasy
	call *(dane+0x18)	#TA wlanie funkcja musi wiedziec jak
	addl $0x8, %esp		#je porownac i oczyscic stos przy powrocie
	movl %eax, %edi		#kod porownania w edi
	popl %edx		#odzyskaj rejestry zamiana
	popl %ecx		#ewentualna bedzie tylko po wskaznikach
	popl %eax		#rejestry odzyskane
	cmpl $0x0, %edi		#jesli A[j]>v kontynuuj petle...
	jz loop_j		#i wszystko gra :-)
end_loop_j:			#koniec petli j
	.align 0x4
	popl %ebx		#(void*) z C nalezy je rzutowac :-)
 	movl (dane+0x4), %esi	#esi=wartosc i
	cmpl (dane+0x8), %esi	#porownaj (po wartsciach) i z j
	jl swap			#i<j (esi<dane+8) swap
	jmp continue		#inaczej omin zamiane
swap:				#zamiana danych (FIXME: wolna :-( )
	.align 0x4
		#dla dowolnych danych, zamiana wskaznikow pod adresami
		#tu kod moznaby przyspieszyc uzywajac instr ==> xchg
		#ale nie wiem jak :-(
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
		#podobnie jak w swap kod zamiany A[l] <=> A[j]
		#tu kod moznaby przyspieszyc uzywajac instr ==> xchg
		#ale nie wiem jak :-(
	.align 0x4		#FIXME wolny kod!! :-(
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
	addl $0x2, dane+0x8	#j+=2
	cmpl (dane+0x8), %ebx	#porownaj j+1 z prawym
	jg qsort_right		#jesli j+1<r sortuj prawa
	jmp endcode		#zakoncz algorytm
qsort_left:			#sortuj lewa strone
	.align 0x4		#nowa wersja wysyla mniej danych na stos
	pushl %eax		#zapisz lewy
	pushl %ebx		#prawy
	pushl (dane+0x8)	#oraz j
	pushl (dane+0x14)	#adres powrotu
	pushl (dane+0x8)	#adres danych r=j+1 i l=l
	pushl %eax		#w odwrotnej kolejnosci
	call qsorta		#wywolanie rekurencyjne
	addl $0x8, %esp		#wyczysc stos
	popl dane+0x14		#wczytaj adres powrotu
	popl dane+0x8		#wczytaj j
	popl %ebx		#odzyskaj ebx=prawy
	popl %eax		#eax=lewy
	jmp ret_from_left	#powroc na swoje miejsce
qsort_right:			#sortuj prawa strone
	.align 0x4
	pushl (dane+0x14)	#zapisz adres powrotu
	pushl %ebx		#r=ebx r=r
	pushl (dane+0x8)	#l=j+1
	call qsorta		#wywolanie rekurencyjne
	addl $0x8, %esp		#wyczysc stos
	popl dane+0x14		#przypomnij sobie adres powrotu
endcode:			#zakoncz zabawe
	.align 0x4		#finish HIM!!! MK3
	subl $0x8, %esp		#clean stack
	pushl (dane+0x14)	#gdzie powrocic!
	ret			#return eax
