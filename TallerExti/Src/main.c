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
#include "timer_driver_hal.h"

//Definimos los pines que se van a utilizar.
GPIO_Handler_t userLed 		= { 0 }; // PinA5
GPIO_Handler_t userLed1 	= { 0 }; // PinC6
GPIO_Handler_t userLed2 	= { 0 }; // PinA7
GPIO_Handler_t userLed3 	= { 0 }; // PinC8
GPIO_Handler_t userSwitch 	= { 0 }; // PinA10
GPIO_Handler_t userData 	= { 0 }; // PinB5
GPIO_Handler_t userSWenc 	= { 0 }; // PinB3
GPIO_Handler_t userCKenc 	= { 0 }; // PinB13

//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer 		= { 0 }; // Timer para el blinky PinA5

//Definimos las lineas EXTI que vamos a utilizar.
EXTI_Config_t swExti = { 0 }; //Exti linea 3 para el sw del encoder
EXTI_Config_t ckExti = { 0 }; //Exti linea 13 para el ck del enconder.

/*
 * Creación de variables globales para convertir un numero de binario
 * a un numero de 7 segmentos.
 * Teniendo en cuenta una configuración antes vista (primeras semanas de clase)
 */

// Variables de los bits que se encienden para generar cada numero del 0 al 9
uint8_t bit0 = 0;
uint8_t bit0n = 0;
uint8_t bit1 = 0;
uint8_t bit1n = 0;
uint8_t bit2 = 0;
uint8_t bit2n = 0;
uint8_t bit3 = 0;

// Variables de cada led del 7 segmento.
uint8_t pinA = 0;
uint8_t pinB = 0;
uint8_t pinC = 0;
uint8_t pinD = 0;
uint8_t pinE = 0;
uint8_t pinF = 0;
uint8_t pinG = 0;

//Variables auxiliares que nos ayudaran en el codigo
uint8_t dir0 = 0;
uint8_t dir1 = 0;
uint8_t dirR = 0;
uint8_t numero = 0;

//llamamos las funciones definidas al final del codigo
void write7segments(uint8_t numero);
void suma(uint8_t *conteo);
void resta(uint8_t *conteo);

//Creación de un enum con los cuatro posibles casos del encoder y tarea.

enum {
	sumaDerecha = 00, restaIzquierda
} casosEncoder;

void initSys(void);

int main(void) {

	initSys();
	while (1) {

	}
}

void initSys(void) {
	//Configuramos los pines que se van a utilizar

	/* Configuramos el PinA5 */
	userLed.pGPIOx = GPIOA;
	userLed.pinConfig.GPIO_PinNumber = PIN_5;
	userLed.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userLed);

	/* Configuramos el PinC6 */
	userLed1.pGPIOx = GPIOC;
	userLed1.pinConfig.GPIO_PinNumber = PIN_6;
	userLed1.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed1.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed1.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed1.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userLed1);

	/* Configuramos el PinA7 */
	userLed2.pGPIOx = GPIOA;
	userLed2.pinConfig.GPIO_PinNumber = PIN_7;
	userLed2.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed2.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed2.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed2.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userLed2);

	/* Configuramos el PinC8 */
	userLed3.pGPIOx = GPIOC;
	userLed3.pinConfig.GPIO_PinNumber = PIN_8;
	userLed3.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed3.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed3.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed3.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userLed3);

	/* Configuramos el PinA10 */
	userSwitch.pGPIOx = GPIOA;
	userSwitch.pinConfig.GPIO_PinNumber = PIN_0;
	userSwitch.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userSwitch.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userSwitch.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userSwitch.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLDOWN;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userSwitch);

	/* Configuramos el PinB5 */
	userData.pGPIOx 						= GPIOB;
	userData.pinConfig.GPIO_PinNumber 		= PIN_5;
	userData.pinConfig.GPIO_PinMode 		= GPIO_MODE_IN;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userData);

	/* Configuramos el PinB3 */
	userSWenc.pGPIOx						= GPIOA;
	userSWenc.pinConfig.GPIO_PinNumber		= PIN_10;
	userSWenc.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userSWenc);

	/* Configuramos el EXTI sw que será en la linea 3 */
	swExti.pGPIOHandler = &userSWenc;
	swExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;

	//Cargamos la configuracion de la interrupcion externa (EXTI)
	exti_Config(&swExti);

	/* Configuramos el PinB13 */
	userCKenc.pGPIOx						= GPIOB;
	userCKenc.pinConfig.GPIO_PinNumber 		= PIN_3;
	userCKenc.pinConfig.GPIO_PinMode 		= GPIO_MODE_IN;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userCKenc);

	/* Configuramos el EXTI ck que será en la linea 13 */
	ckExti.pGPIOHandler = &userCKenc;
	ckExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	exti_Config(&ckExti);

	//Configuramos los timers

	/* Configuramos el timer del blink (TIM2) */
	blinkTimer.pTIMx = TIM4;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000;
	blinkTimer.TIMx_Config.TIMx_Period = 250;
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;
	//Cargamos la configuracion del timer
	timer_Config(&blinkTimer);
	//Encendemos el timer
	timer_SetState(&blinkTimer, TIMER_ON);

	//Encendemos el led que nos indica que se cargaron las configuraciones
	gpio_WritePin(&userLed, SET);
}

void Timer2_Callback(void) {

}

void Timer4_Callback(void) {
	gpio_TooglePin(&userLed);
}

void callback_ExtInt10(void) {

}

void callback_ExtInt3(void) {
	//almacenamos la informacion recibida por los datos
	dir0 = gpio_ReadPin(&userCKenc);
	dir1 = gpio_ReadPin(&userData);

	dirR = (dir0 << 1) | (dir1 << 0);

}

// Funcion que suma 1 a la variable que indiquemos como parametro.
void suma(uint8_t *conteo) {
	*conteo += 1;
}

// Funcion que resta 1 a la variable que indiquemos como parametro
void resta(uint8_t *conteo) {
	*conteo -= 1;
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
