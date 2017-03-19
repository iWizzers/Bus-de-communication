/* ============================================================================ */
/* Description: Gestion des interruptions                                       */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "SPI.h"



//************************************************************
// Routine de service d'interruption SPI
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=USI_VECTOR
__interrupt void USI_ISR(void)
{
	unsigned char caractere = RXSPI();	// Réception du caractère

	switch (caractere) {
	case 'a':
		P1OUT |= BIT0;		// Allumage de la LED
		TXSPI('a');			// Envoi du caractère 'a'
		break;
	case 'e':
		P1OUT &= ~BIT0;		// Extinction de la LED
		TXSPI('e');			// Envoi du caractère 'e'
		break;
	default:
		TXSPI('0');			// Envoi du caractère '0' = pas d'action
		break;
	}
}
