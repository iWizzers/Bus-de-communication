/* ============================================================================ */
/* Description: Permet de gérer la fonctionnalité UART (transmission            */
/* et réception)                                                                */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "UART.h"
#include "global.h"
#include "Robot.h"



BOOL 	reception 			= false,	// Variable contenant l'utilisation du terminal
		etatCommunication 	= true;		// Variable contenant l'état de la communication avec le terminal



//************************************************************
// Fonction InitUART : Initialisation du SPI
//
//       Entrées :
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
// Fonction TXStringUART : Transmission d'une chaine de caractères
//
//       Entrées :
//                 char * : chaine de caractères à transmettre
//
//       Sorties :
//                 NULL
//************************************************************
void TXStringUART(char * chaine)
{
	SINT_32 i,
		nbOctetsALire = 0;


	// Compte le nombre de caractère à envoyer
	for (i = 0; chaine[i]; i++) {
		nbOctetsALire++;
	}


	for (i = 0; i < nbOctetsALire; i++) {
		while (!(IFG2 & UCA0TXIFG)) {
			// Attend que le buffer de transmission soit libre
		}
		UCA0TXBUF = (SINT_32)chaine[i];       	// Transmission du caractère
	}
}



//************************************************************
// Fonction TXCharUART : Transmission d'une chaine de caractères
//
//       Entrées :
//                 unsigned char : caractère à transmettre
//
//       Sorties :
//                 NULL
//************************************************************
void TXCharUART(UCHAR carac)
{
	while (!(IFG2 & UCA0TXIFG)) {
		// Attend que le buffer de transmission soit libre
	}
	UCA0TXBUF = (SINT_32)carac;       		// Transmission du caractère
}



//************************************************************
// Fonction DefinirReceptionUART : Définition de l'état de la réception
//
//       Entrées :
//                 BOOL : permet de définir l'état de l'UART
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirReceptionUART(BOOL etat)
{
	reception = etat;	// Modification de l'utilisation du terminal
}



//************************************************************
// Fonction ObtenirReceptionUART : Récupération de l'état de la réception
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 BOOL : permet de récupérer l'état de l'UART
//************************************************************
BOOL ObtenirReceptionUART(void)
{
	return reception;		// Retourne si un caractère a été reçu
}



//************************************************************
// Fonction ArreterCommunicationUART : Permet d'arrêter la communication UART
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArreterCommunicationUART(void)
{
	etatCommunication = false;

	// Désactivation de l'interruption UART
	IE2 -= UCA0RXIE;


	// Arrêt du robot
	while (ObtenirEtatRobot() != (UCHAR)ARRET) {
		__delay_cycles(25000);
		Stop(ARRET);
	}

	// Désactivation de l'interruption des timers
	TA0CTL -= TAIE;
	TA1CTL -= TAIE;

	// Allumage LED rouge pour signifier la fin du programme
	P1OUT = BIT_LED_ROUGE;
}



//************************************************************
// Fonction ObtenirEtatCommunicationUART : Permet de récupérer l'état de la communication UART
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 BOOL : retourne l'état de la communication
//************************************************************
BOOL ObtenirEtatCommunicationUART(void)
{
	return etatCommunication;	// Retourne l'état de la communication UART
}
