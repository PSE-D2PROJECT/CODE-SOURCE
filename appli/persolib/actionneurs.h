/*
 * actionneurs.h
 *
 *  Created on: 14 nov. 2020
 *      Author: simon
 */

#ifndef PERSOLIB_ACTIONNEURS_H_
#define PERSOLIB_ACTIONNEURS_H_
#include "stm32f1xx_hal.h"
#include "macro_types.h"

// FUNCTIONS
bool_e readButton(void);
void writeGreenLed(bool_e b);
void blinkGreenLed(int cpt, uint32_t delay);
void blinkGreenLedWithBuzzer(int cpt, uint32_t delay);
void writeBlueLed(bool_e b);
void blinkBlueLed(int cpt, uint32_t delay);
void blinkBlueLedWithBuzzer(int cpt, uint32_t delay);
void writeRedLed(bool_e b);
void blinkRedLed(int cpt, uint32_t delay);
void blinkRedLedWithBuzzer(int cpt, uint32_t delay);
void allIndicatorLedsOff(void);
void toggleIndicatorLedLG2(int PRx, uint32_t delay);
void writeBuzzer(bool_e b);
void toneBuzzer(int cpt);
void allLedOn(void);
void allLedOff(void);

//// DEFINE PINS
//
//// RGB
//#define LED_RED_GPIO				GPIOA
//#define LED_RED_PIN					GPIO_PIN_10
//
//#define LED_BLUE_GPIO				GPIOB
//#define LED_BLUE_PIN				GPIO_PIN_3
//
//#define LED_GREEN_GPIO				GPIOB
//#define LED_GREEN_PIN				GPIO_PIN_5
//// FIN RGB
//
//#define BLUE_BUTTON_GPIO			GPIOC
//#define BLUE_BUTTON_PIN 			GPIO_PIN_13
//
//#define BUZZER_GPIO					GPIOB
//#define BUZZER_PIN					GPIO_PIN_4
//
//#define INDICATOR_LED_I_GPIO		GPIOC
//#define INDICATOR_LED_I_PIN			GPIO_PIN_3
//
//#define INDICATOR_LED_II_GPIO		GPIOC
//#define INDICATOR_LED_II_PIN		GPIO_PIN_2
//
//#define INDICATOR_LED_III_GPIO		GPIOC
//#define INDICATOR_LED_III_PIN		GPIO_PIN_8
//
//#define INDICATOR_LED_IV_GPIO		GPIOC
//#define INDICATOR_LED_IV_PIN		GPIO_PIN_6

#endif /* PERSOLIB_ACTIONNEURS_H_ */
