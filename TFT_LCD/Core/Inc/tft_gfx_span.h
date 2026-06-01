/*
 * tft_gfx_span.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#ifndef INC_TFT_GFX_SPAN_H_
#define INC_TFT_GFX_SPAN_H_

#include "tft_gfx_internal.h"

void tft_gfx_span_draw_hspan_impl(int16_t x, int16_t y, uint16_t w, uint16_t color);
void tft_gfx_span_draw_vspan_impl(int16_t x, int16_t y, uint16_t h, uint16_t color);
void tft_gfx_span_fill_rect_impl(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color);

#endif /* INC_TFT_GFX_SPAN_H_ */
