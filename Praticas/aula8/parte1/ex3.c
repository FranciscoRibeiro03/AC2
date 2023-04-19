#include <detpic32.h>

int main(void) {
        // Configuração do Timer 3 para eventos a 2 Hz
        // Constante do Prescaler ( fPBCLK / ((65535 + 1) * fOut) )
        T3CONbits.TCKPS = 7;
        // PR3 (( (fPBCLK / kPrescaler) / fOut ) - 1)
        PR3 = 39062;
        TMR3 = 0;
        T3CONbits.TON = 1;

        IPC3bits.T3IP = 2;
        IEC0bits.T3IE = 1;
        IFS0bits.T3IF = 0;

        EnableInterrupts();

        while (1);
        return 0;
}

void _int_(12) isr_T3(void) {
        static int flag = 0;
        if (flag == 0) putChar('.');
        flag = !flag;
        IFS0bits.T3IF = 0;
}
