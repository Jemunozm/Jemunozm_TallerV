/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver_hal.h"
#include "stm32_assert.h"
#include "exti_driver_hal.h"
#include "timer_driver_hal.h"
#include "adc_driver_hal.h"
#include "usart_driver_hal.h"
#include "systick_driver_hal.h"

//Definimos los pines que se van a utilizar.
GPIO_Handler_t userLed = { 0 }; // PinA5
GPIO_Handler_t userButton = { 0 }; // PinC13
GPIO_Handler_t userTXusart = { 0 }; // PA11
GPIO_Handler_t userRXusart = { 0 }; // PA12

Systick_Handler_t userSystick = { 0 };

//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer = { 0 }; // Timer para el blinky PinA5.

//Definimos el pin USART que vamos a utilizar.
USART_Handler_t usart = { 0 };
uint8_t rxData = 0;
char bufferData[64] = { 0 };
char commandBuffer[64] = { 0 };
char bufferPrint[64] = { 0 };

uint8_t sendMsg = 0;

//llamamos las funciones definidas al final del codigo
void initSys(void);

int main(void) {
	// llamamosala funcion que cuenta con toda la configuración
	initSys();
	// mandamos un holamundo de cuando la configuración estácargada
	usart_writeMsg(&usart, "Hola mundo \r");
	while (1) {
		if(sendMsg){
			usart_writeMsg(&usart, "Hola Mundo \n");

			gpio_TooglePin(&userLed);
			delay_ms(300);
			gpio_TooglePin(&userLed);
			delay_ms(300);
			gpio_TooglePin(&userLed);
			delay_ms(300);
			gpio_TooglePin(&userLed);
			delay_ms(300);

			gpio_TooglePin(&userLed);
			delay_ms(100);
			gpio_TooglePin(&userLed);
			delay_ms(100);
			gpio_TooglePin(&userLed);
			delay_ms(100);
			gpio_TooglePin(&userLed);
			delay_ms(100);

			sendMsg = 0;
		}
	}
}

void initSys(void) {

	//Configuramos los pines que se van a utilizar

	/* Configuramos el PinA5 */
	userLed.pGPIOx = GPIOA;
	userLed.pinConfig.GPIO_PinNumber = PIN_5;
	userLed.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_FAST;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLed);

	userButton.pGPIOx = GPIOC;
	userButton.pinConfig.GPIO_PinNumber = PIN_13;
	userButton.pinConfig.GPIO_PinMode = GPIO_MODE_IN;
	userButton.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userButton);

	/* Configuramos el PinA2*/
	userTXusart.pGPIOx = GPIOA;
	userTXusart.pinConfig.GPIO_PinNumber = PIN_2;
	userTXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userTXusart.pinConfig.GPIO_PinAltFunMode = AF7;
	gpio_Config(&userRXusart);

	/* Configuramos el PinA3*/
	userRXusart.pGPIOx = GPIOA;
	userRXusart.pinConfig.GPIO_PinNumber = PIN_3;
	userRXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userRXusart.pinConfig.GPIO_PinAltFunMode = AF7;
	gpio_Config(&userTXusart);

	//Configuramos los timers

	/* Configuramos el timer del blink (TIM2) */
	blinkTimer.pTIMx = TIM2;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000;
	blinkTimer.TIMx_Config.TIMx_Period = 250;
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;
	timer_Config(&blinkTimer);
	timer_SetState(&blinkTimer, TIMER_ON);

	userSystick.pSystick = SysTick;
	userSystick.Systick_Config.Systick_Reload = 16000;
	userSystick.Systick_Config.Systick_IntState = SYSTICK_INT_ENABLE;

	systick_Config(&userSystick);


	//Configuramos el protocolo de comunicación

	/*Configuramos el USART*/
	usart.ptrUSARTx = USART2;
	usart.USART_Config.baudrate = USART_BAUDRATE_115200;
	usart.USART_Config.datasize = USART_DATASIZE_8BIT;
	usart.USART_Config.mode = USART_MODE_RXTX;
	usart.USART_Config.parity = USART_PARITY_NONE;
	usart.USART_Config.stopbits = USART_STOPBIT_1;
	usart.USART_Config.enableIntTX = USART_TX_INTERRUP_DISABLE;
	usart.USART_Config.enableIntRX = USART_RX_INTERRUP_ENABLE;

	usart_Config(&usart);

}

void Timer2_Callback(void) {
	sendMsg = 1;
}

void usart2_RxCallback(void) {
	rxData = usart_getRxData2();
}

/*
 * Esta función sirve para detectar problemas de parametros
 * incorrectos al momento de ejecutar un programa.
 */
void assert_failed(uint8_t *file, uint32_t line) {
	while (1) {
		//Problems
	}
}
