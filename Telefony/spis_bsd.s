	.file	"spis_bsd.c"
	.version	"01.01"
gcc2_compiled.:
.globl __terminate
.globl __sjthrow
.data
	.p2align 2
	.type	 _rval,@object
	.size	 _rval,4
_rval:
	.long 1
	.p2align 2
	.type	 r_type,@object
	.size	 r_type,4
r_type:
	.long 0
	.p2align 2
	.type	 __SLOW_RND,@object
	.size	 __SLOW_RND,4
__SLOW_RND:
	.long 0
	.p2align 2
	.type	 __URAN,@object
	.size	 __URAN,4
__URAN:
	.long 1
	.section	.rodata
.LC0:
	.byte	 0x72,0x0
	.p2align 5
.LC1:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x43,0x61,0x6e,0x6e,0x6f,0x74
	.byte	 0x20,0x6f,0x70,0x65,0x6e,0x20,0x64,0x65,0x76,0x69
	.byte	 0x63,0x65,0x3a,0x20,0x25,0x73,0xa,0x0
.text
	.p2align 2,0x90
.globl reopen__Fv
		.type		 reopen__Fv,@function
reopen__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $0,_ran_device
	jne .L281
	jmp .L280
	.p2align 2,0x90
.L281:
	addl $-12,%esp
	movl _ran_device,%eax
	pushl %eax
	call fclose
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC0
	pushl $_ran_dev_name
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,_ran_device
	cmpl $0,_ran_device
	jne .L282
	addl $-8,%esp
	pushl $_ran_dev_name
	pushl $.LC1
	call printf
	addl $16,%esp
.L282:
.L280:
	leave
	ret
.Lfe1:
		.size		 reopen__Fv,.Lfe1-reopen__Fv
	.section	.rodata
	.p2align 5
.LC2:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x49,0x6e,0x76,0x61,0x6c,0x69
	.byte	 0x64,0x20,0x66,0x6c,0x61,0x67,0x20,0x6f,0x70,0x65
	.byte	 0x72,0x61,0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f,0x72
	.byte	 0x20,0x2f,0x64,0x65,0x76,0x2f,0x75,0x72,0x61,0x6e
	.byte	 0x64,0x6f,0x6d,0x2e,0xa,0x54,0x72,0x69,0x65,0x64
	.byte	 0x20,0x74,0x6f,0x20,0x75,0x73,0x65,0x20,0x6f,0x70
	.byte	 0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f,0x72,0x20,0x43
	.byte	 0x5f,0x54,0x4d,0x50,0x20,0x77,0x69,0x74,0x68,0x20
	.byte	 0x2f,0x64,0x65,0x76,0x2f,0x72,0x61,0x6e,0x64,0x6f
	.byte	 0x6d,0xa,0x0
.text
	.p2align 2,0x90
.globl flag_ctmp__Fv
		.type		 flag_ctmp__Fv,@function
flag_ctmp__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $1,r_type
	jne .L284
	cmpl $0,__SLOW_RND
	je .L285
	movl $0,__SLOW_RND
	jmp .L286
	.p2align 2,0x90
.L285:
	movl $1,__SLOW_RND
.L286:
	jmp .L283
	.p2align 2,0x90
.L284:
	addl $-12,%esp
	pushl $.LC2
	call printf
	addl $16,%esp
.L283:
	leave
	ret
.Lfe2:
		.size		 flag_ctmp__Fv,.Lfe2-flag_ctmp__Fv
	.section	.rodata
.LC3:
	.byte	 0x2f,0x64,0x65,0x76,0x2f,0x72,0x61,0x6e,0x64,0x6f
	.byte	 0x6d,0x0
.LC4:
	.byte	 0x2f,0x64,0x65,0x76,0x2f,0x75,0x72,0x61,0x6e,0x64
	.byte	 0x6f,0x6d,0x0
	.p2align 5
.LC5:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x49,0x6e,0x76,0x61,0x6c,0x69
	.byte	 0x64,0x20,0x66,0x6c,0x61,0x67,0x20,0x6f,0x70,0x65
	.byte	 0x72,0x61,0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f,0x72
	.byte	 0x20,0x43,0x5f,0x54,0x4d,0x50,0x2e,0xa,0x54,0x72
	.byte	 0x69,0x65,0x64,0x20,0x74,0x6f,0x20,0x75,0x73,0x65
	.byte	 0x20,0x6f,0x70,0x74,0x69,0x6f,0x6e,0x20,0x66,0x6f
	.byte	 0x72,0x20,0x2f,0x64,0x65,0x76,0x2f,0x72,0x61,0x6e
	.byte	 0x64,0x6f,0x6d,0x20,0x77,0x69,0x74,0x68,0x20,0x43
	.byte	 0x5f,0x54,0x4d,0x50,0xa,0x0
.text
	.p2align 2,0x90
.globl flag_uran__Fv
		.type		 flag_uran__Fv,@function
flag_uran__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $0,r_type
	jne .L288
	cmpl $0,__URAN
	je .L289
	movl $0,__URAN
	addl $-8,%esp
	pushl $.LC3
	pushl $_ran_dev_name
	call strcpy
	addl $16,%esp
	call reopen__Fv
	jmp .L287
	.p2align 2,0x90
.L289:
	cmpl $0,__URAN
	jne .L288
	movl $1,__URAN
	addl $-8,%esp
	pushl $.LC4
	pushl $_ran_dev_name
	call strcpy
	addl $16,%esp
	call reopen__Fv
	jmp .L287
	.p2align 2,0x90
.L290:
.L288:
	addl $-12,%esp
	pushl $.LC5
	call printf
	addl $16,%esp
.L287:
	leave
	ret
.Lfe3:
		.size		 flag_uran__Fv,.Lfe3-flag_uran__Fv
	.section	.rodata
	.p2align 5
.LC6:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x55,0x73,0x69,0x6e,0x67,0x20
	.byte	 0x75,0x6e,0x6b,0x6e,0x6f,0x77,0x6e,0x20,0x72,0x61
	.byte	 0x6e,0x64,0x6f,0x6d,0x20,0x74,0x79,0x70,0x65,0x2e
	.byte	 0xa,0x0
.text
	.p2align 2,0x90
.globl type_ran__Fv
		.type		 type_ran__Fv,@function
type_ran__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	cmpl $1,r_type
	jne .L292
	cmpl $0,__SLOW_RND
	je .L292
	movl $273,%eax
	jmp .L291
	jmp .L293
	.p2align 2,0x90
.L292:
	cmpl $1,r_type
	jne .L294
	cmpl $0,__SLOW_RND
	jne .L294
	movl $274,%eax
	jmp .L291
	jmp .L293
	.p2align 2,0x90
.L294:
	cmpl $0,r_type
	jne .L296
	cmpl $0,__URAN
	je .L296
	movl $275,%eax
	jmp .L291
	jmp .L293
	.p2align 2,0x90
.L296:
	cmpl $0,r_type
	jne .L298
	cmpl $0,__URAN
	jne .L298
	movl $276,%eax
	jmp .L291
	jmp .L293
	.p2align 2,0x90
.L298:
	addl $-12,%esp
	pushl $.LC6
	call printf
	addl $16,%esp
	xorl %eax,%eax
	jmp .L291
	.p2align 2,0x90
.L299:
.L297:
.L295:
.L293:
.L291:
	leave
	ret
.Lfe4:
		.size		 type_ran__Fv,.Lfe4-type_ran__Fv
	.section	.rodata
	.p2align 5
.LC7:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x43,0x61,0x6e,0x6e,0x6f,0x74
	.byte	 0x20,0x6f,0x70,0x65,0x6e,0x20,0x64,0x65,0x76,0x69
	.byte	 0x63,0x65,0x3a,0x20,0x25,0x73,0x0
.LC8:
	.byte	 0x2f,0x63,0x74,0x6d,0x70,0x2f,0x5f,0x5f,0x72,0x61
	.byte	 0x6e,0x64,0x6f,0x6d,0x0
.LC9:
	.byte	 0x77,0x0
	.p2align 5
.LC10:
	.byte	 0x52,0x41,0x4e,0x4c,0x49,0x42,0x20,0x4f,0x6f,0x6f
	.byte	 0x70,0x73,0x3a,0xa,0x46,0x61,0x74,0x61,0x6c,0x20
	.byte	 0x45,0x72,0x72,0x6f,0x72,0xa,0x43,0x61,0x6e,0x6e
	.byte	 0x6f,0x74,0x20,0x63,0x72,0x65,0x61,0x74,0x65,0x20
	.byte	 0x25,0x73,0xa,0x0
.LC11:
	.byte	 0x25,0x64,0x0
.text
	.p2align 2,0x90
.globl init_random__Fi
		.type		 init_random__Fi,@function
init_random__Fi:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl 8(%ebp),%eax
	movl %eax,r_type
	cmpl $0,r_type
	jne .L301
	addl $-8,%esp
	pushl $.LC4
	pushl $_ran_dev_name
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC0
	pushl $_ran_dev_name
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,_ran_device
	cmpl $0,_ran_device
	jne .L302
	addl $-8,%esp
	pushl $_ran_dev_name
	pushl $.LC7
	call printf
	addl $16,%esp
	movb $0,%al
	jmp .L300
	.p2align 2,0x90
.L302:
	movb $1,%al
	jmp .L300
	jmp .L303
	.p2align 2,0x90
.L301:
	cmpl $1,r_type
	jne .L303
	movl $.LC8,-8(%ebp)
	addl $-8,%esp
	pushl $.LC0
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L305
	addl $-8,%esp
	pushl $.LC9
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L306
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC10
	call printf
	addl $16,%esp
	jmp .L307
	.p2align 2,0x90
.L306:
	addl $-4,%esp
	pushl $1
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L307:
	jmp .L308
	.p2align 2,0x90
.L305:
	addl $-4,%esp
	pushl $_rval
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fscanf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L308:
	movzwl _rval,%eax
	movw %ax,-10(%ebp)
	movl $0,-16(%ebp)
	.p2align 2,0x90
.L309:
	movswl -10(%ebp),%eax
	cmpl %eax,-16(%ebp)
	jl .L312
	jmp .L310
	.p2align 2,0x90
.L312:
	addl $-12,%esp
	pushl $5
	call random__Fi
	addl $16,%esp
.L311:
	incl -16(%ebp)
	jmp .L309
	.p2align 2,0x90
.L310:
	movb $1,%al
	jmp .L300
	.p2align 2,0x90
.L304:
.L303:
	movb $1,%al
	jmp .L300
	.p2align 2,0x90
.L300:
	leave
	ret
.Lfe5:
		.size		 init_random__Fi,.Lfe5-init_random__Fi
	.p2align 2,0x90
.globl randomize__Fi
		.type		 randomize__Fi,@function
randomize__Fi:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call init_random__Fi
	addl $16,%esp
.L313:
	leave
	ret
.Lfe6:
		.size		 randomize__Fi,.Lfe6-randomize__Fi
	.p2align 2,0x90
.globl do_mess__Fv
		.type		 do_mess__Fv,@function
do_mess__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	addl $2,_rval
	cmpl $4095,_rval
	jle .L315
	movl $0,_rval
.L315:
	movl $.LC8,-8(%ebp)
	addl $-8,%esp
	pushl $.LC9
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L316
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC10
	call printf
	addl $16,%esp
	jmp .L317
	.p2align 2,0x90
.L316:
	addl $-4,%esp
	movl _rval,%eax
	pushl %eax
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L317:
.L314:
	leave
	ret
.Lfe7:
		.size		 do_mess__Fv,.Lfe7-do_mess__Fv
	.section	.rodata
	.p2align 5
.LC12:
	.byte	 0x46,0x61,0x74,0x61,0x6c,0x2c,0x20,0x52,0x61,0x6e
	.byte	 0x64,0x6f,0x6d,0x20,0x64,0x65,0x76,0x69,0x63,0x65
	.byte	 0x20,0x6e,0x6f,0x74,0x20,0x6f,0x70,0x65,0x6e,0x21
	.byte	 0xa,0x0
.text
	.p2align 2,0x90
.globl random__Fi
		.type		 random__Fi,@function
random__Fi:
	pushl %ebp
	movl %esp,%ebp
	subl $48,%esp
	pushl %esi
	pushl %ebx
	cmpl $0,r_type
	jne .L319
	cmpl $0,_ran_device
	jne .L320
	addl $-12,%esp
	pushl $.LC12
	call printf
	addl $16,%esp
	xorl %eax,%eax
	jmp .L318
	.p2align 2,0x90
.L320:
	movl $0,-20(%ebp)
	.p2align 2,0x90
.L321:
	cmpl $3,-20(%ebp)
	jle .L324
	jmp .L322
	.p2align 2,0x90
.L324:
	addl $-12,%esp
	movl _ran_device,%eax
	pushl %eax
	call fgetc
	addl $16,%esp
	movl %eax,%eax
	movl -20(%ebp),%edx
	movl %edx,%ecx
	leal 0(,%ecx,4),%edx
	leal -16(%ebp),%ecx
	movl %eax,(%edx,%ecx)
.L323:
	incl -20(%ebp)
	jmp .L321
	.p2align 2,0x90
.L322:
	movl -12(%ebp),%eax
	movl %eax,%edx
	movl %edx,%ecx
	sall $8,%ecx
	movl %ecx,%eax
	addl -16(%ebp),%eax
	movl -8(%ebp),%edx
	movl %edx,%ecx
	movl %ecx,%edx
	sall $16,%edx
	addl %edx,%eax
	movl -4(%ebp),%edx
	movl %edx,%ecx
	movl %ecx,%edx
	sall $24,%edx
	leal (%edx,%eax),%ecx
	movl %ecx,-24(%ebp)
	movl $-1,%ecx
	movl %ecx,%eax
	xorl %edx,%edx
	divl 8(%ebp)
	movl %eax,-28(%ebp)
	movl -24(%ebp),%ecx
	movl %ecx,%eax
	xorl %edx,%edx
	divl -28(%ebp)
	movl %eax,-28(%ebp)
	movl -28(%ebp),%edx
	movl %edx,-24(%ebp)
	movl -24(%ebp),%edx
	movl %edx,%eax
	jmp .L318
	jmp .L325
	.p2align 2,0x90
