/* ============================================================================ */
/* Description: Permet de g�rer le SPI (transmission et r�ception)              */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */
//********************************************************************************************
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
//********************************************************************************************



#include <msp430g2553.h>
#include "SPI.h"
#include "global.h"
#include "GPIO.h"



//************************************************************
// Fonction InitSPI : Initialisation du SPI
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 char * : chaine de caract�re donnant l'�tat de l'initialisation
//************************************************************
void InitSPI(void)
{
	UCB0CTL0 = UCMSB + UCMST;		// MSB en premier + SPI en maitre
	UCB0CTL1 = UCSSEL_2;				// SMCLK

	UCB0BR0 = 104;						// 1MHz, 9600
	UCB0BR1 = 0;						// 1MHz, 9600
}



//************************************************************
// Fonction TXSPI : Transmission et r�ception d'un caract�re
//
//       Entr�es :
//                 unsigned char : caract�re � envoyer
//
//       Sorties :
//                 NULL
//************************************************************
void TXSPI(UCHAR donnee)
{
	ActiverGPIOPort1((UCHAR)BIT_CS, false); 	// S�lection de l'esclave


	while (!(IFG2 & UCB0TXIFG)) {
  		// Attend que le buffer d'envoi soit libre
	}
	UCB0TXBUF = (SINT_32)donnee;              	// Transmission du caract�re
	__delay_cycles(50);					// D�lai pour l'esclave


	while (!(IFG2 & UCB0TXIFG)) {
  		// Attend que le buffer d'envoi soit libre
	}
	UCB0TXBUF = 0;              		// Transmission du caract�re NULL
	__delay_cycles(50);					// D�lai pour l'esclave
}



//************************************************************
// Fonction TXSPI : Transmission et r�ception d'un caract�re
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 unsigned char : caract�re re�u
//************************************************************
UCHAR RXSPI(void)
{
	UCHAR retour;


	while (!(IFG2 & UCB0RXIFG)) {
 		// Attend que le buffer de r�ception soit libre
	}
	retour = UCB0RXBUF;					// Lecture du caract�re NULL

	while (!(IFG2 & UCB0RXIFG)) {
 		// Attend que le buffer de r�ception soit libre
	}
	retour = (UCHAR)UCB0RXBUF;					// Lecture du caract�re � recevoir


	ActiverGPIOPort1((UCHAR)BIT_CS, true);		// Lib�ration de l'esclave


	return retour;
}



//************************************************************
// Fonction ArreterCommunicationSPI : Permet d'arr�ter la communication SPI
//
//       Entr�es :
//                 unsigned char : caract�re � transmettre
//
//       Sorties :
//                 NULL
//************************************************************
void ArreterCommunicationSPI(UCHAR caractere)
{
	TXSPI(caractere);		// Transmet le caract�re d'arr�t
}
