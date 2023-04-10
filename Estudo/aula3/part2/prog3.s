        .equ    READ_CORE_TIMER, 11
        .equ    RESET_CORE_TIMER, 12

        .equ    SFR_BASE_HI, 0xBF88
        .equ    TRISE, 0x6100
        .equ    LATE, 0x6120
        .equ    TRISB, 0x6040
        .equ    PORTB, 0x6050

        .text
        .globl main


# Register Map
# cnt: $t2
main:   lui     $t0, SFR_BASE_HI
        lw      $t1, TRISE($t0)
        andi    $t1, $t1, 0xFFE1
        sw      $t1, TRISE($t0)

        lw      $t1, TRISB($t0)
        ori     $t1, $t1, 0x0008
        sw      $t1, TRISB($t0)

        li      $t2, 0

do:     sll     $t3, $t2, 1
        lw      $t1, LATE($t0)
        andi    $t1, $t1, 0xFFE1
        or      $t1, $t1, $t3
        sw      $t1, LATE($t0)

        lw      $t1, PORTB($t0)
        srl     $t1, 3
        andi    $t1, $t1, 0x0001

if1:    bne     $t1, 1, else1
        addiu   $t2, $t2, 1
        blt     $t2, 16, endif1
        li      $t2, 0
        j       endif1

else1:  addiu   $t2, $t2, -1
        bge     $t2, 0, endif1
        li      $t2, 15

endif1: li      $a0, 500
        jal     delay

        j       do

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
