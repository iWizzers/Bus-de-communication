/* ============================================================================ */
/* Description: Permet de gérer le SPI (transmission et réception)              */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */
//************************************************************
//
//                   MSP430G2553                                 MSP430G2231
//                                   _______________________________________________
//                 _______________  |                              _______________  |
//		          |               | |                             |               | |
//                |            GND|<-                             |            GND|<-
//                |               |                 Chip Select ->|P1.1           |
//  Chip Select <-|P1.4       P1.7|-> Data Out                    |           P1.7|<- Data In
//    Clock Out <-|P1.5       P1.6|<- Data In          Clock In ->|P1.5       P1.6|-> Data Out
//                |_______________|                               |_______________|
//
//************************************************************



#include <msp430g2231.h>
#include "SPI.h"



//************************************************************
// Fonction InitSPI
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitSPI(void){
	USICTL0 |= USISWRST;                 			// Réglage configuration

	USICTL0 |= USIPE7 + USIPE6 + USIPE5 + USIOE;	// SPI esclave
	USICTL1 |= USIIE | USICKPH;                     // Interruption + Phase

	USICTL0 &= ~USISWRST;                 			// Libération du SPI
}



//************************************************************
// Fonction TXSPI
//
//       Entrées :
//                 unsigned char : caractère à envoyer
//
//       Sorties :
//                 NULL
//************************************************************
void TXSPI(unsigned char carac)
{
	USISRL = carac << 1;	// Décalage du caractère de 1 bit vers la gauche
	USICNT = 8;				// Envoi de 8 bits
}



//************************************************************
// Fonction RXSPI
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 unsigned char : caractère reçu
//************************************************************
unsigned char RXSPI(void){
	while((USIIFG & USICTL1) != BIT0); // Scrutation

	return USISRL;
}
