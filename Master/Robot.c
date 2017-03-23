/* ============================================================================ */
/* Description: Permet de commander le robot                                    */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
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
#include <stdlib.h>	// G�n�ration nombre al�atoire



UCHAR 	etatRobot = ARRET;		// Variable contenant l'�tat du robot (ARRET, AVANCER, RECULER, ...)
UCHAR 	modeRobot = AUTONOME;	// Variable contenant le mode de fonctionnement du robot (AUTONOME, MANUEL)
BOOL 		messageEnvoye = false;	// Variable contenant les messages envoy�s lorsqu'il est en mode AUTONOME



//************************************************************
// Fonction DefinirModeRobot : Permet de d�finir le mode de fonctionnement du robot
//
//       Entr�es :
//                 unsigned char : mode de fonctionnement du robot
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirModeRobot(UCHAR mode)
{
	if (mode == (UCHAR)AUTONOME) {			// Si le robot est en mode autonome
		// Initialisation du capteur � 90�
		TXSPI((UCHAR)'z');
		(void)RXSPI();

		__delay_cycles(500000);		// D�lai afin d'obtenir le fixe
	} else {						// Sinon le robot est en mode manuel
		// Ne fait rien
	}


	modeRobot = mode;
}



//************************************************************
// Fonction ObtenirModeRobot : Permet de r�cup�rer le mode de fonctionnement du robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 unsigned char : mode de fonctionnement du robot
//************************************************************
UCHAR ObtenirModeRobot(void)
{
	return modeRobot;		// Retourne le mode de fonctionnement du robot
}



//************************************************************
// Fonction ObtenirEtatRobot : Permet de r�cup�rer l'�tat du robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 unsigned char : �tat du robot
//************************************************************
UCHAR ObtenirEtatRobot(void)
{
	return etatRobot;		// Retourne l'�tat du robot
}



//************************************************************
// Fonction Avancer : Permet de faire avancer le robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Avancer(void)
{
	SINT_32 distanceObstacle;


	if (ObtenirEtatRobot() != (UCHAR)AVANCE) {				// Si le mode du robot n'est pas 'AVANCE'
		Stop(AVANCE);								// ... on arr�te le robot
	} else {										// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
			if (messageEnvoye == false) {			// On v�rifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT AVANCE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// D�finition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);


		// R�cup�ration de la distance d'un obstacle
		distanceObstacle = ObtenirResultatConversionADC(3);


		if (DISTANCE_MIN_OBSTACLE <= distanceObstacle) {		// Si l'obstacle est trop proche du robot
			ArretUrgence();										// ... on arr�te le robot

			if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
				BalayageCapteur();								// ... on balaye la zone
			} else {
				// Ne fait rien
			}
		} else if (DISTANCE_MAX_OBSTACLE <= distanceObstacle) {	// Sinon si l'obstacle est un peu plus loin du robot
			if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
				ArretUrgence();									// ... on arr�te le robot
				BalayageCapteur();								// ... on balaye la zone
			} else {											// Sinon le robot est en mode manuel
				Stop(AVANCE);									// ... on ralenti
			}
		} else {												// Sinon l'obstacle est trop loin du robot
			IncrementerVitesseRoues();							// ... on acc�l�re
		}
	}
}



