#ifndef ADC_H_
#define ADC_H_



#include "typedef.h"



void 		InitADC							(void);					// Initialisation du convertisseur analogique-numerique

SINT_32 	ObtenirResultatConversionADC	(UCHAR voie);	// Permet de lire le résultat de la conversion analogique-numérique



#endif /* ADC_H_ */
