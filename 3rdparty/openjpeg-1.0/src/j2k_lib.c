/*
 * Copyright (c) 2005, Herv� Drolon, FreeImage Team
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/times.h>
#endif /* WIN32 */
#include "opj_includes.h"

double opj_clock() {
#ifdef WIN32
	/* WIN32: use QueryPerformance (very accurate) */
    LARGE_INTEGER freq , t ;
    /* freq is the clock speed of the CPU */
    QueryPerformanceFrequency(&freq) ;
	/* cout << "freq = " << ((double) freq.QuadPart) << endl; */
    /* t is the high resolution performance counter (see MSDN) */
    QueryPerformanceCounter ( & t ) ;
    return ( t.QuadPart /(double) freq.QuadPart ) ;
#else
	/* Unix or Linux: use resource usage */
    struct rusage t;
    double procTime;
    /* (1) Get the rusage data structure at this moment (man getrusage) */
    getrusage(0,&t);
    /* (2) What is the elapsed time ? - CPU time = User time + System time */
	/* (2a) Get the seconds */
    procTime = t.ru_utime.tv_sec + t.ru_stime.tv_sec;
    /* (2b) More precisely! Get the microseconds part ! */
    return ( procTime + (t.ru_utime.tv_usec + t.ru_stime.tv_usec) * 1e-6 ) ;
#endif
}

void* opj_malloc( size_t size ) {
	void *memblock = malloc(size);
	if(memblock) {
		memset(memblock, 0, size);
	}
	return memblock;
}

void* j2k_realloc( void *memblock, size_t size ) {
	return realloc(memblock, size);
}

void opj_free( void *memblock ) {
	free(memblock);
}

