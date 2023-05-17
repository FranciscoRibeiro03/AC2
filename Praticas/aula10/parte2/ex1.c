#include <detpic32.h>

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

void putc(char byte) {
        while (U2STAbits.UTXBF == 1);
        U2TXREG = byte;
}

void putc1(char byte) {
        while (U1STAbits.UTXBF == 1);
        U1TXREG = byte;
}

void putstr(char *str) {
        while (str != '\0') putc(*(str++));
}

char getc() {
        while (U2STAbits.URXDA == 0);
        return U2RXREG;
}

int main(void) {
        // Configure UART1:
        // 1 - Configure BaudRate Generator
        U1BRG = ((PBCLK + 8 * 115200) / (16 * 115200)) - 1;
        U1MODEbits.BRGH = 0;

        // 2 – Configure number of data bits, parity and number of stop bits
        // (see U1MODE register)
        U1MODEbits.PDSEL = 0;
        U1MODEbits.STSEL = 0;

        // 3 – Enable the trasmitter and receiver modules (see register U1STA)
        U1STAbits.UTXEN = 1;
        U1STAbits.URXEN = 1;

        // 4 – Enable UART1 (see register U1MODE)
        U1MODEbits.ON = 1;

        while (1) {
                putc(0x5A);
                delay(10);
        }
        return 0;
}
