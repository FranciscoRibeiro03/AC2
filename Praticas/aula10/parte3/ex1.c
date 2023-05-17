#include <detpic32.h>

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

void putc(char byte) {
        while (U2STAbits.UTXBF == 1);
        U2TXREG = byte;
}

void putstr(char *str) {
        while (str != '\0') putc(*(str++));
}

int main(void) {
        // Configure UART2:
        // 1 - Configure BaudRate Generator
        U2BRG = ((PBCLK + 8 * 115200) / (16 * 115200)) - 1;
        U2MODEbits.BRGH = 0;

        // 2 – Configure number of data bits, parity and number of stop bits
        // (see U2MODE register)
        U2MODEbits.PDSEL = 0;
        U2MODEbits.STSEL = 0;

        // 3 – Enable the trasmitter and receiver modules (see register U2STA)
        U2STAbits.UTXEN = 1;
        U2STAbits.URXEN = 1;

        // 4 – Enable UART2 (see register U2MODE)
        U2MODEbits.ON = 1;

        // Configure RD11 as output
        TRISDbits.TRISD11 = 0;

        while (1) {
                while (TRMT == 0);
                LATDbits.LATD11 = 1;
                putstr("12345");
                LATDbits.LATD11 = 0;
        }
        return 0;
}
