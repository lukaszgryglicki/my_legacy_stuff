.globl _start
.text
.code16
_start:			#kamera bedzie w Z=-256 aby wygodniej liczyc!
	jmp _code_sect
_data_sect:
.set ONE,0x10000
_sine_table:		#sinusy w formacie 16.16 co 1 stopien od =0 do =90
.word 0x0010		#sin(0)
.word 0x0477		#sin(1) itd
.word 0x08ef
.word 0x0d65
.word 0x11db
.word 0x164f
.word 0x1ac2
.word 0x1f32
.word 0x23a0
.word 0x280c
.word 0x2c74
.word 0x30d8
.word 0x3539
.word 0x3996
.word 0x3dee
.word 0x4241
.word 0x4690
.word 0x4ad8
.word 0x4f1b
.word 0x5358
.word 0x578e
.word 0x5bbe
.word 0x5fe6
.word 0x6406
.word 0x681f
.word 0x6c30
.word 0x7039
.word 0x7438
.word 0x782f
.word 0x7c1c
.word 0x7fff
.word 0x83d9
.word 0x87a8
.word 0x8b6d
.word 0x8f27
.word 0x92d5
.word 0x9679
.word 0x9a10
.word 0x9d9b
.word 0xa11b
.word 0xa48d
.word 0xa7f3
.word 0xab4c
.word 0xae97
.word 0xb1d5
.word 0xb504
.word 0xb826
.word 0xbb39
.word 0xbe3e
.word 0xc134
.word 0xc41b
.word 0xc6f3
.word 0xc9bb
.word 0xcc73
.word 0xcf1b
.word 0xd1b3
.word 0xd43b
.word 0xd6b3
.word 0xd919
.word 0xdb6f
.word 0xddb3
.word 0xdfe7
.word 0xe208
.word 0xe419
.word 0xe617
.word 0xe803
.word 0xe9de
.word 0xeba6
.word 0xed5b
.word 0xeeff
.word 0xf08f
.word 0xf20d
.word 0xf378
.word 0xf4d0
.word 0xf615
.word 0xf746
.word 0xf865
.word 0xf970
.word 0xfa67
.word 0xfb4b
.word 0xfc1c
.word 0xfcd9
.word 0xfd82
.word 0xfe17
.word 0xfe98
.word 0xff06
.word 0xff60
.word 0xffa6
.word 0xffd8
.word 0xfff6		#sin(89)
.word 0xffff		#sin(90)
_triangle_fan_st1:
_st_up_vertex:
.long 1,6*ONE,1
_st_down_vertex:
.long 1,-60000,1
_st_vertexes:
.long 131072,-60000,1
.long 129080,-60000,22760
.long 123166,-60000,44828
.long 113510,-60000,65534
.long 100406,-60000,84250
.long 84250,-60000,100406
.long 65536,-60000,113510
.long 44828,-60000,123166
.long 22760,-60000,129080
.long 1,-60000,131070
.long -22760,-60000,129080
.long -44828,-60000,123166
.long -65534,-60000,113510
.long -84250,-60000,100406
.long -100406,-60000,84250
.long -113510,-60000,65536
.long -123166,-60000,44828
.long -129080,-60000,22760
.long -131070,-60000,1
.long -129080,-60000,-22760
.long -123166,-60000,-44828
.long -113510,-60000,-65534
.long -100406,-60000,-84250
.long -84250,-60000,-100406
.long -65536,-60000,-113510
.long -44828,-60000,-123166
.long -22760,-60000,-129080
.long 1,-60000,-131070
.long 22760,-60000,-129080
.long 44828,-60000,-123166
.long 65534,-60000,-113510
.long 84250,-60000,-100406
.long 100406,-60000,-84250
.long 113510,-60000,-65536
.long 123166,-60000,-44828
.long 129080,-60000,-22760
.long 131070,-60000,1
_st_vertex_buffer:
	.rept 38
	.long 1,1,1
	.endr
_current_pixel_x:
	.word 160
_current_pixel_y:
	.word 100
_current_color:
	.byte 1
_current_t1x:
	.word 160
_current_t1y:
	.word 180
_current_t2x:
	.word 100
_current_t2y:
	.word 150
_current_t3x:
	.word 220
_current_t3y:
	.word 150
_current_rx:
	.word 0
_current_ry:
	.word 0
_current_rz:
	.word 0
_current_sin:
	.long 0
_current_cos:
	.long 0
_figure_done:
	.byte 0x0
_frames_made:
	.word 0x0
_current_rot_dir:
	.byte 0x0
