/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>
#include "usart_driver_hal.h"
#include "gpio_driver_hal.h"
#include "timer_driver_hal.h"
#include "exti_driver_hal.h"
#include "adc_driver_hal.h"

USART_Handler_t usart2 = { 0 };
GPIO_Handler_t usart2t = { 0 };

Timer_Handler_t blinkTimer = { 0 };
GPIO_Handler_t userLed = { 0 };

EXTI_Config_t imprimir = {0};
GPIO_Handler_t user13 = {0};

//USART_Handler_t usart2rx = {0};
GPIO_Handler_t usart2trx = {0};

char bufferMsg[128] = {0};

ADC_Config_t potenciometro = {0};

uint8_t bandera = 0;
uint8_t adcComplete = 0;
uint8_t sendMsg = 0;
uint8_t receivedChar = 0;

void initSys(void);

int main() {
	initSys();
	while (1) {
		if(adcComplete){
			adcComplete = 0;
			sprintf(bufferMsg,"ADC %d\n",potenciometro.adcData);
			usart_writeMsg(&usart2, bufferMsg);
		}
		if (receivedChar){
			if(receivedChar == 'P'){
				usart_writeMsg(&usart2, "Testing, Testing!!\n\r");
			}
			if(receivedChar == 's'){
				usart_writeMsg(&usart2, "make simple ADC \n\r");
				adc_StartSingleConv();
			}
			if(receivedChar == 'C'){
				usart_writeMsg(&usart2, "make continuous ADC \n\r");
				adc_StartContinouosConv();
			}
			if(receivedChar == 'S'){
				usart_writeMsg(&usart2, "stop continuous ADC \n\r");
				adc_StopContinouosConv();
			}
			receivedChar = 0;
		}
	}
	return 0;
}

void initSys(void) {
	/* Configuramos el timer del blink (TIM2) */
	blinkTimer.pTIMx = TIM2;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000;
	blinkTimer.TIMx_Config.TIMx_Period = 250;
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	timer_Config(&blinkTimer);
	timer_SetState(&blinkTimer, SET);

	//Configuramos los pines que se van a utilizar

	/* Configuramos el PinA5 */
	userLed.pGPIOx = GPIOA;
	userLed.pinConfig.GPIO_PinNumber = PIN_5;
	userLed.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	gpio_Config(&userLed);

	usart2.ptrUSARTx = USART2;
	usart2.USART_Config.baudrate = USART_BAUDRATE_230400;
	usart2.USART_Config.datasize = USART_DATASIZE_8BIT;
	usart2.USART_Config.mode = USART_MODE_RXTX;
	usart2.USART_Config.parity = USART_PARITY_NONE;
	usart2.USART_Config.stopbits = USART_STOPBIT_1;
	usart2.USART_Config.enableIntTX = USART_TX_INTERRUP_DISABLE;
	usart2.USART_Config.enableIntRX = USART_RX_INTERRUP_ENABLE;

	usart_Config(&usart2);

	usart2t.pGPIOx = GPIOA;
	usart2t.pinConfig.GPIO_PinNumber = PIN_2;
	usart2t.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	usart2t.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	usart2t.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	usart2t.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	usart2t.pinConfig.GPIO_PinAltFunMode = AF7;

	gpio_Config(&usart2t);

	bufferMsg[0] = 'H';
	bufferMsg[1] = 'o';
	bufferMsg[2] = 'l';
	bufferMsg[3] = 'a';
	bufferMsg[4] = '\n';
	bufferMsg[5] = 0;

	usart_writeMsg(&usart2, bufferMsg);

	user13.pGPIOx = GPIOC;
	user13.pinConfig.GPIO_PinNumber = PIN_13;
	user13.pinConfig.GPIO_PinMode = GPIO_MODE_IN;

	gpio_Config(&user13);

	imprimir.pGPIOHandler = &user13;
	imprimir.edgeType = EXTERNAL_INTERRUPT_FALLING_EDGE;

	exti_Config(&imprimir);

	usart2trx.pGPIOx = GPIOA;
	usart2trx.pinConfig.GPIO_PinNumber = PIN_3;
	usart2trx.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	usart2trx.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	usart2trx.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	usart2trx.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	usart2trx.pinConfig.GPIO_PinAltFunMode = AF7;

	gpio_Config(&usart2trx);

	potenciometro.channel			= CHANNEL_0;
	potenciometro.resolution		= RESOLUTION_12_BIT;
	potenciometro.dataAlignment		= ALIGNMENT_RIGHT;
	potenciometro.samplingPeriod	= SAMPLING_PERIOD_84_CYCLES;
	potenciometro.interrupState		= ADC_INT_ENABLE;
	adc_ConfigSingleChannel(&potenciometro);

}

void Timer2_Callback(void) {
	gpio_TooglePin(&userLed);
	sendMsg = 1;
}

void callback_ExtInt13(void){
	bandera = 1;
}
void usart2_RxCallback(void){
	receivedChar = usart_getRxData2();
}

void adc_CompleteCallback (void){
	adcComplete = 1;
	potenciometro.adcData = adc_GetValue();
}
