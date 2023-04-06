	.equ    INKEY, 1
	.equ    PUT_CHAR, 3
	.equ    PRINT_INT, 6

	.equ	UP, 1
	.equ	DOWN, 0

	.text
	.globl main

# Register Map
# state : $s0
# cnt : $s1
# c : $t2
main:	addiu	$sp, $sp, -12
	sw	$ra, 0($sp)
	sw	$s0, 4($sp)
	sw	$s1, 8($sp)

	li	$s0, 0				# int state = 0;
	li	$s1, 0				# int cnt = 0;

do:	li	$v0, PUT_CHAR			# do {
	li	$a0, '\r'
	syscall					#   putChar('\r');

	li	$v0, PRINT_INT
	move	$a0, $s1
	li	$a1, 10
	li	$t3, 3
	sll	$t3, $t3, 16
	or	$a1, $a1, $t3
	syscall					#   printInt(cnt, 10 | 3 << 16);

	li	$v0, PUT_CHAR
	li	$a0, '\t'
	syscall					#   putChar('\t');

	li	$v0, PRINT_INT
	move	$a0, $s1
	li	$a1, 2
	li	$t3, 8
	sll	$t3, $t3, 16
	or	$a1, $a1, $t3
	syscall					#   printInt(cnt, 10 | 3 << 16);

	li	$a0, 5
	jal	wait				#   wait(5);

	li	$v0, INKEY
	syscall	
	move	$t2, $v0			#   c = inkey();

if1:	bne	$t2, '+', endif1		#   if (c == '+')
	li	$s0, UP				#     state = UP;
endif1:	bne	$t2, '-', endif2		#   if (c == '-')
	li	$s0, DOWN			#     state = DOWN;

endif2:	bne	$s0, UP, else3			#   if (state == UP)
	addiu	$t3, $s1, 1
	andi	$s1, $t3, 0xFF			#     cnt = (cnt + 1) & 0xFF;
	j	endif3
else3:	addiu	$t3, $s1, -1			#   else
	andi	$s1, $t3, 0xFF			#     cnt = (cnt - 1) & 0xFF;

endif3:	bne	$t2, 'q', do			# } while (c != 'q');

	li	$v0, 0				# return 0;

	lw	$s1, 8($sp)
	lw	$s0, 4($sp)
	lw	$ra, 0($sp)
	addiu	$sp, $sp, 12
	jr	$ra

# Register Map
# i : $t0
# ts : $a0
wait:	li	$t0, 0
	mul	$t1, $a0, 515000
for:	bge	$t0, $t1, endf
	addiu	$t0, $t0, 1
	j	for
endf:	jr	$ra
