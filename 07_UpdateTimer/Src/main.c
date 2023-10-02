/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdint.h>
#include "stm32f4xx.h"
#include "gpio_driver_hal.h"
#include "stm32_assert.h"
#include "exti_driver_hal.h"

//Definimos un PIN de prueba
GPIO_Handler_t userLed 	= {0}; // PinA5
GPIO_Handler_t btn13	= {0}; //PC13

EXTI_Config_t blinkExti 	=	{0};

/*
 * The main function, where everything happens.
 */
int main (void){

	//Configuramos el pin
	userLed.pGPIOx							= GPIOA;
	userLed.pinConfig.GPIO_PinNumber		= PIN_5;
	userLed.pinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType	= GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed	= GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;

	btn13.pGPIOx							= GPIOB;
	btn13.pinConfig.GPIO_PinNumber			= PIN_13;
	btn13.pinConfig.GPIO_PinMode			= GPIO_MODE_IN;


	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userLed);

	gpio_WritePin(&userLed, SET);

	blinkExti.pGPIOHandler					= &btn13;
	blinkExti.edgeType						= EXTERNAL_INTERRUPT_RISING_EDGE;

	exti_Config(&blinkExti);


	while(1){


	}
}


/*
 * Overwrite function
 */
//void Timer10_Callback(void){
//	gpio_TooglePin(&userLed);
//}
void callback_ExtInt13(void){
	gpio_TooglePin(&userLed);
}

/*
 * Esta función sirve para detectar problemas de parametros
 * incorrectos al momento de ejecutar un programa.
 */
void assert_failed(uint8_t* file, uint32_t line){
	while(1){
		//Problems
	}
}
