#ifndef UART_H_
#define UART_H_



#include "typedef.h"



void 	InitUART			(void);			// Initialisation du SPI

void 	TXUART				(char *chaine);	// Transmission d'une chaine de caractères

void 	DefinirReception	(BOOL etat);	// Définition de l'état de la réception
BOOL 	ObtenirReception	(void);			// Récupération de l'état de la réception



#endif /* UART_H_ */
