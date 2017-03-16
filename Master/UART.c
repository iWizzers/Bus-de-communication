#include <msp430g2553.h>
#include "UART.h"
#include "global.h"



BOOL reception = false;



void InitUART(void)
{
	BCSCTL1 = CALBC1_1MHZ;      // Set DCO to 1Mhz
	DCOCTL = CALDCO_1MHZ;       // Set DCO to 1Mhz

    P1SEL |= (BIT1 + BIT2);                 // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= (BIT1 + BIT2);                // P1.1 = RXD, P1.2=TXD

    UCA0CTL1 |= UCSSEL_2;                   // SMCLK
    UCA0BR0 = 104;                          // 1MHz, 9600
    UCA0BR1 = 0;                            // 1MHz, 9600

    UCA0CTL0 &= ~UCPEN & ~UCPAR & ~UCMSB;
    UCA0CTL0 &= ~UC7BIT & ~UCSPB & ~UCMODE1;
    UCA0CTL0 &= ~UCMODE0 & ~UCSYNC;
    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**

    IE2 |= UCA0RXIE;                        // Enable USCI_A0 RX interrupt

    __bis_SR_register(GIE);
}



void TXUART(char *chaine)
{
	int i,
		nbOctetsALire = 0;


	for (i = 0; chaine[i]; i++)
	{
		nbOctetsALire++;
	}


	for (i = 0; i < nbOctetsALire; i++)
	{
		while (!(IFG2&UCA0TXIFG)) ;  // USCI_A0 TX buffer ready?
		UCA0TXBUF = chaine[i];              // TX -> RXed character
	}
}



void DefinirReception(BOOL etat)
{
	reception = etat;
}



BOOL ObtenirReception(void)
{
	return reception;
}
