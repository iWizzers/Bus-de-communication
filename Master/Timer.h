#ifndef TIMER_H_
#define TIMER_H_



char * 			InitPWM					(void);						// Initialisation des roues

void 			DefinirFrequenceCliLED	(unsigned int frequence);	// Permet de définir la fréquence de clignotement de la LED rouge

void 			DefinirVitesseRoues		(unsigned int frequence);	// Définition de la période du signal des roues

void 			IncrementerVitesseRoues	(void);						// Permet d'augmenter la vitesse du robot



#endif /* TIMER_H_ */
