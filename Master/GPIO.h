#ifndef GPIO_H_
#define GPIO_H_



#include "typedef.h"



void 	InitGPIO				(void);							// Initialisation des entr�es-sorties

void 	ActiverGPIOPort1		(UCHAR bit, BOOL etat);	// Activation du bit de sortie du port 1
BOOL	ObtenirEtatGPIOPort1	(UCHAR bit);			// R�cup�ration de l'�tat du bit de sortie du port 1

void 	ActiverGPIOPort2		(UCHAR bit, BOOL etat);	// Activation du bit de sortie du port 2
BOOL	ObtenirEtatGPIOPort2	(UCHAR bit);			// R�cup�ration de l'�tat du bit de sortie du port 2



#endif /* GPIO_H_ */
