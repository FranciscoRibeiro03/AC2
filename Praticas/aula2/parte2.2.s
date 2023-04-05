	.equ	READ_CORE_TIMER, 11
	.equ	RESET_CORE_TIMER, 12
	.equ	PUT_CHAR, 3
	.equ	PRINT_INT, 6
	
	.data
	.text
	.globl	main

# Mapa de Registos
# $t0: cnt1
# $t1: cnt5
# $t2: cnt10
main:	li	$t0, 0
	li	$t1, 0
	li	$t2, 0
	
while:
	li	$a0, 100
	jal	delay
	
	addi	$t2, $t2, 1
	
if:
	rem	$t3, $t2, 2
	bnez	$t3, elseif
	
	addi	$t1, $t1, 1
	
elseif:
	rem	$t3, $t2, 10
	bnez	$t3, endif
	
	addi	$t0, $t0, 1
endif:
	li	$v0, PRINT_INT
	move	$a0, $t0
	li	$a1, 5
	sll	$a1, $a1, 16
	ori	$a1, $a1, 10
	syscall
	
	li	$v0, PUT_CHAR
	li	$a0, '\t'
	syscall
	
	li	$v0, PRINT_INT
	move	$a0, $t1
	li	$a1, 5
	sll	$a1, $a1, 16
	ori	$a1, $a1, 10
	syscall
	
	li	$v0, PUT_CHAR
	li	$a0, '\t'
	syscall
	
	li	$v0, PRINT_INT
	move	$a0, $t2
	li	$a1, 5
	sll	$a1, $a1, 16
	ori	$a1, $a1, 10
	syscall
	
	li	$v0, PUT_CHAR
	li	$a0, '\r'
	syscall
	
endw:
	jr	$ra
	
delay:	li	$v0, RESET_CORE_TIMER
	syscall
	
while1:	li	$v0, READ_CORE_TIMER
	syscall
	
	li	$t4, 20000
	mul	$t4, $t4, $a0
	
	bge	$v0, $t4, endw1
	j	while1
endw1:
	jr	$ra
