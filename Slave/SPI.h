#ifndef SPI_H_
#define SPI_H_



#include "typedef.h"



void 			InitSPI							(void);		// Initialisation du SPI

void 			TXSPI							(UCHAR carac);		// Transmission d'un caractère
UCHAR 		RXSPI							(void);			// Réception d'un caractère

void 			ArreterCommunicationSPI			(void);				// Permet d'arrêter la communication SPI
BOOL 			ObtenirEtatCommunicationSPI		(void);				// Permet de récupérer l'état de la communication SPI



#endif /* SPI_H_ */
