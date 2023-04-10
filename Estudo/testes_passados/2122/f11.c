#include <detpic32.h>

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

int main() {
        // RE5 a RE0 como portos de saída
        // 1111 1111 1100 0000
        TRISE = TRISE & 0xFFC0;
        // RB2 como porto de entrada
        // 0000 0000 0000 0100
        TRISB = TRISB | 0x0007;

        unsigned int value, ms;

        value = 1;

        while (1) {
                LATE = (LATE & 0xFFC0) | value;

                value = value << 1;
                if (value > 0b100000) value = 1;

                ms = ((PORTB >> 2) & 0x0001) ? 
                        1000 / 7 : // Porto RB2 está ON
                        1000 / 3;  // Porto RB2 está OFF

                delay(ms);
        }

        return 0;
}
