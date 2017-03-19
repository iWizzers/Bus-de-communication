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




unsigned int 	frequence = 0,
				menu = MENU_PRINCIPAL;
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
			if ((c >= '0') && (c <= '9')) {			// Si le caractère est numérique (de 0 à 9)
				if (frequence < 32767) {
					frequence = frequence * 10 + ((int)c & 0x0F);

					if (frequence > 32767) {
						frequence = 32767;
						TXUART("\rVeuillez saisir une frequence (Hz) : 32767");
					} else {
						while (!(IFG2&UCA0TXIFG)) ;
						UCA0TXBUF = c;
					}
				}
			} else if ((int)c == 127) {				// Si le caractère est 'supprimer'
				if (frequence > 0) {
					frequence = frequence / 10;

					while (!(IFG2&UCA0TXIFG)) ;
					UCA0TXBUF = c;
				}
			} else if ((int)c == 13) {				// Si le caractère est 'entrée'
				DefinirVitesseRoues(frequence);
				TXUART(	"\n\nNouvelle frequence definie"
						"\n\n_______________________________\n\n\n");
				frequence = 0;
				menu = MENU_PRINCIPAL;
				DefinirReception(true);
			}
		} else {
			switch (c) {
			case 'h':	// Affichage de l'aide
				// Affichage du caractère reçu sur le terminal
				while (!(IFG2 & UCA0TXIFG)) ;
				UCA0TXBUF = c;

				TXUART(	"\n\n====== AIDE MENU PRINCIPAL ======\n"
						"[h] - Aide\n"
						"[e] - Eteindre\n"
						"[a] - Passage en mode autonome\n"
						"[p] - Modifier la vitesse maximale\n"
						"[z] - Avancer\n"
						"[q] - Tourner a gauche\n"
						"[s] - Reculer\n"
						"[d] - Tourner a droite"
						"\n\n_______________________________\n\n\n");
				break;
			case 'e':	// Exctinction de la communication
				// Affichage du caractère reçu sur le terminal
				while (!(IFG2 & UCA0TXIFG)) ;
				UCA0TXBUF = c;

				TXUART(	"\n\nExtinction de la communication"
						"\n\n_______________________________\n\n\n");

				// Désactivation de l'interruption UART
				IE2 &= ~UCA0RXIE;
				break;
			case 'a':
				// Affichage du caractère reçu sur le terminal
				while (!(IFG2 & UCA0TXIFG)) ;
				UCA0TXBUF = c;

				TXUART(	"\n\nPassage en mode autonome"
						"\n\n_______________________________\n\n\n");

				DefinirModeRobot(AUTONOME);
				break;
			case 'p':	// Réglage de la PWM
				// Affichage du caractère reçu sur le terminal
				while (!(IFG2 & UCA0TXIFG)) ;
				UCA0TXBUF = c;

				TXUART(	"\n\nModifier la vitesse maximale"
						"\n\n_______________________________\n\n\n");

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
				// Affichage du caractère reçu sur le terminal
				while (!(IFG2 & UCA0TXIFG)) ;
				UCA0TXBUF = c;

				TXUART(	"\n\nTouche non reconnue\n"
						"Saisir 'h' pour afficher l'aide");

				TXUART("\n\n_______________________________\n\n\n");
				break;
			}


			// Envoi SPI
			TXUART("\n\nSPI envoye : ");
			while (!(IFG2 & UCA0TXIFG)) ;
			UCA0TXBUF = c;

			// Réception SPI
			unsigned char var = TXSPI(c);
			TXUART("\n\nSPI recu : ");
			while (!(IFG2 & UCA0TXIFG)) ;
			UCA0TXBUF = var;


			DefinirReception(true);
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


	if (ObtenirModeRobot() == AUTONOME) {
		DefinirFrequenceCliLED(FREQUENCE_ROBOT_AUTONOME);
	} else {
		DefinirFrequenceCliLED(FREQUENCE_ROBOT_MANUEL);
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
