/*
 * tft_gfx_fill.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#ifndef INC_TFT_GFX_FILL_H_
#define INC_TFT_GFX_FILL_H_

#include "tft_gfx_internal.h"

void tft_gfx_fill_rect_impl(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color);
void tft_gfx_fill_circle_impl(int16_t cx, int16_t cy, uint16_t r, uint16_t color);
void tft_gfx_fill_ellipse_impl(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color);
void tft_gfx_fill_triangle_impl(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color);
void tft_gfx_fill_polygon_impl(const tft_point_t *points, uint16_t count, uint16_t color);

#endif /* INC_TFT_GFX_FILL_H_ */
