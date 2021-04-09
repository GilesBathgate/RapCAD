/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2021 Giles Bathgate
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
#ifndef CGALTRACE_H
#define CGALTRACE_H

#define NDEBUG
#include <CGAL/Nef_2/debug.h>

#undef CGAL_NEF_TRACE
#undef CGAL_NEF_TRACEN
#undef CGAL_NEF_TRACEV
#undef CGAL_NEF_CTRACE
#undef CGAL_NEF_CTRACEN

#define CGAL_NEF_TRACE(t) (static_cast<void>(0))
#define CGAL_NEF_TRACEN(t) (static_cast<void>(0))
#define CGAL_NEF_TRACEV(t) (static_cast<void>(0))
#define CGAL_NEF_CTRACE(b,t) (static_cast<void>(0))
#define CGAL_NEF_CTRACEN(b,t) (static_cast<void>(0))

#include <CGAL/intersection_3.h> // SNC_external_structure.h dependency
#include <CGAL/Nef_3/SNC_decorator.h> // SNC_external_structure.h dependency
#include <CGAL/Nef_3/SNC_external_structure.h>

#undef NDEBUG
#include <CGAL/triangulation_assertions.h>

#endif // CGALTRACE_H
