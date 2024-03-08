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
#include "systick_driver_hal.h"
#include "TFT_ILI9486.h"
#include "pll_driver_hal.h"
#include "stdlib.h"
#include "math.h"
#include "gpio_driver_hal.h"
#include "exti_driver_hal.h"
#include "adc_driver_hal.h"
#include "timer_driver_hal.h"
#include "pwm_driver_hal.h"

Systick_Handler_t userSystick = { 0 };
GPIO_Handler_t userLed = { 0 }; // PinB7
Timer_Handler_t blinkTimer = {0};// Timer para el blinky PinB7.
GPIO_Handler_t btnA = { 0 }; // PinC2
EXTI_Config_t extiA = { 0 }; //Exti linea 2 para el sw del encoder
GPIO_Handler_t btnY = { 0 }; // PinC1
EXTI_Config_t extiY = { 0 }; //Exti linea 1 para el sw del encoder
GPIO_Handler_t btnX = { 0 }; // PinC3
EXTI_Config_t extiX = { 0 }; //Exti linea 3 para el sw del encoder
GPIO_Handler_t btnO = { 0 }; // PinC0
EXTI_Config_t extiO = { 0 }; //Exti linea 0 para el sw del encoder

//Definimos el PWM que vamsoautilizar.
PWM_Handler_t frecuenciaM = { 0 };

//Definimos el ADC que vamos a utilizar.
ADC_Config_t vry = { 0 };
ADC_Config_t vrx = { 0 };

//Definimos el que alamcenará los ejes del Joystick con hanlder ADC
ADC_Config_t joystick[16]={0};


void initSystem(void);

int main(void) {

	initSystem();

	while (1) {

}
}
void initSystem(void){

	pll_config();
	SCB->CPACR |= (0xF<<20);

	userSystick.pSystick = SysTick;
	userSystick.Systick_Config.Systick_Reload = 100000;
	userSystick.Systick_Config.Systick_IntState = SYSTICK_INT_ENABLE;


	systick_Config(&userSystick);

	userLed.pGPIOx						= GPIOB;
	userLed.pinConfig.GPIO_PinNumber	= PIN_7;
	userLed.pinConfig.GPIO_PinMode		= GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLed);

	//Configuracion del timer 2 pal blinky
	blinkTimer.pTIMx = TIM2;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 10000; //Genera incrementos de 1 ms
	blinkTimer.TIMx_Config.TIMx_Period = 2500; //Periodo del preescaler
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	//Configuramos el timer
	timer_Config(&blinkTimer);

	//Encendemos el Timer
	timer_SetState(&blinkTimer, TIMER_ON);


	btnA.pGPIOx						= GPIOC;
	btnA.pinConfig.GPIO_PinNumber	= PIN_2;
	btnA.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnA);

	btnY.pGPIOx						= GPIOC;
	btnY.pinConfig.GPIO_PinNumber	= PIN_1;
	btnY.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnY);

	btnX.pGPIOx						= GPIOC;
	btnX.pinConfig.GPIO_PinNumber	= PIN_3;
	btnX.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnX);

	btnO.pGPIOx						= GPIOC;
	btnO.pinConfig.GPIO_PinNumber	= PIN_0;
	btnO.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnO);

	extiA.pGPIOHandler = &btnA;
	extiA.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiA);

	extiY.pGPIOHandler = &btnY;
	extiY.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiY);

	extiX.pGPIOHandler = &btnX;
	extiX.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiX);

	extiO.pGPIOHandler = &btnO;
	extiO.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiO);

	/*Configuramos elPWM que nos empiza la conversión adc*/
	frecuenciaM.ptrTIMx = TIM3;
	frecuenciaM.config.channel = PWM_CHANNEL_1;
	frecuenciaM.config.prescaler = 16;
	frecuenciaM.config.periodo = 25;
	frecuenciaM.config.duttyCicle = 2;
	pwm_Config(&frecuenciaM);

	/* Configuramos el ADC 8 */
	vry.channel = CHANNEL_0;
	vry.resolution = RESOLUTION_12_BIT;
	vry.dataAlignment = ALIGNMENT_RIGHT;
	vry.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	vry.interrupState = ADC_INT_ENABLE;

	vrx.channel = CHANNEL_1;
	vrx.resolution = RESOLUTION_12_BIT;
	vrx.dataAlignment = ALIGNMENT_RIGHT;
	vrx.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	vrx.interrupState = ADC_INT_ENABLE;

	joystick[0] = vrx;
	joystick[1] = vry;

	adc_ConfigMultichannel(joystick,2);//2 es el numero delos ejes del Joystick
	adc_ConfigTrigger(1, &frecuenciaM);
	startPwmSignal(&frecuenciaM);

	tft_Config();
	setRotation(3);
}

void Timer2_Callback(void){
	gpio_TooglePin(&userLed);
}

void assert_failed(uint8_t *file, uint32_t line) {
	while (1) {
		//Problems
	}
}
