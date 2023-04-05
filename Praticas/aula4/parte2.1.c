#include <detpic32.h>

void main(void)
{
	// RB8-RB14, RD5 e RD6 como outputs
	TRISB = TRISB & 0x80FF;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;

	// Selecionar apenas o display menos significativo
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;

	while (1)
	{
		char read = getChar();
		/* if ( (read >= 'a' && read <= 'g') || (read >= 'A' && read <= 'G') )
		{
			if (read == 'a' || read == 'A') LATB = (LATB & 0x80FF) | 0x0100;
			else if (read == 'b' || read == 'B') LATB = (LATB & 0x80FF) | 0x0200;
			else if (read == 'c' || read == 'C') LATB = (LATB & 0x80FF) | 0x0400;
			else if (read == 'd' || read == 'D') LATB = (LATB & 0x80FF) | 0x0800;
			else if (read == 'e' || read == 'E') LATB = (LATB & 0x80FF) | 0x1000;
			else if (read == 'f' || read == 'F') LATB = (LATB & 0x80FF) | 0x2000;
			else if (read == 'g' || read == 'G') LATB = (LATB & 0x80FF) | 0x4000;
		} */

		read = read & 0x07;
		int value = 1 << (read - 1);
		LATB = (LATB & 0x80FF) | (value << 8);
	}
}