_code_sect:
	call _init_vga
	call _init_palette
	call _copy_to_buffer
	call _main_loop
	call _exit_vga
	call _com_exit
_com_exit:
	mov $0x004c,%ax
	int $0x21
	ret
_init_vga:
	mov $0x13,%ax
	int $0x10
	mov $0xa000,%ax
	mov %ax,%es
	ret
_exit_vga:
	mov $0x3, %ax
	int $0x10
	ret
_init_palette:
	call _wait_retrace
	mov $0xbf, %cl
	mov $0x0, %ch		#nasycenie
	mov $0x0, %ah		#kolor 0-R,1-G,2-B
_init_palette.1:
	mov $0x3C6, %dx
	mov $0xFF,%al
	out %al,%dx  		#ustawienie maski na 0xFF
	mov $0x3C8,%dx
	mov %cl,%al
	out %al,%dx  		#wybor do zapisu elementu tablicy kolorow na pozycji cx
	mov $0x3C9,%dx
	cmp $0x0, %ah		#czy red?
	jnz _init_palette.nR
	mov %ch, %al		#wpisz red
	jmp _init_palette.aR	#pomin 0
_init_palette.nR:
	mov $0x0, %al
_init_palette.aR:
	out %al, %dx
	cmp $0x1, %ah		#czy green?
	jnz _init_palette.nG
	mov %ch, %al		#wpisz green
	jmp _init_palette.aG	#pomin 0
_init_palette.nG:
	mov $0x0, %al
_init_palette.aG:
	out %al, %dx
	cmp $0x2, %ah		#czy blue?
	jnz _init_palette.nB
	mov %ch, %al		#wpisz blue
	jmp _init_palette.aB	#pomin 0
_init_palette.nB:
	mov $0x0, %al
_init_palette.aB:
	out %al, %dx
_init_palette.iC:		#increment color
	inc %ch
	cmp $64, %ch
	jnz _init_palette.tSC	#ten sam color
	xor %ch, %ch
	inc %ah
_init_palette.tSC:
	dec %cl
	cmp $0,%cl
	jnz _init_palette.1
	movb $1, (_current_color)
	ret
_wait_retrace:
	in %dx,%al
	test $8,%al
	jz _wait_retrace	#wait for retrace
	ret
_vga_clear:
	xor %di,%di
	xor %ax,%ax
	mov $(32000), %cx
	rep stosw
	ret
_main_loop:
	call _wait_retrace
	call _vga_clear
	call _render_st
	call _copy_from_buffer
	call _select_x_rotate
	call _world_transforms
	call _select_y_rotate
	call _world_transforms
	call _select_z_rotate
	call _world_transforms
	call _wait_key
	call _step_animation
	incw (_frames_made)
	cmpw $1800, (_frames_made)
	jl _main_loop
	ret
_wait_key:
	inb $0x60, %al		#wait for key 1 (tj ESC)
	cmpb $0x1, %al		
	jne _wait_key
	ret
_put_current_pixel:
	mov (_current_pixel_y),%ax
	mov %ax, %di
	shl $8,%ax
	shl $6,%di
	add %ax,%di
	add (_current_pixel_x),%di
	mov (_current_color),%al
	movb %al, %es:(%di)
	ret
_draw_triangle:
	mov (_current_t1x), %ax
	mov %ax, (_current_pixel_x)
	mov (_current_t1y), %ax
	mov %ax, (_current_pixel_y)
	call _put_current_pixel
	mov (_current_t2x), %ax
	mov %ax, (_current_pixel_x)
	mov (_current_t2y), %ax
	mov %ax, (_current_pixel_y)
	call _put_current_pixel
	mov (_current_t3x), %ax
	mov %ax, (_current_pixel_x)
	mov (_current_t3y), %ax
	mov %ax, (_current_pixel_y)
	call _put_current_pixel
	ret
_render_st:
	movb $0x0, (_figure_done)
compute_upper:
	lea _st_up_vertex, %si		# si aktualny wierzch
	#lea _st_vertex_buffer,%si
	movw $160, (_current_t1x)
	movw $100, (_current_t1y)
	mov 8(%si), %ebx		# ebx = Z
	mov (%si), %eax			# eax = X
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	add %ax, (_current_t1x)
	mov 4(%si), %eax		# eax = Y
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	sub %ax, (_current_t1y)
	lea _st_vertexes, %si
	#lea _st_vertex_buffer+24, %si
