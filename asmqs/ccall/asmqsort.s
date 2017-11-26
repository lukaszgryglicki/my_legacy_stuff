##############################################################
# QUICKSORT w AT&T Assemblerze autor: Morgoth DBMA           #
# Lukasz Gryglicki: morgothdbma@o2.pl x11r6bsd@yahoo.com     #
# Napisany na systemie FreeBSD v4.5 uzywajac as (ix86)       #
# zawiera globalna funkcje qsortasm(void*,int(*)(const void* #
# ,const void*),unit,uint) do linkowania zewnetrznego: np    #
# g++ -c program.c; as -o qsort.o qsortasm.s;                #
# g++ -o program program.o qsort.o jako funkcja porownujaca  #
# (parametr drugi) mozna podac NULL, wtedy asembler uzyje    #
# swojej wlasnej funkcji do porownywania stringow cmpstr:    #
# jesli dane=(void*)NULL algorytm zwroci pusta NULL tabele   #
# jako wynik. Algorytm moze byc uzyty jako czysty asembler   #
# nalezy wtedy okreslic tzw. funkcje wejscia dla linkera     #
# as -o objfile.o qsort.s; ld -e pure_start -o prog objfile.o#
# program nie zadziala w Linux'ie gdyz uzywa on              #
# konwencji Microsoftu przy wolaniu funkcji (nie UNIX'a)     #
# aby uruchomic na Linuxie nalezy odpowiednio zmienic f-cje  #
# bsd_kernel: tak aby odpowiadala Linuxowej konwencji        #
# linkowanie z kodem C/C++ powinno (sic!) dzialac            #
# MorgothDBMA kom: 0-693-582-014, dom: 0-25-756-32-0         #
##############################################################

.data				#compiler data
dane:				#sekcja danych
	.long 0x0,0x0,0x0,0x0,0x0,0x0,0x0	#glowna tabela RUN-TIME
				#adresy zmiennych
             #dptr,i, j,  v, tmp,stck,func#
				#tylko 28 bajtow RAM!
CR_key:				#kod przycisku ENTER
	.byte 0xa,0x0		#tak zwane new-line '\n'
str_ptr:			#wskaznik na string (char**)
	.long 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0   #tablica wskaznikow  (char**)
str:				#inaczej tablica wskazn typu (char*)
        .asciz  "abc_git"	#poszczegolne stringi (char*)
        .asciz  "osama  "	#base+len itd
        .asciz  "bin    "	#ASCIZ zakonczone zerem
        .asciz  "laden  "	#string maja rozne dlugosci
        .asciz  "zzzzak "	#nalezy potem poprawnie
        .asciz  "zzupka "	#wypelnic tablice adresow
	.asciz  "morgoth"	#wiadomo :-)~
msg_begin:			#wyswietla to na poczatku
	.asciz "Przed sortowaniem\n\n"	#\n to znak CR
msg_end:			#asciz oznacza dodaj byte 0x0
	.asciz "Po sortowaniu\n\n"	#po lancuchu danych
msg_authora:			#nalezy go uwzgledniac przy
	.asciz "By MorgothDBMA morgothdbma@o2.pl\n\n"	#liczeniu offsetow
 #.globl asm_qsort_str__FPPcUiUi	#global entry for c++
 					#to byla stara wersja
					#sort(char**,uint,uint)
.globl qsortasm__FPvPFPCvPCv_iUiUi	#wersja uniwersalna
				#sort(void*,int(*)(const void*, const void*)
				#,uint,uint)
.globl pure_start		#wejscie dla linkera
.text				#sekcja textu
				#poza rekurencje dane i funkcja
	.align 0x4		#wyrownaj do 32 bitow
bsd_kernel:			#wywolaj przerwanie jadra
	int $0x80		#w eax numer f-kcji argumenty
	ret			#na stosie... powroc na miejsce wywolania
init_str_table:			#poniewaz tablica str_ptr (char**)
		#ladowanie danych do tablicy wskaznikow
		#powinno byc w jakiejs petli
		#gdyz dlugosci poszcz. napisow to 0x8
		#a pointerow to 0x4 wiec idealna petla
		#0x7 razy (ilosc napisow) po np. ecx
		#ale jestem leniwy (FIXED!) :-(
		#2.XII.2002 juz napisalem patrz nizej
