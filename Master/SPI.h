#ifndef SPI_H_
#define SPI_H_



#include "typedef.h"



void 		InitSPI					(void);						// Initialisation du SPI

void 		TXSPI					(UCHAR donnee);		// Transmission d'un caract�re
UCHAR 	RXSPI					(void);						// R�ception d'un caract�re

void 		ArreterCommunicationSPI	(UCHAR caractere);	// Permet d'arr�ter la communication SPI



#endif /* SPI_H_ */
