/* ============================================================================ */
/* Description: Permet de commander le robot                                    */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "Robot.h"
#include "global.h"
#include "Timer.h"
#include "GPIO.h"
#include "ADC.h"
#include "UART.h"



unsigned char 	etatRobot = ARRET;
unsigned char 	modeRobot = AUTONOME;
BOOL 			messageEnvoye = false;



//************************************************************
// Fonction DefinirModeRobot
//
//       Entrées :
//                 unsigned char : mode de fonctionnement du robot
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirModeRobot(unsigned char mode)
{
	modeRobot = mode;
}



//************************************************************
// Fonction ObtenirModeRobot
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 unsigned char : mode de fonctionnement du robot
//************************************************************
unsigned char ObtenirModeRobot(void)
{
	return modeRobot;
}



//************************************************************
// Fonction ObtenirEtatRobot
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 unsigned char : état du robot
//************************************************************
unsigned char ObtenirEtatRobot(void)
{
	return etatRobot;
}



//************************************************************
// Fonction Avancer
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Avancer(void)
{
	if (ObtenirEtatRobot() != AVANCE) {
		Stop(AVANCE);
	} else {
		if (messageEnvoye == false) {
			messageEnvoye = true;
			TXUART("LE ROBOT AVANCE\n");
		}

		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, true);

		unsigned int distanceObstacle = ObtenirResultatConversionADC(3);

		if (DISTANCE_MIN_OBSTACLE <= distanceObstacle) {
			ArretUrgence();
		} else if (DISTANCE_MAX_OBSTACLE <= distanceObstacle) {
			Stop(AVANCE);
		} else {
			IncrementerVitesseRoues();
		}
	}
}



//************************************************************
// Fonction Reculer
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Reculer(void)
{
	if (ObtenirEtatRobot() != RECULE) {
		Stop(RECULE);
	} else {
		if (messageEnvoye == false) {
			messageEnvoye = true;
			TXUART("LE ROBOT RECULE\n");
		}

		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, false);

		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction TournerDroite
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void TournerDroite(void)
{
	if (ObtenirEtatRobot() != TOURNE_DROITE) {
		Stop(TOURNE_DROITE);
	} else {
		if (messageEnvoye == false) {
			messageEnvoye = true;
			TXUART("LE ROBOT TOURNE A DROITE\n");
		}

		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, false);

		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction TournerGauche
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void TournerGauche(void)
{
	if (ObtenirEtatRobot() != TOURNE_GAUCHE) {
		Stop(TOURNE_GAUCHE);
	} else {
		if (messageEnvoye == false) {
			messageEnvoye = true;
			TXUART("LE ROBOT TOURNE A GAUCHE\n");
		}

		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, true);

		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction Stop
//
//       Entrées :
//                 unsigned char : si le robot est arrêté, il prend l'état donné
//
//       Sorties :
//                 NULL
//************************************************************
void Stop(unsigned char etat)
{
	if (messageEnvoye == false) {
		messageEnvoye = true;
		TXUART("ARRET DU ROBOT\n");
	}

	if (TA1CCR1 > 100) {
		TA1CCR1 -= 100;
	}

	if (TA1CCR2 > 100) {
		TA1CCR2 -= 100;
	}

	if ((TA1CCR1 <= 100) && (TA1CCR2 <= 100)) {
		etatRobot = etat;
		TA1CCR1 = TA1CCR2 = 0;
		messageEnvoye = false;
	}
}



//************************************************************
// Fonction ArretUrgence
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArretUrgence(void)
{
	TA1CCR1 = TA1CCR2 = 0;
}
