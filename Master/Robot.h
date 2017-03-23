#ifndef ROBOT_H_
#define ROBOT_H_



#include "typedef.h"



void 		DefinirModeRobot	(UCHAR mode);	// Permet de définir le mode de fonctionnement du robot
UCHAR 	ObtenirModeRobot	(void);					// Permet de récupérer le mode de fonctionnement du robot

UCHAR 	ObtenirEtatRobot	(void);					// Permet de récupérer l'état du robot

void 		Avancer				(void);					// Permet de faire avancer le robot
void 		Reculer				(void);					// Permet de faire reculer le robot
void 		TournerDroite		(void);					// Permet de faire tourner le robot à droite
void 		TournerGauche		(void);					// Permet de faire tourner le robot à gauche
void		Stop				(UCHAR etat);	// Permet de ralentir puis de stopper le robot
void 		ArretUrgence		(void);					// Permet de stopper net le robot

void 		BalayageCapteur		(void);					// Permet de balayer le devant du robot jusqu'à 180°

void 		CommandeAutonome	(void);					// Permet de gérer le robot en mode autonome
void 		CommandeManuelle	(void);					// Permet de gérer le robot en mode manuel

SINT_32 	GenererNombre(SINT_32 min, SINT_32 max);			// Permet de générer un nombre aléatoire entre deux bornes



#endif /* ROBOT_H_ */
