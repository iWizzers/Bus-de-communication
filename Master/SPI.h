#ifndef SPI_H_
#define SPI_H_



#include "typedef.h"



void 		InitSPI					(void);						// Initialisation du SPI

void 		TXSPI					(UCHAR donnee);		// Transmission d'un caractère
UCHAR 	RXSPI					(void);						// Réception d'un caractère

void 		ArreterCommunicationSPI	(UCHAR caractere);	// Permet d'arrêter la communication SPI



#endif /* SPI_H_ */
