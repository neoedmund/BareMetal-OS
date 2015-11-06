	.file	"neos.c"
	.local	buf
	.comm	buf,100,64
	.local	error
	.comm	error,4,4
	.section	.rodata
.LC0:
	.string	"(My) Real OS from here(C)!\n"
.LC1:
	.string	"memsize in MB:"
.LC2:
	.string	"\n"
.LC3:
	.string	"ret="
	.text
	.globl	neos_main
	.type	neos_main, @function
neos_main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movl	$.LC0, %edi
	call	b_output
	movl	$0, %eax
	call	printe820
	movq	$20512, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	leaq	-128(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	str_long2dec
	movl	$.LC1, %edi
	call	b_output
	leaq	-128(%rbp), %rax
	movq	%rax, %rdi
	call	b_output
	movl	$.LC2, %edi
	call	b_output
	movl	$0, %eax
	call	memtest
	movl	$3, %edx
	movl	$2, %esi
	movabsq	$5368709120, %rdi
	call	asm_test
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	%eax, %edx
	leaq	-128(%rbp), %rax
	movq	%rax, %rsi
	movl	%edx, %edi
	call	str_long2dec
	movl	$.LC3, %edi
	call	b_output
	leaq	-128(%rbp), %rax
	movq	%rax, %rdi
	call	b_output
	movl	$.LC2, %edi
	call	b_output
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	neos_main, .-neos_main
	.comm	FreeMemArr,1600,64
	.globl	FreeMemArrCnt
	.bss
	.align 4
	.type	FreeMemArrCnt, @object
	.size	FreeMemArrCnt, 4
FreeMemArrCnt:
	.zero	4
	.globl	HEX
	.data
	.align 32
	.type	HEX, @object
	.size	HEX, 37
HEX:
	.string	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	.text
	.globl	str_itoa
	.type	str_itoa, @function
str_itoa:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movl	%ecx, -40(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L4
.L5:
	movl	-40(%rbp), %eax
	movslq	%eax, %rcx
	movq	-24(%rbp), %rax
	cqto
	idivq	%rcx
	movq	%rdx, %rax
	movb	%al, -5(%rbp)
	movl	-40(%rbp), %eax
	movslq	%eax, %rsi
	movq	-24(%rbp), %rax
	cqto
	idivq	%rsi
	movq	%rax, -24(%rbp)
	movl	-36(%rbp), %eax
	subl	$1, %eax
	subl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-32(%rbp), %rax
	addq	%rax, %rdx
	movsbl	-5(%rbp), %eax
	cltq
	movzbl	HEX(%rax), %eax
	movb	%al, (%rdx)
	addl	$1, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jl	.L5
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	str_itoa, .-str_itoa
	.globl	str_long2hex
	.type	str_long2hex, @function
str_long2hex:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rsi
	movq	-8(%rbp), %rax
	movl	$16, %ecx
	movl	$16, %edx
	movq	%rax, %rdi
	call	str_itoa
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	str_long2hex, .-str_long2hex
	.globl	str_int2hex
	.type	str_int2hex, @function
str_int2hex:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	-4(%rbp), %eax
	cltq
	movq	-16(%rbp), %rsi
	movl	$16, %ecx
	movl	$8, %edx
	movq	%rax, %rdi
	call	str_itoa
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	str_int2hex, .-str_int2hex
	.globl	str_long2dec
	.type	str_long2dec, @function
str_long2dec:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movl	$10, -8(%rbp)
	movl	$0, -4(%rbp)
.L11:
	movl	-36(%rbp), %eax
	cltd
	idivl	-8(%rbp)
	movl	%edx, %eax
	movb	%al, -9(%rbp)
	movl	-36(%rbp), %eax
	cltd
	idivl	-8(%rbp)
	movl	%eax, -36(%rbp)
	movl	-4(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -4(%rbp)
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rax, %rdx
	movsbl	-9(%rbp), %eax
	cltq
	movzbl	HEX(%rax), %eax
	movb	%al, (%rdx)
	cmpl	$0, -36(%rbp)
	jne	.L9
	nop
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movl	-4(%rbp), %eax
	movl	%eax, -16(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L12
.L9:
	jmp	.L11
.L13:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, -20(%rbp)
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rax, %rdx
	movl	-16(%rbp), %eax
	subl	-4(%rbp), %eax
	cltq
	leaq	-1(%rax), %rcx
	movq	-48(%rbp), %rax
	addq	%rcx, %rax
	movzbl	(%rax), %eax
	movb	%al, (%rdx)
	movl	-16(%rbp), %eax
	subl	-4(%rbp), %eax
	cltq
	leaq	-1(%rax), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movl	-20(%rbp), %edx
	movb	%dl, (%rax)
	addl	$1, -4(%rbp)
.L12:
	movl	-16(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	cmpl	-4(%rbp), %eax
	jg	.L13
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	str_long2dec, .-str_long2dec
	.section	.rodata
.LC4:
	.string	"    "
.LC5:
	.string	"OK"
.LC6:
	.string	"NG"
	.text
	.globl	printe820slot
	.type	printe820slot, @function
printe820slot:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movq	%rdi, -120(%rbp)
	movq	-120(%rbp), %rax
	movq	(%rax), %rax
	leaq	-112(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	str_long2hex
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	b_output
	movl	$.LC4, %edi
	call	b_output
	movq	-120(%rbp), %rax
	movq	8(%rax), %rax
	leaq	-112(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	str_long2hex
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	b_output
	movl	$.LC4, %edi
	call	b_output
	movq	-120(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	$1, %eax
	jne	.L15
	movl	$.LC5, %edi
	call	b_output
	jmp	.L16
.L15:
	movl	$.LC6, %edi
	call	b_output
.L16:
	movl	$.LC2, %edi
	call	b_output
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	printe820slot, .-printe820slot
	.section	.rodata
.LC7:
	.string	"assert fail, size="
	.text
	.globl	printe820
	.type	printe820, @function
printe820:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movl	$0, FreeMemArrCnt(%rip)
	movq	$16384, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L22:
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	printe820slot
	movq	-16(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	$1, %eax
	jne	.L18
	movl	FreeMemArrCnt(%rip), %edx
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	$FreeMemArr, %rdx
	movq	%rax, (%rdx)
	movl	FreeMemArrCnt(%rip), %edx
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	$FreeMemArr, %rdx
	movq	%rax, 8(%rdx)
	movl	FreeMemArrCnt(%rip), %eax
	addl	$1, %eax
	movl	%eax, FreeMemArrCnt(%rip)
.L18:
	movl	$32, -20(%rbp)
	cmpl	$32, -20(%rbp)
	je	.L19
	movl	$.LC7, %edi
	call	b_output
	movl	-20(%rbp), %eax
	cltq
	leaq	-128(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	str_long2hex
	leaq	-128(%rbp), %rax
	movq	%rax, %rdi
	call	b_output
	movl	$.LC2, %edi
	call	b_output
.L19:
	movq	-16(%rbp), %rax
	movl	16(%rax), %eax
	testl	%eax, %eax
	jne	.L20
	jmp	.L23
.L20:
	movl	-20(%rbp), %eax
	cltq
	addq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	.L22
.L23:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	printe820, .-printe820
	.globl	mempart_cutoff
	.type	mempart_cutoff, @function
mempart_cutoff:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-48(%rbp), %rax
	movq	(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	addq	%rdx, %rax
	movq	%rax, -32(%rbp)
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jg	.L25
	movq	-16(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jle	.L25
	movq	-16(%rbp), %rax
	cmpq	-32(%rbp), %rax
	jg	.L25
	movq	-40(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-32(%rbp), %rax
	subq	-16(%rbp), %rax
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	jmp	.L24
.L25:
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jl	.L27
	movq	-8(%rbp), %rax
	cmpq	-32(%rbp), %rax
	jge	.L27
	movq	-16(%rbp), %rax
	cmpq	-32(%rbp), %rax
	jl	.L27
	movq	-8(%rbp), %rax
	subq	-24(%rbp), %rax
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	jmp	.L24
.L27:
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jg	.L28
	movq	-16(%rbp), %rax
	cmpq	-32(%rbp), %rax
	jl	.L28
	movq	-40(%rbp), %rax
	movq	$0, 8(%rax)
	jmp	.L24
.L28:
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jle	.L24
	movq	-16(%rbp), %rax
	cmpq	-32(%rbp), %rax
	jge	.L24
	movq	-8(%rbp), %rax
	subq	-24(%rbp), %rax
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	movl	FreeMemArrCnt(%rip), %eax
	cltq
	salq	$4, %rax
	leaq	FreeMemArr(%rax), %rdx
	movq	-16(%rbp), %rax
	movq	%rax, (%rdx)
	movl	FreeMemArrCnt(%rip), %edx
	movq	-32(%rbp), %rax
	subq	-16(%rbp), %rax
	movslq	%edx, %rdx
	salq	$4, %rdx
	addq	$FreeMemArr, %rdx
	movq	%rax, 8(%rdx)
	movl	FreeMemArrCnt(%rip), %eax
	addl	$1, %eax
	movl	%eax, FreeMemArrCnt(%rip)
	nop
.L24:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	mempart_cutoff, .-mempart_cutoff
	.globl	mempart_sub
	.type	mempart_sub, @function
mempart_sub:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L30
.L31:
	movl	-4(%rbp), %eax
	cltq
	salq	$4, %rax
	addq	$FreeMemArr, %rax
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	mempart_cutoff
	addl	$1, -4(%rbp)
.L30:
	movl	FreeMemArrCnt(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L31
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	mempart_sub, .-mempart_sub
	.section	.rodata
.LC8:
	.string	"No."
	.text
	.globl	printTestingMempart
	.type	printTestingMempart, @function
printTestingMempart:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L33
.L36:
	movl	-4(%rbp), %eax
	cltq
	salq	$4, %rax
	addq	$FreeMemArr, %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L34
	jmp	.L35
.L34:
	addl	$1, -8(%rbp)
	movl	$.LC8, %edi
	call	b_output
	movl	-8(%rbp), %eax
	movl	$buf, %esi
	movl	%eax, %edi
	call	str_long2dec
	movl	$buf, %edi
	call	b_output
	movl	$.LC4, %edi
	call	b_output
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movl	$buf, %esi
	movq	%rax, %rdi
	call	str_long2hex
	movl	$buf, %edi
	call	b_output
	movl	$.LC4, %edi
	call	b_output
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movl	$buf, %esi
	movq	%rax, %rdi
	call	str_long2hex
	movl	$buf, %edi
	call	b_output
	movl	$.LC2, %edi
	call	b_output
.L35:
	addl	$1, -4(%rbp)
.L33:
	movl	FreeMemArrCnt(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L36
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	printTestingMempart, .-printTestingMempart
	.section	.rodata
.LC9:
	.string	" %"
.LC10:
	.string	"%"
.LC11:
	.string	"OK \n"
	.text
	.globl	writeUnit
	.type	writeUnit, @function
writeUnit:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%rdx, -56(%rbp)
	movq	-48(%rbp), %rcx
	movabsq	$-6640827866535438581, %rdx
	movq	%rcx, %rax
	imulq	%rdx
	leaq	(%rdx,%rcx), %rax
	sarq	$6, %rax
	movq	%rax, %rdx
	movq	%rcx, %rax
	sarq	$63, %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	$0, -16(%rbp)
	movl	$0, -20(%rbp)
.L44:
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	cmpq	-48(%rbp), %rax
	jle	.L38
	movq	-48(%rbp), %rax
	subq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
.L38:
	movq	-56(%rbp), %rdx
	movq	-8(%rbp), %rcx
	movq	-40(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	asm_writeU64
	movq	-8(%rbp), %rax
	salq	$3, %rax
	addq	%rax, -40(%rbp)
	movq	-8(%rbp), %rax
	addq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jl	.L39
	jmp	.L45
.L39:
	addl	$1, -20(%rbp)
	cmpl	$100, -20(%rbp)
	jne	.L41
	movl	$99, -20(%rbp)
.L41:
	movl	-20(%rbp), %eax
	movl	$buf, %esi
	movl	%eax, %edi
	call	str_long2dec
	movl	$buf, %edi
	call	b_output
	cmpl	$9, -20(%rbp)
	jg	.L42
	movl	$.LC9, %edi
	call	b_output
	jmp	.L43
.L42:
	movl	$.LC10, %edi
	call	b_output
.L43:
	movl	$0, %eax
	call	os_dec_cursor
	movl	$0, %eax
	call	os_dec_cursor
	movl	$0, %eax
	call	os_dec_cursor
	jmp	.L44
.L45:
	movl	$.LC11, %edi
	call	b_output
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	writeUnit, .-writeUnit
	.section	.rodata
.LC12:
	.string	"Error:"
	.text
	.globl	verifyUnit
	.type	verifyUnit, @function
verifyUnit:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%rdx, -56(%rbp)
	movq	-48(%rbp), %rcx
	movabsq	$-6640827866535438581, %rdx
	movq	%rcx, %rax
	imulq	%rdx
	leaq	(%rdx,%rcx), %rax
	sarq	$6, %rax
	movq	%rax, %rdx
	movq	%rcx, %rax
	sarq	$63, %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	$0, -16(%rbp)
	movl	$0, -20(%rbp)
.L54:
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	cmpq	-48(%rbp), %rax
	jle	.L47
	movq	-48(%rbp), %rax
	subq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
.L47:
	movq	-56(%rbp), %rdx
	movq	-8(%rbp), %rcx
	movq	-40(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	asm_verifyU64
	movl	%eax, -24(%rbp)
	cmpl	$0, -24(%rbp)
	je	.L48
	movl	error(%rip), %eax
	addl	$1, %eax
	movl	%eax, error(%rip)
	movl	$.LC12, %edi
	call	b_output
	movl	-24(%rbp), %eax
	cltq
	movq	-8(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$buf, %esi
	movq	%rax, %rdi
	call	str_long2hex
	movl	$buf, %edi
	call	b_output
	movl	$.LC2, %edi
	call	b_output
.L48:
	movq	-8(%rbp), %rax
	salq	$3, %rax
	addq	%rax, -40(%rbp)
	movq	-8(%rbp), %rax
	addq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jl	.L49
	jmp	.L55
.L49:
	addl	$1, -20(%rbp)
	cmpl	$100, -20(%rbp)
	jne	.L51
	movl	$99, -20(%rbp)
.L51:
	movl	-20(%rbp), %eax
	movl	$buf, %esi
	movl	%eax, %edi
	call	str_long2dec
	movl	$buf, %edi
	call	b_output
	cmpl	$9, -20(%rbp)
	jg	.L52
	movl	$.LC9, %edi
	call	b_output
	jmp	.L53
.L52:
	movl	$.LC10, %edi
	call	b_output
.L53:
	movl	$0, %eax
	call	os_dec_cursor
	movl	$0, %eax
	call	os_dec_cursor
	movl	$0, %eax
	call	os_dec_cursor
	jmp	.L54
.L55:
	movl	$.LC11, %edi
	call	b_output
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	verifyUnit, .-verifyUnit
	.section	.rodata
.LC13:
	.string	"Step 1/4 write FF "
.LC14:
	.string	"Step 2/4 verify FF "
.LC15:
	.string	"Step 3/4 write 00 "
.LC16:
	.string	"Step 4/4 verify 00 "
	.text
	.globl	doTestMem
	.type	doTestMem, @function
doTestMem:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	leaq	7(%rax), %rdx
	testq	%rax, %rax
	cmovs	%rdx, %rax
	sarq	$3, %rax
	movq	%rax, -8(%rbp)
	movl	$1, -12(%rbp)
	movq	$-1, -24(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movl	$.LC13, %edi
	call	b_output
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	writeUnit
	movl	$2, -12(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movl	$.LC14, %edi
	call	b_output
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	verifyUnit
	movl	$3, -12(%rbp)
	movq	$0, -24(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movl	$.LC15, %edi
	call	b_output
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	writeUnit
	movl	$4, -12(%rbp)
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movl	$.LC16, %edi
	call	b_output
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	verifyUnit
	movl	$.LC11, %edi
	call	b_output
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	doTestMem, .-doTestMem
	.section	.rodata
.LC17:
	.string	"Testing No."
	.text
	.globl	testFreeMem
	.type	testFreeMem, @function
testFreeMem:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, %eax
	call	printTestingMempart
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L58
.L61:
	movl	-4(%rbp), %eax
	cltq
	salq	$4, %rax
	addq	$FreeMemArr, %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L59
	jmp	.L60
.L59:
	addl	$1, -8(%rbp)
	movl	$.LC17, %edi
	call	b_output
	movl	-8(%rbp), %eax
	movl	$buf, %esi
	movl	%eax, %edi
	call	str_long2dec
	movl	$buf, %edi
	call	b_output
	movl	$.LC4, %edi
	call	b_output
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movl	$buf, %esi
	movq	%rax, %rdi
	call	str_long2hex
	movl	$buf, %edi
	call	b_output
	movl	$.LC4, %edi
	call	b_output
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movl	$buf, %esi
	movq	%rax, %rdi
	call	str_long2hex
	movl	$buf, %edi
	call	b_output
	movl	$.LC2, %edi
	call	b_output
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	doTestMem
.L60:
	addl	$1, -4(%rbp)
.L58:
	movl	FreeMemArrCnt(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L61
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	testFreeMem, .-testFreeMem
	.section	.rodata
.LC18:
	.string	"memtest start...\n"
.LC19:
	.string	"memtest finish.\n"
	.text
	.globl	memtest
	.type	memtest, @function
memtest:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$.LC18, %edi
	call	b_output
	movq	$0, -16(%rbp)
	movq	$2097152, -8(%rbp)
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	mempart_sub
	movl	$0, %eax
	call	testFreeMem
	movl	$.LC19, %edi
	call	b_output
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	memtest, .-memtest
	.ident	"GCC: (Debian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",@progbits
