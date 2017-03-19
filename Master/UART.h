#ifndef UART_H_
#define UART_H_



#include "typedef.h"



void 	InitUART			(void);			// Initialisation du SPI

void 	TXUART				(char *chaine);	// Transmission d'une chaine de caract�res

void 	DefinirReception	(BOOL etat);	// D�finition de l'�tat de la r�ception
BOOL 	ObtenirReception	(void);			// R�cup�ration de l'�tat de la r�ception



#endif /* UART_H_ */
