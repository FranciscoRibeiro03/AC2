#include <detpic32.h>

#define VECTOR_UART2 32

volatile int minCount;

void putc(char byte2send) {
        while (U2STAbits.UTXBF == 1);
        U2TXREG = byte2send;
}

void putstr(char *str) {
        while (*str != '\0') putc(*str++);
}

int main() {
        int baudrate = 9600;
        U2BRG = ((PBCLK + (8 * baudrate)) / (16 * baudrate)) - 1;
        U2MODEbits.BRGH = 0;
        U2MODEbits.PDSEL = 1;
        U2MODEbits.STSEL = 0;
        U2STAbits.UTXEN = 1;
        U2STAbits.URXEN = 1;
        U2MODEbits.ON = 1;

        U2STAbits.URXISEL = 0;

        IFS1bits.U2RXIF = 0;
        IEC1bits.U2RXIE = 1;
        IPC8bits.U2IP = 2;

        EnableInterrupts();

        minCount = 0;

        while (1);

        return 0;
}

void _int_(VECTOR_UART2) isr_uart2(void) {
        if (IFS1bits.U2RXIF) {
                char c;
                c = U2RXREG;

                putc(c);

                if (c >= 'a' && c <= 'z') minCount++;

                if (c == 0x0A) {
                        putstr("O numero de minusculas digitado foi ");
                        if (minCount != 0) {
                                char str[10];
                                int i = 0;
                                while (minCount != 0) {
                                        str[i++] = '0' + (minCount % 10);
                                        minCount /= 10;
                                }
                                str[i] = '\0';
                                int j;
                                for (j = i - 1; j >= 0; j--) {
                                        putc(str[j]);
                                }
                        } else putc('0');
                        putc('\n');
                        minCount = 0;
                }

                IFS1bits.U2RXIF = 0;
        }
}
