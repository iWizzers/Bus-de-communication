#ifndef TIMER_H_
#define TIMER_H_



#include "typedef.h"



void 			InitPWM					(void);															// Initialisation des PWM

void 			DefinirFrequenceCliLED	(UINT_32 frequence);										// Permet de d�finir la fr�quence de clignotement de la LED rouge
UINT_32 	ObtenirFrequenceCliLED	(void);															// Permet de r�cup�rer la frequence de clignotement de la LED rouge

BOOL 			ModifierFrequenceRoues	(UCHAR caractere);										// Permet de modifier la fr�quence des roues (UART)
void 			DefinirVitesseRoues		(UINT_64 frequence);									// D�finition de la p�riode du signal des roues

void 			ArretRoues				(void);															// Permet d'arr�ter les roues du robot
void 			IncrementerVitesseRoues	(void);															// Permet d'augmenter la vitesse du robot
BOOL 			DecrementerVitesseRoues	(void);															// Permet de r�duire la vitesse du robot



#endif /* TIMER_H_ */
