/*
 * md5.h
 *
 * Declaration of functions and data types used for MD5 sum computing
 * library functions.
 * Copyright (C) 1995, 1996, 1997, 1999, 2000 Free Software Foundation, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 3 or later of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: md5.h,v 1.4 2007/07/02 12:46:38 ace Exp $
 */

/* License changed from the GNU LGPL to the GNU GPL (as permitted
   under Term 3 of the GNU LGPL) by Gary Wong for distribution
   with GNU Backgammon. */

#ifndef _MD5_H
#define _MD5_H 1

#include <stdio.h>

#if defined HAVE_LIMITS_H
# include <limits.h>
#endif

#ifdef _LIBC
# include <sys/types.h>
typedef u_int32_t md5_uint32;
#else
# if defined __STDC__ && __STDC__
#  define UINT_MAX_32_BITS 4294967295U
# else
#  define UINT_MAX_32_BITS 0xFFFFFFFF
# endif

# ifndef UINT_MAX
#  define UINT_MAX UINT_MAX_32_BITS
# endif

# if UINT_MAX == UINT_MAX_32_BITS
   typedef unsigned int md5_uint32;
# else
#  if USHRT_MAX == UINT_MAX_32_BITS
    typedef unsigned short md5_uint32;
#  else
#   if ULONG_MAX == UINT_MAX_32_BITS
     typedef unsigned long md5_uint32;
#   else
     "Cannot determine unsigned 32-bit data type."
#   endif
#  endif
# endif
#endif

#undef __P
#if __STDC__ || _MSC_VER
# define __P(x) x
#else
# define __P(x) ()
#endif

#if !defined (__GNUC__) && !defined (__attribute__)
#define __attribute__(x)
#endif

/* Structure to save state of computation between the single steps.  */
struct md5_ctx
{
  md5_uint32 A;
  md5_uint32 B;
  md5_uint32 C;
  md5_uint32 D;

  md5_uint32 total[2];
  md5_uint32 buflen;
  char buffer[128] __attribute__ ((__aligned__ (__alignof__ (md5_uint32))));
};

extern void md5_init_ctx __P ((struct md5_ctx *ctx));
extern void md5_process_block __P ((const void *buffer, size_t len,
				      struct md5_ctx *ctx));
extern void md5_process_bytes __P ((const void *buffer, size_t len,
				      struct md5_ctx *ctx));
extern void *md5_finish_ctx __P ((struct md5_ctx *ctx, void *resbuf));
extern void *md5_read_ctx __P ((const struct md5_ctx *ctx, void *resbuf));
extern int md5_stream __P ((FILE *stream, void *resblock));
extern void *md5_buffer __P ((const char *buffer, size_t len,
				void *resblock));

#endif 
