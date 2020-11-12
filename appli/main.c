/*===========================
 *          INCLUDE
 ==========================*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "macro_types.h"
#include "stm32f1_adc.h"
#include "stm32f1_gpio.h"
#include "stdlib.h"
#include "stdio.h"
#include "stm32f1_timer.h"
#include "tft_ili9341/stm32f1_ili9341.h"

/*===========================
 *  VARIABLES DECLARATION
 ==========================*/

#define GOAL_FIRST_LEVEL					5
#define MAXHEIGHT							220
#define MAXWIDTH							305
#define ADC_PH1								ADC_0
#define ADC_PH2								ADC_1
#define ADC_PH3								ADC_4
#define ADC_PH4 							ADC_8

char GAMELAUNCHED[]="Game launched";
char GAMEINWAIT[]="Game in wait";

int score;
char scoreArray[10];

int goal=GOAL_FIRST_LEVEL;
char goalArray[10];

int16_t PH1VAL;
char PH1VALARRAY[10];
int16_t PH2VAL;
char PH2VALARRAY[10];
int16_t PH3VAL;
char PH3VALARRAY[10];
int16_t PH4VAL;
char PH4VALARRAY[10];

int level;
char levelArray[10];

int remain;
char remainArray[10];

char nonePlayer[10]="N";
char playerArray[10]=" ";

int isLaunch=0;

/*===========================
 *      P. METHODS
 ==========================*/

/*=== LEDS & BUZZER & BP ===*/

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

void writeYellowLed(bool_e b){
	HAL_GPIO_WritePin(LED_YELLOW_GPIO, LED_YELLOW_PIN, b);
}

void blinkYellowLed(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_YELLOW_GPIO, LED_YELLOW_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO, LED_YELLOW_PIN, 0);
		HAL_Delay(delay);
	}
}

void blinkYellowLedWithBuzzer(int cpt, uint32_t delay){
	for(int i=0; i<cpt; i++){
		HAL_GPIO_WritePin(LED_YELLOW_GPIO, LED_YELLOW_PIN, 1);
		HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, 1);
		HAL_Delay(delay);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO, LED_YELLOW_PIN, 0);
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
	HAL_GPIO_WritePin(LED_YELLOW_GPIO, LED_YELLOW_PIN, 1);
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 1);
}

void allLedOff(void){
	HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_PIN, 0);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO, LED_YELLOW_PIN, 0);
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, 0);
}

/*=== SCREEN ===*/

