#ifndef TIMER_H_
#define TIMER_H_



#include "typedef.h"



char * 			InitPWM					(void);															// Initialisation des PWM

void 			DefinirFrequenceCliLED	(unsigned int frequence);										// Permet de d�finir la fr�quence de clignotement de la LED rouge
unsigned int 	ObtenirFrequenceCliLED	(void);															// Permet de r�cup�rer la frequence de clignotement de la LED rouge

BOOL 			ModifierFrequenceRoues	(unsigned char caractere);										// Permet de modifier la fr�quence des roues (UART)
void 			DefinirVitesseRoues		(unsigned long int frequence);									// D�finition de la p�riode du signal des roues

void 			IncrementerVitesseRoues	(void);															// Permet d'augmenter la vitesse du robot
BOOL 			DecrementerVitesseRoues	(void);															// Permet de r�duire la vitesse du robot
void 			CorrigerErreurRoues		(unsigned char incrementsRoueA, unsigned char incrementsRoueB);	// Permet de corriger l'erreur des optocoupleurs des roues



#endif /* TIMER_H_ */
