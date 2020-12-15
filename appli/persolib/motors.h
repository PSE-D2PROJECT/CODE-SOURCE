/*
 * motors.h
 *
 *  Created on: 14 nov. 2020
 *      Author: simon
 */

#ifndef PERSOLIB_MOTORS_H_
#define PERSOLIB_MOTORS_H_
#include "stm32f1xx_hal.h"
#include "macro_types.h"

// FUNCTIONS
void forward(void);
void backward(void);
void leftStatic(void);
void leftDynamic(void);
void rightStatic(void);
void rightDynamic(void);
void backwardLeftDynamic(void);
void backwardRightDynamic(void);
void stop(void);
void randomMovement(void);

//// DEFINE PINS
//#define MOTOR_GAV_GPIO 							GPIOB
//#define MOTOR_GAV_PIN 							GPIO_PIN_1
//
//#define MOTOR_GAR_GPIO 							GPIOB
//#define MOTOR_GAR_PIN 							GPIO_PIN_2
//
//#define MOTOR_DAV_GPIO 							GPIOB
//#define MOTOR_DAV_PIN 							GPIO_PIN_11
//
//#define MOTOR_DAR_GPIO 							GPIOB
//#define MOTOR_DAR_PIN 							GPIO_PIN_12


#endif /* PERSOLIB_MOTORS_H_ */
