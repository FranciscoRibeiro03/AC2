#include <detpic32.h>

int main(void) {
        // Configuração do Timer 3 para eventos a 2 Hz
        // Constante do Prescaler ( fPBCLK / ((65535 + 1) * fOut) )
        T3CONbits.TCKPS = 7;
        // PR3 (( (fPBCLK / kPrescaler) / fOut ) - 1)
        PR3 = 39062;
        TMR3 = 0;
        T3CONbits.TON = 1;

        while (1) {
                while (IFS0bits.T3IF == 0);
                IFS0bits.T3IF = 0;
                putChar('.');
        }
        return 0;
}