/*				#to jest stara wersja nowa uzywa petli
	leal str+0x0, %eax	#jest aktualnie pusta NULL,NULL,...
	#movl $str+0x0, %eax	#rownowazna instrukcja
 	movl %eax, str_ptr+0x0	#nalezy zapelnic ja adresami do danych
	leal str+0x8, %eax	#leal str+0x(lb) oznacza zaladuj
	movl %eax, str_ptr+0x4	#do rejestru adres danych str plus
	leal str+0x10, %eax	#przesuniecie nastepnie umiesc ten
 	movl %eax, str_ptr+0x8	#adres w odpowiednim miejscu w pamieci
	leal str+0x18, %eax	#moznaby to zrobic petla po ECX
	#movl $str+0x18, %eax	#alternatywny sposob ladowania
				#przesuniecia adresu movl $d,%r lub leal d,%r
 	movl %eax, str_ptr+0xc	#ale dlugosci poszczegolnych stringow
	leal str+0x20, %eax	#sa rozne wiec nie da sie tak :-(
 	movl %eax, str_ptr+0x10	#ten kod wolany jest w funkcji
	leal str+0x28, %eax	#wejscia _start
	movl %eax, str_ptr+0x14	#a nastepnie powraca, nie musimy
	leal str+0x30, %eax	#bardzo lubie
 	movl %eax, str_ptr+0x18	#obliczac durne
	ret			#zapisywac adresu powrotu bo nie
				#minipulujemy "ostro" stosem	*/
			#To wyzej to stara wersja bez uzycia
			#petli byla mniej "elastyczna" wiec ja
			#zamienilem na petle :-)
	movl $0x0, %ecx		#zacznij petle od 0
loop_addres:			#tutaj skocz 7 razy ecx=0,1,...6
	leal str, %eax		#eax=adres str (nie wartosc ale adres!)
	movl %ecx, %edx		#edx=i
	shll $0x3, %edx		#edx=8*i (przesuwaj adres o 8 w kazdej petli)
	addl %edx, %eax		#eax=str+8*i (mamy juz SOURCE_ADDR)
	leal str_ptr, %ebx	#ebx=adres str_ptr (tworzymy adres docelowy)
	shrl $0x1, %edx		#edx=4*i (8/2=4 bo wskazniki to 32 bity)
	addl %edx, %ebx		#ebx=addr.str_ptr+4*i
	movl %eax, (%ebx)	#zapisz pod odpowiedni adres dlatego !! (%ebx)
				#wpisanie movl %eax, %ebx nie ma sensu bo
				#nadamy wartosc rejestrowi a nie wskaznikowi
	incl %ecx		#zwieksz licznik
	cmpl $0x7, %ecx		#porownaj z 7
	jl loop_addres		#jesli mniej to kontynuuj
	ret			#powroc z tworzenia tabeli (char**)
