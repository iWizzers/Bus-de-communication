/* ============================================================================ */
/* Description: A l'aide du MSP430G2553, nous pouvons g�rer une communication   */
/* UART en plus d'une communication SPI. Selon le caract�re envoy� (sur l'UART) */
/* nous g�rons l'�tat d'avancement du robot. De plus, un capteur est pr�sent    */
/* afin de ralentir � la vue d'un obstacle voire s'arr�ter.                     */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "global.h"
#include "UART.h"
#include "SPI.h"
#include "GPIO.h"
#include "Timer.h"
#include "ADC.h"
#include "Robot.h"



//************************************************************
// Fonction Init
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
void Init(void)
{
	// INITIALISATION
	InitGPIO();			// Entr�es-sorties
	InitUART();			// UART


	TXStringUART("====== Initialisation ======");
	TXStringUART("\nPWM ...");
	InitPWM();		// PWM
	TXStringUART("\nSPI ...");
	InitSPI();		// SPI en master
	TXStringUART("\nADC ...");
	InitADC();		// Conversion analogique-num�rique
	TXStringUART("\n====== Fin initialisation ======\n\n\n");


	// Activition des interruptions
	__enable_interrupt();


	// Initialisation du capteur � 90�
	TXSPI((UCHAR)'z');
	(void)RXSPI();


	TXStringUART(	"====== Bienvenue dans la gestion du Robot RH-5321 ! ======\n\n\n"
					"> MENU PRINCIPAL <\n\n\n");


	// Boot delay = 3 secondes
	__delay_cycles(3000000);


	// Exctinction de la LED rouge si l'initialisation est un succ�s
	ActiverGPIOPort1((UCHAR)BIT_LED_ROUGE, false);
}



//************************************************************
// Fonction main
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 int : retourne le code de fin d'ex�cution
//************************************************************
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;	// Stoppe le watchdog


    // D�finition du Quartz
    BCSCTL1 = CALBC1_1MHZ;		// 1 MHz
    DCOCTL = CALDCO_1MHZ;		// 1 MHz


    Init();


    // Tant que l'interruption de l'UART est active
    while (ObtenirEtatCommunicationUART() == true) {
    	if (ObtenirModeRobot() == (UCHAR)AUTONOME) {	// Boucle tant que le robot est en mode autonome
    		CommandeAutonome();
    	} else {								// Boucle tant que le robot est en mode manuel
    		CommandeManuelle();
    	}
    }


    return 0;
}
