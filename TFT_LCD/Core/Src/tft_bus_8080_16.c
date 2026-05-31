/*
 * tft_bus_8080_16.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. MuÃ±oz
 */

#include "tft_bus_8080_16.h"
#include "main.h"

void tft_bus_init(void)
{
	/* Pendiente: inicializar estados de reposo y validacion del cableado. */
	CS_GPIO_Port->BSRR = CS_Pin;
	RS_GPIO_Port->BSRR = RS_Pin;
	WR_GPIO_Port->BSRR = WR_Pin;
	RST_GPIO_Port->BSRR = RST_Pin;
}

void tft_bus_write_command8(uint8_t cmd)
{
	/* Comando: CS activo, RS en bajo, pulso de WR y retorno a reposo. */
	CS_GPIO_Port->BSRR = (uint32_t)CS_Pin << 16;
	RS_GPIO_Port->BSRR = (uint32_t)RS_Pin << 16;

	DB0_GPIO_Port->BSRR = ((uint32_t)(uint8_t)(~cmd) << 16) | cmd;

	WR_GPIO_Port->BSRR = (uint32_t)WR_Pin << 16;
	WR_GPIO_Port->BSRR = WR_Pin;

	CS_GPIO_Port->BSRR = CS_Pin;
	RS_GPIO_Port->BSRR = RS_Pin;
}

void tft_bus_write_data8(uint8_t data)
{
	/* Dato de 8 bits: RS en alto, CS activo y pulso de WR. */
	CS_GPIO_Port->BSRR = (uint32_t)CS_Pin << 16;
	RS_GPIO_Port->BSRR = RS_Pin;

	DB0_GPIO_Port->BSRR = ((uint32_t)(uint8_t)(~data) << 16) | data;

	WR_GPIO_Port->BSRR = (uint32_t)WR_Pin << 16;
	WR_GPIO_Port->BSRR = WR_Pin;

	CS_GPIO_Port->BSRR = CS_Pin;
}

void tft_bus_write_data16(uint16_t data)
{
	uint8_t low_byte = (uint8_t)(data & 0xFFU);
	uint8_t high_byte = (uint8_t)(data >> 8);

	/* Bus de 16 bits: byte bajo en GPIOC y byte alto en GPIOB. */
	CS_GPIO_Port->BSRR = (uint32_t)CS_Pin << 16;
	RS_GPIO_Port->BSRR = RS_Pin;

	DB0_GPIO_Port->BSRR = ((uint32_t)(uint8_t)(~low_byte) << 16) | low_byte;
	DB8_GPIO_Port->BSRR = ((uint32_t)(uint8_t)(~high_byte) << 24) | ((uint32_t)high_byte << 8);

	WR_GPIO_Port->BSRR = (uint32_t)WR_Pin << 16;
	WR_GPIO_Port->BSRR = WR_Pin;

	CS_GPIO_Port->BSRR = CS_Pin;
}

void tft_bus_reset(void)
{
	/* Secuencia de reset fisico; la temporizacion debe realizarla la capa superior. */
	RST_GPIO_Port->BSRR = (uint32_t)RST_Pin << 16;
	RST_GPIO_Port->BSRR = RST_Pin;
}
