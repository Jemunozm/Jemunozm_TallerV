/*
 * spi_driver_hal.c
 *
 *  Created on: Oct 23, 2023
 *      Author: imjeviz
 */

#include "stm32f4xx.h"
#include "spi_driver_hal.h"





/* === Headers for private functions === */
static void spi_enable_clock_peripheral(SPI_Handler_t *ptrSpiHandler);
static void spi_config_datasize(SPI_Handler_t *ptrSpiHandler);
static void spi_config_bitorder(SPI_Handler_t *ptrSpiHandler);
static void spi_config_baudrate(SPI_Handler_t *ptrSpiHandler);
static void spi_config_polarity(SPI_Handler_t *ptrSpiHandler);
static void spi_config_phase(SPI_Handler_t *ptrSpiHandler);
static void spi_config_mode(SPI_Handler_t *ptrSpiHandler);
static void spi_config_interrupt(SPI_Handler_t *ptrSpiHandler);
static void spi_enable_peripheral(SPI_Handler_t *ptrSpiHandler);

/**
 * Configuramos el puerto serial...
 * Comezando por activar la señal de reloj de periférico
 */

//void spi_Config(SPI_Config_t *ptrSpiHandler){
//
//	/* 0. Desactivamos las interrupciones globales mientras configuramos el sistema.*/
//	__disable_irq();
//
//	/* 1. Activamos la señal de reloj que viene desde el BUS al que pertenece el periférico */
//	spi_enable_clock_peripheral(ptrSpiHandler);
//
//	/* 2. Configuramos el tamaño del dato, el orden de lectura, la polaridad y la fase
//	 *
//	 *
//	 * Configuración del Baudrate
//	 * Configuramos el modo:
//	 * Por ultimo activamos el modulo SPI cuando todo esté correctamente configurado.
//	 */
//
//	/* 2.1 Comenzamos limpiando los registros, para cargar la configuración desde cero.*/
//
//
//	// 2.2 Configuracion del Parity:
//	spi_config_polarity(ptrSpiHandler);
//
//	// 2.3 Configuramos el tamaño del dato
//	spi_config_datasize(ptrSpiHandler);
//	spi_config_bitorder(ptrSpiHandler);
//
//	// 2.4 Configuramos los stop bits (SFR USART_CR2)
//	spi_config_phase(ptrSpiHandler);
//
//	// 2.5 Configuracion del Baudrate (SFR USART_BRR)
//	spi_config_baudrate(ptrSpiHandler);
//
//	// 2.6 Configuramos el modo: TX only, RX only, RXTX, disable
//	spi_config_mode(ptrSpiHandler);
//
//	// 2.8 Verificamos la configuración de las interrupciones
//	spi_config_interrupt(ptrSpiHandler);
//
//	// 2.7 Activamos el modulo serial.
//	spi_enable_peripheral(ptrSpiHandler);
//
//	/* x. Volvemos a activar las interrupciones del sistema */
//	__enable_irq();
//}









