#ifndef SPI_H_
#define SPI_H_



char * 			InitSPI					(void);						// Initialisation du SPI

unsigned char 	TXSPI					(unsigned char donnee);		// Transmission et réception d'un caractère

void 			ArreterCommunicationSPI	(unsigned char caractere);	// Permet d'arrêter la communication SPI



#endif /* SPI_H_ */
