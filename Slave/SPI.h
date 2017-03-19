#ifndef SPI_H_
#define SPI_H_



void 			InitSPI	(void);					// Initialisation du SPI

void 			TXSPI	(unsigned char carac);	// Transmission d'un caractère

unsigned char 	RXSPI	(void);					// Réception d'un caractère



#endif /* SPI_H_ */
