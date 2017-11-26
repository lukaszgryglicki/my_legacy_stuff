#define byte unsigned char
byte lastx;
byte lasty;
byte px,py,pc;
#define ENTER16 asm ( ".code16\n"
#define LEAVE  );
#define _set_vga_segment__M asm ( "\tmov $0xa000, %ax\n\tmov %ax, %es\n" ) ;
#define _iloop__M asm (" halt:\n\tjmp halt\n" ) ;
void _pixel();
void _start();
void _exit();
void _init_13h();
void _vga_off();
void _start()
{
 _init_13h();
 _set_vga_segment__M
 _vga_off();
 return ;
 px = 160;
 py = 100;
 pc = 0x88;
 while (1) ;
 _iloop__M
 _vga_off();
 _exit();
}
void _exit()
{
 ENTER16
     "\tmov $0x4c, %ax\n"
     "\tint $0x13\n"
 LEAVE
}
void _init_13h()
{
 ENTER16
	 "\tmov $0x13, %ax\n"
	 "\tint $0x10\n"
 LEAVE
}
void _pixel()
{
 ENTER16
	 "\tmov py, %ax\n"
	 "\tmov %ax, %di\n"
	 "\tshl $8,%ax\n"
	 "\tshl $6,%di\n"
	 "\tadd %ax,%di\n"
	 "\tadd px,%di\n"
	 "\tmov pc, %al\n"
	 "\tmovb %al,%es:(%di)\n"
 LEAVE
}
void _vga_off()
{
 ENTER16
	 "\tmov $0x03, %ax\n"
	 "\tint $0x10\n"
 LEAVE
}
