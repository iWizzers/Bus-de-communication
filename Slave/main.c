/* ============================================================================ */
/* Description: A l'aide du MSP430G2231, il est possible d'effectuer une        */
/* communication SPI. De plus, un servomoteur effectue une rotation de 180°     */
/* afin qu'un capteur puisse détecter un obstacle. La rotation s'effectue à la  */
/* demande du MSP430G2553.                                                      */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "GPIO.h"
#include "SPI.h"
#include "Timer.h"



//************************************************************
// Fonction main
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stoppe le watchdog


	// INITIALISATION
	InitGPIO();	// Entrées-sorties
	InitSPI();	// SPI en esclave
	InitPWM();	// PWM du servomoteur


	// Exctinction de la LED rouge si l'initialisation est un succès
	P1OUT -= BIT0;


	// Activition de l'interruption SPI
	__enable_interrupt();


	while (ObtenirEtatCommunicationSPI() == true) {
		// Patiente tant que la communication avec l'UART
		// est effective. Durant ce temps, il fonctionne
		// en interruption pour recevoir et transmettre
		// via la communication SPI. Il gère donc le
		// servomoteur.
	}


	return 0;
}
