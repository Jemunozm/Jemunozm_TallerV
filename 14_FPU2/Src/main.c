///**
// ******************************************************************************
// * @file           : main.c
// * @author         : mamarinmo
// * @brief          : Tarea4
// ******************************************************************************
// * @attention
// *
// * Copyright (c) 2023 STMicroelectronics.
// * All rights reserved.
// *
// * This software is licensed under terms that can be found in the LICENSE file
// * in the root directory of this software component.
// * If no LICENSE file comes with this software, it is provided AS-IS.
// *
// ******************************************************************************
// */
//

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "stm32f4xx.h"

#include "gpio_driver_hal.h"
#include "timer_driver_hal.h"
#include "exti_driver_hal.h"
#include "timer_driver_hal.h"
#include "adc_driver_hal.h"
#include "usart_driver_hal.h"
#include "pwm_driver_hal.h"

#include "arm_math.h"

// Definicion de los Handlers necesario
GPIO_Handler_t userLed = {0}; //Led de estado PinA5
GPIO_Handler_t userTXusart = { 0 }; // PA2
GPIO_Handler_t userRXusart = { 0 }; // PA3

Timer_Handler_t blinkTimer = {0};// Timer para el blinky PinA5.

//Definimos el ADC que vamos a utilizar.
ADC_Config_t sensor1 = { 0 };
ADC_Config_t sensor2 = { 0 };
ADC_Config_t sensor3 = { 0 };

//Definimos el PWM que vamsoautilizar.
PWM_Handler_t frecuenciaM = { 0 };


//Definimos el pin USART que vamos a utilizar.
USART_Handler_t usart = { 0 };
uint8_t rxData = 0;
char bufferData[64] = { 0 };
char bufferMsg[64] = {0};

//ADC_Config_t *sensores[16]={&sensor1,&sensor2,&sensor3};
ADC_Config_t sensores[16]={0};
uint8_t numeroDeSensores = 3;
uint16_t tamañodatos = 512;
float32_t sensor1data[512] = {0};
float32_t sensor2data[512] = {0};
float32_t sensor3data[512] = {0};
uint16_t sensorSize = 512;
float32_t sensordataaux[512] = {0};
uint16_t data = 0;
uint16_t cont = 0;
float32_t valorMaximo = 0;
float32_t valorMinimo = 0;
uint32_t indiceMaximo = 0;
uint32_t indiceMinimo = 0;


float32_t *ptrSensor1 = sensor1data;
float32_t *ptrSensor2 = sensor2data;
float32_t *ptrSensor3 = sensor3data;

uint32_t dataSize = 0;

/* Elementos para generar una selal */
#define SINE_DATA_SIZE 512//Tamaño del arreglo de datos
float32_t fs = 16000000/(16*25); //frecuencia de muestreo
float32_t sineSignal[SINE_DATA_SIZE];
float32_t fft_power1[SINE_DATA_SIZE/2];
float32_t fft_power2[SINE_DATA_SIZE/2];
float32_t fft_power3[SINE_DATA_SIZE/2];
float32_t transformedSignal1[SINE_DATA_SIZE];
float32_t transformedSignal2[SINE_DATA_SIZE];
float32_t transformedSignal3[SINE_DATA_SIZE];
float32_t *ptrSineSignal;

uint32_t ifftFlag = 0;
arm_rfft_fast_instance_f32 config_Rfft_fast_f32;
arm_status status = ARM_MATH_ARGUMENT_ERROR;
arm_status statusInitFFT = ARM_MATH_ARGUMENT_ERROR;
uint16_t fftSize = 512;

//Definicion de las cabeceras de las funciones  del main
void initSystem(void);
void createSignal(void);

/*
 * Funcion principal del sistema
 */
