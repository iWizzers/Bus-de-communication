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




unsigned int 	menu = MENU_PRINCIPAL;
unsigned char 	nbIncrementRoueA = 0,
				nbIncrementRoueB = 0;



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
    unsigned char c = UCA0RXBUF;	// Réception du caractère sur l'UART


    if (ObtenirModeRobot() == AUTONOME) {
    	DefinirModeRobot(MANUEL);
    } else {
    	if (menu == MENU_PARAMETRE_FREQ_CLI_LED) {	// Si nous modifions la PWM
    		if (ModifierFrequenceRoues(c) == true) {
    			menu = MENU_PRINCIPAL;
				DefinirReceptionUART(true);
    		}
		} else {									// Sinon
			switch (c) {
			case 'h':	// Affichage de l'aide
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
			case 'e':	// Exctinction de la communication
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

				TXStringUART(	"\n\nExtinction de la communication"
								"\n\n_______________________________\n\n\n");

				ArreterCommunicationUART();
				ArreterCommunicationSPI(c);

				DefinirReceptionUART(true);
				break;
			case 'a':
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

				TXStringUART(	"\n\nPassage en mode autonome"
								"\n\n_______________________________\n\n\n");

				DefinirModeRobot(AUTONOME);

				DefinirReceptionUART(true);
				break;
			case 'p':	// Réglage de la PWM
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

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
				TXCharUART(c);	// Affichage du caractère reçu sur le terminal

				TXStringUART(	"\n\nTouche non reconnue\n"
								"Saisir 'h' pour afficher l'aide"
								"\n\n_______________________________\n\n\n");


				// Envoi SPI
				TXStringUART("\n\nSPI envoye : ");
				TXCharUART(c);

				// Réception SPI
				unsigned char var = TXSPI(c);
				TXStringUART("\n\nSPI recu : ");
				TXCharUART(var);
				TXStringUART("\n\n");


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
	if (ObtenirEtatGPIOPort1(BIT_LED_ROUGE) == true) {
		ActiverGPIOPort1(BIT_LED_ROUGE, false);
	} else {
		ActiverGPIOPort1(BIT_LED_ROUGE, true);
	}


	TA0CTL &= ~TAIFG; //RAZ TAIFG
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
	/*if ((nbIncrementRoueA == 15) || (nbIncrementRoueB == 15)) {
		if (nbIncrementRoueA > nbIncrementRoueB)
			TA1CCR1 += nbIncrementRoueA - nbIncrementRoueB;
		else if (nbIncrementRoueA < nbIncrementRoueB)
			TA1CCR2 += nbIncrementRoueB - nbIncrementRoueA;

		nbIncrementRoueA = nbIncrementRoueB = 0;
	} else {
		if (P2IN & BIT_OPTO_COUPLEUR_ROUE_A)
			nbIncrementRoueA++;

		if (P2IN & BIT_OPTO_COUPLEUR_ROUE_B)
			nbIncrementRoueB++;
	}*/

	TA1CTL &= ~TAIFG; //RAZ TAIFG
}
