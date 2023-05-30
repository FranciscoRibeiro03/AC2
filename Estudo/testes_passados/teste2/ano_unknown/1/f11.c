#include <detpic32.h>

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

int main() {
        TRISB = TRISB | 0x000F;
        TRISE = TRISE & 0xFF00;

        unsigned int counter = 255;

        while (1) {
                if ((counter & 0x00FF) >= 255) {
                        counter = ~PORTB & 0x000F;
                } else {
                        counter = (counter << 1) | 0x1;
                }
                LATE = (LATE & 0xFF00) | (counter & 0x00FF);
                delay(1000);
        }
        return 0;
}
