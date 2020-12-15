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
#include "persolib/actionneurs.h"
#include "persolib/games.h"
#include "persolib/motors.h"

/*===========================
 *  VARIABLES DECLARATION
 ==========================*/
#define ADC_PH1								ADC_0
#define ADC_PH2								ADC_1
#define ADC_PH3								ADC_4
#define ADC_PH4 							ADC_8

char scoreArray[10];
//char PH1VALARRAY[10];
//char PH2VALARRAY[10];
//char PH3VALARRAY[10];
//char PH4VALARRAY[10];

/*===========================
 *         MAIN
 ==========================*/

int main(void) {
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer) (1ere ligne !!)
	HAL_Init();

	//Initialisation de l'UART2 ï¿½ la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
	//Attention, les pins PA2 et PA3 ne sont pas reliï¿½es jusqu'au connecteur de la Nucleo.
	//Ces broches sont redirigees vers la sonde de dï¿½bogage, la liaison UART etant ensuite encapsulï¿½e sur l'USB vers le PC de dï¿½veloppement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le peripherique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	/*===========================
	 *   PIN INITIALISATION
	 ==========================*/
	// STATUS_LED
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(LED_BLUE_GPIO, LED_BLUE_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(LED_RED_GPIO, LED_RED_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	// INDICATOR LEDS
	BSP_GPIO_PinCfg(INDICATOR_LED_I_GPIO, INDICATOR_LED_I_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(INDICATOR_LED_II_GPIO, INDICATOR_LED_II_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(INDICATOR_LED_III_GPIO, INDICATOR_LED_III_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(INDICATOR_LED_IV_GPIO, INDICATOR_LED_IV_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
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

	ADC_init();

	ILI9341_Init();
	ILI9341_Rotate(ILI9341_Orientation_Landscape_1);
	ILI9341_Fill(ILI9341_COLOR_WHITE);

	// TESTS PHOTORESISTANCES
	uint16_t AD_RES=0, Vamb, DC_Multiplier;
	Vamb=(uint16_t)ADC_getValue(ADC_PH1);
	DC_Multiplier=(uint16_t)65535/(4096-Vamb);

	while(1)
	{

//		if(HAL_GPIO_ReadPin(LED_RED_GPIO, LED_RED_PIN)!=GPIO_PIN_RESET){
//			writeGreenLed(1);
//			writeBlueLed(0);
//		} else {
//			writeGreenLed(0);
//			writeBlueLed(1);
//		}
		writeGreenLed(1);
		AD_RES = (uint16_t)ADC_getValue(ADC_PH1);
		ILI9341_Puts(105, 50, itoa(AD_RES, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
//		ILI9341_Puts(105, 50, itoa((AD_RES-Vamb)*DC_Multiplier, scoreArray, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		HAL_Delay(100);


//		if(ADC_getValue(ADC_PH1)>3000 || ADC_getValue(ADC_PH2)>3000 || ADC_getValue(ADC_PH3)>3000 || ADC_getValue(ADC_PH4)>3000){
//			LG1();
//		}

//		testLG2();


//		ILI9341_Puts(105, 50, itoa(ADC_getValue(ADC_PH1), PH1VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
//		ILI9341_Puts(105, 90, itoa(ADC_getValue(ADC_PH2), PH2VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
//		ILI9341_Puts(105, 130, itoa(ADC_getValue(ADC_PH3), PH3VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
//		ILI9341_Puts(105, 170, itoa(ADC_getValue(ADC_PH4), PH4VALARRAY, 10), &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

	}
}
