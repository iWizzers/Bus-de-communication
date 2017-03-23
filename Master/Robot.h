#ifndef ROBOT_H_
#define ROBOT_H_



void 			DefinirModeRobot	(unsigned char mode);	// Permet de définir le mode de fonctionnement du robot
unsigned char 	ObtenirModeRobot	(void);					// Permet de récupérer le mode de fonctionnement du robot

unsigned char 	ObtenirEtatRobot	(void);					// Permet de récupérer l'état du robot

void 			Avancer				(void);					// Permet de faire avancer le robot
void 			Reculer				(void);					// Permet de faire reculer le robot
void 			TournerDroite		(void);					// Permet de faire tourner le robot à droite
void 			TournerGauche		(void);					// Permet de faire tourner le robot à gauche
void			Stop				(unsigned char etat);	// Permet de ralentir puis de stopper le robot
void 			ArretUrgence		(void);					// Permet de stopper net le robot

void 			BalayageCapteur		(void);					// Permet de balayer le devant du robot jusqu'à 180°

void 			CommandeAutonome	(void);					// Permet de gérer le robot en mode autonome
void 			CommandeManuelle	(void);					// Permet de gérer le robot en mode manuel

int 			GenererNombre(int min, int max);			// Permet de générer un nombre aléatoire entre deux bornes



#endif /* ROBOT_H_ */
