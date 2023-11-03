/*
 * pwm_driver_hal.c
 *
 *  Created on: Nov 2, 2023
 *      Author: imjeviz
 */

#include "stm32f4xx.h"
#include "pwm_driver_hal.h"

/**/
// === Headers for private functions ===
static void pwm_enable_clock_peripheral(PWM_Handler_t *ptrPwmHandler);
static void pwm_set_prescaler(PWM_Handler_t *ptrPwmHandler);
static void pwm_set_period(PWM_Handler_t *ptrPwmHandler);
static void pwm_set_mode(PWM_Handler_t *ptrPwmHandler);
static void pwm_config_duttyCicle(PWM_Handler_t *ptrPwmHandler);

/* Función en la que cargamos la configuración del Timer
 * Recordar que siempre se debe comenzar con activar la señal de reloj
 * del periférico que se está utilizando.
 * Además, en este caso, debemos ser cuidadosos al momento de utilizar las interrupciones.
 * Los timer están conectadors directamente al elemento NVIC del cortex-Mx
 * Debemos configurar y/o utilizar:
 * 	- TIMx_CR1 	(control Register 1)
 * 	- TIMx_SMCR	(slave mode control register) -> mantener en 0 para modo Timer Básico
 * 	- TIMx_DIER (DMA and Interrupt enable register)
 * 	- TIMx_SR 	(Status register)
 * 	- TIMx_CNT	(Counter)
 * 	- TIMx_PSC	(Pre-scaler)
 * 	- TIMx_ARR	(Auto-reload register)
 *
 * 	Como vamos a trabajar con iterrupciones, antes de configurar una nueva,debemos desactivar
 * 	el sistema global de interrupciones, activar la IRQ específica y luego volver a encender
 * 	el sistema.
 */

void pwm_Config(PWM_Handler_t *ptrPwmHandler){

	// 1. Activar la señal de reloj del periférico requerido.
	pwm_enable_clock_peripheral(ptrPwmHandler);

	/* 1a. Cargamos la frecuencia deseada */
	setFrequency(ptrPwmHandler);

	/* 2. Cargamos el valor del dutty-Cycle*/
	setDuttyCycle(ptrPwmHandler);

	/* 2a. Estamos en UP_Mode, el limite se carga en ARR y se comienza en 0 */
	/* agregue acá su código */

	/* 3. Configuramos los bits CCxS del registro TIMy_CCMR1, de forma que sea modo salida
	 * (para cada canal hay un conjunto CCxS)
	 *
	 * 4. Además, en el mismo "case" podemos configurar el modo del PWM, su polaridad...
	 *
	 * 5. Y además activamos el preload bit, para que cada vez que exista un update-event
	 * el valor cargado en el CCRx será recargado en el registro "shadow" del PWM */
	switch(ptrPwmHandler->config.channel){
	case PWM_CHANNEL_1:{
		// Seleccionamos como salida el canal
		/* agregue acá su código */

		// Configuramos el canal como PWM
		/* agregue acá su código */

		// Activamos la funcionalidad de pre-load
		/* agregue acá su código */

		break;
	}

	case PWM_CHANNEL_2:{
		// Seleccionamos como salida el canal
		/* agregue acá su código */

		// Configuramos el canal como PWM
		/* agregue acá su código */

		// Activamos la funcionalidad de pre-load
		/* agregue acá su código */
		break;
	}

    /* agregue acá los otros dos casos */

	default:{
		break;
	}

	/* 6. Activamos la salida seleccionada */
	enableOutput(ptrPwmHandler);

	}// fin del switch-case
}

void pwm_enable_clock_peripheral(PWM_Handler_t *ptrPwmHandler){


	if(ptrPwmHandler->ptrTIMx == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM5){
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM9){
		RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM10){
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM11){
		RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
	}
	else{
		__NOP();
	}
}


/* Función para activar el Timer y activar todo el módulo PWM */
void startPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	/* agregue acá su código */
}

/* Función para desactivar el Timer y detener todo el módulo PWM*/
void stopPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	/* agregue acá su código */
}

/* Función encargada de activar cada uno de los canales con los que cuenta el TimerX */
void enableOutput(PWM_Handler_t *ptrPwmHandler) {
	switch (ptrPwmHandler->config.channel) {
	case PWM_CHANNEL_1: {
		// Activamos la salida del canal 1
		/* agregue acá su código */
		break;
	}

	/* agregue acá su código para los otros tres casos */

	default: {
		break;
	}
	}
}

/*
 * La frecuencia es definida por el conjunto formado por el preescaler (PSC)
 * y el valor límite al que llega el Timer (ARR), con estos dos se establece
 * la frecuencia.
 * */
void setFrequency(PWM_Handler_t *ptrPwmHandler){

	// Cargamos el valor del prescaler, nos define la velocidad (en ns) a la cual
	// se incrementa el Timer
	ptrPwmHandler->ptrTIMx->PSC = ptrPwmHandler->config.prescaler - 1;

	// Cargamos el valor del ARR, el cual es el límite de incrementos del Timer
	// antes de hacer un update y reload.
	ptrPwmHandler->ptrTIMx->ARR = ptrPwmHandler->config.periodo - 1;
}


/* Función para actualizar la frecuencia, funciona de la mano con setFrequency */
void updateFrequency(PWM_Handler_t *ptrPwmHandler, uint16_t newFreq){
	// Actualizamos el registro que manipula el periodo
    /* agregue acá su código */

	// Llamamos a la fucnión que cambia la frecuencia
	/* agregue acá su código */
}

/* El valor del dutty debe estar dado en valores de %, entre 0% y 100%*/
void setDuttyCycle(PWM_Handler_t *ptrPwmHandler){

	// Seleccionamos el canal para configurar su dutty
	switch(ptrPwmHandler->config.channel){
	case PWM_CHANNEL_1:{
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.duttyCicle;

		break;
	}

	case PWM_CHANNEL_2:{
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.duttyCicle;

		break;
	}

	case PWM_CHANNEL_3:{
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.duttyCicle;

		break;
	}

	case PWM_CHANNEL_4:{
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.duttyCicle;

		break;
	}

	/* agregue acá su código con los otros tres casos */

	default:{
		break;
	}

	}// fin del switch-case

}


/* Función para actualizar el Dutty, funciona de la mano con setDuttyCycle */
void updateDuttyCycle(PWM_Handler_t *ptrPwmHandler, uint16_t newDutty){
	// Actualizamos el registro que manipula el dutty
    /* agregue acá su código */

	// Llamamos a la fucnión que cambia el dutty y cargamos el nuevo valor
	/* agregue acá su código */
}

