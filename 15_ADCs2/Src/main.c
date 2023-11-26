/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdint.h>
#include <string.h>
#include <math.h>
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
GPIO_Handler_t userTXusart = { 0 }; // PA11
GPIO_Handler_t userRXusart = { 0 }; // PA12

Systick_Handler_t userSystick = { 0 };

//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer = { 0 }; // Timer para el blinky PinA5.
Timer_Handler_t rgbTimer = {0}; //Timer para el PWM delledRGB.

//Definimos el ADC que vamos a utilizar.
ADC_Config_t sensor1 = { 0 };
ADC_Config_t sensor2 = { 0 };
ADC_Config_t sensor3 = { 0 };

//Definimos el PWM que vamsoautilizar.
PWM_Handler_t rgb = { 0 };
PWM_Handler_t frecuenciaM = { 0 };

//Definimos el pin USART que vamos a utilizar.
USART_Handler_t usart = { 0 };
uint8_t rxData = 0;
char bufferData[64] = { 0 };
char commandBuffer[64] = { 0 };
char bufferPrint[64] = { 0 };


//ADC_Config_t *sensores[16]={&sensor1,&sensor2,&sensor3};
ADC_Config_t sensores[16]={0};
uint16_t sensor1data[512] = {0};
uint16_t sensor2data[512] = {0};
uint16_t sensor3data[512] = {0};
uint16_t data = 0;
uint16_t cont = 0;


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
	startPwmSignal(&frecuenciaM);
	while (1) {


		if(data==512){

			stopPwmSignal(&frecuenciaM);
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


	//Configuramos los timers

	/* Configuramos el timer del blink (TIM2) */
	blinkTimer.pTIMx = TIM5;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000;
	blinkTimer.TIMx_Config.TIMx_Period = 250;
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;
	timer_Config(&blinkTimer);
	timer_SetState(&blinkTimer, TIMER_ON);

	frecuenciaM.ptrTIMx = TIM3;
	frecuenciaM.config.channel = PWM_CHANNEL_1;
	frecuenciaM.config.prescaler = 16;
	frecuenciaM.config.periodo = 25;
	frecuenciaM.config.duttyCicle = 2;
	pwm_Config(&frecuenciaM);


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
	sensor1.channel = CHANNEL_8;
	sensor1.resolution = RESOLUTION_12_BIT;
	sensor1.dataAlignment = ALIGNMENT_RIGHT;
	sensor1.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	sensor1.interrupState = ADC_INT_ENABLE;

	sensor2.channel = CHANNEL_10;
	sensor2.resolution = RESOLUTION_12_BIT;
	sensor2.dataAlignment = ALIGNMENT_RIGHT;
	sensor2.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	sensor2.interrupState = ADC_INT_ENABLE;

	sensor3.channel = CHANNEL_11;
	sensor3.resolution = RESOLUTION_12_BIT;
	sensor3.dataAlignment = ALIGNMENT_RIGHT;
	sensor3.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	sensor3.interrupState = ADC_INT_ENABLE;

	sensores[0] = sensor1;
	sensores[1] = sensor2;
	sensores[2] = sensor3;

	adc_ConfigMultichannel(sensores,3);

	adc_ConfigTrigger(TRIGGER_RISING, &frecuenciaM);

	userSystick.pSystick = SysTick;
	userSystick.Systick_Config.Systick_Reload = 16000;
	userSystick.Systick_Config.Systick_IntState = SYSTICK_INT_ENABLE;
	systick_Config(&userSystick);

//	//Encedemos el LED que nos indica que estamos en modo Directo.
//	gpio_WritePin(&userDir, SET);
	//Encendemos el led que nos indica que se cargaron las configuraciones
	gpio_WritePin(&userLed, SET);
}



// Funcion que suma 1 a la variable que indiquemos como parametro.
void suma(uint8_t *conteo) {
	*conteo += 1;
}
void suma1(uint16_t *conteo) {
	*conteo += 1;
}

/*
 * Función que contiene los posibles casos de funciones
 * de acuerdo al valor tipo char que reciba como parametro.
 */
void analyzeCommand(char *buffer) {

}


void Timer2_Callback(void) {
}

void Timer5_Callback(void){
	gpio_TooglePin(&userLed);
}

void Timer4_Callback(void) {

}

void Timer10_Callback(void) {
}

void callback_ExtInt10(void) {
}

void callback_ExtInt3(void) {
}

void usart2_RxCallback(void) {
	rxData = usart_getRxData2();
}

void adc_CompleteCallback(void) {


	switch(cont){
	case 0:{
		sensor1data[data]=adc_GetValue();
		cont ++;
		break;
	}
	case 1:{
		sensor2data[data]=adc_GetValue();
		cont ++;
		break;
	}
	case 2:{
		sensor3data[data]=adc_GetValue();
		cont=0;
		data++;
		break;
	}
	}
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
