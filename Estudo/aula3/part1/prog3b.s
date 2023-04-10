        .equ    SFR_BASE_HI, 0xBF88
        .equ    TRISE, 0x6100
        .equ    LATE, 0x6120
        .equ    TRISD, 0x60C0
        .equ    PORTD, 0x60D0

        .text
        .globl main

main:   lui     $t1, SFR_BASE_HI
        lw      $t2, TRISE($t1)
        andi    $t2, $t2, 0xFFFE
        sw      $t2, TRISE($t1)                 # TRISE = TRISE & 0xFFFE
        
        lw      $t2, TRISD($t1)
        ori     $t2, $t2, 0x0100
        sw      $t2, TRISD($t1)                 # TRISD = TRISD | 0x0100

loop:   lw      $t2, PORTD($t1)
        not     $t2, $t2
        srl     $t2, $t2, 8
        andi    $t2, $t2, 0x0001

        lw      $t3, LATE($t1)
        andi    $t3, $t3, 0xFFFE
        or      $t3, $t3, $t2
        sw      $t3, LATE($t1)

        j       loop

        jr      $ra
