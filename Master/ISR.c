/* ============================================================================ */
/* Description: Gestion des interruptions                                       */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "global.h"
#include "UART.h"
#include "SPI.h"
#include "GPIO.h"
#include "Timer.h"
#include "Robot.h"




unsigned int 	menu = MENU_PRINCIPAL;	// Variable contenant le type de menu
unsigned char 	nbIncrementRoueA = 0,	// Variable contenant le nombre d'incr�ment du codeur optique de la roue A
				nbIncrementRoueB = 0;	// Variable contenant le nombre d'incr�ment du codeur optique de la roue B



//************************************************************
// Routine de service d'interruption UART
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    unsigned char c = UCA0RXBUF;	// R�ception du caract�re sur l'UART


    if (ObtenirModeRobot() == AUTONOME) {
    	DefinirModeRobot(MANUEL);
    } else {
    	if (menu == MENU_PARAMETRE_FREQ_CLI_LED) {	// Si nous modifions la PWM
    		if (ModifierFrequenceRoues(c) == true) {
    			menu = MENU_PRINCIPAL;
				DefinirReceptionUART(true);
    		} else {
    			// Ne fait rien
    		}
		} else {									// Sinon
			switch (c) {
			case 'h':	// Affichage de l'aide
				TXCharUART(c);	// Affichage du caract�re re�u sur le terminal

				TXStringUART(	"\n\n====== AIDE MENU PRINCIPAL ======\n"
								"[h] - Aide\n"
								"[e] - Eteindre\n"
								"[a] - Passage en mode autonome\n"
								"[p] - Modifier la vitesse maximale\n"
								"[z] - Avancer\n"
								"[q] - Tourner a gauche\n"
								"[s] - Reculer\n"
								"[d] - Tourner a droite"
								"\n\n_______________________________\n\n\n");

				DefinirReceptionUART(true);
				break;
			case 'e':	// Exctinction de la communication
				TXCharUART(c);	// Affichage du caract�re re�u sur le terminal

				TXStringUART(	"\n\nExtinction de la communication"
								"\n\n_______________________________\n\n\n");

				ArreterCommunicationUART();
				ArreterCommunicationSPI(c);

				DefinirReceptionUART(true);
				break;
			case 'a':
				TXCharUART(c);	// Affichage du caract�re re�u sur le terminal

				TXStringUART(	"\n\nPassage en mode autonome"
								"\n\n_______________________________\n\n\n");

				DefinirModeRobot(AUTONOME);

				DefinirReceptionUART(true);
				break;
			case 'p':	// R�glage de la PWM
				TXCharUART(c);	// Affichage du caract�re re�u sur le terminal

				TXStringUART(	"\n\nModifier la vitesse maximale"
								"\n\nVeuillez saisir une frequence (Hz) : ");

				menu = MENU_PARAMETRE_FREQ_CLI_LED;
				break;
			case 'z':
				Avancer();
				break;
			case 'q':
				TournerGauche();
				break;
			case 's':
				Reculer();
				break;
			case 'd':
				TournerDroite();
				break;
			default:	// Touche inconnue
				TXCharUART(c);	// Affichage du caract�re re�u sur le terminal

				TXStringUART(	"\n\nTouche non reconnue\n"
								"Saisir 'h' pour afficher l'aide"
								"\n\n_______________________________\n\n\n");


				DefinirReceptionUART(true);
				break;
			}
		}
    }
}



//************************************************************
// Routine de service d'interruption du timer 0 pour afficher l'activit� du robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0Interrupt(void)
{
	if (ObtenirEtatGPIOPort1(BIT_LED_ROUGE) == true) {	// Si la LED rouge est allum�e
		ActiverGPIOPort1(BIT_LED_ROUGE, false);			// ... on l'�teint
	} else {											// Sinon
		ActiverGPIOPort1(BIT_LED_ROUGE, true);			// ... on l'allume
	}


	TA0CTL &= ~TAIFG; //RAZ TAIFG
}



//************************************************************
// Routine de service d'interruption du timer 1 des optocoupleurs des roues
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1Interrupt(void)
{
	if ((nbIncrementRoueA == 15) || (nbIncrementRoueB == 15)) {		// Si le nombre d'incr�ments des codeurs optiques sont �gales � 1 tour de roue (15 incr�ments)
		CorrigerErreurRoues(nbIncrementRoueA, nbIncrementRoueB);	// ... on corrige l'erreur

		nbIncrementRoueA = nbIncrementRoueB = 0;					// ... RAZ des variables des codeurs optiques
	} else {														// Sinon
		if (P2IN & BIT_OPTO_COUPLEUR_ROUE_A) {						// Si l'entr�e du codeur optique de la roue A est � '1'
			nbIncrementRoueA++;										// ... on incr�mente sa variable
		} else {
			// Ne fait rien
		}

		if (P2IN & BIT_OPTO_COUPLEUR_ROUE_B) {						// Si l'entr�e du codeur optique de la roue A est � '1'
			nbIncrementRoueB++;										// ... on incr�mente sa variable
		} else {
			// Ne fait rien
		}
	}


	TA1CTL &= ~TAIFG; //RAZ TAIFG
}
