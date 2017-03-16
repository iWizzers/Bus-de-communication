#ifndef GPIO_H_
#define GPIO_H_



#include "typedef.h"



char * 	InitGPIO(void);

void 	ActiverGPIO(unsigned char bit, BOOL etat);
BOOL	ObtenirEtatGPIO(unsigned char bit);



#endif /* GPIO_H_ */
