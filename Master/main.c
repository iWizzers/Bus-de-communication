#include <msp430g2553.h>
#include "global.h"
#include "UART.h"
#include "SPI.h"
#include "GPIO.h"
#include "Timer.h"



void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT


    InitUART();


    TXUART("====== Initialisation ======");
    TXUART("\nGPIO ...");
	TXUART(InitGPIO());
	TXUART("\nPWM ...");
	TXUART(InitPWM());
    TXUART("\nSPI ...");
    TXUART(InitSPI());
    ActiverGPIO(LED_ROUGE, false);
    TXUART("\n====== Fin initialisation ======\n\n\n");



    /*char i = 'a', recept;


    recept = TXSPI(i);
	TXUART("\nEnvoye : ");
	TXUART(&i);
	TXUART("\nRecu : ");
	TXUART(&recept);
	TXUART("\n");*/

	/*for (i = 'a'; i <= 'm'; i++) {
		recept = TXSPI(i);

		TXUART("\nEnvoye : ");
		TXUART(&i);
		TXUART("\nRecu : ");
		TXUART(&recept);
		TXUART("\n");
	}*/








    TXUART("====== Bienvenue dans la gestion du LaunchPad ! ======\n\n\n"
    		"> MENU PRINCIPAL <\n\n\n");

    while (IE2 == UCA0RXIE) {
    	TXUART("(MSP430G2553) ");
    	//TXUART(menu != MENU_PARAMETRE_FREQ_CLI_LED ? "Votre choix : " : "Veuillez saisir une frequence (Hz) : ");

    	while (!ObtenirReception());
    	DefinirReception(false);

    	TXUART("\n\n_______________________________\n\n\n");
    }
}
