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
#include "pwm_driver_hal.h"
#include "systick_driver_hal.h"

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
GPIO_Handler_t userLedRed = { 0 }; // PB7
GPIO_Handler_t userLedGreen = { 0 }; // PB8
GPIO_Handler_t userLedBlue = { 0 }; // PB9


Systick_Handler_t userSystick = { 0 };


//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer = { 0 }; // Timer para el blinky PinA5.
Timer_Handler_t displayTimer = { 0 }; // Timer para el 7-segmentos.
Timer_Handler_t refreshTimer = { 0 }; // Timer para refrescar la impresión.
Timer_Handler_t rgbTimer = {0}; //Timer para el PWM delledRGB.

//Definimos las lineas EXTI que vamos a utilizar.
EXTI_Config_t swExti = { 0 }; //Exti linea 10 para el sw del encoder.
EXTI_Config_t ckExti = { 0 }; //Exti linea 3 para el ck del enconder.

//Definimos el ADC que vamos a utilizar.
ADC_Config_t trimmer = { 0 };

//Definimos el PWM que vamsoautilizar.
PWM_Handler_t rgb = { 0 };

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
uint8_t adcChannel = 0;
uint8_t adcChannelaux = 0;
uint8_t channelOption = 0;
uint8_t adcComplete = 0;
uint8_t receivedChar = 0;
uint8_t commandFlag = 0;
uint8_t bandera1 = 0;
uint8_t switchTransistores = 0;
uint16_t red = 0;
uint16_t green = 0;
uint16_t blue = 0;
uint8_t boton = 0;
uint8_t casos = 0;
uint8_t color = 0;
uint16_t numero = 0;
uint8_t dir1 = 0;
uint8_t dirResult = 0;
uint8_t dir0 = 0;


//llamamos las funciones definidas al final del codigo
void initSys(void);
void write7segments(uint8_t numero);
void writeClean(void);
void suma(uint8_t *conteo);
void resta(uint8_t *conteo);
void analyzeCommand(char *buffer);
uint8_t isLetter(char caracter);
uint8_t isNotControl(char caracter);
uint8_t isLetterCode(char caracter);
void caseEncoder(uint8_t *cases);
void changeChannel(ADC_Config_t *adcConfig, uint8_t channel);
uint8_t channelOptions(uint8_t *numero);



