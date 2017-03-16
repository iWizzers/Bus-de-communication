#ifndef UART_H_
#define UART_H_



#include "typedef.h"



void 	InitUART(void);

void 	TXUART(char *chaine);

void 	DefinirReception(BOOL etat);
BOOL 	ObtenirReception(void);



#endif /* UART_H_ */
