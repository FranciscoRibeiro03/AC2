	.equ	GET_CHAR, 2
	.equ	PUT_CHAR, 3

	.text
	.globl main

# Mapa de registos
# c : $t0
main:	li	$v0, GET_CHAR
	syscall
	move	$t0, $v0
	li	$v0, PUT_CHAR
	move	$a0, $t0
	addiu	$a0, $a0, 1
	syscall
	bne	$t0, '\n', main
	li	$v0, 0
	jr	$ra
