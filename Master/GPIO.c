/* ============================================================================ */
/* Description: Permet de gérer les entrées et les sorties                      */
/*                                                                              */
/* Auteurs: Mickaël  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "GPIO.h"
#include "global.h"



//************************************************************
// Fonction InitGPIO
//
//       Entrées :
//                 NULL
//
//       Sorties :
//                 char * : chaine de caractère donnant l'état de l'initialisation
//************************************************************
char * InitGPIO(void)
{
	// PORT 1
	P1DIR = 0;
	P1DIR |= BIT_LED_ROUGE;	// LED rouge en sortie
	P1DIR |= BIT_CS;		// Permet de sélectionner l'esclave (MSP430G2553)


	P1OUT = 0;
	P1OUT |= BIT_LED_ROUGE;	// Activation LED rouge pour visualiser l'intialisation
	P1OUT |= BIT_CS;		// Reset esclave


	P1SEL |= BIT_RX_UART | BIT_TX_UART;     				// Sélection de l'UART (RX et TX)
	P1SEL2 |= BIT_RX_UART | BIT_TX_UART;    				// Sélection de l'UART (RX et TX)
	P1SEL |= BIT_CLK_SPI | BIT_SOMI_SPI | BIT_SIMO_SPI;		// Sélection du SPI (clock, sortie et entrée)
	P1SEL2 |= BIT_CLK_SPI | BIT_SOMI_SPI | BIT_SIMO_SPI;	// Sélection du SPI (clock, sortie et entrée)


	// PORT 2
	P2DIR = 0;
	P2DIR |= BIT_SENS_MOTEUR_A | BIT_PWM_MOTEUR_A;	// Réglages de la roue A en sortie (sens + PWM)
	P2DIR |= BIT_SENS_MOTEUR_B | BIT_PWM_MOTEUR_B;	// Réglages de la roue B en sortie (sens + PWM)


	// Sélection fonction TA1.1 & TA1.2
	P2SEL |= BIT_PWM_MOTEUR_A | BIT_PWM_MOTEUR_B;
	P2SEL2 &= ~(BIT_PWM_MOTEUR_A | BIT_PWM_MOTEUR_B);
	P2SEL &= ~(BIT_SENS_MOTEUR_A | BIT_SENS_MOTEUR_B);
	P2SEL2 &= ~(BIT_SENS_MOTEUR_A | BIT_SENS_MOTEUR_B);


	return "OK";
}



//************************************************************
// Fonction ActiverGPIOPort1
//
//       Entrées :
//                 unsigned char : bit à modifier
//                 BOOL          : activation ou non
//
//       Sorties :
//                 NULL
//************************************************************
void ActiverGPIOPort1(unsigned char bit, BOOL etat)
{
	if (etat == true) {
		P1OUT |= bit;
	} else {
		P1OUT &= ~bit;
	}
}



//************************************************************
// Fonction ObtenirEtatGPIOPort1
//
//       Entrées :
//                 unsigned char : bit dont on veut connaitre l'état
//
//       Sorties :
//                 BOOL          : état de la sortie
//************************************************************
BOOL ObtenirEtatGPIOPort1(unsigned char bit)
{
	BOOL ret;


	if (P1OUT & bit) {
		ret = true;
	} else {
		ret = false;
	}


	return ret;
}



//************************************************************
// Fonction ActiverGPIOPort2
//
//       Entrées :
//                 unsigned char : bit à modifier
//                 BOOL          : activation ou non
//
//       Sorties :
//                 NULL
//************************************************************
void ActiverGPIOPort2(unsigned char bit, BOOL etat)
{
	if (etat == true) {
		P2OUT |= bit;
	} else {
		P2OUT &= ~bit;
	}
}



//************************************************************
// Fonction ObtenirEtatGPIOPort2
//
//       Entrées :
//                 unsigned char : bit dont on veut connaitre l'état
//
//       Sorties :
//                 BOOL          : état de la sortie
//************************************************************
BOOL ObtenirEtatGPIOPort2(unsigned char bit)
{
	BOOL ret;


	if (P2OUT & bit) {
		ret = true;
	} else {
		ret = false;
	}


	return ret;
}
