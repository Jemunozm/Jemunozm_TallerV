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

//Definimos los pines que se van a utilizar.
GPIO_Handler_t userLed = { 0 }; // PinA5
GPIO_Handler_t userLedA = { 0 }; // PinC6
GPIO_Handler_t userLedB = { 0 }; // PinA7
GPIO_Handler_t userLedC = { 0 }; // PinC8
GPIO_Handler_t userLedD = { 0 }; // PinA9
GPIO_Handler_t userLedE = { 0 }; // PinC9
GPIO_Handler_t userLedF = { 0 }; // PinA7
GPIO_Handler_t userLedG = { 0 }; // PinA8
GPIO_Handler_t userDir = { 0 }; // PinB12
GPIO_Handler_t userSwitch = { 0 }; // PinA10
GPIO_Handler_t userData = { 0 }; // PinB5
GPIO_Handler_t userSWenc = { 0 }; // PinA10
GPIO_Handler_t userCKenc = { 0 }; // PinB3
GPIO_Handler_t userTXusart = { 0 }; // PA11
GPIO_Handler_t userRXusart = { 0 }; // PA12

//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer = { 0 }; // Timer para el blinky PinA5.
Timer_Handler_t displayTimer = { 0 }; // Timer para el 7-segmentos.
Timer_Handler_t refreshTimer = { 0 }; // Timer para refrescar la impresión.

//Definimos las lineas EXTI que vamos a utilizar.
EXTI_Config_t swExti = { 0 }; //Exti linea 10 para el sw del encoder.
EXTI_Config_t ckExti = { 0 }; //Exti linea 3 para el ck del enconder.

//Definimos el ADC que vamos a utilizar.
ADC_Config_t trimmer = { 0 };

//Definimos el pin USART que vamos a utilizar.
USART_Handler_t usart = { 0 };
uint8_t rxData = 0;
char bufferData[64] = { 0 };
char commandBuffer[64] = { 0 };
char bufferPrint[64] = { 0 };

// Variables de cada led del 7 segmento.
uint8_t pinA = 0;
uint8_t pinB = 0;
uint8_t pinC = 0;
uint8_t pinD = 0;
uint8_t pinE = 0;
uint8_t pinF = 0;
uint8_t pinG = 0;

//Variables auxiliares que nos ayudaran en el codigo
uint8_t botonEncoder = 0;
uint8_t dataEncoder = 0;
uint8_t ckEncoder = 0;
uint8_t direccionEncoder = 0;
uint8_t switch7segment = 0;
uint8_t adcResolution = 0;
uint8_t adcChannel = 0;
uint8_t adcChannelaux = 0;
uint8_t channelOption = 0;
uint8_t adcComplete = 0;
uint8_t receivedChar = 0;
uint8_t commandFlag = 0;
uint8_t bandera1 = 0;
uint8_t programStatus = 0;
uint8_t switchTransistores = 0;
uint8_t resolutionOption = 0;

//llamamos las funciones definidas al final del codigo
void initSys(void);
void write7segments(uint8_t numero);
void writePattern(uint8_t pattern);
void writeClean(void);
void suma(uint8_t *conteo);
void resta(uint8_t *conteo);
void analyzeCommand(char *buffer);
uint8_t isLetter(char caracter);
uint8_t isNotControl(char caracter);
uint8_t isLetterCode(char caracter);
void caseEncoder(uint8_t *cases);
void changeResolution(ADC_Config_t *adcConfig, uint8_t resolution);
void changeChannel(ADC_Config_t *adcConfig, uint8_t channel);
uint8_t channelOptions(uint8_t *numero);
uint8_t resolutionOptions(uint8_t *numero);

//Creación de un enum con los cuatro posibles casos del encoder y tarea.

enum {
	restaChannel = 0, sumaChannel, restaResolucion, sumaResolucion
};

enum {
	six = 0, eight, ten, twelve
};

