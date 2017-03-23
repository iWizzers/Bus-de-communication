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
#include "global.h"



BOOL 	etatCommunication 	= true;



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
	USICTL0 = USISWRST;                 			// Réglage configuration

	USICTL0 += USIPE7 + USIPE6 + USIPE5 + USIOE;	// SPI esclave

	USICTL1 = USIIE + USICKPH;                     // Interruption + Phase

	USICTL0 -= USISWRST;                 			// Libération du SPI
}



//************************************************************
// Fonction TXSPI
//
//       Entrées :
//                 UCHAR : caractère à envoyer
//
//       Sorties :
//                 NULL
//************************************************************
void TXSPI(UCHAR carac)
{
	USISRL = (SINT_32)carac << 1;	// Décalage du caractère de 1 bit vers la gauche
	USICNT = 8;				// Envoi de 8 bits
}



//************************************************************
// Fonction RXSPI
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 UCHAR : caractère reçu
//************************************************************
UCHAR RXSPI(void){
	while((USIIFG & USICTL1) != BIT0) {
		// Scrutation
	}

	return (UCHAR)USISRL;	// Retourne le caractère reçu sur le SPI
}



//************************************************************
// Fonction ArreterCommunicationSPI : Permet d'arrêter la communication SPI
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArreterCommunicationSPI(void)
{
	etatCommunication = false;

	// Désactivation de l'interruption SPI
	USICTL1 -= USIIE;

	// Désactivation du servomoteur
	P1DIR -= BIT_SERVOMOTEUR;

	// Allumage LED rouge pour signifier la fin du programme
	P1OUT = BIT_LED_ROUGE;
}



//************************************************************
// Fonction ObtenirEtatCommunicationSPI : Permet de récupérer l'état de la communication SPI
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 BOOL : retourne l'état de la communication
//************************************************************
BOOL ObtenirEtatCommunicationSPI(void)
{
	return etatCommunication;	// Retourne l'état de la communication avec l'UART
}
