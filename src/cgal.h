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
#ifdef USE_CGAL
#ifndef CGAL_H
#define CGAL_H

#include "config.h"

#ifndef USE_VALGRIND
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#else
#include <CGAL/Cartesian.h>
#include <CGAL/Gmpq.h>
#endif

namespace CGAL
{
#ifndef USE_VALGRIND
using Kernel3 = Exact_predicates_exact_constructions_kernel;
#else
using Kernel3 = Cartesian<Gmpq>;
#endif
using Scalar = Kernel3::FT;
using Cuboid3 = Kernel3::Iso_cuboid_3;
using Point3 = Kernel3::Point_3;
using Triangle3 =  Kernel3::Triangle_3;
using Point2 =  Kernel3::Point_2;
using Vector3 = Kernel3::Vector_3;
using Direction3 = Kernel3::Direction_3;
using Plane3 = Kernel3::Plane_3;
using Circle3 = Kernel3::Circle_3;
using AffTransformation3 = Kernel3::Aff_transformation_3;
}

#endif // CGAL_H
#endif