policz_pierwszy:
	movw $160, (_current_t2x)
	movw $100, (_current_t2y)	
	mov 8(%si), %ebx		# ebx = Z
	mov (%si), %eax			# eax = X
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	add %ax, (_current_t2x)
	mov 4(%si), %eax		# eax = Y
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	sub %ax, (_current_t2y)
	add $12, %si			# nastepny
powierzchnia:
kopiuj_trojkat:
	mov (_current_t2x), %ax
	mov %ax, (_current_t3x)
	mov (_current_t2y), %ax
	mov %ax, (_current_t3y)
oblicz_x:
	movw $160, (_current_t2x)
	movw $100, (_current_t2y)
	mov 8(%si), %ebx		# ebx = Z
	mov (%si), %eax			# eax = X
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	add %ax, (_current_t2x)
oblicz_y:
	mov 4(%si), %eax		# eax = Y
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	sub %ax, (_current_t2y)
	call _draw_triangle
	add $12, %si
	cmp $(_st_vertexes+12*36), %si
	#cmp $(_st_vertex_buffer+12*36), %si
	jl powierzchnia
	cmpb $0x1, (_figure_done)
	jz anim_st_done
compute_lower:
podstawa:
	lea _st_down_vertex, %si		# si aktualny wierzch
	#lea _st_vertex_buffer+12, %si		# si aktualny wierzch
	movw $160, (_current_t1x)
	movw $100, (_current_t1y)
	mov 8(%si), %ebx		# ebx = Z
	mov (%si), %eax			# eax = X
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	add %ax, (_current_t1x)
	mov 4(%si), %eax		# eax = Y
	mov $(8*16*65536), %edi		# mnoznik
	imul %edi			# wynik w edx:eax
	mov $0x100000, %edi
	add %ebx, %edi
	idiv %edi
	sar $16, %eax
	sub %ax, (_current_t1y)
	lea _st_vertexes, %di
	#lea _st_vertex_buffer+24, %di
	movb $0x1, (_figure_done)
	jmp powierzchnia		# teraz podstawy
anim_st_done:
	ret
_step_animation:
	incw (_current_rx)
	incw (_current_rx)
	incw (_current_rz)
	incw (_current_rz)
	incw (_current_rz)
	incw (_current_ry)
	cmpw $360, (_current_rx)
	jnz _step_animation.1
	movw $0x0, (_current_rx)
_step_animation.1:
	cmpw $360, (_current_ry)
	jnz _step_animation.2
	movw $0x0, (_current_ry)
_step_animation.2:
	cmpw $360, (_current_rz)
	jnz _step_animation.3
	movw $0x0, (_current_rz)
_step_animation.3:
	ret
_copy_to_buffer:
	mov $(38*3), %cx
	lea _triangle_fan_st1, %si
	lea _st_vertex_buffer, %di
_copy_to_buffer.1:
	movl (%si), %eax
	movl %eax, (%di)
	add $4, %si
	add $4, %di
	dec %cx
	cmp $0x0, %cx
	jnz _copy_to_buffer.1
	ret
_copy_from_buffer:
	mov $(38*3), %cx
	lea _triangle_fan_st1, %di
	lea _st_vertex_buffer, %si
_copy_from_buffer.1:
	movl (%si), %eax
	movl %eax, (%di)
	add $4, %si
	add $4, %di
	dec %cx
	cmp $0x0, %cx
	jnz _copy_from_buffer.1
	ret
_world_transforms:
	cmpb $0x1, (_current_rot_dir)
	jl _rot_x
	jz _rot_y
	jg _rot_z
_rot_x:
	mov (_current_rx), %si
	jmp _compute_sinus
_rot_y:
	mov (_current_ry), %si
	jmp _compute_sinus
_rot_z:
	mov (_current_rz), %si
_compute_sinus:
	cmp $0x0, %si
	jnz licz_dalej
	ret
licz_dalej:
	cmp $180, %si
	jle sine_1or2
sine_3or4:
	cmp $270, %si
	jle sine_3
sine_4:
	xor %eax,%eax
	sub $360,%si
	neg %si
	mov $90, %di
	sub %si, %di
	shl %si
	shl %di
	add $_sine_table,%si
	add $_sine_table,%di
	mov (%si),%ax
	neg %eax
	mov %eax,(_current_sin)
	xor %eax,%eax
	mov (%di),%ax
	mov %eax,(_current_cos)
	jmp sinus_computed
sine_3:
	xor %eax,%eax
	mov $270,%di
	sub %si,%di
	sub $180, %si
	shl %si
	shl %di
	add $_sine_table, %si
	add $_sine_table, %di
	mov (%si),%ax
	neg %eax
	mov %eax,(_current_sin)
	xor %eax,%eax
	mov (%di),%ax
	neg %eax
	mov %eax,(_current_cos)
	jmp sinus_computed
