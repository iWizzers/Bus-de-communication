/* ============================================================================ */
/* Description: A l'aide du MSP430G2553, nous pouvons gérer une communication   */
/* UART en plus d'une communication SPI. Selon le caractère envoyé (sur l'UART) */
/* nous gérons l'état d'avancement du robot. De plus, un capteur est présent    */
/* afin de ralentir à la vue d'un obstacle voire s'arrêter.                     */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
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
#include <stdlib.h>	// Génération nombre aléatoire



// Prototypes
int GenererNombre(int min, int max);



//************************************************************
// Fonction main
//
//       Entrées :
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
	BOOL 			quitterModeAutonome;


    WDTCTL = WDTPW + WDTHOLD;	// Stoppe le watchdog


    // Définition du Quartz
    BCSCTL1 = CALBC1_1MHZ;		// 1 MHz
    DCOCTL = CALDCO_1MHZ;		// 1 MHz


    // INITIALISATION
    InitGPIO();			// Entrées-sorties
    InitUART();			// UART


    TXStringUART("====== Initialisation ======");
	TXStringUART("\nPWM ...");
	TXStringUART(InitPWM());		// PWM
    TXStringUART("\nSPI ...");
    TXStringUART(InitSPI());		// SPI en master
    TXStringUART("\nADC ...");
    TXStringUART(InitADC());		// Conversion analogique-numérique
    TXStringUART("\n====== Fin initialisation ======\n\n\n");


    // Exctinction de la LED rouge si l'initialisation est un succès
    ActiverGPIOPort1(BIT_LED_ROUGE, false);


    // Activition de l'interruption SPI
    __enable_interrupt();


    TXStringUART("====== Bienvenue dans la gestion du LaunchPad ! ======\n\n\n"
    		"> MENU PRINCIPAL <\n\n\n");


    TXSPI('z');	// Initialisation du capteur à 90°
    RXSPI();



    // Tant que l'interruption de l'UART est active
    while (ObtenirEtatCommunicationUART() == true) {
    	while (ObtenirModeRobot() == AUTONOME) {			// Boucle tant que le robot est en mode autonome
    		quitterModeAutonome = false;


    		delai = GenererNombre(500, 1000);					// Génération d'un délai entre 10 secondes (500) et 20 secondes (1000)

    		for (i = 0; i < delai; i++) {					// Boucle en fonction du délai
    			if (ObtenirModeRobot() == AUTONOME) {
    				Avancer();
    				__delay_cycles(20000);
    			} else {									// Sinon le robot est en mode manuel
    				quitterModeAutonome = true;
    				break;									// Sort de la boucle du délai
    			}
    		}


    		if (quitterModeAutonome != true) {
				do {
					etatRobotAleatoire = GenererNombre(0, 4);		// Génération d'un nombre aléatoire entre 0 et 4
				} while (etatRobotAleatoire == AVANCE);

				delai = GenererNombre(50, 250);					// Génération d'un délai entre 1 seconde (50) et 5 secondes (250)

				for (i = 0; i < delai; i++) {					// Boucle en fonction du délai
					if (ObtenirModeRobot() == AUTONOME) {		// Si le robot est en mode autonome
						switch (etatRobotAleatoire) {			// En fonction du nombre généré...
						case AVANCE:							// ...il avance
							Avancer();
							break;
						case RECULE:							// ...il recule
							Reculer();
							break;
						case TOURNE_DROITE:						// ...il tourne à droite
							TournerDroite();
							break;
						case TOURNE_GAUCHE:						// ...il tourne à gauche
							TournerGauche();
							break;
						default:								// ...il s'arrête
							if (ObtenirEtatRobot() != ARRET) {	// Si le robot n'est pas arrêté
								Stop(ARRET);					// Ralenti le robot
							}
							break;
						}

						__delay_cycles(20000);					// Délai = 50 Hz
					} else {									// Sinon le robot est en mode manuel
						break;									// Sort de la boucle du délai
					}
				}
    		} else {
    			// Ne fait rien
    		}
    	}


    	TXStringUART("\r(MSP430G2553) ");

    	// Attend la réception d'un caractère
    	while (ObtenirReceptionUART() == false) {
    		if (ObtenirEtatRobot() != ARRET) {	// Si le robot n'est pas arrêté
				__delay_cycles(20000);			// Délai = 50 Hz
				Stop(ARRET);					// Ralenti le robot
    		}
    	}


    	DefinirReceptionUART(false);
    }


    return 0;
}



//************************************************************
// Fonction GenererNombre : Permet de générer un nombre aléatoire
//                          entre deux bornes
//
//       Entrées :
//                 int : nombre minimum
//                 int : nombre maximum
//
//       Sorties :
//                 int : nombre généré entre min et max
//************************************************************
int GenererNombre(int min, int max)
{
    return rand() % (max - min) + min;
}
