/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2019 Giles Bathgate
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

#include <CGAL/config.h>

#undef CGAL_USE_GMPXX

#include <qglobal.h>

#define NDEBUG
#include <CGAL/Nef_2/debug.h>
#undef NDEBUG

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
