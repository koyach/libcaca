/*
 *  libcucul      Unicode canvas library
 *  Copyright (c) 2002-2006 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the Do What The Fuck You Want To
 *  Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/** \file triangle.c
 *  \version \$Id$
 *  \author Sam Hocevar <sam@zoy.org>
 *  \brief Triangle drawing
 *
 *  This file contains triangle drawing functions, both filled and outline.
 */

#include "config.h"

#include <stdlib.h>

#include "cucul.h"
#include "cucul_internals.h"

/**
 * \brief Draw a triangle on the screen using the given character.
 *
 * \param x1 X coordinate of the first point.
 * \param y1 Y coordinate of the first point.
 * \param x2 X coordinate of the second point.
 * \param y2 Y coordinate of the second point.
 * \param x3 X coordinate of the third point.
 * \param y3 Y coordinate of the third point.
 * \param c Character to draw the triangle outline with.
 * \return void
 */
void cucul_draw_triangle(cucul_t *qq, int x1, int y1, int x2, int y2, int x3, int y3, char c)
{
    cucul_draw_line(qq, x1, y1, x2, y2, c);
    cucul_draw_line(qq, x2, y2, x3, y3, c);
    cucul_draw_line(qq, x3, y3, x1, y1, c);
}

/**
 * \brief Draw a thin triangle on the screen.
 *
 * \param x1 X coordinate of the first point.
 * \param y1 Y coordinate of the first point.
 * \param x2 X coordinate of the second point.
 * \param y2 Y coordinate of the second point.
 * \param x3 X coordinate of the third point.
 * \param y3 Y coordinate of the third point.
 * \return void
 */
void cucul_draw_thin_triangle(cucul_t *qq, int x1, int y1, int x2, int y2, int x3, int y3)
{
    cucul_draw_thin_line(qq, x1, y1, x2, y2);
    cucul_draw_thin_line(qq, x2, y2, x3, y3);
    cucul_draw_thin_line(qq, x3, y3, x1, y1);
}

/**
 * \brief Fill a triangle on the screen using the given character.
 *
 * \param x1 X coordinate of the first point.
 * \param y1 Y coordinate of the first point.
 * \param x2 X coordinate of the second point.
 * \param y2 Y coordinate of the second point.
 * \param x3 X coordinate of the third point.
 * \param y3 Y coordinate of the third point.
 * \param c Character to fill the triangle with.
 * \return void
 */
void cucul_fill_triangle(cucul_t *qq, int x1, int y1, int x2, int y2, int x3, int y3, char c)
{
    int x, y, xa, xb, xmax, ymax;

    /* Bubble-sort y1 <= y2 <= y3 */
    if(y1 > y2)
    {
        cucul_fill_triangle(qq, x2, y2, x1, y1, x3, y3, c);
        return;
    }

    if(y2 > y3)
    {
        cucul_fill_triangle(qq, x1, y1, x3, y3, x2, y2, c);
        return;
    }

    /* Promote precision */
    x1 *= 4;
    x2 *= 4;
    x3 *= 4;

    xmax = qq->width - 1;
    ymax = qq->height - 1;

    /* Rasterize our triangle */
    for(y = y1 < 0 ? 0 : y1; y <= y3 && y <= ymax; y++)
    {
        if(y <= y2)
        {
            xa = (y1 == y2) ? x2 : x1 + (x2 - x1) * (y - y1) / (y2 - y1);
            xb = (y1 == y3) ? x3 : x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        }
        else
        {
            xa = (y3 == y2) ? x2 : x3 + (x2 - x3) * (y - y3) / (y2 - y3);
            xb = (y3 == y1) ? x1 : x3 + (x1 - x3) * (y - y3) / (y1 - y3);
        }

        if(xb < xa)
        {
            int tmp = xb;
            xb = xa; xa = tmp;
        }

        /* Rescale xa and xb, slightly cropping */
        xa = (xa + 2) / 4;
        xb = (xb - 2) / 4;

        if(xb < 0) continue;
        if(xa > xmax) continue;
        if(xa < 0) xa = 0;
        if(xb > xmax) xb = xmax;

        for(x = xa; x <= xb; x++)
            cucul_putchar(qq, x, y, c);
    }
}
