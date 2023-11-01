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
GPIO_Handler_t userTRusart	= { 0 };
GPIO_Handler_t userRXusart	= { 0 };



//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer 		= { 0 }; // Timer para el blinky PinA5.
Timer_Handler_t displayTimer 	= { 0 }; // Timer para el 7-segmentos.
Timer_Handler_t refreshTimer 	= { 0 }; // Timer para refrescar la impresión.


//Definimos las lineas EXTI que vamos a utilizar.
EXTI_Config_t swExti = { 0 }; //Exti linea 3 para el sw del encoder.
EXTI_Config_t ckExti = { 0 }; //Exti linea 13 para el ck del enconder.


//Definimos el ADC que vamos a utilizar.
ADC_Config_t trimmer = {0};
//ADC_Config_t trimmer2 = {0};
//ADC_Config_t trimmer3 = {0};

//Definimos el pin USART que vamos a utilizar.
USART_Handler_t usart = { 0 };
uint8_t rxData = 0;
char bufferData[64] = { 0 };
char commandBuffer[64] = { 0 };

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
uint8_t botonEncoder = 0;
uint8_t dataEncoder = 0;
uint8_t direccionEncoder = 0;
uint8_t switch7segment = 0;
uint8_t resolucionADC = 0;
uint8_t channelADC = 0;
uint8_t adcComplete = 0;
uint8_t receivedChar = 0;
uint8_t commandFlag = 0;
uint8_t bandera1 = 0;



//llamamos las funciones definidas al final del codigo
void initSys(void);
void write7segments(uint8_t numero);
void suma(uint8_t *conteo);
void resta(uint8_t *conteo);
uint8_t isLetter(char caracter);
uint8_t isNumber(char caracter);
uint8_t isNotControl(char caracter);
void analyzeCommand(char *buffer);


//Creación de un enum con los cuatro posibles casos del encoder y tarea.

enum {
	sumaResolucion = 0, sumaChannel, restaResolucion, restaChannel
};

