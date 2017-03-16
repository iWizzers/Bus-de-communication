#include <msp430g2553.h>
#include "Timer.h"
#include "global.h"
#include "UART.h"



char * InitPWM(void)
{
	/*P1DIR |= BIT4; 	// Réglages des roues en sorties (sens + PWM)

	P1SEL |= (BIT4); 				// selection fonction TA1.1 & TA1.2
	P1SEL2 &= ~(BIT4); 				// selection fonction TA1.1 & TA1.2

	TA1CTL = (TASSEL_2 | MC_1 | ID_0); 		// source SMCLK pour TimerA , mode comptage Up
	TA1CCTL1 = OUTMOD_7; 					// activation mode de sortie n°7
	TA1CCR0 = 1500; 						// determine la periode du signal
	TA1CCR1 = 1500;*/


	TACTL = 0 | (TASSEL_2 | ID_0); //source SMCLK, pas de predivision ID_0
	TACTL |= MC_1; //comptage en mode up
	TACTL |= TAIE; //autorisation interruption TAIE

	DefinirFrequence(18); // Hz

	__enable_interrupt();	// PWM

	return "OK";
}




void DefinirFrequence(unsigned int valeur)
{
	TACCR0 = ((double)((double)1 / valeur) / 2) * 1000000;
}
