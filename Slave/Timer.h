#ifndef TIMER_H_
#define TIMER_H_



void InitPWM		(void);	// Initialisation du servomoteur

void Init0Degre		(void); // Permet de mettre le capteur à gauche du robot
void Init90Degres	(void);	// Permet de mettre le capteur devant le robot

void Scan180Degres	(void);	// Permet de scanner de 0° à 180°



#endif /* TIMER_H_ */
