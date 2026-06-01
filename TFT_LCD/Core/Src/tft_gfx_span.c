/*
 * tft_gfx_span.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#include "tft_gfx_span.h"
#include "ili9486.h"
#include "tft_bus_8080_16.h"
#include "tft_gfx_clip.h"

void tft_gfx_span_draw_hspan_impl(int16_t x, int16_t y, uint16_t w, uint16_t color)
{
	int32_t x1;

	if (!tft_gfx_clip_hspan(&x, y, &w))
	{
		return;
	}

	x1 = (int32_t)x + (int32_t)w - 1L;
	ili9486_set_address_window((uint16_t)x, (uint16_t)y, (uint16_t)x1, (uint16_t)y);
	tft_bus_write_data16_repeat(color, w);
}

void tft_gfx_span_draw_vspan_impl(int16_t x, int16_t y, uint16_t h, uint16_t color)
{
	int32_t y1;

	if (!tft_gfx_clip_vspan(x, &y, &h))
	{
		return;
	}

	y1 = (int32_t)y + (int32_t)h - 1L;
	ili9486_set_address_window((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y1);
	tft_bus_write_data16_repeat(color, h);
}

void tft_gfx_span_fill_rect_impl(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	uint32_t pixels;

	if (!tft_gfx_clip_rect(&x, &y, &w, &h))
	{
		return;
	}

	pixels = (uint32_t)w * (uint32_t)h;
	ili9486_set_address_window((uint16_t)x, (uint16_t)y,
							   (uint16_t)((int32_t)x + (int32_t)w - 1L),
							   (uint16_t)((int32_t)y + (int32_t)h - 1L));
	tft_bus_write_data16_repeat(color, pixels);
}
