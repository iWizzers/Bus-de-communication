/* ============================================================================ */
/* Description: Gestion des interruptions                                       */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "global.h"
#include "UART.h"
#include "SPI.h"
#include "GPIO.h"
#include "Timer.h"
#include "Robot.h"




SINT_32 	menu = MENU_PRINCIPAL;	// Variable contenant le type de menu



//************************************************************
// Routine de service d'interruption UART
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    UCHAR c = UCA0RXBUF;	// Réception du caractère sur l'UART


    if (ObtenirModeRobot() == (UCHAR)AUTONOME) {
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
			case (UCHAR)'h':	// Affichage de l'aide
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

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
			case (UCHAR)'e':	// Exctinction de la communication
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

				TXStringUART(	"\n\nExtinction de la communication"
								"\n\n_______________________________\n\n\n");

				ArreterCommunicationUART();
				ArreterCommunicationSPI(c);

				DefinirReceptionUART(true);
				break;
			case (UCHAR)'a':
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

				TXStringUART(	"\n\nPassage en mode autonome"
								"\n\n_______________________________\n\n\n");

				DefinirModeRobot(AUTONOME);

				DefinirReceptionUART(true);
				break;
			case (UCHAR)'p':	// Réglage de la PWM
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

				TXStringUART(	"\n\nModifier la vitesse maximale"
								"\n\nVeuillez saisir une frequence (Hz) : ");

				menu = MENU_PARAMETRE_FREQ_CLI_LED;
				break;
			case (UCHAR)'z':
				Avancer();
				break;
			case (UCHAR)'q':
				TournerGauche();
				break;
			case (UCHAR)'s':
				Reculer();
				break;
			case (UCHAR)'d':
				TournerDroite();
				break;
			default:	// Touche inconnue
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

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
// Routine de service d'interruption du timer 0 pour afficher l'activité du robot
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0Interrupt(void)
{
	if (ObtenirEtatGPIOPort1((UCHAR)BIT_LED_ROUGE) == true) {	// Si la LED rouge est allumée
		ActiverGPIOPort1((UCHAR)BIT_LED_ROUGE, false);			// ... on l'éteint
	} else {											// Sinon
		ActiverGPIOPort1((UCHAR)BIT_LED_ROUGE, true);			// ... on l'allume
	}


	TA0CTL -= TAIFG; //RAZ TAIFG
}



//************************************************************
// Routine de service d'interruption du timer 1 des optocoupleurs des roues
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1Interrupt(void)
{
	TA1CTL -= TAIFG;		// RAZ TAIFG
}
