#include <detpic32.h>

#define VECTOR_UART2 32

void putc(char byte2send) {
        while (U2STAbits.UTXBF == 1);
        U2TXREG = byte2send;
}

void putstring(char* string) {
        while (*string != '\0') putc(*string++);
}

volatile int count;

int main() {
        TRISE = TRISE & 0xFFE1; // xxxx xxxx xxx0 000x

        U2BRG = ((PBCLK + 8 * 9600) / (16 * 9600)) - 1;
        U2MODEbits.BRGH = 0;
        U2MODEbits.PDSEL = 2;
        U2MODEbits.STSEL = 1;
        U2STAbits.UTXEN = 1;
        U2STAbits.URXEN = 1;
        U2MODEbits.ON = 1;

        IPC8bits.U2IP = 2;
        IEC1bits.U2RXIE = 1;
        IFS1bits.U2RXIF = 0;

        EnableInterrupts();

        count = 15;

        while (1);

        return 0;
}

void _int_(VECTOR_UART2) isr_uart2(void) {
        if (IFS1bits.U2RXIF == 1) {
                char c;
                c = U2RXREG;
                if (c == 'U') {
                        count = (count + 1) % 16;
                        LATE = (LATE & 0xFFE1) | (count << 1);
                } else if (c == 'R') {
                        count = 0;
                        LATE = (LATE & 0xFFE1) | (count << 1);
                        putstring("RESET\n");
                }

                IFS1bits.U2RXIF = 0;
        }
}
