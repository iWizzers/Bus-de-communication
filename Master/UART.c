/* ============================================================================ */
/* Description: Permet de g�rer la fonctionnalit� UART (transmission            */
/* et r�ception)                                                                */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "UART.h"
#include "global.h"



BOOL reception = false;



//************************************************************
// Fonction InitUART
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void InitUART(void)
{
    UCA0BR0 = 104;              // 1MHz, 9600
    UCA0BR1 = 0;                // 1MHz, 9600

    UCA0CTL0 &= ~UCPEN & ~UCPAR & ~UCMSB;
    UCA0CTL0 &= ~UC7BIT & ~UCSPB & ~UCMODE1;
    UCA0CTL0 &= ~UCMODE0 & ~UCSYNC;
    UCA0CTL1 |= UCSSEL_2;       // SMCLK

    UCA0CTL1 &= ~UCSWRST;       // **Initialize USCI state machine**

    IE2 |= UCA0RXIE;            // Enable USCI_A0 RX interrupt
}



//************************************************************
// Fonction TXUART
//
//       Entr�es :
//                 char * : chaine de caract�re � transmettre
//
//       Sorties :
//                 NULL
//************************************************************
void TXUART(char *chaine)
{
	int i,
		nbOctetsALire = 0;


	for (i = 0; chaine[i]; i++) {
		nbOctetsALire++;
	}


	for (i = 0; i < nbOctetsALire; i++) {
		while (!(IFG2 & UCA0TXIFG)) ;	// Attend que le buffer de transmission soit libre
		UCA0TXBUF = chaine[i];       	// Transmission du caract�re
	}
}



//************************************************************
// Fonction DefinirReception
//
//       Entr�es :
//                 BOOL : permet de d�finir l'�tat de l'UART
//
//       Sorties :
//                 NULL
//************************************************************
void DefinirReception(BOOL etat)
{
	reception = etat;
}



//************************************************************
// Fonction DefinirReception
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 BOOL : permet de r�cup�rer l'�tat de l'UART
//************************************************************
BOOL ObtenirReception(void)
{
	return reception;
}
