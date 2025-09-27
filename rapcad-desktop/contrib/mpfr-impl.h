/* Utilities for MPFR developers, not exported.

Copyright 1999-2020 Free Software Foundation, Inc.
Contributed by the AriC and Caramba projects, INRIA.

This file is part of the GNU MPFR Library.

The GNU MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MPFR Library; see the file COPYING.LESSER.  If not, see
https://www.gnu.org/licenses/ or write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef __MPFR_IMPL_H__
#define __MPFR_IMPL_H__

#include <mpfr.h>

#ifdef MPFR_WIN_THREAD_SAFE_DLL
__MPFR_DECLSPEC mpfr_flags_t * __gmpfr_flags_f (void);
#define __gmpfr_flags (*__gmpfr_flags_f())
#define MPFR_SET_ERANGEFLAG() \
   do __gmpfr_flags |= MPFR_FLAGS_ERANGE; while (0)
#else
#define MPFR_SET_ERANGEFLAG() ((void) 0)
#endif

#define MPFR_UNLIKELY(x) (x)
#define MPFR_EXP(x) ((x)->_mpfr_exp)
#define MPFR_EXP_MIN (LONG_MIN)
#define MPFR_EXP_ZERO (MPFR_EXP_MIN+1)
#define MPFR_EXP_INF  (MPFR_EXP_MIN+3)
#define MPFR_IS_SINGULAR(x) (MPFR_EXP(x) <= MPFR_EXP_INF)
#define MPFR_NOTZERO(x) (MPFR_EXP(x) != MPFR_EXP_ZERO)

#define MPFR_ASSERTN(expr)  MPFR_ASSUME (expr)
#define MPFR_ASSUME(x) ((void) 0)

#endif /* __MPFR_IMPL_H__ */