pure_start:				#tu sie zaczyna program
		#tutaj program zaczyna dzialanie!
		#linker FreeBSD wymaga punktu wejscia _start
		#moze on byc w konflikcie z C/C++ ktore tez go uzywaja
		#wtedy zmieni wejscie .globl _start i _start: na
		#np .globl _go_go_go _go_go_go: ale przy linkowaniu
		#poinformuj o tym linker:
		#as -o prog_name.o prog_name.s
		#ld -e _go_go_go -o prog_name prog_name.o
		#-e od ENTRY_POINT (stdlibc)
				#aby miec wersje do wolania z C
				#usun etykiete _start i zostaw
				#asm_qsort_str__FPPcUiUI
				#tzn asm_qsort_str(char**,uint,uint)
				#w katalogu universal jest wersja
				#sort(void*,int(*)(const void*, const void*)
				#,uint,uint) czyli dla dowolnych danych :-)
				#juz aktualnie ta wersja uzywa tego :-)~
	.align 0x4		#wyrownaj do 32 bitow
	call init_str_table	#zainicjuj tablice danych
	#call write_msg_1	#przed sortowaniem (dawna wersja)
				#zachowalem na pamiatke (nowa nast. 4 linijki)
	pushl $0x13		#poloz dlugosc stringa
	pushl $msg_begin	#poloz adres stringa
	call write_msg		#wypisz string
	addl $0x8, %esp		#wyczysc stos
	call write_out		#wyswietl przed sortowaniem
	leal str_ptr, %eax	#w eax adres danych (char**)
	pushl $0x6		#ilosc danych do posortyowania
	pushl $0x0		#lewa to 0
	#push  $0x0		#poloz 0 jako funkcje
				#algorytm automatycznie wezmnie sobie
				#cmpstr bo tak jest zbudowany
				#to jedna z metod ale badzmy grzeczni
	leal cmpstr, %ebx	#wez adres cmpstr
	pushl %ebx		#poloz, rownowazne pushl $cmpstr
				#tak grzecznie odlozylismy adres f-cji
	pushl %eax		#poloz adres danych
	#call asm_qsort_str__FPPcUiUi	#zawolaj qsort
				#to byla stara i czcigodna wersja
				#sort(char**,uint,uint)
	call qsortasm__FPvPFPCvPCv_iUiUi	#wersja uniwersalna 
	addl $0x10, %esp		#wyczysc stos
				#dlaczego akurat 0xc dodac do stosu
				#nie wiem ale dziala
				#jak widac juz jest 0x10 czyli
				#funkcja jest oddzielna jednostka
				#leksykalna tak jak powinno byc!
				#by Morgoth DBMA :-)
			#niech ktos to obliczy i powie dlaczego!
				#tak jakby f-cja uzywala trzech
				#argumentow na stosie
	#call write_msg_2	#po sortowaniu (stara wersja prawie identyczna)
				#jak write_msg_1, zastapilem ogolna
				#write_msg (argumenty na stosie)
	pushl $0xf		#poloz dlugosc stringa
	pushl $msg_end		#poloz adres stringa
	call write_msg		#wypisz string
	addl $0x8, %esp		#wyczysc stos
	call write_out		#wypisz wynik sortowania
	#call inno_satana	#wypisz dane o autorze
	pushl $0x22		#poloz dlugosc stringa
	pushl $msg_authora	#poloz adres stringa
	call write_msg		#wypisz string
	addl $0x8, %esp		#wyczysc stos
 	movl $0x1, %eax		#funkcja 1 (sys-exit)
	pushl $0x0		#kod powrotu 0x0
	#pushl %eax		#fikcyjny stos dla kernela
	#int $0x80		#zawolaj jadro
	#ret			#say goodbye
			#w nowej wersji zawolam tylko
			#procedure jadra :-)
	call bsd_kernel	#o wlasnie ta procedurke
			#dawniej dla kazdego napisu
			#byla oddzielna funkcja
			#teraz wszystkie napisy pisze write_msg
/*write_msg_1:			#wypisz tabele na ekran
			#powinienem zrobic wersje ktora pobiera
			#ze stosu argumenty i dlugosc
			#nastepnie je wypisuje, nie trzebaby
			#dla kazdego napisu robic odzielnej
			#funkcji, ale jestem leniwy :-(
			#may be (FIXED!)-ed in the future
	.align 0x4		#wyrownaj do 32 bitow
	movl $0x4, %eax		#funkcja nr.4 (sys_write)
	pushl $0x13		#dlugosc komunikatu
	pushl $msg_begin	#adres komunikatu nr1
	pushl $0x1		#deskryptor pliku (1=stdout)
	#pushl %eax		#fikcyjny adres dla kernela
	#int $0x80		#wywolaj przerwanie jadra
	#addl $0x10, %esp	#wyczysc stos
			#zawolam tylko procedure jadra BSD
			#zamiast robic to recznie za kazdym
			#razem call bsd_kernel
	call bsd_kernel		#zawolaj jadro
	addl $0xc, %esp		#wyczysc stos
	ret			#say goodbay	*/
