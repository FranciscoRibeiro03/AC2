#include <detpic32.h>

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

int main(void) {
        TRISE = TRISE & 0xFFFE; // 1111 1111 1111 1110
        TRISD = TRISD | 0x0100; // 0000 0001 0000 0000

        while (1) {
                if (((PORTD >> 8) & 0x0001) == 0) {
                        LATE = LATE | 0x0001;
                        delay(3000);
                        LATE = LATE & 0xFFFE;
                }
        }
}
