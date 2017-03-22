/* ============================================================================ */
/* Description: Permet de g�rer la PWM du servomoteur                           */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2231.h>
#include "Timer.h"
#include "global.h"
#include "typedef.h"



unsigned int 	position 	= _0_DEGRE,
				deplacement45Degres;



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

	deplacement45Degres = _180_DEGRES / 5;
}



//************************************************************
// Fonction Init0Degre
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Init0Degre(void)
{
	position = _0_DEGRE;
	TACCR1 = position;

	__delay_cycles(500000);
}



//************************************************************
// Fonction Init90Degres
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Init90Degres(void)
{
	position = _0_DEGRE + 2 * deplacement45Degres;
	TACCR1 = position;

	__delay_cycles(500000);
}



//************************************************************
// Fonction Scan180Degres
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Scan180Degres(void)
{
	position += deplacement45Degres;
	TACCR1 = position;

	__delay_cycles(500000);
}
