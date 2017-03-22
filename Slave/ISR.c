/* ============================================================================ */
/* Description: Gestion des interruptions                                       */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "SPI.h"
#include "global.h"
#include "timer.h"



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
	P1OUT |= BIT_LED_ROUGE;


	unsigned char caractere = RXSPI();	// R�ception du caract�re

	switch (caractere) {
	case 'i':
		TXSPI('i');
		Init0Degre();
		break;
	case 's':
		TXSPI('s');
		Scan180Degres();
		break;
	case 'z':
		TXSPI('z');
		Init90Degres();
		break;
	case 'e':
		ArreterCommunicationSPI();
		TXSPI('e');
		break;
	default:
		TXSPI('0');			// Envoi du caract�re '0' = pas d'action
		break;
	}


	P1OUT &= ~BIT_LED_ROUGE;
}
