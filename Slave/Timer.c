/* ============================================================================ */
/* Description: Permet de gérer la PWM du servomoteur                           */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "Timer.h"



//************************************************************
// Fonction InitPWM
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitPWM(void)
{
	TACTL = (TASSEL_2 | MC_1 | ID_0); 	// Source SMCLK pour TimerA, mode comptage Up, pas de prédivision
	TACCTL1 = OUTMOD_7; 				// Activation mode de sortie n°7
	TACCR0 = 20000; 					// Détermine la période du signal
	TACCR1 = 500;						// Initialisation à 0°
}
