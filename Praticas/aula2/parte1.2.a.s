	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12
	
	.data
	.text
	.globl	main
	
main:	li	$v0, RESET_CORE_TIMER
	syscall
	
while:	li	$v0, READ_CORE_TIMER
	syscall
	
	li	$t0, 20000
	mul	$t0, $t0, $a0
	
	bge	$v0, $t0, endw
	j	while
endw:
	jr	$ra
