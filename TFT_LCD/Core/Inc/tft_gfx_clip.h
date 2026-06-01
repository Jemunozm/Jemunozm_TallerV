/*
 * tft_gfx_clip.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#ifndef INC_TFT_GFX_CLIP_H_
#define INC_TFT_GFX_CLIP_H_

#include "tft_gfx_internal.h"

void tft_gfx_clip_reset_rect(void);
void tft_gfx_clip_set_rect(int16_t x, int16_t y, int16_t w, int16_t h);
void tft_gfx_clip_get_rect(tft_rect_t *rect);

bool tft_gfx_clip_point(int16_t x, int16_t y);
bool tft_gfx_clip_rect(int16_t *x, int16_t *y, uint16_t *w, uint16_t *h);
bool tft_gfx_clip_line(int16_t *x0, int16_t *y0, int16_t *x1, int16_t *y1);
bool tft_gfx_clip_hspan(int16_t *x, int16_t y, uint16_t *w);
bool tft_gfx_clip_vspan(int16_t x, int16_t *y, uint16_t *h);
bool tft_gfx_clip_rect_visible(int16_t x, int16_t y, uint16_t w, uint16_t h);

#endif /* INC_TFT_GFX_CLIP_H_ */
