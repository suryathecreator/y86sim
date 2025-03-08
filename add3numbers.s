	.file	"add3numbers.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.globl	add3
	.type	add3, @function
add3:
.LFB23:
	.cfi_startproc
	addl	%esi, %edi
	leal	(%rdi,%rdx), %eax
	ret
	.cfi_endproc
.LFE23:
	.size	add3, .-add3
	.ident	"GCC: (GNU) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
