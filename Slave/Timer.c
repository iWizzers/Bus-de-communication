/* ============================================================================ */
/* Description: Permet de g�rer la PWM du servomoteur                           */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "Timer.h"



//************************************************************
// Fonction InitPWM
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitPWM(void)
{
	TACTL = (TASSEL_2 | MC_1 | ID_0); 	// Source SMCLK pour TimerA, mode comptage Up, pas de pr�division
	TACCTL1 = OUTMOD_7; 				// Activation mode de sortie n�7
	TACCR0 = 20000; 					// D�termine la p�riode du signal
	TACCR1 = 500;						// Initialisation � 0�
}