.L319:
	cmpl $1,r_type
	jne .L325
	cmpl $0,__SLOW_RND
	je .L327
	call do_mess__Fv
.L327:
	cmpl $0,8(%ebp)
	jg .L328
	movl $-1,%eax
	jmp .L318
	jmp .L325
	.p2align 2,0x90
.L328:
	call random
	movl %eax,-28(%ebp)
	movl $-2004318072,%ecx
	movl %ecx,%eax
	xorl %edx,%edx
	divl 8(%ebp)
	movl %eax,%esi
	movl -28(%ebp),%eax
	xorl %edx,%edx
	divl %esi
	movl %eax,%ebx
	movl %ebx,%eax
	jmp .L318
	.p2align 2,0x90
.L329:
.L326:
.L325:
	xorl %eax,%eax
	jmp .L318
	.p2align 2,0x90
.L318:
	leal -56(%ebp),%esp
	popl %ebx
	popl %esi
	leave
	ret
.Lfe8:
		.size		 random__Fi,.Lfe8-random__Fi
	.p2align 2,0x90
.globl kill_random__Fv
		.type		 kill_random__Fv,@function
kill_random__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	cmpl $0,r_type
	jne .L331
	cmpl $0,_ran_device
	je .L332
	addl $-12,%esp
	movl _ran_device,%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L332:
	jmp .L333
	.p2align 2,0x90
.L331:
	cmpl $1,r_type
	jne .L333
	addl $-12,%esp
	pushl $20
	call random__Fi
	addl $16,%esp
	movl %eax,%eax
	movl _rval,%edx
	addl $10,%edx
	leal (%eax,%edx),%ecx
	movl %ecx,_rval
	cmpl $4095,_rval
	jle .L335
	movl $0,_rval
.L335:
	movl $.LC8,-8(%ebp)
	addl $-8,%esp
	pushl $.LC9
	movl -8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L336
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC10
	call printf
	addl $16,%esp
	jmp .L333
	.p2align 2,0x90
