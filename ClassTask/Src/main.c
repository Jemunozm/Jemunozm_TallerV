/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32_assert.h"
#include "gpio_driver_hal.h"

//Headers definition
int add(int x, int y);

// Definimos los pines a utilizar
GPIO_Handler_t userLedC9 = { 0 }; // PinC9
GPIO_Handler_t userLedC6 = { 0 }; // PinC6
GPIO_Handler_t userLedB8 = { 0 }; // PinB8
GPIO_Handler_t userLedA6 = { 0 }; // PinA6
GPIO_Handler_t userLedC7 = { 0 }; // PinC7
GPIO_Handler_t userLedC8 = { 0 }; // PinC8
GPIO_Handler_t userLedA7 = { 0 }; // PinA7
GPIO_Handler_t userLed = { 0 }; // PinA5
GPIO_Handler_t userBoton = { 0 }; //PinC13

//llamamos las funciones definidas al final del codigo
void temporizador(uint8_t *conteo);
void cronometro(uint8_t *conteo);

int main(void) {
	/*
	 * RESPUESTA A LA PRIMERA PREGUNTA DE LA TAREA1b
	 * El primer error es que no estabamos moviendo el valor del registro
	 * tantas veces a la derecha como indica el pin especifico, sino que
	 * estabamos el registro tantas veces a la izquierda como indicaba el pin.
	 * Además que aunque tuvieramos el valor del PIN el registro IDR, podría haber
	 * mas valores activos en 1, entonces hacía falta aplicar una mascara
	 * para solo dejar el valor que deseamos leer.
	 *
	 * Teniendo en cuenta lo anterior mi solucion fue mover ahora si a la
	 * *DERECHA* tantas veces como la ubiccion del PIN, el valor que aparece
	 * en el registro.
	 * pPinHandler->pGPIOx->IDR >> pPinHandler->pinConfig.GPIO_PinNumber
	 * Es decir cambie la direccion del Shift. Por ultimo aplique una mascara
	 * con un AND para dejar el primer bit del IDR, es decir nuestro valor
	 * ya que este se encuentra allí despues de haber aplicado el SHIFT.
	 * Quedando así la linea:
	 * pinValue = (pPinHandler->pGPIOx->IDR >> pPinHandler->pinConfig.GPIO_PinNumber) & 1;
	 */
	/* Configuramos los pines */
	userLed.pGPIOx = GPIOA;
	userLed.pinConfig.GPIO_PinNumber = PIN_5;
	userLed.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/*Configuramos el PinC13*/
	userBoton.pGPIOx = GPIOC;
	userBoton.pinConfig.GPIO_PinNumber = PIN_13;
	userBoton.pinConfig.GPIO_PinMode = GPIO_MODE_IN;

	/*
	 * Configurando pines para el cronometro/temporizador
	 */

	/* Configuramos el PinA7 */
	userLedA7.pGPIOx = GPIOA;
	userLedA7.pinConfig.GPIO_PinNumber = PIN_7;
	userLedA7.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedA7.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedA7.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedA7.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el pinC8 */
	userLedC8.pGPIOx = GPIOC;
	userLedC8.pinConfig.GPIO_PinNumber = PIN_8;
	userLedC8.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedC8.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedC8.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedC8.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el pinC7 */
	userLedC7.pGPIOx = GPIOC;
	userLedC7.pinConfig.GPIO_PinNumber = PIN_7;
	userLedC7.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedC7.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedC7.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedC7.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el pinA6 */
	userLedA6.pGPIOx = GPIOA;
	userLedA6.pinConfig.GPIO_PinNumber = PIN_6;
	userLedA6.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedA6.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedA6.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedA6.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el pinB8 */
	userLedB8.pGPIOx = GPIOB;
	userLedB8.pinConfig.GPIO_PinNumber = PIN_8;
	userLedB8.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedB8.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedB8.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedB8.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el pinC6 */
	userLedC6.pGPIOx = GPIOC;
	userLedC6.pinConfig.GPIO_PinNumber = PIN_6;
	userLedC6.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedC6.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedC6.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedC6.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el pinC9 */
	userLedC9.pGPIOx = GPIOC;
	userLedC9.pinConfig.GPIO_PinNumber = PIN_9;
	userLedC9.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedC9.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedC9.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedC9.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Cargamos la configuracion en los registros que gorbiernan en los puertos */
	gpio_Config(&userLedC9);
	gpio_Config(&userLedC6);
	gpio_Config(&userLedB8);
	gpio_Config(&userLedA6);
	gpio_Config(&userLedC7);
	gpio_Config(&userLedC8);
	gpio_Config(&userLedA7);
	gpio_Config(&userLed);
	gpio_Config(&userBoton);

		gpio_WritePin(&userLed,	SET);
	//	uint8_t boton =	gpio_ReadPin(&userBoton);
	//	gpio_ReadPin(&userLed);
		gpio_TooglePin(&userLed);
	uint8_t conteo = 0;

	while (1) {
		//		boton = gpio_ReadPin(&userBoton);
		//		if (boton==1){
		//			gpio_WritePin(&userLed, SET);
		//		}
		//		else{
		//			gpio_WritePin(&userLed, RESET);
		//		}
		gpio_WritePin(&userLed, SET);
		uint8_t posPin = 0;
		for (uint8_t i = 0; (i < 61 && gpio_ReadPin(&userBoton) == 1); ++i) {

			/*
			 * En este cuadro de variables repetimos por elnumero de pines
			 * el siguiente patron de dos lineas.
			 * En la primera linea creamos una mascara
			 * la cual toma la variable conteo que contiene un numero de 8
			 * bits por lo que cada posicion de ese numero de 8 bits, es
			 * un LED, es decir un pin y para obtener el valor adecuado
			 * hacia ese pin lo que haremos será mover la variable conteo
			 * hacia la derecha la posicion del led y por ultimo filtrar con
			 * un operador "and" para solo tener ese valor especifico ya sea
			 * 1 ó 0.
			 * Luego llamamos a la función gpio_WritePin con los parametros:
			 * primero el pin a editar y luego el valor que tomará ese pin
			 * entre 0 y 1 de acuerdo al numero en binario que se va a mostrar.
			 */
			posPin = (conteo >> 6) & 1; //Movemos la variable conteo 6 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedC9, posPin); //encedemos el PinC9 asociado a nuestro LED que marca el 7mo bit.
			posPin = (conteo >> 5) & 1; //Movemos la variable conteo 5 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedC6, posPin); //encedemos el PinC6 asociado a nuestro LED que marca el 6to bit.
			posPin = (conteo >> 4) & 1; //Movemos la variable conteo 4 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedB8, posPin); //encedemos el PinB8 asociado a nuestro LED que marca el 5to bit.
			posPin = (conteo >> 3) & 1; //Movemos la variable conteo 3 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedA6, posPin); //encedemos el PinA6 asociado a nuestro LED que marca el 4to bit.
			posPin = (conteo >> 2) & 1; //Movemos la variable conteo 2 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedC7, posPin); //encedemos el PinC7 asociado a nuestro LED que marca el 3er bit.
			posPin = (conteo >> 1) & 1; //Movemos la variable conteo 1 vez hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedC8, posPin); //encedemos el PinC8 asociado a nuestro LED que marca el 2do bit.
			posPin = (conteo >> 0) & 1; //Movemos la variable conteo 0 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedA7, posPin); //encedemos el PinA7 asociado a nuestro LED que marca el primer bit.

			/*Como el microcontrolodor va muy rapido y necesitamos que el cronometro cuente de a segundos
			 * utilizaremosun ciclo for con muchas operaciones para demorar al micro leyendo entre conteo.
			 */
			for (uint32_t i = 0; i < 1337755; i++)
				;

			if(conteo == 60){
				conteo = 0;
			}


			/*
			 * Aqui llamamos a la funcion cronometro para que empiece
			 * a contar,es decir sumar un 1 a la variable conteo
			 * por cadapaso por el ciclo, para que vaya de 1 en 1.
			 */
			cronometro(&conteo);
		}
		gpio_WritePin(&userLed, RESET);
		for (uint8_t i = 0; (i < 61 && gpio_ReadPin(&userBoton) == 0); ++i) {

			/*
			 * En este cuadro de variables repetimos por elnumero de pines
			 * el siguiente patron de dos lineas.
			 * En la primera linea creamos una mascara
			 * la cual toma la variable conteo que contiene un numero de 8
			 * bits por lo que cada posicion de ese numero de 8 bits, es
			 * un LED, es decir un pin y para obtener el valor adecuado
			 * hacia ese pin lo que haremos será mover la variable conteo
			 * hacia la derecha la posicion del led y por ultimo filtrar con
			 * un operador "and" para solo tener ese valor especifico ya sea
			 * 1 ó 0.
			 * Luego llamamos a la función gpio_WritePin con los parametros:
			 * primero el pin a editar y luego el valor que tomará ese pin
			 * entre 0 y 1 de acuerdo al numero en binario que se va a mostrar.
			 */
			posPin = (conteo >> 6) & 1; //Movemos la variable conteo 6 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedC9, posPin); //encedemos el PinC9 asociado a nuestro LED que marca el 7mo bit.
			posPin = (conteo >> 5) & 1; //Movemos la variable conteo 5 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedC6, posPin); //encedemos el PinC6 asociado a nuestro LED que marca el 6to bit.
			posPin = (conteo >> 4) & 1; //Movemos la variable conteo 4 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedB8, posPin); //encedemos el PinB8 asociado a nuestro LED que marca el 5to bit.
			posPin = (conteo >> 3) & 1; //Movemos la variable conteo 3 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedA6, posPin); //encedemos el PinA6 asociado a nuestro LED que marca el 4to bit.
			posPin = (conteo >> 2) & 1; //Movemos la variable conteo 2 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedC7, posPin); //encedemos el PinC7 asociado a nuestro LED que marca el 3er bit.
			posPin = (conteo >> 1) & 1; //Movemos la variable conteo 1 vez hacia la derecha y tomamos el ul ultimo valor.
			gpio_WritePin(&userLedC8, posPin); //encedemos el PinC8 asociado a nuestro LED que marca el 2do bit.
			posPin = (conteo >> 0) & 1; //Movemos la variable conteo 0 veces hacia la derecha y tomamos el ultimo valor.
			gpio_WritePin(&userLedA7, posPin); //encedemos el PinA7 asociado a nuestro LED que marca el primer bit.

			/*Como el microcontrolodor va muy rapido y necesitamos que el cronometro cuente de a segundos
			 * utilizaremosun ciclo for con muchas operaciones para demorar al micro leyendo entre conteo.
			 * Elmuchas lo encontré buscando a que velocidad funciona el micro por operación.
			 */
			for (uint32_t i = 0; i < 1337755; i++);



			/*
			 * Aqui llamamos a la funcion temporizador para que empiece
			 * el conteo regresivo, es decir resta un 1 a la variable conteo
			 * por cada paso por el ciclo, para que vaya de 1 en 1.
			 */
			temporizador(&conteo);
			if(conteo == 0){
							conteo = 60;
						}
		}
	}
}

/*
 * Esta función sirve para detectar problemas de parametros
 * incorrectos al momento de ejecutar un programa.
 */
void assert_failed(uint8_t *file, uint32_t line) {
	while (1) {
		//problems...
	}
}

// Funcion que suma 1 a la variable que indiquemos como parametro.
void cronometro(uint8_t *conteo) {
	*conteo += 1;
}

// Funcion que resta 1 a la variable que indiquemos como parametro
void temporizador(uint8_t *conteo) {
	*conteo -= 1;
}

