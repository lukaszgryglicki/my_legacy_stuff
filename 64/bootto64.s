[ORG 0x00007C00]
[BITS 16]



 boot_loader:
 ;Parameter from BIOS: dl = boot drive


 ;Set default state

cli
xor bx,bx
mov es,bx
mov fs,bx
mov gs,bx
mov ds,bx
mov ss,bx
mov sp,0x7C00
sti

 jmp 0:.clear_cs
.clear_cs:


 ;Load kernel from floppy disk

mov ax,0x020D			;ah = Function 0x02 ;al = Number of sectors
mov bx,startLongMode		;es:bx = Destination
mov cx,0x0002			;cx = Cylinder and sector
xor dh,dh			;dx = Head and drive number
int 0x13			;Int 0x13 Function 0x02 (Load sectors)


 ;Enable A20 via keyboard controller

call .wait_for_keyboard_controller

mov al,0xD1			;Out A20 control
out 0x64,al

call .wait_for_keyboard_controller

mov al,0xDF			;Out Enable A20
out 0x60,al


 ;Build page tables

;The page tables will look like this:

;PML4:
;dq 0x000000000000b00f		;00000000 00000000 00000000 00000000 00000000 00000000 10010000 00001111
;times 511 dq 0x0000000000000000

;PDP:
;dq 0x000000000000c00f		;00000000 00000000 00000000 00000000 00000000 00000000 10100000 00001111
;times 511 dq 0x0000000000000000

;PD:
;dq 0x000000000000018f		;00000000 00000000 00000000 00000000 00000000 00000000 00000001 10001111
;times 511 dq 0x0000000000000000

;This defines one 2MB page at the start of memory, so we can access the first 2MBs as if paging was disabled

xor bx,bx
mov es,bx
cld
mov di,0xa000

mov ax,0xb00f
stosw

xor ax,ax
mov cx,0x07ff
rep stosw

mov ax,0xc00f
stosw

xor ax,ax
mov cx,0x07ff
rep stosw

mov ax,0x018f
stosw

xor ax,ax
mov cx,0x07ff
rep stosw


 ;Enter long mode

mov eax,10100000b		;Set PAE and PGE
mov cr4,eax

mov edx, 0x0000a000		;Point cr3 at PML4
mov cr3,edx

mov ecx,0xC0000080		;Specify EFER MSR

rdmsr				;Enable long mode
or eax,0x00000100
wrmsr

mov ebx,cr0			;Activate long mode
or ebx,0x80000001		;by enabling paging and protection simultaneously
mov cr0,ebx			;skipping protected mode entirely

lgdt [gdt.pointer]

jmp gdt.code:startLongMode	;Load CS with 64 bit segment and flush the instruction cache



.wait_for_keyboard_controller:

mov cx,0xFFFF

.A20_loop:
in al,0x64
test al,2
loopnz .A20_loop

 ret



 ;Global Descriptor Table

 gdt:
dq 0x0000000000000000

.code equ $ - gdt
dq 0x0020980000000000

.data equ $ - gdt
dq 0x0000900000000000

.pointer:
dw $-gdt-1
dd gdt



 ;Fill boot sector

times 510-($-$$) db 0
dw 0xAA55			;Boot loader signature



[BITS 64]



 startLongMode:

cli				;Interupts are disabled because no IDT has been set up

mov edi,0x00b8000		;Display:Put long mode kernel here.
mov rax,0x0720077407750750
mov [edi],rax
mov rax,0x0767076e076f076c
mov [edi+8],rax
mov rax,0x0764076f076d0720
mov [edi+16],rax
mov rax,0x0765076b07200765
mov [edi+24],rax
mov rax,0x076c0765076e0772
mov [edi+32],rax
mov rax,0x0772076507680720
mov [edi+40],rax
mov rax,0x07200720072e0765
mov [edi+48],rax

 jmp $
