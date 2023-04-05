#include <detpic32.h>

int main()
{

	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 0;
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;

	while (1)
	{
		// Start conversion
		AD1CON1bits.ASAM = 1;
		// Wait while conversion not done (AD1IF == 0)
		while (IFS1bits.AD1IF == 0);
		// Read conversion result (ADC1BUF0 value) and print it

		int *p = (int *)(&ADC1BUF0);
		for (; p <= (int *)(&ADC1BUFF); p += 4) {
			printInt(*p, 10 | 4 << 16);
			putChar(' ');
		}
		putChar('\n');

		// Reset AD1IF
		IFS1bits.AD1IF = 0;
	}

}
