/* ============================================================================ */
/* Description: Permet de gérer la PWM du servomoteur                           */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "Timer.h"
#include "global.h"




//************************************************************
// Fonction InitPWM
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

	DefinirFrequenceCliLED(FREQUENCE_ROBOT_AUTONOME); 	// Détermine la periode du signal

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
// Fonction DefinirFrequenceCliLED
//
//       Entrées :
//                 unsigned int : défini la période du signal de la LED rouge
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirFrequenceCliLED(unsigned int frequence)
{
	TA0CCR0 = ((double)1 / frequence) / ((double)1 / 1000000 * 4 * 2);
}



//************************************************************
// Fonction DefinirVitesseRoues
//
//       Entrées :
//                 unsigned int : défini la période du signal des roues
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirVitesseRoues(unsigned int frequence)
{
	TA1CCR0 = ((double)1 / frequence) / ((double)1 / 1000000);
}



//************************************************************
// Fonction IncrementerVitesseRoues
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void IncrementerVitesseRoues(void)
{
	if ((TA1CCR1 <= TA1CCR0) && (TA1CCR2 <= TA1CCR0)) {
		TA1CCR1 += 100;
		TA1CCR2 += 100;
	}
}