int main(void) {
	initSys();
	usart_writeMsg(&usart, "Hola mundo \r");
	adc_StartSingleConv();
	while (1) {

		adcChannelaux = adcChannel + 1;

		//Actualizamos la variable que nos dará la direccion de conteo para los casos.
		direccionEncoder = (botonEncoder << 1) | (dataEncoder << 0);
		resolutionOption = resolutionOptions(&adcResolution);
		changeResolution(&trimmer, resolutionOption);
		channelOption = channelOptions(&adcChannel);
		changeChannel(&trimmer, channelOption);

		// Realizamos unos determinados pasos cuando la bandera del encoder esté arriba.
		if (ckEncoder) {
			//Bajamos la bandera dela interrupción.
			ckEncoder = 0;
			//almacenamos la informacion recibida por los datos
			dataEncoder = gpio_ReadPin(&userData);
			//Actualizamos la variable que nos dará la direccion de conteo para los casos.
			direccionEncoder = (botonEncoder << 1) | (dataEncoder << 0);

			caseEncoder(&direccionEncoder);
		}
		// Realizamos unos determinados pasos cuando la bandera del timer de impresión esté arriba.
		if (bandera1 && adcComplete) {
			bandera1 = 0;
			adcComplete = 0;
			sprintf(bufferPrint, "ADC %d, Sensor: %d, Resolution: %d \n\n",
					trimmer.adcData, adcChannelaux, adcResolution);
			usart_writeMsg(&usart, bufferPrint);
			adc_StartSingleConv();
		}

		if (rxData != 0) {
			if (isLetterCode(rxData)) {
				strcat(commandBuffer, (char*) &rxData);
				commandFlag = 1;
			}
			rxData = 0;
		}

		if (commandFlag) {
			analyzeCommand(commandBuffer);
			for (int i = 0; i < sizeof(commandBuffer); i++) {
				commandBuffer[i] = 0;
			}
			commandFlag = 0;
		}

		if (switchTransistores) {
			//Bajamos la bandera
			switchTransistores = 0;
			//Limpiamos los valores que se encuentran en el 7 segmentos.
			writeClean();
			gpio_WritePin(&userDir, RESET);
			if (switch7segment) {
				/*
				 * Escribimos el valor switch7segment en el pin userSwitch pin que se encarga
				 * de suichear nuestros transistores
				 * para así encender el 7segmentos izquierdo (resolucion)
				 */
				gpio_WritePin(&userSwitch, switch7segment);
				//Escribimos el numero de la resolucion.
				writePattern(adcResolution);
				/* cambiamos elvalor de switch para que en el proximo evento
				 * del timer este cambie de transistor
				 */
				switch7segment = 0;
				if (botonEncoder) {
					gpio_WritePin(&userDir, SET);
				}
			} else {
				/*
				 * Escribimos el valor switch7segment en el pin userSwitch pin que se encarga
				 * de suichear nuestros transistores
				 * para así encender el 7 segmentos de la derecha (canales).
				 */
				gpio_WritePin(&userSwitch, switch7segment);
				//Escribimos el numero del canal

				write7segments(adcChannelaux);
				/* cambiamos elvalor de switch para que en el proximo evento
				 * del timer este cambie de transistor
				 */
				switch7segment = 1;
				if (!botonEncoder) {
					gpio_WritePin(&userDir, SET);
				}
			}

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

	/* Configuramos el PinC6 */
	userLedA.pGPIOx = GPIOC;
	userLedA.pinConfig.GPIO_PinNumber = PIN_6;
	userLedA.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedA.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedA.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedA.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA7 */
	userLedB.pGPIOx = GPIOA;
	userLedB.pinConfig.GPIO_PinNumber = PIN_7;
	userLedB.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedB.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedB.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedB.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinC8 */
	userLedC.pGPIOx = GPIOC;
	userLedC.pinConfig.GPIO_PinNumber = PIN_8;
	userLedC.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedC.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedC.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedC.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA9 */
	userLedD.pGPIOx = GPIOA;
	userLedD.pinConfig.GPIO_PinNumber = PIN_9;
	userLedD.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedD.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedD.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedD.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinC9 */
	userLedE.pGPIOx = GPIOC;
	userLedE.pinConfig.GPIO_PinNumber = PIN_9;
	userLedE.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedE.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedE.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedE.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA6 */
	userLedF.pGPIOx = GPIOA;
	userLedF.pinConfig.GPIO_PinNumber = PIN_6;
	userLedF.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedF.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedF.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedF.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA8 */
	userLedG.pGPIOx = GPIOA;
	userLedG.pinConfig.GPIO_PinNumber = PIN_8;
	userLedG.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLedG.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedG.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedG.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

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
	userData.pGPIOx = GPIOB;
	userData.pinConfig.GPIO_PinNumber = PIN_5;
	userData.pinConfig.GPIO_PinMode = GPIO_MODE_IN;

	/* Configuramos el PinB3 */
	userSWenc.pGPIOx = GPIOA;
	userSWenc.pinConfig.GPIO_PinNumber = PIN_10;
	userSWenc.pinConfig.GPIO_PinMode = GPIO_MODE_IN;

	/* Configuramos el PinB13 */
	userCKenc.pGPIOx = GPIOB;
	userCKenc.pinConfig.GPIO_PinNumber = PIN_3;
	userCKenc.pinConfig.GPIO_PinMode = GPIO_MODE_IN;

	/* Configuramos el PinA2*/
	userTXusart.pGPIOx = GPIOA;
	userTXusart.pinConfig.GPIO_PinNumber = PIN_11;
	userTXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userTXusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userTXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userTXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userTXusart.pinConfig.GPIO_PinAltFunMode = AF8;

	/* Configuramos el PinA3*/
	userRXusart.pGPIOx = GPIOA;
	userRXusart.pinConfig.GPIO_PinNumber = PIN_12;
	userRXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userRXusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userRXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userRXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userRXusart.pinConfig.GPIO_PinAltFunMode = AF8;

	//Cargamos la configuracion  en los registros que gobiernan el puerto.
	gpio_Config(&userLed);
	gpio_Config(&userLedA);
	gpio_Config(&userLedB);
	gpio_Config(&userLedC);
	gpio_Config(&userLedD);
	gpio_Config(&userLedE);
	gpio_Config(&userLedF);
	gpio_Config(&userLedG);
	gpio_Config(&userDir);
	gpio_Config(&userSwitch);
	gpio_Config(&userData);
	gpio_Config(&userSWenc);
	gpio_Config(&userCKenc);
	gpio_Config(&userRXusart);
	gpio_Config(&userTXusart);

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

	/* Configuramos el timer que refresca la informacion (TIM10) */
	refreshTimer.pTIMx = TIM10;
	refreshTimer.TIMx_Config.TIMx_Prescaler = 16000;
	refreshTimer.TIMx_Config.TIMx_Period = 3000;
	refreshTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	refreshTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	//Cargamos la configuracion de los timers
	timer_Config(&blinkTimer);
	timer_Config(&displayTimer);
	timer_Config(&refreshTimer);

	//Encendemos los timers
	timer_SetState(&blinkTimer, TIMER_ON);
	timer_SetState(&displayTimer, TIMER_ON);
	timer_SetState(&refreshTimer, TIMER_ON);

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

	//Configuramos el protocolo de comunicación

	/*Configuramos el USART*/
	usart.ptrUSARTx = USART6;
	usart.USART_Config.baudrate = USART_BAUDRATE_230400;
	usart.USART_Config.datasize = USART_DATASIZE_8BIT;
	usart.USART_Config.mode = USART_MODE_RXTX;
	usart.USART_Config.parity = USART_PARITY_NONE;
	usart.USART_Config.stopbits = USART_STOPBIT_1;
	usart.USART_Config.enableIntTX = USART_TX_INTERRUP_DISABLE;
	usart.USART_Config.enableIntRX = USART_RX_INTERRUP_ENABLE;

	usart_Config(&usart);

	//Configuramos el ADC que vamos a usar

	/* Configuramos el ADC 8 */
	trimmer.channel = CHANNEL_8;
	trimmer.resolution = RESOLUTION_6_BIT;
	trimmer.dataAlignment = ALIGNMENT_RIGHT;
	trimmer.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	trimmer.interrupState = ADC_INT_ENABLE;
	adc_ConfigSingleChannel(&trimmer);

	//Encedemos el LED que nos indica que estamos en modo Directo.
	gpio_WritePin(&userDir, SET);
}

void caseEncoder(uint8_t *cases) {
	/*
	 * Creamos un switch case con los casos para el encoder con sus debidas funciones
	 * paracada movimiento.
	 */
	switch (*cases) {
	case restaChannel: {
		if (adcChannel != 0) {
			resta(&adcChannel);
		}
		break;
	}
	case sumaChannel: {
		if (adcChannel < 2) {
			suma(&adcChannel);
		}
		break;
	}
	case restaResolucion: {
		if (adcResolution != 0) {
			resta(&adcResolution);
		}
		break;
	}
	case sumaResolucion: {
		if (adcResolution < 3) {
			suma(&adcResolution);
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

void writePattern(uint8_t pattern) {
	switch (pattern) {
	case six: {
		gpio_WritePin(&userLedE, SET);
		break;
	}
	case eight: {
		gpio_WritePin(&userLedD, SET);
		break;
	}
	case ten: {
		gpio_WritePin(&userLedD, SET);
		gpio_WritePin(&userLedG, SET);
		break;
	}
	case twelve: {
		gpio_WritePin(&userLedD, SET);
		gpio_WritePin(&userLedG, SET);
		gpio_WritePin(&userLedA, SET);
		break;
	}
	default: {
		__NOP();
		break;
	}
	}
}

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

// Función que cambia la resolucion del ADC
void changeResolution(ADC_Config_t *adcConfig, uint8_t resolution) {
	adcConfig->resolution = resolution;
	adc_ConfigSingleChannel(adcConfig);
	adc_StartSingleConv();
}

// Función que cambia el canal del ADC
void changeChannel(ADC_Config_t *adcConfig, uint8_t channel) {
	adcConfig->channel = channel;
	adc_ConfigSingleChannel(adcConfig);
	adc_StartSingleConv();
}

uint8_t channelOptions(uint8_t *numero) {
	switch (*numero) {
	case 0: {
		return 8;
		break;
	}
	case 1: {
		return 10;
		break;
	}
	case 2: {
		return 11;
		break;
	}
	default: {
		return 8;
		break;
	}
	}
}

uint8_t resolutionOptions(uint8_t *numero) {
	switch (*numero) {
	case 0: {
		return 3;
		break;
	}
	case 1: {
		return 2;
		break;
	}
	case 2: {
		return 1;
		break;
	}
	case 3: {
		return 0;
		break;
	}
	default: {
		return 3;
		break;
	}
	}
}

void analyzeCommand(char *buffer) {

	if (strcmp(commandBuffer, "p") == 0) {
		usart_writeMsg(&usart, "Testing, Testing!! \n");
	}

	else if (strcmp(commandBuffer, "m") == 0) {
		usart_writeMsg(&usart, "Cambiar Modo \n\n");
		botonEncoder ^= 1;
	}

	else if (strcmp(commandBuffer, "a") == 0) {
		if (botonEncoder) {
			uint8_t sumar = 0;
			sumar = botonEncoder + 2;
			caseEncoder(&sumar);
			sprintf(bufferPrint, "Resolution %d\n\n", adcResolution);
			usart_writeMsg(&usart, bufferPrint);
		} else {
			uint8_t sumar = 0;
			sumar = botonEncoder + 1;
			caseEncoder(&sumar);
			adcChannelaux = adcChannel + 1;
			sprintf(bufferPrint, "Sensor %d\n\n", adcChannelaux);
			usart_writeMsg(&usart, bufferPrint);
		}
	}

	else if (strcmp(commandBuffer, "d") == 0) {
		if (botonEncoder) {
			uint8_t restar = 0;
			restar = botonEncoder + 1;
			caseEncoder(&restar);
			sprintf(bufferPrint, "Resolution %d\n\n", adcResolution);
			usart_writeMsg(&usart, bufferPrint);
		} else {
			uint8_t restar = 0;
			restar = botonEncoder;
			caseEncoder(&restar);
			adcChannelaux = adcChannel + 1;
			sprintf(bufferPrint, "Sensor %d\n\n", adcChannelaux);
			usart_writeMsg(&usart, bufferPrint);
		}
	}
}

uint8_t isLetterCode(char caracter) {
	if (caracter == 'a' || caracter == 'p' || caracter == 'm'
			|| caracter == 'd') {
		return 1;
	}
	return 0;
}

void Timer2_Callback(void) {
	switchTransistores = 1;
}

void Timer4_Callback(void) {
	gpio_TooglePin(&userLed);
}

void Timer10_Callback(void) {
	bandera1 = 1;
}

void callback_ExtInt10(void) {
	/*
	 * Cada vez que presionamos el boton asociado a esta interrupción
	 * hacemos un XOR para así cambiar el estado y asu vez la dirección
	 * del encoder.
	 */
	botonEncoder ^= 1;
}

void callback_ExtInt3(void) {
	ckEncoder = 1;

}

void usart6_RxCallback(void) {
	rxData = usart_getRxData6();
}

void adc_CompleteCallback(void) {
	adcComplete = 1;
	trimmer.adcData = adc_GetValue();
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
