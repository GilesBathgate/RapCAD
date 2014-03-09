/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2014 Giles Bathgate
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
#if USE_CGAL
#ifndef CGAL_H
#define CGAL_H

#include "cgalassert.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/Nef_nary_union_3.h>
#include <CGAL/bounding_box.h>

namespace CGAL
{
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel3;
typedef CGAL::Polygon_2<Kernel3> Polygon2;
typedef CGAL::Polyhedron_3<Kernel3> Polyhedron3;
typedef Polyhedron3::HalfedgeDS HalfedgeDS;
typedef CGAL::Nef_polyhedron_3<Kernel3> NefPolyhedron3;
typedef CGAL::Kernel3::FT FT;
typedef Kernel3::Iso_cuboid_3 Cuboid3;
typedef Kernel3::Point_3 Point3;
typedef Kernel3::Triangle_3 Triangle3;
typedef Kernel3::Point_2 Point2;
typedef Kernel3::Vector_3 Vector3;
typedef Kernel3::Circle_3 Circle3;
typedef NefPolyhedron3::Aff_transformation_3 AffTransformation3;
}

#endif // CGAL_H
#endif
