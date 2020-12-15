/*
 * motors.c
 *
 *  Created on: 14 nov. 2020
 *      Author: simon
 */

#include "motors.h"
#include "actionneurs.h"
#include "macro_types.h"
#include "stm32f1_timer.h"
#include "tft_ili9341/stm32f1_ili9341.h"

void forward(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 0);
}

void backward(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 1);
}

void leftStatic(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 0);
}

void leftDynamic(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 0);
}

void rightStatic(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 0);
}

void rightDynamic(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 0);
}

void backwardLeftDynamic(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 1);
}

void backwardRightDynamic(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 0);
}

void stop(void){
	HAL_GPIO_WritePin(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, 0);
	HAL_GPIO_WritePin(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, 0);
}

void randomMovement(void){
	int rFunction = rand() % 12;
	uint32_t rTimer = (rand() % 2000) + 500;
	if (rFunction == 0 || rFunction == 1 || rFunction == 2){
		forward();
		HAL_Delay(rTimer);
	} else if (rFunction == 3 || rFunction == 4 || rFunction == 5){
		backward();
		HAL_Delay(rTimer);
	} else if (rFunction == 6){
		leftStatic();
		HAL_Delay(rTimer);
	} else if (rFunction == 7){
		rightStatic();
		HAL_Delay(rTimer);
	} else if (rFunction == 8){
		leftDynamic();
		HAL_Delay(rTimer);
	} else if (rFunction == 9){
		rightDynamic();
		HAL_Delay(rTimer);
	} else if (rFunction == 10){
		backwardLeftDynamic();
		HAL_Delay(rTimer);
	} else {
		backwardRightDynamic();
		HAL_Delay(rTimer);
	}
}
