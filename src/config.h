/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2023 Giles Bathgate
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_H
#define CONFIG_H

#define CGAL_NO_CORE 1
#define CGAL_DO_NOT_USE_MPZF 1

#define CGAL_DEBUG
#define CGAL_ENABLE_DISABLE_ASSERTIONS_AT_RUNTIME
#define CGAL_NO_WARNINGS
#include <CGAL/assertions.h>

#define CGAL_NDEBUG
#include <CGAL/FPU.h>

#define CGAL_DISABLE_ROUNDING_MATH_CHECK
#include <CGAL/Interval_nt.h>

#undef CGAL_NDEBUG
#include <CGAL/Chinese_remainder_traits.h>
#define CGAL_NDEBUG

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(5,3,0)
#include <CGAL/IO/Color.h>
namespace CGAL { namespace IO {
using Color = CGAL::Color;
}}
#endif

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(5,3,0)
#include "cgaltrace.h" // nef trace hack
#endif

#if CGAL_VERSION_NR < CGAL_VERSION_NUMBER(4,8,0)
#include "cgalassert.h" // cgalassert hack
#endif

#if CGAL_VERSION_NR > CGAL_VERSION_NUMBER(4,2,0)
#define USE_SIMPLIFY
#define USE_SUBDIV
#endif

#if __cplusplus > 199711 && CGAL_VERSION_NR > CGAL_VERSION_NUMBER(4,2,0)
#define USE_OFFSET
#endif

#endif // CONFIG_H
