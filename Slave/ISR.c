/* ============================================================================ */
/* Description: Gestion des interruptions                                       */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "SPI.h"



//************************************************************
// Routine de service d'interruption SPI
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=USI_VECTOR
__interrupt void USI_ISR(void)
{
	unsigned char caractere = RXSPI();	// R�ception du caract�re

	switch (caractere) {
	case 'a':
		P1OUT |= BIT0;		// Allumage de la LED
		TXSPI('a');			// Envoi du caract�re 'a'
		break;
	case 'e':
		P1OUT &= ~BIT0;		// Extinction de la LED
		TXSPI('e');			// Envoi du caract�re 'e'
		break;
	default:
		TXSPI('0');			// Envoi du caract�re '0' = pas d'action
		break;
	}
}