sine_1or2:
	cmp $90, %si
	jle sine_1
sine_2:
	xor %eax,%eax
	sub $180, %si
	neg %si
	mov $90,%di
	sub %si, %di
	shl %si
	shl %di
	add $_sine_table, %si
	add $_sine_table, %di
	mov (%si),%ax
	mov %eax,(_current_sin)
	xor %eax,%eax
	mov (%di),%ax
	neg %eax
	mov %eax,(_current_cos)
	jmp sinus_computed
sine_1:
	xor %eax,%eax
	mov $90,%di
	sub %si,%di
	shl %si
	shl %di
	add $_sine_table, %si
	add $_sine_table, %di
	mov (%si),%ax
	mov %eax,(_current_sin)
	xor %eax,%eax
	mov (%di),%ax
	mov %eax,(_current_cos)
sinus_computed:	
	lea _triangle_fan_st1, %di
	cmpb $0x1, (_current_rot_dir)
	jl _do_rotate_x
	jz _do_rotate_y
	jg _do_rotate_z
_do_rotate_x:
_world_transforms.x:
	mov 4(%di), %eax	# eax = Y
	imull (_current_cos)	# now EDX:EAX = Y*COS(k)
	mov $ONE, %esi
	idivl %esi		# teraz w eax y*cos(k)
	mov %eax, %esi
	mov 8(%di), %eax	# eax = Z
	imull (_current_sin)	# Z*sin(k)*64k
	mov $ONE, %ebx 
	idivl %ebx		# Z*sin(k)
	subl %esi, %eax		# eax = y = ycosk-zsink
	neg %eax
	mov %eax, 4(%di)
	#			# wyliczony Y teraz Z
	mov 4(%di), %eax
	imull (_current_sin)
	mov $ONE, %esi
	idivl %esi
	mov %eax, %esi
	mov 8(%di), %eax
	imull (_current_cos)
	mov $ONE, %ebx 
	idivl %ebx	
	addl %esi, %eax
	mov %eax, 8(%di)
	add $12, %di
	cmp $(_triangle_fan_st1+12*38), %di
	jl _world_transforms.x
	ret
_do_rotate_y:
_world_transforms.y:
	mov (%di), %eax	
	imull (_current_cos)
	mov $ONE, %esi
	idivl %esi	
	mov %eax, %esi
	mov 8(%di), %eax
	imull (_current_sin)
	mov $ONE, %ebx 
	idivl %ebx	
	subl %esi, %eax	
	neg %eax
	mov %eax, (%di)
	#		
	mov (%di), %eax
	imull (_current_sin)
	mov $ONE, %esi
	idivl %esi
	mov %eax, %esi
	mov 8(%di), %eax
	imull (_current_cos)
	mov $ONE, %ebx 
	idivl %ebx	
	addl %esi, %eax
	mov %eax, 8(%di)
	add $12, %di
	cmp $(_triangle_fan_st1+12*38), %di
	jl _world_transforms.y
	ret
_do_rotate_z:
_world_transforms.z:
	mov (%di), %eax		# X	
	imull (_current_cos)	# Xcos(k)*64k
	mov $ONE, %esi
	idivl %esi		# Xcosk
	mov %eax, %esi		# esi = Xcosk
	mov 4(%di), %eax	# eax = Y
	imull (_current_sin)	# Ysink*64k
	mov $ONE, %ebx 
	idivl %ebx		# Ysink
	subl %esi, %eax		# Ysink-Xcosk
	neg %eax		# teraz dobrze ?
	mov %eax, (%di)		# to X
	#		
	mov (%di), %eax		# X
	imull (_current_sin)
	mov $ONE, %esi
	idivl %esi		# Xsink
	mov %eax, %esi	
	mov 4(%di), %eax	# Y
	imull (_current_cos)
	mov $ONE, %ebx 
	idivl %ebx	
	addl %esi, %eax		# Xsink+Ycosk
	mov %eax, 4(%di)	# to Y
	add $12, %di		# next
	cmp $(_triangle_fan_st1+12*38), %di
	jl _world_transforms.z
	ret
_select_x_rotate:
	movb $0x0, (_current_rot_dir)
	ret
_select_y_rotate:
	movb $0x1, (_current_rot_dir)
	ret
_select_z_rotate:
	movb $0x2, (_current_rot_dir)
	ret
