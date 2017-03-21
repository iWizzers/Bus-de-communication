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



unsigned int 		frequenceCliLED;
unsigned long int	frequenceRoues;




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

	TA0CCR0 = ((double)1 / frequence) / ((double)1 / 1000000 * 4 * 2);
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


	if ((caractere >= '0') && (caractere <= '9')) {	// Si le caract�re est num�rique (de 0 � 9)
		frequenceRoues = frequenceRoues * 10 + ((int)caractere & 0x0F);

		if (frequenceRoues < FREQUENCE_MAX_ROUES_ROBOT) {
			TXCharUART(caractere);
		} else {
			frequenceRoues = FREQUENCE_MAX_ROUES_ROBOT;

			TXStringUART("\rVeuillez saisir une frequence (Hz) : 250000");
		}
	} else if ((int)caractere == 127) {				// Si le caract�re est 'supprimer'
		if (frequenceRoues > 0) {
			frequenceRoues = frequenceRoues / 10;

			TXCharUART(caractere);
		}
	} else if ((int)caractere == 13) {				// Si le caract�re est 'entr�e'
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

	TA1CCR0 = ((double)1 / frequence) / ((double)1 / 1000000);
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
	if (ObtenirFrequenceCliLED() < FREQUENCE_MAX_LED_ROBOT) {
		DefinirFrequenceCliLED(ObtenirFrequenceCliLED() + 1);
	}


	if ((TA1CCR1 <= TA1CCR0) && (TA1CCR2 <= TA1CCR0)) {
		TA1CCR1 += 100;
		TA1CCR2 += 100;
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
