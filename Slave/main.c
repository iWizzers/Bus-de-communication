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
	while (1) {
		__delay_cycles(1000000); // Delai 1 seconde

		/* Passage de X° à 0°
		 *          ______________
		 *         |   ________   |
		 *         |  |        |  |
		 *         X  | Servo  |  V -> 0°
		 *            |________|
		 */
		TACCR1 = 500;

		__delay_cycles(1000000); // Delai 1 seconde

		/* Passage de X° à 180°
		 *          ______________
		 *         |   ________   |
		 *         |  |        |  |
		 * 180° <- V  | Servo  |  X
		 *            |________|
		 */
		TACCR1 = 2500;	// 180°
	}

	return 0;
}
