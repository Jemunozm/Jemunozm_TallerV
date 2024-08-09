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
#include "exti_driver_hal.h"
#include "timer_driver_hal.h"

//Definimos los pines que se van a utilizar.
GPIO_Handler_t userLed = { 0 }; // PinA5
GPIO_Handler_t userLedA = { 0 }; // PinC6
GPIO_Handler_t userLedB = { 0 }; // PinA7
GPIO_Handler_t userLedC = { 0 }; // PinC8
GPIO_Handler_t userLedD = { 0 }; // PinA9
GPIO_Handler_t userLedE = { 0 }; // PinC9
GPIO_Handler_t userLedF = { 0 }; // PinA7
GPIO_Handler_t userLedG = { 0 }; // PinA8
GPIO_Handler_t userBtn  = { 0 }; // Btn

//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer = { 0 }; // Timer para el blinky PinA5.
Timer_Handler_t displayTimer = { 0 }; // Timer para el 7-segmentos.
Timer_Handler_t refreshTimer = { 0 }; // Timer para refrescar la impresión.

//Definimos las lineas EXTI que vamos a utilizar.
EXTI_Config_t extiBtn = { 0 }; //Exti linea 10 para el sw del encoder.

// Variables de cada led del 7 segmento.
uint8_t pinA = 0;
uint8_t pinB = 0;
uint8_t pinC = 0;
uint8_t pinD = 0;
uint8_t pinE = 0;
uint8_t pinF = 0;
uint8_t pinG = 0;

//Variables auxiliares que nos ayudaran en el codigo
uint8_t boton = 0;
uint8_t dir1 = 0;
uint8_t dirResult = 0;
uint8_t dir0 = 0;
uint8_t Bandera = 0;
uint8_t banderaBoton = 0;
uint16_t periodo = 0;

//llamamos las funciones definidas al final del codigo
void initSys(void);
void write7segments(uint8_t numero);
void writeClean(void);
void suma(uint8_t *conteo);
void resta(uint8_t *conteo);



int main(void) {
	periodo = 1005;
	// llamamosala funcion que cuenta con toda la configuración
	initSys();

	while (1) {

		banderaBoton = gpio_ReadPin(&userBtn);

		if(Bandera){
			if (!banderaBoton){
				if(periodo > 100){
					periodo -= 100;
					displayTimer.TIMx_Config.TIMx_Period = periodo;
					timer_Config(&displayTimer);
					timer_SetState(&displayTimer, TIMER_ON);
				}
				else{
					periodo = 1005;
				}
			}
			gpio_TooglePin(&userLed);
			Bandera = 0;
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
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLed);

	/* Configuramos el PinC6 */
	userLedA.pGPIOx = GPIOC;
	userLedA.pinConfig.GPIO_PinNumber = PIN_6;
	userLedA.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedA.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedA.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedA.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLedA);

	/* Configuramos el PinA7 */
	userLedB.pGPIOx = GPIOA;
	userLedB.pinConfig.GPIO_PinNumber = PIN_7;
	userLedB.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedB.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedB.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedB.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLedB);


	/* Configuramos el PinC8 */
	userLedC.pGPIOx = GPIOC;
	userLedC.pinConfig.GPIO_PinNumber = PIN_8;
	userLedC.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedC.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedC.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedC.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLedC);

	/* Configuramos el PinA9 */
	userLedD.pGPIOx = GPIOA;
	userLedD.pinConfig.GPIO_PinNumber = PIN_9;
	userLedD.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedD.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedD.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedD.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLedD);

	/* Configuramos el PinC9 */
	userLedE.pGPIOx = GPIOC;
	userLedE.pinConfig.GPIO_PinNumber = PIN_9;
	userLedE.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedE.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedE.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedE.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLedE);

	/* Configuramos el PinA6 */
	userLedF.pGPIOx = GPIOA;
	userLedF.pinConfig.GPIO_PinNumber = PIN_6;
	userLedF.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedF.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedF.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedF.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLedF);

	/* Configuramos el PinA8 */
	userLedG.pGPIOx = GPIOA;
	userLedG.pinConfig.GPIO_PinNumber = PIN_8;
	userLedG.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedG.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedG.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedG.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLedG);

	/* Configuramos el Btn pin PC13 */
	userBtn.pGPIOx = GPIOC;
	userBtn.pinConfig.GPIO_PinNumber = PIN_13;
	userBtn.pinConfig.GPIO_PinMode	 = GPIO_MODE_IN;
	gpio_Config(&userBtn);

	//Configuramos los timers

	/* Configuramos el timer del blink (TIM2) */
	blinkTimer.pTIMx = TIM5;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000;
	blinkTimer.TIMx_Config.TIMx_Period = 250;
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;
	timer_Config(&blinkTimer);
	timer_SetState(&blinkTimer, TIMER_ON);

	/* Configuramos el timer del 7-segmentos (TIM4) */
	displayTimer.pTIMx = TIM2;
	displayTimer.TIMx_Config.TIMx_Prescaler = 16000;
	displayTimer.TIMx_Config.TIMx_Period = periodo;
	displayTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	displayTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;
	timer_Config(&displayTimer);
	timer_SetState(&displayTimer, TIMER_ON);

	/* Configuramos el timer que refresca la informacion (TIM10) */
	refreshTimer.pTIMx = TIM10;
	refreshTimer.TIMx_Config.TIMx_Prescaler = 16000;
	refreshTimer.TIMx_Config.TIMx_Period = 3000;
	refreshTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	refreshTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;
	timer_Config(&refreshTimer);
	timer_SetState(&refreshTimer, TIMER_ON);

	//Configuramos las interrupciones externas (EXTI)

	/* Configuramos el EXTI sw que será en la linea 3 */
	extiBtn.pGPIOHandler = &userLedA;
	extiBtn.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiBtn);

}

