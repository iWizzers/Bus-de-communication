#ifndef SPI_H_
#define SPI_H_



void 			InitSPI	(void);					// Initialisation du SPI

void 			TXSPI	(unsigned char carac);	// Transmission d'un caract�re

unsigned char 	RXSPI	(void);					// R�ception d'un caract�re



#endif /* SPI_H_ */
