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
#include "spi_driver_hal.h"

//Definimos los pines que se van a utilizar.
GPIO_Handler_t userLed = { 0 }; // PinA5
GPIO_Handler_t userTXusart = { 0 }; // PA11
GPIO_Handler_t userRXusart = { 0 }; // PA12
GPIO_Handler_t spiSlave = {0};
GPIO_Handler_t spiMOSI = {0};
GPIO_Handler_t spiSCK = {0};

//Definimos los timers que se emplearan.
Timer_Handler_t blinkTimer = { 0 }; // Timer para el blinky PinA5.

//Definimos el pin USART que vamos a utilizar.
USART_Handler_t usart = { 0 };
uint8_t rxData = 0;
char bufferData[64] = { 0 };
char commandBuffer[64] = { 0 };
char bufferPrint[64] = { 0 };
char bufferspi[8] = { 0 };
uint8_t numeros[5] = {0x10,0x20,0x30,0x40,0x50};
SPI_Handler_t spi = {0};

//Variables auxiliares que nos ayudaran en el codigo
uint8_t bandera1 = 0;

//llamamos las funciones definidas al final del codigo
void initSys(void);

int main(void) {
	// llamamosala funcion que cuenta con toda la configuración
	initSys();
	// mandamos un holamundo de cuando la configuración estácargada
	usart_writeMsg(&usart, "Hola mundo \r");
	while (1) {
		spi_selectSlave(&spi);
		spi_transmit(&spi, numeros, 4);
		spi_unSelectSlave(&spi);
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
	userTXusart.pinConfig.GPIO_PinNumber = PIN_11;
	userTXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userTXusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userTXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userTXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userTXusart.pinConfig.GPIO_PinAltFunMode = AF8;
	gpio_Config(&userTXusart);

	/* Configuramos el PinA3*/
	userRXusart.pGPIOx = GPIOA;
	userRXusart.pinConfig.GPIO_PinNumber = PIN_12;
	userRXusart.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	userRXusart.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userRXusart.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userRXusart.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	userRXusart.pinConfig.GPIO_PinAltFunMode = AF8;
	gpio_Config(&userRXusart);


	spiMOSI.pGPIOx = GPIOA;
	spiMOSI.pinConfig.GPIO_PinNumber = PIN_7;
	spiMOSI.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	spiMOSI.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	spiMOSI.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	spiMOSI.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	spiMOSI.pinConfig.GPIO_PinAltFunMode = AF5;
	gpio_Config(&spiMOSI);

	spiSCK.pGPIOx = GPIOB;
	spiSCK.pinConfig.GPIO_PinNumber = PIN_3;
	spiSCK.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	spiSCK.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	spiSCK.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	spiSCK.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	spiSCK.pinConfig.GPIO_PinAltFunMode = AF5;
	gpio_Config(&spiSCK);

	spi.ptrSPIx = SPI1;
	spi.SPI_Config.baudrate = SPI_BAUDRATE_64;
	spi.SPI_Config.bitorder = SPI_MSBFIRST;
	spi.SPI_Config.datasize = SPI_DATAFRAME_8BIT;
	spi.SPI_Config.fullDupplexEnable = SPI_FULLDUPLEX;
	spi.SPI_Config.mode = SPI_MODE_0;
	spi.SPI_slavePin.pGPIOx = GPIOA;
	spi.SPI_slavePin.pinConfig.GPIO_PinNumber = PIN_4;
	spi.SPI_slavePin.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	spi.SPI_slavePin.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	spi.SPI_slavePin.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	spi.SPI_slavePin.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	spi_Config(&spi);

	//Encendemos el led que nos indica que se cargaron las configuraciones
	gpio_WritePin(&userLed, SET);

	//Configuramos los timers

	/* Configuramos el timer del blink (TIM2) */
	blinkTimer.pTIMx = TIM5;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 16000;
	blinkTimer.TIMx_Config.TIMx_Period = 250;
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;
	timer_Config(&blinkTimer);
	timer_SetState(&blinkTimer, TIMER_ON);


	//Configuramos el protocolo de comunicación

	/*Configuramos el USART*/
	usart.ptrUSARTx = USART2;
	usart.USART_Config.baudrate = USART_BAUDRATE_38400;
	usart.USART_Config.datasize = USART_DATASIZE_8BIT;
	usart.USART_Config.mode = USART_MODE_RXTX;
	usart.USART_Config.parity = USART_PARITY_NONE;
	usart.USART_Config.stopbits = USART_STOPBIT_1;
	usart.USART_Config.enableIntTX = USART_TX_INTERRUP_DISABLE;
	usart.USART_Config.enableIntRX = USART_RX_INTERRUP_ENABLE;

	usart_Config(&usart);



}
void Timer5_Callback(void) {
	gpio_TooglePin(&userLed);
}

void usart2_RxCallback(void) {
	rxData = usart_getRxData2();
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
