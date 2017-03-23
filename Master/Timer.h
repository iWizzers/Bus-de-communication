#ifndef TIMER_H_
#define TIMER_H_



#include "typedef.h"



void 			InitPWM					(void);															// Initialisation des PWM

void 			DefinirFrequenceCliLED	(UINT_32 frequence);										// Permet de définir la fréquence de clignotement de la LED rouge
UINT_32 	ObtenirFrequenceCliLED	(void);															// Permet de récupérer la frequence de clignotement de la LED rouge

BOOL 			ModifierFrequenceRoues	(UCHAR caractere);										// Permet de modifier la fréquence des roues (UART)
void 			DefinirVitesseRoues		(UINT_64 frequence);									// Définition de la période du signal des roues

void 			ArretRoues				(void);															// Permet d'arrêter les roues du robot
void 			IncrementerVitesseRoues	(void);															// Permet d'augmenter la vitesse du robot
BOOL 			DecrementerVitesseRoues	(void);															// Permet de réduire la vitesse du robot



#endif /* TIMER_H_ */
