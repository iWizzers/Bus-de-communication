#ifndef ROBOT_H_
#define ROBOT_H_



void 			DefinirModeRobot	(unsigned char mode);	// Permet de d�finir le mode de fonctionnement du robot
unsigned char 	ObtenirModeRobot	(void);					// Permet de r�cup�rer le mode de fonctionnement du robot

unsigned char 	ObtenirEtatRobot	(void);					// Permet de r�cup�rer l'�tat du robot

void 			Avancer				(void);					// Permet de faire avancer le robot
void 			Reculer				(void);					// Permet de faire reculer le robot
void 			TournerDroite		(void);					// Permet de faire tourner le robot � droite
void 			TournerGauche		(void);					// Permet de faire tourner le robot � gauche
void			Stop				(unsigned char etat);	// Permet de ralentir puis de stopper le robot
void 			ArretUrgence		(void);					// Permet de stopper net le robot



#endif /* ROBOT_H_ */
