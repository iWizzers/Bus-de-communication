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



unsigned int 		frequenceCliLED;	// Variable contenant la fréquence de clignotement de la LED rouge
unsigned long int	frequenceRoues;		// Variable contenant la fréquence des roues




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

	TA0CCR0 = ((double)1 / frequence) / ((double)1 / 1000000 * 4 * 2);	// Définition de la période du signal
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


	if ((caractere >= '0') && (caractere <= '9')) {							// Si le caractère est numérique (de 0 à 9)
		frequenceRoues = frequenceRoues * 10 + ((int)caractere & 0x0F);		// ... on modifie la fréquence des roues

		if (frequenceRoues < FREQUENCE_MAX_ROUES_ROBOT) {					// Si la fréquence est inférieure au max du robot
			TXCharUART(caractere);											// ... on affiche le caractère
		} else {															// Sinon
			frequenceRoues = FREQUENCE_MAX_ROUES_ROBOT;						// ... on définit la vitesse à la fréquence max
			TXStringUART("\rVeuillez saisir une frequence (Hz) : 250000");
		}
	} else if ((int)caractere == 127) {										// Si le caractère est 'supprimer'
		if (frequenceRoues > 0) {											// Si la fréquence est supérieure à 0
			frequenceRoues = frequenceRoues / 10;							// ... on rectifie la fréquence
			TXCharUART(caractere);											// ... on affiche le caractère
		} else {
			// Ne fait rien
		}
	} else if ((int)caractere == 13) {										// Si le caractère est 'entrée'
		if (frequenceRoues < FREQUENCE_MIN_ROUES_ROBOT) {					// Si la fréquence donnée est inférieure à la fréquence min du robot
			frequenceRoues = FREQUENCE_MIN_ROUES_ROBOT;						// ... on définit la vitesse à la fréquence min
			TXStringUART("\nFrequence non admise. Definition frequence min : 200");
		} else {
			// Ne fait rien
		}

		DefinirVitesseRoues(frequenceRoues);								// On définit la période du signal

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

	TA1CCR0 = ((double)1 / frequence) / ((double)1 / 1000000);		// Définition de la période du signal
}



//************************************************************
// Fonction ArretRoues : Permet d'arrêter les roues du robot
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArretRoues(void)
{
	TA1CCR1 = TA1CCR2 = 0;								// Arrêt des roues

	DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT);	// Définition de la fréquence de clignotement de la LED rouge
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
	if (ObtenirFrequenceCliLED() < FREQUENCE_MAX_LED_ROBOT) {	// Si la fréquence de clignotement est inférieur à la fréquence max de la LED
		DefinirFrequenceCliLED(ObtenirFrequenceCliLED() + 1);	// ... on l'augmente
	} else {
		// Ne fait rien
	}


	if ((TA1CCR1 < TA1CCR0) && (TA1CCR2 < TA1CCR0)) {			// Si la vitesse des deux roues est plus petites que le signal
		TA1CCR1 += 100;											// ... on augmente leurs vitesse
		TA1CCR2 += 100;
	} else {													// Sinon
		TA1CCR1 = TA1CCR2 = TA1CCR0;							// ... on définit la vitesse max
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


	if (FREQUENCE_MIN_LED_ROBOT < ObtenirFrequenceCliLED()) {	// Si la fréquence de clignotement est supérieur à la fréquence min de la LED
		DefinirFrequenceCliLED(ObtenirFrequenceCliLED() - 1);	// ... on la diminue
	} else {
		// Ne fait rien
	}


	if (TA1CCR1 >= 100) {										// Si la vitesse de la roue A est plus grande que 100
		TA1CCR1 -= 100;											// ... on diminue sa vitesse
	} else {
		// Ne fait rien
	}


	if (TA1CCR2 >= 100) {										// Si la vitesse de la roue B est plus grande que 100
		TA1CCR2 -= 100;											// ... on diminue sa vitesse
	} else {
		// Ne fait rien
	}


	if ((TA1CCR1 < 100) && (TA1CCR2 < 100)) {					// Si la vitesse des deux roues est plus petites que 100
		ArretRoues();											// ... on arrête les roues
		DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT);		// ... on définit la fréquence de la LED rouge
		ret = true;
	} else {
		ret = false;
	}


	return ret;
}



//************************************************************
// Fonction CorrigerErreurRoues : Permet de corriger l'erreur des optocoupleurs des roues
//
//       Entrées :
//                 unsigned char : nombre d'incréments de la roue A
//                 unsigned char : nombre d'incréments de la roue B
//
//       Sorties :
//                 NULL
//************************************************************
void CorrigerErreurRoues(unsigned char incrementsRoueA, unsigned char incrementsRoueB)
{
	/*unsigned int valeur;

	if (incrementsRoueA > incrementsRoueB) {
		valeur = TA1CCR1 * (incrementsRoueA - incrementsRoueB) / 100;

		if (TA1CCR1 < TA1CCR0) {
			TA1CCR1 = valeur;
		} else {
			// Ne fait rien
		}
	} else if (incrementsRoueA < incrementsRoueB) {
		valeur = TA1CCR2 * (incrementsRoueB - incrementsRoueA) / 100;

		if (TA1CCR2 < TA1CCR0) {
			TA1CCR2 = valeur;
		} else {
			// Ne fait rien
		}
	} else {
		// Ne fait rien
	}*/
}
