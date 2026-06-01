/*
 * ili9486.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#include "main.h"
#include "ili9486.h"
#include "tft_bus_8080_16.h"

static uint16_t s_width = ILI9486_WIDTH;
static uint16_t s_height = ILI9486_HEIGHT;

static void ili9486_write_command(uint8_t command);
static void ili9486_write_data8(uint8_t data);
static void ili9486_write_data16(uint16_t data);
static void ili9486_write_u16_be(uint16_t value);
static void ili9486_swap_u16(uint16_t *a, uint16_t *b);

static void ili9486_write_command(uint8_t command)
{
	tft_bus_write_command8(command);
}

static void ili9486_write_data8(uint8_t data)
{
	tft_bus_write_data8(data);
}

static void ili9486_write_data16(uint16_t data)
{
	tft_bus_write_data16(data);
}

static void ili9486_write_u16_be(uint16_t value)
{
	ili9486_write_data8((uint8_t)(value >> 8));
	ili9486_write_data8((uint8_t)(value & 0xFFU));
}

static void ili9486_swap_u16(uint16_t *a, uint16_t *b)
{
	uint16_t temp = *a;
	*a = *b;
	*b = temp;
}

void ili9486_init(void)
{
	static const uint8_t gamma_pos[] = {
		0x0F, 0x24, 0x1C, 0x0A, 0x0F, 0x08, 0x43, 0x88,
		0x32, 0x0F, 0x10, 0x06, 0x0F, 0x07, 0x00
	};
	static const uint8_t gamma_neg[] = {
		0x0F, 0x38, 0x30, 0x09, 0x0F, 0x0F, 0x4E, 0x77,
		0x3C, 0x07, 0x10, 0x05, 0x23, 0x1B, 0x00
	};

	tft_bus_init();

	/* Hardware reset pulse */
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(20U);
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
	HAL_Delay(120U);

	ili9486_write_command(SOFT_RESET);
	HAL_Delay(50U);

	ili9486_write_command(SLEEP_OUT);
	HAL_Delay(120U);

	ili9486_write_command(POWER_CONTROL_1);
	ili9486_write_data8(0x0DU);
	ili9486_write_data8(0x0DU);

	ili9486_write_command(POWER_CONTROL_2);
	ili9486_write_data8(0x43U);
	ili9486_write_data8(0x00U);

	ili9486_write_command(POWER_CONTROL_3);
	ili9486_write_data8(0x00U);

	ili9486_write_command(VCOM_CONTROL_1);
	ili9486_write_data8(0x00U);
	ili9486_write_data8(0x48U);

	ili9486_write_command(DISPLAY_FUNCTION_CONTROL);
	ili9486_write_data8(0x00U);
	ili9486_write_data8(0x22U);
	ili9486_write_data8(0x3BU);

	ili9486_write_command(PGAMCTRL);
	for (uint8_t i = 0U; i < (uint8_t)sizeof(gamma_pos); i++)
	{
		ili9486_write_data8(gamma_pos[i]);
	}

	ili9486_write_command(NGAMCTRL);
	for (uint8_t i = 0U; i < (uint8_t)sizeof(gamma_neg); i++)
	{
		ili9486_write_data8(gamma_neg[i]);
	}

	ili9486_write_command(DISPLAY_INVERSION_OFF);
	ili9486_write_command(INTERFACE_PIXEL_FORMAT);
	ili9486_write_data8(0x55U); /* 16-bit/pixel */

	ili9486_set_rotation(portrait);

	ili9486_write_command(DISPLAY_ON);
	HAL_Delay(25U);
}

void ili9486_set_rotation(uint8_t rotation)
{
	uint8_t madctl = 0x08U;

	switch (rotation)
	{
		case portrait:
			madctl = 0x08U;
			s_width = ILI9486_WIDTH;
			s_height = ILI9486_HEIGHT;
			break;

		case landscape:
			madctl = 0x68U;
			s_width = ILI9486_HEIGHT;
			s_height = ILI9486_WIDTH;
			break;

		case portraitInverted:
			madctl = 0xD8U;
			s_width = ILI9486_WIDTH;
			s_height = ILI9486_HEIGHT;
			break;

		case landscapeInverted:
			madctl = 0xA8U;
			s_width = ILI9486_HEIGHT;
			s_height = ILI9486_WIDTH;
			break;

		default:
			madctl = 0x08U;
			s_width = ILI9486_WIDTH;
			s_height = ILI9486_HEIGHT;
			rotation = portrait;
			break;
	}

	ili9486_write_command(MEMORY_ACCESS_CONTROL);
	ili9486_write_data8(madctl);
}

void ili9486_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	if ((x0 >= s_width) || (y0 >= s_height))
	{
		return;
	}

	if (x1 >= s_width)
	{
		x1 = (uint16_t)(s_width - 1U);
	}

	if (y1 >= s_height)
	{
		y1 = (uint16_t)(s_height - 1U);
	}

	if (x0 > x1)
	{
		ili9486_swap_u16(&x0, &x1);
	}

	if (y0 > y1)
	{
		ili9486_swap_u16(&y0, &y1);
	}

	ili9486_write_command(COLUMN_ADDR_SET);
	ili9486_write_u16_be(x0);
	ili9486_write_u16_be(x1);

	ili9486_write_command(PAGE_ADDR_SET);
	ili9486_write_u16_be(y0);
	ili9486_write_u16_be(y1);

	ili9486_write_command(MEMORY_WRITE);
}

void ili9486_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x >= s_width) || (y >= s_height))
	{
		return;
	}

	ili9486_set_address_window(x, y, x, y);
	ili9486_write_data16(color);
}

void ili9486_fill_screen(uint16_t color)
{
	uint32_t pixels = (uint32_t)s_width * (uint32_t)s_height;

	ili9486_set_address_window(0U, 0U, (uint16_t)(s_width - 1U), (uint16_t)(s_height - 1U));
	tft_bus_write_data16_repeat(color, pixels);
}
