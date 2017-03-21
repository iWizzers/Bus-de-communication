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
#include <stdlib.h>	// G�n�ration nombre al�atoire



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
    InitGPIO();			// Entr�es-sorties
    InitUART();			// UART


    TXStringUART("====== Initialisation ======");
	TXStringUART("\nPWM ...");
	TXStringUART(InitPWM());		// PWM
    TXStringUART("\nSPI ...");
    TXStringUART(InitSPI());		// SPI en master
    TXStringUART("\nADC ...");
    TXStringUART(InitADC());		// Conversion analogique-num�rique
    TXStringUART("\n====== Fin initialisation ======\n\n\n");


    // Exctinction de la LED rouge si l'initialisation est un succ�s
    ActiverGPIOPort1(BIT_LED_ROUGE, false);


    // Activition de l'interruption SPI
    __enable_interrupt();


    TXStringUART("====== Bienvenue dans la gestion du LaunchPad ! ======\n\n\n"
    		"> MENU PRINCIPAL <\n\n\n");



    // Tant que l'interruption de l'UART est active
    while (ObtenirEtatCommunicationUART() == true) {
    	while (ObtenirModeRobot() == AUTONOME) {			// Boucle tant que le robot est en mode autonome
    		etatRobotAleatoire = GenererNombre(0, 4);		// G�n�ration d'un nombre al�atoire entre 0 et 4

    		delai = GenererNombre(50, 500);					// G�n�ration d'un d�lai entre 1 seconde (50) et 10 secondes (500)

    		for (i = 0; i < delai; i++) {					// Boucle en fonction du d�lai
    			if (ObtenirModeRobot() == AUTONOME) {		// Si le robot est en mode autonome
    				switch (etatRobotAleatoire) {			// En fonction du nombre g�n�r�...
					case AVANCE:							// ...il avance
						Avancer();
						break;
					case RECULE:							// ...il recule
						Reculer();
						break;
					case TOURNE_DROITE:						// ...il tourne � droite
						TournerDroite();
						break;
					case TOURNE_GAUCHE:						// ...il tourne � gauche
						TournerGauche();
						break;
					default:								// ...il s'arr�te
						if (ObtenirEtatRobot() != ARRET) {	// Si le robot n'est pas arr�t�
							Stop(ARRET);					// Ralenti le robot
						}
						break;
					}

					__delay_cycles(20000);					// D�lai = 50 Hz
    			} else {									// Sinon le robot est en mode manuel
    				break;									// Sort de la boucle du d�lai
    			}
    		}
    	}


    	TXStringUART("\r(MSP430G2553) ");

    	// Attend la r�ception d'un caract�re
    	while (ObtenirReceptionUART() == false) {
    		if (ObtenirEtatRobot() != ARRET) {	// Si le robot n'est pas arr�t�
				__delay_cycles(20000);			// D�lai = 50 Hz
				Stop(ARRET);					// Ralenti le robot
    		}
    	}


    	DefinirReceptionUART(false);
    }


    return 0;
}



//************************************************************
// Fonction GenererNombre : Permet de g�n�rer un nombre al�atoire
//                          entre deux bornes
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
    return rand() % (max - min) + min;
}
