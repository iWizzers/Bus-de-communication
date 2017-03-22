/* ============================================================================ */
/* Description: A l'aide du MSP430G2231, il est possible d'effectuer une        */
/* communication SPI. De plus, un servomoteur effectue une rotation de 180°     */
/* afin qu'un capteur puisse détecter un obstacle.                              */
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
	P1OUT &= ~BIT0;


	// Activition de l'interruption SPI
	__enable_interrupt();


	// Gestion du servomoteur
	while (ObtenirEtatCommunicationSPI() == true) ;


	return 0;
}
