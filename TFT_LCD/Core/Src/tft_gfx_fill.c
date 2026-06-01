/*
 * tft_gfx_fill.c
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#include <math.h>

#include "tft_gfx_fill.h"
#include "tft_gfx_clip.h"
#include "tft_gfx_span.h"

void tft_gfx_fill_rect_impl(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	tft_gfx_span_fill_rect_impl(x, y, w, h, color);
}

void tft_gfx_fill_circle_impl(int16_t cx, int16_t cy, uint16_t r, uint16_t color)
{
	int32_t f = 1 - (int32_t)r;
	int32_t ddF_x = 1;
	int32_t ddF_y = -2 * (int32_t)r;
	int32_t x = 0;
	int32_t y = (int32_t)r;
	uint16_t span_width;

	if (!tft_gfx_clip_rect_visible((int16_t)(cx - (int32_t)r), (int16_t)(cy - (int32_t)r), (uint16_t)(r * 2U + 1U), (uint16_t)(r * 2U + 1U)))
	{
		return;
	}

	span_width = (uint16_t)(r * 2U + 1U);
	tft_gfx_span_draw_hspan_impl((int16_t)(cx - (int32_t)r), cy, span_width, color);

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

		tft_gfx_span_draw_hspan_impl((int16_t)(cx - x), (int16_t)(cy + y), (uint16_t)((x << 1U) + 1U), color);
		tft_gfx_span_draw_hspan_impl((int16_t)(cx - x), (int16_t)(cy - y), (uint16_t)((x << 1U) + 1U), color);

		if (y != x)
		{
			tft_gfx_span_draw_hspan_impl((int16_t)(cx - y), (int16_t)(cy + x), (uint16_t)((y << 1U) + 1U), color);
			tft_gfx_span_draw_hspan_impl((int16_t)(cx - y), (int16_t)(cy - x), (uint16_t)((y << 1U) + 1U), color);
		}
	}
}

void tft_gfx_fill_ellipse_impl(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint16_t color)
{
	int32_t y;

	if ((rx == 0U) || (ry == 0U))
	{
		if (rx == 0U)
		{
			tft_gfx_span_draw_vspan_impl(cx, (int16_t)(cy - (int16_t)ry), (uint16_t)((ry << 1U) + 1U), color);
		}
		else
		{
			tft_gfx_span_draw_hspan_impl((int16_t)(cx - (int16_t)rx), cy, (uint16_t)((rx << 1U) + 1U), color);
		}
		return;
	}

	if (!tft_gfx_clip_rect_visible((int16_t)(cx - (int32_t)rx), (int16_t)(cy - (int32_t)ry), (uint16_t)(rx * 2U + 1U), (uint16_t)(ry * 2U + 1U)))
	{
		return;
	}

	for (y = -(int32_t)ry; y <= (int32_t)ry; y++)
	{
		double normalized = 1.0 - (((double)y * (double)y) / ((double)ry * (double)ry));
		int32_t x;

		if (normalized < 0.0)
		{
			continue;
		}

		x = (int32_t)sqrt(normalized * ((double)rx * (double)rx));
		tft_gfx_span_draw_hspan_impl((int16_t)((int32_t)cx - x), (int16_t)((int32_t)cy + y), (uint16_t)((x << 1U) + 1U), color);
	}
}

void tft_gfx_fill_triangle_impl(tft_point_t p0, tft_point_t p1, tft_point_t p2, uint16_t color)
{
	tft_point_t points[3];

	points[0] = p0;
	points[1] = p1;
	points[2] = p2;

	tft_gfx_fill_polygon_impl(points, 3U, color);
}

void tft_gfx_fill_polygon_impl(const tft_point_t *points, uint16_t count, uint16_t color)
{
	int16_t min_y;
	int16_t max_y;
	int16_t clip_min_y;
	int16_t clip_max_y;
	int16_t intersections[TFT_GFX_MAX_POLYGON_INTERSECTIONS];
	uint16_t intersection_count;
	uint16_t i;
	int16_t y;

	if ((points == NULL) || (count < 3U) || (count > TFT_GFX_MAX_POLYGON_POINTS))
	{
		return;
	}

	min_y = points[0].y;
	max_y = points[0].y;

	for (i = 1U; i < count; i++)
	{
		if (points[i].y < min_y)
		{
			min_y = points[i].y;
		}
		if (points[i].y > max_y)
		{
			max_y = points[i].y;
		}
	}

	{
		tft_rect_t clip;
		tft_gfx_clip_get_rect(&clip);
		clip_min_y = clip.y;
		clip_max_y = (int16_t)(clip.y + clip.h - 1);
	}

	if ((max_y < clip_min_y) || (min_y > clip_max_y))
	{
		return;
	}

	if (min_y < clip_min_y)
	{
		min_y = clip_min_y;
	}
	if (max_y > clip_max_y)
	{
		max_y = clip_max_y;
	}

	for (y = min_y; y <= max_y; y++)
	{
		intersection_count = 0U;

		for (i = 0U; i < count; i++)
		{
			const tft_point_t *a = &points[i];
			const tft_point_t *b = &points[(i + 1U) % count];
			int16_t y_min_edge;
			int16_t y_max_edge;
			double x_intersection;

			if (a->y == b->y)
			{
				continue;
			}

			y_min_edge = (a->y < b->y) ? a->y : b->y;
			y_max_edge = (a->y > b->y) ? a->y : b->y;

			if ((y < y_min_edge) || (y >= y_max_edge))
			{
				continue;
			}

			x_intersection = (double)a->x + (((double)(y - a->y) * (double)(b->x - a->x)) / (double)(b->y - a->y));
			if (intersection_count < TFT_GFX_MAX_POLYGON_INTERSECTIONS)
			{
				intersections[intersection_count++] = (int16_t)((x_intersection >= 0.0) ? (x_intersection + 0.5) : (x_intersection - 0.5));
			}
		}

		for (i = 0U; i + 1U < intersection_count; i += 2U)
		{
			int16_t x_start = intersections[i];
			int16_t x_end = intersections[i + 1U];
			uint16_t span_width;

			if (x_start > x_end)
			{
				int16_t temp = x_start;
				x_start = x_end;
				x_end = temp;
			}

			if (!tft_gfx_clip_hspan(&x_start, y, &span_width))
			{
				continue;
			}

			if ((int32_t)x_start + (int32_t)span_width - 1L > (int32_t)x_end)
			{
				span_width = (uint16_t)((int32_t)x_end - (int32_t)x_start + 1L);
			}

			if (span_width > 0U)
			{
				tft_gfx_span_draw_hspan_impl(x_start, y, span_width, color);
			}
		}
	}
}
