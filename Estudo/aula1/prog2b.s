	.equ    INKEY, 1
	.equ    PUT_CHAR, 3

	.text
	.globl main

# Mapa de registos
# c : $t0
main:   li      $v0, INKEY
	syscall
	move    $t0, $v0	# c = inkey()

if:	beq	$t0, 0, else	# if (c != 0)
	li	$v0, PUT_CHAR
	move	$a0, $t0
	syscall			#   putChar(c)
	j	endif		# else

else:	li	$v0, PUT_CHAR
	li	$a0, '.'
	syscall			#   putChar('.')

endif:	bne	$t0, '\n', main
	li	$v0, 0
	jr	$ra