write_msg:			#wypisz informacje na stdout
			#pobierz ze stosu adres danych
			#i dlugosc lancucha
	popl %edi		#adres powrotu
	pushl $0x1		#deskryptor pliku (1=stdout)
	movl $0x4, %eax		#funkcja nr.4 (sys_write)
	call bsd_kernel		#zawolaj jadro aby wykonalo sys_write
	addl $0x4, %esp		#oczysc stos
	pushl %edi		#odloz adres powrotu
	ret			#wroc pod podany wyzej adres
write_out:			#wypisz tabele stringow na ekran
			#wypisuje 7 linijek napisow na ekran
			#kozystajac z tablicy wskaznikow
			#str_ptr (ktora ma zostac posortowana)
	.align 0x4		#wyrownaj do 32 bitow
	movl $0x0, %ecx		#ecx=0
			#drukuje jedna linie poslugujac
			#sie aktualnym adresem w esi
			#a ecx jako licznikiem petli
print_line:			#drukuj poszczegolne linie
	.align 0x4		#wyrownaj do 32 bitow
	movl %ecx, %esi		#esi=ecx (petla w gore...)
	shll $0x2, %esi		#esi*4
	#leal str_ptr, %edx	#zaladuj do edx adres str_ptr
	#addl %edx, %esi	#utworz adres logiczne
			#nie wiem ktora metoda jest szybsza :-(
	addl $str_ptr, %esi	#ale zamiat uzywac leal
				#wystarczy uzyc operatora $
				#etyk czasami <=> (etyk) czyli wartosc z
				#z niej ale $etyk zawsze adres lub uzyc LEA
	movl $0x4, %eax		#eax=4 (sys_write)
	pushl $0x7		#do zapisania ilosc znakow
	pushl (%esi)		#adres do zapisania
	pushl $0x1		#poloz deskryptor pliku (1=stdout)
	#pushl %eax		#poloz fikcyjny adres powrotu...
	#int $0x80		#zawojaj przerwanie jadra
	#addl $0x10, %esp	#wyczysc stos po przerwaniu
	call bsd_kernel		#nowa wersja uzywa procedury jadra
	addl $0xc, %esp		#i oczyszcza stos
	movl $0x4, %eax		#funkcja nr.4 (sys_write)
	pushl $0x1		#dlugosc znaku ENTER (CR)=1
	pushl $CR_key		#adres nowej linii (CR)
	pushl $0x1		#deskryptor pliku (1=stdout)
	#pushl %eax		#fikcyjny adres dla kernela
	#int $0x80		#wywolaj przerwanie jadra
	#addl $0x10, %esp	#wyczysc stos
	call bsd_kernel		#zawolaj jadro
	addl $0xc, %esp		#wyczysc stos
	incl %ecx		#zwieksz licznik ECX
	cmpl $0x7, %ecx		#jesli licznik to 6 zakoncz
	jl print_line		#jeli ecx e (0,6) to powtarzaj
	ret			#say goodbay
cmpstr:				#orownaj dane w pamieci
			#funkcja porownuje dwa stringi
			#ktorych adresy dostaje na stosie
			#(bardzo szybka!) uzywa lodsb
			#wynik 0x0 lub 0x1 zwraca w eax
			#jest tez ta funkcja domyslna dla
			#wywolania gdy int(*)(const void*,const void*)=NULL
			#tzn nie podamy z C funkcji porownujacej
	.align 0x4		#wyrownaj do 32bitow
	popl %esi		#ignoruj adres powrotu
	popl %esi		#adres str1
	popl %edi		#adres str2
while_len:			#petla po min dlugosci (a,b)
			#dopoki dane obu stringow sa takie same
			#i nie osiagnieto konca zadnego z nich
	.align 0x4		#wyrownaj do 32bitow
	#xor %ax, %ax		#raczej niepotrzebne :-)
	lodsb			#esi-- i al=dana[b]
	movb %al, %ah		#ah=dana[b]
	movl %esi, %edx		#edx=esi+1 po zwiekszeniu przez lodsb
	movl %edi, %esi		#esi=edi teraz ten bedziemy zwiekszac
	lodsb			#esi++ (rownowazne edi++) al=dana[a]
	movl %edx, %edi		#edi=edx(wiec=esi) ah=dana[b], al=dana[a]
	xchg %esi, %edi		#zamien rejestry bo byla zamiana przy lodsb
	cmpb $0x0, %ah		#jesli koniec stringu b
	je done_cmp		#to juz porownalismy
	cmpb $0x0, %al		#jesli koniec stringu a
	je done_cmp		#analogicznie
	cmpb %ah, %al		#mamy dane to porownujemy je (2 bajty w ax)
	je while_len		#jesli sa rowne to nastepne dwa itd.
