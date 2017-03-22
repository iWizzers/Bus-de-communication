#ifndef SPI_H_
#define SPI_H_



char * 			InitSPI					(void);						// Initialisation du SPI

void 			TXSPI					(unsigned char donnee);		// Transmission d'un caractère
unsigned char 	RXSPI					(void);						// Réception d'un caractère

void 			ArreterCommunicationSPI	(unsigned char caractere);	// Permet d'arrêter la communication SPI



#endif /* SPI_H_ */
