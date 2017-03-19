/* ============================================================================ */
/* Description: Permet de g�rer les entr�es et les sorties                      */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "GPIO.h"



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
	P1DIR |= BIT0;		// LED rouge en sortie
	P1DIR |= BIT2; 		// R�glage du servomoteur en sortie
	P1DIR |= BIT6;		// SDO en sortie


	P1OUT = 0;
	P1OUT |= BIT0;		// Activation LED rouge pour visualiser l'intialisation


	P1SEL |= BIT2; 					// S�lection fonction TA0.1
	P1SEL |= BIT5 + BIT6 + BIT7;	// S�lection du SPI (clock, sortie et entr�e)
}
