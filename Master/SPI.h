#ifndef SPI_H_
#define SPI_H_



char * 			InitSPI					(void);						// Initialisation du SPI

void 			TXSPI					(unsigned char donnee);		// Transmission d'un caract�re
unsigned char 	RXSPI					(void);						// R�ception d'un caract�re

void 			ArreterCommunicationSPI	(unsigned char caractere);	// Permet d'arr�ter la communication SPI



#endif /* SPI_H_ */
