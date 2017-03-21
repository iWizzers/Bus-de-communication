/* ============================================================================ */
/* Description: Gestion des interruptions                                       */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "SPI.h"
#include "global.h"



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
	case 'j':
		if (P1OUT & BIT_LED_ROUGE)
			P1OUT &= ~BIT_LED_ROUGE;		// Allumage de la LED
		else
			P1OUT |= BIT_LED_ROUGE;
		TXSPI('a');			// Envoi du caractère 'a'
		break;
	case 'e':
		ArreterCommunicationSPI();
		break;
	default:
		TXSPI('0');			// Envoi du caractère '0' = pas d'action
		break;
	}
}
