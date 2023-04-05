	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12
	.equ	PUT_CHAR, 3
	.equ	PRINT_INT, 6
	
	.data
	.text
	.globl	main
	
main:	li	$t0, 0
while:
	li	$v0, RESET_CORE_TIMER
	syscall
	
while1:	li	$v0, READ_CORE_TIMER
	syscall
	bge	$v0, 200000, endw1
	j	while1
	
endw1:	li	$v0, PRINT_INT
	move	$a0, $t0
	li	$a1, 4
	sll	$a1, $a1, 16
	ori	$a1, $a1, 10
	syscall
	addiu	$t0, $t0, 1
	
	li	$v0, PUT_CHAR
	li	$a0, '\r'
	syscall
	
	j	while
	
	li	$v0, 0
	jr	$ra
