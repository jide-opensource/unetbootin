/*
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

/* This prefix file is for use on Mac OS X and Windows only. It should contain only:
 *    1) files to precompile on Mac OS X and Windows for faster builds
 *    2) in one case at least: OS-X-specific performance bug workarounds
 *    3) the special trick to catch us using new or delete without including "config.h"
 * The project should be able to build without this header, although we rarely test that.
 */


/* Things that need to be defined globally should go into "config.h". */

#if defined(__APPLE__)
#ifdef __cplusplus
#define NULL __null
#else
#define NULL ((void *)0)
#endif
#endif

#if defined(WIN32) || defined(_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_ // Prevent inclusion of winsock.h in windows.h
#endif

// If we don't define these, they get defined in windef.h. 
// We want to use std::min and std::max
#ifdef __cplusplus
#define max max
#define min min
#endif

#endif // defined(WIN32) || defined(_WIN32)

#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#if defined(__APPLE__)
#include <regex.h>
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(__APPLE__)
#include <unistd.h>
#endif

#ifdef __cplusplus

#include <algorithm>
#include <cstddef>
#include <new>

/* Work around bug 3553309 by re-including <ctype.h>. */
#include <cctype>
#if defined(__APPLE__)
#define isalnum(c)      __istype((c), (_CTYPE_A|_CTYPE_D))
#define isalpha(c)      __istype((c), _CTYPE_A)
#define iscntrl(c)      __istype((c), _CTYPE_C)
#define isdigit(c)      __isctype((c), _CTYPE_D)        /* ANSI -- locale independent */
#define isgraph(c)      __istype((c), _CTYPE_G)
#define islower(c)      __istype((c), _CTYPE_L)
#define isprint(c)      __istype((c), _CTYPE_R)
#define ispunct(c)      __istype((c), _CTYPE_P)
#define isspace(c)      __istype((c), _CTYPE_S)
#define isupper(c)      __istype((c), _CTYPE_U)
#define isxdigit(c)     __isctype((c), _CTYPE_X)        /* ANSI -- locale independent */
#define tolower(c)      __tolower(c)
#define toupper(c)      __toupper(c)
#endif

#endif

#include <sys/types.h>
#if defined(__APPLE__)
#include <sys/param.h>
#endif
#include <sys/stat.h>
#if defined(__APPLE__)
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <time.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>

#include <AvailabilityMacros.h>

#if defined(__APPLE__)
#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_4
#define BUILDING_ON_TIGER 1
#endif
#endif

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

#ifdef __cplusplus
#define new ("if you use new/delete make sure to include config.h at the top of the file"()) 
#define delete ("if you use new/delete make sure to include config.h at the top of the file"()) 
#endif

/* Work around bug with C++ library that screws up Objective-C++ when exception support is disabled. */
#if defined(__APPLE__)
#undef try
#undef catch
#endif
