/*
 * games.c
 *
 *  Created on: 14 nov. 2020
 *      Author: simon
 */

#include "games.h"
#include "actionneurs.h"
#include "motors.h"
#include "macro_types.h"
#include "stm32f1_timer.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "stm32f1_adc.h"

#define	TIME_LIMIT							5000000
#define LG2_TIME_LIMIT						10000000
#define GOAL_FIRST_LEVEL					5
#define ADC_PH1								ADC_0
#define ADC_PH2								ADC_1
#define ADC_PH3								ADC_4
#define ADC_PH4 							ADC_8

int score;
char scoreArray[10];

int goal=GOAL_FIRST_LEVEL;
char goalArray[10];

//char PH1VALARRAY[10];
//char PH2VALARRAY[10];
//char PH3VALARRAY[10];
//char PH4VALARRAY[10];

int level;
char levelArray[10];

int remain;
char remainArray[10];

char nonePlayer[10]="N";
char playerArray[10]=" ";

int isLaunch=0;

char randomChoiceArray[10];

void LG1(void){
	uint16_t currentS=0;
	score=0;
	TIMER_run_us(TIMER1_ID, 10000000, 0);
	while(score<GOAL_FIRST_LEVEL || currentS<(uint16_t)TIME_LIMIT){
		if(ADC_getValue(ADC_PH1)>3000 || ADC_getValue(ADC_PH2)>3000 || ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
			score++;
			writeGreenLed(1);
			HAL_Delay(500);
		}
		writeGreenLed(0);
		ILI9341_Puts(105, 130, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		currentS = TIMER_read(TIMER1_ID);
		remain=(TIME_LIMIT/1000000)-((10*currentS)/38600);
		ILI9341_Puts(105, 170, itoa(remain, remainArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		if(score>=GOAL_FIRST_LEVEL || currentS>=(uint16_t)TIME_LIMIT){
			if(score>=GOAL_FIRST_LEVEL)
				// win
				toneBuzzer(2);
			else
				// loose
				toneBuzzer(4);
			stop_timer(TIMER1_ID);
			break;
		}
	}
}

void LG2(void){
	uint16_t currentS = 0;
	uint16_t currentSubTimer=0;
	int score=0;
	TIMER_run_us(TIMER1_ID, 10000000, 0);
	while(currentS<(uint16_t)TIME_LIMIT){
		int randomChoice = rand() % 4;
		//ILI9341_Puts(105, 50, itoa(randomChoice, randomChoiceArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		switch(randomChoice){
			case 0:
				HAL_GPIO_WritePin(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, 1);
				TIMER_run_us(TIMER2_ID, 10000000, 0);
				while(currentSubTimer<(uint16_t)LG2_TIME_LIMIT){
					ILI9341_Puts(105, 50, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					if(ADC_getValue(ADC_PH1)>3000){
						HAL_Delay(400);
						score++;
						stop_timer(TIMER2_ID);
						break;
					} else if(ADC_getValue(ADC_PH2)>3000 || ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
						score--;
						HAL_Delay(400);
						stop_timer(TIMER2_ID);
						break;
					}
				}
				HAL_GPIO_WritePin(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, 1);
				break;
			case 1:
				toggleIndicatorLedLG2(randomChoice, 1000);
				break;
			case 2:
				toggleIndicatorLedLG2(randomChoice, 1000);
				break;
			case 3:
				toggleIndicatorLedLG2(randomChoice, 1000);
				break;
			default: break;
		}
		allIndicatorLedsOff();
	}
}

void testLG2(void){

	int score=0;
	TIMER_run_us(TIMER1_ID, 10000000, 0);
	while(TIMER_read(TIMER1_ID)<(uint16_t)LG2_TIME_LIMIT){
		int randomChoice = rand() % 4;
		if(randomChoice==0 || randomChoice==1){
			HAL_GPIO_WritePin(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, 1);
			HAL_GPIO_WritePin(INDICATOR_LED_II_GPIO, INDICATOR_LED_II_PIN, 1);
			TIMER_run_us(TIMER2_ID, 10000000, 0);
			while(TIMER_read(TIMER2_ID)<(uint16_t)2000000){
				if(ADC_getValue(ADC_PH1)>3000 || ADC_getValue(ADC_PH2)>3000){
					score++;
					ILI9341_Puts(105, 50, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					stop_timer(TIMER2_ID);
					toneBuzzer(2);
					break;
				} else if(ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
					score--;
					ILI9341_Puts(105, 50, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					stop_timer(TIMER2_ID);
					toneBuzzer(2);
					break;
				}
				if(TIMER_read(TIMER2_ID)>=(uint16_t)2000000){
					break;
				}
			}
			HAL_Delay(200);
			HAL_GPIO_WritePin(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, 0);
			HAL_GPIO_WritePin(INDICATOR_LED_II_GPIO, INDICATOR_LED_II_PIN, 0);
		} else if(randomChoice==2 || randomChoice==3){
			HAL_GPIO_WritePin(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, 1);
			HAL_GPIO_WritePin(INDICATOR_LED_IV_GPIO, INDICATOR_LED_IV_PIN, 1);
			TIMER_run_us(TIMER2_ID, 10000000, 0);
			while(TIMER_read(TIMER2_ID)<(uint16_t)2000000){
				if(ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
					score++;
					ILI9341_Puts(105, 50, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					stop_timer(TIMER2_ID);
					toneBuzzer(2);
					break;
				} else if(ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
					score--;
					ILI9341_Puts(105, 50, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					stop_timer(TIMER2_ID);
					toneBuzzer(2);
					break;
				}
				if(TIMER_read(TIMER2_ID)>=(uint16_t)2000000){
					break;
				}
			}
			HAL_Delay(200);
			HAL_GPIO_WritePin(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, 0);
			HAL_GPIO_WritePin(INDICATOR_LED_IV_GPIO, INDICATOR_LED_IV_PIN, 0);
		}
		ILI9341_Puts(105, 50, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		if(TIMER_read(TIMER1_ID)>=(uint16_t)LG2_TIME_LIMIT){
			stop_timer(TIMER1_ID);
			break;
		}
	}
	toneBuzzer(5);
}

void PRxMove(void){
	if(ADC_getValue(ADC_PH1)>3000){
		forward();
	} else if(ADC_getValue(ADC_PH2)>3000){
		backward();
	} else if(ADC_getValue(ADC_PH3)>3000){
		rightDynamic();
	} else if(ADC_getValue(ADC_PH4)>3000){
		leftDynamic();
	}
}