int main(void){
	// Activamos el cooprocesador matematico
	SCB->CPACR |= (0xF << 20);

	//Inicializamos todos los elementos
	initSystem();

	/* Loop forever*/
	while (1){
	/*
	* Crear la señal
	*/
	if (rxData == 'a'){
		startPwmSignal(&frecuenciaM);
		while(!(data==(tamañodatos-1))){
			__NOP();
		}
//		for(uint16_t i = 0; i<512;i++){
//			sensordataaux[i] = sensor1data[i];
//		}
		stopPwmSignal(&frecuenciaM);

		arm_max_f32(ptrSensor1, sensorSize, &valorMaximo, &indiceMaximo);
		arm_min_f32(ptrSensor1, sensorSize, &valorMinimo, &indiceMinimo);
		data=0;

		statusInitFFT = arm_rfft_fast_init_f32(&config_Rfft_fast_f32, fftSize);

		while(!(statusInitFFT == ARM_MATH_SUCCESS)){
			__NOP();
		}

		arm_rfft_fast_f32(&config_Rfft_fast_f32, sensor1data, transformedSignal1, ifftFlag);

		arm_abs_f32(transformedSignal1, sensor1data, fftSize);

		transformedSignal1[0] = 0;
		arm_cmplx_mag_f32(transformedSignal1, fft_power1, SINE_DATA_SIZE/2);

		float32_t   maxValue;
		uint32_t    maxIndex;
		float32_t	minValue;
		uint32_t 	minIndex;

		arm_max_f32(fft_power1, SINE_DATA_SIZE/2, &maxValue, &maxIndex);

		arm_min_f32(fft_power1, SINE_DATA_SIZE/2, &minValue, &minIndex);

		sprintf(bufferMsg, "max muestra: %.1f\r\n", valorMaximo);
		usart_writeMsg(&usart, bufferMsg);

		sprintf(bufferMsg, "min muestra: %.1f\r\n", valorMinimo);
		usart_writeMsg(&usart, bufferMsg);

		sprintf(bufferMsg, "frequency: %f\r\n\n", ((maxIndex) * fs/SINE_DATA_SIZE)/2);
		usart_writeMsg(&usart, bufferMsg);
		statusInitFFT = ARM_MATH_ARGUMENT_ERROR;

		rxData = '\0';
	}
	if (rxData == 'b'){
		startPwmSignal(&frecuenciaM);
		while(!(data==(tamañodatos-1))){
			__NOP();
		}
		stopPwmSignal(&frecuenciaM);
		arm_max_f32(ptrSensor2, sensorSize, &valorMaximo, &indiceMaximo);
		arm_min_f32(ptrSensor2, sensorSize, &valorMinimo, &indiceMinimo);
		data=0;

		statusInitFFT = arm_rfft_fast_init_f32(&config_Rfft_fast_f32, fftSize);

		while(!(statusInitFFT == ARM_MATH_SUCCESS)){
			__NOP();
		}

		arm_rfft_fast_f32(&config_Rfft_fast_f32, sensor2data, transformedSignal2, ifftFlag);

		arm_abs_f32(transformedSignal2, sensor2data, fftSize);

		transformedSignal2[0] = 0;
	    arm_cmplx_mag_f32(transformedSignal2, fft_power2, SINE_DATA_SIZE/2);

	    float32_t   maxValue;
		uint32_t    maxIndex;
		float32_t	minValue;
		uint32_t 	minIndex;

	    arm_max_f32(fft_power2, SINE_DATA_SIZE/2, &maxValue, &maxIndex);

	    arm_min_f32(fft_power2, SINE_DATA_SIZE/2, &minValue, &minIndex);

	    sprintf(bufferMsg, "max muestra: %.1f\n", valorMaximo);
	    usart_writeMsg(&usart, bufferMsg);

	    sprintf(bufferMsg, "min muestra: %.1f\n", valorMinimo);
	    usart_writeMsg(&usart, bufferMsg);

	    sprintf(bufferMsg, "frequency: %f\r\n\n", ((maxIndex) * fs/SINE_DATA_SIZE)/2);
	    usart_writeMsg(&usart, bufferMsg);

		rxData = '\0';
	}
	if (rxData == 'c'){
		startPwmSignal(&frecuenciaM);

		while(!(data==(tamañodatos-1))){
			__NOP();
		}
		stopPwmSignal(&frecuenciaM);
		arm_max_f32(ptrSensor3, sensorSize, &valorMaximo, &indiceMaximo);
		arm_min_f32(ptrSensor3, sensorSize, &valorMinimo, &indiceMinimo);
		data=0;

		statusInitFFT = arm_rfft_fast_init_f32(&config_Rfft_fast_f32, fftSize);

		while(!(statusInitFFT == ARM_MATH_SUCCESS)){
			__NOP();
		}
		arm_rfft_fast_f32(&config_Rfft_fast_f32, sensor3data, transformedSignal3, ifftFlag);

		arm_abs_f32(transformedSignal3, sensor3data, fftSize);

		transformedSignal3[0] = 0;
	    arm_cmplx_mag_f32(transformedSignal3, fft_power3, SINE_DATA_SIZE/2);

	    float32_t   maxValue;
		uint32_t    maxIndex;
		float32_t	minValue;
		uint32_t 	minIndex;

	    arm_max_f32(fft_power3, SINE_DATA_SIZE/2, &maxValue, &maxIndex);

	    arm_min_f32(fft_power3, SINE_DATA_SIZE/2, &minValue, &minIndex);

	    sprintf(bufferMsg, "max muestra: %.1f\n", valorMaximo);
	    usart_writeMsg(&usart, bufferMsg);

	    sprintf(bufferMsg, "min muestra: %.1f\n", valorMinimo);
	    usart_writeMsg(&usart, bufferMsg);

	    sprintf(bufferMsg, "frequency: %f\r\n\n", ((maxIndex) * fs/SINE_DATA_SIZE)/2);
	    usart_writeMsg(&usart, bufferMsg);

		rxData = '\0';
	}

//
//	if(rxData == 'A'){
//		for(uint16_t i = 0; i < 512; i++){
//			sprintf(bufferMsg,	"%.1f\t %.1f\r\n", 10*sensordataaux[i],fft_power1[i] );
//			usart_writeMsg(&usart, bufferMsg);
//		}
//		data=0;
//		rxData = '\0';
//	}

//
//	if (rxData == 'I'){
//
//		statusInitFFT = arm_rfft_fast_init_f32(&config_Rfft_fast_f32, fftSize);
//
//		if(statusInitFFT == ARM_MATH_SUCCESS){
//			sprintf(bufferMsg, "Initialization...SUCCESS! \n");
//			usart_writeMsg(&usart, bufferMsg);
//		}
//
//		rxData = '\0';
//	}

//	if (rxData == 'F'){
//
//		statusInitFFT = arm_rfft_fast_init_f32(&config_Rfft_fast_f32, fftSize);
//
//		if(statusInitFFT == ARM_MATH_SUCCESS){
//
//			arm_rfft_fast_f32(&config_Rfft_fast_f32, sensor1data, transformedSignal1, ifftFlag);
//
//			arm_abs_f32(transformedSignal1, sensor1data, fftSize);
//
//			arm_rfft_fast_f32(&config_Rfft_fast_f32, sensor2data, transformedSignal2, ifftFlag);
//
//			arm_abs_f32(transformedSignal2, sensor2data, fftSize);
//
//			arm_rfft_fast_f32(&config_Rfft_fast_f32, sensor3data, transformedSignal3, ifftFlag);
//
//			arm_abs_f32(transformedSignal3, sensor3data, fftSize);
//
//		}
//		else{
//			usart_writeMsg(&usart, "FFT NOT INITIALIZED...\n");
//		}
//		rxData = '\0';
//	}
//	if (rxData == 'D'){
//
//		transformedSignal1[0] = 0;
//	    arm_cmplx_mag_f32(transformedSignal1, fft_power1, SINE_DATA_SIZE/2);
//	    for (int i = 1; i < SINE_DATA_SIZE/2; i++) {
//	        sprintf(bufferMsg, "%i\tfrq: %.1f\tenergy %.6f\r\n", i, i * fs/SINE_DATA_SIZE, fft_power1[i]);
//			usart_writeMsg(&usart, bufferMsg);
//	    }
//
//		transformedSignal2[0] = 0;
//	    arm_cmplx_mag_f32(transformedSignal2, fft_power2, SINE_DATA_SIZE/2);
//	    for (int i = 1; i < SINE_DATA_SIZE/2; i++) {
//	        sprintf(bufferMsg, "%i\tfrq: %.1f\tenergy %.6f\r\n", i, i * fs/SINE_DATA_SIZE, fft_power2[i]);
//			usart_writeMsg(&usart, bufferMsg);
//	    }
//
//		transformedSignal3[0] = 0;
//	    arm_cmplx_mag_f32(transformedSignal3, fft_power3, SINE_DATA_SIZE/2);
//	    for (int i = 1; i < SINE_DATA_SIZE/2; i++) {
//	        sprintf(bufferMsg, "%i\tfrq: %.1f\tenergy %.6f\r\n", i, i * fs/SINE_DATA_SIZE, fft_power3[i]);
//			usart_writeMsg(&usart, bufferMsg);
//	    }
//	    rxData = '\0';
//	}
//	if (rxData == 'X'){
//	    float32_t   maxValue;
//	    uint32_t    maxIndex;
//	    float32_t	minValue;
//	    uint32_t 	minIndex;
//
//	    arm_max_f32(fft_power1, SINE_DATA_SIZE/2, &maxValue, &maxIndex);
//	    sprintf(bufferMsg, "\r\n");
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    arm_min_f32(fft_power1, SINE_DATA_SIZE/2, &minValue, &minIndex);
//	    sprintf(bufferMsg, "\r\n");
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "max power: %f\r\n", maxValue);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "min power: %f\r\n", minValue);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "max index: %ld\r\n", maxIndex);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "min index: %ld\r\n", minIndex);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "frequency: %f\r\n", ((maxIndex/2) * fs/SINE_DATA_SIZE));
//	    usart_writeMsg(&usart, bufferMsg);
//
//
//
//	    arm_max_f32(fft_power2, SINE_DATA_SIZE/2, &maxValue, &maxIndex);
//	    sprintf(bufferMsg, "\r\n");
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    arm_min_f32(fft_power2, SINE_DATA_SIZE/2, &minValue, &minIndex);
//	    sprintf(bufferMsg, "\r\n");
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "max power: %f\r\n", maxValue);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "min power: %f\r\n", minValue);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "max index: %ld\r\n", maxIndex);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "min index: %ld\r\n", minIndex);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "frequency: %f\r\n", ((maxIndex/2) * fs/SINE_DATA_SIZE));
//	    usart_writeMsg(&usart, bufferMsg);
//
//
//
//	    arm_max_f32(fft_power3, SINE_DATA_SIZE/2, &maxValue, &maxIndex);
//	    sprintf(bufferMsg, "\r\n");
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    arm_min_f32(fft_power3, SINE_DATA_SIZE/2, &minValue, &minIndex);
//	    sprintf(bufferMsg, "\r\n");
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "max power: %f\r\n", maxValue);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "min power: %f\r\n", minValue);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "max index: %ld\r\n", maxIndex);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "min index: %ld\r\n", minIndex);
//	    usart_writeMsg(&usart, bufferMsg);
//
//	    sprintf(bufferMsg, "frequency: %f\r\n", ((maxIndex/2) * fs/SINE_DATA_SIZE));
//	    usart_writeMsg(&usart, bufferMsg);
//	    rxData = '\0';
//	}

	}
	return 0;
}

