#include <detpic32.h>

#define NSamples 2

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

void sendToDisplay(unsigned int value) {
        static const char codes[] = {
                //gfedcba
                0b0111111, // 0
                0b0000110, // 1
                0b1011011, // 2
                0b1001111, // 3
                0b1100110, // 4
                0b1101101, // 5
                0b1111101, // 6
                0b0000111, // 7
                0b1111111, // 8
                0b1101111, // 9
                0b1110111, // a
                0b1111100, // b
                0b0111001, // c
                0b1011110, // d
                0b1111001, // e
                0b1110001  // f
        };

        LATB = (LATB & 0x80FF) | (codes[value] << 8);
}

int main() {
        TRISBbits.TRISB4 = 1;
        AD1PCFGbits.PCFG4 = 0;
        AD1CON1bits.SSRC = 7;
        AD1CON1bits.CLRASAM = 1;
        AD1CON3bits.SAMC = 16;
        AD1CON2bits.SMPI = NSamples - 1;
        AD1CHSbits.CH0SA = 4;
        AD1CON1bits.ON = 1;

        // Portos RD5, RD6 e RB8-RB14 configurados como saída (displays)
        // TRISD : 1111 1111 1001 1111
        // TRISB : 1000 0000 1111 1111
        TRISD = TRISD & 0xFF9F;
        TRISB = TRISB & 0x80FF;

        // Porto RE1 configurado como saída (LED1), com valor inicial 0
        TRISEbits.TRISE1 = 0;
        LATEbits.LATE1 = 0;

        while (1) {
                AD1CON1bits.ASAM = 1;
                while (IFS1bits.AD1IF == 0);

                // Parte i)
                int media = 0;
                int i;
                int *p = (int *)(&ADC1BUF0);
                for (i = 0; i < NSamples; i++) {
                        media += p[i * 4];
                }
                media = media / NSamples;

                printInt(media, 16 | 3 << 16);
                putChar('\n');

                // Parte ii)
                LATDbits.LATD5 = 1;
                LATDbits.LATD6 = 0;
                int value = (media * 9) / 1023;
                sendToDisplay(value);

                // Parte iii)
                LATEbits.LATE1 = !LATEbits.LATE1;

                IFS1bits.AD1IF = 0;
                delay(200);
        }
}
