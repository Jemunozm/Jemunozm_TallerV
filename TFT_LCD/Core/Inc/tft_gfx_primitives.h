/*
 * tft_gfx_primitives.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#ifndef INC_TFT_GFX_PRIMITIVES_H_
#define INC_TFT_GFX_PRIMITIVES_H_

#include "tft_gfx_internal.h"

void tft_gfx_primitives_draw_pixel_impl(int16_t x, int16_t y, uint16_t color);
void tft_gfx_primitives_draw_line_impl(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void tft_gfx_primitives_draw_circle_impl(int16_t cx, int16_t cy, uint16_t r, uint16_t color);
void tft_gfx_primitives_draw_ellipse_impl(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color);
void tft_gfx_primitives_draw_triangle_impl(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color);
void tft_gfx_primitives_draw_polygon_impl(const tft_point_t *points, uint16_t count, uint16_t color);

#endif /* INC_TFT_GFX_PRIMITIVES_H_ */
