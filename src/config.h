/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2020 Giles Bathgate
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

#define BOOST_NO_STDLIB_CONFIG

#define CGAL_NO_CORE 1
#include <CGAL/config.h>

#undef CGAL_USE_GMPXX
#define CGAL_DO_NOT_USE_BOOST_MP 1
#define CGAL_DO_NOT_USE_MPZF 1

#define CGAL_DEBUG
#include <CGAL/assertions.h>

#define NDEBUG
#include <CGAL/Nef_2/debug.h>
#include <CGAL/intersection_3.h> //Dependency
#include <CGAL/Nef_3/SNC_decorator.h> //Dependency
#include <CGAL/Nef_3/SNC_external_structure.h>

#undef NDEBUG
#include <CGAL/triangulation_assertions.h>

#define CGAL_DISABLE_ROUNDING_MATH_CHECK
#include <CGAL/Interval_nt.h>

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
