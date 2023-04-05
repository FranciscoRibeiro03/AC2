#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
	// Configure port RE6-RE3 as output
	TRISEbits.TRISE6 = 0;
	TRISEbits.TRISE5 = 0;
	TRISEbits.TRISE4 = 0;
	TRISEbits.TRISE3 = 0;

	int count = 0;

	while (1)
	{
		LATE = ( LATE & 0xFF87 ) | ( count << 3 );
				
		delay(250);

		count = (count + 1) % 10;
	}
	return 0;
}