void displayLeftPart(){
	ILI9341_Puts(20, 50, "Level:", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(20, 90, "Player:", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(20, 130, "Score:", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(20, 170, "Remain:", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(20, 210, "Goal:", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void displayRightPart(){
	ILI9341_DrawLine(152, 40, 152, 220, ILI9341_COLOR_BLACK);
	ILI9341_Puts(162, 60, "See results on", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(172, 100, "192.168.1.42", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_DrawLine(160, 130, 300, 130, ILI9341_COLOR_BLACK);
	ILI9341_Puts(180, 140, "Robot name", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(185, 170, "PSE 20/21", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(162, 200, "Denis/Duperray", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void displayData(void){
	if(level!=0 && remain != 0 && score != 0 && goal != 0){
		itoa(level, levelArray, 10);
		itoa(remain, remainArray, 10);
		itoa(score, scoreArray, 10);
		itoa(goal, goalArray, 10);
		ILI9341_Puts(105, 50, levelArray, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		ILI9341_Puts(105, 90, nonePlayer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		ILI9341_Puts(105, 130, scoreArray, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		ILI9341_Puts(105, 170, remainArray, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	}
}

void displayPlayer(void){
	ILI9341_Puts(105, 90, playerArray, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void displayLevel(int level){
	itoa(level, levelArray, 10);
	ILI9341_Puts(105, 50, levelArray, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void displayGoal(int goal){
	itoa(goal, goalArray, 10);
	ILI9341_Puts(105, 210, goalArray, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void basicData(void){
	level=0;
	remain=0;
	goal=0;
}

void updateData(void){
	level=2;
	remain=27;
}

void displayInterface(){
	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	setGameStatus(isLaunch);
	displayLeftPart();
//	displayRightPart();
}

void setGameStatus(bool_e b){
	// clear "d"
	ILI9341_Puts(234, 10, " ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	if(b)
		ILI9341_Puts(105, 10, GAMELAUNCHED, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	else
		ILI9341_Puts(105, 10, GAMEINWAIT, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void updatePhotoValCont(void){
	itoa(ADC_getValue(ADC_PH1), PH1VALARRAY, 10);
	ILI9341_Puts(105, 50, PH1VALARRAY, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

/*=== GAME ===*/

void gameTimed(void){
	// int second;
	uint16_t currentS=0;
	updateData();
	displayData();
	score=0;
	writeYellowLed(1);
	displayPlayer();
	displayLevel(2);
	displayGoal(5);
	TIMER_run_us(TIMER1_ID, 10000000, 0);
	while(score<GOAL_FIRST_LEVEL || currentS<(uint16_t)5000000){
		if(ADC_getValue(ADC_PH1)>3000){
			score++;
			writeGreenLed(1);
			HAL_Delay(500);
		}
		writeGreenLed(0);
		ILI9341_Puts(105, 130, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		currentS = TIMER_read(TIMER1_ID);
		remain=(5000000/1000000)-((10*currentS)/38600);
		ILI9341_Puts(105, 170, itoa(remain, remainArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		if(score>=GOAL_FIRST_LEVEL || currentS>(uint16_t)5000000){
			// win/lost scren ?
			setGameStatus(0);
			if(score>GOAL_FIRST_LEVEL)
				win();
			else
				lost();
			// clean score & remain data on screen
			ILI9341_Puts(105, 130, "  ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			ILI9341_Puts(105, 170, "  ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			ILI9341_Puts(105, 210, "  ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			ILI9341_Puts(105, 50, "  ", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			setToZero();
			allLedOff();
			break;
		}
	}
}

void win(void){
	blinkGreenLedWithBuzzer(4, 200);
}

void lost(void){
	blinkRedLedWithBuzzer(4, 200);
}

void setToZero(void){
	level=0;
	remain=0;
	score=0;
	goal=0;
}

void deleteMatchData(void){
	score=0;
	remain=0;
}

bool_e testTimer(void){
	TIMER_run_us(TIMER1_ID, 10000000, 0);
	char timerArray[20];
	while(1){
		ILI9341_Puts(105, 130, itoa(TIMER_read(TIMER1_ID), timerArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		writeGreenLed(1);
		if(TIMER_read(TIMER1_ID)>(uint16_t)5000000){
			stop_timer(TIMER1_ID);
			writeGreenLed(0);
			break;
		}
	}
	writeRedLed(1);
	writeGreenLed(0);
	return 1;
}

void gameCombinaison(void){
	uint16_t currentValueTimer2=0;
//	uint16_t currentValueSubTimer3=0;
	TIMER_run_us(TIMER2_ID, 10000000, 0);
	while(currentValueTimer2<(uint16_t)7000000){
		int randomChoice = rand() % 1;
//		switch(randomChoice){
//			case 0:
//				blinkGreenLed(1, 300);
//				HAL_Delay(500);
////				TIMER_RUN_US(TIMER3_ID, 10000000, 0);
////				while(ADC_getValue(ADC_PH1)<3000 || currentValueSubTimer3<6){
////					if(ADC_getValue(ADC_PH1)>3000){
////						combScore++;
////						stop_timer(TIMER3_ID);
////						break;
////					} else if(ADC_getValue(ADC_PH2)<3000){
////						combScore--;
////						stop_timer(TIMER3_ID);
////						break;
////					}
////				}
//				break;
//			case 1:
//				blinkGreenLed(2, 300);
//				HAL_Delay(500);
//				break;
//			default: break;
//		}
		currentValueTimer2 = TIMER_read(TIMER2_ID);
		if(currentValueTimer2>=7000){
			blinkRedLed(10, 200);
			break;
		}
	}
}

/*=== GAME LEOPOLD ===*/
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
	uint32_t rTimer = (rand() % 2000) + 500;					// dur�e al�atoire de 0,5 � 2 secondes
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

/*===========================
 *       V1-FEATURES
 ==========================*/
void LG1(void){
	uint16_t currentS=0;
	score=0;
	TIMER_run_us(TIMER1_ID, 10000000, 0);
	while(score<GOAL_FIRST_LEVEL || currentS<(uint16_t)5000000){
		if(ADC_getValue(ADC_PH1)>3000 || ADC_getValue(ADC_PH2)>3000 || ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
			score++;
			writeGreenLed(1);
			HAL_Delay(500);
		}
		writeGreenLed(0);
		ILI9341_Puts(105, 130, itoa(score, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		currentS = TIMER_read(TIMER1_ID);
		remain=(5000000/1000000)-((10*currentS)/38600);
		ILI9341_Puts(105, 170, itoa(remain, remainArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		if(score>=GOAL_FIRST_LEVEL || currentS>(uint16_t)5000000){
			// win/lost scren ?
			setGameStatus(0);
			if(score>GOAL_FIRST_LEVEL)
				win();
			else
				lost();
			setToZero();
			allLedOff();
			break;
		}
	}
}


/*===========================
 *         MAIN
 ==========================*/

int main(void) {
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer) (1ere ligne !!)
	HAL_Init();

	//Initialisation de l'UART2 ï¿½ la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
	//Attention, les pins PA2 et PA3 ne sont pas reliï¿½es jusqu'au connecteur de la Nucleo.
	//Ces broches sont redirigï¿½es vers la sonde de dï¿½bogage, la liaison UART ï¿½tant ensuite encapsulï¿½e sur l'USB vers le PC de dï¿½veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le pï¿½riphï¿½rique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	/*===========================
	 *   PIN INITIALISATION
	 ==========================*/
	// STATUS_LED
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(LED_YELLOW_GPIO, LED_YELLOW_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(LED_RED_GPIO, LED_RED_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	// BUZZER
	BSP_GPIO_PinCfg(BUZZER_GPIO, BUZZER_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	// BP
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	// MOTORS
	BSP_GPIO_PinCfg(MOTOR_GAV_GPIO, MOTOR_GAV_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(MOTOR_GAR_GPIO, MOTOR_GAR_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(MOTOR_DAV_GPIO, MOTOR_DAV_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(MOTOR_DAR_GPIO, MOTOR_DAR_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	writeBuzzer(0);

//	displayInterface();

	ADC_init();

	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
	ILI9341_Fill(ILI9341_COLOR_WHITE);

	while(1)
	{
		if(ADC_getValue(ADC_PH1)>3000 || ADC_getValue(ADC_PH2)>3000 || ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
//			setGameStatus(1);
			gameTimed();
		}
//		displayData();

//		ILI9341_Puts(105, 50, itoa(ADC_getValue(ADC_PH1), PH1VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
//		ILI9341_Puts(105, 90, itoa(ADC_getValue(ADC_PH2), PH2VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
//		ILI9341_Puts(105, 130, itoa(ADC_getValue(ADC_PH3), PH3VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
//		ILI9341_Puts(105, 170, itoa(ADC_getValue(ADC_PH4), PH4VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);


	}
}
