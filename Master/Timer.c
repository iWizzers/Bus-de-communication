/* ============================================================================ */
/* Description: Permet de g�rer la PWM du servomoteur                           */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "Timer.h"
#include "global.h"
#include "UART.h"



unsigned int 		frequenceCliLED;	// Variable contenant la fr�quence de clignotement de la LED rouge
unsigned long int	frequenceRoues;		// Variable contenant la fr�quence des roues




//************************************************************
// Fonction InitPWM : Initialisation des PWM
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 char * : chaine de caract�re donnant l'�tat de l'initialisation
//************************************************************
char * InitPWM(void)
{
	// Timer 0
	TA0CTL = TASSEL_2 | MC_3 | ID_2; 		// Source SMCLK pour TimerA, mode comptage UP/DOWN, pr�division de 4

	DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT); 	// D�termine la periode du signal

	TA0CTL |= TAIE;							// Activation interruption


	// Timer 1
	TA1CTL = TASSEL_2 | MC_1 | ID_0; 		// Source SMCLK pour TimerA, mode comptage Up, pas de pr�division

	TA1CCTL1 = OUTMOD_7; 					// Activation mode de sortie n�7
	TA1CCTL2 = OUTMOD_7; 					// Activation mode de sortie n�7

	DefinirVitesseRoues(250);				// D�termine la periode du signal
	TA1CCR1 = 0;							// Initialisation de la roue A
	TA1CCR2 = 0;							// Initialisation de la roue B

	TA1CTL |= TAIE;							// Activation interruption


	return "OK";
}



//************************************************************
// Fonction DefinirFrequenceCliLED : Permet de d�finir la fr�quence de clignotement de la LED rouge
//
//       Entr�es :
//                 unsigned int : d�fini la p�riode du signal de la LED rouge
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirFrequenceCliLED(unsigned int frequence)
{
	frequenceCliLED = frequence;

	TA0CCR0 = ((double)1 / frequence) / ((double)1 / 1000000 * 4 * 2);	// D�finition de la p�riode du signal
}



//************************************************************
// Fonction ObtenirFrequenceCliLED : Permet de r�cup�rer la frequence de clignotement de la LED rouge
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 unsigned int : retourne la p�riode du signal de la LED rouge
//************************************************************
unsigned int ObtenirFrequenceCliLED(void)
{
	return frequenceCliLED;
}



//************************************************************
// Fonction ModifierFrequenceRoues : Permet de modifier la fr�quence des roues (UART)
//
//       Entr�es :
//                 unsigned char : caract�re re�u sur l'UART
//
//       Sorties :
//                 NULL
//************************************************************
BOOL ModifierFrequenceRoues(unsigned char caractere)
{
	BOOL ret = false;


	if ((caractere >= '0') && (caractere <= '9')) {							// Si le caract�re est num�rique (de 0 � 9)
		frequenceRoues = frequenceRoues * 10 + ((int)caractere & 0x0F);		// ... on modifie la fr�quence des roues

		if (frequenceRoues < FREQUENCE_MAX_ROUES_ROBOT) {					// Si la fr�quence est inf�rieure au max du robot
			TXCharUART(caractere);											// ... on affiche le caract�re
		} else {															// Sinon
			frequenceRoues = FREQUENCE_MAX_ROUES_ROBOT;						// ... on d�finit la vitesse � la fr�quence max
			TXStringUART("\rVeuillez saisir une frequence (Hz) : 250000");
		}
	} else if ((int)caractere == 127) {										// Si le caract�re est 'supprimer'
		if (frequenceRoues > 0) {											// Si la fr�quence est sup�rieure � 0
			frequenceRoues = frequenceRoues / 10;							// ... on rectifie la fr�quence
			TXCharUART(caractere);											// ... on affiche le caract�re
		} else {
			// Ne fait rien
		}
	} else if ((int)caractere == 13) {										// Si le caract�re est 'entr�e'
		if (frequenceRoues < FREQUENCE_MIN_ROUES_ROBOT) {					// Si la fr�quence donn�e est inf�rieure � la fr�quence min du robot
			frequenceRoues = FREQUENCE_MIN_ROUES_ROBOT;						// ... on d�finit la vitesse � la fr�quence min
			TXStringUART("\nFrequence non admise. Definition frequence min : 200");
		} else {
			// Ne fait rien
		}

		DefinirVitesseRoues(frequenceRoues);								// On d�finit la p�riode du signal

		TXStringUART(	"\n\nNouvelle frequence definie"
						"\n\n_______________________________\n\n\n");

		ret = true;
	}


	return ret;
}



//************************************************************
// Fonction DefinirVitesseRoues : D�finition de la p�riode du signal des roues
//
//       Entr�es :
//                 unsigned long int : d�fini la p�riode du signal des roues
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirVitesseRoues(unsigned long int frequence)
{
	frequenceRoues = 0;

	TA1CCR0 = ((double)1 / frequence) / ((double)1 / 1000000);		// D�finition de la p�riode du signal
}



//************************************************************
// Fonction ArretRoues : Permet d'arr�ter les roues du robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArretRoues(void)
{
	TA1CCR1 = TA1CCR2 = 0;								// Arr�t des roues

	DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT);	// D�finition de la fr�quence de clignotement de la LED rouge
}



//************************************************************
// Fonction IncrementerVitesseRoues : Permet d'augmenter la vitesse du robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void IncrementerVitesseRoues(void)
{
	if (ObtenirFrequenceCliLED() < FREQUENCE_MAX_LED_ROBOT) {	// Si la fr�quence de clignotement est inf�rieur � la fr�quence max de la LED
		DefinirFrequenceCliLED(ObtenirFrequenceCliLED() + 1);	// ... on l'augmente
	} else {
		// Ne fait rien
	}


	if ((TA1CCR1 < TA1CCR0) && (TA1CCR2 < TA1CCR0)) {			// Si la vitesse des deux roues est plus petites que le signal
		TA1CCR1 += 100;											// ... on augmente leurs vitesse
		TA1CCR2 += 100;
	} else {													// Sinon
		TA1CCR1 = TA1CCR2 = TA1CCR0;							// ... on d�finit la vitesse max
	}
}



//************************************************************
// Fonction DecrementerVitesseRoues : Permet de r�duire la vitesse du robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 BOOL : retourne true si le robot est arr�t�
//************************************************************
BOOL DecrementerVitesseRoues(void)
{
	BOOL ret;


	if (FREQUENCE_MIN_LED_ROBOT < ObtenirFrequenceCliLED()) {	// Si la fr�quence de clignotement est sup�rieur � la fr�quence min de la LED
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
		ArretRoues();											// ... on arr�te les roues
		DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT);		// ... on d�finit la fr�quence de la LED rouge
		ret = true;
	} else {
		ret = false;
	}


	return ret;
}



//************************************************************
// Fonction CorrigerErreurRoues : Permet de corriger l'erreur des optocoupleurs des roues
//
//       Entr�es :
//                 unsigned char : nombre d'incr�ments de la roue A
//                 unsigned char : nombre d'incr�ments de la roue B
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
