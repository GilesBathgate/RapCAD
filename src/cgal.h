/*
 *   RapCAD - Rapid prototyping CAD IDE (www.rapcad.org)
 *   Copyright (C) 2010-2011 Giles Bathgate
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

#ifndef CGAL_H
#define CGAL_H

#include <CGAL/Gmpq.h>
#include <CGAL/Extended_cartesian.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_2.h>
#include <CGAL/Nef_polyhedron_3.h>

namespace CGAL
{
typedef CGAL::Extended_cartesian<CGAL::Gmpq> Kernel2;
typedef CGAL::Nef_polyhedron_2<Kernel2> NefPolyhedron2;
typedef CGAL::Aff_transformation_2<Kernel2> AffTransformation2;

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel3;
typedef CGAL::Polyhedron_3<Kernel3> Polyhedron3;
typedef Polyhedron3::HalfedgeDS HalfedgeDS;
typedef CGAL::Nef_polyhedron_3<Kernel3> NefPolyhedron3;
typedef Kernel3::Point_3 Point3;
typedef NefPolyhedron3::Aff_transformation_3 AffTransformation3;
}

#endif // CGAL_H
