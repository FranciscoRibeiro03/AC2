        .equ    READ_CORE_TIMER, 11
        .equ    RESET_CORE_TIMER, 12

        .equ    SFR_BASE_HI, 0xBF88
        .equ    TRISE, 0x6100
        .equ    LATE, 0x6120

        .text
        .globl main


# Register Map
# cnt: $t2
main:   lui     $t0, SFR_BASE_HI
        lw      $t1, TRISE($t0)
        andi    $t1, $t1, 0xFFE1
        sw      $t1, TRISE($t0)

        li      $t2, 0

do:     sll     $t3, $t2, 1
        lw      $t1, LATE($t0)
        andi    $t1, $t1, 0xFFE1
        or      $t1, $t1, $t3
        sw      $t1, LATE($t0)

        li      $a0, 1000
        jal     delay

        addiu   $t2, $t2, 1
if:     bne     $t2, 16, endif
        li      $t2, 0
endif:  j       do

        jr      $ra




# Register Map
# ms: $a0
delay:  li      $v0, RESET_CORE_TIMER
        syscall
        mul     $a0, $a0, 20000
while:  li      $v0, READ_CORE_TIMER
        syscall
        blt     $v0, $a0, while
        jr      $ra