int main(void) {
	initSys();
	usart_writeMsg(&usart, "Hola mundo \r");
	while (1) {


//		if(bandera1){
//			bandera1 = 0;
//			usart_writeMsg(&usart, "Hola mundo \r");
//		}
		//Actualizamos dos nuestras variables auxiliares.
		direccionEncoder = (botonEncoder << 1) | (dataEncoder << 0);

		/*
		 * Este conjunto me ayuda a identificar que tipo de caracter
		 * estoy recibiendo y de acuerdo a ello imprimo y ejecuto
		 * comandos.
		 */
		if (rxData != 0) {
			if (isNotControl(rxData)) {
				usart_WriteChar(&usart, rxData);
				strcat(commandBuffer, (char*) &rxData);
			}

			else if (rxData == 0x0D) {
//				strcat(commandBuffer, (char*) &rxData);
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


	}
}

void initSys(void){


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
	userDir.pGPIOx = GPIOB;
	userDir.pinConfig.GPIO_PinNumber = PIN_12;
	userDir.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userDir.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userDir.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userDir.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	/* Configuramos el PinA10 */
	userSwitch.pGPIOx = GPIOA;
	userSwitch.pinConfig.GPIO_PinNumber = PIN_10;
	userSwitch.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userSwitch.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userSwitch.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userSwitch.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLDOWN;

	/* Configuramos el PinB5 */
	userData.pGPIOx 						= GPIOB;
	userData.pinConfig.GPIO_PinNumber 		= PIN_5;
	userData.pinConfig.GPIO_PinMode 		= GPIO_MODE_IN;

	/* Configuramos el PinB3 */
	userSWenc.pGPIOx						= GPIOB;
	userSWenc.pinConfig.GPIO_PinNumber		= PIN_3;
	userSWenc.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;

	/* Configuramos el PinB13 */
	userCKenc.pGPIOx = GPIOB;
	userCKenc.pinConfig.GPIO_PinNumber = PIN_13;
	userCKenc.pinConfig.GPIO_PinMode = GPIO_MODE_IN;

	/* Configuramos el PinA2*/
	userTRusart.pGPIOx = GPIOA;
	userTRusart.pinConfig.GPIO_PinNumber = PIN_2;
	userTRusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userTRusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userTRusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userTRusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userTRusart.pinConfig.GPIO_PinAltFunMode = AF7;

	/* Configuramos el PinA3*/
	userRXusart.pGPIOx = GPIOA;
	userRXusart.pinConfig.GPIO_PinNumber = PIN_3;
	userRXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userRXusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userRXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userRXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userRXusart.pinConfig.GPIO_PinAltFunMode = AF7;

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
	gpio_Config(&userRXusart);
	gpio_Config(&userTRusart);

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
	refreshTimer.TIMx_Config.TIMx_Period = 1500;
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


	userSWenc.pGPIOx						= GPIOB;
	userSWenc.pinConfig.GPIO_PinNumber		= PIN_3;
	userSWenc.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;

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
	usart.ptrUSARTx = USART2;
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
	trimmer.channel			= CHANNEL_8;
	trimmer.resolution			= RESOLUTION_12_BIT;
	trimmer.dataAlignment		= ALIGNMENT_RIGHT;
	trimmer.samplingPeriod		= SAMPLING_PERIOD_84_CYCLES;
	trimmer.interrupState		= ADC_INT_ENABLE;
	adc_ConfigSingleChannel(&trimmer);

	//Encedemos el LED que nos indica que estamos en modo Directo.
	gpio_WritePin(&userDir, SET);
}

void Timer2_Callback(void) {
	switch7segment = 1;
}

void Timer4_Callback(void) {
	gpio_TooglePin(&userLed);
}

void Timer10_Callback(void){
	bandera1 = 1;
}

void callback_ExtInt3(void) {
	/*
	 * Cada vez que presionamos el boton asociado a esta interrupción
	 * hacemos un XOR para así cambiar el estado y asu vez la dirección
	 * del encoder.
	 */
	botonEncoder ^= 1;
	gpio_TooglePin(&userDir);
}

void callback_ExtInt13(void) {
	//almacenamos la informacion recibida por los datos
	dataEncoder = gpio_ReadPin(&userData);

	//Actualizamos la variable que nos dará la direccion de conteo para los casos.
	direccionEncoder = (botonEncoder << 1) | (dataEncoder << 0);
}

void usart2_RxCallback(void){
	rxData = usart_getRxData2();
}

void adc_CompleteCallback (void){
	adcComplete = 1;
	trimmer.adcData = adc_GetValue();
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

// Función que cambia la resolucion del ADC
void changeResolution(ADC_Config_t *adcConfig, uint8_t resolution) {
	adcConfig->resolution = resolution;
	adc_ConfigSingleChannel (adcConfig);
}

// Función que cambia el canal del ADC
void changeChannel(ADC_Config_t *adcConfig, uint8_t channel) {
	adcConfig->channel = channel;
	adc_ConfigSingleChannel (adcConfig);
}

void analyzeCommand(char *buffer){

	if(strcmp(commandBuffer, "help1") == 0){
		usart_writeMsg(&usart, "Menu de ayuda (help): \n\r");
		usart_writeMsg(&usart, "1)  help -> Se imprime el menu de ayuda \n");
		usart_writeMsg(&usart, "2)  t	 -> Se hace testing del usart \n");
		usart_writeMsg(&usart, "3)  m 	 -> Se habilita el ADC \n");
		usart_writeMsg(&usart, "4)  c 	 -> Se habilita el modo continuo \n");
		usart_writeMsg(&usart, "5)  s	 -> Se desabilita el modo continuo \n");
		usart_writeMsg(&usart, "6)  ar0  -> Cambiar la resolución a 12bits \n");
		usart_writeMsg(&usart, "7)  ar1  -> Cambiar la resolución a 10bits \n");
		usart_writeMsg(&usart, "8)  ar2  -> Cambiar la resolución a 8bits \n");
		usart_writeMsg(&usart, "9)  ar3  -> Cambiar la resolución a 6bits \n");
		usart_writeMsg(&usart, "10) ac0  -> Cambiar al canal 1 \n");
		usart_writeMsg(&usart, "11) ac1  -> Cambiar al canal 2 \n");
		usart_writeMsg(&usart, "12) ac2  -> Cambiar al canal 3 \n");
		usart_writeMsg(&usart, "13) ar+  -> Aumentar la resolución \n");
		usart_writeMsg(&usart, "14) ar-  -> Disminuir la resolución \n");
		usart_writeMsg(&usart, "15) ac+  -> Subir de canal \n");
		usart_writeMsg(&usart, "16) ac-  -> Bajar de canal \n");
		usart_writeMsg(&usart, "17) tr+  -> Aumentar tasa de refresco \n");
		usart_writeMsg(&usart, "18) tr-  -> Disminuir tasa de refresco \n");
	} else if(strcmp(commandBuffer,"t") == 0){
		usart_writeMsg(&usart, "Testing, Testing!! \n");
	} else if(strcmp(commandBuffer,"m") == 0){
		usart_writeMsg(&usart, "make a simple ADC \n\b\r");
	} else if(strcmp(commandBuffer,"c") == 0){
		usart_writeMsg(&usart, "Make a continuous ADC \n");
	} else if(strcmp(commandBuffer,"s") == 0){
		usart_writeMsg(&usart, "Stop continuous ADC \n");
	} else if(strcmp(commandBuffer,"ar0") == 0){
		usart_writeMsg(&usart, "Se cambió la resolución a 12bits \n");
	} else if(strcmp(commandBuffer,"ar1") == 0){
		usart_writeMsg(&usart, "Se cambió la resolución a 10bits \n");
	} else if(strcmp(commandBuffer,"ar2") == 0){
		usart_writeMsg(&usart, "Se cambió la resolución a 8bits \n");
	} else if(strcmp(commandBuffer,"ar3") == 0){
		usart_writeMsg(&usart, "Se cambió la resolución a 6bits \n");
	} else if(strcmp(commandBuffer,"ac0") == 0){
		usart_writeMsg(&usart, "Se cambió al canal 1 \n");
	} else if(strcmp(commandBuffer,"ac1") == 0){
		usart_writeMsg(&usart, "Se cambió al canal 2 \n");
	} else if(strcmp(commandBuffer,"ac2") == 0){
		usart_writeMsg(&usart, "Se cambió al canal 3 \n");
	} else if(strcmp(commandBuffer,"ar+") == 0){
		usart_writeMsg(&usart, "Se aumentó la resolución \n");
	} else if(strcmp(commandBuffer,"ar-") == 0){
		usart_writeMsg(&usart, "Se disminuyó la resolución \n");
	} else if(strcmp(commandBuffer,"ac+") == 0){
		usart_writeMsg(&usart, "Se subió de canal \n");
	} else if(strcmp(commandBuffer,"ac-") == 0){
		usart_writeMsg(&usart, "Se bajó de canal \n");
	} else if(strcmp(commandBuffer,"tr+") == 0){
		usart_writeMsg(&usart, "Se aumentó la tasa de refresco \n");
	} else if(strcmp(commandBuffer,"tr-") == 0){
		usart_writeMsg(&usart, "Se disminuyó latasa de refresco \n");
	}

}


uint8_t isLetter(char caracter) {
	if ((caracter >= 'A' && caracter <= 'Z')
			|| (caracter >= 'a' && caracter <= 'z')) {
		return 1;
	}
	return 0;
}

uint8_t isNumber(char caracter) {
	if (caracter >= '0' && caracter <= '9') {
		return 1;
	}
	return 0;
}

uint8_t isNotControl(char caracter){
	if(caracter >= 31){
		return 1;
	}
	return 0;
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
