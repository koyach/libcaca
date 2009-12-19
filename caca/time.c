/*
 *  libcaca       Colour ASCII-Art library
 *  Copyright (c) 2002-2006 Sam Hocevar <sam@hocevar.net>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 *  This file contains simple timer routines.
 */

#include "config.h"

#if !defined(__KERNEL__)
#   include <stdlib.h>
#   if defined(HAVE_SYS_TIME_H)
#       include <sys/time.h>
#   endif
#   include <time.h>
#   if defined(USE_WIN32)
#       include <windows.h>
#   endif
#   if defined(HAVE_UNISTD_H)
#       include <unistd.h>
#   endif
#endif

#include "caca.h"
#include "caca_internals.h"

void _caca_sleep(int usec)
{
#if defined(HAVE_USLEEP)
    usleep(usec);
#elif defined(HAVE_SLEEP)
    Sleep((usec + 500) / 1000);
#else
    /* SLEEP */
#endif
}

int _caca_getticks(caca_timer_t *timer)
{
#if defined(HAVE_GETTIMEOFDAY)
    struct timeval tv;
#elif defined(USE_WIN32)
    static __int64 freq = -1; /* FIXME: can this move to caca_context? */
    __int64 usec;
#endif
    int ticks = 0;
    int new_sec, new_usec;

#if defined(HAVE_GETTIMEOFDAY)
    gettimeofday(&tv, NULL);
    new_sec = tv.tv_sec;
    new_usec = tv.tv_usec;
#elif defined(USE_WIN32)
    if(freq == -1)
    {
        if(!QueryPerformanceFrequency((LARGE_INTEGER *)&freq))
            freq = 0;
    }

    QueryPerformanceCounter((LARGE_INTEGER *)&usec);
    new_sec = (int)(usec * 1000000 / freq / 1000000);
    new_usec = (int)((usec * 1000000 / freq) % 1000000);
#endif

    if(timer->last_sec != 0)
    {
        /* If the delay was greater than 60 seconds, return 10 seconds
         * otherwise we may overflow our ticks counter. */
        if(new_sec >= timer->last_sec + 60)
            ticks = 60 * 1000000;
        else
        {
            ticks = (new_sec - timer->last_sec) * 1000000;
            ticks += new_usec;
            ticks -= timer->last_usec;
        }
    }

    timer->last_sec = new_sec;
    timer->last_usec = new_usec;

    return ticks;
}

