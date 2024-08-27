/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include <string.h>
#include <stm32f4xx.h>
#include "gpio_driver_hal.h"
#include "timer_driver_hal.h"
#include "exti_driver_hal.h"
#include "usart_driver_hal.h"

USART_Handler_t usart2 = { 0 };
GPIO_Handler_t usart2t = { 0 };

Timer_Handler_t blinkTimer = { 0 };
GPIO_Handler_t userLed = { 0 };
GPIO_Handler_t userLed1 = { 0 };

EXTI_Config_t imprimir = {0};
GPIO_Handler_t user13 = {0};

//USART_Handler_t usart2rx = {0};
GPIO_Handler_t usart2trx = {0};

char bufferMsg[128] = {0};


uint8_t bandera = 0;
uint8_t sendMsg = 0;
uint8_t receivedChar = 0;

void initSys(void);
//void analyzeCommand(char *buffer);

int main() {
	// llamamosala funcion que cuenta con toda la configuración
	initSys();
	// mandamos un holamundo de cuando la configuración estácargada
	usart_writeMsg(&usart2, "Hola mundo \r");
	while (1) {

		if (receivedChar){
			if(receivedChar == 'P'){
				usart_writeMsg(&usart2, "Testing, Testing!!\n\r");
			}
			if(receivedChar == 's'){
				usart_writeMsg(&usart2, "Hola \n\r");
			}
			if(receivedChar == 'C'){
				usart_writeMsg(&usart2, "led on \n\r");
				gpio_WritePin(&userLed1, SET);
			}
			if(receivedChar == 'S'){
				usart_writeMsg(&usart2, "led off \n\r");
				gpio_WritePin(&userLed1, RESET);
			}
			receivedChar = 0;
		}
//
//		// Cuando recibimos un dato se levanta la bandera.
//		if (rxData != 0) {
//			/* si este dato pertence a las letras claves de la tarea
//			 * se guardará en un buffer y se levantará otra bandera.
//			 */
//			if (isLetterCode(rxData)) {
//				strcat(commandBuffer, (char*) &rxData);
//				commandFlag = 1;
//			}
//			// bajamos la bandera de cuando se recibe un dato.
//			rxData = 0;
//		}
//		/*
//		 * Si la bandera está activa entraremos al buffer donde guardamos la letra
//		 * y lo comparamos dentro de lafuncion analizeCommand donde tenemos
//		 * las funciones para letra presionada ademas delmensaje que se debe enviar.
//		 */
//		if (commandFlag) {
//			analyzeCommand(commandBuffer);
//			for (int i = 0; i < sizeof(commandBuffer); i++) {
//				commandBuffer[i] = 0;
//			}
//			commandFlag = 0;
//		}
	}
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

	/* Configuramos el PinA9 */
	userLed1.pGPIOx = GPIOA;
	userLed1.pinConfig.GPIO_PinNumber = PIN_9;
	userLed1.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed1.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed1.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed1.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	gpio_Config(&userLed1);

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

	//usart2.transmisionBuffer[0] = 'H';
	bufferMsg[0] = 'H';
	bufferMsg[1] = 'o';
	bufferMsg[2] = 'l';
	bufferMsg[3] = 'a';
	bufferMsg[4] = '\n';
	bufferMsg[5] = 0;

	usart_writeMsg(&usart2, bufferMsg);

	usart2trx.pGPIOx = GPIOA;
	usart2trx.pinConfig.GPIO_PinNumber = PIN_3;
	usart2trx.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	usart2trx.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	usart2trx.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	usart2trx.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	usart2trx.pinConfig.GPIO_PinAltFunMode = AF7;

	gpio_Config(&usart2trx);

	user13.pGPIOx = GPIOC;
	user13.pinConfig.GPIO_PinNumber = PIN_13;
	user13.pinConfig.GPIO_PinMode = GPIO_MODE_IN;

	gpio_Config(&user13);

	imprimir.pGPIOHandler = &user13;
	imprimir.edgeType = EXTERNAL_INTERRUPT_FALLING_EDGE;

	exti_Config(&imprimir);

}


//void analyzeCommand(char *buffer) {
//
//	if (strcmp(commandBuffer, "T") == 0) {
//		usart_writeMsg(&usart, "Communication on! testing!! \n\n");
//	}
//
//	else if (strcmp(commandBuffer, "h") == 0) {
//		usart_writeMsg(&usart, "- press h to help \n\n - press T to testing \n\n - press O to light on led 1 \n\n - press R change state pin \n\n");
//	}
//
//	else if (strcmp(commandBuffer, "O") == 0) {
//		gpio_WritePin(&userLed1, SET);
//	}
//
//	else if (strcmp(commandBuffer, "R") == 0) {
//		gpio_TooglePin(&userLed1);
//	}
//
////	else if (strcmp(commandBuffer, "R") == 0) {
////		if (botonEncoder) {
////			blinkTimer.TIMx_Config.TIMx_Period = 50;
////			timer_Config(&blinkTimer);
////			timer_SetState(&blinkTimer, SET);
////			sprintf(bufferPrint, "new period timer \n\n");
////			usart_writeMsg(&usart, bufferPrint);
////		} else {
////			gpio_TooglePin(&userLed1);
////			sprintf(bufferPrint, "new pin state \n\n");
////			usart_writeMsg(&usart, bufferPrint);
////		}
////	}
//}

/*
 * Función que analiza si el valor que entra pertenece
 * a un grupo de caracteres especificos.
 */
//uint8_t isLetterCode(char caracter) {
//	if (caracter == 'a' || caracter == 'p' || caracter == 'm'
//			|| caracter == 'd') {
//		return 1;
//	}
//	return 0;
//}

void Timer2_Callback(void) {
	gpio_TooglePin(&userLed);
	sendMsg = 1;
}

void callback_ExtInt13(void){
	bandera = 1;
}
void usart2_RxCallback(void){
	receivedChar = usart_getRxData2();
//	rxData = usart_getRxData2();
}

