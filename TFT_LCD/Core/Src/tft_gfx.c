/*
 * tft_gfx.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#include "tft_gfx.h"
#include "ili9486.h"
#include "tft_gfx_clip.h"
#include "tft_gfx_fill.h"
#include "tft_gfx_primitives.h"
#include "tft_gfx_span.h"

uint16_t tft_gfx_color565(uint8_t r, uint8_t g, uint8_t b)
{
	return (uint16_t)(((uint16_t)(r & 0xF8U) << 8U) |
					  ((uint16_t)(g & 0xFCU) << 3U) |
					  ((uint16_t)(b >> 3U)));
}

void tft_gfx_set_clip_rect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	tft_gfx_clip_set_rect(x, y, w, h);
}

void tft_gfx_reset_clip_rect(void)
{
	tft_gfx_clip_reset_rect();
}

void tft_gfx_get_clip_rect(tft_rect_t *rect)
{
	tft_gfx_clip_get_rect(rect);
}

void tft_gfx_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
	tft_gfx_primitives_draw_pixel_impl(x, y, color);
}

void tft_gfx_draw_hspan(int16_t x, int16_t y, uint16_t w, uint16_t color)
{
	tft_gfx_span_draw_hspan_impl(x, y, w, color);
}

void tft_gfx_draw_vspan(int16_t x, int16_t y, uint16_t h, uint16_t color)
{
	tft_gfx_span_draw_vspan_impl(x, y, h, color);
}

void tft_gfx_fill_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	tft_gfx_fill_rect_impl(x, y, w, h, color);
}

void tft_gfx_draw_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	if ((w == 0U) || (h == 0U))
	{
		return;
	}

	tft_gfx_draw_hspan(x, y, w, color);
	tft_gfx_draw_hspan(x, (int16_t)(y + (int16_t)h - 1), w, color);
	tft_gfx_draw_vspan(x, y, h, color);
	tft_gfx_draw_vspan((int16_t)(x + (int16_t)w - 1), y, h, color);
}

void tft_gfx_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	tft_gfx_primitives_draw_line_impl(x0, y0, x1, y1, color);
}

void tft_gfx_draw_circle(int16_t cx, int16_t cy, uint16_t r, uint16_t color)
{
	tft_gfx_primitives_draw_circle_impl(cx, cy, r, color);
}

void tft_gfx_fill_circle(int16_t cx, int16_t cy, uint16_t r, uint16_t color)
{
	tft_gfx_fill_circle_impl(cx, cy, r, color);
}

void tft_gfx_draw_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color)
{
	tft_gfx_primitives_draw_ellipse_impl(cx, cy, rx, ry, color);
}

void tft_gfx_fill_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color)
{
	tft_gfx_fill_ellipse_impl(cx, cy, rx, ry, color);
}

void tft_gfx_draw_triangle(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color)
{
	tft_gfx_primitives_draw_triangle_impl(p0, p1, p2, color);
}

void tft_gfx_fill_triangle(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color)
{
	tft_gfx_fill_triangle_impl(p0, p1, p2, color);
}

void tft_gfx_draw_polygon(const tft_point_t *points, uint16_t count, uint16_t color)
{
	tft_gfx_primitives_draw_polygon_impl(points, count, color);
}

void tft_gfx_fill_polygon(const tft_point_t *points, uint16_t count, uint16_t color)
{
	tft_gfx_fill_polygon_impl(points, count, color);
}

void tft_gfx_fill_screen(uint16_t color)
{
	tft_gfx_fill_rect_impl(0, 0, (uint16_t)ILI9486_WIDTH, (uint16_t)ILI9486_HEIGHT, color);
}
