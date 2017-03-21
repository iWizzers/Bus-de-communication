/* ============================================================================ */
/* Description: Permet de gérer la PWM du servomoteur                           */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "Timer.h"
#include "global.h"
#include "UART.h"



unsigned int 		frequenceCliLED;
unsigned long int	frequenceRoues;




//************************************************************
// Fonction InitPWM : Initialisation des PWM
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 char * : chaine de caractère donnant l'état de l'initialisation
//************************************************************
char * InitPWM(void)
{
	// Timer 0
	TA0CTL = TASSEL_2 | MC_3 | ID_2; 		// Source SMCLK pour TimerA, mode comptage UP/DOWN, prédivision de 4

	DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT); 	// Détermine la periode du signal

	TA0CTL |= TAIE;							// Activation interruption


	// Timer 1
	TA1CTL = TASSEL_2 | MC_1 | ID_0; 		// Source SMCLK pour TimerA, mode comptage Up, pas de prédivision

	TA1CCTL1 = OUTMOD_7; 					// Activation mode de sortie n°7
	TA1CCTL2 = OUTMOD_7; 					// Activation mode de sortie n°7

	DefinirVitesseRoues(250);				// Détermine la periode du signal
	TA1CCR1 = 0;							// Initialisation de la roue A
	TA1CCR2 = 0;							// Initialisation de la roue B

	TA1CTL |= TAIE;							// Activation interruption


	return "OK";
}



//************************************************************
// Fonction DefinirFrequenceCliLED : Permet de définir la fréquence de clignotement de la LED rouge
//
//       Entrées :
//                 unsigned int : défini la période du signal de la LED rouge
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirFrequenceCliLED(unsigned int frequence)
{
	frequenceCliLED = frequence;

	TA0CCR0 = ((double)1 / frequence) / ((double)1 / 1000000 * 4 * 2);
}



//************************************************************
// Fonction ObtenirFrequenceCliLED : Permet de récupérer la frequence de clignotement de la LED rouge
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 unsigned int : retourne la période du signal de la LED rouge
//************************************************************
unsigned int ObtenirFrequenceCliLED(void)
{
	return frequenceCliLED;
}



//************************************************************
// Fonction ModifierFrequenceRoues : Permet de modifier la fréquence des roues (UART)
//
//       Entrées :
//                 unsigned char : caractère reçu sur l'UART
//
//       Sorties :
//                 NULL
//************************************************************
BOOL ModifierFrequenceRoues(unsigned char caractere)
{
	BOOL ret = false;


	if ((caractere >= '0') && (caractere <= '9')) {	// Si le caractère est numérique (de 0 à 9)
		frequenceRoues = frequenceRoues * 10 + ((int)caractere & 0x0F);

		if (frequenceRoues < FREQUENCE_MAX_ROUES_ROBOT) {
			TXCharUART(caractere);
		} else {
			frequenceRoues = FREQUENCE_MAX_ROUES_ROBOT;

			TXStringUART("\rVeuillez saisir une frequence (Hz) : 250000");
		}
	} else if ((int)caractere == 127) {				// Si le caractère est 'supprimer'
		if (frequenceRoues > 0) {
			frequenceRoues = frequenceRoues / 10;

			TXCharUART(caractere);
		}
	} else if ((int)caractere == 13) {				// Si le caractère est 'entrée'
		if (frequenceRoues < FREQUENCE_MIN_ROUES_ROBOT) {
			frequenceRoues = FREQUENCE_MIN_ROUES_ROBOT;
			TXStringUART("\nFrequence non admise. Definition frequence min : 200");
		}

		DefinirVitesseRoues(frequenceRoues);

		TXStringUART(	"\n\nNouvelle frequence definie"
						"\n\n_______________________________\n\n\n");

		ret = true;
	}


	return ret;
}



//************************************************************
// Fonction DefinirVitesseRoues : Définition de la période du signal des roues
//
//       Entrées :
//                 unsigned long int : défini la période du signal des roues
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirVitesseRoues(unsigned long int frequence)
{
	frequenceRoues = 0;

	TA1CCR0 = ((double)1 / frequence) / ((double)1 / 1000000);
}



//************************************************************
// Fonction IncrementerVitesseRoues : Permet d'augmenter la vitesse du robot
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void IncrementerVitesseRoues(void)
{
	if (ObtenirFrequenceCliLED() < FREQUENCE_MAX_LED_ROBOT) {
		DefinirFrequenceCliLED(ObtenirFrequenceCliLED() + 1);
	}


	if ((TA1CCR1 <= TA1CCR0) && (TA1CCR2 <= TA1CCR0)) {
		TA1CCR1 += 100;
		TA1CCR2 += 100;
	}
}



//************************************************************
// Fonction DecrementerVitesseRoues : Permet de réduire la vitesse du robot
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 BOOL : retourne true si le robot est arrêté
//************************************************************
BOOL DecrementerVitesseRoues(void)
{
	BOOL ret;


	if (FREQUENCE_MIN_LED_ROBOT < ObtenirFrequenceCliLED()) {
		DefinirFrequenceCliLED(ObtenirFrequenceCliLED() - 1);
	}


	if (TA1CCR1 > 100) {
		TA1CCR1 -= 100;
	}


	if (TA1CCR2 > 100) {
		TA1CCR2 -= 100;
	}


	if ((TA1CCR1 <= 100) && (TA1CCR2 <= 100)) {
		TA1CCR1 = TA1CCR2 = 0;
		DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT);
		ret = true;
	} else {
		ret = false;
	}


	return ret;
}
