/* ============================================================================ */
/* Description: Permet de g�rer la fonctionnalit� UART (transmission            */
/* et r�ception)                                                                */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "UART.h"
#include "global.h"
#include "Robot.h"



BOOL 	reception 			= false,	// Variable contenant l'utilisation du terminal
		etatCommunication 	= true;		// Variable contenant l'�tat de la communication avec le terminal



//************************************************************
// Fonction InitUART : Initialisation du SPI
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitUART(void)
{
    UCA0BR0 = 104;              // 1MHz, 9600
    UCA0BR1 = 0;                // 1MHz, 9600

	UCA0CTL0 = 0;
    UCA0CTL1 = UCSSEL_2;       // SMCLK

    IE2 += UCA0RXIE;            // Enable USCI_A0 RX interrupt
}



//************************************************************
// Fonction TXStringUART : Transmission d'une chaine de caract�res
//
//       Entr�es :
//                 char * : chaine de caract�res � transmettre
//
//       Sorties :
//                 NULL
//************************************************************
void TXStringUART(char * chaine)
{
	SINT_32 i,
		nbOctetsALire = 0;


	// Compte le nombre de caract�re � envoyer
	for (i = 0; chaine[i]; i++) {
		nbOctetsALire++;
	}


	for (i = 0; i < nbOctetsALire; i++) {
		while (!(IFG2 & UCA0TXIFG)) {
			// Attend que le buffer de transmission soit libre
		}
		UCA0TXBUF = (SINT_32)chaine[i];       	// Transmission du caract�re
	}
}



//************************************************************
// Fonction TXCharUART : Transmission d'une chaine de caract�res
//
//       Entr�es :
//                 unsigned char : caract�re � transmettre
//
//       Sorties :
//                 NULL
//************************************************************
void TXCharUART(UCHAR carac)
{
	while (!(IFG2 & UCA0TXIFG)) {
		// Attend que le buffer de transmission soit libre
	}
	UCA0TXBUF = (SINT_32)carac;       		// Transmission du caract�re
}



//************************************************************
// Fonction DefinirReceptionUART : D�finition de l'�tat de la r�ception
//
//       Entr�es :
//                 BOOL : permet de d�finir l'�tat de l'UART
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirReceptionUART(BOOL etat)
{
	reception = etat;	// Modification de l'utilisation du terminal
}



//************************************************************
// Fonction ObtenirReceptionUART : R�cup�ration de l'�tat de la r�ception
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 BOOL : permet de r�cup�rer l'�tat de l'UART
//************************************************************
BOOL ObtenirReceptionUART(void)
{
	return reception;		// Retourne si un caract�re a �t� re�u
}



//************************************************************
// Fonction ArreterCommunicationUART : Permet d'arr�ter la communication UART
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArreterCommunicationUART(void)
{
	etatCommunication = false;

	// D�sactivation de l'interruption UART
	IE2 -= UCA0RXIE;


	// Arr�t du robot
	while (ObtenirEtatRobot() != (UCHAR)ARRET) {
		__delay_cycles(25000);
		Stop(ARRET);
	}

	// D�sactivation de l'interruption des timers
	TA0CTL -= TAIE;
	TA1CTL -= TAIE;

	// Allumage LED rouge pour signifier la fin du programme
	P1OUT = BIT_LED_ROUGE;
}



//************************************************************
// Fonction ObtenirEtatCommunicationUART : Permet de r�cup�rer l'�tat de la communication UART
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 BOOL : retourne l'�tat de la communication
//************************************************************
BOOL ObtenirEtatCommunicationUART(void)
{
	return etatCommunication;	// Retourne l'�tat de la communication UART
}
