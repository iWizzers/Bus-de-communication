#ifndef GPIO_H_
#define GPIO_H_



#include "typedef.h"



void 	InitGPIO				(void);							// Initialisation des entrées-sorties

void 	ActiverGPIOPort1		(UCHAR bit, BOOL etat);	// Activation du bit de sortie du port 1
BOOL	ObtenirEtatGPIOPort1	(UCHAR bit);			// Récupération de l'état du bit de sortie du port 1

void 	ActiverGPIOPort2		(UCHAR bit, BOOL etat);	// Activation du bit de sortie du port 2
BOOL	ObtenirEtatGPIOPort2	(UCHAR bit);			// Récupération de l'état du bit de sortie du port 2



#endif /* GPIO_H_ */
