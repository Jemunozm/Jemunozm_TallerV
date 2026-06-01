/*
 * tft_gfx_clip.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#include "tft_gfx_clip.h"
#include "ili9486.h"

static tft_rect_t s_clip_rect = {0, 0, (int16_t)ILI9486_WIDTH, (int16_t)ILI9486_HEIGHT};

static int16_t tft_gfx_clip_min_i16(int16_t a, int16_t b);
static int16_t tft_gfx_clip_max_i16(int16_t a, int16_t b);

static int16_t tft_gfx_clip_min_i16(int16_t a, int16_t b)
{
	return (a < b) ? a : b;
}

static int16_t tft_gfx_clip_max_i16(int16_t a, int16_t b)
{
	return (a > b) ? a : b;
}

void tft_gfx_clip_reset_rect(void)
{
	s_clip_rect.x = 0;
	s_clip_rect.y = 0;
	s_clip_rect.w = (int16_t)ILI9486_WIDTH;
	s_clip_rect.h = (int16_t)ILI9486_HEIGHT;
}

void tft_gfx_clip_set_rect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	if ((w <= 0) || (h <= 0))
	{
		tft_gfx_clip_reset_rect();
		return;
	}

	s_clip_rect.x = x;
	s_clip_rect.y = y;
	s_clip_rect.w = w;
	s_clip_rect.h = h;
}

void tft_gfx_clip_get_rect(tft_rect_t *rect)
{
	if (rect != NULL)
	{
		*rect = s_clip_rect;
	}
}

bool tft_gfx_clip_point(int16_t x, int16_t y)
{
	int32_t clip_x1 = (int32_t)s_clip_rect.x + (int32_t)s_clip_rect.w - 1L;
	int32_t clip_y1 = (int32_t)s_clip_rect.y + (int32_t)s_clip_rect.h - 1L;

	return ((int32_t)x >= s_clip_rect.x) && ((int32_t)x <= clip_x1) &&
		   ((int32_t)y >= s_clip_rect.y) && ((int32_t)y <= clip_y1);
}

bool tft_gfx_clip_rect_visible(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
	int16_t rx = x;
	int16_t ry = y;
	uint16_t rw = w;
	uint16_t rh = h;

	return tft_gfx_clip_rect(&rx, &ry, &rw, &rh);
}

bool tft_gfx_clip_rect(int16_t *x, int16_t *y, uint16_t *w, uint16_t *h)
{
	int32_t x0;
	int32_t y0;
	int32_t x1;
	int32_t y1;
	int32_t clip_x0;
	int32_t clip_y0;
	int32_t clip_x1;
	int32_t clip_y1;

	if ((x == NULL) || (y == NULL) || (w == NULL) || (h == NULL))
	{
		return false;
	}

	if ((*w == 0U) || (*h == 0U))
	{
		return false;
	}

	x0 = *x;
	y0 = *y;
	x1 = x0 + (int32_t)(*w) - 1L;
	y1 = y0 + (int32_t)(*h) - 1L;

	clip_x0 = s_clip_rect.x;
	clip_y0 = s_clip_rect.y;
	clip_x1 = (int32_t)s_clip_rect.x + (int32_t)s_clip_rect.w - 1L;
	clip_y1 = (int32_t)s_clip_rect.y + (int32_t)s_clip_rect.h - 1L;

	if ((x1 < clip_x0) || (x0 > clip_x1) || (y1 < clip_y0) || (y0 > clip_y1))
	{
		return false;
	}

	x0 = tft_gfx_clip_max_i16((int16_t)x0, (int16_t)clip_x0);
	y0 = tft_gfx_clip_max_i16((int16_t)y0, (int16_t)clip_y0);
	x1 = tft_gfx_clip_min_i16((int16_t)x1, (int16_t)clip_x1);
	y1 = tft_gfx_clip_min_i16((int16_t)y1, (int16_t)clip_y1);

	if ((x1 < x0) || (y1 < y0))
	{
		return false;
	}

	*x = (int16_t)x0;
	*y = (int16_t)y0;
	*w = (uint16_t)(x1 - x0 + 1L);
	*h = (uint16_t)(y1 - y0 + 1L);
	return true;
}

bool tft_gfx_clip_hspan(int16_t *x, int16_t y, uint16_t *w)
{
	int32_t x0;
	int32_t x1;
	int32_t clip_x0;
	int32_t clip_x1;

	if ((x == NULL) || (w == NULL) || (*w == 0U))
	{
		return false;
	}

	clip_x0 = s_clip_rect.x;
	clip_x1 = (int32_t)s_clip_rect.x + (int32_t)s_clip_rect.w - 1L;

	if (((int32_t)y < s_clip_rect.y) || ((int32_t)y > ((int32_t)s_clip_rect.y + (int32_t)s_clip_rect.h - 1L)))
	{
		return false;
	}

	x0 = *x;
	x1 = x0 + (int32_t)(*w) - 1L;

	if ((x1 < clip_x0) || (x0 > clip_x1))
	{
		return false;
	}

	x0 = tft_gfx_clip_max_i16((int16_t)x0, (int16_t)clip_x0);
	x1 = tft_gfx_clip_min_i16((int16_t)x1, (int16_t)clip_x1);

	if (x1 < x0)
	{
		return false;
	}

	*x = (int16_t)x0;
	*w = (uint16_t)(x1 - x0 + 1L);
	return true;
}

bool tft_gfx_clip_vspan(int16_t x, int16_t *y, uint16_t *h)
{
	int32_t y0;
	int32_t y1;
	int32_t clip_y0;
	int32_t clip_y1;

	if ((y == NULL) || (h == NULL) || (*h == 0U))
	{
		return false;
	}

	clip_y0 = s_clip_rect.y;
	clip_y1 = (int32_t)s_clip_rect.y + (int32_t)s_clip_rect.h - 1L;

	if (((int32_t)x < s_clip_rect.x) || ((int32_t)x > ((int32_t)s_clip_rect.x + (int32_t)s_clip_rect.w - 1L)))
	{
		return false;
	}

	y0 = *y;
	y1 = y0 + (int32_t)(*h) - 1L;

	if ((y1 < clip_y0) || (y0 > clip_y1))
	{
		return false;
	}

	y0 = tft_gfx_clip_max_i16((int16_t)y0, (int16_t)clip_y0);
	y1 = tft_gfx_clip_min_i16((int16_t)y1, (int16_t)clip_y1);

	if (y1 < y0)
	{
		return false;
	}

	*y = (int16_t)y0;
	*h = (uint16_t)(y1 - y0 + 1L);
	return true;
}

bool tft_gfx_clip_line(int16_t *x0, int16_t *y0, int16_t *x1, int16_t *y1)
{
	int16_t x0_in;
	int16_t y0_in;
	int16_t x1_in;
	int16_t y1_in;
	float p[4];
	float q[4];
	float u1 = 0.0f;
	float u2 = 1.0f;
	float dx;
	float dy;
	float u;
	int32_t clip_x0 = s_clip_rect.x;
	int32_t clip_y0 = s_clip_rect.y;
	int32_t clip_x1 = (int32_t)s_clip_rect.x + (int32_t)s_clip_rect.w - 1L;
	int32_t clip_y1 = (int32_t)s_clip_rect.y + (int32_t)s_clip_rect.h - 1L;

	if ((x0 == NULL) || (y0 == NULL) || (x1 == NULL) || (y1 == NULL))
	{
		return false;
	}

	x0_in = *x0;
	y0_in = *y0;
	x1_in = *x1;
	y1_in = *y1;

	dx = (float)((int32_t)x1_in - (int32_t)x0_in);
	dy = (float)((int32_t)y1_in - (int32_t)y0_in);

	p[0] = -dx;
	p[1] = dx;
	p[2] = -dy;
	p[3] = dy;

	q[0] = (float)((int32_t)x0_in - clip_x0);
	q[1] = (float)(clip_x1 - (int32_t)x0_in);
	q[2] = (float)((int32_t)y0_in - clip_y0);
	q[3] = (float)(clip_y1 - (int32_t)y0_in);

	for (uint8_t i = 0U; i < 4U; i++)
	{
		if (p[i] == 0.0f)
		{
			if (q[i] < 0.0f)
			{
				return false;
			}
		}
		else
		{
			u = q[i] / p[i];
			if (p[i] < 0.0f)
			{
				if (u > u2)
				{
					return false;
				}
				if (u > u1)
				{
					u1 = u;
				}
			}
			else
			{
				if (u < u1)
				{
					return false;
				}
				if (u < u2)
				{
					u2 = u;
				}
			}
		}
	}

	if (u2 < u1)
	{
		return false;
	}

	*x0 = (int16_t)((float)x0_in + (u1 * dx) + (((u1 * dx) >= 0.0f) ? 0.5f : -0.5f));
	*y0 = (int16_t)((float)y0_in + (u1 * dy) + (((u1 * dy) >= 0.0f) ? 0.5f : -0.5f));
	*x1 = (int16_t)((float)x0_in + (u2 * dx) + (((u2 * dx) >= 0.0f) ? 0.5f : -0.5f));
	*y1 = (int16_t)((float)y0_in + (u2 * dy) + (((u2 * dy) >= 0.0f) ? 0.5f : -0.5f));

	return true;
}
