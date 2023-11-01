/*
 * spi_driver_hal.h
 *
 *  Created on: Oct 22, 2023
 *      Author: imjeviz
 */

#ifndef SPI_DRIVER_HAL_H_
#define SPI_DRIVER_HAL_H_

#include <stdio.h>
#include "stm32f4xx.h"


enum
{
	SPI_MODE_SLAVE = 0,
	SPI_MODE_MASTER
};

enum
{
	SPI_DATAFRAME_8BIT = 0,
	SPI_DATAFRAME_16BIT
};

enum
{
	SPI_DATAFORMAT_MSBFIRST = 0,
	SPI_DATAFORMAT_LSBFIRST,
};

enum
{
	SPI_CLOCKPOLARITY_OFF = 0,
	SPI_CLOCKPOLARITY_ON
};

enum
{
	SPI_CLOCKPHASE_OFF = 0,
	SPI_CLOCKPHASE_ON
};

enum
{
	SPI_MODE_0 = 0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
};

enum
{
	SPI_BIDIMODE_OFF = 0,
	SPI_BIDIMODE_ON
};

enum
{
	SPI_BIDIOE_OFF = 0,
	SPI_BIDIOE_ON
};


enum
{
	SPI_RXO_FULLDUPLEX = 0,
	SPI_RXO_OUTDISABLED
};

enum
{
	SPI_SSM_OFF = 0,
	SPI_SSM_ON
};

enum
{
	SPI_OFF = 0,
	SPI_ON
};

enum
{
	SPI_BAUDRATE_2 = 0,
	SPI_BAUDRATE_4,
	SPI_BAUDRATE_8,
	SPI_BAUDRATE_16,
	SPI_BAUDRATE_32,
	SPI_BAUDRATE_64,
	SPI_BAUDRATE_128,
	SPI_BAUDRATE_256
};


/* Estructura para la configuración de la comunicacion:
 * Velocidad (baudrate)
 * Tamaño de los datos
 * Control de errores
 * Modo de comunicación
 */
typedef struct
{
	uint8_t mode;
	uint8_t datasize;
	uint8_t baudrate;
	uint8_t bitorder;
	uint8_t polarity;
	uint8_t phase;
	uint8_t	enableIntRX;
	uint8_t	enableIntTX;
}SPI_Config_t;

/*
 * Definicion del Handler para un USART:
 * - Estructura que contiene los SFR que controlan el periferico
 * - Estructura que contiene la configuración especifica del objeto
 * - Buffer de recepcion de datos
 * - Elemento que indica cuantos datos se recibieron
 * - Buffer de transmision de datos
 * - Elemento que indica cuantos datos se deben enviar.
 */
typedef struct
{
	SPI_TypeDef		*ptrSPIx;
	SPI_Config_t	SPI_Config;
//	uint8_t			receptionBuffer[64];
//	uint8_t			dataInputSize;
//	uint8_t			transmisionBuffer[64];
//	uint8_t			dataOutputSize;
}SPI_Handler_t;

void spi_Config(SPI_Handler_t *ptrSpiHandler);
//int  spi_WriteChar(SPI_Handler_t *ptrSpiHandler, int dataToSend );
//void spi_writeMsg(SPI_Handler_t *ptrSpiHandler, char *msgToSend );

#endif /* SPI_DRIVER_HAL_H_ */
