#include <detpic32.h>

void setDutyCycle(unsigned int dc) {
        OC2RS = ((PR2 + 1) * dc) / 100;
}

int main() {
        TRISB = TRISB | 0x0009; // xxxx xxxx xxxx 1xx1

        T2CONbits.TCKPS = 2;
        PR2 = 33332;
        TMR2 = 0;
        T2CONbits.TON = 1;

        OC2CONbits.OCM = 6;
        OC2CONbits.OCTSEL = 0;
        setDutyCycle(25);
        OC1CONbits.ON = 1;

        while (1) {
                unsigned int value = PORTB & 0x0009;
                if (value == 0b0001) setDutyCycle(25);
                else if (value == 0b1000) setDutyCycle(70);

                // Delay de 250 us
                resetCoreTimer();
                while (readCoreTimer() < 5000);
        }

        return 0;
}
