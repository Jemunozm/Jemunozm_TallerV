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
GPIO_Handler_t userLed4 	= { 0 }; // PinA9
GPIO_Handler_t userLed5 	= { 0 }; // PinC9
GPIO_Handler_t userLed6 	= { 0 }; // PinA7
GPIO_Handler_t userLed7 	= { 0 }; // PinA8
GPIO_Handler_t userDir 		= { 0 }; // PinB12
GPIO_Handler_t userSwitch 	= { 0 }; // PinA10
GPIO_Handler_t userData 	= { 0 }; // PinB5
GPIO_Handler_t userSWenc 	= { 0 }; // PinB3
GPIO_Handler_t userCKenc 	= { 0 }; // PinB13

//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer 		= { 0 }; // Timer para el blinky PinA5
Timer_Handler_t displayTimer 	= { 0 }; // Timer para el 7-segmentos

//Definimos las lineas EXTI que vamos a utilizar.
EXTI_Config_t swExti = { 0 }; //Exti linea 3 para el sw del encoder
EXTI_Config_t ckExti = { 0 }; //Exti linea 13 para el ck del enconder.

/*
 * The main function, where everything happens.
 */

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
uint8_t dirResult = 0;
uint8_t switch7segment = 0;
uint8_t numero = 0;
uint8_t unidad = 0;
uint8_t decena = 0;

//llamamos las funciones definidas al final del codigo
void write7segments(uint8_t numero);
void suma(uint8_t *conteo);
void resta(uint8_t *conteo);

//Creación de un enum con los cuatro posibles casos del encoder y tarea.

enum {
	restaDerecha = 00, sumaDerecha, sumaIzquierda, restaIzquierda
} casosEncoder;

int main(void) {

	//Configuramos los pines que se van a utilizar

	/* Configuramos el PinA5 */
	userLed.pGPIOx = GPIOA;
	userLed.pinConfig.GPIO_PinNumber = PIN_5;
	userLed.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinC6 */
	userLed1.pGPIOx = GPIOC;
	userLed1.pinConfig.GPIO_PinNumber = PIN_6;
	userLed1.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed1.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed1.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed1.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA7 */
	userLed2.pGPIOx = GPIOA;
	userLed2.pinConfig.GPIO_PinNumber = PIN_7;
	userLed2.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed2.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed2.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed2.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinC8 */
	userLed3.pGPIOx = GPIOC;
	userLed3.pinConfig.GPIO_PinNumber = PIN_8;
	userLed3.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed3.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed3.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed3.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA9 */
	userLed4.pGPIOx = GPIOA;
	userLed4.pinConfig.GPIO_PinNumber = PIN_9;
	userLed4.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed4.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed4.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed4.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinC9 */
	userLed5.pGPIOx = GPIOC;
	userLed5.pinConfig.GPIO_PinNumber = PIN_9;
	userLed5.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed5.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed5.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed5.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA6 */
	userLed6.pGPIOx = GPIOA;
	userLed6.pinConfig.GPIO_PinNumber = PIN_6;
	userLed6.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed6.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed6.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed6.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA8 */
	userLed7.pGPIOx = GPIOA;
	userLed7.pinConfig.GPIO_PinNumber = PIN_8;
	userLed7.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed7.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed7.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed7.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinB12 */
	userDir.pGPIOx = GPIOC;
	userDir.pinConfig.GPIO_PinNumber = PIN_5;
	userDir.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userDir.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userDir.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userDir.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA10 */
	userSwitch.pGPIOx = GPIOA;
	userSwitch.pinConfig.GPIO_PinNumber = PIN_0;
	userSwitch.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userSwitch.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userSwitch.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userSwitch.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLDOWN;

	/* Configuramos el PinB5 */
	userData.pGPIOx 						= GPIOB;
	userData.pinConfig.GPIO_PinNumber 		= PIN_5;
	userData.pinConfig.GPIO_PinMode 		= GPIO_MODE_IN;

	/* Configuramos el PinB3 */
	userSWenc.pGPIOx						= GPIOA;
	userSWenc.pinConfig.GPIO_PinNumber		= PIN_10;
	userSWenc.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;

	/* Configuramos el PinB13 */
	userCKenc.pGPIOx						= GPIOB;
	userCKenc.pinConfig.GPIO_PinNumber 		= PIN_3;
	userCKenc.pinConfig.GPIO_PinMode 		= GPIO_MODE_IN;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userLed);
	gpio_Config(&userLed1);
	gpio_Config(&userLed2);
	gpio_Config(&userLed3);
	gpio_Config(&userLed4);
	gpio_Config(&userLed5);
	gpio_Config(&userLed6);
	gpio_Config(&userLed7);
	gpio_Config(&userDir);
	gpio_Config(&userSwitch);
	gpio_Config(&userData);
	gpio_Config(&userSWenc);
	gpio_Config(&userCKenc);

	//Encendemos el led que nos indica que se cargaron las configuraciones
	gpio_WritePin(&userLed, SET);

	//Configuramos los timers

	/* Configuramos el timer del blink (TIM2) */
	blinkTimer.pTIMx = TIM4;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000;
	blinkTimer.TIMx_Config.TIMx_Period = 250;
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	/* Configuramos el timer del 7-segmentos (TIM4) */
	displayTimer.pTIMx = TIM2;
	displayTimer.TIMx_Config.TIMx_Prescaler = 16000;
	displayTimer.TIMx_Config.TIMx_Period = 10;
	displayTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	displayTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	//Cargamos la configuracion de los timers
	timer_Config(&blinkTimer);
	timer_Config(&displayTimer);

	//Encendemos los timers
	timer_SetState(&blinkTimer, TIMER_ON);
	timer_SetState(&displayTimer, TIMER_ON);

	//Configuramos las interrupciones externas (EXTI)

	/* Configuramos el EXTI sw que será en la linea 3 */
	swExti.pGPIOHandler = &userSWenc;
	swExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;

	/* Configuramos el EXTI ck que será en la linea 13 */
	ckExti.pGPIOHandler = &userCKenc;
	ckExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;

	//Cargamos la configuracion de las interrupciones exxternas (EXTI)
	exti_Config(&swExti);
	exti_Config(&ckExti);

	//Encedemos el LED que nos indica que estamos en modo Directo.
	gpio_WritePin(&userDir, SET);
	while (1) {
		//Actualizamos dos nuestras variables auxiliares.
		unidad = numero%10;
		decena = numero/10;
	}
}

