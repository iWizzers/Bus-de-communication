/* ============================================================================ */
/* Description: Permet de g�rer les entr�es et les sorties                      */
/*                                                                              */
/* Auteurs: Micka�l  MERCIER                                                    */
/*          Xingyong ZHAO                                                       */
/* ============================================================================ */



#include <msp430g2553.h>
#include "GPIO.h"
#include "global.h"



//************************************************************
// Fonction InitGPIO : Initialisation des entr�es-sorties
//
//       Entr�es :
//                 NULL
//
//       Sorties :
//                 char * : chaine de caract�re donnant l'�tat de l'initialisation
//************************************************************
char * InitGPIO(void)
{
	// PORT 1
	P1DIR = 0;
	P1DIR |= BIT_LED_ROUGE;	// LED rouge en sortie
	P1DIR |= BIT_CS;		// Permet de s�lectionner l'esclave (MSP430G2553)


	P1OUT = 0;
	P1OUT |= BIT_LED_ROUGE;	// Activation LED rouge pour visualiser l'intialisation
	P1OUT |= BIT_CS;		// Reset esclave


	P1SEL |= BIT_RX_UART | BIT_TX_UART;     				// S�lection de l'UART (RX et TX)
	P1SEL2 |= BIT_RX_UART | BIT_TX_UART;    				// S�lection de l'UART (RX et TX)
	P1SEL |= BIT_CLK_SPI | BIT_SOMI_SPI | BIT_SIMO_SPI;		// S�lection du SPI (clock, sortie et entr�e)
	P1SEL2 |= BIT_CLK_SPI | BIT_SOMI_SPI | BIT_SIMO_SPI;	// S�lection du SPI (clock, sortie et entr�e)


	// PORT 2
	P2DIR = 0;
	P2DIR |= BIT_SENS_MOTEUR_A | BIT_PWM_MOTEUR_A;	// R�glages de la roue A en sortie (sens + PWM)
	P2DIR |= BIT_SENS_MOTEUR_B | BIT_PWM_MOTEUR_B;	// R�glages de la roue B en sortie (sens + PWM)


	// S�lection fonction TA1.1 & TA1.2
	P2SEL |= BIT_PWM_MOTEUR_A | BIT_PWM_MOTEUR_B;
	P2SEL2 &= ~(BIT_PWM_MOTEUR_A | BIT_PWM_MOTEUR_B);
	P2SEL &= ~(BIT_SENS_MOTEUR_A | BIT_SENS_MOTEUR_B);
	P2SEL2 &= ~(BIT_SENS_MOTEUR_A | BIT_SENS_MOTEUR_B);


	return "OK";
}



//************************************************************
// Fonction ActiverGPIOPort1 : Activation du bit de sortie du port 1
//
//       Entr�es :
//                 unsigned char : bit � modifier
//                 BOOL          : activation ou non
//
//       Sorties :
//                 NULL
//************************************************************
void ActiverGPIOPort1(unsigned char bit, BOOL etat)
{
	if (etat == true) {
		P1OUT |= bit;	// On active le bit de sortie
	} else {
		P1OUT &= ~bit;	// On d�sactive le bit de sortie
	}
}



//************************************************************
// Fonction ObtenirEtatGPIOPort1 : R�cup�ration de l'�tat du bit de sortie du port 1
//
//       Entr�es :
//                 unsigned char : bit dont on veut connaitre l'�tat
//
//       Sorties :
//                 BOOL          : �tat de la sortie
//************************************************************
BOOL ObtenirEtatGPIOPort1(unsigned char bit)
{
	BOOL ret;


	if (P1OUT & bit) {	// Si le bit de sortie est actif
		ret = true;		// On renvoie true
	} else {			// Sinon
		ret = false;	// On renvoie false
	}


	return ret;
}



//************************************************************
// Fonction ActiverGPIOPort2 : Activation du bit de sortie du port 2
//
//       Entr�es :
//                 unsigned char : bit � modifier
//                 BOOL          : activation ou non
//
//       Sorties :
//                 NULL
//************************************************************
void ActiverGPIOPort2(unsigned char bit, BOOL etat)
{
	if (etat == true) {
		P2OUT |= bit;	// On active le bit de sortie
	} else {
		P2OUT &= ~bit;	// On d�sactive le bit de sortie
	}
}



//************************************************************
// Fonction ObtenirEtatGPIOPort2 : R�cup�ration de l'�tat du bit de sortie du port 2
//
//       Entr�es :
//                 unsigned char : bit dont on veut connaitre l'�tat
//
//       Sorties :
//                 BOOL          : �tat de la sortie
//************************************************************
BOOL ObtenirEtatGPIOPort2(unsigned char bit)
{
	BOOL ret;


	if (P2OUT & bit) {	// Si le bit de sortie est actif
		ret = true;		// On renvoie true
	} else {			// Sinon
		ret = false;	// On renvoie false
	}


	return ret;
}
