#ifndef GLOBAL_H_
#define GLOBAL_H_



// Définition du choix du menu
#define MENU_PRINCIPAL				0
#define MENU_PARAMETRE_FREQ_CLI_LED	1



// Définition des fréquences min et max afin de vérifier l'état du robot par le biais de la LED rouge
#define FREQUENCE_MIN_LED_ROBOT		1		// Hz
#define FREQUENCE_MAX_LED_ROBOT		50		// Hz

#define FREQUENCE_MIN_ROUES_ROBOT 	200		// Hz
#define FREQUENCE_MAX_ROUES_ROBOT 	250000	// Hz



// Définition des modes de fonctionnement du robot
#define MANUEL						0
#define AUTONOME					1



// Définition des états du robot
#define ARRET						0
#define AVANCE						1
#define RECULE						2
#define TOURNE_DROITE				3
#define TOURNE_GAUCHE				4



// Définition des distances d'un obstacle
#define DISTANCE_MIN_OBSTACLE		850		// = 4 cm
#define DISTANCE_MAX_OBSTACLE		525		// = 10 cm



// Définition du port 1
#define BIT_LED_ROUGE				BIT0
#define BIT_RX_UART					BIT1
#define BIT_TX_UART					BIT2
#define BIT_CAPTEUR_IR_AVANT		BIT3
#define BIT_CS						BIT4
#define BIT_CLK_SPI					BIT5
#define BIT_SOMI_SPI				BIT6
#define BIT_SIMO_SPI				BIT7



// Définition du port 2
#define BIT_OPTO_COUPLEUR_ROUE_A	BIT0	// Roue gauche
#define BIT_SENS_MOTEUR_A			BIT1
#define BIT_PWM_MOTEUR_A			BIT2
#define BIT_OPTO_COUPLEUR_ROUE_B	BIT3	// Roue droite
#define BIT_PWM_MOTEUR_B			BIT4
#define BIT_SENS_MOTEUR_B			BIT5



#endif /* GLOBAL_H_ */
