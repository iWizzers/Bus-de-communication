#ifndef ROBOT_H_
#define ROBOT_H_



#include "typedef.h"



void 		DefinirModeRobot	(UCHAR mode);	// Permet de d�finir le mode de fonctionnement du robot
UCHAR 	ObtenirModeRobot	(void);					// Permet de r�cup�rer le mode de fonctionnement du robot

UCHAR 	ObtenirEtatRobot	(void);					// Permet de r�cup�rer l'�tat du robot

void 		Avancer				(void);					// Permet de faire avancer le robot
void 		Reculer				(void);					// Permet de faire reculer le robot
void 		TournerDroite		(void);					// Permet de faire tourner le robot � droite
void 		TournerGauche		(void);					// Permet de faire tourner le robot � gauche
void		Stop				(UCHAR etat);	// Permet de ralentir puis de stopper le robot
void 		ArretUrgence		(void);					// Permet de stopper net le robot

void 		BalayageCapteur		(void);					// Permet de balayer le devant du robot jusqu'� 180�

void 		CommandeAutonome	(void);					// Permet de g�rer le robot en mode autonome
void 		CommandeManuelle	(void);					// Permet de g�rer le robot en mode manuel

SINT_32 	GenererNombre(SINT_32 min, SINT_32 max);			// Permet de g�n�rer un nombre al�atoire entre deux bornes



#endif /* ROBOT_H_ */
