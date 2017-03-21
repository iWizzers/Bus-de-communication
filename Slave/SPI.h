#ifndef SPI_H_
#define SPI_H_



#include "typedef.h"



void 			InitSPI							(void);					// Initialisation du SPI

void 			TXSPI							(unsigned char carac);	// Transmission d'un caract�re
unsigned char 	RXSPI							(void);					// R�ception d'un caract�re

void 			ArreterCommunicationSPI			(void);					// Permet d'arr�ter la communication SPI
BOOL 			ObtenirEtatCommunicationSPI		(void);					// Permet de r�cup�rer l'�tat de la communication SPI



#endif /* SPI_H_ */