void initSystem(void){
		/* Configuramos el pin de estado*/
	userLed.pGPIOx = GPIOA;
	userLed.pinConfig.GPIO_PinNumber = PIN_5;
	userLed.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userLed.pinConfig.GPIO_PinAltFunMode = AF0;

	/* Cargamos la configuracion de los registros que gobiernan el puerto*/
	gpio_Config(&userLed);

	/* Escribimos SET (1) en el userLed para encender el led de estado*/
	gpio_WritePin(&userLed, SET);

	//Configuracion del timer 2 pal blinky
	blinkTimer.pTIMx = TIM2;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000; //Genera incrementos de 1 ms
	blinkTimer.TIMx_Config.TIMx_Period = 250; //Periodo del preescaler
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	//Configuramos el timer
	timer_Config(&blinkTimer);

	//Encendemos el Timer
	timer_SetState(&blinkTimer, TIMER_ON);

	/* Configuramos los pines del puerto serial*/
	/* Pin donde funciona el USART2 Tx*/
	userTXusart.pGPIOx = GPIOA;
	userTXusart.pinConfig.GPIO_PinNumber = PIN_2;
	userTXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userTXusart.pinConfig.GPIO_PinAltFunMode = AF7;
	userTXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userTXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_FAST;

	/* Cargamos la configuracion de los registros que gobiernan el puerto*/
	gpio_Config(&userTXusart);


	/* Pin donde funciona el USART6 Rx*/
	userRXusart.pGPIOx = GPIOA;
	userRXusart.pinConfig.GPIO_PinNumber = PIN_3;
	userRXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userRXusart.pinConfig.GPIO_PinAltFunMode = AF7;
	userRXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userRXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_FAST;
	/* Cargamos la configuracion de los registros que gobiernan el puerto*/
	gpio_Config(&userRXusart);

	/* Configuramos los pines del puerto serial USART6*/
	usart.ptrUSARTx = USART2;
	usart.USART_Config.baudrate = USART_BAUDRATE_115200;
	usart.USART_Config.datasize = USART_DATASIZE_8BIT;
	usart.USART_Config.parity = USART_PARITY_NONE;
	usart.USART_Config.stopbits = USART_STOPBIT_1;
	usart.USART_Config.mode = USART_MODE_RXTX;
	usart.USART_Config.enableIntRX = USART_RX_INTERRUP_ENABLE;
	usart.USART_Config.enableIntTX = USART_RX_INTERRUP_DISABLE;
	usart_Config(&usart);

	/*Configuramos elPWM que nos empiza la conversión adc*/
	frecuenciaM.ptrTIMx = TIM3;
	frecuenciaM.config.channel = PWM_CHANNEL_1;
	frecuenciaM.config.prescaler = 16;
	frecuenciaM.config.periodo = 25;
	frecuenciaM.config.duttyCicle = 2;
	pwm_Config(&frecuenciaM);

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

	adc_ConfigMultichannel(sensores,numeroDeSensores);

	adc_ConfigTrigger(TRIGGER_RISING, &frecuenciaM);

	//Encendemos el led que nos indica que se cargaron las configuraciones
	gpio_WritePin(&userLed, SET);
}

void Timer2_Callback(void){
	gpio_TooglePin(&userLed);
}

void usart2_RxCallback(void){
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