.L336:
	addl $-4,%esp
	movl _rval,%eax
	pushl %eax
	pushl $.LC11
	movl -4(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L337:
.L334:
.L333:
.L330:
	leave
	ret
.Lfe9:
		.size		 kill_random__Fv,.Lfe9-kill_random__Fv
.data
	.p2align 2
	.type	 GET_T,@object
	.size	 GET_T,4
GET_T:
	.long 20
	.section	.rodata
.LC13:
	.byte	 0xa,0x0
.text
	.p2align 2,0x90
.globl TransformKey__FUli
		.type		 TransformKey__FUli,@function
TransformKey__FUli:
	pushl %ebp
	movl %esp,%ebp
	cmpl $0,12(%ebp)
	je .L341
	addl $200,8(%ebp)
.L341:
	movl 8(%ebp),%eax
	addl $-9,%eax
	cmpl $256,%eax
	ja .L342
	movl .L447(,%eax,4),%eax
	jmp *%eax
	.p2align 2,0x90
	.section	.rodata
	.p2align 2
	.p2align 2
.L447:
	.long .L393
	.long .L370
	.long .L371
	.long .L372
	.long .L373
	.long .L374
	.long .L375
	.long .L376
	.long .L377
	.long .L378
	.long .L369
	.long .L379
	.long .L380
	.long .L383
	.long .L342
	.long .L359
	.long .L365
	.long .L347
	.long .L360
	.long .L362
	.long .L367
	.long .L363
	.long .L351
	.long .L357
	.long .L358
	.long .L390
	.long .L389
	.long .L391
	.long .L342
	.long .L343
	.long .L361
	.long .L346
	.long .L348
	.long .L349
	.long .L350
	.long .L352
	.long .L353
	.long .L354
	.long .L388
	.long .L387
	.long .L392
	.long .L394
	.long .L381
	.long .L368
	.long .L366
	.long .L345
	.long .L364
	.long .L344
	.long .L356
	.long .L355
	.long .L386
	.long .L385
	.long .L384
	.long .L342
	.long .L342
	.long .L342
	.long .L382
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L342
	.long .L445
	.long .L423
	.long .L424
	.long .L425
	.long .L426
	.long .L427
	.long .L428
	.long .L429
	.long .L430
	.long .L431
	.long .L422
	.long .L432
	.long .L433
	.long .L436
	.long .L342
	.long .L412
	.long .L418
	.long .L400
	.long .L413
	.long .L415
	.long .L420
	.long .L416
	.long .L404
	.long .L410
	.long .L411
	.long .L443
	.long .L442
	.long .L444
	.long .L342
	.long .L396
	.long .L414
	.long .L399
	.long .L401
	.long .L402
	.long .L403
	.long .L405
	.long .L406
	.long .L407
	.long .L441
	.long .L440
	.long .L446
	.long .L395
	.long .L434
	.long .L421
	.long .L419
	.long .L398
	.long .L417
	.long .L397
	.long .L409
	.long .L408
	.long .L439
	.long .L438
	.long .L437
	.long .L342
	.long .L342
	.long .L342
	.long .L435
.text
	.p2align 2,0x90
.L343:
	movl $97,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L344:
	movl $98,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L345:
	movl $99,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L346:
	movl $100,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L347:
	movl $101,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L348:
	movl $102,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L349:
	movl $103,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L350:
	movl $104,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L351:
	movl $105,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L352:
	movl $106,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L353:
	movl $107,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L354:
	movl $108,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L355:
	movl $109,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L356:
	movl $110,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L357:
	movl $111,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L358:
	movl $112,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L359:
	movl $113,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L360:
	movl $114,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L361:
	movl $115,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L362:
	movl $116,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L363:
	movl $117,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L364:
	movl $118,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L365:
	movl $119,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L366:
	movl $120,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L367:
	movl $121,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L368:
	movl $122,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L369:
	movl $48,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L370:
	movl $49,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L371:
	movl $50,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L372:
	movl $51,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L373:
	movl $52,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L374:
	movl $53,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L375:
	movl $54,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L376:
	movl $55,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L377:
	movl $56,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L378:
	movl $57,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L379:
	movl $45,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L380:
	movl $61,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L381:
	movl $92,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L382:
	movl $32,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L383:
	movl $251,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L384:
	movl $47,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L385:
	movl $46,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L386:
	movl $44,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L387:
	movl $39,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L388:
	movl $59,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L389:
	movl $93,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L390:
	movl $91,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L391:
	movl $254,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L392:
	movl $96,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L393:
	movl $250,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L394:
	movl $255,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L395:
	movl $255,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L396:
	movl $65,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L397:
	movl $66,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L398:
	movl $67,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L399:
	movl $68,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L400:
	movl $69,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L401:
	movl $70,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L402:
	movl $71,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L403:
	movl $72,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L404:
	movl $73,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L405:
	movl $74,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L406:
	movl $75,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L407:
	movl $76,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L408:
	movl $77,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L409:
	movl $78,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L410:
	movl $79,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L411:
	movl $80,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L412:
	movl $81,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L413:
	movl $82,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L414:
	movl $83,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L415:
	movl $84,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L416:
	movl $85,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L417:
	movl $86,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L418:
	movl $87,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L419:
	movl $88,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L420:
	movl $89,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L421:
	movl $90,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L422:
	movl $41,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L423:
	movl $33,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L424:
	movl $64,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L425:
	movl $35,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L426:
	movl $36,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L427:
	movl $37,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L428:
	movl $94,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L429:
	movl $38,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L430:
	movl $42,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L431:
	movl $40,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L432:
	movl $95,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L433:
	movl $43,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L434:
	movl $124,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L435:
	movl $32,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L436:
	movl $251,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L437:
	movl $63,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L438:
	movl $62,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L439:
	movl $60,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L440:
	movl $34,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L441:
	movl $58,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L442:
	movl $125,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L443:
	movl $123,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L444:
	movl $254,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L445:
	movl $250,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L446:
	movl $126,%eax
	jmp .L340
	jmp .L342
	.p2align 2,0x90
.L448:
.L342:
	movl $7,%eax
	jmp .L340
	jmp .L449
	jmp .L340
	.p2align 2,0x90
.L449:
.L340:
	leave
	ret
.Lfe10:
		.size		 TransformKey__FUli,.Lfe10-TransformKey__FUli
	.p2align 2,0x90
.globl FSwap__FiiPPc
		.type		 FSwap__FiiPPc,@function
FSwap__FiiPPc:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	movl 12(%ebp),%eax
	leal 0(,%eax,4),%edx
	movl 16(%ebp),%eax
	movl (%eax,%edx),%edx
	pushl %edx
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl 8(%ebp),%eax
	leal 0(,%eax,4),%edx
	movl 16(%ebp),%eax
	movl (%eax,%edx),%edx
	pushl %edx
	movl 12(%ebp),%eax
	leal 0(,%eax,4),%edx
	movl 16(%ebp),%eax
	movl (%eax,%edx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	movl 8(%ebp),%eax
	leal 0(,%eax,4),%edx
	movl 16(%ebp),%eax
	movl (%eax,%edx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	jmp .L451
	jmp .L450
	.p2align 2,0x90
.L451:
.L450:
	leave
	ret
.Lfe11:
		.size		 FSwap__FiiPPc,.Lfe11-FSwap__FiiPPc
	.section	.rodata
	.p2align 5
.LC14:
	.byte	 0x46,0x61,0x74,0x61,0x6c,0x20,0x69,0x6e,0x20,0x46
	.byte	 0x53,0x6f,0x72,0x74,0x28,0x63,0x68,0x61,0x72,0x2a
	.byte	 0x2a,0x2c,0x20,0x69,0x6e,0x74,0x29,0x20,0x63,0x68
	.byte	 0x61,0x72,0x2a,0x2a,0x20,0x70,0x6f,0x69,0x6e,0x74
	.byte	 0x73,0x20,0x74,0x6f,0x20,0x4e,0x55,0x4c,0x4c,0x21
	.byte	 0xa,0x0
	.p2align 5
.LC15:
	.byte	 0x57,0x61,0x72,0x6e,0x69,0x6e,0x67,0x20,0x69,0x6e
	.byte	 0x20,0x46,0x53,0x6f,0x72,0x74,0x28,0x63,0x68,0x61
	.byte	 0x72,0x2a,0x2a,0x2c,0x20,0x69,0x6e,0x74,0x29,0x20
	.byte	 0x61,0x62,0x6f,0x75,0x74,0x20,0x74,0x6f,0x20,0x73
	.byte	 0x6f,0x72,0x74,0x20,0x30,0x2d,0x6c,0x65,0x6e,0x67
	.byte	 0x74,0x68,0x20,0x74,0x61,0x62,0x6c,0x65,0x2c,0x20
	.byte	 0x61,0x62,0x6f,0x72,0x74,0x65,0x64,0x21,0xa,0x0
.text
	.p2align 2,0x90
.globl FSort__FPPci
		.type		 FSort__FPPci,@function
FSort__FPPci:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	cmpl $0,8(%ebp)
	jne .L453
	addl $-12,%esp
	pushl $.LC14
	call printf
	addl $16,%esp
	jmp .L452
	.p2align 2,0x90
.L453:
	cmpl $0,12(%ebp)
	jne .L454
	addl $-12,%esp
	pushl $.LC15
	call printf
	addl $16,%esp
	jmp .L452
	.p2align 2,0x90
.L454:
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L455:
	movl -4(%ebp),%eax
	cmpl 12(%ebp),%eax
	jl .L458
	jmp .L456
	.p2align 2,0x90
.L458:
	movl $0,-8(%ebp)
	.p2align 2,0x90
.L459:
	movl 12(%ebp),%eax
	decl %eax
	movl %eax,%edx
	subl -4(%ebp),%edx
	cmpl %edx,-8(%ebp)
	jl .L462
	jmp .L457
	.p2align 2,0x90
.L462:
	addl $-8,%esp
	movl -8(%ebp),%eax
	leal 0(,%eax,4),%edx
	movl %edx,%eax
	addl 8(%ebp),%eax
	leal 4(%eax),%edx
	movl (%edx),%eax
	pushl %eax
	movl -8(%ebp),%eax
	leal 0(,%eax,4),%edx
	movl 8(%ebp),%eax
	movl (%eax,%edx),%edx
	pushl %edx
	call StrMore__FPcT0
	addl $16,%esp
	movb %al,%al
	testb %al,%al
	je .L461
	addl $-4,%esp
	movl 8(%ebp),%eax
	pushl %eax
	movl -8(%ebp),%eax
	pushl %eax
	movl -8(%ebp),%eax
	incl %eax
	pushl %eax
	call FSwap__FiiPPc
	addl $16,%esp
.L463:
.L461:
	incl -8(%ebp)
	jmp .L459
	.p2align 2,0x90
.L460:
.L457:
	incl -4(%ebp)
	jmp .L455
	.p2align 2,0x90
.L456:
	jmp .L464
	jmp .L452
	.p2align 2,0x90
.L464:
.L452:
	leave
	ret
.Lfe12:
		.size		 FSort__FPPci,.Lfe12-FSort__FPPci
	.p2align 2,0x90
.globl isLetter__Fi
		.type		 isLetter__Fi,@function
isLetter__Fi:
	pushl %ebp
	movl %esp,%ebp
	cmpl $96,8(%ebp)
	jle .L468
	cmpl $122,8(%ebp)
	jle .L467
	jmp .L468
	.p2align 2,0x90
.L468:
	cmpl $64,8(%ebp)
	jle .L466
	cmpl $90,8(%ebp)
	jle .L467
	jmp .L466
	.p2align 2,0x90
.L467:
	movl $1,%eax
	jmp .L465
	jmp .L469
	.p2align 2,0x90
.L466:
	xorl %eax,%eax
	jmp .L465
	.p2align 2,0x90
.L469:
	jmp .L470
	jmp .L465
	.p2align 2,0x90
.L470:
.L465:
	leave
	ret
.Lfe13:
		.size		 isLetter__Fi,.Lfe13-isLetter__Fi
	.p2align 2,0x90
.globl CutStr__FRPci
		.type		 CutStr__FRPci,@function
CutStr__FRPci:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl 8(%ebp),%eax
	cmpl $0,(%eax)
	jne .L472
	jmp .L471
	.p2align 2,0x90
.L472:
	movl $0,-4(%ebp)
	movl $0,-8(%ebp)
	.p2align 2,0x90
.L473:
	addl $-12,%esp
	movl 8(%ebp),%eax
	movl (%eax),%edx
	pushl %edx
	call strlen
	addl $16,%esp
	movl %eax,%eax
	cmpl %eax,-8(%ebp)
	jl .L476
	jmp .L474
	.p2align 2,0x90
.L476:
	movl 12(%ebp),%eax
	addl $-10,%eax
	cmpl %eax,-4(%ebp)
	jl .L477
	movl 8(%ebp),%eax
	movl (%eax),%edx
	movl -8(%ebp),%ecx
	leal (%ecx,%edx),%eax
	cmpb $32,(%eax)
	jne .L477
	movl 8(%ebp),%eax
	movl (%eax),%edx
	movl -8(%ebp),%ecx
	leal (%ecx,%edx),%eax
	movb $10,(%eax)
	movl $0,-4(%ebp)
.L477:
	movl 12(%ebp),%eax
	addl $10,%eax
	cmpl %eax,-4(%ebp)
	jl .L478
	movl 8(%ebp),%eax
	movl (%eax),%edx
	movl -8(%ebp),%ecx
	leal (%ecx,%edx),%eax
	movb $10,(%eax)
	movl $0,-4(%ebp)
.L478:
	incl -4(%ebp)
.L475:
	incl -8(%ebp)
	jmp .L473
	.p2align 2,0x90
.L474:
	jmp .L479
	jmp .L471
	.p2align 2,0x90
.L479:
.L471:
	leave
	ret
.Lfe14:
		.size		 CutStr__FRPci,.Lfe14-CutStr__FRPci
	.section	.rodata
.LC16:
	.byte	 0x53,0x74,0x6f,0x70,0x21,0x20,0x41,0x52,0x47,0x5f
	.byte	 0x50,0x54,0x52,0x3d,0x25,0x70,0xa,0x0
.text
	.p2align 2,0x90
.globl stop__FPi
		.type		 stop__FPi,@function
stop__FPi:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	addl $-8,%esp
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC16
	call printf
	addl $16,%esp
	jmp .L481
	jmp .L480
	.p2align 2,0x90
.L481:
.L480:
	leave
	ret
.Lfe15:
		.size		 stop__FPi,.Lfe15-stop__FPi
	.section	.rodata
.LC17:
	.byte	 0x53,0x54,0x4f,0x50,0x21,0xa,0x0
.text
	.p2align 2,0x90
.globl Trap__Fv
		.type		 Trap__Fv,@function
Trap__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	addl $-8,%esp
	pushl $.LC17
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -4(%ebp),%eax
	pushl %eax
	call takeint__FPi
	addl $16,%esp
	jmp .L483
	jmp .L482
	.p2align 2,0x90
.L483:
.L482:
	leave
	ret
.Lfe16:
		.size		 Trap__Fv,.Lfe16-Trap__Fv
	.p2align 2,0x90
.globl StrMore__FPcT0
		.type		 StrMore__FPcT0,@function
StrMore__FPcT0:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%ebx
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%eax
	cmpl %eax,%ebx
	jae .L485
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%ebx
	jmp .L486
	.p2align 2,0x90
.L485:
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%ebx
.L486:
	movl %ebx,-4(%ebp)
	xorl %edx,%edx
	.p2align 2,0x90
.L487:
	movl 8(%ebp),%eax
	movl 12(%ebp),%ecx
	movb (%edx,%eax),%al
	cmpb (%edx,%ecx),%al
	jne .L490
	cmpl -4(%ebp),%edx
	jle .L489
	jmp .L490
	.p2align 2,0x90
.L490:
	jmp .L488
	.p2align 2,0x90
.L489:
	incl %edx
	jmp .L487
	.p2align 2,0x90
.L488:
	movl 8(%ebp),%eax
	movl 12(%ebp),%ecx
	movb (%edx,%eax),%al
	cmpb (%edx,%ecx),%al
	jle .L491
	movb $1,%al
	jmp .L484
	jmp .L492
	.p2align 2,0x90
.L491:
	movb $0,%al
	jmp .L484
	.p2align 2,0x90
.L492:
	jmp .L493
	jmp .L484
	.p2align 2,0x90
.L493:
.L484:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe17:
		.size		 StrMore__FPcT0,.Lfe17-StrMore__FPcT0
	.p2align 2,0x90
.globl getch__Fv
		.type		 getch__Fv,@function
getch__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $112,%esp
	pushl %edi
	pushl %esi
	addl $-8,%esp
	leal -44(%ebp),%eax
	pushl %eax
	movswl __sF+14,%eax
	pushl %eax
	call tcgetattr
	addl $16,%esp
	leal -88(%ebp),%edi
	leal -44(%ebp),%esi
	cld
	movl $11,%ecx
	rep
	movsl
	andb $254,-75(%ebp)
	andb $247,-76(%ebp)
	movb $1,-56(%ebp)
	movb $0,-55(%ebp)
	addl $-4,%esp
	leal -88(%ebp),%eax
	pushl %eax
	pushl $2
	movswl __sF+14,%eax
	pushl %eax
	call tcsetattr
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	je .L496
	xorl %eax,%eax
	jmp .L494
	.p2align 2,0x90
.L495:
.L496:
	movl $0,-92(%ebp)
	decl __sF+4
	cmpl $0,__sF+4
	jge .L497
	addl $-12,%esp
	pushl $__sF
	call __srget
	addl $16,%esp
	movl %eax,%eax
	jmp .L498
	.p2align 2,0x90
.L497:
	movl __sF,%edx
	movzbl (%edx),%eax
	incl __sF
.L498:
	movl %eax,-92(%ebp)
	cmpl $10,-92(%ebp)
	jne .L499
	jmp .L496
	.p2align 2,0x90
.L499:
	addl $-4,%esp
	leal -44(%ebp),%eax
	pushl %eax
	pushl $2
	movswl __sF+14,%eax
	pushl %eax
	call tcsetattr
	addl $16,%esp
	addl $-8,%esp
	movsbl -92(%ebp),%eax
	pushl %eax
	pushl $cout
	call __ls__7ostreamc
	addl $16,%esp
	movl -92(%ebp),%edx
	movl %edx,%eax
	jmp .L494
	.p2align 2,0x90
.L494:
	leal -120(%ebp),%esp
	popl %esi
	popl %edi
	leave
	ret
.Lfe18:
		.size		 getch__Fv,.Lfe18-getch__Fv
	.p2align 2,0x90
.globl GetStrType__Fi
		.type		 GetStrType__Fi,@function
GetStrType__Fi:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%eax
	movl %eax,GET_T
.L500:
	leave
	ret
.Lfe19:
		.size		 GetStrType__Fi,.Lfe19-GetStrType__Fi
	.p2align 2,0x90
.globl SwitchIn__Fv
		.type		 SwitchIn__Fv,@function
SwitchIn__Fv:
	pushl %ebp
	movl %esp,%ebp
	cmpl $20,GET_T
	jne .L502
	movl $21,GET_T
	jmp .L503
	.p2align 2,0x90
.L502:
	movl $20,GET_T
.L503:
.L501:
	leave
	ret
.Lfe20:
		.size		 SwitchIn__Fv,.Lfe20-SwitchIn__Fv
	.p2align 2,0x90
.globl DecompressData__FPc
		.type		 DecompressData__FPc,@function
DecompressData__FPc:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L505:
	cmpl $79,-4(%ebp)
	jle .L508
	jmp .L506
	.p2align 2,0x90
.L508:
	movl 8(%ebp),%eax
	movl -4(%ebp),%edx
	addl %edx,%eax
	cmpb $95,(%eax)
	jne .L507
	movl 8(%ebp),%eax
	movl -4(%ebp),%edx
	addl %edx,%eax
	movb $32,(%eax)
.L509:
.L507:
	incl -4(%ebp)
	jmp .L505
	.p2align 2,0x90
.L506:
.L504:
	leave
	ret
.Lfe21:
		.size		 DecompressData__FPc,.Lfe21-DecompressData__FPc
	.p2align 2,0x90
.globl CompressData__FPc
		.type		 CompressData__FPc,@function
CompressData__FPc:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L511:
	cmpl $79,-4(%ebp)
	jle .L514
	jmp .L512
	.p2align 2,0x90
.L514:
	movl 8(%ebp),%eax
	movl -4(%ebp),%edx
	addl %edx,%eax
	cmpb $32,(%eax)
	jne .L513
	movl 8(%ebp),%eax
	movl -4(%ebp),%edx
	addl %edx,%eax
	movb $95,(%eax)
.L515:
.L513:
	incl -4(%ebp)
	jmp .L511
	.p2align 2,0x90
.L512:
.L510:
	leave
	ret
.Lfe22:
		.size		 CompressData__FPc,.Lfe22-CompressData__FPc
	.section	.rodata
.LC19:
	.byte	 0x25,0x63,0x0
	.p2align 3
.LC18:
	.long 0x0,0x40000000
.text
	.p2align 2,0x90
.globl FromBinary__FP7__sFILET0
		.type		 FromBinary__FP7__sFILET0,@function
FromBinary__FP7__sFILET0:
	pushl %ebp
	movl %esp,%ebp
	subl $68,%esp
	pushl %ebx
	nop
	.p2align 2,0x90
.L517:
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call fgetc
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $-1,-4(%ebp)
	jne .L519
	jmp .L518
	.p2align 2,0x90
.L519:
	movl -4(%ebp),%eax
	addl $-48,%eax
	movl %eax,-40(%ebp)
	movl $1,-44(%ebp)
	.p2align 2,0x90
.L520:
	cmpl $7,-44(%ebp)
	jle .L523
	jmp .L521
	.p2align 2,0x90
.L523:
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call fgetc
	addl $16,%esp
	movl %eax,%eax
	movl -44(%ebp),%edx
	movl %edx,%ecx
	leal 0(,%ecx,4),%edx
	leal -40(%ebp),%ecx
	leal -48(%eax),%ebx
	movl %ebx,(%edx,%ecx)
.L522:
	incl -44(%ebp)
	jmp .L520
	.p2align 2,0x90
.L521:
	movl $0,-8(%ebp)
	movl $0,-48(%ebp)
	.p2align 2,0x90
.L524:
	cmpl $7,-48(%ebp)
	jle .L527
	jmp .L525
	.p2align 2,0x90
.L527:
	movl $7,%edx
	movl %edx,%eax
	subl -48(%ebp),%eax
	movl %eax,-52(%ebp)
	fildl -52(%ebp)
	subl $8,%esp
	fstpl (%esp)
	fldl .LC18
	subl $8,%esp
	fstpl (%esp)
	call pow
	addl $16,%esp
	movl -48(%ebp),%eax
	movl %eax,%edx
	leal 0(,%edx,4),%eax
	leal -40(%ebp),%edx
	fildl (%eax,%edx)
	fmulp %st,%st(1)
	fnstcw -54(%ebp)
	movw -54(%ebp),%dx
	orw $3072,%dx
	movw %dx,-56(%ebp)
	fldcw -56(%ebp)
	fistpl -52(%ebp)
	movl -52(%ebp),%eax
	fldcw -54(%ebp)
	addl %eax,-8(%ebp)
.L526:
	incl -48(%ebp)
	jmp .L524
	.p2align 2,0x90
.L525:
	addl $-4,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC19
	movl 12(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	jmp .L517
	.p2align 2,0x90
.L518:
.L516:
	movl -72(%ebp),%ebx
	leave
	ret
.Lfe23:
		.size		 FromBinary__FP7__sFILET0,.Lfe23-FromBinary__FP7__sFILET0
	.section	.rodata
	.p2align 3
.LC20:
	.long 0x0,0x40000000
.text
	.p2align 2,0x90
.globl ToBinary__FP7__sFILET0
		.type		 ToBinary__FP7__sFILET0,@function
ToBinary__FP7__sFILET0:
	pushl %ebp
	movl %esp,%ebp
	subl $72,%esp
	nop
	.p2align 2,0x90
.L529:
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call fgetc
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $-1,-4(%ebp)
	jne .L531
	jmp .L530
	.p2align 2,0x90
.L531:
	movl $7,-40(%ebp)
	.p2align 2,0x90
.L532:
	cmpl $0,-40(%ebp)
	jge .L535
	jmp .L533
	.p2align 2,0x90
.L535:
	fildl -40(%ebp)
	subl $8,%esp
	fstpl (%esp)
	fldl .LC20
	subl $8,%esp
	fstpl (%esp)
	call pow
	addl $16,%esp
	fnstcw -46(%ebp)
	movw -46(%ebp),%dx
	orw $3072,%dx
	movw %dx,-48(%ebp)
	fldcw -48(%ebp)
	fistpl -44(%ebp)
	movl -44(%ebp),%eax
	fldcw -46(%ebp)
	cmpl %eax,-4(%ebp)
	jl .L536
	movl $7,%edx
	movl %edx,%eax
	subl -40(%ebp),%eax
	leal 0(,%eax,4),%edx
	leal -36(%ebp),%eax
	movl $1,(%edx,%eax)
	fildl -40(%ebp)
	subl $8,%esp
	fstpl (%esp)
	fldl .LC20
	subl $8,%esp
	fstpl (%esp)
	call pow
	addl $16,%esp
	fnstcw -46(%ebp)
	movw -46(%ebp),%dx
	orw $3072,%dx
	movw %dx,-48(%ebp)
	fldcw -48(%ebp)
	fistpl -44(%ebp)
	movl -44(%ebp),%eax
	fldcw -46(%ebp)
	subl %eax,-4(%ebp)
	jmp .L534
	.p2align 2,0x90
.L536:
	movl $7,%edx
	movl %edx,%eax
	subl -40(%ebp),%eax
	leal 0(,%eax,4),%edx
	leal -36(%ebp),%eax
	movl $0,(%edx,%eax)
.L537:
.L534:
	decl -40(%ebp)
	jmp .L532
	.p2align 2,0x90
.L533:
	movl $0,-52(%ebp)
	.p2align 2,0x90
.L538:
	cmpl $7,-52(%ebp)
	jle .L541
	jmp .L539
	.p2align 2,0x90
.L541:
	addl $-4,%esp
	movl -52(%ebp),%eax
	movl %eax,%edx
	leal 0(,%edx,4),%eax
	leal -36(%ebp),%edx
	movl (%eax,%edx),%eax
	pushl %eax
	pushl $.LC11
	movl 12(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
.L540:
	incl -52(%ebp)
	jmp .L538
	.p2align 2,0x90
.L539:
	jmp .L529
	.p2align 2,0x90
.L530:
.L528:
	leave
	ret
.Lfe24:
		.size		 ToBinary__FP7__sFILET0,.Lfe24-ToBinary__FP7__sFILET0
	.section	.rodata
.LC21:
	.byte	 0x25,0x73,0x0
.LC22:
	.byte	 0x70,0x6f,0x64,0x61,0x6c,0x65,0x73,0x20,0x7a,0x61
	.byte	 0x20,0x64,0x6c,0x75,0x67,0x61,0x20,0x6e,0x61,0x7a
	.byte	 0x77,0x65,0xa,0x0
.text
	.p2align 2,0x90
.globl takechar__FRc
		.type		 takechar__FRc,@function
takechar__FRc:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	pushl $.LC21
	call scanf
	addl $16,%esp
	addl $-4,%esp
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC19
	leal -80(%ebp),%eax
	pushl %eax
	call sscanf
	addl $16,%esp
	addl $-12,%esp
	leal -80(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%eax
	cmpl $1,%eax
	jbe .L543
	addl $-8,%esp
	pushl $.LC22
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L543:
.L542:
	leave
	ret
.Lfe25:
		.size		 takechar__FRc,.Lfe25-takechar__FRc
	.section	.rodata
.LC23:
	.byte	 0x50,0x6f,0x64,0x61,0x6c,0x65,0x73,0x20,0x7a,0x61
	.byte	 0x20,0x64,0x6c,0x75,0x67,0x61,0x20,0x6e,0x61,0x7a
	.byte	 0x77,0x65,0xa,0x0
.text
	.p2align 2,0x90
.globl takechar__FPc
		.type		 takechar__FPc,@function
takechar__FPc:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	pushl $.LC21
	call scanf
	addl $16,%esp
	addl $-4,%esp
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC19
	leal -80(%ebp),%eax
	pushl %eax
	call sscanf
	addl $16,%esp
	addl $-12,%esp
	leal -80(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%eax
	cmpl $1,%eax
	jbe .L545
	addl $-8,%esp
	pushl $.LC23
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L545:
.L544:
	leave
	ret
.Lfe26:
		.size		 takechar__FPc,.Lfe26-takechar__FPc
	.section	.rodata
.LC24:
	.byte	 0x8,0x20,0x8,0x0
	.p2align 5
.LC25:
	.byte	 0x53,0x6f,0x72,0x72,0x79,0x2c,0x20,0x79,0x6f,0x75
	.byte	 0x20,0x68,0x61,0x76,0x65,0x20,0x75,0x73,0x65,0x64
	.byte	 0x20,0x69,0x6c,0x6c,0x65,0x67,0x61,0x6c,0x20,0x63
	.byte	 0x68,0x61,0x72,0x61,0x63,0x74,0x65,0x72,0x20,0x55
	.byte	 0x4e,0x49,0x43,0x4f,0x44,0x45,0x20,0x69,0x73,0x20
	.byte	 0x30,0x78,0x31,0x62,0x2c,0x20,0x64,0x6f,0x20,0x69
	.byte	 0x74,0x20,0x61,0x67,0x61,0x69,0x6e,0x2e,0xa,0x0
.LC26:
	.byte	 0x2d,0x0
.text
	.p2align 2,0x90
.globl takestring__FPci
		.type		 takestring__FPci,@function
takestring__FPci:
	pushl %ebp
	movl %esp,%ebp
	subl $108,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	addl $-8,%esp
	leal -44(%ebp),%eax
	pushl %eax
	movswl __sF+14,%eax
	pushl %eax
	call tcgetattr
	addl $16,%esp
	leal -88(%ebp),%edi
	leal -44(%ebp),%esi
	cld
	movl $11,%ecx
	rep
	movsl
	andb $254,-75(%ebp)
	andb $247,-76(%ebp)
	movb $1,-56(%ebp)
	movb $0,-55(%ebp)
	addl $-4,%esp
	leal -88(%ebp),%eax
	pushl %eax
	pushl $2
	movswl __sF+14,%eax
	pushl %eax
	call tcsetattr
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	je .L548
	jmp .L546
	.p2align 2,0x90
.L547:
.L548:
	movl $0,-92(%ebp)
	movl 8(%ebp),%eax
	movb $0,(%eax)
	.p2align 2,0x90
.L549:
	cmpl $78,-92(%ebp)
	jle .L551
	jmp .L558
	.p2align 2,0x90
.L551:
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	leal (%edx,%eax),%ebx
	decl __sF+4
	cmpl $0,__sF+4
	jge .L552
	addl $-12,%esp
	pushl $__sF
	call __srget
	addl $16,%esp
	movl %eax,%edx
	movb %dl,%al
	jmp .L553
	.p2align 2,0x90
.L552:
	movl __sF,%edx
	movb (%edx),%al
	incl __sF
.L553:
	movb %al,(%ebx)
	cmpl $0,-92(%ebp)
	jne .L554
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	cmpb $10,(%eax)
	jne .L554
	jmp .L555
	.p2align 2,0x90
.L554:
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	cmpb $32,(%eax)
	jne .L556
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	movb $95,(%eax)
.L556:
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	cmpb $10,(%eax)
	jne .L557
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	movb $0,(%eax)
	jmp .L558
	.p2align 2,0x90
.L557:
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	cmpb $8,(%eax)
	jne .L559
	cmpl $0,-92(%ebp)
	jle .L560
	addl $-12,%esp
	pushl $.LC24
	call printf
	addl $16,%esp
.L560:
	decl -92(%ebp)
	cmpl $0,-92(%ebp)
	jge .L561
	movl $0,-92(%ebp)
.L561:
	jmp .L555
	.p2align 2,0x90
.L559:
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	cmpb $27,(%eax)
	jne .L562
	addl $-8,%esp
	pushl $.LC25
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L548
	.p2align 2,0x90
.L562:
	cmpl $64222,12(%ebp)
	je .L563
	addl $-8,%esp
	movl 8(%ebp),%eax
	movl -92(%ebp),%edx
	addl %edx,%eax
	movsbl (%eax),%edx
	pushl %edx
	pushl $cout
	call __ls__7ostreamc
	addl $16,%esp
.L563:
	incl -92(%ebp)
.L555:
	jmp .L549
	.p2align 2,0x90
.L550:
.L558:
	cmpl $0,-92(%ebp)
	jne .L564
	addl $-8,%esp
	pushl $.LC26
	movl 8(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC26
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L564:
	call ln__Fv
	addl $-4,%esp
	leal -44(%ebp),%eax
	pushl %eax
	pushl $2
	movswl __sF+14,%eax
	pushl %eax
	call tcsetattr
	addl $16,%esp
	jmp .L565
	jmp .L546
	.p2align 2,0x90
.L565:
.L546:
	leal -120(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe27:
		.size		 takestring__FPci,.Lfe27-takestring__FPci
	.p2align 2,0x90
.globl lns__Fi
		.type		 lns__Fi,@function
lns__Fi:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movw $0,-2(%ebp)
	.p2align 2,0x90
.L567:
	movswl -2(%ebp),%eax
	cmpl 8(%ebp),%eax
	jl .L570
	jmp .L568
	.p2align 2,0x90
.L570:
	call ln__Fv
.L569:
	incw -2(%ebp)
	jmp .L567
	.p2align 2,0x90
.L568:
	jmp .L571
	jmp .L566
	.p2align 2,0x90
.L571:
.L566:
	leave
	ret
.Lfe28:
		.size		 lns__Fi,.Lfe28-lns__Fi
	.section	.rodata
	.p2align 5
.LC27:
	.byte	 0x50,0x61,0x72,0x61,0x6d,0x65,0x74,0x72,0x20,0x6c
	.byte	 0x69,0x74,0x65,0x72,0x6f,0x77,0x79,0x21,0x20,0x5a
	.byte	 0x6d,0x6e,0x69,0x65,0x6e,0x69,0x61,0x6d,0x20,0x6e
	.byte	 0x61,0x20,0x30,0xa,0x0
.text
	.p2align 2,0x90
.globl takeint__FPi
		.type		 takeint__FPi,@function
takeint__FPi:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	pushl $.LC21
	call scanf
	addl $16,%esp
	addl $-4,%esp
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC11
	leal -80(%ebp),%eax
	pushl %eax
	call sscanf
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L575
	movl 8(%ebp),%eax
	movl $0,(%eax)
	addl $-8,%esp
	pushl $.LC27
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L575:
	jmp .L576
	jmp .L574
	.p2align 2,0x90
.L576:
.L574:
	leave
	ret
.Lfe29:
		.size		 takeint__FPi,.Lfe29-takeint__FPi
	.section	.rodata
	.p2align 5
.LC28:
	.byte	 0x50,0x61,0x72,0x61,0x6d,0x65,0x74,0x72,0x20,0x6c
	.byte	 0x69,0x74,0x65,0x72,0x6f,0x77,0x79,0x21,0x20,0x5a
	.byte	 0x6d,0x69,0x65,0x6e,0x69,0x61,0x6d,0x20,0x6e,0x61
	.byte	 0x20,0x30,0xa,0x0
.text
	.p2align 2,0x90
.globl takeint__FRi
		.type		 takeint__FRi,@function
takeint__FRi:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	pushl $.LC21
	call scanf
	addl $16,%esp
	addl $-4,%esp
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC11
	leal -80(%ebp),%eax
	pushl %eax
	call sscanf
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L578
	movl 8(%ebp),%eax
	movl $0,(%eax)
	addl $-8,%esp
	pushl $.LC28
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L578:
	jmp .L579
	jmp .L577
	.p2align 2,0x90
.L579:
.L577:
	leave
	ret
.Lfe30:
		.size		 takeint__FRi,.Lfe30-takeint__FRi
	.p2align 2,0x90
.globl takeint__FPUi
		.type		 takeint__FPUi,@function
takeint__FPUi:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	pushl $.LC21
	call scanf
	addl $16,%esp
	addl $-4,%esp
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC11
	leal -80(%ebp),%eax
	pushl %eax
	call sscanf
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L581
	movl 8(%ebp),%eax
	movl $0,(%eax)
	addl $-8,%esp
	pushl $.LC27
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L581:
	jmp .L582
	jmp .L580
	.p2align 2,0x90
.L582:
.L580:
	leave
	ret
.Lfe31:
		.size		 takeint__FPUi,.Lfe31-takeint__FPUi
	.p2align 2,0x90
.globl takeint__FRUi
		.type		 takeint__FRUi,@function
takeint__FRUi:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	pushl $.LC21
	call scanf
	addl $16,%esp
	addl $-4,%esp
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC11
	leal -80(%ebp),%eax
	pushl %eax
	call sscanf
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L584
	movl 8(%ebp),%eax
	movl $0,(%eax)
	addl $-8,%esp
	pushl $.LC28
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L584:
	jmp .L585
	jmp .L583
	.p2align 2,0x90
.L585:
.L583:
	leave
	ret
.Lfe32:
		.size		 takeint__FRUi,.Lfe32-takeint__FRUi
	.p2align 2,0x90
.globl downcase__FPc
		.type		 downcase__FPc,@function
downcase__FPc:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L590:
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%eax
	cmpl %eax,-4(%ebp)
	jl .L593
	jmp .L591
	.p2align 2,0x90
.L593:
	movl 8(%ebp),%eax
	movl -4(%ebp),%edx
	addl %edx,%eax
	cmpb $64,(%eax)
	jle .L592
	movl 8(%ebp),%eax
	movl -4(%ebp),%edx
	addl %edx,%eax
	cmpb $90,(%eax)
	jg .L592
	movl 8(%ebp),%eax
	movl -4(%ebp),%edx
	addl %edx,%eax
	movl 8(%ebp),%edx
	movl -4(%ebp),%ecx
	addl %ecx,%edx
	movb (%edx),%cl
	addb $32,%cl
	movb %cl,(%eax)
.L594:
.L592:
	incl -4(%ebp)
	jmp .L590
	.p2align 2,0x90
.L591:
	jmp .L595
	jmp .L589
	.p2align 2,0x90
.L595:
.L589:
	leave
	ret
.Lfe33:
		.size		 downcase__FPc,.Lfe33-downcase__FPc
	.section	.rodata
.LC29:
	.byte	 0x42,0x72,0x61,0x6b,0x5f,0x6e,0x61,0x7a,0x77,0x79
	.byte	 0x0
.LC30:
	.byte	 0x28,0x6e,0x75,0x6c,0x6c,0x29,0x0
.LC31:
	.byte	 0x30,0x30,0x30,0x2d,0x30,0x30,0x30,0x0
.LC32:
	.byte	 0x6e,0x6f,0x2d,0x6d,0x61,0x69,0x6c,0x0
.text
	.p2align 2,0x90
.globl __4TELE
		.type		 __4TELE,@function
__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl $0,(%ebx)
	addl $-8,%esp
	pushl $.LC29
	leal 4(%ebx),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L597:
	cmpl $1199,-4(%ebp)
	jle .L600
	jmp .L601
	.p2align 2,0x90
.L600:
	addl $-8,%esp
	pushl $.LC30
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC31
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC32
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
.L599:
	incl -4(%ebp)
	jmp .L597
	.p2align 2,0x90
.L598:
.L601:
	movl %ebx,%eax
	jmp .L596
.L596:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe34:
		.size		 __4TELE,.Lfe34-__4TELE
	.section	.rodata
.LC33:
	.byte	 0x55,0x77,0x6f,0x6c,0x6e,0x69,0x65,0x6e,0x69,0x65
	.byte	 0x20,0x25,0x64,0x20,0x4b,0x42,0x20,0x70,0x61,0x6d
	.byte	 0x69,0x65,0x63,0x69,0x2e,0xa,0x0
	.p2align 5
.LC34:
	.byte	 0x4e,0x61,0x63,0x69,0x73,0x6e,0x69,0x6a,0x20,0x63
	.byte	 0x6f,0x6b,0x6f,0x6c,0x77,0x69,0x65,0x6b,0x20,0x61
	.byte	 0x62,0x79,0x20,0x7a,0x61,0x6b,0x6f,0x6e,0x63,0x7a
	.byte	 0x79,0x63,0x20,0x64,0x7a,0x69,0x61,0x6c,0x61,0x6e
	.byte	 0x69,0x65,0x20,0x70,0x72,0x6f,0x67,0x72,0x61,0x6d
	.byte	 0x75,0x2e,0x20,0x2e,0x20,0x2e,0xa,0x0
.text
	.p2align 2,0x90
.globl _$_4TELE
		.type		 _$_4TELE,@function
_$_4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $281
	pushl $.LC33
	call printf
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC34
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L603:
	movl 12(%ebp),%eax
	andl $1,%eax
	testl %eax,%eax
	je .L605
	addl $-12,%esp
	pushl %ebx
	call __builtin_delete
	addl $16,%esp
	jmp .L605
	.p2align 2,0x90
.L604:
.L605:
.L602:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe35:
		.size		 _$_4TELE,.Lfe35-_$_4TELE
	.p2align 2,0x90
.globl PokazNazwe__4TELE
		.type		 PokazNazwe__4TELE,@function
PokazNazwe__4TELE:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	leal 4(%edx),%ecx
	movl %ecx,%eax
	jmp .L606
.L606:
	leave
	ret
.Lfe36:
		.size		 PokazNazwe__4TELE,.Lfe36-PokazNazwe__4TELE
	.section	.rodata
	.p2align 5
.LC35:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x61,0x7a,0x77
	.byte	 0x65,0x20,0x64,0x6c,0x61,0x20,0x73,0x70,0x69,0x73
	.byte	 0x75,0x20,0x74,0x65,0x6c,0x65,0x66,0x6f,0x6e,0x6f
	.byte	 0x77,0x3a,0x20,0x0
.text
	.p2align 2,0x90
.globl NadajNazwe__4TELE
		.type		 NadajNazwe__4TELE,@function
NadajNazwe__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $.LC35
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $20
	leal 4(%ebx),%eax
	pushl %eax
	call takestring__FPci
	addl $16,%esp
.L607:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe37:
		.size		 NadajNazwe__4TELE,.Lfe37-NadajNazwe__4TELE
	.section	.rodata
	.p2align 5
.LC36:
	.byte	 0x54,0x6f,0x20,0x6a,0x75,0x7a,0x20,0x6d,0x61,0x6b
	.byte	 0x73,0x79,0x6d,0x61,0x6c,0x6e,0x61,0x20,0x69,0x6c
	.byte	 0x6f,0x73,0x63,0x20,0x74,0x65,0x6c,0x65,0x66,0x6f
	.byte	 0x6e,0x6f,0x77,0x3a,0x20,0x0
.LC37:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x61,0x7a,0x77
	.byte	 0x65,0x20,0x74,0x65,0x6c,0x65,0x66,0x6f,0x6e,0x75
	.byte	 0x3a,0x20,0x0
.LC38:
	.byte	 0x3a,0x20,0x0
.LC39:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x75,0x6d,0x65
	.byte	 0x72,0x20,0x64,0x6f,0x20,0x0
.LC40:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x65,0x2d,0x6d,0x61
	.byte	 0x69,0x6c,0x61,0x20,0x64,0x6f,0x20,0x0
.LC41:
	.byte	 0x20,0x6d,0x61,0x69,0x6c,0x3a,0x20,0x0
.LC42:
	.byte	 0x20,0x0
.LC43:
	.byte	 0x44,0x6f,0x64,0x61,0x6e,0x6f,0x20,0x6e,0x61,0x73
	.byte	 0x74,0x65,0x70,0x75,0x6a,0x6f,0x63,0x79,0x20,0x74
	.byte	 0x65,0x6c,0x65,0x66,0x6f,0x6e,0x20,0x0
.LC44:
	.byte	 0x44,0x6f,0x64,0x61,0x63,0x20,0x6e,0x61,0x73,0x74
	.byte	 0x65,0x70,0x6e,0x79,0x3f,0x20,0x0
.text
	.p2align 2,0x90
.globl DodajTelefon__4TELE
		.type		 DodajTelefon__4TELE,@function
DodajTelefon__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movb $116,-1(%ebp)
	.p2align 2,0x90
.L609:
	cmpb $116,-1(%ebp)
	je .L611
	jmp .L610
	.p2align 2,0x90
.L611:
	cmpl $1200,(%ebx)
	jne .L612
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	pushl $1200
	addl $-8,%esp
	pushl $.LC36
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreami
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	jmp .L608
	.p2align 2,0x90
.L612:
	addl $-8,%esp
	pushl $.LC29
	leal 4(%ebx),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L613
	addl $-12,%esp
	pushl %ebx
	call NadajNazwe__4TELE
	addl $16,%esp
.L613:
	addl $-8,%esp
	pushl $.LC37
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $0
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call takestring__FPci
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC38
	addl $-8,%esp
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC39
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $0
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call takestring__FPci
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC38
	addl $-8,%esp
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC40
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $0
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call takestring__FPci
	addl $16,%esp
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC41
	addl $-8,%esp
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC42
	addl $-8,%esp
	movl (%ebx),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC43
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	incl (%ebx)
	addl $-8,%esp
	pushl $.LC44
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	jmp .L609
	.p2align 2,0x90
.L610:
.L608:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe38:
		.size		 DodajTelefon__4TELE,.Lfe38-DodajTelefon__4TELE
	.section	.rodata
	.p2align 5
.LC45:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6a,0x61,0x6b,0x61
	.byte	 0x20,0x6c,0x69,0x74,0x65,0x72,0x65,0x20,0x63,0x68
	.byte	 0x63,0x65,0x73,0x7a,0x20,0x7a,0x6f,0x62,0x61,0x63
	.byte	 0x7a,0x79,0x63,0x3a,0x20,0x0
.LC46:
	.byte	 0x57,0x73,0x7a,0x79,0x73,0x74,0x6b,0x69,0x65,0x20
	.byte	 0x74,0x65,0x6c,0x65,0x66,0x6f,0x6e,0x79,0x20,0x6e
	.byte	 0x61,0x20,0x6c,0x69,0x74,0x65,0x72,0x65,0x20,0x0
.text
	.p2align 2,0x90
.globl ZobaczTelefony__4TELE
		.type		 ZobaczTelefony__4TELE,@function
ZobaczTelefony__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $.LC45
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call downcase__FRc
	addl $16,%esp
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	movsbl -1(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC46
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	addl $-8,%esp
	movsbl -1(%ebp),%eax
	pushl %eax
	pushl %ebx
	call Zobacz__4TELEc
	addl $16,%esp
.L614:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe39:
		.size		 ZobaczTelefony__4TELE,.Lfe39-ZobaczTelefony__4TELE
	.section	.rodata
.LC47:
	.byte	 0x2c,0x20,0x74,0x65,0x6c,0x65,0x66,0x6f,0x6e,0x6f
	.byte	 0x77,0x20,0x0
.LC48:
	.byte	 0x4c,0x49,0x54,0x45,0x52,0x41,0x3a,0x9,0x0
	.p2align 5
.LC49:
	.byte	 0x25,0x2d,0x32,0x64,0x29,0x20,0x25,0x2d,0x33,0x30
	.byte	 0x73,0x20,0x4e,0x55,0x4d,0x45,0x52,0x3a,0x20,0x25
	.byte	 0x2d,0x32,0x30,0x73,0x20,0x6d,0x61,0x69,0x6c,0x3a
	.byte	 0x20,0x25,0x2d,0x32,0x30,0x73,0xa,0x0
.text
	.p2align 2,0x90
.globl Zobacz__4TELEc
		.type		 Zobacz__4TELEc,@function
Zobacz__4TELEc:
	pushl %ebp
	movl %esp,%ebp
	subl $36,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%eax
	movb %al,-1(%ebp)
	movl $0,-8(%ebp)
	movl $0,-12(%ebp)
	.p2align 2,0x90
.L616:
	movl -12(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L619
	jmp .L617
	.p2align 2,0x90
.L619:
	movl -12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movb (%edx,%eax),%dl
	cmpb -1(%ebp),%dl
	je .L621
	movl -12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%ecx
	movsbl (%edx,%ecx),%eax
	movsbl -1(%ebp),%edx
	leal -32(%edx),%ecx
	cmpl %ecx,%eax
	je .L621
	jmp .L618
	.p2align 2,0x90
.L621:
	incl -8(%ebp)
.L620:
.L618:
	incl -12(%ebp)
	jmp .L616
	.p2align 2,0x90
.L617:
	cmpl $0,-8(%ebp)
	je .L622
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC47
	addl $-8,%esp
	movsbl -1(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC48
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreami
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
.L622:
	movl $0,-8(%ebp)
	movl $0,-16(%ebp)
	.p2align 2,0x90
.L623:
	movl -16(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L626
	jmp .L624
	.p2align 2,0x90
.L626:
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movb (%edx,%eax),%dl
	cmpb -1(%ebp),%dl
	je .L628
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%ecx
	movsbl (%edx,%ecx),%eax
	movsbl -1(%ebp),%edx
	leal -32(%edx),%ecx
	cmpl %ecx,%eax
	je .L628
	jmp .L625
	.p2align 2,0x90
.L628:
	incl -8(%ebp)
	addl $-12,%esp
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC49
	call printf
	addl $32,%esp
.L627:
.L625:
	incl -16(%ebp)
	jmp .L623
	.p2align 2,0x90
.L624:
	cmpl $0,-8(%ebp)
	je .L629
	addl $-12,%esp
	pushl $2
	call lns__Fi
	addl $16,%esp
.L629:
.L615:
	movl -40(%ebp),%ebx
	leave
	ret
.Lfe40:
		.size		 Zobacz__4TELEc,.Lfe40-Zobacz__4TELEc
	.section	.rodata
	.p2align 5
.LC50:
	.byte	 0x53,0x70,0x69,0x73,0x20,0x6e,0x69,0x65,0x20,0x6d
	.byte	 0x61,0x20,0x6e,0x61,0x64,0x61,0x6e,0x65,0x6a,0x20
	.byte	 0x6e,0x61,0x7a,0x77,0x79,0x2c,0x20,0x6e,0x69,0x65
	.byte	 0x20,0x6d,0x6f,0x67,0x65,0x20,0x67,0x6f,0x20,0x7a
	.byte	 0x61,0x70,0x69,0x73,0x61,0x63,0x2e,0xa,0x0
	.p2align 5
.LC51:
	.byte	 0x4e,0x61,0x64,0x61,0x6a,0x20,0x6e,0x61,0x7a,0x77
	.byte	 0x65,0x20,0x75,0x7a,0x79,0x77,0x61,0x6a,0x61,0x63
	.byte	 0x20,0x70,0x6f,0x6c,0x65,0x63,0x65,0x6e,0x69,0x61
	.byte	 0x20,0x64,0x2e,0xa,0x0
.LC52:
	.byte	 0x44,0x61,0x74,0x61,0x2f,0x0
.LC53:
	.byte	 0x2e,0x64,0x61,0x74,0x0
.LC54:
	.byte	 0x72,0x2b,0x0
.LC55:
	.byte	 0x20,0x69,0x73,0x74,0x6e,0x69,0x65,0x6a,0x65,0x2c
	.byte	 0x20,0x6e,0x61,0x64,0x70,0x69,0x73,0x61,0x63,0x20
	.byte	 0x67,0x6f,0x3f,0x20,0x0
.LC56:
	.byte	 0x50,0x6c,0x69,0x6b,0x20,0x0
.LC57:
	.byte	 0x77,0x2b,0x0
.LC58:
	.byte	 0x20,0x64,0x6f,0x20,0x7a,0x61,0x70,0x69,0x73,0x75
	.byte	 0x2e,0xa,0x0
.LC59:
	.byte	 0x4e,0x69,0x65,0x20,0x6d,0x6f,0x67,0x65,0x20,0x6f
	.byte	 0x74,0x77,0x6f,0x72,0x7a,0x79,0x63,0x20,0x70,0x6c
	.byte	 0x69,0x6b,0x75,0x20,0x0
.LC60:
	.byte	 0x25,0x73,0xa,0x0
.LC61:
	.byte	 0x25,0x64,0xa,0x0
.LC62:
	.byte	 0x4d,0x61,0x64,0x65,0x20,0x62,0x79,0x20,0x6d,0x6f
	.byte	 0x72,0x67,0x6f,0x74,0x68,0x64,0x62,0x6d,0x61,0xa
	.byte	 0x0
.text
	.p2align 2,0x90
.globl ZapiszTelefony__4TELE
		.type		 ZapiszTelefony__4TELE,@function
ZapiszTelefony__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $100,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $.LC29
	leal 4(%ebx),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L631
	addl $-8,%esp
	pushl $.LC50
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC51
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L630
	.p2align 2,0x90
.L631:
	addl $-8,%esp
	pushl $.LC52
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	leal 4(%ebx),%eax
	pushl %eax
	leal -80(%ebp),%eax
	pushl %eax
	call strcat
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC53
	leal -80(%ebp),%eax
	pushl %eax
	call strcat
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC54
	leal -80(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-84(%ebp)
	cmpl $0,-84(%ebp)
	je .L632
	addl $-8,%esp
	pushl $.LC55
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC56
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -85(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	addl $-12,%esp
	movl -84(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
	cmpb $116,-85(%ebp)
	je .L632
	jmp .L630
	.p2align 2,0x90
.L633:
.L632:
	addl $-8,%esp
	pushl $.LC57
	leal -80(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-84(%ebp)
	cmpl $0,-84(%ebp)
	jne .L634
	addl $-8,%esp
	pushl $.LC58
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC59
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L630
	.p2align 2,0x90
.L634:
	addl $-12,%esp
	pushl %ebx
	call CheckOut__4TELE
	addl $16,%esp
	addl $-4,%esp
	leal 4(%ebx),%eax
	pushl %eax
	pushl $.LC60
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-4,%esp
	movl (%ebx),%eax
	pushl %eax
	pushl $.LC61
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	movl $0,-92(%ebp)
	.p2align 2,0x90
.L635:
	movl -92(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L638
	jmp .L636
	.p2align 2,0x90
.L638:
	addl $-4,%esp
	movl -92(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	pushl $.LC60
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-4,%esp
	movl -92(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	pushl $.LC60
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-4,%esp
	movl -92(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	pushl $.LC60
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
.L637:
	incl -92(%ebp)
	jmp .L635
	.p2align 2,0x90
.L636:
	addl $-8,%esp
	pushl $.LC62
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	addl $-12,%esp
	movl -84(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L630:
	movl -104(%ebp),%ebx
	leave
	ret
.Lfe41:
		.size		 ZapiszTelefony__4TELE,.Lfe41-ZapiszTelefony__4TELE
	.section	.rodata
	.p2align 5
.LC63:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x61,0x7a,0x77
	.byte	 0x65,0x20,0x70,0x6c,0x69,0x6b,0x75,0x20,0x64,0x6f
	.byte	 0x20,0x77,0x63,0x7a,0x79,0x74,0x61,0x6e,0x69,0x61
	.byte	 0x3a,0x20,0x0
.LC64:
	.byte	 0x20,0x6e,0x69,0x65,0x20,0x69,0x73,0x74,0x6e,0x69
	.byte	 0x65,0x6a,0x65,0x2e,0xa,0x0
.text
	.p2align 2,0x90
.globl WczytajTelefony__4TELE
		.type		 WczytajTelefony__4TELE,@function
WczytajTelefony__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $180,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $.LC52
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC63
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $25
	leal -160(%ebp),%eax
	pushl %eax
	call takestring__FPci
	addl $16,%esp
	addl $-8,%esp
	leal -160(%ebp),%eax
	pushl %eax
	leal -80(%ebp),%eax
	pushl %eax
	call strcat
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC53
	leal -80(%ebp),%eax
	pushl %eax
	call strcat
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC54
	leal -80(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-164(%ebp)
	cmpl $0,-164(%ebp)
	jne .L640
	addl $-8,%esp
	pushl $.LC64
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC56
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L639
	.p2align 2,0x90
.L640:
	addl $-4,%esp
	leal 4(%ebx),%eax
	pushl %eax
	pushl $.LC60
	movl -164(%ebp),%eax
	pushl %eax
	call fscanf
	addl $16,%esp
	addl $-4,%esp
	pushl %ebx
	pushl $.LC61
	movl -164(%ebp),%eax
	pushl %eax
	call fscanf
	addl $16,%esp
	movl $0,-168(%ebp)
	.p2align 2,0x90
.L641:
	movl -168(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L644
	jmp .L642
	.p2align 2,0x90
.L644:
	addl $-4,%esp
	movl -168(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	pushl $.LC60
	movl -164(%ebp),%eax
	pushl %eax
	call fscanf
	addl $16,%esp
	addl $-4,%esp
	movl -168(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	pushl $.LC60
	movl -164(%ebp),%eax
	pushl %eax
	call fscanf
	addl $16,%esp
	addl $-4,%esp
	movl -168(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	pushl $.LC60
	movl -164(%ebp),%eax
	pushl %eax
	call fscanf
	addl $16,%esp
.L643:
	incl -168(%ebp)
	jmp .L641
	.p2align 2,0x90
.L642:
	addl $-12,%esp
	movl -164(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
	addl $-12,%esp
	pushl %ebx
	call CheckOut__4TELE
	addl $16,%esp
.L639:
	movl -184(%ebp),%ebx
	leave
	ret
.Lfe42:
		.size		 WczytajTelefony__4TELE,.Lfe42-WczytajTelefony__4TELE
	.section	.rodata
	.p2align 5
.LC65:
	.byte	 0x4e,0x61,0x20,0x6a,0x61,0x6b,0x61,0x20,0x6c,0x69
	.byte	 0x74,0x65,0x72,0x65,0x20,0x7a,0x61,0x63,0x7a,0x79
	.byte	 0x6e,0x61,0x20,0x73,0x69,0x65,0x20,0x74,0x65,0x6c
	.byte	 0x65,0x66,0x6f,0x6e,0x3a,0x20,0x0
.LC66:
	.byte	 0x25,0x34,0x64,0x29,0x20,0x25,0x73,0xa,0x0
	.p2align 5
.LC67:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x75,0x6d,0x65
	.byte	 0x72,0x20,0x70,0x6f,0x64,0x20,0x6b,0x74,0x6f,0x72
	.byte	 0x79,0x6d,0x20,0x7a,0x6e,0x61,0x6a,0x64,0x75,0x6a
	.byte	 0x65,0x20,0x73,0x69,0x65,0x20,0x74,0x65,0x6c,0x65
	.byte	 0x66,0x6f,0x6e,0x20,0x64,0x6f,0x20,0x75,0x73,0x75
	.byte	 0x6e,0x69,0x65,0x63,0x69,0x61,0x3a,0x20,0x0
.LC68:
	.byte	 0x50,0x6f,0x7a,0x61,0x20,0x7a,0x61,0x6b,0x72,0x65
	.byte	 0x73,0x65,0x6d,0x20,0x74,0x61,0x62,0x65,0x6c,0x69
	.byte	 0x2e,0x20,0x2e,0x20,0x2e,0xa,0x0
.LC69:
	.byte	 0x55,0x73,0x75,0x77,0x61,0x63,0x20,0x64,0x61,0x6c
	.byte	 0x65,0x6a,0x3f,0x20,0x0
.text
	.p2align 2,0x90
.globl UsunTelefon__4TELE
		.type		 UsunTelefon__4TELE,@function
UsunTelefon__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $36,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movb $116,-1(%ebp)
	.p2align 2,0x90
.L646:
	cmpb $116,-1(%ebp)
	je .L648
	jmp .L647
	.p2align 2,0x90
.L648:
	addl $-8,%esp
	pushl $.LC65
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -2(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	addl $-12,%esp
	leal -2(%ebp),%eax
	pushl %eax
	call downcase__FRc
	addl $16,%esp
	movl $0,-8(%ebp)
	.p2align 2,0x90
.L649:
	movl -8(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L652
	jmp .L650
	.p2align 2,0x90
.L652:
	movl -8(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movb (%edx,%eax),%dl
	cmpb -2(%ebp),%dl
	je .L654
	movl -8(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%ecx
	movsbl (%edx,%ecx),%eax
	movsbl -2(%ebp),%edx
	leal -32(%edx),%ecx
	cmpl %ecx,%eax
	je .L654
	jmp .L651
	.p2align 2,0x90
.L654:
	addl $-4,%esp
	movl -8(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -8(%ebp),%eax
	pushl %eax
	pushl $.LC66
	call printf
	addl $16,%esp
.L653:
.L651:
	incl -8(%ebp)
	jmp .L649
	.p2align 2,0x90
.L650:
	addl $-8,%esp
	pushl $.LC67
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -12(%ebp),%eax
	pushl %eax
	call takeint__FPi
	addl $16,%esp
	cmpl $0,-12(%ebp)
	jl .L656
	movl (%ebx),%eax
	decl %eax
	cmpl %eax,-12(%ebp)
	jg .L656
	jmp .L655
	.p2align 2,0x90
.L656:
	addl $-8,%esp
	pushl $.LC68
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L645
	.p2align 2,0x90
.L655:
	decl (%ebx)
	movl -12(%ebp),%eax
	movl %eax,-16(%ebp)
	.p2align 2,0x90
.L657:
	movl -16(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L660
	jmp .L658
	.p2align 2,0x90
.L660:
	addl $-8,%esp
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 164(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96164(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192164(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
.L659:
	incl -16(%ebp)
	jmp .L657
	.p2align 2,0x90
.L658:
	addl $-8,%esp
	pushl $.LC69
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	jmp .L646
	.p2align 2,0x90
.L647:
.L645:
	movl -40(%ebp),%ebx
	leave
	ret
.Lfe43:
		.size		 UsunTelefon__4TELE,.Lfe43-UsunTelefon__4TELE
	.p2align 2,0x90
.globl Zamien__4TELEii
		.type		 Zamien__4TELEii,@function
Zamien__4TELEii:
	pushl %ebp
	movl %esp,%ebp
	subl $100,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl 16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	movl 16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl 16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	movl 16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	movl 16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	movl 16(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call strcpy
	addl $16,%esp
.L661:
	movl -104(%ebp),%ebx
	leave
	ret
.Lfe44:
		.size		 Zamien__4TELEii,.Lfe44-Zamien__4TELEii
	.p2align 2,0x90
.globl Sortuj__4TELE
		.type		 Sortuj__4TELE,@function
Sortuj__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L663:
	movl -4(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L666
	jmp .L664
	.p2align 2,0x90
.L666:
	movl $0,-8(%ebp)
	.p2align 2,0x90
.L667:
	movl (%ebx),%eax
	decl %eax
	movl %eax,%edx
	subl -4(%ebp),%edx
	cmpl %edx,-8(%ebp)
	jl .L670
	jmp .L665
	.p2align 2,0x90
.L670:
	addl $-8,%esp
	movl -8(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 164(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -8(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call StrMore__FPcT0
	addl $16,%esp
	movb %al,%al
	testb %al,%al
	je .L669
	addl $-4,%esp
	movl -8(%ebp),%eax
	pushl %eax
	movl -8(%ebp),%eax
	incl %eax
	pushl %eax
	pushl %ebx
	call Zamien__4TELEii
	addl $16,%esp
.L671:
.L669:
	incl -8(%ebp)
	jmp .L667
	.p2align 2,0x90
.L668:
.L665:
	incl -4(%ebp)
	jmp .L663
	.p2align 2,0x90
.L664:
.L662:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe45:
		.size		 Sortuj__4TELE,.Lfe45-Sortuj__4TELE
	.section	.rodata
	.p2align 5
.LC70:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x75,0x6d,0x65
	.byte	 0x72,0x20,0x70,0x6f,0x64,0x20,0x6b,0x74,0x6f,0x72
	.byte	 0x79,0x6d,0x20,0x7a,0x6e,0x61,0x6a,0x64,0x75,0x6a
	.byte	 0x65,0x20,0x73,0x69,0x65,0x20,0x74,0x65,0x6c,0x65
	.byte	 0x66,0x6f,0x6e,0x20,0x64,0x6f,0x20,0x6d,0x6f,0x64
	.byte	 0x79,0x66,0x69,0x6b,0x61,0x63,0x6a,0x69,0x3a,0x20
	.byte	 0x0
.LC71:
	.byte	 0x4d,0x6f,0x64,0x79,0x66,0x69,0x6b,0x6f,0x77,0x61
	.byte	 0x63,0x20,0x64,0x61,0x6c,0x65,0x6a,0x3f,0x20,0x0
.text
	.p2align 2,0x90
.globl ModyfikujTelefon__4TELE
		.type		 ModyfikujTelefon__4TELE,@function
ModyfikujTelefon__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movb $116,-1(%ebp)
	.p2align 2,0x90
.L673:
	cmpb $116,-1(%ebp)
	je .L675
	jmp .L674
	.p2align 2,0x90
.L675:
	addl $-12,%esp
	pushl %ebx
	call Szukaj__4TELE
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC70
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -8(%ebp),%eax
	pushl %eax
	call takeint__FPi
	addl $16,%esp
	cmpl $0,-8(%ebp)
	jl .L677
	movl (%ebx),%eax
	decl %eax
	cmpl %eax,-8(%ebp)
	jg .L677
	jmp .L676
	.p2align 2,0x90
.L677:
	addl $-8,%esp
	pushl $.LC68
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L672
	.p2align 2,0x90
.L676:
	addl $-8,%esp
	movl -8(%ebp),%eax
	pushl %eax
	pushl %ebx
	call Modyfikuj__4TELEi
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC71
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	jmp .L673
	.p2align 2,0x90
.L674:
.L672:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe46:
		.size		 ModyfikujTelefon__4TELE,.Lfe46-ModyfikujTelefon__4TELE
	.section	.rodata
.LC72:
	.byte	 0x20,0x4e,0x55,0x4d,0x45,0x52,0x20,0x0
.LC73:
	.byte	 0x41,0x6b,0x74,0x75,0x61,0x6c,0x6e,0x69,0x65,0x20
	.byte	 0x0
.LC74:
	.byte	 0x4d,0x6f,0x64,0x79,0x66,0x69,0x6b,0x6f,0x77,0x61
	.byte	 0x63,0x20,0x6e,0x61,0x7a,0x77,0x65,0x3f,0x20,0x0
.LC75:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x6f,0x77,0x61
	.byte	 0x20,0x6e,0x61,0x7a,0x77,0x65,0x3a,0x20,0x0
.LC76:
	.byte	 0x4d,0x6f,0x64,0x79,0x66,0x69,0x6b,0x6f,0x77,0x61
	.byte	 0x63,0x20,0x6e,0x75,0x6d,0x65,0x72,0x3f,0x20,0x0
.LC77:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x6f,0x77,0x79
	.byte	 0x20,0x6e,0x75,0x6d,0x65,0x72,0x3a,0x20,0x0
.LC78:
	.byte	 0x4d,0x6f,0x64,0x79,0x66,0x69,0x6b,0x6f,0x77,0x61
	.byte	 0x63,0x20,0x65,0x2d,0x6d,0x61,0x69,0x6c,0x3f,0x20
	.byte	 0x0
.LC79:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x6e,0x6f,0x77,0x79
	.byte	 0x20,0x65,0x2d,0x6d,0x61,0x69,0x6c,0x3a,0x20,0x0
.text
	.p2align 2,0x90
.globl Modyfikuj__4TELEi
		.type		 Modyfikuj__4TELEi,@function
Modyfikuj__4TELEi:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC41
	addl $-8,%esp
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC72
	addl $-8,%esp
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC73
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC74
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	cmpb $116,-1(%ebp)
	jne .L679
	addl $-8,%esp
	pushl $.LC75
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $0
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call takestring__FPci
	addl $16,%esp
.L679:
	addl $-8,%esp
	pushl $.LC76
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	cmpb $116,-1(%ebp)
	jne .L680
	addl $-8,%esp
	pushl $.LC77
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $0
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call takestring__FPci
	addl $16,%esp
.L680:
	addl $-8,%esp
	pushl $.LC78
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	cmpb $116,-1(%ebp)
	jne .L681
	addl $-8,%esp
	pushl $.LC79
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $0
	movl 12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call takestring__FPci
	addl $16,%esp
.L681:
.L678:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe47:
		.size		 Modyfikuj__4TELEi,.Lfe47-Modyfikuj__4TELEi
	.section	.rodata
.LC80:
	.byte	 0x25,0x34,0x64,0x29,0x20,0x25,0x2d,0x33,0x30,0x73
	.byte	 0x20,0x25,0x2d,0x32,0x30,0x73,0x20,0x6d,0x61,0x69
	.byte	 0x6c,0x3a,0x20,0x25,0x2d,0x32,0x30,0x73,0xa,0x0
.text
	.p2align 2,0x90
.globl ZobaczWszystko__4TELE
		.type		 ZobaczWszystko__4TELE,@function
ZobaczWszystko__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl $0,-4(%ebp)
	movb $97,-5(%ebp)
	.p2align 2,0x90
.L683:
	cmpb $122,-5(%ebp)
	jle .L686
	jmp .L684
	.p2align 2,0x90
.L686:
	movl $0,-12(%ebp)
	.p2align 2,0x90
.L687:
	movl -12(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L690
	jmp .L685
	.p2align 2,0x90
.L690:
	movl -12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movb (%edx,%eax),%dl
	cmpb -5(%ebp),%dl
	je .L692
	movl -12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%ecx
	movsbl (%edx,%ecx),%eax
	movsbl -5(%ebp),%edx
	leal -32(%edx),%ecx
	cmpl %ecx,%eax
	je .L692
	jmp .L689
	.p2align 2,0x90
.L692:
	incl -4(%ebp)
	addl $-12,%esp
	movl -12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -12(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -4(%ebp),%eax
	pushl %eax
	pushl $.LC80
	call printf
	addl $32,%esp
.L691:
.L689:
	incl -12(%ebp)
	jmp .L687
	.p2align 2,0x90
.L688:
.L685:
	incb -5(%ebp)
	jmp .L683
	.p2align 2,0x90
.L684:
.L682:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe48:
		.size		 ZobaczWszystko__4TELE,.Lfe48-ZobaczWszystko__4TELE
	.section	.rodata
.LC81:
	.byte	 0x50,0x6f,0x64,0x61,0x6a,0x20,0x77,0x7a,0x6f,0x72
	.byte	 0x7a,0x65,0x63,0x3a,0x20,0x0
.LC82:
	.byte	 0x57,0x7a,0x6f,0x72,0x7a,0x65,0x63,0x3a,0x20,0x0
.text
	.p2align 2,0x90
.globl Szukaj__4TELE
		.type		 Szukaj__4TELE,@function
Szukaj__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $180,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $.LC81
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $15
	leal -80(%ebp),%eax
	pushl %eax
	call takestring__FPci
	addl $16,%esp
	movl $0,-84(%ebp)
	.p2align 2,0x90
.L694:
	addl $-12,%esp
	leal -80(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%eax
	cmpl %eax,-84(%ebp)
	jl .L697
	jmp .L695
	.p2align 2,0x90
.L697:
	addl $-12,%esp
	leal -80(%ebp),%eax
	movl %eax,%edx
	addl -84(%ebp),%edx
	pushl %edx
	call downcase__FRc
	addl $16,%esp
.L696:
	incl -84(%ebp)
	jmp .L694
	.p2align 2,0x90
.L695:
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC82
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	movl $0,-168(%ebp)
	.p2align 2,0x90
.L698:
	movl -168(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L701
	jmp .L699
	.p2align 2,0x90
.L701:
	addl $-8,%esp
	movl -168(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	leal -164(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	movl $0,-172(%ebp)
	.p2align 2,0x90
.L702:
	addl $-12,%esp
	leal -164(%ebp),%eax
	pushl %eax
	call strlen
	addl $16,%esp
	movl %eax,%eax
	cmpl %eax,-172(%ebp)
	jl .L705
	jmp .L703
	.p2align 2,0x90
.L705:
	addl $-12,%esp
	leal -164(%ebp),%eax
	movl %eax,%edx
	addl -172(%ebp),%edx
	pushl %edx
	call downcase__FRc
	addl $16,%esp
.L704:
	incl -172(%ebp)
	jmp .L702
	.p2align 2,0x90
.L703:
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	leal -164(%ebp),%eax
	pushl %eax
	call strstr
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	je .L700
	addl $-12,%esp
	movl -168(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -168(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -168(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -168(%ebp),%eax
	pushl %eax
	pushl $.LC80
	call printf
	addl $32,%esp
.L706:
.L700:
	incl -168(%ebp)
	jmp .L698
	.p2align 2,0x90
.L699:
.L693:
	movl -184(%ebp),%ebx
	leave
	ret
.Lfe49:
		.size		 Szukaj__4TELE,.Lfe49-Szukaj__4TELE
	.section	.rodata
	.p2align 5
.LC83:
	.byte	 0x53,0x70,0x69,0x73,0x20,0x6e,0x69,0x65,0x20,0x6d
	.byte	 0x61,0x20,0x6e,0x61,0x64,0x61,0x6e,0x65,0x6a,0x20
	.byte	 0x6e,0x61,0x7a,0x77,0x79,0x2c,0x20,0x6e,0x69,0x65
	.byte	 0x20,0x6d,0x6f,0x67,0x65,0x20,0x77,0x79,0x67,0x65
	.byte	 0x6e,0x65,0x72,0x6f,0x77,0x61,0x63,0x20,0x70,0x6c
	.byte	 0x69,0x6b,0x75,0x20,0x54,0x58,0x54,0x2e,0xa,0x0
.LC84:
	.byte	 0x2e,0x74,0x78,0x74,0x0
.LC85:
	.byte	 0x47,0x65,0x6e,0x65,0x72,0x75,0x6a,0x65,0x20,0x70
	.byte	 0x6c,0x69,0x6b,0x20,0x0
.LC86:
	.byte	 0x4c,0x49,0x54,0x45,0x52,0x41,0x9,0x25,0x63,0xa
	.byte	 0x0
.text
	.p2align 2,0x90
.globl GenerujTXT__4TELE
		.type		 GenerujTXT__4TELE,@function
GenerujTXT__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $116,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	addl $-8,%esp
	pushl $.LC29
	leal 4(%ebx),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L708
	addl $-8,%esp
	pushl $.LC83
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC51
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L707
	.p2align 2,0x90
.L708:
	addl $-8,%esp
	pushl $.LC52
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-8,%esp
	leal 4(%ebx),%eax
	pushl %eax
	leal -80(%ebp),%eax
	pushl %eax
	call strcat
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC84
	leal -80(%ebp),%eax
	pushl %eax
	call strcat
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC54
	leal -80(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-84(%ebp)
	cmpl $0,-84(%ebp)
	je .L709
	addl $-8,%esp
	pushl $.LC55
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC56
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -85(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	addl $-12,%esp
	movl -84(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
	cmpb $116,-85(%ebp)
	je .L709
	jmp .L707
	.p2align 2,0x90
.L710:
.L709:
	addl $-8,%esp
	pushl $.LC57
	leal -80(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-84(%ebp)
	cmpl $0,-84(%ebp)
	jne .L711
	addl $-8,%esp
	pushl $.LC58
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC59
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	jmp .L707
	.p2align 2,0x90
.L711:
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	leal -80(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC85
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	movl $0,-92(%ebp)
	movb $97,-85(%ebp)
	.p2align 2,0x90
.L712:
	cmpb $122,-85(%ebp)
	jle .L715
	jmp .L713
	.p2align 2,0x90
.L715:
	movl $0,-96(%ebp)
	movl $0,-100(%ebp)
	.p2align 2,0x90
.L716:
	movl -100(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L719
	jmp .L717
	.p2align 2,0x90
.L719:
	movl -100(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movb (%edx,%eax),%dl
	cmpb -85(%ebp),%dl
	je .L721
	movl -100(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%ecx
	movsbl (%edx,%ecx),%eax
	movsbl -85(%ebp),%edx
	leal -32(%edx),%ecx
	cmpl %ecx,%eax
	je .L721
	jmp .L718
	.p2align 2,0x90
.L721:
	incl -96(%ebp)
.L720:
.L718:
	incl -100(%ebp)
	jmp .L716
	.p2align 2,0x90
.L717:
	cmpl $0,-96(%ebp)
	je .L714
	addl $-4,%esp
	movsbl -85(%ebp),%eax
	pushl %eax
	pushl $.LC86
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
	movl $0,-104(%ebp)
	.p2align 2,0x90
.L723:
	movl -104(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L726
	jmp .L724
	.p2align 2,0x90
.L726:
	movl -104(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movb (%edx,%eax),%dl
	cmpb -85(%ebp),%dl
	je .L728
	movl -104(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%ecx
	movsbl (%edx,%ecx),%eax
	movsbl -85(%ebp),%edx
	leal -32(%edx),%ecx
	cmpl %ecx,%eax
	je .L728
	jmp .L725
	.p2align 2,0x90
.L728:
	incl -92(%ebp)
	addl $-8,%esp
	movl -104(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 192084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -104(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 96084(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -104(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	movl -92(%ebp),%eax
	pushl %eax
	pushl $.LC80
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $32,%esp
.L727:
.L725:
	incl -104(%ebp)
	jmp .L723
	.p2align 2,0x90
.L724:
	addl $-8,%esp
	pushl $.LC13
	movl -84(%ebp),%eax
	pushl %eax
	call fprintf
	addl $16,%esp
.L722:
.L714:
	incb -85(%ebp)
	jmp .L712
	.p2align 2,0x90
.L713:
	addl $-12,%esp
	movl -84(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
.L707:
	movl -120(%ebp),%ebx
	leave
	ret
.Lfe50:
		.size		 GenerujTXT__4TELE,.Lfe50-GenerujTXT__4TELE
	.section	.rodata
.LC87:
	.byte	 0x20,0x74,0x6f,0x20,0x6e,0x69,0x65,0x20,0x6a,0x65
	.byte	 0x73,0x74,0x20,0x6c,0x69,0x74,0x65,0x72,0x61,0x2e
	.byte	 0xa,0x0
	.p2align 5
.LC88:
	.byte	 0x20,0x6a,0x65,0x73,0x74,0x20,0x20,0x6e,0x69,0x65
	.byte	 0x70,0x6f,0x70,0x72,0x61,0x77,0x6e,0x79,0xa,0x4e
	.byte	 0x69,0x65,0x70,0x6f,0x70,0x72,0x61,0x77,0x6e,0x79
	.byte	 0x20,0x6a,0x65,0x73,0x74,0x20,0x70,0x69,0x65,0x72
	.byte	 0x77,0x73,0x7a,0x79,0x20,0x7a,0x6e,0x61,0x6b,0x20
	.byte	 0x0
.LC89:
	.byte	 0x29,0x57,0x70,0x69,0x73,0x20,0x0
.LC90:
	.byte	 0x28,0x0
.LC91:
	.byte	 0x5a,0x6d,0x69,0x65,0x6e,0x69,0x63,0x20,0x74,0x65
	.byte	 0x6e,0x20,0x77,0x70,0x69,0x73,0x3f,0x20,0x0
.LC92:
	.byte	 0x20,0x6e,0x61,0x20,0x0
.LC93:
	.byte	 0x5a,0x61,0x6d,0x69,0x65,0x6e,0x20,0x0
.text
	.p2align 2,0x90
.globl CheckOut__4TELE
		.type		 CheckOut__4TELE,@function
CheckOut__4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $20,%esp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl $0,-4(%ebp)
	.p2align 2,0x90
.L730:
	movl -4(%ebp),%eax
	cmpl (%ebx),%eax
	jl .L733
	jmp .L731
	.p2align 2,0x90
.L733:
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	cmpb $122,(%edx,%eax)
	jg .L735
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	cmpb $90,(%edx,%eax)
	jle .L736
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	cmpb $96,(%edx,%eax)
	jle .L735
	jmp .L736
	.p2align 2,0x90
.L736:
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	cmpb $64,(%edx,%eax)
	jle .L735
	jmp .L732
	.p2align 2,0x90
.L735:
	addl $-8,%esp
	pushl $.LC87
	addl $-8,%esp
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movsbl (%edx,%eax),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC88
	addl $-8,%esp
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC89
	addl $-8,%esp
	movl -4(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC90
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreami
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC91
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -5(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	cmpb $116,-5(%ebp)
	jne .L732
	addl $-8,%esp
	pushl $.LC92
	addl $-8,%esp
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%ebx),%eax
	movsbl (%edx,%eax),%edx
	pushl %edx
	addl $-8,%esp
	pushl $.LC93
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	movl -4(%ebp),%edx
	movl %edx,%eax
	sall $2,%eax
	addl %edx,%eax
	movl %eax,%edx
	sall $4,%edx
	leal 84(%edx),%eax
	leal (%eax,%ebx),%edx
	pushl %edx
	call takechar__FPc
	addl $16,%esp
.L737:
.L734:
.L732:
	incl -4(%ebp)
	jmp .L730
	.p2align 2,0x90
.L731:
.L729:
	movl -24(%ebp),%ebx
	leave
	ret
.Lfe51:
		.size		 CheckOut__4TELE,.Lfe51-CheckOut__4TELE
	.section	.rodata
.LC94:
	.byte	 0x42,0x72,0x61,0x6b,0x20,0x70,0x6c,0x69,0x6b,0x75
	.byte	 0x3a,0x20,0x0
.text
	.p2align 2,0x90
.globl ReadFile__FPc
		.type		 ReadFile__FPc,@function
ReadFile__FPc:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	addl $-12,%esp
	pushl $2
	call lns__Fi
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC54
	movl 8(%ebp),%eax
	pushl %eax
	call fopen
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-4(%ebp)
	cmpl $0,-4(%ebp)
	jne .L739
	addl $-8,%esp
	pushl $.LC13
	addl $-8,%esp
	movl 8(%ebp),%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC94
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	pushl $2
	call lns__Fi
	addl $16,%esp
	jmp .L738
	.p2align 2,0x90
.L739:
	.p2align 2,0x90
.L740:
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fgetc
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-8(%ebp)
	cmpl $-1,-8(%ebp)
	jne .L742
	jmp .L741
	.p2align 2,0x90
.L742:
	addl $-8,%esp
	movsbl -8(%ebp),%eax
	pushl %eax
	pushl $cout
	call __ls__7ostreamc
	addl $16,%esp
	jmp .L740
	.p2align 2,0x90
.L741:
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call fclose
	addl $16,%esp
	addl $-12,%esp
	pushl $2
	call lns__Fi
	addl $16,%esp
.L738:
	leave
	ret
.Lfe52:
		.size		 ReadFile__FPc,.Lfe52-ReadFile__FPc
	.section	.rodata
	.p2align 5
.LC95:
	.byte	 0x4b,0x6f,0x6e,0x63,0x7a,0x79,0x73,0x7a,0x20,0x64
	.byte	 0x7a,0x69,0x61,0x6c,0x61,0x6e,0x69,0x65,0x20,0x70
	.byte	 0x72,0x6f,0x67,0x72,0x61,0x6d,0x75,0x2c,0x20,0x7a
	.byte	 0x61,0x70,0x69,0x73,0x61,0x63,0x3f,0x20,0x0
.text
	.p2align 2,0x90
.globl Ask__FP4TELE
		.type		 Ask__FP4TELE,@function
Ask__FP4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	addl $-8,%esp
	pushl $.LC95
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-12,%esp
	leal -1(%ebp),%eax
	pushl %eax
	call takechar__FPc
	addl $16,%esp
	cmpb $116,-1(%ebp)
	jne .L744
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call ZapiszTelefony__4TELE
	addl $16,%esp
.L744:
.L743:
	leave
	ret
.Lfe53:
		.size		 Ask__FP4TELE,.Lfe53-Ask__FP4TELE
	.section	.rodata
.LC96:
	.byte	 0x68,0x0
.LC97:
	.byte	 0x44,0x69,0x61,0x6c,0x6f,0x67,0x2f,0x48,0x65,0x6c
	.byte	 0x70,0x0
.LC98:
	.byte	 0x64,0x0
.LC99:
	.byte	 0x7a,0x0
.LC100:
	.byte	 0x73,0x0
.LC101:
	.byte	 0x6b,0x0
.LC102:
	.byte	 0x6c,0x0
.LC103:
	.byte	 0x75,0x0
.LC104:
	.byte	 0x6d,0x0
.LC105:
	.byte	 0x61,0x0
.LC106:
	.byte	 0x67,0x0
.LC107:
	.byte	 0x63,0x0
.LC108:
	.byte	 0x74,0x0
	.p2align 5
.LC109:
	.byte	 0x4e,0x69,0x65,0x7a,0x6e,0x61,0x6e,0x65,0x20,0x70
	.byte	 0x6f,0x6c,0x65,0x63,0x65,0x6e,0x69,0x65,0x2c,0x20
	.byte	 0x77,0x70,0x69,0x73,0x7a,0x20,0x68,0x20,0x61,0x62
	.byte	 0x79,0x20,0x7a,0x6f,0x62,0x61,0x63,0x7a,0x79,0x63
	.byte	 0x20,0x6c,0x69,0x73,0x74,0x65,0x20,0x70,0x6f,0x6c
	.byte	 0x65,0x63,0x65,0x6e,0x2e,0xa,0x0
.text
	.p2align 2,0x90
.globl ExecuteCmd__FPcP4TELE
		.type		 ExecuteCmd__FPcP4TELE,@function
ExecuteCmd__FPcP4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	addl $-8,%esp
	pushl $.LC96
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L746
	addl $-12,%esp
	pushl $.LC97
	call ReadFile__FPc
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L746:
	addl $-8,%esp
	pushl $.LC98
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L748
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call DodajTelefon__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L748:
	addl $-8,%esp
	pushl $.LC99
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L750
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call ZobaczTelefony__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L750:
	addl $-8,%esp
	pushl $.LC100
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L752
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call ZapiszTelefony__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L752:
	addl $-8,%esp
	pushl $.LC101
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L754
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call Ask__FP4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L754:
	addl $-8,%esp
	pushl $.LC102
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L756
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call WczytajTelefony__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L756:
	addl $-8,%esp
	pushl $.LC103
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L758
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call UsunTelefon__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L758:
	addl $-8,%esp
	pushl $.LC104
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L760
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call ModyfikujTelefon__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L760:
	addl $-8,%esp
	pushl $.LC105
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L762
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call ZobaczWszystko__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L762:
	addl $-8,%esp
	pushl $.LC9
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L764
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call Szukaj__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L764:
	addl $-8,%esp
	pushl $.LC106
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L766
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call GenerujTXT__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L766:
	addl $-8,%esp
	pushl $.LC107
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L768
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call CheckOut__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L768:
	addl $-8,%esp
	pushl $.LC108
	movl 8(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L770
	addl $-12,%esp
	movl 12(%ebp),%eax
	pushl %eax
	call Sortuj__4TELE
	addl $16,%esp
	jmp .L747
	.p2align 2,0x90
.L770:
	addl $-8,%esp
	pushl $.LC109
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
.L771:
.L769:
.L767:
.L765:
.L763:
.L761:
.L759:
.L757:
.L755:
.L753:
.L751:
.L749:
.L747:
.L745:
	leave
	ret
.Lfe54:
		.size		 ExecuteCmd__FPcP4TELE,.Lfe54-ExecuteCmd__FPcP4TELE
	.section	.rodata
.LC110:
	.byte	 0x44,0x69,0x61,0x6c,0x6f,0x67,0x2f,0x49,0x6e,0x66
	.byte	 0x6f,0x0
.LC111:
	.byte	 0x20,0x2d,0x3e,0x20,0x0
.LC112:
	.byte	 0x4d,0x6f,0x72,0x67,0x6f,0x74,0x68,0x44,0x42,0x4d
	.byte	 0x41,0x20,0x4c,0x69,0x6e,0x75,0x78,0x20,0x0
.text
	.p2align 2,0x90
.globl Wykonaj__FP4TELE
		.type		 Wykonaj__FP4TELE,@function
Wykonaj__FP4TELE:
	pushl %ebp
	movl %esp,%ebp
	subl $88,%esp
	addl $-8,%esp
	pushl $.LC30
	leal -80(%ebp),%eax
	pushl %eax
	call strcpy
	addl $16,%esp
	addl $-12,%esp
	pushl $.LC110
	call ReadFile__FPc
	addl $16,%esp
	.p2align 2,0x90
.L773:
	addl $-8,%esp
	pushl $.LC101
	leal -80(%ebp),%eax
	pushl %eax
	call strcmp
	addl $16,%esp
	movl %eax,%eax
	testl %eax,%eax
	jne .L775
	jmp .L774
	.p2align 2,0x90
.L775:
	addl $-8,%esp
	pushl $.LC111
	addl $-8,%esp
	addl $-12,%esp
	movl 8(%ebp),%eax
	pushl %eax
	call PokazNazwe__4TELE
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	addl $-8,%esp
	pushl $.LC112
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	pushl $0
	leal -80(%ebp),%eax
	pushl %eax
	call takestring__FPci
	addl $16,%esp
	addl $-12,%esp
	leal -80(%ebp),%eax
	pushl %eax
	call downcase__FPc
	addl $16,%esp
	addl $-8,%esp
	movl 8(%ebp),%eax
	pushl %eax
	leal -80(%ebp),%eax
	pushl %eax
	call ExecuteCmd__FPcP4TELE
	addl $16,%esp
	jmp .L773
	.p2align 2,0x90
.L774:
.L772:
	leave
	ret
.Lfe55:
		.size		 Wykonaj__FP4TELE,.Lfe55-Wykonaj__FP4TELE
	.p2align 2,0x90
.globl main
		.type		 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	subl $76,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	call __get_eh_context
	movl %eax,%eax
	movl %eax,%edx
	movl %edx,%eax
	movl %eax,-64(%ebp)
	addl $-12,%esp
	pushl $288084
	call __builtin_new
	addl $16,%esp
	movl %eax,%eax
	movl %eax,-56(%ebp)
	movb $1,-57(%ebp)
	movl -64(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	movl %edx,-28(%ebp)
	movl $0,-24(%ebp)
	leal -20(%ebp),%edx
	movl %ebp,(%edx)
	movl $.L777,%ecx
	movl %ecx,4(%edx)
	movl %esp,8(%edx)
	leal -28(%ebp),%edx
	movl %edx,(%eax)
	addl $-12,%esp
	movl -56(%ebp),%eax
	pushl %eax
	call __4TELE
	addl $16,%esp
	movl %eax,%eax
	movb $0,-57(%ebp)
	movl %eax,-4(%ebp)
	movl -64(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	movl (%edx),%ecx
	movl %ecx,(%eax)
	cmpb $0,-57(%ebp)
	je .L780
	addl $-12,%esp
	movl -56(%ebp),%eax
	pushl %eax
	call __builtin_delete
	addl $16,%esp
	jmp .L780
	.p2align 2,0x90
.L779:
.L780:
	addl $-12,%esp
	movl -4(%ebp),%eax
	pushl %eax
	call Wykonaj__FP4TELE
	addl $16,%esp
	cmpl $0,-4(%ebp)
	je .L782
	addl $-8,%esp
	pushl $3
	movl -4(%ebp),%eax
	pushl %eax
	call _$_4TELE
	addl $16,%esp
	jmp .L782
	.p2align 2,0x90
.L781:
.L782:
	call getch__Fv
	xorl %eax,%eax
	jmp .L776
	xorl %eax,%eax
	jmp .L776
	jmp .L789
	jmp .L783
	.p2align 2,0x90
.L777:
	movl %ebp,%ebp
#APP
	
#NO_APP
.L783:
	movl -64(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	movl %edx,-52(%ebp)
	movl $0,-48(%ebp)
	leal -44(%ebp),%edx
	movl %ebp,(%edx)
	movl $.L784,%ecx
	movl %ecx,4(%edx)
	movl %esp,8(%edx)
	leal -52(%ebp),%edx
	movl %edx,(%eax)
	cmpb $0,-57(%ebp)
	je .L787
	addl $-12,%esp
	movl -56(%ebp),%eax
	pushl %eax
	call __builtin_delete
	addl $16,%esp
	jmp .L787
	.p2align 2,0x90
.L786:
.L787:
	movl -64(%ebp),%eax
	addl $4,%eax
	movl (%eax),%edx
	movl (%edx),%ecx
	movl %ecx,(%eax)
	call __sjthrow
	jmp .L788
	.p2align 2,0x90
.L784:
	movl %ebp,%ebp
#APP
	
#NO_APP
.L788:
	call __terminate
	.p2align 2,0x90
.L789:
.L776:
	leal -88(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe56:
		.size		 main,.Lfe56-main
.section	.gnu.linkonce.t.ln__Fv,"ax",@progbits
	.p2align 2
	.globl	ln__Fv
	.weak	ln__Fv
		.type		 ln__Fv,@function
ln__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	addl $-8,%esp
	pushl $endl__FR7ostream
	pushl $cout
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
.L338:
	leave
	ret
.Lfe57:
		.size		 ln__Fv,.Lfe57-ln__Fv
.section	.gnu.linkonce.t.downcase__FRc,"ax",@progbits
	.p2align 2
	.globl	downcase__FRc
	.weak	downcase__FRc
		.type		 downcase__FRc,@function
downcase__FRc:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%eax
	cmpb $64,(%eax)
	jle .L587
	cmpb $90,(%eax)
	jg .L587
	addb $32,(%eax)
.L587:
	jmp .L588
	jmp .L586
	.p2align 2
.L588:
.L586:
	leave
	ret
.Lfe58:
		.size		 downcase__FRc,.Lfe58-downcase__FRc
	.local	_ran_device
	.comm	_ran_device,4,4
	.local	_ran_dev_name
	.comm	_ran_dev_name,30,1
	.ident	"GCC: (GNU) cplusplus 2.95.3 20010315 (release) [FreeBSD]"
