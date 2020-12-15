/*
 * actionneurs.c
 *
 *  Created on: 14 nov. 2020
 *      Author: simon
 */

#include "actionneurs.h"
#include "macro_types.h"
#include "config.h"

bool_e readButton(void){
	return HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
}

void writeGreenLed(bool_e b){
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

void blinkGreenLed(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
		HAL_Delay(delay);
	}
}

void blinkGreenLedWithBuzzer(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
		HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
		HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, 0);
		HAL_Delay(delay);
	}
}

void writeBlueLed(bool_e b){
	HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, b);
}

void blinkBlueLed(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, 0);
		HAL_Delay(delay);
	}
}

void blinkBlueLedWithBuzzer(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, 1);
		HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, 0);
		HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, 0);
		HAL_Delay(delay);
		}
}

void writeRedLed(bool_e b){
	HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, b);
}

void blinkRedLed(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, 0);
		HAL_Delay(delay);
	}
}

void blinkRedLedWithBuzzer(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, 1);
		HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, 0);
		HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, 0);
		HAL_Delay(delay);
		}
}

void allIndicatorLedsOff(void){
	HAL_GPIO_WritePin(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, 0);
	HAL_GPIO_WritePin(INDICATOR_LED_II_GPIO, INDICATOR_LED_II_PIN, 0);
	HAL_GPIO_WritePin(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, 0);
	HAL_GPIO_WritePin(INDICATOR_LED_IV_GPIO, INDICATOR_LED_IV_PIN, 0);
}

void toggleIndicatorLedLG2(int PRx, uint32_t delay){
	switch(PRx){
		case 0:
			HAL_GPIO_WritePin(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, 1);
			HAL_Delay(delay);
			HAL_GPIO_WritePin(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, 0);
			break;
		case 1:
			HAL_GPIO_WritePin(INDICATOR_LED_II_GPIO, INDICATOR_LED_II_PIN, 1);
			HAL_Delay(delay);
			HAL_GPIO_WritePin(INDICATOR_LED_II_GPIO, INDICATOR_LED_II_PIN, 0);
			break;
		case 2:
			HAL_GPIO_WritePin(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, 1);
			HAL_Delay(delay);
			HAL_GPIO_WritePin(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, 0);
			break;
		case 3:
			HAL_GPIO_WritePin(INDICATOR_LED_IV_GPIO, INDICATOR_LED_IV_PIN, 1);
			HAL_Delay(delay);
			HAL_GPIO_WritePin(INDICATOR_LED_IV_GPIO, INDICATOR_LED_IV_PIN, 0);
			break;
		default: break;

	}
	HAL_GPIO_WritePin(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, 1);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, 0);
}

void writeBuzzer(bool_e b){
	HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, b);
}

void toneBuzzer(int cpt){
	for(int i=0; i<cpt; i++){
		writeBuzzer(1);
		HAL_Delay(200);
		writeBuzzer(0);
		HAL_Delay(200);
	}
}

void allLedOn(void){
	HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, 1);
	HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, 1);
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
}

void allLedOff(void){
	HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, 0);
	HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, 0);
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
}
