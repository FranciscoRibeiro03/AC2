#include <detpic32.h>

int main()
{
	volatile int aux;

	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 0;
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;

	TRISDbits.TRISD11 = 0;

	while (1)
	{
		// Start conversion
		AD1CON1bits.ASAM = 1;
		// Set LATD11 (LATD11 = 1)
		LATDbits.LATD11 = 1;
		// Wait while conversion not done (AD1IF == 0)
		while (IFS1bits.AD1IF == 0);
		// Reset LATD11 (LATD11 = 0)
		LATDbits.LATD11 = 0;
		// Read conversion result (ADC1BUF0 value) to "aux" variable
		aux = ADC1BUF0;
		// Reset AD1IF
		IFS1bits.AD1IF = 0;
	}

	return 0;

}
