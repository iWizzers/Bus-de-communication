#include <msp430g2553.h>
#include "GPIO.h"
#include "global.h"



char * InitGPIO(void)
{
	P1DIR = LED_ROUGE;
	P1OUT = LED_ROUGE;


	return "OK";
}



void ActiverGPIO(unsigned char bit, BOOL etat)
{
	if (etat == true)
	{
		P1OUT |= bit;
	}
	else
	{
		P1OUT &= ~bit;
	}
}



BOOL ObtenirEtatGPIO(unsigned char bit)
{
	BOOL ret;

	if (P1OUT & bit)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}

	return ret;
}
