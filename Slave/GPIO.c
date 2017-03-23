/* ============================================================================ */
/* Description: Permet de g�rer les entr�es et les sorties                      */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "GPIO.h"
#include "global.h"



//************************************************************
// Fonction InitGPIO
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitGPIO(void)
{
	P1DIR = 0;
	P1DIR += BIT_LED_ROUGE;		// LED rouge en sortie
	P1DIR += BIT_SERVOMOTEUR; 	// R�glage du servomoteur en sortie
	P1DIR += BIT_SDO_SPI;		// SDO en sortie


	P1OUT = 0;
	P1OUT += BIT_LED_ROUGE;		// Activation LED rouge pour visualiser l'intialisation


	P1SEL = 0;
	P1SEL += BIT_SERVOMOTEUR; 							// S�lection fonction TA0.1
	P1SEL += BIT_CLK_SPI + BIT_SDO_SPI + BIT_SDI_SPI;	// S�lection du SPI (clock, sortie et entr�e)
}
