/* ============================================================================ */
/* Description: Permet de gérer les conversions analogiques-numériques          */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "ADC.h"



//************************************************************
// Fonction InitADC
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 char * : chaine de caractère donnant l'état de l'initialisation
//************************************************************
void InitADC(void)
{
	ADC10CTL0 = 0;
	ADC10CTL1 = 0;


	// Choix de la référence de tension Vcc GND
	// Référence interne active et générateur à 2,5 Volts  ADC10 actif
	// Les autres bits sont supposés à 0
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + REF2_5V + REFON + ADC10ON;


	// Choix du diviseur par 1 pour l'horloge, démarrage conversion logiciel
	// Horloge de conversion 1MHz, conversion monovoie-monocoup
	ADC10CTL1 = ADC10DIV_0 + ADC10SSEL_2 + SHS_0 + CONSEQ_0;
}



//************************************************************
// Fonction ObtenirResultatConversionADC
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 int : valeur de conversion
//************************************************************
SINT_32 ObtenirResultatConversionADC(UCHAR voie)
{
	ADC10CTL1 = ((SINT_32)voie * 0x1000) + ADC10DIV_0 + ADC10SSEL_2 + SHS_0 + CONSEQ_0 ;
	ADC10CTL0 += ENC + ADC10SC;		// Début de la conversion


  	while (ADC10CTL1 & ADC10BUSY) {
		// Tant que ADC occupé on attend
	}

	ADC10CTL0 -= ENC;				// Conversion finie alors désactivation de la conversion analogique-numérique


    return ADC10MEM;
}