done_cmp:			#az do znalezienia roznicy (lub konca lancucha
			#mamy juz roznice tych stringow
			#lub koniec jednego albo obu
			#tzn. juz je orownalismy
	.align 0x4		#wyrownaj do 32 bitow
	cmpb %ah, %al		#a lub b) sa rozne to porownaj
	jle set_register	#dana[a,i]<=dana[b,i] dla i<min_len(a,b)
	xorl %eax, %eax		#inaczej wynik porownania 0 w EAX
	jmp ret_from_cmp	#powrot z funkcji
set_register:			#ustal wynik porownania na 0x1
		#ustaw wynik porownania EAX=0x1
	.align 0x4		#kod 32bitowy (wydajnosc!)
	movl $0x1, %eax		#wlasnie tu
ret_from_cmp:			#that's all folks
		#powroc do obszaru gdzie f-cje wywolano
	.align 0x4		#wiadomo :-)
	subl $0xc, %esp		#oczysc stos
	ret			#powroc w miejsce wywolania
halt:				#zatrzymaj algorytm
			#jesli adres danych wskazuje NULL
			#nalezy zatrzymac algorytm..
	subl $0xc, %esp		#wyczysc stos wtedy jest rowny 12
	ret			#zakoncz algorytm
				#powroc do C/C++ lub pure_start
get_function:			#ustaw domyslna f-cje asemblera
			#pobierz domyslna f-cje porownujaca
			#jesli tu sie dostalismy to znaczy
			#ze uzytkownik podal NULL jako f-cje
	leal cmpstr, %eax	#zaladuj adres funkcji asemblera
	movl %eax, dane+0x18	#wyslij ten adres qsort'owi
	jmp have_function	#juz ja mamy
qsortasm__FPvPFPCvPCv_iUiUi:	#go!
		#posortuj dane ze stosu
		#pobiera kolejno adres danych w RAM
		#adres f-cji porownujacej (jesli jest ona z C/C++)
		#to zapisuje przed wywolaniem rejestry a potem
		#odzyskuje je ze stosu (gdyz C/C++ moze
		#zrobic niezly balagan na stosie) z zalozenia
		#nie ufamy C/C++ :-}, gdy adres f-cji to 0x0 (NULL)
		#program uzyje domyslnej procedury cmpstr
		#patrz wyzej, dalej ze stosu pobiera lewy i prawy
		#funkcja musi trzymac w RAM adres powrotu
		#gdyz czesto jest wolana rekursywnie
		#i robi duzy ruch na stosie tuz przed powrotem
		#odtwarza z RAM adres powrotu, gdy wola inne funkcje
		#powrot z RAM jest zapisywany i potem odczytywany
		#ze stosu, typ int qsortasm(void* dane, int(*cmpfunc)(const
		# void*, const void*), unsigned int lewy, unsigned int prawy)
	popl dane+0x14		#glupawy adres powrotu zapamietaj stos+4
	popl dane		#adres danych globalny! stos+8
	popl dane+0x18		#funkcja globalna! stos+12
	cmpl $0x0,(dane+0x18)	#jesli nie podano funkcji
	je get_function		#wez domyslna
