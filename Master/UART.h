#ifndef UART_H_
#define UART_H_



#include "typedef.h"



void 	InitUART						(void);					// Initialisation du SPI

void 	TXStringUART					(char * chaine);			// Transmission d'une chaine de caractères
void 	TXCharUART						(UCHAR carac);	// Transmission d'un caractère

void 	DefinirReceptionUART			(BOOL etat);			// Définition de l'état de la réception
BOOL 	ObtenirReceptionUART			(void);					// Récupération de l'état de la réception

void 	ArreterCommunicationUART		(void);					// Permet d'arrêter la communication UART
BOOL 	ObtenirEtatCommunicationUART	(void);					// Permet de récupérer l'état de la communication UART



#endif /* UART_H_ */
