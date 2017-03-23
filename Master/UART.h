#ifndef UART_H_
#define UART_H_



#include "typedef.h"



void 	InitUART						(void);					// Initialisation du SPI

void 	TXStringUART					(char * chaine);			// Transmission d'une chaine de caract�res
void 	TXCharUART						(UCHAR carac);	// Transmission d'un caract�re

void 	DefinirReceptionUART			(BOOL etat);			// D�finition de l'�tat de la r�ception
BOOL 	ObtenirReceptionUART			(void);					// R�cup�ration de l'�tat de la r�ception

void 	ArreterCommunicationUART		(void);					// Permet d'arr�ter la communication UART
BOOL 	ObtenirEtatCommunicationUART	(void);					// Permet de r�cup�rer l'�tat de la communication UART



#endif /* UART_H_ */
