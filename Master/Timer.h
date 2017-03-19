#ifndef TIMER_H_
#define TIMER_H_



char * 			InitPWM					(void);						// Initialisation des roues

void 			DefinirFrequenceCliLED	(unsigned int frequence);	// Permet de d�finir la fr�quence de clignotement de la LED rouge

void 			DefinirVitesseRoues		(unsigned int frequence);	// D�finition de la p�riode du signal des roues

void 			IncrementerVitesseRoues	(void);						// Permet d'augmenter la vitesse du robot



#endif /* TIMER_H_ */
