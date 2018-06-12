	.file	"assemble.c"
	.section .rdata,"dr"
.LC0:
	.ascii "%s, 0x%x\12\0"
	.text
	.globl	printSymtab
	.def	printSymtab;	.scl	2;	.type	32;	.endef
	.seh_proc	printSymtab
printSymtab:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$72, %rsp
	.seh_stackalloc	72
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.seh_endprologue
	movq	%rcx, %rbx
	movq	(%rbx), %rax
	movq	8(%rbx), %rdx
	movq	%rax, -96(%rbp)
	movq	%rdx, -88(%rbp)
	movl	$0, -68(%rbp)
.L2:
	movq	-88(%rbp), %rcx
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
	movl	20(%rax), %ecx
	movq	-88(%rbp), %r8
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%r8, %rax
	movl	%ecx, %r8d
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rcx
	call	printf
	addl	$1, -68(%rbp)
	movl	-96(%rbp), %eax
	cmpl	%eax, -68(%rbp)
	jl	.L2
	nop
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.seh_endproc
	.globl	getType
	.def	getType;	.scl	2;	.type	32;	.endef
	.seh_proc	getType
getType:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	cmpl	$0, 16(%rbp)
	js	.L4
	cmpl	$9, 16(%rbp)
	jg	.L4
	movl	$0, %eax
	jmp	.L5
.L4:
	cmpl	$9, 16(%rbp)
	jle	.L6
	cmpl	$11, 16(%rbp)
	jg	.L6
	movl	$1, %eax
	jmp	.L5
.L6:
	cmpl	$11, 16(%rbp)
	jle	.L7
	cmpl	$13, 16(%rbp)
	jg	.L7
	movl	$2, %eax
	jmp	.L5
.L7:
	cmpl	$14, 16(%rbp)
	jne	.L8
	movl	$3, %eax
	jmp	.L5
.L8:
	cmpl	$15, 16(%rbp)
	jne	.L9
	movl	$0, %eax
	jmp	.L5
.L9:
	movl	$-1, %eax
.L5:
	popq	%rbp
	ret
	.seh_endproc
	.globl	noOfArgs
	.def	noOfArgs;	.scl	2;	.type	32;	.endef
	.seh_proc	noOfArgs
noOfArgs:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	cmpl	$0, 16(%rbp)
	js	.L11
	cmpl	$5, 16(%rbp)
	jg	.L11
	movl	$3, %eax
	jmp	.L12
.L11:
	cmpl	$5, 16(%rbp)
	jle	.L13
	cmpl	$9, 16(%rbp)
	jg	.L13
	movl	$2, %eax
	jmp	.L12
.L13:
	cmpl	$10, 16(%rbp)
	jne	.L14
	movl	$3, %eax
	jmp	.L12
.L14:
	cmpl	$11, 16(%rbp)
	jne	.L15
	movl	$4, %eax
	jmp	.L12
.L15:
	cmpl	$11, 16(%rbp)
	jle	.L16
	cmpl	$13, 16(%rbp)
	jg	.L16
	movl	$2, %eax
	jmp	.L12
.L16:
	cmpl	$14, 16(%rbp)
	jne	.L17
	movl	$1, %eax
	jmp	.L12
.L17:
	cmpl	$15, 16(%rbp)
	jne	.L18
	movl	$2, %eax
	jmp	.L12
.L18:
	movl	$-1, %eax
.L12:
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "add\0"
.LC2:
	.ascii "sub\0"
.LC3:
	.ascii "rsb\0"
.LC4:
	.ascii "and\0"
.LC5:
	.ascii "eor\0"
.LC6:
	.ascii "orr\0"
.LC7:
	.ascii "mov\0"
.LC8:
	.ascii "tst\0"
.LC9:
	.ascii "teq\0"
.LC10:
	.ascii "cmp\0"
.LC11:
	.ascii "mul\0"
.LC12:
	.ascii "mla\0"
.LC13:
	.ascii "ldr\0"
.LC14:
	.ascii "str\0"
.LC15:
	.ascii "lsl\0"
.LC16:
	.ascii "ERROR \0"
.LC17:
	.ascii "eq \0"
.LC18:
	.ascii "ne \0"
.LC19:
	.ascii "ge \0"
.LC20:
	.ascii "lt \0"
.LC21:
	.ascii "gt \0"
.LC22:
	.ascii "le \0"
.LC23:
	.ascii "%s \0"
.LC24:
	.ascii " SET CPSR\0"
	.text
	.globl	printInstruction
	.def	printInstruction;	.scl	2;	.type	32;	.endef
	.seh_proc	printInstruction
printInstruction:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movl	%r9d, 40(%rbp)
	movl	16(%rbp), %ecx
	call	noOfArgs
	movl	%eax, -8(%rbp)
	movl	$0, -4(%rbp)
	cmpl	$0, 16(%rbp)
	jne	.L20
	leaq	.LC1(%rip), %rcx
	call	printf
	jmp	.L21
.L20:
	cmpl	$1, 16(%rbp)
	jne	.L22
	leaq	.LC2(%rip), %rcx
	call	printf
	jmp	.L21
.L22:
	cmpl	$2, 16(%rbp)
	jne	.L23
	leaq	.LC3(%rip), %rcx
	call	printf
	jmp	.L21
.L23:
	cmpl	$3, 16(%rbp)
	jne	.L24
	leaq	.LC4(%rip), %rcx
	call	printf
	jmp	.L21
.L24:
	cmpl	$4, 16(%rbp)
	jne	.L25
	leaq	.LC5(%rip), %rcx
	call	printf
	jmp	.L21
.L25:
	cmpl	$5, 16(%rbp)
	jne	.L26
	leaq	.LC6(%rip), %rcx
	call	printf
	jmp	.L21
.L26:
	cmpl	$6, 16(%rbp)
	jne	.L27
	leaq	.LC7(%rip), %rcx
	call	printf
	jmp	.L21
.L27:
	cmpl	$7, 16(%rbp)
	jne	.L28
	leaq	.LC8(%rip), %rcx
	call	printf
	jmp	.L21
.L28:
	cmpl	$8, 16(%rbp)
	jne	.L29
	leaq	.LC9(%rip), %rcx
	call	printf
	jmp	.L21
.L29:
	cmpl	$9, 16(%rbp)
	jne	.L30
	leaq	.LC10(%rip), %rcx
	call	printf
	jmp	.L21
.L30:
	cmpl	$10, 16(%rbp)
	jne	.L31
	leaq	.LC11(%rip), %rcx
	call	printf
	jmp	.L21
.L31:
	cmpl	$11, 16(%rbp)
	jne	.L32
	leaq	.LC12(%rip), %rcx
	call	printf
	jmp	.L21
.L32:
	cmpl	$12, 16(%rbp)
	jne	.L33
	leaq	.LC13(%rip), %rcx
	call	printf
	jmp	.L21
.L33:
	cmpl	$13, 16(%rbp)
	jne	.L34
	leaq	.LC14(%rip), %rcx
	call	printf
	jmp	.L21
.L34:
	cmpl	$14, 16(%rbp)
	jne	.L35
	movl	$98, %ecx
	call	putchar
	jmp	.L21
.L35:
	cmpl	$15, 16(%rbp)
	jne	.L36
	leaq	.LC15(%rip), %rcx
	call	printf
	jmp	.L21
.L36:
	leaq	.LC16(%rip), %rcx
	call	printf
.L21:
	cmpl	$0, 24(%rbp)
	jne	.L37
	leaq	.LC17(%rip), %rcx
	call	printf
	jmp	.L45
.L37:
	cmpl	$1, 24(%rbp)
	jne	.L39
	leaq	.LC18(%rip), %rcx
	call	printf
	jmp	.L45
.L39:
	cmpl	$2, 24(%rbp)
	jne	.L40
	leaq	.LC19(%rip), %rcx
	call	printf
	jmp	.L45
.L40:
	cmpl	$3, 24(%rbp)
	jne	.L41
	leaq	.LC20(%rip), %rcx
	call	printf
	jmp	.L45
.L41:
	cmpl	$4, 24(%rbp)
	jne	.L42
	leaq	.LC21(%rip), %rcx
	call	printf
	jmp	.L45
.L42:
	cmpl	$5, 24(%rbp)
	jne	.L43
	leaq	.LC22(%rip), %rcx
	call	printf
	jmp	.L45
