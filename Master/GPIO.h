#ifndef GPIO_H_
#define GPIO_H_



#include "typedef.h"



char * 	InitGPIO				(void);							// Initialisation des entr�es-sorties

void 	ActiverGPIOPort1		(unsigned char bit, BOOL etat);	// Activation du bit de sortie du port 1
BOOL	ObtenirEtatGPIOPort1	(unsigned char bit);			// R�cup�ration de l'�tat du bit de sortie du port 1

void 	ActiverGPIOPort2		(unsigned char bit, BOOL etat);	// Activation du bit de sortie du port 2
BOOL	ObtenirEtatGPIOPort2	(unsigned char bit);			// R�cup�ration de l'�tat du bit de sortie du port 2



#endif /* GPIO_H_ */