//************************************************************
// Fonction Reculer : Permet de faire reculer le robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Reculer(void)
{
	if (ObtenirEtatRobot() != (UCHAR)RECULE) {				// Si le mode du robot n'est pas 'RECULE'
		Stop(RECULE);								// ... on arr�te le robot
	} else {										// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
			if (messageEnvoye == false) {			// On v�rifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT RECULE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// D�finition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);


		// On acc�l�re
		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction TournerDroite : Permet de faire tourner le robot � droite
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void TournerDroite(void)
{
	if (ObtenirEtatRobot() != (UCHAR)TOURNE_DROITE) {				// Si le mode du robot n'est pas 'TOURNE_DROITE'
		Stop(TOURNE_DROITE);								// ... on arr�te le robot
	} else {												// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
			if (messageEnvoye == false) {					// On v�rifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT TOURNE A DROITE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// D�finition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);


		// On acc�l�re
		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction TournerGauche : Permet de faire tourner le robot � gauche
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void TournerGauche(void)
{
	if (ObtenirEtatRobot() != (UCHAR)TOURNE_GAUCHE) {				// Si le mode du robot n'est pas 'TOURNE_GAUCHE'
		Stop(TOURNE_GAUCHE);								// ... on arr�te le robot
	} else {												// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
			if (messageEnvoye == false) {					// On v�rifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT TOURNE A GAUCHE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// D�finition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);


		// On acc�l�re
		IncrementerVitesseRoues();
	}
}



//************************************************************
// Fonction Stop : Permet de ralentir puis de stopper le robot
//
//       Entr�es :
//                 unsigned char : si le robot est arr�t�, il prend l'�tat donn�
//
//       Sorties :
//                 NULL
//************************************************************
void Stop(UCHAR etat)
{
	if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
		if (messageEnvoye == false) {			// On v�rifie si on a transmit le message
			messageEnvoye = true;
			TXStringUART("ARRET DU ROBOT\n");	// On transmet le message
		} else {
			// Ne fait rien
		}
	} else {
		// Ne fait rien
	}


	if (DecrementerVitesseRoues() == true) {	// Si le robot est arr�t�
		etatRobot = etat;						// ... on d�finit l'�tat du robot � celui donn�

		messageEnvoye = false;

		if (ObtenirModeRobot() == (UCHAR)MANUEL) {		// Si le robot est en mode manuel
			if (etat == (UCHAR)ARRET) {				// Si l'�tat donn� est 'ARRET'
				DefinirReceptionUART(true);		// ... on d�finit l'�tat de l'UART
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}
	} else {
		if (DISTANCE_MIN_OBSTACLE <= ObtenirResultatConversionADC(3)) {		// Si l'obstacle est trop proche du robot
			ArretUrgence();													// ... on arr�te le robot
		} else {
			// Ne fait rien
		}
	}
}



//************************************************************
// Fonction ArretUrgence : Permet de stopper net le robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void ArretUrgence(void)
{
	ArretRoues();	// Arr�t des deux roues
}



//************************************************************
// Fonction BalayageCapteur : Permet de stopper net le robot
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void BalayageCapteur(void)
{
	SINT_32	tableObstacle[5] = {0, 0, 0, 0, 0};
	SINT_32 	i;
	BOOL 		actionEffectuee = false;


	// Transmission initialisation balayage au MSP430G2231 (0�)
	TXSPI((UCHAR)'i');
	(void)RXSPI();
	__delay_cycles(500000); // D�lai initialisation


	// Lecture distance obstacle et insertion dans le tableau
	tableObstacle[0] = ObtenirResultatConversionADC(3);


	// Boucle de lecture
	for (i = 1; i < 5; i++) {
		// Transmission d�clage du balayage au MSP430G2231 (45�)
		TXSPI((UCHAR)'s');
		(void)RXSPI();
		__delay_cycles(250000); // D�lai d�calage

		// Lecture distance obstacle et insertion dans le tableau
		tableObstacle[i] = ObtenirResultatConversionADC(3);
	}


	// Initialisation du capteur � 90�
	TXSPI((UCHAR)'z');
	(void)RXSPI();


	do {
		switch (GenererNombre(0, 5)) {	// G�n�ration d'un nombre al�atoire entre 0 et 5
		case 0:
			if (DISTANCE_MAX_OBSTACLE >= tableObstacle[0]) {	// Sinon si le robot ne poss�de pas d'obstacle � gauche de lui
				// D�finition du sens des roues
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);

				TA1CCR1 = TA1CCR0 / 2;					// ... d�finition de la vitesse des roues
				TA1CCR2 = TA1CCR0 / 2;

				__delay_cycles(800000);								// ... d�lai initialisation capteur + tourner

				actionEffectuee = true;
			}

			break;
		case 2:
			if (DISTANCE_MAX_OBSTACLE >= tableObstacle[2]) {		// Si le robot ne poss�de pas d'obstacle devant lui
				IncrementerVitesseRoues();							// ... on acc�l�re
				__delay_cycles(500000);								// ... d�lai initialisation capteur

				actionEffectuee = true;
			}

			break;
		case 4:
			if (DISTANCE_MAX_OBSTACLE >= tableObstacle[4]) {	// Sinon si le robot ne poss�de pas d'obstacle � droite de lui
				// D�finition du sens des roues
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);

				TA1CCR1 = TA1CCR0 / 2;					// ... d�finition de la vitesse des roues
				TA1CCR2 = TA1CCR0 / 2;

				__delay_cycles(800000);								// ... d�lai initialisation capteur + tourner

				actionEffectuee = true;
			}

			break;
		default:	// Le robot fait demi-tour
			// D�finition du sens des roues
			if (tableObstacle[0] > tableObstacle[4]) {				// Si l'obstacle de gauche est plus loin que celui de droite
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);	// ... demi-tour par la gauche
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);
			} else {												// Sinon l'obstacle de droite est plus loin que celui de gauche
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);	// ... demi-tour par la droite
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);
			}

			TA1CCR1 = TA1CCR0 / 2;					// ... d�finition de la vitesse des roues
			TA1CCR2 = TA1CCR0 / 2;

			__delay_cycles(1500000);							// ... d�lai initialisation capteur + demi-tour

			actionEffectuee = true;

			break;
		}
	} while (actionEffectuee != true);
}



//************************************************************
// Fonction CommandeAutonome : Permet de g�rer le robot en mode autonome
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void CommandeAutonome(void)
{
	SINT_32	i;								// Variable de boucle
	SINT_32 	delai;							// Variable contenant les d�lais pour le fonctionnement du robot
	UINT_32 	etatRobotAleatoire;				// Variable contenant l'�tat al�atoire du robot


	delai = GenererNombre(500, 1000);				// G�n�ration d'un d�lai entre 10 secondes (500) et 20 secondes (1000)

	for (i = 0; i < delai; i++) {					// Boucle en fonction du d�lai
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
			Avancer();								// ... il avance
			__delay_cycles(20000);					// ... D�lai 50 Hz (invisible � l'oeil humain)
		} else {									// Sinon le robot est en mode manuel = caract�re re�u sur l'UART
			break;									// Sort de la boucle du d�lai
		}
	}


	if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
		do {
			etatRobotAleatoire = (UINT_32)GenererNombre(0, 4);	// G�n�ration d'un nombre al�atoire entre 0 et 4
		} while (etatRobotAleatoire == (UCHAR)AVANCE);


		delai = GenererNombre(50, 250);					// G�n�ration d'un d�lai entre 1 seconde (50) et 5 secondes (250)

		for (i = 0; i < delai; i++) {					// Boucle en fonction du d�lai
			if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
				switch (etatRobotAleatoire) {			// En fonction du nombre g�n�r�...
				case (UCHAR)RECULE:							// ...il recule
					Reculer();
					break;
				case (UCHAR)TOURNE_DROITE:						// ...il tourne � droite
					TournerDroite();
					break;
				case (UCHAR)TOURNE_GAUCHE:						// ...il tourne � gauche
					TournerGauche();
					break;
				default:								// ...il s'arr�te
					if (ObtenirEtatRobot() != (UCHAR)ARRET) {	// Si le robot n'est pas arr�t�
						Stop(ARRET);					// ... ralenti le robot
					}
					break;
				}

				__delay_cycles(20000);					// ... d�lai 50 Hz (invisible � l'oeil humain)
			} else {									// Sinon le robot est en mode manuel = caract�re re�u sur l'UART
				break;									// Sort de la boucle du d�lai
			}
		}
	} else {
		// Ne fait rien
	}
}



//************************************************************
// Fonction CommandeManuelle : Permet de g�rer le robot en mode manuel
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void CommandeManuelle(void)
{
	// Affichage sur le terminal
	TXStringUART("\r(MSP430G2553) ");


	while (ObtenirReceptionUART() == false) {	// Attend la r�ception d'un caract�re
		if (ObtenirEtatRobot() != (UCHAR)ARRET) {		// Si le robot n'est pas arr�t�
			__delay_cycles(20000);				// ... d�lai 50 Hz (invisible � l'oeil humain)
			Stop(ARRET);						// ... ralenti le robot
		}
	}


	// RAZ de l'UART
	DefinirReceptionUART(false);
}



//************************************************************
// Fonction GenererNombre : Permet de g�n�rer un nombre al�atoire
//                          entre deux bornes
//
//       Entr�es :
//                 int : nombre minimum
//                 int : nombre maximum
//
//       Sorties :
//                 int : nombre g�n�r� entre min et max
//************************************************************
SINT_32 GenererNombre(SINT_32 min, SINT_32 max)
{
    return (rand() % (max - min)) + min;	// Retourne le nombre g�n�r�
}
