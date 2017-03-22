/* ============================================================================ */
/* Description: Permet de gérer la PWM du servomoteur                           */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
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

	deplacement45Degres = _180_DEGRES / 5;
}



//************************************************************
// Fonction Init0Degre
//
//       Entrées :
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
//       Entrées :
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
//       Entrées :
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
