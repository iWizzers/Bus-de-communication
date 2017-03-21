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
ROTATION 		sens 		= horaire;



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
	TACCR1 = _0_DEGRE;					// Initialisation � 0�

	deplacement45Degres = _180_DEGRES / 5;

	__delay_cycles(1000000);
}



//************************************************************
// Fonction Deplacement45Degres
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Deplacement45Degres(void)
{
	if (sens == horaire) {
		/* Passage de X� � 180�
		 *           X____________
		 *             ________   |
		 *            |        |  |
		 *            | Servo  |  V -> 180�
		 *            |________|
		 */
		position += deplacement45Degres;
	} else {
		/* Passage de X� � 0�
		 *          ____________X
		 *         |   ________
		 *         |  |        |
		 *   0� <- V  | Servo  |
		 *            |________|
		 */
		position -= deplacement45Degres;
	}


	if (position == _0_DEGRE) {
		sens = horaire;
	} else if (position == _180_DEGRES) {
		sens = antiHoraire;
	} else {
		// Ne fait rien
	}


	TACCR1 = position;
}
