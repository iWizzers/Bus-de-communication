/* ============================================================================ */
/* Description: Permet de g�rer les conversions analogiques-num�riques          */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "ADC.h"



//************************************************************
// Fonction InitADC
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 char * : chaine de caract�re donnant l'�tat de l'initialisation
//************************************************************
void InitADC(void)
{
	ADC10CTL0 = 0;
	ADC10CTL1 = 0;


	// Choix de la r�f�rence de tension Vcc GND
	// R�f�rence interne active et g�n�rateur � 2,5 Volts  ADC10 actif
	// Les autres bits sont suppos�s � 0
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + REF2_5V + REFON + ADC10ON;


	// Choix du diviseur par 1 pour l'horloge, d�marrage conversion logiciel
	// Horloge de conversion 1MHz, conversion monovoie-monocoup
	ADC10CTL1 = ADC10DIV_0 + ADC10SSEL_2 + SHS_0 + CONSEQ_0;
}



//************************************************************
// Fonction ObtenirResultatConversionADC
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 int : valeur de conversion
//************************************************************
SINT_32 ObtenirResultatConversionADC(UCHAR voie)
{
	ADC10CTL1 = ((SINT_32)voie * 0x1000) + ADC10DIV_0 + ADC10SSEL_2 + SHS_0 + CONSEQ_0 ;
	ADC10CTL0 += ENC + ADC10SC;		// D�but de la conversion


  	while (ADC10CTL1 & ADC10BUSY) {
		// Tant que ADC occup� on attend
	}

	ADC10CTL0 -= ENC;				// Conversion finie alors d�sactivation de la conversion analogique-num�rique


    return ADC10MEM;
}