void Timer2_Callback(void) {

	/*
	 * Limpiamos la configuracion que tiene el dual 7 segmentos antes de
	 * escribir, ya sea en el 7 segmento derecho o izquierdo.
	 */
	gpio_WritePin(&userLed1, RESET);
	gpio_WritePin(&userLed2, RESET);
	gpio_WritePin(&userLed3, RESET);
	gpio_WritePin(&userLed4, RESET);
	gpio_WritePin(&userLed5, RESET);
	gpio_WritePin(&userLed6, RESET);
	gpio_WritePin(&userLed7, RESET);

	/*
	 * Aplicaremos un XOR a una variable (switch7segment) la cual
	 * nos ayudará a elegir el caso del switch.
	 */
	switch7segment ^= 1;
	/*
	 * Escribimos el valor switch7segment en el pin userSwitch pin que se encarga
	 * de suichear nuestros transistores para así encender el 7 segmentos
	 * de la derecha (unidades) o el 7 segmentos izquierdo (decenas)
	 */
	gpio_WritePin(&userSwitch, switch7segment);
	/*
	 * Creamos un switch para elegir lo que se va a imprimir
	 * de acuerdo al suicheo hecho anteriormente.
	 * el derecho las unidades (caso 0)
	 * y el izquierdo las decenas (caso 1)
	 */
	switch (switch7segment) {
	case 0: {
		/*
		 * llamamos a la funcion que nos cambia las variables globales
		 * para el numero que ingresamos (unidad en este caso)
		 * y lo imprimimos.
		 */
		write7segments(unidad);
		gpio_WritePin(&userLed1, pinA);
		gpio_WritePin(&userLed2, pinB);
		gpio_WritePin(&userLed3, pinC);
		gpio_WritePin(&userLed4, pinD);
		gpio_WritePin(&userLed5, pinE);
		gpio_WritePin(&userLed6, pinF);
		gpio_WritePin(&userLed7, pinG);

		break;
	}
	case 1: {
		/*
		 * llamamos a la funcion que nos cambia las variables globales
		 * para el numero que ingresamos (unidad en este caso)
		 * y lo imprimimos.
		 */
		write7segments(decena);
		gpio_WritePin(&userLed1, pinA);
		gpio_WritePin(&userLed2, pinB);
		gpio_WritePin(&userLed3, pinC);
		gpio_WritePin(&userLed4, pinD);
		gpio_WritePin(&userLed5, pinE);
		gpio_WritePin(&userLed6, pinF);
		gpio_WritePin(&userLed7, pinG);
		break;
	}
	default: {
		break;
	}
	}
}

void Timer4_Callback(void) {
	gpio_TooglePin(&userLed);
}

void callback_ExtInt10(void) {
	/*
	 * Cada vez que presionamos el boton asociado a esta interrupción
	 * hacemos un XOR para así cambiar el estado y asu vez la dirección
	 * del encoder.
	 */
	dir0 ^= 1;
	gpio_TooglePin(&userDir);
}

void callback_ExtInt3(void) {
	//almacenamos la informacion recibida por los datos
	dir1 = gpio_ReadPin(&userData);

	//ACtualizamos la variable que nos dará la direccion de conteo para los casos.
	dirResult = (dir0 << 1) | (dir1 << 0);
	// verificamos el boton ya que nos indica direccion
	/*
	 * Este switch nos da las posbiles opciones de lectura del encoder
	 * debido a sus interrupciones.
	 * Estos casos estan explicados al inicio con un enum.
	 */

	switch (dirResult) {
	case restaDerecha: {
		if (numero > 0){
			resta(&numero);
		}
		break;
	}
	case sumaDerecha: {
		if (numero != 99){
			suma(&numero);
		}
		break;
	}
	case sumaIzquierda: {
		if (numero != 99){
			suma(&numero);
		}
		break;
	}
	case restaIzquierda: {
		if (numero > 0){
			resta(&numero);
		}
		break;
	}
	default: {
		break;
	}
	}
}

/*
 * Funcion que recibe como parametro una variable que
 * contiene un numero en binario, para así cambiar unas variables globales
 * con las que escribimos los numeros en el 7 segmentos.
 */
void write7segments(uint8_t numero) {
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
