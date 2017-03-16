#include <msp430g2231.h>
#include "GPIO.h"
#include "SPI.h"



int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer


	unsigned char receive;

	InitPort();
	InitSPI();

	//__bis_SR_register(GIE);   // Enter LPM0 w/ interrupt

	while (1)
	{
		receive = listen_SPI();// Scrutation
		switch(receive)
		{
		case 'a':
			P1OUT |= BIT0;
			Send_char_SPI('a');
			break;
		case 'e':
			P1OUT &= ~BIT0;
			Send_char_SPI('e');
			break;
		default:
			Send_char_SPI('b');
			break;
		}
	}
}



/*#pragma vector=USI_VECTOR
__interrupt void universal_serial_interface(void)
{

}
*/
