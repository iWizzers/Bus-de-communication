/* ============================================================================ */
/* Description: Gestion des interruptions                                       */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "SPI.h"
#include "global.h"



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
	case 'j':
		if (P1OUT & BIT_LED_ROUGE)
			P1OUT &= ~BIT_LED_ROUGE;		// Allumage de la LED
		else
			P1OUT |= BIT_LED_ROUGE;
		TXSPI('a');			// Envoi du caract�re 'a'
		break;
	case 'e':
		ArreterCommunicationSPI();
		break;
	default:
		TXSPI('0');			// Envoi du caract�re '0' = pas d'action
		break;
	}
}