have_function:			#teraz juz na pewno jest
			#teraz juz na pewno mamy niezerowy
			#adres f-cji porownujacej (nie ma
			#100 % pewnosci ze dobrej)
	cmpl $0x0, dane		#czy adres danych nie jest przypadkiem
				#NULL, jesli tak to..
	je halt			#zrob hal, zakoncz f-cje
	popl %eax		#lewy dla qsorta stos+16
	popl %ebx		#prawy dla qsorta trzeba sie dokopac
	pushl (dane+0x14)	#stos+20 i pushl stos+16
				#zachowaj kopie adresu powrotu
			#qsorta modyfikuje te wartosc
			#wiec trzeba ja odlozyc
			#dopiero od tad dane dla qsorta
	pushl %ebx		#manipuluje stosem (w rozliczeniu stosu
			#nieistotne do qsorta posprzata po sobie)
			#ciagle stos + 16
	pushl %eax		#poloz lewy i prawy duzo istrukcji
	call qsorta		#wywolaj szybsza wersje (ale tylko raz)
	addl $0x8, %esp		#wykonane, wyczysc stos po qsorta
			#no i zgadza sie bo qsort 2 arg na stosie=8
	popl dane+0x14		#odzyskaj adres powrotu po
			#dzialaniu qsorta stos=20
	subl $0x10, %esp	#odejmnij 16 stos=4
			#te 4 bajty to adres powrotu
			#tzn funkcja dobrze po sobie sprzata
	pushl dane+0x14		#wrzuc na stos adres powrotu
	ret		#nastepnie 2 popy globalne =8 zdeejmnij l,p =16
			######### !!!!!! ########
			#to jest czcigodna wersja poprzednia
			#nie usuwac tego komentarza!
/*asm_qsort_str__FPPcUiUi:	#go!
	.align 0x4		#wyrownaj do 0x20 bitow (0x4 bajtow)
	popl dane+0x14		#glupawy adres powrotu zapamietaj
	popl dane		#adres danych globalny!
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
	ret		#nastepnie 2 popy globalne =8 zdeejmnij l,p =16*/
qsorta:			#wypas by Morgoth DBMA
			#ta wersja zna juz swoje dane w RAM
			#i f-cje porownujaca w RAM
			#jest tez wolana rekurencyjnie ale
			#na stosie dostaje juz tylko 2 dane
			#tzn. lewy i prawy (aby bylo szybciej)
			#ta f-cja wlasciwie robi qsort
			#a ta glowna wejsciowa tylko "odpowiednio"
			#ja wola :-)   :-)   :-)
	.align 0x4		#do 32 bitow
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
			#powtarzaj douki i<j
			#a potem continue
	.align 0x4		#do 32 bitow
	movl (dane+0x10), %ecx	#ecx=tmp (tmp po wyjsciu z petli ma A[i])
				#oczywiscie chodzi o adres :)
	movl (dane+0xc), %edx	#edx=v=A[l]
	pushl %eax		#zapisz 4 odstawowe rejestry
loop_i:				#petla i
			#zwiekszaj i dopuki
			#A[i]<v
	.align 0x4		#wyrownanie do 32 bitow
	incl dane+0x4		#i++;
	addl $0x4, %ecx		#A[i]=A[i+1] ecx zawiera adres!
	cmpl (dane+0x4), %ebx	#porownaj prawy z aktualnym i
	jl end_loop_i		#jesli poza tabela zakoncz
	pushl %ecx		#rozne f-cje C/C++ moga
	pushl %ebx		#je modyfikowac
	pushl %edx		#jest to konieczne gdyz
	pushl (%edx)		#porownaj V z...
	pushl (%ecx)		#A[i]
	call *(dane+0x18)	#funkcja zewnetrzna z C/C++ lub wewn.proc.ASM
				#dlaczego trzeba call *(adres)
				#nie wiem i nie rozumiem jeszcze
				#dokladnie operatora '*'
				#(HOW!) jak to dziala? :-(
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
			#petla o i zakonczyla sie
			#odzyskaj rejestry (poza petla)
			#aby nie robic non-stop pop-push
			#i ustaw odowiednie adresy
	.align 0x4		#wyrownaj do 32 bitow
	popl %eax		#eax,ebx lewy-prawy
	movl %ecx, dane+0x10	#tmp=address A[i]
	movl (dane+0x8), %ecx	#ecx=j
	shll $0x2, %ecx		#ecx=4*j  ==> 32bity!
	addl (dane), %ecx	#ecx=A[j]
	pushl %ebx		#procka
