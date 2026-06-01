/*
 * tft_gfx_primitives.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#include <math.h>

#include "tft_gfx_primitives.h"
#include "ili9486.h"
#include "tft_gfx_clip.h"

static void tft_gfx_primitives_plot_ellipse_points(int16_t cx, int16_t cy, int32_t x, int32_t y, uint16_t color);

static void tft_gfx_primitives_plot_ellipse_points(int16_t cx, int16_t cy, int32_t x, int32_t y, uint16_t color)
{
	tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx + x), (int16_t)((int32_t)cy + y), color);
	tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx - x), (int16_t)((int32_t)cy + y), color);
	tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx + x), (int16_t)((int32_t)cy - y), color);
	tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx - x), (int16_t)((int32_t)cy - y), color);
}

void tft_gfx_primitives_draw_pixel_impl(int16_t x, int16_t y, uint16_t color)
{
	if (!tft_gfx_clip_point(x, y))
	{
		return;
	}

	ili9486_draw_pixel((uint16_t)x, (uint16_t)y, color);
}

void tft_gfx_primitives_draw_line_impl(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	int32_t dx;
	int32_t sx;
	int32_t dy;
	int32_t sy;
	int32_t err;
	int32_t e2;

	if (!tft_gfx_clip_line(&x0, &y0, &x1, &y1))
	{
		return;
	}

	dx = (int32_t)x1 - (int32_t)x0;
	sx = (x0 < x1) ? 1 : -1;
	dy = -((int32_t)y1 - (int32_t)y0);
	sy = (y0 < y1) ? 1 : -1;
	err = dx + dy;

	for (;;)
	{
		tft_gfx_primitives_draw_pixel_impl(x0, y0, color);

		if ((x0 == x1) && (y0 == y1))
		{
			break;
		}

		e2 = err << 1;
		if (e2 >= dy)
		{
			err += dy;
			x0 = (int16_t)((int32_t)x0 + sx);
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 = (int16_t)((int32_t)y0 + sy);
		}
	}
}

void tft_gfx_primitives_draw_circle_impl(int16_t cx, int16_t cy, uint16_t r, uint16_t color)
{
	int32_t f = 1 - (int32_t)r;
	int32_t ddF_x = 1;
	int32_t ddF_y = -2 * (int32_t)r;
	int32_t x = 0;
	int32_t y = (int32_t)r;

	if (!tft_gfx_clip_rect_visible((int16_t)(cx - (int32_t)r), (int16_t)(cy - (int32_t)r), (uint16_t)(r * 2U + 1U), (uint16_t)(r * 2U + 1U)))
	{
		return;
	}

	tft_gfx_primitives_draw_pixel_impl(cx, (int16_t)((int32_t)cy + r), color);
	tft_gfx_primitives_draw_pixel_impl(cx, (int16_t)((int32_t)cy - r), color);
	tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx + r), cy, color);
	tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx - r), cy, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx + x), (int16_t)((int32_t)cy + y), color);
		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx - x), (int16_t)((int32_t)cy + y), color);
		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx + x), (int16_t)((int32_t)cy - y), color);
		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx - x), (int16_t)((int32_t)cy - y), color);
		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx + y), (int16_t)((int32_t)cy + x), color);
		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx - y), (int16_t)((int32_t)cy + x), color);
		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx + y), (int16_t)((int32_t)cy - x), color);
		tft_gfx_primitives_draw_pixel_impl((int16_t)((int32_t)cx - y), (int16_t)((int32_t)cy - x), color);
	}
}

void tft_gfx_primitives_draw_ellipse_impl(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color)
{
	double rx2;
	double ry2;
	double p;
	double px;
	double py;
	int32_t x = 0;
	int32_t y = (int32_t)ry;
	int32_t twoRx2;
	int32_t twoRy2;

	if ((rx == 0U) || (ry == 0U))
	{
		if (rx == 0U)
		{
			tft_gfx_primitives_draw_line_impl(cx, (int16_t)(cy - (int16_t)ry), cx, (int16_t)(cy + (int16_t)ry), color);
		}
		else
		{
			tft_gfx_primitives_draw_line_impl((int16_t)(cx - (int16_t)rx), cy, (int16_t)(cx + (int16_t)rx), cy, color);
		}
		return;
	}

	if (!tft_gfx_clip_rect_visible((int16_t)(cx - (int32_t)rx), (int16_t)(cy - (int32_t)ry), (uint16_t)(rx * 2U + 1U), (uint16_t)(ry * 2U + 1U)))
	{
		return;
	}

	rx2 = (double)rx * (double)rx;
	ry2 = (double)ry * (double)ry;
	twoRx2 = (int32_t)(2U * rx * rx);
	twoRy2 = (int32_t)(2U * ry * ry);
	px = 0.0;
	py = (double)twoRx2 * (double)y;
	p = ry2 - (rx2 * (double)ry) + (0.25 * rx2);

	while (px < py)
	{
		tft_gfx_primitives_plot_ellipse_points(cx, cy, x, y, color);
		x++;
		px += twoRy2;
		if (p < 0.0)
		{
			p += ry2 + px;
		}
		else
		{
			y--;
			py -= twoRx2;
			p += ry2 + px - py;
		}
	}

	p = (ry2 * ((double)x + 0.5) * ((double)x + 0.5)) + (rx2 * ((double)y - 1.0) * ((double)y - 1.0)) - (rx2 * ry2);
	while (y >= 0)
	{
		tft_gfx_primitives_plot_ellipse_points(cx, cy, x, y, color);
		y--;
		py -= twoRx2;
		if (p > 0.0)
		{
			p += rx2 - py;
		}
		else
		{
			x++;
			px += twoRy2;
			p += rx2 - py + px;
		}
	}
}

void tft_gfx_primitives_draw_triangle_impl(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color)
{
	tft_gfx_primitives_draw_line_impl(p0.x, p0.y, p1.x, p1.y, color);
	tft_gfx_primitives_draw_line_impl(p1.x, p1.y, p2.x, p2.y, color);
	tft_gfx_primitives_draw_line_impl(p2.x, p2.y, p0.x, p0.y, color);
}

void tft_gfx_primitives_draw_polygon_impl(const tft_point_t *points, uint16_t count, uint16_t color)
{
	uint16_t i;

	if ((points == NULL) || (count < 2U))
	{
		return;
	}

	for (i = 0U; i < (uint16_t)(count - 1U); i++)
	{
		tft_gfx_primitives_draw_line_impl(points[i].x, points[i].y, points[i + 1U].x, points[i + 1U].y, color);
	}

	tft_gfx_primitives_draw_line_impl(points[count - 1U].x, points[count - 1U].y, points[0U].x, points[0U].y, color);
}
