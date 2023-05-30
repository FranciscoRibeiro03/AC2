#include <detpic32.h>

#define ADCsamples 4

#define VECTOR_ADC 27
#define VECTOR_T2 8

volatile int valueForDisplays;
volatile int t2count;

void send2displays(unsigned int value) {
        const char codes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

        static int flag = 0;

        if (flag) {
                LATB = (LATB & 0x80FF) | (codes[value % 10] << 8);
                LATD = (LATD & 0xFF9F) | 0x0020;
        } else {
                LATB = (LATB & 0x80FF) | (codes[value / 10] << 8);
                LATD = (LATD & 0xFF9F) | 0x0040;
        }

        flag = !flag;
}

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

int main() {
        TRISE = TRISE & 0xFF00; // xxxx xxxx 0000 0000
        TRISB = TRISB & 0x80FF; // x000 0000 xxxx xxxx
        TRISD = TRISD & 0xFF9F; // xxxx xxxx x00x xxxx

        TRISBbits.TRISB4 = 1;
        AD1PCFGbits.PCFG4 = 0;
        AD1CON1bits.SSRC = 7;
        AD1CON1bits.CLRASAM = 1;
        AD1CON3bits.SAMC = 16;
        AD1CON2bits.SMPI = ADCsamples - 1;
        AD1CHSbits.CH0SA = 4;
        AD1CON1bits.ON = 1;

        IPC6bits.AD1IP = 2;
        IFS1bits.AD1IF = 0;
        IEC1bits.AD1IE = 1;

        T2CONbits.TCKPS = 2;
        PR2 = 49999;
        TMR2 = 0;
        T2CONbits.TON = 1;

        IPC2bits.T2IP = 2;
        IEC0bits.T2IE = 1;
        IFS0bits.T2IF = 0;

        EnableInterrupts();

        t2count = 0;
        valueForDisplays = 0;
        AD1CON1bits.ASAM = 1;

        while (1);

        return 0;
}

void _int_(VECTOR_ADC) isr_adr(void) {
        int i, avg = 0;
        int *p = (int *)(&ADC1BUF0);
        for (i = 0; i < ADCsamples; i++) {
                avg += p[i * 4];
        }
        avg = avg / ADCsamples;

        valueForDisplays = ((avg * 15 + 511) / 1023) + 1;
        int valueForLeds = ((avg * 7 + 511) / 1023) + 1;

        int leds = 0;
        for (i = 0; i < valueForLeds; i++) {
                leds = (leds << 1) | 0x1;
        }
        LATE = (LATE & 0xFF00) | leds;

        IFS1bits.AD1IF = 0;
}

void _int_(VECTOR_T2) isr_t2(void) {
        send2displays(valueForDisplays);
        if (t2count == 0) 
                AD1CON1bits.ASAM = 1;
        t2count = (t2count + 1) % 10;
        IFS0bits.T2IF = 0;
}
