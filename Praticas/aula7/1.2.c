#include <detpic32.h>

volatile unsigned char voltage = 0; // Global variable

void delay(int ms)
{
	resetCoreTimer();
	while (readCoreTimer() < (20000 * ms));
}

void send2displays(unsigned char value)
{
	// static const char display7Scodes[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
	// Use the above variable (display7Scodes), but write the values in binary
	static const char display7Scodes[] = { 
		0b00111111, 
		0b00000110, 
		0b01011011, 
		0b01001111, 
		0b01100110, 
		0b01101101, 
		0b01111101, 
		0b00000111, 
		0b01111111, 
		0b01101111, 
		0b01110111, 
		0b01111100, 
		0b00111001, 
		0b01011110, 
		0b01111001, 
		0b01110001 
	};

	static char flag = 0;

	if (flag == 1) {
		// select display high
		// send digit_high (dh) to display:	dh = value >> 4;
		LATD = (LATD & 0xFF9F) | 0x0040;
		LATB = (LATB & 0x80FF) | (display7Scodes[(value >> 4)] << 8);
		flag = 0;
	} else {
		// select display low
		// send digit_low (dl) to display:	dl = value & 0x0F;
		LATD = (LATD & 0xFF9F) | 0x0020;
		LATB = (LATB & 0x80FF) | (display7Scodes[(value & 0x0F)] << 8);
		flag = 1;
	}
}

unsigned char toBcd(unsigned char value)
{
	return ((value / 10) << 4) + (value % 10);
}

int main(void)
{
	unsigned int cnt = 0;

	TRISB = TRISB & 0x80FF;
	TRISD = TRISD & 0xFF9F;	

	// Configure all (digital I/O, analog input, A/D module, interrupts)
	TRISBbits.TRISB4 = 1;		// RB4 digital output disconnected
	AD1PCFGbits.PCFG4 = 0;		// RB4 configured as analog input (AN4)
	AD1CON1bits.SSRC = 7;		// Conversion trigger selection bits: in this
								// mode an internal counter ends sampling and
								// starts conversion
	AD1CON1bits.CLRASAM = 1;	// Stop conversions when the 1st A/D converter
								// interrupt is generated. At the same time,
								// hardware clears the ASAM bit
	AD1CON3bits.SAMC = 16;		// Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 7;		// Interrupt is generated after XX samples
								// (replace XX by the desired number of
								// consecutive samples)
	AD1CHSbits.CH0SA = 4;		// replace x by the desired input
								// analog channel (0 to 15)
	AD1CON1bits.ON = 1;			// Enable A/D converter
								// This must the last command of the A/D
								// configuration sequence

	IPC6bits.AD1IP = 2;			// configure priority of A/D interrupts to 2
	IFS1bits.AD1IF = 0;			// clear A/D interrupt flag
	IEC1bits.AD1IE = 1;			// enable A/D interrupts

	// Configure interrupt system
	EnableInterrupts();

	while(1)
	{
		if (cnt == 0)		// 0, 200 ms, 400ms, ... (5 samples/second)
		{
			// Start A/D conversion
			AD1CON1bits.ASAM = 1;
		}

		// Send "voltage" value to displays
		send2displays(toBcd(voltage));

		cnt = (cnt + 1) % 2;
		delay(10);
	}
	return 0;
}

void _int_(27) isr_adc(void)
{
	// Read 8 samples (ADC1BUF0 to ADC1BUF7) and calculate the average
	int i;
	int *p = (int *)(&ADC1BUF0);
	int sum = 0;
	for (i = 0; i < 8; i++)
	{
		sum += p[i*4];
		printInt(p[i*4], 10 | 4 << 16);
		putChar(' ');
	}
	int media = sum / 8;

	putChar('-');
	putChar(' ');
	
	// Convert voltage amplitude to decimal and store the result in the global variable "voltage"
	voltage = (media * 33) / 1023;

	printInt(voltage, 10 | 4 << 16);
	putChar('\n');

	// Reset AD1IF flag
	IFS1bits.AD1IF = 0;
}
