#include <msp430g2231.h>
#include "GPIO.h"



void InitPort(void)
{
	P1OUT = 0x00;                        // P1.4 set, else reset
	P1DIR = BIT0;                         // P1.0 output, else input

	P1DIR &= ~(BIT1 + BIT5 + BIT7);
	P1DIR |= BIT6;

	P1SEL |= BIT5 + BIT6 + BIT7;
}