loop_j:				#petla j
		#j++ dopuki A[j]>v
		#potem porownanie i z j
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
	.align 0x4		#32 bity
			#odzyskaj rejestry poza petla
			#patrz end_loop_i i ustaw adresy
	popl %ebx		#(void*) z C nalezy je rzutowac :-)
 	movl (dane+0x4), %esi	#esi=wartosc i
	cmpl (dane+0x8), %esi	#porownaj (po wartsciach) i z j
	jl swap			#i<j (esi<dane+8) swap
	jmp continue		#inaczej omin zamiane
swap:				#zamiana danych (FIXED: wolna :-( )
	.align 0x4		#xchg wcale nie szybsza, probowalem !
		#dla dowolnych danych, zamiana wskaznikow pod adresami
		#tu kod moznaby przyspieszyc uzywajac instr ==> xchg
		#ale nie wiem jak :-(
			#juz wiem bo zrobilem, trzeba 2-3 movl i 2 xchg
			#nie oplaca sie :-) ta wersja jest rownie szybka
			#lub nawet szybsza o 1-2 instrukcje :-)
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
		#ale nie wiem jak :-( (patrz swap to jest zbedne) :-)   :-)
	.align 0x4		#FIXED wolny kod!! :-( (xchg wcale nie szybszy)
	movl (dane+0x4), %edi	#edi = i    (wiec jest OK, bo zamiana danych
				#wskazywanych przez RAM w RAMie, wyluskania
				#sa niezbedne cala tabela ma niznane elementy
				#jest tablica wskaznikow np. void** wiec
				#nalezy zrobic podwojne wyluskanie aby dostac
				#sie do elementu :-)
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
			#sprawdz czy potrzebna lewa rekursja
			#wzgledem j-1
	.align 0x4		#do 32 bitow
	decl dane+0x8		#j--
	cmpl (dane+0x8), %eax	#porownaj j-1 z lewym
	jl qsort_left		#jesli j-1>l sortuj lewa
ret_from_left:			#po qsort_left
			#juz po lewej rekursji
			#o ile wogole byla
			#teraz sprawdz czy potrzebna prawa
			#wzgledem j-1+2 tzn j+1
	.align 0x4		#do 0x20 bitow
	addl $0x2, dane+0x8	#j+=2
	cmpl (dane+0x8), %ebx	#porownaj j+1 z prawym
	jg qsort_right		#jesli j+1<r sortuj prawa
	jmp endcode		#zakoncz algorytm
qsort_left:			#sortuj lewa strone
			#sortuj (l, j+1)
			#ale najpierw zapisz wazne rejestry i
			#kod powrotu aby pozniej ta instancja
			#funkcji wiedziala jak powrocic
			#do poprzedniej ramki stosu
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
			#tak jak qsort_left (patrz wyzej)
			#ale sa roznice: sort(j+1,r) inny zakres
			#i nie misimy zapisywac rejestrow
			#(tylko kod powrotu) gdyz rejestry nie
			#beda juz uzywane poniewaz po posortowaniu
			#prawej pozostanie juz tylko powrot do
			#porzedniej ramki stosu, a tamta i tak
			#odzyska swoje rejestry lub powroci do C++
			#ewentualnie dla pure_assembler do f-cji
			#wolajacej np _start itp.
	.align 0x4		#wyrownaj ulozenie kodu do 32 bitow
	pushl (dane+0x14)	#zapisz adres powrotu
	pushl %ebx		#r=ebx r=r
	pushl (dane+0x8)	#l=j+1
	call qsorta		#wywolanie rekurencyjne
	addl $0x8, %esp		#wyczysc stos
	popl dane+0x14		#przypomnij sobie adres powrotu
endcode:			#zakoncz zabawe
			#posprzataj na stosie
			#odczytaj zapisana ramke powrotna
			#stosu wyczysc po sobie stos
			#i powroc w miejsce wywolania
	.align 0x4		#finish HIM!!! MK3
	subl $0x8, %esp		#clean stack
	pushl (dane+0x14)	#gdzie powrocic!
	ret			#return eax
			#Inno a Satana... Morgoth DBMA
			#...a Distance There is...
