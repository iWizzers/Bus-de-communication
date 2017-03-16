#include <msp430g2553.h>
#include "global.h"
#include "UART.h"
#include "SPI.h"
#include "GPIO.h"
#include "Timer.h"




unsigned char 	led1 = 0;
unsigned int 	frequence = 0,
				menu = MENU_PRINCIPAL;



#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    unsigned char c = UCA0RXBUF;


    if (menu == MENU_PARAMETRE_FREQ_CLI_LED)	// Caractère de '0' à '9'
    {
    	if ((c >= '0') && (c <= '9'))
    	{
    		if (frequence < 32767)
    		{
				frequence = frequence * 10 + ((int)c & 0x0F);

				if (frequence > 32767)
				{
					frequence = 32767;
					TXUART("\rVeuillez saisir une frequence (Hz) : 32767");
				}
				else
				{
					while (!(IFG2&UCA0TXIFG)) ;
					UCA0TXBUF = c;
				}
    		}
    	}
    	else if ((int)c == 127)				// Caractère 'supprimer'
    	{
    		if (frequence > 0)
    		{
				frequence = frequence / 10;

				while (!(IFG2&UCA0TXIFG)) ;
				UCA0TXBUF = c;
			}
    	}
    	else if ((int)c == 13)				// Caractère 'entrée'
    	{
    		DefinirFrequence(frequence);
    		TXUART("\n\nNouvelle frequence definie");
    		frequence = 0;
			menu = MENU_PRINCIPAL;
			DefinirReception(true);
		}
    }
    else
    {
    	while (!(IFG2 & UCA0TXIFG)) ;
		UCA0TXBUF = c;


		TXUART("\n\n");


		/*switch (c)
		{
		case 'h':
			TXUART(	"====== AIDE MENU PRINCIPAL ======\n"
					"[h] - Aide\n"
					"[e] - Eteindre\n"
					"[1] - Allumer/Eteindre la LED\n"
					"[p] - Parametrer la frequence de clignotement de la LED");
			break;
		case 'e':
			TXUART("Extinction de la communication");

			IE2 &= ~UCA0RXIE;	// Disable USCI_A0 RX interrupt
			TACTL &= ~TAIE;

			P1OUT = 0;
			break;
		case '1':
			if (!led1)
			{
				TXUART("Allumage LED 1");
				ActiverGPIO(LED_ROUGE, true);
			}
			else
			{
				TXUART("Extinction LED 1");
				ActiverGPIO(LED_ROUGE, false);
			}

			led1 = ObtenirEtatGPIO(LED_ROUGE);
			break;
		case 'p':
			TXUART("Parametrer la frequence");

			menu = MENU_PARAMETRE_FREQ_CLI_LED;
			break;
		default:
			TXUART(	"Touche non reconnue\n"
					"Saisir 'h' pour afficher l'aide");
		}*/


		TXUART("\n\nSPI envoye : ");
		while (!(IFG2 & UCA0TXIFG)) ;
		UCA0TXBUF = c;
		unsigned char var = TXSPI(c);
		TXUART("\n\nSPI recu : ");
		while (!(IFG2 & UCA0TXIFG)) ;
		UCA0TXBUF = var;



		DefinirReception(true);
    }
}



#pragma vector=TIMER0_A1_VECTOR
__interrupt void TimerInterrupt(void)
{
	if (led1)
	{
		if (ObtenirEtatGPIO(LED_ROUGE))
		{
			ActiverGPIO(LED_ROUGE, false);
		}
		else
		{
			ActiverGPIO(LED_ROUGE, true);
		}
	}

	TACTL &= ~TAIFG; //RAZ TAIFG
}
