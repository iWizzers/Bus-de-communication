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
#include "SPI.h"



unsigned char 	etatRobot = ARRET;
unsigned char 	modeRobot = AUTONOME;
BOOL 			messageEnvoye = false;



//************************************************************
// Fonction DefinirModeRobot : Permet de définir le mode de fonctionnement du robot
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
// Fonction ObtenirModeRobot : Permet de récupérer le mode de fonctionnement du robot
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
// Fonction ObtenirEtatRobot : Permet de récupérer l'état du robot
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
// Fonction Avancer : Permet de faire avancer le robot
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Avancer(void)
{
	unsigned int distanceObstacle = ObtenirResultatConversionADC(3);
	unsigned int tableObstacle[5] = {0, 0, 0, 0, 0};
	int i;


	if (ObtenirEtatRobot() != AVANCE) {
		Stop(AVANCE);
	} else {
		if (messageEnvoye == false) {
			messageEnvoye = true;
			TXStringUART("LE ROBOT AVANCE\n");
		} else {
			// Ne fait rien
		}


		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, true);


		distanceObstacle = ObtenirResultatConversionADC(3);


		if (DISTANCE_MIN_OBSTACLE <= distanceObstacle) {
			ArretUrgence();
		} else if (DISTANCE_MAX_OBSTACLE <= distanceObstacle) {
			if (ObtenirModeRobot() == AUTONOME) {
				ArretUrgence();


				TXSPI('i');
				RXSPI();

				__delay_cycles(500000);

				tableObstacle[0] = ObtenirResultatConversionADC(3);


				for (i = 1; i < 5; i++) {
					TXSPI('s');
					RXSPI();

					__delay_cycles(500000);

					tableObstacle[i] = ObtenirResultatConversionADC(3);
				}


				TXSPI('z');
				RXSPI();


				if (DISTANCE_MAX_OBSTACLE >= tableObstacle[0]) {
					ActiverGPIOPort2(BIT_SENS_MOTEUR_A, false);
					ActiverGPIOPort2(BIT_SENS_MOTEUR_B, true);

					TA1CCR1 = TA1CCR2 = TA1CCR0 / 2;

					__delay_cycles(800000);
				} else if (DISTANCE_MAX_OBSTACLE >= tableObstacle[2]) {
					IncrementerVitesseRoues();
				} else if (DISTANCE_MAX_OBSTACLE >= tableObstacle[4]) {
					ActiverGPIOPort2(BIT_SENS_MOTEUR_A, true);
					ActiverGPIOPort2(BIT_SENS_MOTEUR_B, false);

					TA1CCR1 = TA1CCR2 = TA1CCR0 / 2;

					__delay_cycles(800000);
				} else {
					ActiverGPIOPort2(BIT_SENS_MOTEUR_A, false);
					ActiverGPIOPort2(BIT_SENS_MOTEUR_B, true);

					TA1CCR1 = TA1CCR2 = TA1CCR0 / 2;

					__delay_cycles(1500000);
				}
			} else {
				Stop(AVANCE);
			}
		} else {
			IncrementerVitesseRoues();
		}
	}
}



//************************************************************
// Fonction Reculer : Permet de faire reculer le robot
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
			TXStringUART("LE ROBOT RECULE\n");
		} else {
			// Ne fait rien
		}

		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, false);

		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction TournerDroite : Permet de faire tourner le robot à droite
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
			TXStringUART("LE ROBOT TOURNE A DROITE\n");
		} else {
			// Ne fait rien
		}

		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, false);

		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction TournerGauche : Permet de faire tourner le robot à gauche
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
			TXStringUART("LE ROBOT TOURNE A GAUCHE\n");
		} else {
			// Ne fait rien
		}

		ActiverGPIOPort2(BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2(BIT_SENS_MOTEUR_B, true);

		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction Stop : Permet de ralentir puis de stopper le robot
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
		TXStringUART("ARRET DU ROBOT\n");
	} else {
		// Ne fait rien
	}


	if (DecrementerVitesseRoues() == true) {
		etatRobot = etat;
		messageEnvoye = false;

		if (etat == ARRET) {
			DefinirReceptionUART(true);
		} else {
			// Ne fait rien
		}
	} else {
		// Ne fait rien
	}
}



//************************************************************
// Fonction ArretUrgence : Permet de stopper net le robot
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
	DefinirFrequenceCliLED(FREQUENCE_MIN_LED_ROBOT);
}