.L43:
	cmpl	$6, 24(%rbp)
	jne	.L44
	movl	$32, %ecx
	call	putchar
	jmp	.L45
.L44:
	leaq	.LC16(%rip), %rcx
	call	printf
.L45:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	32(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdx
	leaq	.LC23(%rip), %rcx
	call	printf
	addl	$1, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jl	.L45
	cmpl	$0, 40(%rbp)
	je	.L46
	leaq	.LC24(%rip), %rcx
	call	printf
.L46:
	movl	$10, %ecx
	call	putchar
	nop
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	contains
	.def	contains;	.scl	2;	.type	32;	.endef
	.seh_proc	contains
contains:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, %eax
	movb	%al, 24(%rbp)
	movq	16(%rbp), %rcx
	call	strlen
	movl	%eax, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L48
.L51:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	%al, 24(%rbp)
	jne	.L49
	movl	$1, %eax
	jmp	.L50
.L49:
	addl	$1, -4(%rbp)
.L48:
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jl	.L51
	movl	$0, %eax
.L50:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC25:
	.ascii "r0\0"
.LC26:
	.ascii "r1\0"
.LC27:
	.ascii "r2\0"
.LC28:
	.ascii "r3\0"
.LC29:
	.ascii "r4\0"
.LC30:
	.ascii "r5\0"
.LC31:
	.ascii "r6\0"
.LC32:
	.ascii "r7\0"
.LC33:
	.ascii "r8\0"
.LC34:
	.ascii "r9\0"
.LC35:
	.ascii "r10\0"
.LC36:
	.ascii "r11\0"
.LC37:
	.ascii "r12\0"
.LC38:
	.ascii "sp\0"
.LC39:
	.ascii "lr\0"
.LC40:
	.ascii "pc\0"
.LC41:
	.ascii "cpsr\0"
.LC42:
	.ascii "register does not exist\0"
	.text
	.globl	getRegister
	.def	getRegister;	.scl	2;	.type	32;	.endef
	.seh_proc	getRegister
getRegister:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	.LC25(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L53
	movl	$0, %eax
	jmp	.L54
.L53:
	leaq	.LC26(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L55
	movl	$1, %eax
	jmp	.L54
.L55:
	leaq	.LC27(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L56
	movl	$2, %eax
	jmp	.L54
.L56:
	leaq	.LC28(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L57
	movl	$3, %eax
	jmp	.L54
.L57:
	leaq	.LC29(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L58
	movl	$4, %eax
	jmp	.L54
.L58:
	leaq	.LC30(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L59
	movl	$5, %eax
	jmp	.L54
.L59:
	leaq	.LC31(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L60
	movl	$6, %eax
	jmp	.L54
.L60:
	leaq	.LC32(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L61
	movl	$7, %eax
	jmp	.L54
.L61:
	leaq	.LC33(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L62
	movl	$8, %eax
	jmp	.L54
.L62:
	leaq	.LC34(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L63
	movl	$9, %eax
	jmp	.L54
.L63:
	leaq	.LC35(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L64
	movl	$10, %eax
	jmp	.L54
.L64:
	leaq	.LC36(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L65
	movl	$11, %eax
	jmp	.L54
.L65:
	leaq	.LC37(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L66
	movl	$12, %eax
	jmp	.L54
.L66:
	leaq	.LC38(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L67
	movl	$13, %eax
	jmp	.L54
.L67:
	leaq	.LC39(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L68
	movl	$14, %eax
	jmp	.L54
.L68:
	leaq	.LC40(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L69
	movl	$15, %eax
	jmp	.L54
.L69:
	leaq	.LC41(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L70
	movl	$16, %eax
	jmp	.L54
.L70:
	leaq	.LC42(%rip), %rcx
	call	puts
	movl	$-1, %eax
.L54:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC43:
	.ascii "b\0"
	.text
	.globl	typeCheck
	.def	typeCheck;	.scl	2;	.type	32;	.endef
	.seh_proc	typeCheck
typeCheck:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	-5(%rbp), %rax
	movq	16(%rbp), %rdx
	movq	%rax, %rcx
	call	strcpy
	movb	$0, -2(%rbp)
	leaq	-5(%rbp), %rax
	leaq	.LC1(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC2(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC3(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC5(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC6(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC7(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC8(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC9(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC10(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L72
	leaq	-5(%rbp), %rax
	leaq	.LC15(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L73
.L72:
	movl	$0, %eax
	jmp	.L80
.L73:
	leaq	-5(%rbp), %rax
	leaq	.LC11(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L75
	leaq	-5(%rbp), %rax
	leaq	.LC12(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L76
.L75:
	movl	$1, %eax
	jmp	.L80
.L76:
	leaq	-5(%rbp), %rax
	leaq	.LC13(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	jne	.L77
	leaq	-5(%rbp), %rax
	leaq	.LC14(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L78
.L77:
	movl	$2, %eax
	jmp	.L80
.L78:
	movb	$0, -4(%rbp)
	leaq	-5(%rbp), %rax
	leaq	.LC43(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L79
	movl	$3, %eax
	jmp	.L80
.L79:
	movl	$-1, %eax
.L80:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	hasCondition
	.def	hasCondition;	.scl	2;	.type	32;	.endef
	.seh_proc	hasCondition
hasCondition:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rcx
	call	strlen
	leaq	-1(%rax), %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$115, %al
	je	.L82
	movq	16(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$98, %al
	jne	.L83
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$1, %rax
	je	.L82
.L83:
	movq	16(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$98, %al
	je	.L84
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$3, %rax
	je	.L82
.L84:
	movl	$1, %eax
	jmp	.L86
.L82:
	movl	$0, %eax
.L86:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC44:
	.ascii "eq\0"
.LC45:
	.ascii "ne\0"
.LC46:
	.ascii "ge\0"
.LC47:
	.ascii "lt\0"
.LC48:
	.ascii "gt\0"
.LC49:
	.ascii "le\0"
.LC50:
	.ascii "al\0"
	.text
	.globl	getCondition
	.def	getCondition;	.scl	2;	.type	32;	.endef
	.seh_proc	getCondition
getCondition:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rcx
	call	hasCondition
	testl	%eax, %eax
	je	.L88
	movq	16(%rbp), %rcx
	call	strlen
	leaq	-2(%rax), %rdx
	movq	16(%rbp), %rax
	addq	%rax, %rdx
	leaq	-2(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	movb	$0, 0(%rbp)
	leaq	-2(%rbp), %rax
	leaq	.LC44(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L89
	movl	$0, %eax
	jmp	.L97
.L89:
	leaq	-2(%rbp), %rax
	leaq	.LC45(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L91
	movl	$1, %eax
	jmp	.L97
.L91:
	leaq	-2(%rbp), %rax
	leaq	.LC46(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L92
	movl	$2, %eax
	jmp	.L97
.L92:
	leaq	-2(%rbp), %rax
	leaq	.LC47(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L93
	movl	$3, %eax
	jmp	.L97
.L93:
	leaq	-2(%rbp), %rax
	leaq	.LC48(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L94
	movl	$4, %eax
	jmp	.L97
.L94:
	leaq	-2(%rbp), %rax
	leaq	.LC49(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L95
	movl	$5, %eax
	jmp	.L97
.L95:
	leaq	-2(%rbp), %rax
	leaq	.LC50(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L96
	movl	$6, %eax
	jmp	.L97
.L96:
	movl	$-1, %eax
	jmp	.L97
.L88:
	movl	$6, %eax
.L97:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	getOpcode
	.def	getOpcode;	.scl	2;	.type	32;	.endef
	.seh_proc	getOpcode
getOpcode:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	-3(%rbp), %rax
	movq	16(%rbp), %rdx
	movq	%rax, %rcx
	call	strcpy
	movb	$0, 0(%rbp)
	leaq	-3(%rbp), %rax
	leaq	.LC1(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L99
	movl	$0, %eax
	jmp	.L116
.L99:
	leaq	-3(%rbp), %rax
	leaq	.LC2(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L101
	movl	$1, %eax
	jmp	.L116
.L101:
	leaq	-3(%rbp), %rax
	leaq	.LC3(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L102
	movl	$2, %eax
	jmp	.L116
.L102:
	leaq	-3(%rbp), %rax
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L103
	movl	$3, %eax
	jmp	.L116
.L103:
	leaq	-3(%rbp), %rax
	leaq	.LC5(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L104
	movl	$4, %eax
	jmp	.L116
.L104:
	leaq	-3(%rbp), %rax
	leaq	.LC6(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L105
	movl	$5, %eax
	jmp	.L116
.L105:
	leaq	-3(%rbp), %rax
	leaq	.LC7(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L106
	movl	$6, %eax
	jmp	.L116
.L106:
	leaq	-3(%rbp), %rax
	leaq	.LC8(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L107
	movl	$7, %eax
	jmp	.L116
.L107:
	leaq	-3(%rbp), %rax
	leaq	.LC9(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L108
	movl	$8, %eax
	jmp	.L116
.L108:
	leaq	-3(%rbp), %rax
	leaq	.LC10(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L109
	movl	$9, %eax
	jmp	.L116
.L109:
	leaq	-3(%rbp), %rax
	leaq	.LC11(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L110
	movl	$10, %eax
	jmp	.L116
.L110:
	leaq	-3(%rbp), %rax
	leaq	.LC12(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L111
	movl	$11, %eax
	jmp	.L116
.L111:
	leaq	-3(%rbp), %rax
	leaq	.LC13(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L112
	movl	$12, %eax
	jmp	.L116
.L112:
	leaq	-3(%rbp), %rax
	leaq	.LC14(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L113
	movl	$13, %eax
	jmp	.L116
.L113:
	leaq	-3(%rbp), %rax
	leaq	.LC15(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L114
	movl	$15, %eax
	jmp	.L116
.L114:
	movb	$0, -2(%rbp)
	leaq	-3(%rbp), %rax
	leaq	.LC43(%rip), %rdx
	movq	%rax, %rcx
	call	equals
	testl	%eax, %eax
	je	.L115
	movl	$14, %eax
	jmp	.L116
.L115:
	movl	$-1, %eax
.L116:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	condCheck
	.def	condCheck;	.scl	2;	.type	32;	.endef
	.seh_proc	condCheck
condCheck:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rcx
	call	typeCheck
	movl	%eax, -4(%rbp)
	cmpl	$3, -4(%rbp)
	jne	.L118
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$1, %rax
	jne	.L119
	movl	$6, %eax
	jmp	.L123
.L119:
	movq	16(%rbp), %rax
	leaq	1(%rax), %rdx
	leaq	-6(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	leaq	-6(%rbp), %rax
	movq	%rax, %rcx
	call	getCondition
	jmp	.L123
.L118:
	cmpl	$-1, -4(%rbp)
	jne	.L121
	movl	$-1, %eax
	jmp	.L123
.L121:
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$3, %rax
	jne	.L122
	movl	$6, %eax
	jmp	.L123
.L122:
	movq	16(%rbp), %rax
	leaq	3(%rax), %rdx
	leaq	-6(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	leaq	-6(%rbp), %rax
	movq	%rax, %rcx
	call	getCondition
.L123:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC51:
	.ascii ":\0"
	.text
	.globl	isLabel
	.def	isLabel;	.scl	2;	.type	32;	.endef
	.seh_proc	isLabel
isLabel:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$112, %rsp
	.seh_stackalloc	112
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	-80(%rbp), %rax
	movq	16(%rbp), %rdx
	movq	%rax, %rcx
	call	strcpy
	leaq	-80(%rbp), %rax
	movl	$58, %edx
	movq	%rax, %rcx
	call	contains
	testl	%eax, %eax
	je	.L125
	leaq	-80(%rbp), %rax
	leaq	.LC51(%rip), %rdx
	movq	%rax, %rcx
	call	strtok
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	jmp	.L127
.L125:
	movl	$0, %eax
.L127:
	addq	$112, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	conditionToBinary
	.def	conditionToBinary;	.scl	2;	.type	32;	.endef
	.seh_proc	conditionToBinary
conditionToBinary:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	cmpl	$0, 16(%rbp)
	jne	.L129
	movl	$0, %eax
	jmp	.L130
.L129:
	cmpl	$1, 16(%rbp)
	jne	.L131
	movl	$268435456, %eax
	jmp	.L130
.L131:
	cmpl	$2, 16(%rbp)
	jne	.L132
	movl	$-1610612736, %eax
	jmp	.L130
.L132:
	cmpl	$3, 16(%rbp)
	jne	.L133
	movl	$-1342177280, %eax
	jmp	.L130
.L133:
	cmpl	$4, 16(%rbp)
	jne	.L134
	movl	$-1073741824, %eax
	jmp	.L130
.L134:
	cmpl	$5, 16(%rbp)
	jne	.L135
	movl	$-805306368, %eax
	jmp	.L130
.L135:
	cmpl	$6, 16(%rbp)
	jne	.L136
	movl	$-536870912, %eax
	jmp	.L130
.L136:
	movl	$-1, %eax
.L130:
	popq	%rbp
	ret
	.seh_endproc
	.globl	isSet
	.def	isSet;	.scl	2;	.type	32;	.endef
	.seh_proc	isSet
isSet:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rcx
	call	hasCondition
	testl	%eax, %eax
	je	.L138
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$6, %rax
	je	.L139
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$4, %rax
	jne	.L140
.L139:
	movq	16(%rbp), %rcx
	call	strlen
	leaq	-3(%rax), %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$115, %al
	sete	%al
	movzbl	%al, %eax
	jmp	.L141
.L138:
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$4, %rax
	je	.L142
	movq	16(%rbp), %rcx
	call	strlen
	cmpq	$2, %rax
	jne	.L140
.L142:
	movq	16(%rbp), %rcx
	call	strlen
	leaq	-1(%rax), %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$115, %al
	sete	%al
	movzbl	%al, %eax
	jmp	.L141
.L140:
	movl	$0, %eax
.L141:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC52:
	.ascii "Usage = %s input output\12\0"
.LC53:
	.ascii "r\0"
.LC54:
	.ascii "wb\0"
.LC55:
	.ascii "File Load Failure\12\0"
.LC56:
	.ascii " \0"
	.align 8
.LC57:
	.ascii "Duplicate symbol, aborting assembly.\12\0"
.LC58:
	.ascii "SYMBOL TABLE:\0"
.LC59:
	.ascii "%s has address 0x%x\12\0"
.LC60:
	.ascii "\12\0"
.LC61:
	.ascii ", \0"
.LC62:
	.ascii "\12;\0"
.LC63:
	.ascii "%u\12\0"
.LC64:
	.ascii "%x\12\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$2408, %rsp
	.seh_stackalloc	2408
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.seh_endprologue
	movl	%ecx, 2320(%rbp)
	movq	%rdx, 2328(%rbp)
	call	__main
	movl	$1, %edx
	movl	$512, %ecx
	call	calloc
	movq	%rax, 2240(%rbp)
	movl	$0, 2268(%rbp)
	movl	$0, 2264(%rbp)
	movl	$0, 2128(%rbp)
	movl	$24, %edx
	movl	$120, %ecx
	call	calloc
	movq	%rax, 2136(%rbp)
	movl	$0, 2260(%rbp)
	cmpl	$3, 2320(%rbp)
	je	.L144
	movq	2328(%rbp), %rax
	movq	(%rax), %rbx
	movq	__imp___iob_func(%rip), %rax
	call	*%rax
	addq	$96, %rax
	movq	%rbx, %r8
	leaq	.LC52(%rip), %rdx
	movq	%rax, %rcx
	call	fprintf
	movl	$1, %eax
	jmp	.L172
.L144:
	movq	2328(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	leaq	.LC53(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 2232(%rbp)
	movq	2328(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	leaq	.LC54(%rip), %rdx
	movq	%rax, %rcx
	call	fopen
	movq	%rax, 2224(%rbp)
	cmpq	$0, 2232(%rbp)
	jne	.L147
	movq	__imp___iob_func(%rip), %rax
	call	*%rax
	addq	$96, %rax
	movq	%rax, %r9
	movl	$18, %r8d
	movl	$1, %edx
	leaq	.LC55(%rip), %rcx
	call	fwrite
	movl	$1, %eax
	jmp	.L172
.L151:
	movq	2240(%rbp), %rax
	leaq	.LC56(%rip), %rdx
	movq	%rax, %rcx
	call	strtok
	movq	%rax, 2152(%rbp)
	leaq	.LC56(%rip), %rdx
	movl	$0, %ecx
	call	strtok
	movq	%rax, 2144(%rbp)
	cmpq	$0, 2144(%rbp)
	je	.L148
	movq	2144(%rbp), %rax
	movq	%rax, %rcx
	call	strlen
	leaq	-1(%rax), %rdx
	movq	2144(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
.L148:
	movq	2152(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$59, %al
	je	.L147
	movq	2152(%rbp), %rax
	movl	$58, %edx
	movq	%rax, %rcx
	call	contains
	testl	%eax, %eax
	je	.L149
	movq	2152(%rbp), %rax
	movq	%rax, %rcx
	call	isLabel
	movq	%rax, %rdx
	leaq	2032(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	movq	2128(%rbp), %rax
	movq	2136(%rbp), %rdx
	movq	%rax, 1984(%rbp)
	movq	%rdx, 1992(%rbp)
	leaq	2032(%rbp), %rdx
	leaq	1984(%rbp), %rax
	movq	%rax, %rcx
	call	symtabContains
	testl	%eax, %eax
	je	.L150
	movq	__imp___iob_func(%rip), %rax
	call	*%rax
	addq	$96, %rax
	movq	%rax, %r9
	movl	$37, %r8d
	movl	$1, %edx
	leaq	.LC57(%rip), %rcx
	call	fwrite
	movl	$-1, %eax
	jmp	.L172
.L150:
	movq	2136(%rbp), %rcx
	movl	2260(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
	movq	%rax, %rcx
	leaq	2032(%rbp), %rax
	movq	%rax, %rdx
	call	strcpy
	movq	2136(%rbp), %rcx
	movl	2260(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	leaq	(%rcx,%rax), %rdx
	movl	2264(%rbp), %eax
	movl	%eax, 20(%rdx)
	addl	$1, 2260(%rbp)
	movl	2260(%rbp), %eax
	movl	%eax, 2128(%rbp)
	jmp	.L147
.L149:
	movq	2152(%rbp), %rax
	movq	%rax, %rcx
	call	typeCheck
	cmpl	$-1, %eax
	je	.L147
	addl	$32, 2264(%rbp)
.L147:
	movq	2232(%rbp), %rdx
	movq	2240(%rbp), %rax
	movq	%rdx, %r8
	movl	$512, %edx
	movq	%rax, %rcx
	call	fgets
	testq	%rax, %rax
	jne	.L151
	movl	2264(%rbp), %eax
	movl	%eax, 2220(%rbp)
	movq	2232(%rbp), %rax
	movl	$0, %r8d
	movl	$0, %edx
	movq	%rax, %rcx
	call	fseek
	movl	$0, 2264(%rbp)
	leaq	.LC58(%rip), %rcx
	call	puts
	movq	2128(%rbp), %rax
	movq	2136(%rbp), %rdx
	movq	%rax, 1984(%rbp)
	movq	%rdx, 1992(%rbp)
	leaq	1984(%rbp), %rax
	movq	%rax, %rcx
	call	printSymtab
	movl	$10, %ecx
	call	putchar
	jmp	.L152
.L169:
	movl	$0, 2268(%rbp)
	movq	2240(%rbp), %rax
	leaq	.LC56(%rip), %rdx
	movq	%rax, %rcx
	call	strtok
	movq	%rax, 2208(%rbp)
	movq	2208(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$59, %al
	je	.L152
	movq	2208(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$32, %al
	jle	.L152
	movq	2208(%rbp), %rax
	movl	$58, %edx
	movq	%rax, %rcx
	call	contains
	testl	%eax, %eax
	je	.L153
	movq	2208(%rbp), %rax
	movq	%rax, %rcx
	call	isLabel
	movq	%rax, %rdx
	leaq	2032(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	movq	2128(%rbp), %rax
	movq	2136(%rbp), %rdx
	movq	%rax, 1984(%rbp)
	movq	%rdx, 1992(%rbp)
	leaq	2032(%rbp), %rdx
	leaq	1984(%rbp), %rax
	movq	%rax, %rcx
	call	getKeyVal
	movl	%eax, 2160(%rbp)
	movl	2160(%rbp), %edx
	leaq	2032(%rbp), %rax
	movl	%edx, %r8d
	movq	%rax, %rdx
	leaq	.LC59(%rip), %rcx
	call	printf
	jmp	.L152
.L153:
	movq	2208(%rbp), %rax
	movq	%rax, %rcx
	call	getOpcode
	movl	%eax, 2204(%rbp)
	movq	2208(%rbp), %rax
	movq	%rax, %rcx
	call	getCondition
	movl	%eax, 2200(%rbp)
	movq	2208(%rbp), %rax
	movq	%rax, %rcx
	call	isSet
	movl	%eax, 2196(%rbp)
	leaq	.LC60(%rip), %rdx
	movl	$0, %ecx
	call	strtok
	movq	%rax, 2184(%rbp)
	movq	2184(%rbp), %rax
	leaq	.LC61(%rip), %rdx
	movq	%rax, %rcx
	call	strtok
	movq	%rax, 2000(%rbp)
	movl	$1, 2256(%rbp)
	jmp	.L154
.L155:
	leaq	.LC61(%rip), %rdx
	movl	$0, %ecx
	call	strtok
	movq	%rax, %rdx
	movl	2256(%rbp), %eax
	cltq
	movq	%rdx, 2000(%rbp,%rax,8)
	addl	$1, 2256(%rbp)
.L154:
	movl	2204(%rbp), %eax
	movl	%eax, %ecx
	call	noOfArgs
	subl	$1, %eax
	cmpl	%eax, 2256(%rbp)
	jl	.L155
	movl	2204(%rbp), %eax
	movl	%eax, %ecx
	call	noOfArgs
	cmpl	$1, %eax
	jle	.L156
	leaq	.LC62(%rip), %rdx
	movl	$0, %ecx
	call	strtok
	movq	%rax, 2176(%rbp)
	movl	$0, 2252(%rbp)
	movq	2176(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$32, %al
	jne	.L157
	movl	$1, 2252(%rbp)
.L157:
	movl	2252(%rbp), %eax
	movslq	%eax, %rbx
	movl	2204(%rbp), %eax
	movl	%eax, %ecx
	call	noOfArgs
	leal	-1(%rax), %ecx
	movq	2176(%rbp), %rax
	leaq	(%rbx,%rax), %rdx
	movslq	%ecx, %rax
	movq	%rdx, 2000(%rbp,%rax,8)
.L156:
	movl	2196(%rbp), %r8d
	leaq	2000(%rbp), %rcx
	movl	2200(%rbp), %edx
	movl	2204(%rbp), %eax
	movl	%r8d, %r9d
	movq	%rcx, %r8
	movl	%eax, %ecx
	call	printInstruction
	cmpl	$0, 2204(%rbp)
	js	.L158
	cmpl	$5, 2204(%rbp)
	jg	.L158
	movq	2016(%rbp), %r9
	movq	2008(%rbp), %r8
	movq	2000(%rbp), %rdx
	movl	2204(%rbp), %eax
	movl	2196(%rbp), %ecx
	movl	%ecx, 32(%rsp)
	movl	%eax, %ecx
	call	compute
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L158:
	cmpl	$6, 2204(%rbp)
	jne	.L160
	movq	2008(%rbp), %rdx
	movq	2000(%rbp), %rax
	movl	2196(%rbp), %ecx
	movl	%ecx, %r8d
	movq	%rax, %rcx
	call	move
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L160:
	cmpl	$6, 2204(%rbp)
	jle	.L161
	cmpl	$9, 2204(%rbp)
	jg	.L161
	movq	2008(%rbp), %rcx
	movq	2000(%rbp), %rdx
	movl	2204(%rbp), %eax
	movq	%rcx, %r8
	movl	%eax, %ecx
	call	flagger
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L161:
	cmpl	$10, 2204(%rbp)
	jne	.L162
	movq	2016(%rbp), %rcx
	movq	2008(%rbp), %rdx
	movq	2000(%rbp), %rax
	movl	2196(%rbp), %r8d
	movl	%r8d, %r9d
	movq	%rcx, %r8
	movq	%rax, %rcx
	call	mul
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L162:
	cmpl	$11, 2204(%rbp)
	jne	.L163
	movq	2024(%rbp), %r9
	movq	2016(%rbp), %r8
	movq	2008(%rbp), %rdx
	movq	2000(%rbp), %rax
	movl	2196(%rbp), %ecx
	movl	%ecx, 32(%rsp)
	movq	%rax, %rcx
	call	mla
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L163:
	cmpl	$11, 2204(%rbp)
	jle	.L164
	cmpl	$13, 2204(%rbp)
	jg	.L164
	movl	2264(%rbp), %r9d
	movq	2008(%rbp), %r8
	movq	2000(%rbp), %r10
	leaq	-80(%rbp), %rdx
	leaq	post_buffer.4689(%rip), %rax
	movl	$256, %ecx
	movq	%rdx, %rdi
	movq	%rax, %rsi
	rep movsq
	movq	%rsi, %rax
	movq	%rdi, %rdx
	movl	(%rax), %ecx
	movl	%ecx, (%rdx)
	leaq	4(%rdx), %rdx
	leaq	4(%rax), %rax
	movl	2204(%rbp), %eax
	leaq	-80(%rbp), %rdx
	movq	%rdx, 40(%rsp)
	movl	2220(%rbp), %edx
	movl	%edx, 32(%rsp)
	movq	%r10, %rdx
	movl	%eax, %ecx
	call	sdt
	movq	%rax, 2168(%rbp)
	movabsq	$-4294967296, %rax
	andq	2168(%rbp), %rax
	testq	%rax, %rax
	je	.L165
	movq	2168(%rbp), %rax
	shrq	$32, %rax
	movl	%eax, 2164(%rbp)
	movq	2168(%rbp), %rax
	movl	%eax, 2268(%rbp)
	movl	post_buffer.4689(%rip), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	leaq	4+post_buffer.4689(%rip), %rax
	movl	2164(%rbp), %edx
	movl	%edx, (%rcx,%rax)
	movl	post_buffer.4689(%rip), %eax
	addl	$1, %eax
	movl	%eax, post_buffer.4689(%rip)
	jmp	.L159
.L165:
	movq	2168(%rbp), %rax
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L164:
	cmpl	$14, 2204(%rbp)
	jne	.L167
	movl	2264(%rbp), %r9d
	movq	2000(%rbp), %rcx
	movq	2128(%rbp), %rax
	movq	2136(%rbp), %rdx
	movq	%rax, 1984(%rbp)
	movq	%rdx, 1992(%rbp)
	leaq	1984(%rbp), %rax
	movq	%rax, %r8
	movl	%r9d, %edx
	call	branch
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L167:
	cmpl	$15, 2204(%rbp)
	jne	.L168
	movq	2000(%rbp), %rdx
	leaq	2032(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	leaq	2032(%rbp), %rax
	movq	$-1, %rcx
	movq	%rax, %rdx
	movl	$0, %eax
	movq	%rdx, %rdi
	repnz scasb
	movq	%rcx, %rax
	notq	%rax
	leaq	-1(%rax), %rdx
	leaq	2032(%rbp), %rax
	addq	%rdx, %rax
	movl	$1936465964, (%rax)
	movw	$8300, 4(%rax)
	movb	$0, 6(%rax)
	movq	2008(%rbp), %rdx
	leaq	2032(%rbp), %rax
	movq	%rax, %rcx
	call	strcat
	leaq	2032(%rbp), %rax
	movq	%rax, %rcx
	call	puts
	movq	2000(%rbp), %rax
	movl	2196(%rbp), %ecx
	leaq	2032(%rbp), %rdx
	movl	%ecx, %r8d
	movq	%rax, %rcx
	call	move
	movl	%eax, 2268(%rbp)
	jmp	.L159
.L168:
	movl	$1, %eax
	jmp	.L172
.L159:
	movl	2200(%rbp), %eax
	movl	%eax, %ecx
	call	conditionToBinary
	orl	%eax, 2268(%rbp)
	movl	2268(%rbp), %eax
	movb	%al, 2124(%rbp)
	movl	2268(%rbp), %eax
	shrl	$8, %eax
	movb	%al, 2125(%rbp)
	movl	2268(%rbp), %eax
	shrl	$16, %eax
	movb	%al, 2126(%rbp)
	movl	2268(%rbp), %eax
	shrl	$24, %eax
	movb	%al, 2127(%rbp)
	movq	2224(%rbp), %rdx
	leaq	2124(%rbp), %rax
	movq	%rdx, %r9
	movl	$4, %r8d
	movl	$1, %edx
	movq	%rax, %rcx
	call	fwrite
	addl	$32, 2264(%rbp)
.L152:
	movq	2232(%rbp), %rdx
	movq	2240(%rbp), %rax
	movq	%rdx, %r8
	movl	$512, %edx
	movq	%rax, %rcx
	call	fgets
	testq	%rax, %rax
	jne	.L169
	movl	post_buffer.4689(%rip), %eax
	movl	%eax, %edx
	leaq	.LC63(%rip), %rcx
	call	printf
	movl	$0, 2248(%rbp)
	jmp	.L170
.L171:
	movl	2248(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	4+post_buffer.4689(%rip), %rax
	movl	(%rdx,%rax), %eax
	movl	%eax, 2216(%rbp)
	movl	2216(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC64(%rip), %rcx
	call	printf
	movl	2216(%rbp), %eax
	movb	%al, 2120(%rbp)
	movl	2216(%rbp), %eax
	shrl	$8, %eax
	movb	%al, 2121(%rbp)
	movl	2216(%rbp), %eax
	shrl	$16, %eax
	movb	%al, 2122(%rbp)
	movl	2216(%rbp), %eax
	shrl	$24, %eax
	movb	%al, 2123(%rbp)
	movq	2224(%rbp), %rdx
	leaq	2120(%rbp), %rax
	movq	%rdx, %r9
	movl	$4, %r8d
	movl	$1, %edx
	movq	%rax, %rcx
	call	fwrite
	addl	$1, 2248(%rbp)
.L170:
	movl	post_buffer.4689(%rip), %eax
	cmpl	%eax, 2248(%rbp)
	jl	.L171
	movq	2232(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	movq	2224(%rbp), %rax
	movq	%rax, %rcx
	call	fclose
	movq	2240(%rbp), %rax
	movq	%rax, %rcx
	call	free
	movl	$0, %eax
.L172:
	addq	$2408, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	ret
	.seh_endproc
	.globl	symtabContains
	.def	symtabContains;	.scl	2;	.type	32;	.endef
	.seh_proc	symtabContains
symtabContains:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$40, %rsp
	.seh_stackalloc	40
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.seh_endprologue
	movq	%rcx, %rbx
	movq	(%rbx), %rcx
	movq	8(%rbx), %rbx
	movq	%rcx, -128(%rbp)
	movq	%rbx, -120(%rbp)
	movq	%rdx, -56(%rbp)
	movl	$0, -100(%rbp)
	jmp	.L174
.L177:
	movq	-120(%rbp), %rcx
	movl	-100(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
	cmpq	%rax, -56(%rbp)
	jne	.L175
	movl	$1, %eax
	jmp	.L176
.L175:
	addl	$1, -100(%rbp)
.L174:
	movl	-128(%rbp), %eax
	cmpl	%eax, -100(%rbp)
	jl	.L177
	movl	$0, %eax
.L176:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.seh_endproc
	.globl	getKeyVal
	.def	getKeyVal;	.scl	2;	.type	32;	.endef
	.seh_proc	getKeyVal
getKeyVal:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$72, %rsp
	.seh_stackalloc	72
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.seh_endprologue
	movq	%rcx, %rbx
	movq	(%rbx), %rcx
	movq	8(%rbx), %rbx
	movq	%rcx, -96(%rbp)
	movq	%rbx, -88(%rbp)
	movq	%rdx, -24(%rbp)
	movl	$0, -68(%rbp)
	jmp	.L179
.L182:
	movq	-88(%rbp), %rcx
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
	movq	%rax, %rcx
	movq	-24(%rbp), %rax
	movq	%rax, %rdx
	call	equals
	testl	%eax, %eax
	je	.L180
	movq	-88(%rbp), %rcx
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
	movl	20(%rax), %eax
	jmp	.L181
.L180:
	addl	$1, -68(%rbp)
.L179:
	movl	-96(%rbp), %eax
	cmpl	%eax, -68(%rbp)
	jl	.L182
	movl	$-1, %eax
.L181:
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.seh_endproc
	.globl	equals
	.def	equals;	.scl	2;	.type	32;	.endef
	.seh_proc	equals
equals:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rdx
	movq	16(%rbp), %rcx
	call	strcmp
	testl	%eax, %eax
	sete	%al
	movzbl	%al, %eax
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	circularShift
	.def	circularShift;	.scl	2;	.type	32;	.endef
	.seh_proc	circularShift
circularShift:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movl	%r8d, 32(%rbp)
	movl	$31, -4(%rbp)
	movl	32(%rbp), %eax
	andl	-4(%rbp), %eax
	movl	%eax, 32(%rbp)
	cmpl	$0, 16(%rbp)
	je	.L186
	movl	32(%rbp), %eax
	movl	24(%rbp), %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %r8d
	movl	32(%rbp), %eax
	negl	%eax
	andl	-4(%rbp), %eax
	movl	24(%rbp), %edx
	movl	%eax, %ecx
	shrl	%cl, %edx
	movl	%edx, %eax
	orl	%r8d, %eax
	jmp	.L187
.L186:
	movl	32(%rbp), %eax
	movl	24(%rbp), %edx
	movl	%eax, %ecx
	shrl	%cl, %edx
	movl	%edx, %r8d
	movl	32(%rbp), %eax
	negl	%eax
	andl	-4(%rbp), %eax
	movl	24(%rbp), %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	orl	%r8d, %eax
.L187:
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	getRotate
	.def	getRotate;	.scl	2;	.type	32;	.endef
	.seh_proc	getRotate
getRotate:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movb	$0, -1(%rbp)
	jmp	.L189
.L192:
	movl	16(%rbp), %eax
	movb	$0, %al
	testl	%eax, %eax
	je	.L195
	movl	$2, %r8d
	movl	16(%rbp), %edx
	movl	$1, %ecx
	call	circularShift
	movl	%eax, 16(%rbp)
	addb	$2, -1(%rbp)
.L189:
	cmpb	$30, -1(%rbp)
	jbe	.L192
	jmp	.L191
.L195:
	nop
.L191:
	cmpb	$30, -1(%rbp)
	jbe	.L193
	movl	$-1, %eax
	jmp	.L194
.L193:
	movzbl	-1(%rbp), %eax
	sall	$7, %eax
	orl	16(%rbp), %eax
	andl	$4095, %eax
.L194:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC65:
	.ascii "lsr\0"
.LC66:
	.ascii "asr\0"
.LC67:
	.ascii "ror\0"
	.text
	.globl	getShift
	.def	getShift;	.scl	2;	.type	32;	.endef
	.seh_proc	getShift
getShift:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	.LC15(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L197
	movl	$0, %eax
	jmp	.L198
.L197:
	leaq	.LC65(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L199
	movl	$1, %eax
	jmp	.L198
.L199:
	leaq	.LC66(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L200
	movl	$2, %eax
	jmp	.L198
.L200:
	leaq	.LC67(%rip), %rdx
	movq	16(%rbp), %rcx
	call	equals
	testl	%eax, %eax
	je	.L201
	movl	$3, %eax
	jmp	.L198
.L201:
	movl	$4, %eax
.L198:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC68:
	.ascii "%ld\12\0"
.LC69:
	.ascii "%[^, ] %*[,] %[^\12]\0"
.LC70:
	.ascii "%s = %d\12\0"
.LC71:
	.ascii "%[^ ] %s\0"
	.text
	.globl	processOperand2
	.def	processOperand2;	.scl	2;	.type	32;	.endef
	.seh_proc	processOperand2
processOperand2:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$208, %rsp
	.seh_stackalloc	208
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	$0, -4(%rbp)
	movq	16(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$35, %al
	jne	.L203
	movq	16(%rbp), %rax
	leaq	1(%rax), %rcx
	leaq	-32(%rbp), %rax
	movl	$0, %r8d
	movq	%rax, %rdx
	call	strtol
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC68(%rip), %rcx
	call	printf
	movl	-20(%rbp), %eax
	movl	%eax, %ecx
	call	getRotate
	andl	$4095, %eax
	orl	%eax, -4(%rbp)
	orl	$33554432, -4(%rbp)
	jmp	.L204
.L203:
	leaq	-176(%rbp), %rdx
	leaq	-35(%rbp), %rax
	movq	%rdx, %r9
	movq	%rax, %r8
	leaq	.LC69(%rip), %rdx
	movq	16(%rbp), %rcx
	call	sscanf
	leaq	-35(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -5(%rbp)
	movzbl	-5(%rbp), %edx
	leaq	-35(%rbp), %rax
	movl	%edx, %r8d
	movq	%rax, %rdx
	leaq	.LC70(%rip), %rcx
	call	printf
	movzbl	-176(%rbp), %eax
	cmpb	$32, %al
	jle	.L205
	leaq	-112(%rbp), %rcx
	leaq	-38(%rbp), %rdx
	leaq	-176(%rbp), %rax
	movq	%rcx, %r9
	movq	%rdx, %r8
	leaq	.LC71(%rip), %rdx
	movq	%rax, %rcx
	call	sscanf
	leaq	-38(%rbp), %rax
	movq	%rax, %rcx
	call	getShift
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	cmpl	$1, %eax
	je	.L207
	cmpl	$1, %eax
	jb	.L216
	cmpl	$2, %eax
	je	.L209
	cmpl	$3, %eax
	je	.L210
	jmp	.L215
.L207:
	orl	$32, -4(%rbp)
	jmp	.L211
.L209:
	orl	$64, -4(%rbp)
	jmp	.L211
.L210:
	orl	$96, -4(%rbp)
	jmp	.L211
.L215:
	movl	$-1, -4(%rbp)
	jmp	.L211
.L216:
	nop
.L211:
	movzbl	-112(%rbp), %eax
	cmpb	$35, %al
	jne	.L212
	leaq	-112(%rbp), %rax
	addq	$1, %rax
	leaq	-48(%rbp), %rdx
	movl	$0, %r8d
	movq	%rax, %rcx
	call	strtol
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	sall	$7, %eax
	andl	$3968, %eax
	orl	%eax, -4(%rbp)
	jmp	.L205
.L212:
	leaq	-112(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	sall	$8, %eax
	orl	-4(%rbp), %eax
	orl	$16, %eax
	movl	%eax, -4(%rbp)
.L205:
	movzbl	-5(%rbp), %eax
	orl	%eax, -4(%rbp)
.L204:
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC64(%rip), %rcx
	call	printf
	movl	-4(%rbp), %eax
	addq	$208, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC72:
	.ascii "Expression: %s\12\0"
	.text
	.globl	processExpression
	.def	processExpression;	.scl	2;	.type	32;	.endef
	.seh_proc	processExpression
processExpression:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$208, %rsp
	.seh_stackalloc	208
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rdx
	leaq	.LC72(%rip), %rcx
	call	printf
	movl	$0, -4(%rbp)
	movq	16(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$35, %al
	jne	.L218
	movq	16(%rbp), %rax
	leaq	1(%rax), %rcx
	leaq	-32(%rbp), %rax
	movl	$0, %r8d
	movq	%rax, %rdx
	call	strtol
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC68(%rip), %rcx
	call	printf
	cmpl	$0, -20(%rbp)
	jns	.L219
	movl	-20(%rbp), %eax
	sarl	$31, %eax
	xorl	%eax, -20(%rbp)
	subl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	orl	%eax, -4(%rbp)
	jmp	.L221
.L219:
	orl	$8388608, -4(%rbp)
	movl	-20(%rbp), %eax
	orl	%eax, -4(%rbp)
	jmp	.L221
.L218:
	leaq	-176(%rbp), %rdx
	leaq	-35(%rbp), %rax
	movq	%rdx, %r9
	movq	%rax, %r8
	leaq	.LC69(%rip), %rdx
	movq	16(%rbp), %rcx
	call	sscanf
	leaq	-35(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -5(%rbp)
	movzbl	-5(%rbp), %edx
	leaq	-35(%rbp), %rax
	movl	%edx, %r8d
	movq	%rax, %rdx
	leaq	.LC70(%rip), %rcx
	call	printf
	movzbl	-176(%rbp), %eax
	cmpb	$32, %al
	jle	.L222
	leaq	-112(%rbp), %rcx
	leaq	-38(%rbp), %rdx
	leaq	-176(%rbp), %rax
	movq	%rcx, %r9
	movq	%rdx, %r8
	leaq	.LC71(%rip), %rdx
	movq	%rax, %rcx
	call	sscanf
	leaq	-38(%rbp), %rax
	movq	%rax, %rcx
	call	getShift
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	cmpl	$1, %eax
	je	.L224
	cmpl	$1, %eax
	jb	.L233
	cmpl	$2, %eax
	je	.L226
	cmpl	$3, %eax
	je	.L227
	jmp	.L232
.L224:
	orl	$32, -4(%rbp)
	jmp	.L228
.L226:
	orl	$64, -4(%rbp)
	jmp	.L228
.L227:
	orl	$96, -4(%rbp)
	jmp	.L228
.L232:
	movl	$-1, -4(%rbp)
	jmp	.L228
.L233:
	nop
.L228:
	movzbl	-112(%rbp), %eax
	cmpb	$35, %al
	jne	.L229
	leaq	-112(%rbp), %rax
	addq	$1, %rax
	leaq	-48(%rbp), %rdx
	movl	$0, %r8d
	movq	%rax, %rcx
	call	strtol
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	sall	$7, %eax
	andl	$3968, %eax
	orl	%eax, -4(%rbp)
	jmp	.L222
.L229:
	leaq	-112(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	sall	$8, %eax
	orl	-4(%rbp), %eax
	orl	$1, %eax
	movl	%eax, -4(%rbp)
.L222:
	movzbl	-5(%rbp), %eax
	orl	%eax, -4(%rbp)
	orl	$33554432, -4(%rbp)
.L221:
	movl	-4(%rbp), %eax
	addq	$208, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	compute
	.def	compute;	.scl	2;	.type	32;	.endef
	.seh_proc	compute
compute:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movq	%r9, 40(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -12(%rbp)
	cmpl	$5, 16(%rbp)
	ja	.L235
	movl	16(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L237(%rip), %rax
	movl	(%rdx,%rax), %eax
	movslq	%eax, %rdx
	leaq	.L237(%rip), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section .rdata,"dr"
	.align 4
.L237:
	.long	.L236-.L237
	.long	.L238-.L237
	.long	.L239-.L237
	.long	.L240-.L237
	.long	.L241-.L237
	.long	.L242-.L237
	.text
.L240:
	movb	$0, -5(%rbp)
	jmp	.L243
.L241:
	movb	$1, -5(%rbp)
	jmp	.L243
.L238:
	movb	$2, -5(%rbp)
	jmp	.L243
.L239:
	movb	$3, -5(%rbp)
	jmp	.L243
.L236:
	movb	$4, -5(%rbp)
	jmp	.L243
.L242:
	movb	$12, -5(%rbp)
	jmp	.L243
.L235:
	movb	$15, -5(%rbp)
	nop
.L243:
	movq	40(%rbp), %rax
	movq	%rax, %rcx
	call	processOperand2
	movl	%eax, -12(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -13(%rbp)
	movq	32(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -14(%rbp)
	movzbl	-5(%rbp), %eax
	sall	$21, %eax
	orl	%eax, -4(%rbp)
	movl	-12(%rbp), %eax
	orl	%eax, -4(%rbp)
	cmpl	$0, 48(%rbp)
	je	.L244
	orl	$1048576, -4(%rbp)
.L244:
	movzbl	-13(%rbp), %eax
	sall	$12, %eax
	orl	%eax, -4(%rbp)
	movzbl	-14(%rbp), %eax
	sall	$16, %eax
	orl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	move
	.def	move;	.scl	2;	.type	32;	.endef
	.seh_proc	move
move:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movl	%r8d, 32(%rbp)
	movl	$0, -4(%rbp)
	movq	16(%rbp), %rcx
	call	getRegister
	movb	%al, -5(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rcx
	call	processOperand2
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC64(%rip), %rcx
	call	printf
	orl	$27262976, -4(%rbp)
	movzbl	-5(%rbp), %eax
	sall	$12, %eax
	orl	%eax, -4(%rbp)
	movl	-12(%rbp), %eax
	orl	%eax, -4(%rbp)
	cmpl	$0, 32(%rbp)
	je	.L247
	orl	$1048576, -4(%rbp)
.L247:
	movl	-4(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	flagger
	.def	flagger;	.scl	2;	.type	32;	.endef
	.seh_proc	flagger
flagger:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -12(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -13(%rbp)
	movl	16(%rbp), %eax
	cmpl	$8, %eax
	je	.L251
	cmpl	$9, %eax
	je	.L252
	cmpl	$7, %eax
	jne	.L256
	movb	$8, -1(%rbp)
	jmp	.L254
.L251:
	movb	$9, -1(%rbp)
	jmp	.L254
.L252:
	movb	$10, -1(%rbp)
	jmp	.L254
.L256:
	movb	$15, -1(%rbp)
	nop
.L254:
	movq	32(%rbp), %rax
	movq	%rax, %rcx
	call	processOperand2
	movl	%eax, -12(%rbp)
	movzbl	-1(%rbp), %eax
	sall	$21, %eax
	orl	%eax, -8(%rbp)
	movzbl	-13(%rbp), %eax
	sall	$16, %eax
	orl	%eax, -8(%rbp)
	movl	-12(%rbp), %eax
	orl	%eax, -8(%rbp)
	orl	$1048576, -8(%rbp)
	movl	-8(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	shift
	.def	shift;	.scl	2;	.type	32;	.endef
	.seh_proc	shift
shift:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movl	%r8d, 32(%rbp)
	movl	$0, %eax
	popq	%rbp
	ret
	.seh_endproc
	.globl	mul
	.def	mul;	.scl	2;	.type	32;	.endef
	.seh_proc	mul
mul:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movl	%r9d, 40(%rbp)
	movq	16(%rbp), %rcx
	call	getRegister
	movb	%al, -5(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -6(%rbp)
	movq	32(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -7(%rbp)
	movl	$0, -4(%rbp)
	movzbl	-6(%rbp), %eax
	orl	%eax, -4(%rbp)
	orl	$144, -4(%rbp)
	movzbl	-7(%rbp), %eax
	sall	$8, %eax
	orl	%eax, -4(%rbp)
	movzbl	-5(%rbp), %eax
	sall	$16, %eax
	orl	%eax, -4(%rbp)
	cmpl	$0, 40(%rbp)
	je	.L260
	orl	$1048576, -4(%rbp)
.L260:
	movl	-4(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	mla
	.def	mla;	.scl	2;	.type	32;	.endef
	.seh_proc	mla
mla:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	%r8, 32(%rbp)
	movq	%r9, 40(%rbp)
	movq	16(%rbp), %rcx
	call	getRegister
	movb	%al, -5(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -6(%rbp)
	movq	32(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -7(%rbp)
	movq	40(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, -8(%rbp)
	movl	$0, -4(%rbp)
	movzbl	-6(%rbp), %eax
	orl	%eax, -4(%rbp)
	orl	$144, -4(%rbp)
	movzbl	-7(%rbp), %eax
	sall	$8, %eax
	orl	%eax, -4(%rbp)
	movzbl	-8(%rbp), %eax
	sall	$12, %eax
	orl	%eax, -4(%rbp)
	movzbl	-5(%rbp), %eax
	sall	$16, %eax
	orl	%eax, -4(%rbp)
	orl	$2097152, -4(%rbp)
	cmpl	$0, 48(%rbp)
	je	.L263
	orl	$1048576, -4(%rbp)
.L263:
	movl	-4(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC73:
	.ascii "Error, using immediate with STR\0"
.LC74:
	.ascii "%*[[] %[^]] %*[], ] %s \0"
.LC75:
	.ascii "%*[[] %[^,] %*[, ] %[^]] \0"
	.text
	.globl	sdt
	.def	sdt;	.scl	2;	.type	32;	.endef
	.seh_proc	sdt
sdt:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$248, %rsp
	.seh_stackalloc	248
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.seh_endprologue
	movl	%ecx, 144(%rbp)
	movq	%rdx, 152(%rbp)
	movq	%r8, 160(%rbp)
	movl	%r9d, 168(%rbp)
	movq	184(%rbp), %rbx
	movl	168(%rbp), %eax
	addl	$64, %eax
	movl	%eax, 76(%rbp)
	movl	$0, 72(%rbp)
	movq	152(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, 71(%rbp)
	movzbl	71(%rbp), %edx
	movq	152(%rbp), %rax
	movl	%edx, %r8d
	movq	%rax, %rdx
	leaq	.LC70(%rip), %rcx
	call	printf
	movb	$0, 111(%rbp)
	movl	$0, 92(%rbp)
	movl	92(%rbp), %eax
	movl	%eax, 96(%rbp)
	movl	96(%rbp), %eax
	movl	%eax, 100(%rbp)
	movl	100(%rbp), %eax
	movl	%eax, 104(%rbp)
	movl	$0, 88(%rbp)
	movq	$0, 80(%rbp)
	movq	160(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$61, %al
	jne	.L266
	movl	$1, 104(%rbp)
	cmpl	$12, 144(%rbp)
	jne	.L267
	movq	160(%rbp), %rax
	leaq	1(%rax), %rcx
	leaq	56(%rbp), %rax
	movl	$0, %r8d
	movq	%rax, %rdx
	call	strtol
	movl	%eax, 64(%rbp)
	movl	$0, 92(%rbp)
	cmpl	$255, 64(%rbp)
	ja	.L268
	leaq	-32(%rbp), %rax
	movw	$35, (%rax)
	movq	160(%rbp), %rax
	leaq	1(%rax), %rdx
	leaq	-32(%rbp), %rax
	movq	%rax, %rcx
	call	strcat
	leaq	-32(%rbp), %rdx
	movq	152(%rbp), %rax
	movl	$0, %r8d
	movq	%rax, %rcx
	call	move
	movl	%eax, %eax
	jmp	.L269
.L268:
	movl	64(%rbp), %eax
	movq	%rax, 80(%rbp)
	movl	$0, 104(%rbp)
	movl	$1, 96(%rbp)
	movl	96(%rbp), %eax
	movl	%eax, 100(%rbp)
	movb	$15, 111(%rbp)
	movl	(%rbx), %eax
	sall	$5, %eax
	movl	%eax, %edx
	movl	176(%rbp), %eax
	addl	%edx, %eax
	subl	76(%rbp), %eax
	leal	7(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$3, %eax
	movl	%eax, 88(%rbp)
	jmp	.L270
.L267:
	leaq	.LC73(%rip), %rcx
	call	puts
	movq	$-1, %rax
	jmp	.L269
.L266:
	movq	160(%rbp), %rax
	addq	$3, %rax
	movzbl	(%rax), %eax
	cmpb	$93, %al
	je	.L271
	movq	160(%rbp), %rax
	addq	$4, %rax
	movzbl	(%rax), %eax
	cmpb	$93, %al
	jne	.L272
.L271:
	leaq	-96(%rbp), %rcx
	leaq	53(%rbp), %rdx
	movq	160(%rbp), %rax
	movq	%rcx, %r9
	movq	%rdx, %r8
	leaq	.LC74(%rip), %rdx
	movq	%rax, %rcx
	call	sscanf
	leaq	-96(%rbp), %rax
	movq	%rax, %rcx
	call	puts
	movzbl	-96(%rbp), %eax
	cmpb	$32, %al
	jle	.L273
	movl	$0, 100(%rbp)
	jmp	.L275
.L273:
	movl	$1, 100(%rbp)
	jmp	.L275
.L272:
	leaq	-96(%rbp), %rcx
	leaq	53(%rbp), %rdx
	movq	160(%rbp), %rax
	movq	%rcx, %r9
	movq	%rdx, %r8
	leaq	.LC75(%rip), %rdx
	movq	%rax, %rcx
	call	sscanf
	movl	$1, 100(%rbp)
.L275:
	movzbl	-96(%rbp), %eax
	cmpb	$43, %al
	jne	.L276
	leaq	-96(%rbp), %rax
	addq	$1, %rax
	leaq	-32(%rbp), %rcx
	movq	%rax, %rdx
	call	strcpy
	leaq	-32(%rbp), %rdx
	leaq	-96(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	movl	$1, 96(%rbp)
	jmp	.L277
.L276:
	movzbl	-96(%rbp), %eax
	cmpb	$45, %al
	jne	.L278
	leaq	-96(%rbp), %rax
	addq	$1, %rax
	leaq	-32(%rbp), %rcx
	movq	%rax, %rdx
	call	strcpy
	leaq	-32(%rbp), %rdx
	leaq	-96(%rbp), %rax
	movq	%rax, %rcx
	call	strcpy
	movl	$0, 96(%rbp)
	jmp	.L277
.L278:
	movzbl	-96(%rbp), %eax
	cmpb	$114, %al
	je	.L279
	movzbl	-96(%rbp), %eax
	cmpb	$32, %al
	jg	.L277
.L279:
	movl	$1, 96(%rbp)
.L277:
	movzbl	-96(%rbp), %eax
	cmpb	$32, %al
	jle	.L280
	leaq	-96(%rbp), %rax
	movq	%rax, %rcx
	call	processExpression
	movl	%eax, 88(%rbp)
.L280:
	movl	88(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC64(%rip), %rcx
	call	printf
	leaq	53(%rbp), %rax
	movq	%rax, %rcx
	call	getRegister
	movb	%al, 111(%rbp)
.L270:
	cmpl	$12, 144(%rbp)
	jne	.L281
	movl	$1, 92(%rbp)
.L281:
	orl	$67108864, 72(%rbp)
	movl	104(%rbp), %eax
	sall	$25, %eax
	orl	%eax, 72(%rbp)
	movl	100(%rbp), %eax
	sall	$24, %eax
	orl	%eax, 72(%rbp)
	movl	96(%rbp), %eax
	sall	$23, %eax
	orl	%eax, 72(%rbp)
	movl	92(%rbp), %eax
	sall	$20, %eax
	orl	%eax, 72(%rbp)
	movzbl	111(%rbp), %eax
	sall	$16, %eax
	orl	%eax, 72(%rbp)
	movzbl	71(%rbp), %eax
	sall	$12, %eax
	orl	%eax, 72(%rbp)
	movl	88(%rbp), %eax
	orl	%eax, 72(%rbp)
	movq	80(%rbp), %rax
	salq	$32, %rax
	movq	%rax, %rdx
	movl	72(%rbp), %eax
	orq	%rdx, %rax
.L269:
	addq	$248, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC76:
	.ascii "Current Locctr: %u\12\0"
	.text
	.globl	branch
	.def	branch;	.scl	2;	.type	32;	.endef
	.seh_proc	branch
branch:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$88, %rsp
	.seh_stackalloc	88
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.seh_endprologue
	movq	%rcx, -16(%rbp)
	movl	%edx, -8(%rbp)
	movq	%r8, %rbx
	movq	(%rbx), %rax
	movq	8(%rbx), %rdx
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
	movl	$0, -52(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC76(%rip), %rcx
	call	printf
	movq	-80(%rbp), %rax
	movq	-72(%rbp), %rdx
	movq	%rax, -96(%rbp)
	movq	%rdx, -88(%rbp)
	leaq	-96(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rax, %rcx
	call	getKeyVal
	movl	%eax, -56(%rbp)
	movl	-8(%rbp), %eax
	leal	64(%rax), %edx
	movl	-56(%rbp), %eax
	subl	%edx, %eax
	leal	7(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$3, %eax
	movl	%eax, -56(%rbp)
	sarl	$2, -56(%rbp)
	movl	-56(%rbp), %eax
	andl	$16777215, %eax
	orl	%eax, -52(%rbp)
	orl	$167772160, -52(%rbp)
	movl	-52(%rbp), %eax
	addq	$88, %rsp
	popq	%rbx
	popq	%rbp
	ret
	.seh_endproc
.lcomm post_buffer.4689,2052,32
	.ident	"GCC: (Rev1, Built by MSYS2 project) 7.2.0"
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	putchar;	.scl	2;	.type	32;	.endef
	.def	strlen;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	strcpy;	.scl	2;	.type	32;	.endef
	.def	strtok;	.scl	2;	.type	32;	.endef
	.def	calloc;	.scl	2;	.type	32;	.endef
	.def	fprintf;	.scl	2;	.type	32;	.endef
	.def	fopen;	.scl	2;	.type	32;	.endef
	.def	fwrite;	.scl	2;	.type	32;	.endef
	.def	fgets;	.scl	2;	.type	32;	.endef
	.def	fseek;	.scl	2;	.type	32;	.endef
	.def	strcat;	.scl	2;	.type	32;	.endef
	.def	fwrite;	.scl	2;	.type	32;	.endef
	.def	fclose;	.scl	2;	.type	32;	.endef
	.def	free;	.scl	2;	.type	32;	.endef
	.def	strcmp;	.scl	2;	.type	32;	.endef
	.def	strtol;	.scl	2;	.type	32;	.endef
	.def	sscanf;	.scl	2;	.type	32;	.endef
