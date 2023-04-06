        .equ    PRINT_STR, 8
        .equ    PRINT_INT, 6
        .equ    READ_INT10, 5
        .equ    PRINT_INT10, 7

        .data
str1:   .asciiz "\nIntroduza um inteiro (sinal e módulo): "
str2:   .asciiz "\nValor em base 10 (signed): "
str3:   .asciiz "\nValor em base 2: "
str4:   .asciiz "\nValor em base 16: "
str5:   .asciiz "\nValor em base 10 (unsigned): "
str6:   .asciiz "\nValor em base 10 (unsigned), formatado: "

        .text
        .globl main

# Mapa de registos
# value : $t0
main:   li      $v0, PRINT_STR
        la      $a0, str1
        syscall

        li      $v0, READ_INT10
        syscall
        move    $t0, $v0

        li      $v0, PRINT_STR
        la      $a0, str2
        syscall

        li      $v0, PRINT_INT10
        move    $a0, $t0
        syscall

        li      $v0, PRINT_STR
        la      $a0, str3
        syscall

        li      $v0, PRINT_INT
        move    $a0, $t0
        li      $a1, 2
        syscall

        li      $v0, PRINT_STR
        la      $a0, str4
        syscall

        li      $v0, PRINT_INT
        move    $a0, $t0
        li      $a1, 16
        syscall

        li      $v0, PRINT_STR
        la      $a0, str5
        syscall

        li      $v0, PRINT_INT
        move    $a0, $t0
        li      $a1, 10
        syscall

        li      $v0, PRINT_STR
        la      $a0, str6
        syscall

        li      $v0, PRINT_INT
        move    $a0, $t0
        li      $a1, 10
        li      $t1, 5
        sll     $t1, $t1, 16
        or      $a1, $a1, $t1
        syscall

        j       main

        li      $v0, 0
        jr      $ra