int main(void) {
	// llamamosala funcion que cuenta con toda la configuración
	initSys();
	// mandamos un holamundo de cuando la configuración estácargada
	usart_writeMsg(&usart, "Hola mundo \r");
	//Empezamos la primera cnversión del ADC.
	adc_StartSingleConv();
	write7segments(casos);
	while (1) {
		if (adcComplete){
			caseEncoder(&casos);
		}


		if(gpio_ReadPin(&userSWenc) && casos < 9){
			systick_Delay_ms(1000);
			if (gpio_ReadPin(&userSWenc)){
				suma(&casos);
				write7segments(casos);
			}
		}else if(gpio_ReadPin(&userSWenc) && casos == 9){
			systick_Delay_ms(1000);
			if (gpio_ReadPin(&userSWenc)){
				casos = 0;
				write7segments(casos);
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

	/* Configuramos el PinB12 */
	userDir.pGPIOx = GPIOC;
	userDir.pinConfig.GPIO_PinNumber = PIN_5;
	userDir.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userDir.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userDir.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userDir.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userDir);

	/* Configuramos el PinA10 */
	userSwitch.pGPIOx = GPIOA;
	userSwitch.pinConfig.GPIO_PinNumber = PIN_0;
	userSwitch.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userSwitch.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userSwitch.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userSwitch.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLDOWN;
	gpio_Config(&userSwitch);

	/* Configuramos el PinB5 */
	userData.pGPIOx = GPIOB;
	userData.pinConfig.GPIO_PinNumber = PIN_5;
	userData.pinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpio_Config(&userData);

	/* Configuramos el PinB3 */
	userSWenc.pGPIOx = GPIOA;
	userSWenc.pinConfig.GPIO_PinNumber = PIN_10;
	userSWenc.pinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpio_Config(&userSWenc);

	/* Configuramos el PinB13 */
	userCKenc.pGPIOx = GPIOB;
	userCKenc.pinConfig.GPIO_PinNumber = PIN_3;
	userCKenc.pinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpio_Config(&userCKenc);

	/* Configuramos el PinA2*/
	userTXusart.pGPIOx = GPIOA;
	userTXusart.pinConfig.GPIO_PinNumber = PIN_2;
	userTXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userTXusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userTXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userTXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userTXusart.pinConfig.GPIO_PinAltFunMode = AF7;
	gpio_Config(&userRXusart);

	/* Configuramos el PinA3*/
	userRXusart.pGPIOx = GPIOA;
	userRXusart.pinConfig.GPIO_PinNumber = PIN_3;
	userRXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userRXusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userRXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userRXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userRXusart.pinConfig.GPIO_PinAltFunMode = AF7;
	gpio_Config(&userTXusart);

	/* Configuramos el PinB6 */
	userLedRed.pGPIOx = GPIOB;
	userLedRed.pinConfig.GPIO_PinNumber = PIN_7;
	userLedRed.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userLedRed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedRed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedRed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userLedRed.pinConfig.GPIO_PinAltFunMode = AF2;
	gpio_Config(&userLedRed);

	/* Configuramos el PinB8 */
	userLedGreen.pGPIOx = GPIOB;
	userLedGreen.pinConfig.GPIO_PinNumber = PIN_8;
	userLedGreen.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userLedGreen.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedGreen.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedGreen.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userLedGreen.pinConfig.GPIO_PinAltFunMode = AF2;
	gpio_Config(&userLedGreen);

	/* Configuramos el PinB9 */
	userLedBlue.pGPIOx = GPIOB;
	userLedBlue.pinConfig.GPIO_PinNumber = PIN_9;
	userLedBlue.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userLedBlue.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLedBlue.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLedBlue.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userLedBlue.pinConfig.GPIO_PinAltFunMode = AF2;
	gpio_Config(&userLedBlue);

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
	displayTimer.TIMx_Config.TIMx_Period = 10;
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

	rgb.ptrTIMx = TIM4;
	rgb.config.channel = PWM_CHANNEL_2;
	rgb.config.prescaler = 16;
	rgb.config.periodo = 4095;
	rgb.config.duttyCicle = 150;
	//Cargamos la configuracion
	pwm_Config(&rgb);
	startPwmSignal(&rgb);

	//Configuramos las interrupciones externas (EXTI)

	/* Configuramos el EXTI sw que será en la linea 3 */
	swExti.pGPIOHandler = &userSWenc;
	swExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&swExti);

	/* Configuramos el EXTI ck que será en la linea 13 */
	ckExti.pGPIOHandler = &userCKenc;
	ckExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&ckExti);

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

	//Configuramos el ADC que vamos a usar

	/* Configuramos el ADC 8 */
	trimmer.channel = CHANNEL_8;
	trimmer.resolution = RESOLUTION_12_BIT;
	trimmer.dataAlignment = ALIGNMENT_RIGHT;
	trimmer.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	trimmer.interrupState = ADC_INT_ENABLE;
	adc_ConfigSingleChannel(&trimmer);

	userSystick.pSystick = SysTick;
	userSystick.Systick_Config.Systick_Reload = 16000;
	userSystick.Systick_Config.Systick_IntState = SYSTICK_INT_ENABLE;
	systick_Config(&userSystick);

	//Encedemos el LED que nos indica que estamos en modo Directo.
	gpio_WritePin(&userDir, SET);
	//Encendemos el led que nos indica que se cargaron las configuraciones
	gpio_WritePin(&userLed, SET);
}

void changePwm(PWM_Handler_t *ptrTIMx, uint8_t channel) {
	switch (channel) {
	case 0: {
		ptrTIMx->config.channel = PWM_CHANNEL_2;
		break;
	}
	case 1: {
		ptrTIMx->config.channel = PWM_CHANNEL_3;
		break;
	}
	case 2: {
		ptrTIMx->config.channel = PWM_CHANNEL_4;
		break;
	}
	default: {
		break;
	}
	}
}

