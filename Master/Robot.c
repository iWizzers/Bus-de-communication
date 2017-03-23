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
#include <stdlib.h>	// Génération nombre aléatoire



UCHAR 	etatRobot = ARRET;		// Variable contenant l'état du robot (ARRET, AVANCER, RECULER, ...)
UCHAR 	modeRobot = AUTONOME;	// Variable contenant le mode de fonctionnement du robot (AUTONOME, MANUEL)
BOOL 		messageEnvoye = false;	// Variable contenant les messages envoyés lorsqu'il est en mode AUTONOME



//************************************************************
// Fonction DefinirModeRobot : Permet de définir le mode de fonctionnement du robot
//
//       Entrées :
//                 unsigned char : mode de fonctionnement du robot
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirModeRobot(UCHAR mode)
{
	if (mode == (UCHAR)AUTONOME) {			// Si le robot est en mode autonome
		// Initialisation du capteur à 90°
		TXSPI((UCHAR)'z');
		(void)RXSPI();

		__delay_cycles(500000);		// Délai afin d'obtenir le fixe
	} else {						// Sinon le robot est en mode manuel
		// Ne fait rien
	}


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
UCHAR ObtenirModeRobot(void)
{
	return modeRobot;		// Retourne le mode de fonctionnement du robot
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
UCHAR ObtenirEtatRobot(void)
{
	return etatRobot;		// Retourne l'état du robot
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
	SINT_32 distanceObstacle;


	if (ObtenirEtatRobot() != (UCHAR)AVANCE) {				// Si le mode du robot n'est pas 'AVANCE'
		Stop(AVANCE);								// ... on arrête le robot
	} else {										// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
			if (messageEnvoye == false) {			// On vérifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT AVANCE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// Définition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);


		// Récupération de la distance d'un obstacle
		distanceObstacle = ObtenirResultatConversionADC(3);


		if (DISTANCE_MIN_OBSTACLE <= distanceObstacle) {		// Si l'obstacle est trop proche du robot
			ArretUrgence();										// ... on arrête le robot

			if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
				BalayageCapteur();								// ... on balaye la zone
			} else {
				// Ne fait rien
			}
		} else if (DISTANCE_MAX_OBSTACLE <= distanceObstacle) {	// Sinon si l'obstacle est un peu plus loin du robot
			if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
				ArretUrgence();									// ... on arrête le robot
				BalayageCapteur();								// ... on balaye la zone
			} else {											// Sinon le robot est en mode manuel
				Stop(AVANCE);									// ... on ralenti
			}
		} else {												// Sinon l'obstacle est trop loin du robot
			IncrementerVitesseRoues();							// ... on accélère
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
	if (ObtenirEtatRobot() != (UCHAR)RECULE) {				// Si le mode du robot n'est pas 'RECULE'
		Stop(RECULE);								// ... on arrête le robot
	} else {										// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
			if (messageEnvoye == false) {			// On vérifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT RECULE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// Définition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);


		// On accélère
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
	if (ObtenirEtatRobot() != (UCHAR)TOURNE_DROITE) {				// Si le mode du robot n'est pas 'TOURNE_DROITE'
		Stop(TOURNE_DROITE);								// ... on arrête le robot
	} else {												// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
			if (messageEnvoye == false) {					// On vérifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT TOURNE A DROITE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// Définition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);


		// On accélère
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
	if (ObtenirEtatRobot() != (UCHAR)TOURNE_GAUCHE) {				// Si le mode du robot n'est pas 'TOURNE_GAUCHE'
		Stop(TOURNE_GAUCHE);								// ... on arrête le robot
	} else {												// Sinon
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
			if (messageEnvoye == false) {					// On vérifie si on a transmit le message
				messageEnvoye = true;
				TXStringUART("LE ROBOT TOURNE A GAUCHE\n");	// On transmet le message
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}


		// Définition du sens des roues
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);
		ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);


		// On accélère
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
void Stop(UCHAR etat)
{
	if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
		if (messageEnvoye == false) {			// On vérifie si on a transmit le message
			messageEnvoye = true;
			TXStringUART("ARRET DU ROBOT\n");	// On transmet le message
		} else {
			// Ne fait rien
		}
	} else {
		// Ne fait rien
	}


	if (DecrementerVitesseRoues() == true) {	// Si le robot est arrêté
		etatRobot = etat;						// ... on définit l'état du robot à celui donné

		messageEnvoye = false;

		if (ObtenirModeRobot() == (UCHAR)MANUEL) {		// Si le robot est en mode manuel
			if (etat == (UCHAR)ARRET) {				// Si l'état donné est 'ARRET'
				DefinirReceptionUART(true);		// ... on définit l'état de l'UART
			} else {
				// Ne fait rien
			}
		} else {
			// Ne fait rien
		}
	} else {
		if (DISTANCE_MIN_OBSTACLE <= ObtenirResultatConversionADC(3)) {		// Si l'obstacle est trop proche du robot
			ArretUrgence();													// ... on arrête le robot
		} else {
			// Ne fait rien
		}
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
	ArretRoues();	// Arrêt des deux roues
}



//************************************************************
// Fonction BalayageCapteur : Permet de stopper net le robot
//
//       Entrées :
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


	// Transmission initialisation balayage au MSP430G2231 (0°)
	TXSPI((UCHAR)'i');
	(void)RXSPI();
	__delay_cycles(500000); // Délai initialisation


	// Lecture distance obstacle et insertion dans le tableau
	tableObstacle[0] = ObtenirResultatConversionADC(3);


	// Boucle de lecture
	for (i = 1; i < 5; i++) {
		// Transmission déclage du balayage au MSP430G2231 (45°)
		TXSPI((UCHAR)'s');
		(void)RXSPI();
		__delay_cycles(250000); // Délai décalage

		// Lecture distance obstacle et insertion dans le tableau
		tableObstacle[i] = ObtenirResultatConversionADC(3);
	}


	// Initialisation du capteur à 90°
	TXSPI((UCHAR)'z');
	(void)RXSPI();


	do {
		switch (GenererNombre(0, 5)) {	// Génération d'un nombre aléatoire entre 0 et 5
		case 0:
			if (DISTANCE_MAX_OBSTACLE >= tableObstacle[0]) {	// Sinon si le robot ne possède pas d'obstacle à gauche de lui
				// Définition du sens des roues
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);

				TA1CCR1 = TA1CCR0 / 2;					// ... définition de la vitesse des roues
				TA1CCR2 = TA1CCR0 / 2;

				__delay_cycles(800000);								// ... délai initialisation capteur + tourner

				actionEffectuee = true;
			}

			break;
		case 2:
			if (DISTANCE_MAX_OBSTACLE >= tableObstacle[2]) {		// Si le robot ne possède pas d'obstacle devant lui
				IncrementerVitesseRoues();							// ... on accélère
				__delay_cycles(500000);								// ... délai initialisation capteur

				actionEffectuee = true;
			}

			break;
		case 4:
			if (DISTANCE_MAX_OBSTACLE >= tableObstacle[4]) {	// Sinon si le robot ne possède pas d'obstacle à droite de lui
				// Définition du sens des roues
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);

				TA1CCR1 = TA1CCR0 / 2;					// ... définition de la vitesse des roues
				TA1CCR2 = TA1CCR0 / 2;

				__delay_cycles(800000);								// ... délai initialisation capteur + tourner

				actionEffectuee = true;
			}

			break;
		default:	// Le robot fait demi-tour
			// Définition du sens des roues
			if (tableObstacle[0] > tableObstacle[4]) {				// Si l'obstacle de gauche est plus loin que celui de droite
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, false);	// ... demi-tour par la gauche
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, true);
			} else {												// Sinon l'obstacle de droite est plus loin que celui de gauche
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_A, true);	// ... demi-tour par la droite
				ActiverGPIOPort2((UCHAR)BIT_SENS_MOTEUR_B, false);
			}

			TA1CCR1 = TA1CCR0 / 2;					// ... définition de la vitesse des roues
			TA1CCR2 = TA1CCR0 / 2;

			__delay_cycles(1500000);							// ... délai initialisation capteur + demi-tour

			actionEffectuee = true;

			break;
		}
	} while (actionEffectuee != true);
}



//************************************************************
// Fonction CommandeAutonome : Permet de gérer le robot en mode autonome
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void CommandeAutonome(void)
{
	SINT_32	i;								// Variable de boucle
	SINT_32 	delai;							// Variable contenant les délais pour le fonctionnement du robot
	UINT_32 	etatRobotAleatoire;				// Variable contenant l'état aléatoire du robot


	delai = GenererNombre(500, 1000);				// Génération d'un délai entre 10 secondes (500) et 20 secondes (1000)

	for (i = 0; i < delai; i++) {					// Boucle en fonction du délai
		if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
			Avancer();								// ... il avance
			__delay_cycles(20000);					// ... Délai 50 Hz (invisible à l'oeil humain)
		} else {									// Sinon le robot est en mode manuel = caractère reçu sur l'UART
			break;									// Sort de la boucle du délai
		}
	}


	if (ObtenirModeRobot() == (UCHAR)AUTONOME) {				// Si le robot est en mode autonome
		do {
			etatRobotAleatoire = (UINT_32)GenererNombre(0, 4);	// Génération d'un nombre aléatoire entre 0 et 4
		} while (etatRobotAleatoire == (UCHAR)AVANCE);


		delai = GenererNombre(50, 250);					// Génération d'un délai entre 1 seconde (50) et 5 secondes (250)

		for (i = 0; i < delai; i++) {					// Boucle en fonction du délai
			if (ObtenirModeRobot() == (UCHAR)AUTONOME) {		// Si le robot est en mode autonome
				switch (etatRobotAleatoire) {			// En fonction du nombre généré...
				case (UCHAR)RECULE:							// ...il recule
					Reculer();
					break;
				case (UCHAR)TOURNE_DROITE:						// ...il tourne à droite
					TournerDroite();
					break;
				case (UCHAR)TOURNE_GAUCHE:						// ...il tourne à gauche
					TournerGauche();
					break;
				default:								// ...il s'arrête
					if (ObtenirEtatRobot() != (UCHAR)ARRET) {	// Si le robot n'est pas arrêté
						Stop(ARRET);					// ... ralenti le robot
					}
					break;
				}

				__delay_cycles(20000);					// ... délai 50 Hz (invisible à l'oeil humain)
			} else {									// Sinon le robot est en mode manuel = caractère reçu sur l'UART
				break;									// Sort de la boucle du délai
			}
		}
	} else {
		// Ne fait rien
	}
}



//************************************************************
// Fonction CommandeManuelle : Permet de gérer le robot en mode manuel
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void CommandeManuelle(void)
{
	// Affichage sur le terminal
	TXStringUART("\r(MSP430G2553) ");


	while (ObtenirReceptionUART() == false) {	// Attend la réception d'un caractère
		if (ObtenirEtatRobot() != (UCHAR)ARRET) {		// Si le robot n'est pas arrêté
			__delay_cycles(20000);				// ... délai 50 Hz (invisible à l'oeil humain)
			Stop(ARRET);						// ... ralenti le robot
		}
	}


	// RAZ de l'UART
	DefinirReceptionUART(false);
}



//************************************************************
// Fonction GenererNombre : Permet de générer un nombre aléatoire
//                          entre deux bornes
//
//       Entrées :
//                 int : nombre minimum
//                 int : nombre maximum
//
//       Sorties :
//                 int : nombre généré entre min et max
//************************************************************
SINT_32 GenererNombre(SINT_32 min, SINT_32 max)
{
    return (rand() % (max - min)) + min;	// Retourne le nombre généré
}
