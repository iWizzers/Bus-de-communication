/* ============================================================================ */
/* Description: A l'aide du MSP430G2231, il est possible d'effectuer une        */
/* communication SPI. De plus, un servomoteur effectue une rotation de 180�     */
/* afin qu'un capteur puisse d�tecter un obstacle.                              */
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
// G�n�ration nombre al�atoire
#include <stdlib.h>



// Prototypes
int GenererNombre(int min, int max);



//************************************************************
// Fonction main
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 NULL
//************************************************************
int main(void)
{
	unsigned int 	i,
					delai,
					etatRobotAleatoire;


    WDTCTL = WDTPW + WDTHOLD;	// Stoppe le watchdog


    // D�finition du Quartz
    BCSCTL1 = CALBC1_1MHZ;		// 1 MHz
    DCOCTL = CALDCO_1MHZ;		// 1 MHz


    // INITIALISATION
    InitGPIO();		// Entr�es-sorties
    InitUART();		// UART


    TXUART("====== Initialisation ======");
	TXUART("\nPWM ...");
	TXUART(InitPWM());		// PWM des roues
    TXUART("\nSPI ...");
    TXUART(InitSPI());		// SPI en master
    TXUART("\nADC ...");
    TXUART(InitADC());		// Conversion analogique-num�rique
    TXUART("\n====== Fin initialisation ======\n\n\n");


    // Exctinction de la LED rouge si l'initialisation est un succ�s
    ActiverGPIOPort1(BIT_LED_ROUGE, false);


    // Activition de l'interruption SPI
    __enable_interrupt();


    TXUART("====== Bienvenue dans la gestion du LaunchPad ! ======\n\n\n"
    		"> MENU PRINCIPAL <\n\n\n");



    // Tant que l'interruption de l'UART est active
    while (IE2 == UCA0RXIE) {
    	while (ObtenirModeRobot() == AUTONOME) {
    		etatRobotAleatoire = GenererNombre(0, 4);

    		delai = GenererNombre(40, 600);

    		for (i = 0; i < delai; i++) {
    			if (ObtenirModeRobot() == AUTONOME) {
    				switch (etatRobotAleatoire) {
					case AVANCE:
						Avancer();
						break;
					case RECULE:
						Reculer();
						break;
					case TOURNE_DROITE:
						TournerDroite();
						break;
					case TOURNE_GAUCHE:
						TournerGauche();
						break;
					default:
						Stop(ARRET);
						break;
					}

					__delay_cycles(25000);
    			} else {
    				break;
    			}
    		}
    	}


    	TXUART("(MSP430G2553) ");

    	// Attend la r�ception d'un caract�re
    	while (ObtenirReception() == false) {
    		if (ObtenirEtatRobot() != ARRET) {
				__delay_cycles(20000);
				Stop(ARRET);
    		}
    	}

    	TXUART("\n\n_______________________________\n\n\n");

    	DefinirReception(false);
    }


	// Arr�t du robot
	while (ObtenirEtatRobot() != ARRET) {
		__delay_cycles(25000);
		Stop(ARRET);
	}

	// D�sactivation de l'interruption des timers
	TA0CTL &= ~TAIE;
	TA1CTL &= ~TAIE;

    // Allumage LED rouge pour signifier la fin du programme
    P1OUT = 0 | BIT_LED_ROUGE;


    return 0;
}



//************************************************************
// Fonction GenererNombre
//
//       Entr�es :
//                 int : nombre minimum
//                 int : nombre maximum
//
//       Sorties :
//                 int : nombre g�n�r� entre min et max
//************************************************************
int GenererNombre(int min, int max)
{
    return (rand() / (double)RAND_MAX ) * (max - min) + min;
}
