/*
 * ili9486.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. MuÃ±oz
 */

#include "ili9486.h"
#include "tft_bus_8080_16.h"

void ili9486_init(void)
{
	/* Pendiente: secuencia completa de inicio del ILI9486. */
}

void ili9486_set_rotation(uint8_t rotation)
{
	/* Pendiente: configuracion de MADCTL segun la orientacion elegida. */
	(void)rotation;
}

void ili9486_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	/* Pendiente: envio de CASET, PASET y RAMWR. */
	(void)x0;
	(void)y0;
	(void)x1;
	(void)y1;
}

void ili9486_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	/* Pendiente: escritura de un pixel usando la ventana de direccionamiento. */
	(void)x;
	(void)y;
	(void)color;
}

void ili9486_fill_screen(uint16_t color)
{
	/* Pendiente: relleno eficiente del panel con color uniforme. */
	(void)color;
}
