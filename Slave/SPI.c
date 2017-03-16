#include <msp430g2231.h>
#include "SPI.h"



void InitSPI(void){
	USICTL0 |= USISWRST;                 // start config
	USICTL0 |= USIPE7 + USIPE6 + USIPE5 + USIOE; // Port, SPI slave
	USICTL1 |= USIIE;                     // Counter interrupt, flag remains set

	USICNT = 8;                           // init-load counter
	//USICTL1 = USICKPL;                  // Setup clock polarity --> keep low
	USICTL1 = USICKPH;					// phase
	USICTL0 &= ~USISWRST;                 // USI released for operation stop config
}

void Send_char_SPI(unsigned char carac)
{
	USISRL = carac;
	USICNT = 8;

	while (USICNT != 0) ;

	USICNT = 8;
}

unsigned char listen_SPI(){
	while((USICTL1 & USIIFG) != BIT0); // Scrutation

	return USISRL;
}
