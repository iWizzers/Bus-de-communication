/* ============================================================================ */
/* Description: Permet de gérer les entrées et les sorties                      */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "GPIO.h"
#include "global.h"



//************************************************************
// Fonction InitGPIO
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitGPIO(void)
{
	P1DIR = 0;
	P1DIR += BIT_LED_ROUGE;		// LED rouge en sortie
	P1DIR += BIT_SERVOMOTEUR; 	// Réglage du servomoteur en sortie
	P1DIR += BIT_SDO_SPI;		// SDO en sortie


	P1OUT = 0;
	P1OUT += BIT_LED_ROUGE;		// Activation LED rouge pour visualiser l'intialisation


	P1SEL = 0;
	P1SEL += BIT_SERVOMOTEUR; 							// Sélection fonction TA0.1
	P1SEL += BIT_CLK_SPI + BIT_SDO_SPI + BIT_SDI_SPI;	// Sélection du SPI (clock, sortie et entrée)
}
