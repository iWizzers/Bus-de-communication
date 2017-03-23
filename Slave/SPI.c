/* ============================================================================ */
/* Description: Permet de g�rer le SPI (transmission et r�ception)              */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
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
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitSPI(void){
	USICTL0 = USISWRST;                 			// R�glage configuration

	USICTL0 += USIPE7 + USIPE6 + USIPE5 + USIOE;	// SPI esclave

	USICTL1 = USIIE + USICKPH;                     // Interruption + Phase

	USICTL0 -= USISWRST;                 			// Lib�ration du SPI
}



//************************************************************
// Fonction TXSPI
//
//       Entr�es :
//                 UCHAR : caract�re � envoyer
//
//       Sorties :
//                 NULL
//************************************************************
void TXSPI(UCHAR carac)
{
	USISRL = (SINT_32)carac << 1;	// D�calage du caract�re de 1 bit vers la gauche
	USICNT = 8;				// Envoi de 8 bits
}



//************************************************************
// Fonction RXSPI
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 UCHAR : caract�re re�u
//************************************************************
UCHAR RXSPI(void){
	while((USIIFG & USICTL1) != BIT0) {
		// Scrutation
	}

	return (UCHAR)USISRL;	// Retourne le caract�re re�u sur le SPI
}



//************************************************************
// Fonction ArreterCommunicationSPI : Permet d'arr�ter la communication SPI
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArreterCommunicationSPI(void)
{
	etatCommunication = false;

	// D�sactivation de l'interruption SPI
	USICTL1 -= USIIE;

	// D�sactivation du servomoteur
	P1DIR -= BIT_SERVOMOTEUR;

	// Allumage LED rouge pour signifier la fin du programme
	P1OUT = BIT_LED_ROUGE;
}



//************************************************************
// Fonction ObtenirEtatCommunicationSPI : Permet de r�cup�rer l'�tat de la communication SPI
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 BOOL : retourne l'�tat de la communication
//************************************************************
BOOL ObtenirEtatCommunicationSPI(void)
{
	return etatCommunication;	// Retourne l'�tat de la communication avec l'UART
}
