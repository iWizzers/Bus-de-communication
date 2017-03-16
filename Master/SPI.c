#include <msp430g2553.h>
#include "SPI.h"
#include "GPIO.h"
#include "global.h"



unsigned char MST_Data, SLV_Data;


char * InitSPI(void)
{
	P1DIR |= BIT4;
	P1OUT |= BIT4;
	P1SEL |= (BIT5 + BIT6 + BIT7);                 // P1.1 = RXD, P1.2=TXD
	P1SEL2 |= (BIT5 + BIT6 + BIT7);                // P1.1 = RXD, P1.2=TXD

	UCB0CTL1 |= UCSSEL_2;
	UCB0BR0 = 104;                          // 1MHz, 9600
	UCB0BR1 = 0;                            // 1MHz, 9600

	UCB0CTL0 = 0 | (UCMSB | UCMST);
	UCB0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**


	P1OUT &= ~BIT4;                           // Now with SPI signals initialized,
	P1OUT |= BIT4;                            // reset slave

	__delay_cycles(75);                 // Wait for slave to initialize


	return "OK";
}



unsigned char TXSPI(unsigned char donnee)
{
	unsigned char retour;


	ActiverGPIO(CS, false); //CS low


	while (!(IFG2 & UCB0TXIFG)) ;  				// USCI_A0 TX buffer ready? --wait flag
	UCB0TXBUF = donnee;              				// TX -> RXed character


	__delay_cycles(50);


	/*while (!(IFG2 & UCB0TXIFG)) ;  				// USCI_A0 TX buffer ready? --wait flag
	UCB0TXBUF = 0;              				// TX -> RXed character


	__delay_cycles(50);                     // Add time between transmissions to make sure slave can keep up*/


	while (!(IFG2 & UCB0RXIFG)) ; 				//receptionUSCI_A0 ? --wait flag
	retour = UCB0RXBUF;


	__delay_cycles(50);


	ActiverGPIO(CS, true); //CS high


	return retour;
}