void caseEncoder(uint8_t *cases) {
	/*
	 * Creamos un switch case con los casos para el encoder con sus debidas funciones
	 * paracada movimiento.
	 */
	switch (*cases) {
	case 0:{
			if(adcChannel == 2){
				adcChannel = 0;
			}else if (adcChannel < 2){
				suma(&adcChannel);
			}
			if(adcChannel == 0){
				red = trimmer.adcData;
				changePwm(&rgb, 0);
				updateDuttyCycle(&rgb, red);
				pwm_Config(&rgb);
				enableOutput(&rgb);
			}else if(adcChannel == 1){
				green = trimmer.adcData;
				changePwm(&rgb, 1);
				updateDuttyCycle(&rgb, green);
				pwm_Config(&rgb);
				enableOutput(&rgb);
			} else if(adcChannel == 2){
				blue = trimmer.adcData;
				changePwm(&rgb, 2);
				updateDuttyCycle(&rgb, blue);
				enableOutput(&rgb);
				pwm_Config(&rgb);
			}
			channelOption = channelOptions(&adcChannel);
			changeChannel(&trimmer, channelOption);
		}
		break;
	case 1:{
		changeChannel(&trimmer, 10);

		changePwm(&rgb, 2);
		disableOutput(&rgb);
		changePwm(&rgb, 0);
		disableOutput(&rgb);

		changePwm(&rgb, 1);
		pwm_Config(&rgb);
		enableOutput(&rgb);
		green = trimmer.adcData;
		break;
	}
	case 2:{
		changeChannel(&trimmer, 11);

		changePwm(&rgb, 1);
		disableOutput(&rgb);
		changePwm(&rgb, 0);
		disableOutput(&rgb);


		changePwm(&rgb, 2);
		pwm_Config(&rgb);
		enableOutput(&rgb);
		blue = trimmer.adcData;

		break;
	}
	case 3:{
		changeChannel(&trimmer, 8);

		changePwm(&rgb, 2);
		disableOutput(&rgb);
		changePwm(&rgb, 1);
		disableOutput(&rgb);

		changePwm(&rgb, 0);
		pwm_Config(&rgb);
		enableOutput(&rgb);
		red = trimmer.adcData;
		break;
	}
	case 4:{
		changeChannel(&trimmer, 8);
		changePwm(&rgb, 0);
		red = numero;
		updateDuttyCycle(&rgb, red);
		enableOutput(&rgb);
		pwm_Config(&rgb);
		break;
	}
	case 5:{
		changeChannel(&trimmer, 10);
		changePwm(&rgb, 1);
		green = numero;
		updateDuttyCycle(&rgb, green);
		enableOutput(&rgb);
		pwm_Config(&rgb);
		break;
	}
	case 6:{
		changeChannel(&trimmer, 11);
		changePwm(&rgb, 2);
		blue = numero;
		updateDuttyCycle(&rgb, blue);
		enableOutput(&rgb);
		pwm_Config(&rgb);
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
void suma1(uint16_t *conteo) {
	*conteo += 1;
}
// Funcion que resta 1 a la variable que indiquemos como parametro
void resta(uint8_t *conteo) {
	*conteo -= 1;
}
void resta1(uint16_t *conteo) {
	*conteo -= 1;
}

// Función que cambia el canal del ADC
void changeChannel(ADC_Config_t *adcConfig, uint8_t channel) {
	adcConfig->channel = channel;
	adc_ConfigSingleChannel(adcConfig);
	adc_StartSingleConv();
}

/* Función que de acuerdo al numero que tenemos en el encoder
 * retornará el numero del channel para luego ser configurado.
 */
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

/*
 * Función que contiene los posibles casos de funciones
 * de acuerdo al valor tipo char que reciba como parametro.
 */
void analyzeCommand(char *buffer) {

}


void Timer2_Callback(void) {
	switchTransistores = 1;
}

void Timer5_Callback(void){
	gpio_TooglePin(&userLed);
}

void Timer4_Callback(void) {

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
		case 1: {
			if (numero != 4094){
				suma1(&numero);
			}
			break;
		}
		case 3: {
			if (numero > 0){
				resta1(&numero);
			}
			break;
		}
		default: {
			break;
		}
		}

}

void usart2_RxCallback(void) {
	rxData = usart_getRxData2();
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
