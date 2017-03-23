#ifndef ADC_H_
#define ADC_H_



#include "typedef.h"



void 		InitADC							(void);					// Initialisation du convertisseur analogique-numerique

SINT_32 	ObtenirResultatConversionADC	(UCHAR voie);	// Permet de lire le r�sultat de la conversion analogique-num�rique



#endif /* ADC_H_ */