/*
 * Funcion que recibe como parametro una variable que
 * contiene un numero en binario, para así cambiar unas variables globales
 * con las que escribimos los numeros en el 7 segmentos.
 */
void write7segments(uint8_t numero) {

	// Variables de los bits que se encienden para generar cada numero del 0 al 9
	uint8_t bit0 = 0;
	uint8_t bit0n = 0;
	uint8_t bit1 = 0;
	uint8_t bit1n = 0;
	uint8_t bit2 = 0;
	uint8_t bit2n = 0;
	uint8_t bit3 = 0;

	bit0 = (numero >> 0) & 1;
	bit0n = (~numero >> 0) & 1;
	bit1 = (numero >> 1) & 1;
	bit1n = (~numero >> 1) & 1;
	bit2 = (numero >> 2) & 1;
	bit2n = (~numero >> 2) & 1;
	bit3 = (numero >> 3) & 1;

	/*
	 * Esta configuracion de los pines A,B,C,D,E,F,G,que son los leds que
	 * encienden al encoder, es tomada de un ejemplo en digital donde
	 * se realizan operasiones logicas de todos lo posibles casos en los
	 * que se encienda cada led de acuerdo a cada numero.
	 */
	pinA = (bit3 | bit1) | ((~(bit0 ^ bit2)) & 1);
	pinB = bit2n | ((~(bit1 ^ bit0)) & 1);
	pinC = bit2 | bit1n | bit0;
	pinD = (bit1 & bit0n) | (bit2n & bit0n) | (bit2n & bit1)
			| (bit2 & bit1n & bit0);
	pinE = (bit1 & bit0n) | (bit2n & bit0n);
	pinF = bit3 | (bit2 & bit1n) | (bit2 & bit0n) | (bit1n & bit0n);
	pinG = bit3 | (bit2 ^ bit1) | (bit1 & bit0n);

	gpio_WritePin(&userLedA, pinA);
	gpio_WritePin(&userLedB, pinB);
	gpio_WritePin(&userLedC, pinC);
	gpio_WritePin(&userLedD, pinD);
	gpio_WritePin(&userLedE, pinE);
	gpio_WritePin(&userLedF, pinF);
	gpio_WritePin(&userLedG, pinG);
}
/*
 * Funcion que limpia o resetea los pines del 7 segmentos.
 */
void writeClean(void) {
	gpio_WritePin(&userLedA, RESET);
	gpio_WritePin(&userLedB, RESET);
	gpio_WritePin(&userLedC, RESET);
	gpio_WritePin(&userLedD, RESET);
	gpio_WritePin(&userLedE, RESET);
	gpio_WritePin(&userLedF, RESET);
	gpio_WritePin(&userLedG, RESET);
}
// Funcion que suma 1 a la variable que indiquemos como parametro.
void suma(uint8_t *conteo) {
	*conteo += 1;
}
// Funcion que resta 1 a la variable que indiquemos como parametro
void resta(uint8_t *conteo) {
	*conteo -= 1;
}

void Timer2_Callback(void) {
	Bandera = 1;
}

void Timer5_Callback(void){
}

void Timer4_Callback(void) {

}

void Timer10_Callback(void) {
}

void callback_ExtInt10(void) {
	/*
	 * Cada vez que presionamos el boton asociado a esta interrupción
	 * hacemos un XOR para así cambiar el estado y asu vez la dirección
	 * del encoder.
	 */

}

void callback_ExtInt3(void) {

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
