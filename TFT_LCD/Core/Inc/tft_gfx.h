/*
 * tft_gfx.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#ifndef INC_TFT_GFX_H_
#define INC_TFT_GFX_H_

#include <stdint.h>

typedef struct
{
	int16_t x;
	int16_t y;
} tft_point_t;

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t w;
	int16_t h;
} tft_rect_t;

void tft_gfx_set_clip_rect(int16_t x, int16_t y, int16_t w, int16_t h);
void tft_gfx_reset_clip_rect(void);
void tft_gfx_get_clip_rect(tft_rect_t *rect);

uint16_t tft_gfx_color565(uint8_t r, uint8_t g, uint8_t b);

void tft_gfx_draw_pixel(int16_t x, int16_t y, uint16_t color);
void tft_gfx_draw_hspan(int16_t x, int16_t y, uint16_t w, uint16_t color);
void tft_gfx_draw_vspan(int16_t x, int16_t y, uint16_t h, uint16_t color);

void tft_gfx_fill_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color);
void tft_gfx_draw_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color);
void tft_gfx_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

void tft_gfx_draw_circle(int16_t cx, int16_t cy, uint16_t r, uint16_t color);
void tft_gfx_fill_circle(int16_t cx, int16_t cy, uint16_t r, uint16_t color);
void tft_gfx_draw_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color);
void tft_gfx_fill_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color);

void tft_gfx_draw_triangle(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color);
void tft_gfx_fill_triangle(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color);
void tft_gfx_draw_polygon(const tft_point_t *points, uint16_t count, uint16_t color);
void tft_gfx_fill_polygon(const tft_point_t *points, uint16_t count, uint16_t color);

void tft_gfx_fill_screen(uint16_t color);

#endif /* INC_TFT_GFX_H_ */
