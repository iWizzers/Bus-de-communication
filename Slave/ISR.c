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
	P1OUT += BIT_LED_ROUGE;			// Allumage de la LED rouge afin de visualiser la communication SPI


	UCHAR caractere = RXSPI();		// R�ception du caract�re

	switch (caractere) {
	case (UCHAR)'i':				// Caract�re initialisation du scan
		TXSPI((UCHAR)'i');		// ... transmission du m�me caract�re pour valider la fonction
		Init0Degre();			// ... initialisation du scan (servomoteur � 0�)
		break;
	case (UCHAR)'s':				// Caract�re pour effectuer la rotation du servomoteur
		TXSPI((UCHAR)'s');		// ... transmission du m�me caract�re pour valider la fonction
		Scan180Degres();			// ... effectue une rotation de 45� afin de d�placer le capteur IR
		break;
	case (UCHAR)'z':				// Caract�re pour initialiser le servomteur pour que le capteur regarde devant le robot
		TXSPI((UCHAR)'z');		// ... transmission du m�me caract�re pour valider la fonction
		Init90Degres();			// ... initialisation du servomoteur pour obtenir le capteur IR face au robot
		break;
	case (UCHAR)'e':				// Caract�re pour l'extinction de la communication SPI
		ArreterCommunicationSPI();	// ... coupe la communication SPI
		TXSPI((UCHAR)'e');		// ... transmission du m�me caract�re pour valider la fonction
		break;
	default:
		TXSPI((UCHAR)'0');		// Envoi du caract�re '0' = pas d'action
		break;
	}


	P1OUT -= BIT_LED_ROUGE;		// Extinction de la LED rouge afin de terminer la communication SPI
}
