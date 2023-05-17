#include <detpic32.h>

#define VECTOR_ADC 27
#define VECTOR_UART2 32
#define VECTOR_TIMER1 4
#define VECTOR_TIMER3 12

int voltageConversion(int VAL_AD)
{
        return (VAL_AD * 33 + 511) / 1023;
}

unsigned char toBcd(unsigned char value)
{
        return ((value / 10) << 4) + (value % 10);
}

void delay(int ms)
{
        resetCoreTimer();
        while (readCoreTimer() < ms * 20000)
                ;
}

void send2displays(unsigned char value)
{
        static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

        static char displayFlag = 0;

        unsigned char dh = value >> 4;   // Get the index of the decimal part
        unsigned char dl = value & 0x0F; // Get the index of the unitary part

        // Get the correct hex code for the number
        dh = display7Scodes[dh];
        dl = display7Scodes[dl];

        if (displayFlag == 0)
        {
                LATD = (LATD | 0x0040) & 0xFFDF;                    // Dipslay High active and Display Low OFF
                LATB = (LATB & 0x80FF) | ((unsigned int)(dh)) << 8; // Clean the display and set the right value
        }
        else
        {
                LATD = (LATD | 0x0020) & 0xFFBF;                    // Display High OFF and Display High active
                LATB = (LATB & 0x80FF) | ((unsigned int)(dl)) << 8; // Clean the display and set the right value
        }

        displayFlag = !displayFlag;
}

volatile int voltage = 0;

int main(void)
{
        TRISBbits.TRISB4 = 1;    // RB4 digital output disconnected
        AD1PCFGbits.PCFG4 = 0;   // RB4 configured as analog input (AN4)
        AD1CON1bits.SSRC = 7;    // Conversion trigger constant
        AD1CON1bits.CLRASAM = 1; // Stop conversion when the 1st A/D converter intetupr is generated.
                                 // At the same time, hardware clears ASAM bit
        AD1CON3bits.SAMC = 16;   // Sample time is 16 TAD (TAD = 100ns)
        AD1CON2bits.SMPI = 7;    // Interrupt is generated after 8 samples
        AD1CHSbits.CH0SA = 4;    // analog channel input 4
        AD1CON1bits.ON = 1;      // Enable the A/d configuration sequence

        IPC6bits.AD1IP = 2; // configure priority of A/D interrupts to 2
        IFS1bits.AD1IF = 0; // clear A/D interrupt flag
        IEC1bits.AD1IE = 1; // enable A/D interrupts

        TRISB = TRISB & 0x80FF; // RB14 to RB8 as output
        TRISD = TRISD & 0xFF9F; // Displays high/low as output

        // Configure UART2: 115200, N, 8, 1
        U2BRG = ((PBCLK + 8 * 115200) / (16 * 115200)) - 1;
        U2MODEbits.BRGH = 0;
        U2MODEbits.PDSEL = 0; // 8 bits sem paridade
        U2MODEbits.STSEL = 0; // 1 stop bit
        U2STAbits.UTXEN = 1;  // Enable do módulo de transmissão
        U2STAbits.URXEN = 1;  // Enable do módulo de receção
        U2MODEbits.ON = 1;

        // Configure UART2 interrupts, with RX interruprs enabled and TX interrupts disabled
        IEC1bits.U2RXIE = 1;
        IEC1bits.U2TXIE = 0;
        IPC8bits.U2IP = 1;
        IFS1bits.U2RXIF = 0;
        IFS1bits.U2TXIF = 0;
        U2STAbits.URXISEL = 0;
        U2STAbits.UTXISEL = 0;

        T1CONbits.TCKPS = 2;
        PR1 = 62499;
        TMR1 = 0;
        T1CONbits.TON = 1;

        T3CONbits.TCKPS = 2;
        PR3 = 49999;
        TMR3 = 0;
        T3CONbits.TON = 1;

        IPC1bits.T1IP = 2;
        IEC0bits.T1IE = 1;
        IFS0bits.T1IF = 0;

        IPC3bits.T3IP = 2;
        IEC0bits.T3IE = 1;
        IFS0bits.T3IF = 0;

        EnableInterrupts();

        while (1)
                ;

        return 0;
}

void _int_(VECTOR_TIMER1) isr_T1(void)
{
        AD1CON1bits.ASAM = 1;
        IFS0bits.T1IF = 0;
}

void _int_(VECTOR_TIMER3) isr_T3(void)
{
        send2displays(toBcd(voltage));
        IFS0bits.T3IF = 0;
}

void _int_(VECTOR_ADC) isr_adc(void)
{
        int *p = (int *)(&ADC1BUF0);
        int i, sum = 0;
        for (i = 0; i < 8; i++)
        {
                sum += p[i * 4];
        }
        voltage = voltageConversion(sum) / 8;
        IFS1bits.AD1IF = 0;
}

void _int_(VECTOR_UART2) isr_uart2(void) {
        if (IFS1bits.U2RXIF == 1) {
                char c;
                c = U2RXREG;
                putc(c);
                if (c == 't') LATCbits.LATC14 = 0;
                else if (c == 'T') LATCbits.LATC14 = 1;
                IFS1bits.U2RXIF = 0;
        }
}
