	.equ	BASE, 0xBF88
	.equ	TRISE, 0x6100
	.equ	LATE, 0x6120
	.equ	TRISB, 0x6040
	.equ	PORTB, 0x6050
	.data
	.text
	.globl	main
	
main:	lui	$t0, BASE
	
	lw	$t1, TRISB($t0)
	ori	$t1, $t1, 0x0001
	sw	$t1, TRISB($t0)		# Configurar RB0 como input
	
	lw	$t1, TRISE($t0)
	andi	$t1, $t1, 0xFFFE
	sw	$t1, TRISE($t0)		# Configurar RE0 como output
	
loop:	lw	$t1, PORTB($t0)
	not	$t1, $t1
	andi	$t1, $t1, 0x0001	# Ler valor de RB0
	sll	$t1, $t1, 1
	lw	$t2, LATE($t0)
	andi	$t2, $t2, 0xFFFD
	or	$t2, $t2, $t1
	sw	$t2, LATE($t0)		# Escrever valor em RE0
	j	loop			# Loop infinito
	jr	$ra
