        .equ    SFR_BASE_HI, 0xBF88
        .equ    TRISE, 0x6100
        .equ    LATE, 0x6120
        .equ    TRISB, 0x6040
        .equ    PORTB, 0x6050

        .text
        .globl main

main:   lui     $t1, SFR_BASE_HI
        lw      $t2, TRISE($t1)
        andi    $t2, $t2, 0xFFFE
        sw      $t2, TRISE($t1)                 # TRISE = TRISE & 0xFFFE
        
        lw      $t2, TRISB($t1)
        ori     $t2, $t2, 0x0001
        sw      $t2, TRISB($t1)                 # TRISB = TRISB & 0x0001

loop:   lw      $t2, PORTB($t1)
        not     $t2, $t2
        andi    $t2, $t2, 0x0001

        lw      $t3, LATE($t1)
        andi    $t3, $t3, 0xFFFE
        or      $t3, $t3, $t2
        sw      $t3, LATE($t1)

        j       loop

        jr      $ra
