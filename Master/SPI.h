#ifndef SPI_H_
#define SPI_H_



char * 			InitSPI					(void);						// Initialisation du SPI

unsigned char 	TXSPI					(unsigned char donnee);		// Transmission et r�ception d'un caract�re

void 			ArreterCommunicationSPI	(unsigned char caractere);	// Permet d'arr�ter la communication SPI



#endif /* SPI_H_ */
